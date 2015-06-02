/*
 * =====================================================================================
 *
 *       Filename:  proc.cpp
 *
 *    Description:  初始化PCB块和相关操作
 *
 *        Version:  1.0
 *        Created:  2015年05月13日 10时58分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "proc.h"
#include "types.h"
#include "defs.h"
#include "wait_queue.h"

//当前占用cpu且处于运行状态的进程控制块指针
struct ProcStruct *current = NULL;

//idle proc
struct ProcStruct *idleproc = NULL;

//任务总数
static int nr_process = 0;  

static uint32_t glb_pid_map[MAX_PID/32];

//创建一个进程控制块,并进行相应的初始化
struct ProcStruct *allocProcStruct(void)
{
	struct ProcStruct *proc = (struct ProcStruct *)malloc(sizeof(struct ProcStruct));
	if (proc != NULL)
	{
		proc->state = PROC_UNINIT;
		proc->pid = -1;
		proc->runs = 0;
		proc->needResched = 0;
		memset(proc->nameProc, 0, PROC_NAME_LEN);
		proc->nice = 0;
		proc->wait_state = 0;
		proc->timeSlice = 0;
		proc->rq = NULL;

		proc->parentPid = -1;
		proc->nextByteAdress = -1;

		proc->byteCodeStartAdress = -1;
		proc->byteCodeEndAdress = -1;
		
		proc->pcb_stack = (struct _context *)malloc(sizeof(struct _context));
		proc->pcb_stack->top = -1;

		list_init(&(proc->runLink));
	}
	return proc;
}

//设置进程的名字
/*char *setProcName(struct ProcStruct *proc, const char *name)
{
	lmemset(proc->nameProc, 0, sizeof(proc->nameProc));
	return memcpy(proc->nameProc, name, PROC_NAME_LEN);
}

//获取进程名
char *getProcName(struct ProcStruct *proc)
{
	static char name[PROC_NAME_LEN + 1];
	lmemset(name, 0, sizeof(name));
	return memcpy(name, proc->nameProc, PROC_NAME_LEN);
}
*/

//getPid——为每一个进程分配唯一的一个pid
static pid_t allocPid(void)
{
	int i;
	uint32_t j;

	for (i = 0; i < MAX_PID/32; ++i)
	{
		if (glb_pid_map[i] == 0xFFFFFFFF)
		{
			continue;
		}
		for (j = 0; j < 32; ++j)
		{
			if (((1u << j) & glb_pid_map[i]) == 0)
			{
				glb_pid_map[i] |= 1u << j;
				return (pid_t)(i *32 + j);
			}
		}
	}
	return -1;
}

//释放占用的pid
static void freePid(pid_t pid)
{
	if (pid < 0 || pid > MAX_PID)
	{
		return;
	}
	glb_pid_map[pid/32] &= ~(1u << (pid % 32));
}

//procRun使进程运行在cpu上
/*struct ProcStruct *procRun(struct ProcStruct *proc)
{
	printf("now run cpu proc is, nameProc = %s, pid = %d\n", proc->nameProc, proc->pid);
	return proc;

	//注意上下文的保存
	//switch_to()
}
*/

//初始化第一个进程pid = 0
void procInit(void)
{
	if ((idleproc = allocProcStruct()) == NULL)
	{
		printf("cannot alloc proc.\n");
	}
	
	idleproc->pid = allocPid();
	idleproc->state = PROC_RUNNABLE;
	idleproc->needResched = 1;

	nr_process++;

	list_add(&(idleproc->runLink), procList);

}

void initProcList()
{
	procList = (list_entry_t *)malloc(sizeof(struct ProcStruct));
	list_init(procList);
}

//并发模块创建进程
void parallelProcess(int byteStart, int byteEnd, int ppid)
{
	printf("%d, %d, %d\n", byteStart, byteEnd, ppid);

	struct ProcStruct *proc;
	if (nr_process >= MAX_PROCESS)
	{
		printf("no free process!\n");
	}
	if ((proc = allocProcStruct()) == NULL)
	{
		printf("cannot alloc proc!\n");
	}
	proc->pid = allocPid();
	proc->state = PROC_RUNNABLE;
	proc->needResched = 1;
	proc->parentPid = ppid;
	proc->byteCodeStartAdress = byteStart;
	proc->byteCodeEndAdress = byteEnd;
    proc->code_num = 0;
	
	nr_process++;

	current = proc;
}

//进程的创建
void createProcess(int byteStart, int byteEnd, int CodeNum)
{
	struct ProcStruct *proc;

	if (nr_process >= MAX_PROCESS)
	{
		printf("no free process!\n");
	}
	if ((proc = allocProcStruct()) == NULL)
	{
		printf("cannot alloc proc!\n");
	}
	proc->pid = allocPid();
	proc->state = PROC_RUNNABLE;
	proc->byteCodeStartAdress = byteStart;
	proc->byteCodeEndAdress = byteEnd;
	proc->code_num = CodeNum;

	nr_process++;
	current = proc;
}

//进程执行结束后释放资源
void procFinished(struct ProcStruct *proc)
			{
	freePid(proc->pid);
	free(proc);
}

//cpu调度
void cpuSchedProc( )
{
	schedule();
}

//将进程加入运行队列
void addRunEqueue()
{
	addRunQueue();
}

static struct waitQueue __wq;
//初始化等待队列
void initWaitQueue()
{
	wq = &__wq;
	waitQueueInit(wq);
}

//字符到int类型的转换
int chToInt(char ch)
{
	return (ch - '0');
}

//当字节码中遇见parallel关键字,创建进程
void Pfork(char *createProcAdress, int ppid)
{
	printf("createProcAdress = %s\n", createProcAdress);
	printf("ppid = %d\n", ppid);

	char chByteStart, chByteEnd;
	int byteStart, byteEnd;
	while (*createProcAdress != '\0')
	{
		chByteStart = *createProcAdress;
		byteStart = chToInt(chByteStart);
		createProcAdress++;

		if (*createProcAdress == '*' && *createProcAdress != '\0')
		{
			createProcAdress++;
		}
		chByteEnd = *createProcAdress;
		byteEnd = chToInt(chByteEnd);
		createProcAdress++;

		if (*createProcAdress == '#' && *createProcAdress != '\0')
		{
			createProcAdress++;
		}

		//创建一个进程就加入就绪队列
		parallelProcess(byteStart, byteEnd, ppid);
		printf("parallelProcess finished!\n");

		addRunEqueue();
		printf("addRunEqueue finished!\n");
	}
}

//阻塞主进程的执行
void Pwait(struct ProcStruct *proc)
{
	printf("leiyu\n");
	proc->state = PROC_SLEEPING;
	addWaitQueue(wq, proc);
}

//进程退出
void Pexit(struct ProcStruct *proc)
{
	//释放PCB
	//释放Pid
	freePid(proc->pid);
	free(proc);
}

//当cpu发生中断，就将中断的程序放入就绪队列
void interpAddToRunQueue(struct ProcStruct *proc)
{
	schedClassEnqueue(proc);
}

//解释字节码遇见parallel关键字创建进程后的的调度
void parallelSched()
{
	printf("parallelSched start....\n");

	struct ProcStruct *next;
	while (rq->proc_num)
	{
		schedule();
	}
	if (rq->proc_num == 0 && wq->wait_num != 0)
	{
		next = waitQueuePickNext(wq);
		schedClassEnqueue(next);
		schedule();
	}
	
	printf("parallelSched end....\n");
}

//在就绪队列中判断进程proc的子进程是否执行完毕,执行完毕返回1,没有执行完毕0
int judgeChildProc(struct ProcStruct *proc, struct runQueue *rq)
{
	list_entry_t *pos, *temp;
	struct ProcStruct *runNode;
	temp = &(rq->run_list);
	list_for_each(pos, temp)
	{
		runNode = list_entry(pos, struct ProcStruct, runLink);
		if (runNode->parentPid == proc->pid)
		{
			return 0;
		}

	}
	return 1;
}
