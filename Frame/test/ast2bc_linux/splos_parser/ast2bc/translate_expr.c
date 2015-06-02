//
// Created by nick on 2015/5/9.
//

#include <string.h>
#include "translate_expr.h"
#include "symbol.h"
#include "expr.h"
#include "gen.h"
#include "alloc.h"
#include "pcl.h"

static Symbol (* expr_translator[])(AstExpression, BCvector*) = {
        error_translate,
        assign_translate, // =
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        binary_translate, // ||
        binary_translate, // &&
        binary_translate, // |
        binary_translate, // ^
        binary_translate, // &
        binary_translate, // ==
        binary_translate, // !=
        binary_translate, // >
        binary_translate, // <
        binary_translate, // >=
        binary_translate, // <=
        binary_translate, // <<
        binary_translate, // >>
        binary_translate, // +
        binary_translate, // -
        binary_translate, // *
        binary_translate, // /
        binary_translate, // %
        unary_translate, // cast
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        unary_translate, // !
        unary_translate, // sizeof
        array_index_translate, // []
        function_call_translate, // call
        error_translate,
        error_translate,
        error_translate,
        error_translate,
        primary_translate, // id
        primary_translate, // const
        primary_translate, // str
        error_translate
};

Symbol expression_translate(AstExpression expr, BCvector* bc)
{
    return (*expr_translator[expr->op])(expr, bc);
}

Symbol branch_expression_translate(AstExpression expr, BCvector* bc)
{

}

Symbol array_index_translate(AstExpression expr, BCvector* bc)
{
    Symbol t;
    if (expr->kids[0]->ty->bty->categ == INT)
        expr->kids[1]->val.i[0] /= 4;

    bcvector_push1(bc, IPUSH, expr->kids[1]->val.i[0]);
    bcvector_push1(bc, IPUSH, expr->kids[0]->num);
    if (expr->lvalue) {
        if (expr->kids[0]->ty->bty->categ == CHAR)
            bcvector_push(bc, CASTORE);
        else
            bcvector_push(bc, IASTORE);
    } else {
        if (expr->kids[0]->ty->bty->categ == CHAR)
            bcvector_push(bc, CALOAD);
        else
            bcvector_push(bc, IALOAD);
    }

    CALLOC(t);
    t->ty = expr->kids[0]->ty->bty;

    return t;
}

Symbol primary_translate(AstExpression expr, BCvector* bc)
{
    if (expr->op == OP_CONST) {
        if (expr->ty->categ == CHAR) {
            bcvector_push1(bc, CPUSH, expr->val.i[0]);
        } else if (expr->ty->categ == INT) {
            bcvector_push1(bc, IPUSH, expr->val.i[0]);
        }
    } else if (expr->op == OP_ID) {
        if (expr->lvalue)
            return expr->val.p;

        if (expr->ty->categ == CHAR)
            bcvector_push1(bc, CLOAD, expr->num);
        else if (expr->ty->categ == INT)
            bcvector_push1(bc, ILOAD, expr->num);
        else if (expr->ty->categ == POINTER)
            bcvector_push1(bc, ALOAD, expr->num);

        return expr->val.p;
    }
}

Symbol function_call_translate(AstExpression expr, BCvector* bc)
{
    AstExpression arg;
    Symbol faddr, recv;
    ILArg ilarg;

    expr->kids[0]->isfunc = 0;
    //faddr = expression_translate(expr->kids[0], bc);

    for (arg = expr->kids[1]; arg; arg = (AstExpression)arg->next) {
        expression_translate(arg, bc);
    }

    recv = NULL;
    if (expr->ty->categ != VOID);

    if (expr->callNum == 0)
        function_generate(bc, func_table_find(((Symbol)expr->kids[0]->val.p)->name));
    else
        bcvector_push1(bc, SYSCALL, expr->callNum);

    return recv;
}

Symbol unary_translate(AstExpression expr, BCvector* bc)
{
    Symbol src;

    if (expr->op == OP_NOT)
        return branch_expression_translate(expr, bc);

    src = expression_translate(expr->kids[0], bc);
    if (expr->op == OP_CAST)
        return cast_translate(expr, bc);
}

uint8_t OPcmap[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    CBOR, CBXOR, CBAND, IJE, IJNE, IJG, IJL, IJGE, IJLE, CSHL, CSHR, CADD, CSUB, CMUL, CDIV, CREM};
uint8_t OPimap[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    IBOR, IBXOR, IBAND, 0, 0, 0, 0, 0, 0, ISHL, ISHR, IADD, ISUB, IMUL, IDIV, IREM};

Symbol binary_translate(AstExpression expr, BCvector* bc)
{
    Symbol src1, src2;

    if (expr->op == OP_OR || expr->op == OP_AND ||
        (expr->op >= OP_EQUAL && expr->op <= OP_LESS_EQ))
        return branch_expression_translate(expr, bc);

    src2 = expression_translate(expr->kids[1], bc);
    src1 = expression_translate(expr->kids[0], bc);

    if (expr->ty->categ == CHAR)
        binary_generate(OPcmap[expr->op], bc);
    else if (expr->ty->categ == INT)
        binary_generate(OPimap[expr->op], bc);

    return NULL;
}

Symbol assign_translate(AstExpression expr, BCvector* bc)
{
    Symbol dst, src;
    Field fld = NULL;

    src = expression_translate(expr->kids[1], bc);
    dst = expression_translate(expr->kids[0], bc);

    if (expr->kids[0]->op == OP_INDEX)
        return dst;

    if (dst->ty->categ == CHAR) {
        bcvector_push1(bc, CSTORE, dst->num);
    } else if (dst->ty->categ == INT) {
        bcvector_push1(bc, ISTORE, dst->num);
    }

    return dst;
}

Symbol error_translate(AstExpression expr, BCvector* bc)
{

}

Symbol cast_translate(AstExpression expr, BCvector* bc)
{
    if (expr->ty->categ == INT && expr->kids[0]->ty->categ == CHAR)
        bcvector_push(bc, C2I);

    if (expr->ty->categ == CHAR && expr->kids[0]->ty->categ == INT)
        bcvector_push(bc, I2C);
}

AstExpression not_translate(AstExpression expr)
{
    static int rops[] = {OP_UNEQUAL, OP_EQUAL, OP_LESS_EQ, OP_GREAT_EQ, OP_LESS, OP_GREAT};
    AstExpression t;

    switch (expr->op) {

        /* !(a || b) = !a && !b */
    case OP_OR:
        expr->op = OP_AND;
        expr->kids[0] = not_translate(expr->kids[0]);
        expr->kids[1] = not_translate(expr->kids[1]);
        return expr;

        /* !(a && b) = !a || !b */
    case OP_AND:
        expr->op = OP_OR;
        expr->kids[0] = not_translate(expr->kids[0]);
        expr->kids[1] = not_translate(expr->kids[1]);
        return expr;

        /* (= => !=), (<= => >) ... */
    case OP_EQUAL:
    case OP_UNEQUAL:
    case OP_GREAT:
    case OP_LESS:
    case OP_GREAT_EQ:
    case OP_LESS_EQ:
        expr->op = rops[expr->op - OP_EQUAL];
        return expr;

    case OP_NOT:
        return expr->kids[0];

    default:
    CREATE_AST_NODE(t, Expression);
        t->coord = expr->coord;
        t->ty = T(INT);
        t->op = OP_NOT;
        t->kids[0] = expr;
        return FoldConstant(t);
    }
}


Symbol branch_translate(AstExpression expr, BBlock trueBB, BBlock falseBB, BCvector* bc)
{
    BBlock rtestBB;
    Symbol src1, src2;
    Type ty;

    switch (expr->op) {
    case OP_AND:
        rtestBB = CreateBBlock();
        branch_translate(not_translate(expr->kids[0]), falseBB, rtestBB, bc);
        bblock_backfill(rtestBB, bc->size);
        StartBBlock(rtestBB);
        branch_translate(expr->kids[1], trueBB, falseBB, bc);
        break;
    case OP_OR:
        rtestBB = CreateBBlock();
        branch_translate(expr->kids[0], trueBB, rtestBB, bc);
        bblock_backfill(rtestBB, bc->size);
        StartBBlock(rtestBB);
        branch_translate(expr->kids[1], trueBB, falseBB, bc);
        break;
    case OP_EQUAL:
    case OP_UNEQUAL:
    case OP_GREAT:
    case OP_LESS:
    case OP_GREAT_EQ:
    case OP_LESS_EQ:
        src2 = expression_translate(expr->kids[1], bc);
        src1 = expression_translate(expr->kids[0], bc);

        branch_generate(OPcmap[expr->op], bc, trueBB, falseBB);
        break;
    case OP_NOT:
        src1 = expression_translate(expr->kids[0], bc);
        ty = expr->kids[0]->ty;
        break;
    case OP_CONST:
        if (expr->val.i[0] || expr->val.i[1])
            goto_generate(bc, trueBB);
        break;
    default:

        break;
    }
}

