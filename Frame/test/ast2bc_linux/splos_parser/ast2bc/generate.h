//
// Created by nick on 2015/5/14 0014.
//

#include "bcvector.h"
#include "pcl.h"
#include "ast.h"
#include "decl.h"
#include "gen.h"
#include "stmt.h"
#include "expr.h"

#ifndef AST2BC_GENERATE_H
#define AST2BC_GENERATE_H

void binary_generate(uint8_t byte_code, BCvector* bc);

void branch_generate(uint8_t byte_code, BCvector* bc, BBlock trueBB, BBlock falseBB);

void goto_generate(BCvector* bc, BBlock block);

void function_generate(BCvector* bc, size_t num);

#endif //AST2BC_GENERATE_H
