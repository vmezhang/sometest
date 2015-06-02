//
// Created by nick on 2015/5/9.
//

#ifndef AST2BC_TRANSLATE_EXPR_H
#define AST2BC_TRANSLATE_EXPR_H

#include "public.h"
#include "pcl.h"
#include "ast.h"
#include "expr.h"
#include "gen.h"
#include "byte_code.h"
#include "generate.h"
#include "bcvector.h"

Symbol expression_translate(AstExpression expr, BCvector* bc);

Symbol branch_expression_translate (AstExpression expr, BCvector* bc);

Symbol array_index_translate(AstExpression expr, BCvector* bc);

Symbol primary_translate(AstExpression expr, BCvector* bc);

Symbol function_call_translate(AstExpression expr, BCvector* bc);

Symbol unary_translate(AstExpression expr, BCvector* bc);

Symbol binary_translate(AstExpression expr, BCvector* bc);

Symbol assign_translate(AstExpression expr, BCvector* bc);

Symbol error_translate(AstExpression expr, BCvector* bc);

Symbol cast_translate(AstExpression expr, BCvector* bc);

AstExpression not_translate(AstExpression expr);

Symbol branch_translate(AstExpression expr, BBlock trueBB, BBlock falseBB, BCvector* bc);

#endif //AST2BC_TRANSLATE_EXPR_H
