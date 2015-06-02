/*
 * =====================================================================================
 *
 *       Filename:  read_byte_code.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年05月23日 19时11分30秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */
 
#ifndef __READ_BYTE_CODE_H__
#define __READ_BYTE_CODE_H__

 
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "byte_code.h"
#include "read_byte_code.h"

ByteCode *byte_code = NULL;

static void bc_read(int* main_begin, int* main_end, int* global_end)
{
    int fd = open("/tmp/output.sbc", O_CREAT | O_RDWR);
    size_t bc_size;
    uint8_t t8;
    uint32_t t32;
    size_t i;

    read(fd, (void*)&t32, sizeof(t32));
    *global_end = t32;
    read(fd, (void*)&t32, sizeof(t32));
    *main_begin = t32;
    read(fd, (void*)&t32, sizeof(t32));
    *main_end = t32;
    read(fd, (void*)&t32, sizeof(t32));
    bc_size = t32;

    byte_code = (ByteCode*)malloc(bc_size * sizeof(struct byte_codes));

    for (i = 0; i < bc_size; i++) {
        read(fd, (void*)&t8, sizeof(t8));
        byte_code[i].code = t8;
        read(fd, (void*)&t32, sizeof(t32));
        byte_code[i].arg1 = t32;
        read(fd, (void*)&t32, sizeof(t32));
        byte_code[i].arg2 = t32;
    }

    close(fd);
}

#endif


