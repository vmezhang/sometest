/*
 * =====================================================================================
 *
 *       Filename:  sched_FIFO.c
 *
 *    Description:  先来先服务调度算法的实现
 *
 *        Version:  1.0
 *        Created:  2015年05月13日 10时03分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */
 
#include <assert.h>
#include <stdio.h>

#include "defs.h"
#include "list.h"
#include "proc.h"
#include "sched_FIFO.h"

//初始化运行队列
static void FCFS_init(struct runQueue *rq) 
{
    list_init(&(rq->run_list));
    rq->proc_num = 0;
}

//加入运行队列
static void FCFS_enqueue(struct runQueue *rq, struct ProcStruct *proc) 
{
    printf("FCFS_enqueue!\n");
  //  assert(list_empty(&(proc->runLink)));
    list_add_before(&(rq->run_list), &(proc->runLink));
    proc->rq = rq;
    rq->proc_num++;
}

//从对列中删除
static void FCFS_dequeue(struct runQueue *rq, struct ProcStruct *proc) 
{
    printf("FCFS_dequeue!\n");
    assert(!list_empty(&(proc->runLink)) && proc->rq == rq);
    list_del_init(&(proc->runLink));
    rq->proc_num--;
}

//从对列中选择一个进程运行
static struct ProcStruct *FCFS_pick_next(struct runQueue *rq) 
{
    printf("FCFS_pick_next!\n");	
    struct ProcStruct *temp;
    list_entry_t *le = list_next(&(rq->run_list));
    if (le != &(rq->run_list))
    {
        temp = le2proc(le, runLink);
	temp->runs = 0;
	return temp;
    }
    return NULL;
}

static void FCFS_proc_tick(struct runQueue *rq, struct ProcStruct *proc) 
{
    if (proc->timeSlice > 0) 
    {
        proc->timeSlice --;
    }
    if (proc->timeSlice == 0) 
    {
        proc->needResched = 1;
    }
}

struct schedClass default_sched_class = {
    .name = "FCFS_scheduler",
    .init = FCFS_init,
    .enqueue = FCFS_enqueue,
    .dequeue = FCFS_dequeue,
    .pickNext = FCFS_pick_next,
    .procTick = FCFS_proc_tick,
};

