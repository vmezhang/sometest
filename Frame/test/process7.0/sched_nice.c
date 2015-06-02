/*
 * =====================================================================================
 *
 *       Filename:  sched_nice.c
 *
 *    Description:  高优权优先调度算法的实现
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

#include <stdio.h>
#include <assert.h>

#include "types.h"
#include "list.h"
#include "proc.h"
#include "assert.h"
#include "sched_nice.h"

static void nice_init(struct runQueue *rq) 
{
	list_init(&(rq->run_list));
	rq->proc_num = 0;
}


static void nice_enqueue(struct runQueue *rq, struct ProcStruct *proc) 
{
	printf("nice_enqueue!\n\n");
	assert(list_empty(&(proc->runLink)));
	list_add_after(&(rq->run_list), &(proc->runLink));
	proc->rq = rq;
	rq->proc_num++;
}

static void nice_dequeue(struct runQueue *rq, struct ProcStruct *proc) 
{
	printf("nice_dequeue!\n");
	assert(!list_empty(&(proc->runLink)) && proc->rq == rq);
	list_del_init(&(proc->runLink));
	rq->proc_num--;
}

static struct ProcStruct *nice_pick_next(struct runQueue *rq) 
{
	printf("nice_pick_next!\n");	
	list_entry_t *pos, *temp;
	int max;
	struct ProcStruct *findProc, *findProc1;
	temp = &(rq->run_list);

	pos = temp->next;
	findProc = list_entry(pos, struct ProcStruct, runLink);
	max = findProc->nice;
	if (pos == temp)
	{
		return NULL;
	}

	list_for_each(pos, temp)
	{
		findProc1 = list_entry(pos, struct ProcStruct, runLink);
		if (findProc1->nice > max)
		{
			max = findProc1->nice;
			findProc = findProc1;
		}
	}
	return findProc;
}

static void nice_proc_tick(struct runQueue *rq, struct ProcStruct *proc) 
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

struct schedClass nice_sched_class = {
	 .name = "nice_scheduler",
	 .init = nice_init,
	 .enqueue = nice_enqueue,
	 .dequeue = nice_dequeue,
	 .pickNext = nice_pick_next,
	 .procTick = nice_proc_tick,
};

