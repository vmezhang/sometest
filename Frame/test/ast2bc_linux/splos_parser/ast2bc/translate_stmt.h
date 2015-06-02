//
// Created by nick on 2015/5/9.
//

#ifndef AST2BC_TRANSLATE_H
#define AST2BC_TRANSLATE_H

#include "public.h"
#include "byte_code.h"
#include "bcvector.h"
#include "pcl.h"
#include "ast.h"
#include "decl.h"
#include "gen.h"
#include "stmt.h"
#include "expr.h"


void ast_translate(AstTranslationUnit ast);

void declare_translate(AstDeclaration decl, BCvector* bc);

void function_translate(AstFunction func, BCvector* bc);

void statement_translate(AstStatement stmt, BCvector* bc);

void exprstmt_translate(AstStatement stmt, BCvector* bc);

void if_translate(AstStatement stmt, BCvector* bc);

void while_translate(AstStatement stmt, BCvector* bc);

void break_translate(AstStatement stmt, BCvector* bc);

void continue_translate(AstStatement stmt, BCvector* bc);

void return_translate(AstStatement stmt, BCvector* bc);

void compound_translate(AstStatement stmt, BCvector* bc);

void parallel_translate(AstStatement stmt, BCvector* bc);


#endif //AST2BC_TRANSLATE_H
