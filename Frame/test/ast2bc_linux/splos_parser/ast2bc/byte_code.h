//
// Created by nick on 2015/5/9.
//

#ifndef AST2BC_BYTE_CODE_H
#define AST2BC_BYTE_CODE_H

#include "public.h"
#include <stdint.h>
#include <stdlib.h>

struct byte_code {
    uint8_t    code;
    size_t     num;
    uint32_t   arg1;
    uint32_t   arg2;
};

#endif //AST2BC_BYTE_CODE_H
