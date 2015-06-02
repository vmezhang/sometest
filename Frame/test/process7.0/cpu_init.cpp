// =====================================================================================
//
//       Filename:  cpu_init.cpp
//
//    Description:  cpu初始化的实现
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

#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#include "cpu_init.h"

struct ProcStruct *PCB = NULL;
HeapArea heap = NULL;

void *thread_routine(void *arg)
{
	//int args = *(int *)arg;
	int args = 0;
	    printf("args:%d\n", args);
	while(1)
	{
	    pthread_mutex_lock(&task_lock);
	    while(task_num == 0)
	    {
		pthread_cond_wait(&ready_task,&task_lock);
	    }
	    cpu_state--;
	    task_num--;
	    pthread_mutex_unlock(&task_lock);
	    //printf("args:%d\n", args);
        /*for(int a=0;a<20;a++)
        {
            printf("a:%d   b:%d\n", code[a].code,code[a].arg1);
        }*/
        printf("args:%d\n", args);
        simulate(_VMstack[(args)], code, (args));
	    pthread_mutex_lock(&task_lock);
	    cpu_state++;
        printf("cpu_state:%d\n", cpu_state);
	    pthread_mutex_unlock(&task_lock);
	}
	return NULL;
}

int add_task(struct ProcStruct *proc)
{
    printf("add_task start...\n");
    for(int co=0;co<bytecode_num;co++)
    {
        code[co].code = 0;
        code[co].arg1 = 0;
        code[co].arg2 = 0;
    }
    int i,j,s;
    PCB = proc;
    int codeNum = PCB->code_num;
    if(codeNum != 0)
    {
        for(int m=0;m<codeNum;m++)
        {
            code[m].code = byte_code[m].code;
            code[m].arg1 = byte_code[m].arg1;
            code[m].arg2 = byte_code[m].arg2;
        }
    }
    int start_addr = PCB->byteCodeStartAdress;
    int end_addr = PCB->byteCodeEndAdress;
    pthread_mutex_lock(&task_lock);
    for(i=start_addr,j=codeNum,s=0; i<end_addr; i++,j++,s++)
    {
        code[s].code = byte_code[i].code;
	printf("code:%d:s:%d\n",code[s].code, s);
	printf("ssscode:%d:sssss:%d\n",byte_code[i].code, s);
        code[s].arg1 = byte_code[i].arg1;
        code[s].arg2 = byte_code[i].arg2;
    }
    task_num++;
    printf("tasknum:%d\n", task_num);
    pthread_mutex_unlock(&task_lock);
    pthread_cond_signal(&ready_task); 
sleep(1);
}

int cpu_init()
{

    heap = (HeapArea)malloc(sizeof(heap_area)); 
	heap->next = NULL;
	    
	heap->arrayref = (ArrayRef *)malloc(sizeof(ArrayRef));   //初始化数组结构
	heap->arrayref->next = NULL;
	pthread_t *thread_link;
	int num;
	
	for(int vm=0; vm<max_thread_num; vm++)
	{
	    _VMstack[vm] = (LinkVMstack)malloc(sizeof(LinkVMstackNode));
	    _VMstack[vm]->next = NULL;
	}

	printf("OSPF Running ...\n");

    pthread_mutex_init(&task_lock,NULL);
    pthread_cond_init(&ready_task,NULL);
    thread_link = (pthread_t *)malloc(sizeof(pthread_t)*max_thread_num);  

    for(num=0;num<max_thread_num;num++)
    {
            pthread_create(&(thread_link[num]), NULL, thread_routine, (void *)&num);
    }

        return 0;
}
