//
// Created by nick on 2015/5/14 0014.
//

#include "generate.h"

void binary_generate(uint8_t byte_code, BCvector* bc)
{
    bcvector_push(bc, byte_code);

    /*
    if (byte_code == IADD)
        printf("IADD\n");
    else if (byte_code == CADD)
        printf("CADD\n");
    else if (byte_code == ISUB)
        printf("ISUB\n");
    else if (byte_code == IMUL)
        printf("IMUL\n");
    else if (byte_code == IDIV)
        printf("IDIV\n");
    else if (byte_code == IREM)
        printf("IREM\n");
    else if (byte_code == ISHL)
        printf("ISHL\n");
    else if (byte_code == ISHR)
        printf("ISHR\n");
    else if (byte_code == IBAND)
        printf("IBAND\n");
    else if (byte_code == IBOR)
        printf("IBOR\n");
    else if (byte_code == IBXOR)
        printf("IBXOR\n");
    else if (byte_code == JE)
        printf("JE\n");
    else if (byte_code == JNE)
        printf("JNE\n");
    else if (byte_code == JG)
        printf("JL\n");
    else if (byte_code == JL)
        printf("JL\n");
    else if (byte_code == JGE)
        printf("JGE\n");
    else if (byte_code == JLE)
        printf("JLE\n");
*/
}

void branch_generate(uint8_t byte_code, BCvector* bc, BBlock trueBB, BBlock falseBB)
{
    if (trueBB->pos)
        bcvector_push1(bc, byte_code, trueBB->pos);
    else {
        bcvector_push1(bc, byte_code, 0);
        bblock_push(trueBB, (size_t*)&bc->data[bc->size-1].arg1);
    }

    goto_generate(bc, falseBB);
}

void goto_generate(BCvector* bc, BBlock block)
{
    if (block->pos) {
        bcvector_push1(bc, GOTO, block->pos);
    } else {
        bcvector_push1(bc, GOTO, 0);
        bblock_push(block, (size_t*)&bc->data[bc->size-1].arg1);
    }
}

void function_generate(BCvector* bc, size_t num)
{
    bcvector_push1(bc, FUNCCALL, num);
}