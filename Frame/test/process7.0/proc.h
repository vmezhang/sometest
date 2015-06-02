/*
 * =====================================================================================
 *
 *       Filename:  proc.h
 *
 *    Description:  定义PCB
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

#ifndef __PROC_H_
#define __PROC_H_

#include "list.h"
#include "sched.h"
#include "defs.h"
#include "types.h"
#include "byte_code.h"

//进程状态
enum procState
{
	PROC_UNINIT = 0,         //没有初始化
	PROC_RUNNABLE,           //就绪状态
	PROC_RUNNING,            //运行状态
	PROC_SLEEPING,           //睡眠状态
	PROC_ZOMBLE,             //死亡或者等待父进程释放资源
};

#define PROC_NAME_LEN  15
#define MAX_PROCESS    4096
#define MAX_PID        (MAX_PROCESS * 2)

//上下文切换保存的寄存器
struct context
{
	uint32_t eip;
	uint32_t esp;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t esi;
	uint32_t edi;
	uint32_t ebp;
};

//进程上下文
struct _context
{
    int element[OP_STACK];
    int top;
};

//定义PCB
struct ProcStruct
{
	enum procState state;                  //进程状态
	int pid;                               //进程ID
	char nameProc[PROC_NAME_LEN + 1];      //进程名
	volatile int needResched;              //是否需要释放CPU
	int nice;                              //进程优先级
	int runs;                              //进程运行时间
	int timeSlice;                         //进程剩余的时间
	uint32_t wait_state;                   //等待的状态
	struct ProcStruct *parent;             //父进程
	int parentPid;                         //父进程的pid
	int nextByteAdress;                    //下一条字节码的位置
	
//	char *codeAdress;                      //代码段地址
//	char *dateAdress;                      //数据段地址
//	int codeLength;                        //代码段长度
//	int dateLength;                        //数据段长度

        int byteCodeStartAdress;              //字节码开始地址
	int byteCodeEndAdress;                //字节码结束地址

	struct runQueue *rq;                  //进程包含在运行队列
	struct waitQueue *wq;                 //等待队列
	list_entry_t runLink;                 //进程列表

	struct mm_struct *mm;                 //内存管理的信息
	struct fs_struct *fs_struct;          //文件系统信息

	struct context contextInfo;           //进程切换的上下文信息
	
	struct _context *pcb_stack;           //任务中断时的stack
        int pc;                               //任务中断时的pc
        int code_num;
};

#define WT_INTERRUPTED         0x80000000     //wait 状态将会被打断

#define le2proc(le, member) \
	to_struct((le), struct ProcStruct, member)

//将进程的PCB块进行链接
static list_entry_t *procList;

extern struct ProcStruct *idleproc, *initproc, *current;

//char *setProcName(struct ProcStruct *proc, const char *name);
//char *getProcName(struct ProcStruct *proc);

struct ProcStruct *allocProcStruct(void);
void initProcList();
void procInit(void);
void procRun(struct ProcStruct *proc);
void cpuSchedProc();
void addRunEqueue();
void parallelProcess(int byteStart, int byteEnd, int ppid);
void createProcess(int byteStart, int byteEnd, int);
void initWaitQueue();

/*当CPU解释执行到包含parallel关键字是，就把每一个并发函数字节码的起始地址和终止地址
 * 都组织到一个字符串中,然后由Pfork创建进程
 * createProcAdress格式 字节码首地址*字节码结束地址#字节码首地址
 * */

void Pfork(char *createProcAdress, int ppid);
void Pwait(struct ProcStruct *proc);
void interpAddToRunQueue(struct ProcStruct *proc);
void parallelSched();

#endif

