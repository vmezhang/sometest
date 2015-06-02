//
// Created by nick on 2015/5/14 0014.
//

#include "bcvector.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void bcvector_push(BCvector* bc, uint8_t byte_code)
{
    bc->data[bc->size].code = byte_code;
    bc->data[bc->size].arg1 = 0;
    bc->data[bc->size].arg2 = 0;
    bc->data[bc->size].num = 0;
    bc->size++;
}

void bcvector_push1(BCvector* bc, uint8_t byte_code, uint32_t arg)
{
    bc->data[bc->size].code = byte_code;
    bc->data[bc->size].num = 1;
    bc->data[bc->size].arg1 = arg;
    bc->data[bc->size].arg2 = 0;
    bc->size++;
}

void bcvector_push2(BCvector* bc, uint8_t byte_code, uint32_t arg1, uint32_t arg2)
{
    bc->data[bc->size].code = byte_code;
    bc->data[bc->size].num = 2;
    bc->data[bc->size].arg1 = arg1;
    bc->data[bc->size].arg2 = arg2;
    bc->size++;
}

void bcvector_print(BCvector* bc)
{
    size_t i;
    for (i = 0; i < bc->size; i++) {
        printf("%2d: ", i);

        switch(bc->data[i].code) {
        case IPUSH:
            printf("IPUSH");
            break;
        case CPUSH:
            printf("CPUSH");
            break;
        case ILOAD:
            printf("ILOAD");
            break;
        case CLOAD:
            printf("CLOAD");
            break;
        case ISTORE:
            printf("ISTORE");
            break;
        case CSTORE:
            printf("CSTORE");
            break;
        case IADD:
            printf("IADD");
            break;
        case ISUB:
            printf("ISBU");
            break;
        case IMUL:
            printf("IMUL");
            break;
        case IDIV:
            printf("IDIV");
            break;
        case IJE:
            printf("IJE");
            break;
        case IJNE:
            printf("IJNE");
            break;
        case IJG:
            printf("IJG");
            break;
        case IJL:
            printf("IJL");
            break;
        case IJGE:
            printf("IJGE");
            break;
        case IJLE:
            printf("IJLE");
            break;
        case GOTO:
            printf("GOTO");
            break;
            break;
        case IBOR:
            printf("IBOR");
            break;
        case ISHL:
            printf("ISHL");
            break;
        case ISHR:
            printf("ISHR");
            break;
        case PARA:
            printf("PARA");
            break;
        case FUNCCALL:
            printf("CALL");
            break;
        case CALOAD:
            printf("CALOAD");
            break;
        case IALOAD:
            printf("IALOAD");
            break;
        case CASTORE:
            printf("CASTORE");
            break;
        case IASTORE:
            printf("IASTORE");
            break;
        case ALOAD:
            printf("ALOAD");
            break;
        case ASTORE:
            printf("ASTORE");
            break;
        default:
            printf("%d", bc->data[i].code);
            break;
        }

        if (bc->data[i].num == 0)
            printf("\n");
        else if (bc->data[i].num == 1)
            printf(" %d\n", bc->data[i].arg1);
        else if (bc->data[i].num == 2)
            printf(" %d %d\n", bc->data[i].arg1, bc->data[i].arg2);
    }
}

void func_table_push(char* name)
{
    strcpy(ft.table[ft.size], name);
    ft.size++;
}

size_t func_table_find(char* name)
{
    size_t i = 0;
    for (; i < ft.size; i++) {
        if (strcmp(ft.table[i], name) == 0)
            return i;
    }
    return 0xffffffff;
}

void func_table_pos(size_t num, size_t pos)
{
    ft.pos[num] = pos;
}

void bcstack_push(uint8_t code, uint32_t num)
{
    bcs.data[bcs.size] = code;
    bcs.num[bcs.size] = num;
    bcs.size++;
}

void bcstack_clear(BCvector* bc)
{
    int i;
    for (i = bcs.size - 1; i >= 0; i--)
        bcvector_push1(bc, bcs.data[i], bcs.num[i]);

    bcs.size = 0;
}

void func_tran(BCvector* bc)
{
    size_t i;

    ft.pos[ft.size] = bc->size;

    for (i = 0; i < bc->size; i++) {
        if (bc->data[i].code == FUNCCALL) {
            bc->data[i].arg2 = ft.pos[bc->data[i].arg1+1];
            bc->data[i].arg1 = ft.pos[bc->data[i].arg1];
        }
    }


}

void func_find_main()
{
    size_t i;

    i = func_table_find("main");
    md.main_begin = ft.pos[i];
    md.main_end = ft.pos[i+1];
}

void all_dump(BCvector* bc)
{
    int fd = open("/tmp/output.sbc", O_CREAT | O_RDWR, 0777);
    size_t i;

    write(fd, &md.global_end, sizeof(md.global_end));
    write(fd, &md.main_begin, sizeof(md.main_begin));
    write(fd, &md.main_end, sizeof(md.main_end));
    write(fd, &bc->size, sizeof(bc->size));
    for (i = 0; i < bc->size; i++) {
        write(fd, &bc->data[i].code, sizeof(bc->data[i].code));
        write(fd, &bc->data[i].arg1, sizeof(bc->data[i].arg1));
        write(fd, &bc->data[i].arg2, sizeof(bc->data[i].arg2));
    }

    close(fd);
}
