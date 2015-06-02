// =====================================================================================
//
//       Filename:  Syscalls.h
//
//    Description:  系统调用相关
//
// =====================================================================================

#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include <stdint.h>

namespace ospf {

namespace syscall {

// system table(系统调用表宏)
// 文件系统相关系统调用
#define SYS_EXIT    0x0

#define SYS_OPEN    0x1
#define SYS_CLOSE   0x2
#define SYS_READ    0x3
#define SYS_WRITE   0x4

// 进程调度相关系统调用
#define SYS_PFORK   0x5
#define SYS_WAIT    0xB
#define SYS_KILL    0x6

// 内存管理相关系统调用
#define SYS_MALLOC  0x7
#define SYS_FREE    0x8

#define SYS_CREAT   0x9
#define SYS_LSEEK   0xA
// 系统调用个数
#define MAX_SYSCALL_NUM  0xB

bool syscall(uint32_t sysno, uint32_t *args);

int32_t get_syscall_nr_args(uint32_t sysno);

}

}

#endif  // SYSCALLS_H_
