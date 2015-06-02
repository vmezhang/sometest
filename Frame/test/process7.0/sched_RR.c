/*
 * =====================================================================================
 *
 *       Filename:  sched_RR.c
 *
 *    Description:  时间片轮转算法的实现
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

#include "types.h"
#include "list.h"
#include "proc.h"
#include "sched_RR.h"

//初始化运行队列
static void RR_init(struct runQueue *rq) 
{
    list_init(&(rq->run_list));
    rq->proc_num = 0;
}

//进程加入运行队列
static void RR_enqueue(struct runQueue *rq, struct ProcStruct *proc) 
{
    assert(list_empty(&(proc->runLink)));
    list_add_before(&(rq->run_list), &(proc->runLink));
    proc->rq = rq;
    rq->proc_num++;
}

//进程从就绪队列中删除
static void RR_dequeue(struct runQueue *rq, struct ProcStruct *proc) 
{
    assert(!list_empty(&(proc->runLink)) && proc->rq == rq);
    list_del_init(&(proc->runLink));
    rq->proc_num--;
}

//在就绪队列中挑选下一个可以运行的进程
static struct ProcStruct *RR_pick_next(struct runQueue *rq) 
{
    int time_slice = 3;
    struct ProcStruct *temp;

    list_entry_t *le = list_next(&(rq->run_list));
    if (le != &(rq->run_list)) 
    {
        temp =  le2proc(le, runLink);
	if (time_slice >= temp->runs)
	{
		printf("procName = %s, pid = %d, run time is : temp->runs = %d, cpu is time_slice = %d, Run Finished!\n", 
				    temp->nameProc, temp->pid, temp->runs, time_slice);
	}
	else
	{
		temp->runs = temp->runs - time_slice;
		printf("procName = %s, pid = %d, run time is : temp->runs = %d, cpu is time_slice = %d, Not Run Finished!\n", 
				    temp->nameProc, temp->pid, temp->runs, time_slice);
		printf("Add process to Runnable Queue after!\n");
	}
	return temp;
    }
    return NULL;
}

//挑选定时器列表
static void RR_proc_tick(struct runQueue *rq, struct ProcStruct *proc) 
{
    if (proc->timeSlice > 0) 
    {
        proc->timeSlice --;
    }
    if (proc->timeSlice == 0) {
        proc->needResched = 1;
    }
}

struct schedClass RR_sched_class = {
    .name = "RR_scheduler",
    .init = RR_init,
    .enqueue = RR_enqueue,
    .dequeue = RR_dequeue,
    .pickNext = RR_pick_next,
    .procTick = RR_proc_tick,
};

