/*
 * =====================================================================================
 *
 *       Filename:  sched.h
 *
 *    Description:  定义进程调度器框架
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

#ifndef __SCHED_H__
#define __SCHED_H__

#include "defs.h"
#include "list.h"

#define MAX_TIME_SLICE 20

struct ProcStruct;

struct runQueue;

typedef struct 
{
    unsigned int expires;       //the expire time
    struct ProcStruct *proc;    //the proc wait in this timer. If the expire time is end, then this proc will be scheduled
    list_entry_t timer_link;    //the timer list
} ltimer_t;

#define le2timer(le, member)            \
	to_struct((le), ltimer_t, member)

// 初始化一个定时器
static inline ltimer_t *timer_init(ltimer_t *timer, struct ProcStruct *proc, int expires) 
{
    timer->expires = expires;
    timer->proc = proc;
    list_init(&(timer->timer_link));
    return timer;
}

//定义进程调度器类
struct schedClass 
{
    // 调度器的名字
    const char *name;
    // 初始化运行队列
    void (*init)(struct runQueue *rq);
    // 将进程proc插入队列rq
    void (*enqueue)(struct runQueue *rq, struct ProcStruct *proc);
    // 将进程proc从队列rq中删除
    void (*dequeue)(struct runQueue *rq, struct ProcStruct *proc);
    // 返回运行队列中下一个执行的进程
    struct ProcStruct *(*pickNext)(struct runQueue *rq);
    // timetick处理函数
    void (*procTick)(struct runQueue *rq, struct ProcStruct *proc);
};

//定义运行队列
struct runQueue 
{
    list_entry_t run_list;              //运行队列的哨兵
    unsigned int proc_num;              //内部的进程总数

};

extern struct runQueue *rq;             //将rq定义为一个外部变量

//定义等待队列
struct waitQueue
{
   list_entry_t wait_list;              //等待队列的哨兵
   unsigned int wait_num;               //等待队列的进程数
};

extern struct waitQueue *wq;            //将wq定义为一个外部变量

void schedClassEnqueue(struct ProcStruct *proc);
void schedClassDequeue(struct ProcStruct *proc);
struct ProcStruct *schedClassPickNext();

void schedInit();
void wakeupProc(struct ProcStruct *proc);
void schedule();
void addRunQueue();

#endif /* !__SCHED_H__ */

