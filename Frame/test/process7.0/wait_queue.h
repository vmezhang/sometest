/*
 * =====================================================================================
 *
 *       Filename:  wait_queue.h
 *
 *    Description:  等待队列相关函数定义
 *
 *        Version:  1.0
 *        Created:  2015年05月21日 13时08分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef _WAIT_QUEUEU_H_
#define _WAIT_QUEUEU_H_

#include <assert.h>
#include <stdio.h>

#include "defs.h"
#include "list.h"
#include "proc.h"
#include "sched.h"

void waitQueueInit(struct waitQueue *wq);
void addWaitQueue(struct waitQueue *wq, struct ProcStruct *proc);
void delWaitQueue(struct waitQueue *wq, struct ProcStruct *proc);
struct ProcStruct *waitQueuePickNext(struct waitQueue *wq); 

#endif

