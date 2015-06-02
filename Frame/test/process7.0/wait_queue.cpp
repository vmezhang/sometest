/*
 * =====================================================================================
 *
 *       Filename:  wait_queue.c
 *
 *    Description:  实现等待队列的相关操作
 *
 *        Version:  1.0
 *        Created:  2015年05月20日 22时06分29秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "wait_queue.h"

//初始化等待队列
void waitQueueInit(struct waitQueue *wq) 
{
    list_init(&(wq->wait_list));
    wq->wait_num = 0;
}

//进程加入等待队列
void addWaitQueue(struct waitQueue *wq, struct ProcStruct *proc) 
{
    printf("addWaitQueue!\n");
    assert(list_empty(&(proc->runLink)));
    list_add_before(&(wq->wait_list), &(proc->runLink));
    proc->wq = wq;
    wq->wait_num++;
}

//从等待队列中删除一个进程
void delWaitQueue(struct waitQueue *wq, struct ProcStruct *proc) 
{
    printf("delWaitQueue!\n");
    assert(!list_empty(&(proc->runLink)) && proc->wq == wq);
    list_del_init(&(proc->runLink));
    wq->wait_num--;
}

//从等待队列中选择一个进程`
struct ProcStruct *waitQueuePickNext(struct waitQueue *wq) 
{
    printf("waitQueuePickNext!\n");	
    list_entry_t *le = list_next(&(wq->wait_list));
    if (le != &(wq->wait_list))
    {
        return le2proc(le, runLink);
    }
    return NULL;
}

