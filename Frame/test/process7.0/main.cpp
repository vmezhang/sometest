/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  测试用的主函数
 *
 *        Version:  1.0
 *        Created:  2015年05月14日 18时12分25秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "cpu_init.h"
#include "proc.h"
#include "sched.h"
#include "list.h"
#include "wait_queue.h"
#include "read_byte_code.h"

//ByteCode byte_code1[4]={{0x01,100,1},{0x02,0,2},{0x03,0,3},{0x04,0,4}};
//ByteCode byte_code[20]={{0x01,100,1},{0x02,0,2},{0x03,0,3},{0x01,1,5},{0x04,0,4},{0x19,6,6},{0x01,1,7},{0x01,0,8},{0x0E,1,9}};

struct runQueue *rq = NULL;
struct waitQueue *wq = NULL;

void printCurrent()
{
	printf("create new process : current->pid = %d, current->byteCodeStartAdress = %d, current->byteCodeEndAdress = %d, current->code_num = %d\n",current->pid, current->byteCodeStartAdress, current->byteCodeEndAdress, current->code_num);
}

int main(int argc, char *argv[])
{

	list_entry_t *pos;
	int byteStart, byteEnd, globalEnd;
	struct ProcStruct *next;

	schedInit();                                          //init schedule 
  
	printf("SchedInit finished!\n");
	
	cpu_init();                                          //init cpu
 
	printf("Cpu init finished!\n");

	initProcList();                                      //runnbale process 
 
	printf("Init process list finished!\n");

	procInit();                                          //init process table

	printf("The first proc init finished!\n");

	initWaitQueue();                                     //init wait queue

	printf("Init wait queue finished!\n");

	bc_read(&byteStart, &byteEnd, &globalEnd);           //读取字节码

	/*
	for (int i = 0; i < 7; i++) {
		printf("%d %d %d\n", byte_code[i].code, byte_code[i].arg1, byte_code[i].arg2);
	}
*/
	createProcess(byteStart, byteEnd, globalEnd);        //根据字节码的初始下标和终止下标创建第一个进程

//	createProcess(0, 9, 3);                              //根据字节码的初始下标和终止下标创建第一个进程

	printCurrent();

	addRunEqueue();                                     //将创建的进程加入到就绪队列
	
	printf("The proc addRunEqueu finished!\n");


	    while (cpu_state != 0 && rq->proc_num)
	    {
		    printf("schedul start...\n");
		    cpuSchedProc();
	    }
	    while (rq->proc_num == 0 && wq->wait_num != 0)
	    {
		    next = waitQueuePickNext(wq);
		    delWaitQueue(wq, next);
		    schedClassEnqueue(next);
		    schedule();
	    }
	
	return 0;
}


