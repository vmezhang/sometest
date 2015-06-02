//
// Created by nick on 2015/5/9.
//

#include "translate_stmt.h"
#include "translate_expr.h"
#include "stmt.h"

void ast_translate(AstTranslationUnit ast)
{
    BCvector bc;
    AstNode p;
    int i;

    bc.size = 0;
    for (p = ast->extDecls; p; p = p->next)
        if (p->kind == NK_Function && ((AstFunction)p)->stmt)
            func_table_push(((AstFunction)p)->fsym->name);

    for (p = ast->extDecls, i = 0; p; p = p->next, i++)
        if (p->kind == NK_Declaration)
            declare_translate((AstDeclaration)p, &bc);

    md.global_end = bc.size;

    for (p = ast->extDecls, i = 0; p; p = p->next)
        if (p->kind == NK_Function && ((AstFunction)p)->stmt) {
            func_table_pos(i, bc.size);
            function_translate((AstFunction)p, &bc);
            i++;
        }

    func_tran(&bc);

    func_find_main();

    bcvector_print(&bc);

    all_dump(&bc);
}

void declare_translate(AstDeclaration decl, BCvector* bc)
{
    Symbol t;
    AstInitDeclarator initDec = (AstInitDeclarator)decl->initDecs;
    for ( ; initDec; initDec = (AstInitDeclarator)initDec->next) {
        if (initDec->dec->id == NULL)
            continue;

        t = LookupID(initDec->dec->id);

        expression_translate(AsVar(t)->idata->expr, bc);
        if (t->ty->categ == CHAR)
            bcvector_push1(bc, CSTORE, t->num);
        else if (t->ty->categ == INT)
            bcvector_push1(bc, ISTORE, t->num);
        else
            ;

        //printf("global: %s\n", initDec->dec->id);
    }
}

void function_translate(AstFunction func, BCvector* bc)
{
    BBlock bb;
    Symbol s;

    FSYM = func->fsym;
    if (!FSYM->defined)
        return;

    TempNum = 0;
    /* 创建函数中间代码的开始结束 */
    FSYM->entryBB   = CreateBBlock ();
    FSYM->exitBB    = CreateBBlock ();

    for (s = func->fsym->params; s; s = s->next) {
        if (s->ty->categ == CHAR)
            bcstack_push(CSTORE, s->num);
        else if (s->ty->categ == INT)
            bcstack_push(ISTORE, s->num);
        else if (s->ty->categ == POINTER)
            bcstack_push(ASTORE, s->num);
    }
    bcstack_clear(bc);

    /* 当前中间代码的入口点 */
    CurrentBB = FSYM->entryBB;



    statement_translate(func->stmt, bc);

    StartBBlock (FSYM->exitBB);
}

static void (*stmt_translator[])(AstStatement, BCvector* bc) = {
        exprstmt_translate,
        NULL,
        NULL,
        NULL,
        if_translate,
        NULL,
        while_translate,
        NULL,
        NULL,
        NULL,
        break_translate,
        continue_translate,
        return_translate,
        compound_translate,
        parallel_translate,
};

void statement_translate(AstStatement stmt, BCvector* bc)
{
    (*stmt_translator[stmt->kind - NK_ExpressionStatement])(stmt, bc);
}

void exprstmt_translate(AstStatement stmt, BCvector* bc)
{
    AstExpressionStatement exprStmt = AsExpr(stmt);

    if (exprStmt->expr != NULL) {
        expression_translate(exprStmt->expr, bc);
    }
}

void if_translate(AstStatement stmt, BCvector* bc)
{
    AstIfStatement if_stmt = AsIf(stmt);
    BBlock nextBB;
    BBlock trueBB;
    BBlock falseBB;

    nextBB = CreateBBlock ();
    trueBB = CreateBBlock ();

    if (if_stmt->elseStmt == NULL) {
        branch_translate(not_translate(if_stmt->expr), nextBB, trueBB, bc);
        bblock_backfill(trueBB, bc->size);
        StartBBlock(trueBB);
        statement_translate(if_stmt->thenStmt, bc);
    } else {
        falseBB = CreateBBlock();

        branch_translate(not_translate(if_stmt->expr), falseBB, trueBB, bc);
        //branch_translate(if_stmt->expr, trueBB, falseBB, bc);
        bblock_backfill(trueBB, bc->size);
        StartBBlock(trueBB);
        statement_translate(if_stmt->thenStmt, bc);

        goto_generate(bc, nextBB);

        bblock_backfill(falseBB, bc->size);
        StartBBlock(falseBB);
        statement_translate(if_stmt->elseStmt, bc);

        goto_generate(bc, nextBB);
    }

    bblock_backfill(nextBB, bc->size);
    StartBBlock(nextBB);
}

void while_translate(AstStatement stmt, BCvector* bc)
{
    AstLoopStatement whileStmt = AsLoop(stmt);

    /* 循环体基本块 */
    whileStmt->loopBB = CreateBBlock ();
    /* 继续判断条件 */
    whileStmt->contBB = CreateBBlock ();
    /* 循环结束后基本块 */
    whileStmt->nextBB = CreateBBlock ();

    /* 条件 */
    goto_generate(bc, whileStmt->contBB);

    /* 循环体 */
    bblock_backfill(whileStmt->loopBB, bc->size);
    StartBBlock (whileStmt->loopBB);
    statement_translate (whileStmt->stmt, bc);

    /* 判断条件,分支 */
    bblock_backfill(whileStmt->contBB, bc->size);
    StartBBlock (whileStmt->contBB);
    branch_translate (whileStmt->expr, whileStmt->loopBB, whileStmt->nextBB, bc);

    bblock_backfill(whileStmt->nextBB, bc->size);
    StartBBlock (whileStmt->nextBB);
}

void break_translate(AstStatement stmt, BCvector* bc)
{
    AstBreakStatement brkStmt = AsBreak(stmt);

        /* loop 跳转 */
    goto_generate (bc, AsLoop(brkStmt->target)->nextBB);

    StartBBlock (CreateBBlock ());
}

void continue_translate(AstStatement stmt, BCvector* bc)
{
    AstContinueStatement contStmt = AsCont(stmt);

    goto_generate (bc, contStmt->target->contBB);
    StartBBlock (CreateBBlock ());
}

void return_translate(AstStatement stmt, BCvector* bc)
{
    AstReturnStatement retStmt = AsRet(stmt);

    if (retStmt->expr) {
        expression_translate(retStmt->expr, bc);
        if (retStmt->expr->ty->categ == CHAR)
            bcvector_push(bc, CRET);
        else
            bcvector_push(bc, IRET);
    } else
        bcvector_push(bc, VRET);
}

void compound_translate(AstStatement stmt, BCvector* bc)
{
    AstCompoundStatement compStmt = AsComp (stmt);
    AstNode p;
    Vector  ilocals = compStmt->ilocals;
    Symbol  v;
    int     i;

    for (i = 0; i < LEN (ilocals); ++i) {
        InitData    initd;
        Type        ty;
        Symbol      dst, src;
        int         size;

        v = GET_ITEM (ilocals, i);
        initd   = AsVar(v)->idata;
        size    = 0;

        ty = initd->expr->ty;

        dst = CreateOffset (ty, v, initd->offset);

        if (dst->ty->categ != ARRAY)
        while (initd != NULL) {

            ty = initd->expr->ty;

            dst = CreateOffset (ty, v, initd->offset);
//            if (initd->expr->op == OP_STR) {
//
//                String str = initd->expr->val.p;
//                //src = AddString (ArrayOf (str->len + 1, ty->bty), str);
//            } else {

            if (dst->ty->categ != ARRAY)
                src = expression_translate (initd->expr, bc);
//            }

            /* 赋值 */
            //GenerateMove (ty, dst, src);

            if (dst->ty->categ == CHAR)
                bcvector_push1(bc, CSTORE, dst->num);
            else if (dst->ty->categ == INT)
                bcvector_push1(bc, ISTORE, dst->num);
//            else if (dst->ty->categ == ARRAY) {
//                bcvector_push1(bc, IPUSH, dst->ty->size);
//                if (dst->ty->bty->categ == CHAR)
//                    bcvector_push1(bc, CNEWARRAY, dst->num);
//                else if (dst->ty->bty->categ == INT)
//                    bcvector_push1(bc, INEWARRAY, dst->num);
//            }

            size  = initd->offset + ty->size;
            initd = initd->next;
        }

        else {
            int j = 0;
            bcvector_push1(bc, IPUSH, dst->ty->size);
            if (dst->ty->bty->categ == CHAR)
                bcvector_push1(bc, CNEWARRAY, dst->num);
            else if (dst->ty->bty->categ == INT)
                bcvector_push1(bc, INEWARRAY, dst->num);
            while (initd != NULL) {

                ty = initd->expr->ty;

                src = expression_translate (initd->expr, bc);


                bcvector_push1(bc, IPUSH, j);
                bcvector_push1(bc, IPUSH, dst->num);

                    if (dst->ty->bty->categ == CHAR)
                        bcvector_push(bc, CASTORE);
                    else if (dst->ty->bty->categ == INT)
                        bcvector_push(bc, IASTORE);

                size  = initd->offset + ty->size;
                initd = initd->next;
                j++;
            }
        }
    }

    for (p = compStmt->stmts; p; p = p->next) {
        statement_translate((AstStatement)p, bc);
    }
}

void parallel_translate(AstStatement stmt, BCvector* bc)
{
    AstParallelStatement paraStmt = AsParall(stmt);
    AstCompoundStatement compStmt = AsComp (paraStmt->stmt);
    AstNode p;
    size_t i = 0;

    for (p = compStmt->stmts; p; p = p->next)
        ++i;

    bcvector_push1(bc, PARA, i);

    statement_translate(paraStmt->stmt, bc);
}
