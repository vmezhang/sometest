// =====================================================================================
//
//       Filename:  cpu_init.h
//
//    Description:  cpu初始化相关函数的定义
//
//        Version:  1.0
//        Created:  2014年12月30日 14时38分52秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Qianyi.lh (liuhuan), qianyi.lh@alibaba-inc.com
//        Company:  Alibaba-Inc Aliyun
//
// =====================================================================================

#ifndef __CPU_INIT_H__
#define __CPU_INIT_H__

#define max_thread_num 2
#define bytecode_num 0xff
#define CPUSTATE 2

#include <stdlib.h>
#include <pthread.h>

#include "Simulate.h"
#include "byte_code.h"
#include "proc.h"
      
//static ByteCode byte_code[bytecode_num]={{1,100,1},{0x02,0,2},{0x03,0,3},{0x01,1,5},{0x04,0,4},{0x01,1,7},{0x01,0,8},{14,1,9},{35,1,1},{1,100,1}};
      
extern HeapArea heap;   //初始化堆
static LinkVMstack _VMstack[max_thread_num];
static pthread_t *thread_link;
static pthread_cond_t  ready_task;
static pthread_mutex_t task_lock;
static int task_num=0;
static int cpu_state=CPUSTATE;
static ByteCode code[bytecode_num];

int add_task(struct ProcStruct *proc);
int cpu_init();

#endif
