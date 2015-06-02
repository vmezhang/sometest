/*************************************************************************
	> File Name: syscall.h

	> Author: zhanglp

	> Mail: zhanglp92@gmail.com

	> Created Time: 2015年01月05日 星期一 17时04分42秒

	> Description: 系统调用
 ************************************************************************/

#ifndef     SYSCALL
#error "You must define SYSCALL macro before include this file"
#endif


/* SYSCALL (系统调用的类别, 系统调用的名称, 名称长度, 系统调用号) */
/* 系统调用0 号表示普通调用 */


SYSCALL (SYS_OPEN,      "open",   4,  0x1)
SYSCALL (SYS_CLOSE,     "close",  5,  0x2)
SYSCALL (SYS_READ,      "read",   4,  0x3)
SYSCALL (SYS_WRITE,     "write",  5,  0x4)
SYSCALL (SYS_PFORK,     "pfork",  5,  0x5)
SYSCALL (SYS_WAIT,      "wait",   4,  0xB)
SYSCALL (SYS_KILL,      "kill",   4,  0x6)
SYSCALL (SYS_MALLOC,    "malloc", 6,  0x7)
SYSCALL (SYS_FREE,      "free",   4,  0x8)
SYSCALL (SYS_CREAT,     "creat",  5,  0x9)
SYSCALL (SYS_LSEEK,     "lseek",  5,  0xA)
SYSCALL (SYS_EXIT,      "exit",   4,  0xC) 

SYSCALL (SYS_PRINT,     "print",  5,  0xD)
