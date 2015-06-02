//
// Created by nick on 2015/5/14 0014.
//

#ifndef AST2BC_BCVECTOR_H
#define AST2BC_BCVECTOR_H

#include "byte_code.h"

typedef struct bcvector
{
    struct byte_code data[1000];
    size_t size;
} BCvector;

void bcvector_push(BCvector* bc, uint8_t);

void bcvector_push1(BCvector* bc, uint8_t byte_code, uint32_t arg);

void bcvector_push2(BCvector* bc, uint8_t byte_code, uint32_t arg1, uint32_t arg2);

void bcvector_print(BCvector* bc);

struct func_table
{
    char table[100][100];
    size_t pos[100];
    size_t size;
} ft;

void func_table_push(char* name);

size_t func_table_find(char* name);

void func_table_pos(size_t num, size_t pos);

struct bcstack
{
    uint8_t data[100];
    uint32_t num[100];
    size_t size;
} bcs;

void bcstack_push(uint8_t code, uint32_t num);

void bcstack_clear(BCvector* bc);

void func_tran(BCvector* bc);

void func_find_main();

struct meta_data {
    size_t global_end;
    size_t main_begin;
    size_t main_end;
} md;

void all_dump(BCvector* bc);

#endif //AST2BC_BCVECTOR_H
