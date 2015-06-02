/*
 * =====================================================================================
 *
 *       Filename:  sched.cpp
 *
 *    Description:  进程调度器接口的封装
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

#include "list.h"
#include "proc.h"
#include "sched.h"
#include "cpu_init.h"
#include "sched_FIFO.h"

static list_entry_t timer_list;

static struct schedClass *sched_class;

//static struct runQueue *rq;     //运行队列

//static struct waitQueue *wq;   //等待队列

void schedClassEnqueue(struct ProcStruct *proc) 
{
    if (proc != idleproc) 
    {
        sched_class->enqueue(rq, proc);
    }
}

void schedClassDequeue(struct ProcStruct *proc) 
{
    sched_class->dequeue(rq, proc);
}

struct ProcStruct *schedClassPickNext() 
{
    return sched_class->pickNext(rq);
}

void schedClassProcTick(struct ProcStruct *proc)
{
    if (proc != idleproc) 
    {
        sched_class->procTick(rq, proc);
    }
    else {
        proc->needResched = 1;
    }
}

static struct runQueue __rq;

//初始化调度器
void schedInit() 
{
	sched_class = &default_sched_class;
	 rq = &__rq;
	printf("sched class: %s\n", sched_class->name);
	sched_class->init(rq);
}

//完成把一个就绪进程放入到就绪队列中工作
void wakeupProc(struct ProcStruct *proc) 
{
    assert(proc->state != PROC_ZOMBLE);
    if (proc->state != PROC_RUNNABLE) 
    {
	    proc->state = PROC_RUNNABLE;
            proc->wait_state = 0;
            if (proc != current) 
	    {
                schedClassEnqueue(proc);
            }
    }
    else
    {
	    printf("wakeup runnable process.\n");
    }
}

//进程调度
void schedule() 
{
    struct ProcStruct *next;
    current->needResched = 0;
    if ((next = schedClassPickNext()) != NULL)
    {
            schedClassDequeue(next);
    }
    if (next == NULL) 
    {
	    next = idleproc;
    }
    printf("byteCodeStartAdress = %d, byteCodeEndAdress = %d\n", next->byteCodeStartAdress, next->byteCodeEndAdress);
   //进程运行，对接cpu执行
   add_task(next);
}

//将创建的进程添加到就绪队列
void addRunQueue()
{
	if (current->state == PROC_RUNNABLE)
	{
		schedClassEnqueue(current);
	}
}

//添加定时器
void add_timer(ltimer_t *timer)
 {
	assert(timer->expires > 0 && timer->proc != NULL);
        assert(list_empty(&(timer->timer_link)));
        list_entry_t *le = list_next(&timer_list);
        while (le != &timer_list) 
        {
            ltimer_t *next = le2timer(le, timer_link);
            if (timer->expires < next->expires)
             {
                next->expires -= timer->expires;
                break;
            }
            timer->expires -= next->expires;
            le = list_next(le);
        }
        list_add_before(le, &(timer->timer_link));
}

//删除定时器
void del_timer(ltimer_t *timer)
{
        if (!list_empty(&(timer->timer_link))) 
	{
            if (timer->expires != 0) 
	    {
                list_entry_t *le = list_next(&(timer->timer_link));
                if (le != &timer_list)
		{
                    ltimer_t *next = le2timer(le, timer_link);
                    next->expires += timer->expires;
                }
            }
            list_del_init(&(timer->timer_link));
        }
}

//定时器列表
void run_timer_list(void) 
{
        list_entry_t *le = list_next(&timer_list);
        if (le != &timer_list) 
	{
            ltimer_t *timer = le2timer(le, timer_link);
            assert(timer->expires != 0);
            timer->expires --;
            while (timer->expires == 0) 
	    {
                le = list_next(le);
                struct ProcStruct *proc = timer->proc;
                if (proc->wait_state != 0) 
		{
                    assert(proc->wait_state & WT_INTERRUPTED);
                }
                else 
		{
                    printf("process %d's wait_state == 0.\n", proc->pid);
                }
                wakeupProc(proc);
                del_timer(timer);
                if (le == &timer_list) {
                    break;
                }
                timer = le2timer(le, timer_link);
            }
        }
        schedClassProcTick(current);
}
