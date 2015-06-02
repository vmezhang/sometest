//
// Created by nick on 2015/5/9.
//

#ifndef AST2BC_PUBLIC_H
#define AST2BC_PUBLIC_H

//#include <cstdint>

enum BYTECODE {
    IPUSH = 1,
    ISTORE = 2,
    ILOAD = 3,
    INEWARRAY = 4,
    IADD = 5,
    GOTO = 6,
    IJLE = 7,
    IMUL = 8,
    VRET = 9,
    SYSCALL = 10,
    ALOAD = 11,
    CALOAD = 12,
    CASTORE = 13,
    CNEWARRAY = 14,
    ASTORE = 15,
    I2C = 16,
    IALOAD = 17,
    IASTORE = 18,
    IDIV = 19,
    IJE = 20,
    IJNE = 21,
    IJL = 22,
    IJGE = 23,
    IJG = 24,
    IBXOR = 25,
    IREM = 26,
    IRET = 27,
    ISHL = 28,
    ISHR = 29,
    ISUB = 30,
    POP = 31,
    DUP = 32,
    SWAP = 33,
    FUNCCALL = 34,
    PARA = 35,


    C2I,
    CPUSH,
    CLOAD,
    CSTORE,
    CADD,
    CSUB,
    CMUL,
    CDIV,
    CREM = 20,
    CSHL,
    CSHR,
    IBAND,
    CBAND,
    IBOR,
    CBOR,
    CBXOR,
    CRET,
};

#endif //AST2BC_PUBLIC_H
