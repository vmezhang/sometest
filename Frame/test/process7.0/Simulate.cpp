// =====================================================================================
//
//       Filename:  Simulate.cpp
//
//    Description:  字节码解释器
//
//        Version:  1.0
//        Created:  2014年12月30日 14时56分07秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Qianyi.lh (liuhuan), qianyi.lh@alibaba-inc.com
//        Company:  Alibaba-Inc Aliyun
//
// =====================================================================================

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "cpu_init.h"
#include "Simulate.h"
#include "byte_code.h"
#include "proc.h"
#include "sched.h"
#include "Syscalls.h"

typedef int (*sim_func_t)(int, int, int);

    int ipush(int value, int index2 , int );
    int istore(int index, int index2, int );
    int iload(int index, int index2, int );
    int newarray1(int index, int index2, int );
    int newarray2(int index, int index2, int );
    int aload(int index, int index2, int);
    int astore(int index, int index2, int);
    int caload(int index1, int index2, int);
    int castore(int index1, int index2, int);
    int _goto(int branch, int index2, int);
    int i2c(int index1, int index2, int);
    int iadd(int index1, int index2, int);
    int iaload(int index1, int index2, int);
    int iastore(int index1, int index2, int);
    int idiv(int index1, int index2, int);
    int if_icmpeq(int branch, int index2, int);
    int if_icmpne(int branch, int index2, int);
    int if_icmplt(int branch, int index2, int);
    int if_icmpge(int branch, int index2, int);
    int if_icmppg(int branch, int index2, int);
    int if_icmple(int branch, int index2, int);
    int imul(int index1, int index2, int);
    int ior(int index1, int index2, int);
    int irem(int index1, int index2, int);
    int ireturn(int index1, int index2, int);
    int ishl(int index1, int index2, int);
    int ishr(int index1, int index2, int);
    int isub(int index1, int index2, int);
    //int newarray1(int index, int index2);
    //int newarray2(int index, int index2);
    int pop(int index1, int index2, int);
    int _return(int index1, int index2, int);
    int dup(int index1, int index2, int);
    int swap(int index1, int index2, int);
    int invokeinterface(int index1, int index2, int);
    int parallel(int index1, int index2, int);
    int32_t sim_syscall(int, int, int);

    void push_stack(operate_stack *_stack, int _pc);
    void read_array(int ref, operate_stack *_stack);

// 具体字节码处理函数
static sim_func_t sim_func[BYTE_CODE_MAX + 1] = {
        NULL,
        &ipush,&istore,&iload,
        &newarray1,&iadd,&_goto,&if_icmple,&imul,&_return,&sim_syscall,&aload,
        &caload,&castore,&newarray2,
        &astore,&i2c,
        &iaload,
        &iastore,&idiv,
        &if_icmpeq,&if_icmpne,
        &if_icmplt,&if_icmpge,
        &if_icmppg,
        &ior,&irem,
        &ireturn,&ishl,&ishr,
        &isub,&pop,
        &dup,&swap,
        &invokeinterface,
        &parallel
};
LinkVMstack VMstack[6];
pthread_mutex_t _lock;  

int intr_no;
bool is_intr;
int timer;
char array_str[60];
void read_array(int ref, operate_stack *_stack)
{
	pthread_mutex_lock(&_lock);
    ArrayRef *arrayref = heap->arrayref;
    ArrayRef *top = arrayref->next;
    int i = 1;
    while(i < ref)
    {
        i++;
        top = top->next;
    }
    if(i == ref)
    {
        strcpy(array_str, (char *)top->carray_data);
    }
    pthread_mutex_unlock(&_lock);
}

void push_stack(operate_stack *_stack, int _pc)
{
    while(_stack->top != -1)
    {
        PCB->pcb_stack->top++;
        PCB->pcb_stack->element[PCB->pcb_stack->top] = _stack->element[_stack->top];
        _stack->top--;
    }
    PCB->pc = _pc+1;
}

bool simulate(LinkVMstack _VMstack, ByteCode *bytecode, int mm)
{
    intr_no = 0;
    is_intr = false;
    int p = mm;
    int a = 0;
    int fd;
    timer=0;
   /* cpu_set_t mask;
    cpu_set_t get;
    int num = sysconf(_SC_NPROCESSORS_CONF);   //获取当前的CPU数
    printf("当前计算机的核数为：%d\n", num);
    pthread_detach(pthread_self());
    CPU_ZERO(&mask);
    CPU_SET(p-1,&mask);
    if(sched_setaffinity(0, sizeof(mask), &mask) == -1)   //0代表对当前线程进行设定
    {
        printf("set affinity failed..\n");
    }
    else
    {
        CPU_ZERO(&get);
        if(sched_getaffinity(0, sizeof(get), &get) == -1)
        {
            printf("can`t get cpu affinity...\n");
        }
        for(a =0; a<num; a++)
        {
            if(CPU_ISSET(a, &get))
            {
                //printf("线程%d绑定到里核%lu上...\n", pthread_self(), a);
            }
        }
    }*/

    printf("核%d正在运行任务......\n",p);
    VMstack[p] = _VMstack;
	VMstack[p]->StackFrame = (stack_frame *)malloc(sizeof(stack_frame));   //初始化栈帧
	VMstack[p]->StackFrame->next = NULL;
    VMstack[p]->StackFrame->pc = 0;

	VMstack[p]->StackFrame->OpStack = (operate_stack *)malloc(sizeof(operate_stack));
	VMstack[p]->StackFrame->OpStack->top = -1;   //初始化操作数栈
//	heap->arrayref = (ArrayRef *)malloc(sizeof(ArrayRef));   //初始化数组结构
//	heap->arrayref->next = NULL;
    pthread_mutex_init(&_lock,NULL); 
            bool inter = false;

            if(PCB->pcb_stack->top != -1)
            {
                while(PCB->pcb_stack->top != -1)
                {
                    printf("a:%d   b:%d\n", PCB->pcb_stack->element[PCB->pcb_stack->top], PCB->pc);
                    VMstack[p]->StackFrame->OpStack->top++;
                    VMstack[p]->StackFrame->OpStack->element[VMstack[p]->StackFrame->OpStack->top] = PCB->pcb_stack->element[PCB->pcb_stack->top];
                    PCB->pcb_stack->top--;
                }
                VMstack[p]->StackFrame->pc = PCB->pc;
            }
            /*struct itimerval timer;
            signal(SIGVTALRM, handler_sigtime);
            timer.it_value.tv_sec = 2;
            timer.it_value.tv_usec = 0;
            setitimer(ITIMER_VIRTUAL, &timer, NULL);*/
            while(1)
            {
                if ((is_intr = inter)) {
                   // intr_no = 1;
                        if (intr_no != 0) {
                                // TODO 中断处理
                                //printf("段错误，核%d将要退出任务\n",p);
                                return false;
                                exit(1);
                        }
                }
	            if(timer == 10000)
                {
                    push_stack(VMstack[p]->StackFrame->OpStack, VMstack[p]->StackFrame->pc);
                    interpAddToRunQueue(PCB);
                }
                if (bytecode[VMstack[p]->StackFrame->pc].code <= 0 || bytecode[VMstack[p]->StackFrame->pc].code > BYTE_CODE_MAX) {
                        // errno = XXXX
                       //printf("编译字节码出现错误，将要退出本次任务\n");
                        break;
                }
                printf("cpu:%d  pc:%d\n",p,VMstack[p]->StackFrame->pc);
		printf("code:%d  arg1:%d   arg2:%d  \n", bytecode[VMstack[p]->StackFrame->pc].code,bytecode[VMstack[p]->StackFrame->pc].arg1,bytecode[VMstack[p]->StackFrame->pc].arg2);
                
               inter = sim_func[bytecode[VMstack[p]->StackFrame->pc].code](bytecode[VMstack[p]->StackFrame->pc].arg1, bytecode[VMstack[p]->StackFrame->pc].arg2, p);
               if(bytecode[VMstack[p]->StackFrame->pc].code == 35)
               {
                    break;
               }
                (VMstack[p]->StackFrame->pc)++;
        }
 //       free(VMstack[m]->StackFrame);
  //      free(VMstack[m]->StackFrame->OpStack);
        
        free(VMstack[p]->StackFrame->OpStack);
        free(VMstack[p]->StackFrame);

        if(VMstack[p]->StackFrame->OpStack->top == -1)
        {
            printf("nothing return\n");
        }
        else
        {
            int ref = VMstack[p]->StackFrame->OpStack->element[VMstack[p]->StackFrame->OpStack->top];
            VMstack[p]->StackFrame->OpStack->top--;
            read_array(ref, VMstack[p]->StackFrame->OpStack);
	        pthread_mutex_lock(&_lock);
            if((fd = open("/tmp/result.txt", O_RDWR|O_CREAT, S_IRWXU)) == -1)
            {
                printf("open error\n");
                return true;
            }
            if(write(fd, array_str, strlen(array_str)) != strlen(array_str))
            {
                printf("write error\n");
            }
            printf("result:%s\n", array_str);
	        pthread_mutex_unlock(&_lock);
        }

       // pexit(PCB);
        return true;
}

//创建一个数组
    int newarray1(int index, int index2, int m)
    {
            //stack_frame *StackFrame = VMstack->StackFrame;
            operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
            ArrayRef *arrayref = heap->arrayref;
            //int array[10];
            ArrayRef *top = arrayref;
            ArrayRef *heap_array;
            pthread_mutex_lock(&_lock); 
            heap_array = (ArrayRef *)malloc(sizeof(ArrayRef));
            if(heap_array == NULL)
            {
                    return false;
                    intr_no = 1;
            }
            if(OpStack->top == -1)
            {
                    intr_no = 1;
                    return false;
            }
           // pthread_mutex_lock(&_lock); 
           int *int_array = (int *)(heap_array->array_data);
            heap_array->array_data = (int *)malloc(sizeof(int) * 8);
            int count = OpStack->element[OpStack->top];
            heap_array->ret = count;
            OpStack->top++;
            OpStack->element[OpStack->top] = index;
            while(top->next != NULL)
            {
                    top = top->next;
            }
            heap_array->next = top->next;
            top->next = heap_array;
            int * p = (int *)(heap_array->array_data);
           // heap_array->array_data[3] = 11;
            p[3] = 11; 
           // printf("m: %d array:%d\n", m, heap_array->array_data[3]);
            printf("cpu:%d ret:%d array_num:%d\n", m, heap_array->ret,OpStack->element[OpStack->top]);
       //     free(heap_array->array_data);
       //     free(heap_array);
            pthread_mutex_unlock(&_lock);  
            timer++;
            return true;
    }
    int ipush(int value, int index2, int m)
    {
            //printf("m: %d push\n", m);
           // stackframe *StackFrame = VMstack->StackFrame;
            operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
            pthread_mutex_lock(&_lock); 
            if(OpStack->top == OP_STACK - 1)
            {
                    intr_no = 1;
                    return false;
            }
            OpStack->top++;
            OpStack->element[OpStack->top] = value;
            printf("cpu:%d ipush:%d\n", m, OpStack->element[OpStack->top]);
            pthread_mutex_unlock(&_lock); 
            timer++;
            return true;
    }
    int istore(int index, int index2, int m)
    {
            //stack_frame *StackFrame = VMstack->StackFrame;
            operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
            if(OpStack->top == -1)
            {
                    intr_no = 1;
                    return false;
            }
            int value = OpStack->element[OpStack->top];
            OpStack->top--;
            pthread_mutex_lock(&_lock); 
            heap->localvariable_table[index] = value;
            printf("cpu:%d istore:%d\n", m, heap->localvariable_table[index]);
            pthread_mutex_unlock(&_lock);  
            timer++;
            return true;
    }
    int iload(int index, int index2, int m)
    {
            //stack_frame *StackFrame = VMstack[m]->StackFrame;
            operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
            if(OpStack->top == OP_STACK - 1)
            {
                    intr_no = 1;
                    return false;
            }
            OpStack->top++;
            pthread_mutex_lock(&_lock); 
            OpStack->element[OpStack->top] = heap->localvariable_table[index];
            printf("cpu:%d iload:%d\n", m, OpStack->element[OpStack->top]);
            pthread_mutex_unlock(&_lock);  
            timer++;
            return true;
    }

//从局部变量表加载数组到操作数栈
    int aload(int index, int index2, int m)
    {
        //stack_frame *StackFrame = VMstack->StackFrame;
        operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
        //ArrayRef *arrayref = heap->arrayref;
        if(OpStack->top == OP_STACK - 1)
        {
            intr_no = 1;
            return false;
        }
        OpStack->top++;
        pthread_mutex_lock(&_lock);
        OpStack->element[OpStack->top] = heap->localvariable_table[index];
        pthread_mutex_unlock(&_lock);
            timer++;
        return true;
    }
//从操作数栈中加载数组到局部变量表
    int astore(int index, int index2, int m)
    {
        //stack_frame *StackFrame = VMstack->StackFrame;
        operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
        //ArrayRef *arrayref = heap->arrayref;
        if(OpStack->top == -1)
        {
            intr_no = 1;
            return false;
        }
        else
        {
            pthread_mutex_lock(&_lock);
            heap->localvariable_table[index] = OpStack->element[OpStack->top];
            OpStack->top--;
            pthread_mutex_unlock(&_lock);
            timer++;
            return true;
        }
    }
//从数组中加载一个char类型数据到操作数栈
    int caload(int index1, int index2, int m)
    {
        //stack_frame *StackFrame = VMstack->StackFrame;
        operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
        pthread_mutex_lock(&_lock);
        ArrayRef *arrayref = heap->arrayref;
        ArrayRef *top = arrayref->next;
        int i = 1;
        char arr_data;
        if(OpStack->top == -1)
        {
            return false;
        }
        int index = OpStack->element[OpStack->top];
        OpStack->top--;
        int ref = OpStack->element[OpStack->top];
        OpStack->top--;
        while(i < ref)
        {
            i++;
            top = top->next;
        }
        if(i == ref)
        {
            arr_data = (top->carray_data[index]);
            OpStack->top++;
            OpStack->element[OpStack->top] = arr_data;
        }
        else
        {
            return false;
        }
        pthread_mutex_unlock(&_lock);
        return true;
    }
//从操作数栈读取一个char类型数据到数组
    int castore(int index1, int index2, int m)
    {
        //stack_frame *StackFrame = VMstack->StackFrame;
        operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
        pthread_mutex_lock(&_lock);
        ArrayRef *arrayref = heap->arrayref;
        ArrayRef *top = arrayref->next;
        char value;
        int i = 1;
        int index,ref;
        if(OpStack->top == -1)
        {
            return false;
        }
        value = OpStack->element[OpStack->top];
        OpStack->top--;
        index = OpStack->element[OpStack->top];
        OpStack->top--;
        ref = OpStack->element[OpStack->top];
        OpStack->top--;
        while(i < ref)
        {
            i++;
            top = top->next;
        }
        if(i == ref)
        {
            top->carray_data[index] = value;
        }

        else
        {
            return false;
        }
        pthread_mutex_unlock(&_lock);
        return true;
    }
//分支跳转
int _goto(int branch, int index2, int m)
{
    stack_frame *StackFrame = VMstack[m]->StackFrame;
    //operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    StackFrame->pc = branch-1;
    //printf("goto\n");
            timer++;
    return true;
}
//将int类型数据转换为char类型
int i2c(int index1, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;

    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    else
    {
        char s = char(OpStack->element[OpStack->top]);
        OpStack->element[OpStack->top] = s;
        pthread_mutex_unlock(&_lock); 
            timer++;
        return true;
    }
}
//int类型数据相加
int iadd(int index1, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    else
    {
        int value1 = OpStack->element[OpStack->top];
        OpStack->top--;
        int value2 = OpStack->element[OpStack->top];
        int result = value1 + value2;
        OpStack->element[OpStack->top] = result;
        printf("iadd:%d\n",result);
        pthread_mutex_unlock(&_lock); 
            timer++;
        return true;
    }
}
//数组中加载int类型到操作数栈
int iaload(int index1, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    ArrayRef *arrayref = heap->arrayref;
    ArrayRef *top = arrayref->next;
    int i = 0;
    int arr_data;
        pthread_mutex_lock(&_lock);
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    int index = OpStack->element[OpStack->top];
    OpStack->top--;
    int ref = OpStack->element[OpStack->top];
    OpStack->top--;
    while(i < ref)
    {
        i++;
        top = top->next;
    }
    if(i == ref)
    {
        arr_data = top->array_data[index];
        OpStack->top++;
        OpStack->element[OpStack->top] = arr_data;
        printf("iaload:%d\n", top->array_data[index]);
    }
    else
    {
            intr_no = 1;
        return false;
    }
        pthread_mutex_unlock(&_lock);
            timer++;
    return true;
}
//从操作数栈读取int类型数据到数组
int iastore(int index1, int index2, int m)
{
	printf("qqqq\n");
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    ArrayRef *arrayref = heap->arrayref;
    ArrayRef *top = arrayref->next;
    int value;
    int i = 0;
    int index,ref;
        pthread_mutex_lock(&_lock);
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    printf("top:%d\n", OpStack->top);
    ref = OpStack->element[OpStack->top];
    OpStack->top--;
    printf("top:%d\n", OpStack->top);
    index = OpStack->element[OpStack->top];
    OpStack->top--;
    printf("top:%d\n", OpStack->top);
    value = OpStack->element[OpStack->top];
    OpStack->top--;
    printf("index:%d  ref:%d  value:%d\n", index,ref,value);
    while(i < ref)
    {
        i++;
        top = top->next;
    }
    if(i == ref)
    {
        top->array_data[index] = value;
        printf("iastore:%d\n", top->array_data[index]);
    }
    else
    {
            intr_no = 1;
        return false;
    }
        pthread_mutex_unlock(&_lock);
            timer++;
    return true;
}
//int类型数据除法
int idiv(int index1, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    // ArrayRef *arrayref = heap->arrayref;
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    else
    {
        pthread_mutex_lock(&_lock);
        int value1 = OpStack->element[OpStack->top];
        OpStack->top--;
        int value2 = OpStack->element[OpStack->top];
        int result = value1/value2;
        OpStack->element[OpStack->top] = result;
        pthread_mutex_unlock(&_lock);
            timer++;
        return true;
    }
}
//int数值的条件分支判断相等
int if_icmpeq(int branch, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    int value1 = OpStack->element[OpStack->top];
    OpStack->top--;
    int value2 = OpStack->element[OpStack->top];
    OpStack->top--;
    if(value1 == value2)
    {
       // goto branch; //跳转到编号为branch的指令继续执行
        VMstack[m]->StackFrame->pc = branch-1;
    }
    pthread_mutex_unlock(&_lock); 
            timer++;
    return true;
}
//int数值的条件分支判断不等
int if_icmpne(int branch, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    int value1 = OpStack->element[OpStack->top];
    OpStack->top--;
    int value2 = OpStack->element[OpStack->top];
    OpStack->top--;
    if(value1 != value2)
    {
       // goto branch; //跳转到编号为branch的指令继续执行
        VMstack[m]->StackFrame->pc = branch-1;
    }
    pthread_mutex_unlock(&_lock); 
            timer++;
    return true;
}
//int数值的条件分支判断小于
int if_icmplt(int branch, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    int value1 = OpStack->element[OpStack->top];
    OpStack->top--;
    int value2 = OpStack->element[OpStack->top];
    OpStack->top--;
    if(value1 < value2)
    {
        //goto branch; //跳转到编号为branch的指令继续执行
        VMstack[m]->StackFrame->pc = branch-1;
    }
    pthread_mutex_unlock(&_lock); 
            timer++;
    return true;
}
//int数值的条件分支判断小于等于
int if_icmpge(int branch, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    int value1 = OpStack->element[OpStack->top];
    OpStack->top--;
    int value2 = OpStack->element[OpStack->top];
    OpStack->top--;
    if(value1 <= value2)
    {
        //goto branch; //跳转到编号为branch的指令继续执行
        VMstack[m]->StackFrame->pc = branch-1;
    }
    pthread_mutex_unlock(&_lock); 
            timer++;
    return true;
}
//int数值的条件分支判断大于
int if_icmppg(int branch, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    int value1 = OpStack->element[OpStack->top];
    OpStack->top--;
    int value2 = OpStack->element[OpStack->top];
    OpStack->top--;
    if(value1 > value2)
    {
        //goto branch; //跳转到编号为branch的指令继续执行
        VMstack[m]->StackFrame->pc = branch-1;
    }
    pthread_mutex_unlock(&_lock); 
            timer++;
    return true;
}
//int数值的条件分支判断大于等于
int if_icmple(int branch, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    // ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    int value1 = OpStack->element[OpStack->top];
    OpStack->top--;
    int value2 = OpStack->element[OpStack->top];
    OpStack->top--;
    if(value1 >= value2)
    {
       // goto branch; //跳转到编号为branch的指令继续执行
        VMstack[m]->StackFrame->pc = branch-1;
    }
    pthread_mutex_unlock(&_lock); 
            timer++;
    return true;
}
//从局部变量表加载int类型数据到操作数栈
/*int iload(int index)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    if(OpStack->top == OP_STACK - 1)
    {
        return false;
    }
    OpStack->top++;
    pthread_mutex_lock(&_lock);
    OpStack->element[OpStack->top] = heap->localvariable_table[index];
    pthread_mutex_unlock(&_lock);
    return true;
}*/
//int类型数据乘法
int imul(int index1, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    else
    {
        int value1 = OpStack->element[OpStack->top];
        OpStack->top--;
        int value2 = OpStack->element[OpStack->top];
        int result = value1*value2;
        OpStack->element[OpStack->top] = result;
        printf("value1:%d  value2:%d  imul:%d\n",value1,value2,result);
        pthread_mutex_unlock(&_lock); 
            timer++;
        return true;
    }
}
//int类型数值的布尔或运算
int ior(int index1, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    else
    {
        int value1 = OpStack->element[OpStack->top];
        OpStack->top--;
        int value2 = OpStack->element[OpStack->top];
        int result = value1 | value2;
        OpStack->element[OpStack->top] = result;
        pthread_mutex_unlock(&_lock); 
            timer++;
        return true;
    }
}
//int类型数据求余
int irem(int index1, int index2, int m)
{
    //stack_frame *StackFrame = VMstack[m]->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    else
    {
        int value1 = OpStack->element[OpStack->top];
        OpStack->top--;
        int value2 = OpStack->element[OpStack->top];
        int result = value1 % value2;
        OpStack->element[OpStack->top] = result;
        pthread_mutex_unlock(&_lock); 
            timer++;
        return true;
    }
}
//结束方法，并返回一个int类型数据
int ireturn(int index1, int index2, int m)
{
    stack_frame *StackFrame = VMstack[m]->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    if(OpStack->top == OP_STACK - 1 || OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    int value = OpStack->element[OpStack->top];
    OpStack->top--;
    StackFrame->pc = OpStack->element[OpStack->top];
    OpStack->element[OpStack->top] = value;
    //value入栈调用者栈帧的操作数栈
    //销毁当前栈帧操作数栈，恢复调用者栈帧
            timer++;
    return true;
}

// int数值左移运算
int ishl(int index1, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    else
    {
        int value1 = OpStack->element[OpStack->top];
        OpStack->top--;
        int value2 = OpStack->element[OpStack->top];
        int result = value1 << value2;
        OpStack->element[OpStack->top] = result;
            timer++;
        return true;
    }
}
//int数值右移运算
int ishr(int index1, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    else
    {
        int value1 = OpStack->element[OpStack->top];
        OpStack->top--;
        int value2 = OpStack->element[OpStack->top];
        int result = value1 >> value2;
        OpStack->element[OpStack->top] = result;
        pthread_mutex_unlock(&_lock); 
            timer++;
        return true;
    }
}
//将一个int类型数据保存到局部变量表中
/*int istore(int index)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    if(OpStack->top == -1)
    {
        return false;
    }
    int value = OpStack->element[OpStack->top];
    OpStack->top--;
    pthread_mutex_lock(&_lock);
    heap->localvariable_table[index] = value;
    pthread_mutex_unlock(&_lock);
    return true;
}*/
//int类型数据相减
int isub(int index1, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock); 
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    else
    {
        int value1 = OpStack->element[OpStack->top];
        OpStack->top--;
        int value2 = OpStack->element[OpStack->top];
        int result = value1 - value2;
        OpStack->element[OpStack->top] = result;
        pthread_mutex_unlock(&_lock); 
            timer++;
        return true;
    }
}
//创建一个int数组
/*int newarray1(int index,int index2)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    ArrayRef *arrayref = heap->arrayref;
    pthread_mutex_lock(&_lock);
    ArrayRef *top = arrayref;
    ArrayRef *heap_array;
    heap_array = (ArrayRef *)malloc(sizeof(ArrayRef));
    if(heap_array == NULL)
    {
        return false;
    }
    if(OpStack->top == -1)
    {
        return false;
    }
    int count = OpStack->element[OpStack->top];
    heap_array->ret = count;
    heap_array->array_data = (int *)array;
    OpStack->element[OpStack->top] = index;
    while(top->next != NULL)
    {
        top = top->next;
    }
    heap_array->next = top->next;
    top->next = heap_array;
    pthread_mutex_unlock(&_lock);
    return true;
}*/
//创建一个char数组
int newarray2(int index, int index2, int m)
{
     //stack_frame *StackFrame = VMstack->StackFrame;
            operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
//            printf("haha1\n");
            ArrayRef *arrayref = heap->arrayref;
 //           printf("haha2\n");
            //int array[10];
            ArrayRef *top = arrayref;
  //          printf("haha3\n");
            ArrayRef *heap_array;
   //         printf("haha4\n");
            pthread_mutex_lock(&_lock); 
            heap_array = (ArrayRef *)malloc(sizeof(ArrayRef));
    //        printf("haha5\n");
            if(heap_array == NULL)
            {
    //                printf("haha6\n");
                    return false;
                    intr_no = 1;
            }
            if(OpStack->top == -1)
            {
     //               printf("haha7\n");
                    intr_no = 1;
                    return false;
            }
           // pthread_mutex_lock(&_lock); 
           char *int_array = (char *)(heap_array->carray_data);
            heap_array->carray_data = (char *)malloc(sizeof(char) * 20);
      //      printf("haha8\n");
            int count = OpStack->element[OpStack->top];
            heap_array->ret = count;
            OpStack->element[OpStack->top] = index;
            while(top->next != NULL)
            {
                    top = top->next;
            }
            heap_array->next = top->next;
            top->next = heap_array;
            char * p = (char *)(heap_array->carray_data);
           // heap_array->array_data[3] = 11;
            p[3] = 'a'; 
           // printf("m: %d array:%d\n", m, heap_array->array_data[3]);
            printf("cpu:%d ret:%d array_num:%d\n", m, heap_array->ret,OpStack->element[OpStack->top]);
            printf("cpu:%d astore:%c\n", m, p[3]);
            printf("cpu:%d aload:%c\n", m, p[3]);
       //     free(heap_array->array_data);
       //     free(heap_array);
            pthread_mutex_unlock(&_lock);  
            timer++;
            return true;
}
//将操作数栈的栈顶元素出栈
int pop(int index1, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    int value1 = OpStack->element[OpStack->top];
    printf("value1:%d\n", value1);
    OpStack->top--;
            timer++;
    return true;
}

//将一个常量入栈
/*int ipush(int value)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    if(OpStack->top == OP_STACK - 1)
    {
        return false;
    }
    OpStack->top++;
    OpStack->element[OpStack->top] = value;
    return true;
}*/
//无返回值的方法返回
int _return(int index1, int index2, int m)
{
    stack_frame *StackFrame = VMstack[m]->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    if(OpStack->top == OP_STACK - 1 || OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    StackFrame->pc = OpStack->element[OpStack->top];
    printf("任务运行结束\n");
            timer++;
    return true;
}
//复制栈顶部一个字长内容
int dup(int index1, int index2,int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    if(OpStack->top == -1 || OpStack->top == OP_STACK - 1)
    {
            intr_no = 1;
        return false;
    }
    int value = OpStack->element[OpStack->top];
    OpStack->top++;
    OpStack->element[OpStack->top] = value;
            timer++;
    return true;
}
//交换栈顶部两个字长内容
int swap(int index1, int index2, int m)
{
    //stack_frame *StackFrame = VMstack->StackFrame;
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    int value1 = OpStack->element[OpStack->top];
    OpStack->top--;
    int value2 = OpStack->element[OpStack->top];
    OpStack->element[OpStack->top] = value1;
    OpStack->top++;
    OpStack->element[OpStack->top] = value2;
            timer++;
    return true;
}
//调用实例方法
int invokeinterface(int index1, int index2, int m)
{
    operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    if(OpStack->top == OP_STACK - 1)
    {
        intr_no = 1;
        return false;
    }
    OpStack->top++;
    VMstack[m]->StackFrame->OpStack->element[OpStack->top] = VMstack[m]->StackFrame->pc;
    VMstack[m]->StackFrame->pc = index1; 
    printf("pc:%d\n", VMstack[m]->StackFrame->pc);
            timer++;
    return true;
}
//并行程序标志
int parallel(int index1, int index2, int m)
{
    char str[100];
    char buf[20];
    int i;
    int index = index1;
    //snprintf(str, 8, "%d*%d", 0, 7);

    snprintf(str, 8, "%d*%d", byte_code[VMstack[m]->StackFrame->pc+1].arg1, byte_code[VMstack[m]->StackFrame->pc+1].arg2);
    for(i=2;i<=index;i++)
    {  
        snprintf(buf, 8, "#%d*%d", byte_code[VMstack[m]->StackFrame->pc+i].arg1, byte_code[VMstack[m]->StackFrame->pc+i].arg2);
        strcat(str, buf);
    }
    printf("parallel:%s\n", str);

    push_stack(VMstack[m]->StackFrame->OpStack, VMstack[m]->StackFrame->pc);

    Pwait(PCB);
    Pfork(str, PCB->pid);
    //parallelSched();
   printf("sdsadds\n"); 
    return true;
}


// 字节码系统调用过程转换实现
int sim_syscall( int byte, int indexw, int m)
{
	operate_stack *OpStack = VMstack[m]->StackFrame->OpStack;
    //ArrayRef *arrayref = heap->arrayref;
    char byte_data[50];
    int index1,index2,index3;
    if(OpStack->top == -1)
    {
            intr_no = 1;
        return false;
    }
    
	int fd;
	
	int ref = VMstack[m]->StackFrame->OpStack->element[VMstack[m]->StackFrame->OpStack->top];
            VMstack[m]->StackFrame->OpStack->top--;
            read_array(ref, VMstack[m]->StackFrame->OpStack);
	        pthread_mutex_lock(&_lock);
            if((fd = open("/tmp/result.txt", O_RDWR|O_CREAT, S_IRWXU)) == -1)
            {
                printf("open error\n");
                return true;
            }
            if(write(fd, array_str, strlen(array_str)) != strlen(array_str))
            {
                printf("write error\n");
            }
            printf("result:%s\n", array_str);
	        pthread_mutex_unlock(&_lock);
            return true;
    
	uint32_t *word_data = (uint32_t *)byte_data;
        uint32_t sysno = *word_data++;
        int32_t nr_args = ospf::syscall::get_syscall_nr_args(sysno);
        uint32_t nr_data = 1;
    
        
        if (nr_args == -1) {
		printf("系统调用参数错误!\n");
                return false;
		// ........
        }
        if(nr_args == 2)
        {
        	snprintf(byte_data ,4, "%d", OpStack->element[OpStack->top]);
        	OpStack->top--;
        	index1 = OpStack->element[OpStack->top];
        	read_array(index1, OpStack);
        	int len = strlen(array_str);
        	char buf[20];
        	snprintf(buf, 4, "%d", len);
        	strcat(byte_data, buf);
        	strcat(byte_data, array_str);
        }
        if(nr_args == 3)
        {
        	index1 = OpStack->element[OpStack->top];
        	snprintf(byte_data ,4, "%d", index1);
        	OpStack->top--;
        	char buf1[20];
        	strcat(byte_data, buf1);
        	index2 = OpStack->element[OpStack->top];
        	OpStack->top--;
        	read_array(index1, OpStack);
        	int len = strlen(array_str);
        	char buf2[20];
        	snprintf(buf2, 4, "%d", len);
        	strcat(byte_data, buf2);
        	strcat(byte_data, array_str);
        	index3 = OpStack->element[OpStack->top];
        	OpStack->top--;
        }
        
	// 该 if 处理含有参数的系统调用
	if (nr_args > 0) {
        	if (sysno == SYS_EXIT) {							// exit
			uint32_t num;

        		num = *word_data++;

                	uint32_t count = (sizeof(uint32_t)-1) / sizeof(uint32_t);	
                	nr_data += count;

			uint32_t *args = new uint32_t[nr_args];
			args[0] = num;
                	
			ospf::syscall::syscall(sysno, args);
			
			delete []args;

		}else if (sysno == SYS_OPEN) {							// open
			uint32_t flag;
			uint8_t *buffer = NULL;
			uint32_t len;
        		
			len = *word_data++;
			flag = *word_data++;
                	buffer = new uint8_t[len];
                
                	uint32_t count = (len + sizeof(uint32_t)-1) / sizeof(uint32_t);	
                	uint32_t *pbuff = (uint32_t *)buffer;
                	for (uint32_t i = 0; i < count; ++i) {
                		pbuff[i] = *word_data++;
                	}
                	
                	nr_data += count;
                	
			uint32_t *args = new uint32_t[nr_args];
			args[0] = (uint32_t)buffer;
			args[1] = flag;
                                
			ospf::syscall::syscall(sysno, args);
			if (buffer) {
				delete []buffer;
			        buffer = NULL;
			}
			delete []args;

        	} else if (sysno == SYS_CLOSE) {						// close
			uint32_t fileno;

        		fileno = *word_data++;

                	uint32_t count = (sizeof(uint32_t)-1) / sizeof(uint32_t);	
                	nr_data += count;

			uint32_t *args = new uint32_t[nr_args];
			args[0] = fileno;
                	
			ospf::syscall::syscall(sysno, args);
			
			delete []args;

        	} else if (sysno == SYS_WRITE) {						// write
			uint32_t fileno;
			uint8_t *buffer = NULL;
			uint32_t len;

        		fileno = *word_data++;
        		len = *word_data++;
                	buffer = new uint8_t[len];
                
                	uint32_t count = (len + sizeof(uint32_t)-1) / sizeof(uint32_t);	
                	uint32_t *pbuff = (uint32_t *)buffer;
                	for (uint32_t i = 0; i < count; ++i) {
                		pbuff[i] = *word_data++;
                	}
                	
                	nr_data += count;
			
			uint32_t *args = new uint32_t[nr_args];
			args[0] = index3;
			args[1] = (uint32_t)array_str;
			args[2] = index1;        
                	
			ospf::syscall::syscall(sysno, args);
			if (buffer) {
				delete []buffer;
			        buffer = NULL;
			}
			delete []args;

        	} else if (sysno == SYS_READ) {							// read
			uint32_t fileno;
			uint8_t *buffer = NULL;
			uint32_t len;
        		
			fileno = *word_data++;
        		len = *word_data++;
                	buffer = new uint8_t[len];
                
                	uint32_t count = (len + sizeof(uint32_t)-1) / sizeof(uint32_t);	
                	uint32_t *pbuff = (uint32_t *)buffer;
                	for (uint32_t i = 0; i < count; ++i) {
                		pbuff[i] = *word_data++;
                	}
                	
                	nr_data += count;
                	
			uint32_t *args = new uint32_t[nr_args];
			args[0] = fileno;
			args[1] = (uint32_t)buffer;
			args[2] = len;
                                
			ospf::syscall::syscall(sysno, args);
			if (buffer) {
				delete []buffer;
				buffer = NULL;
			}
			delete []args;

		} else if (sysno == SYS_KILL) {							// kill
			uint32_t pid;
			uint32_t operate;

        		pid = *word_data++;
			operate = *word_data++;

                	uint32_t count = (sizeof(uint32_t)-1) / sizeof(uint32_t);	
                	nr_data += count;

			uint32_t *args = new uint32_t[nr_args];
			args[0] = pid;
			args[1] = operate;
                	
			ospf::syscall::syscall(sysno, args);
			
			delete []args;

        	} else if (sysno == SYS_MALLOC) {						// malloc
			uint32_t size;

        		size = *word_data++;

                	uint32_t count = (sizeof(uint32_t)-1) / sizeof(uint32_t);	
                	nr_data += count;

			uint32_t *args = new uint32_t[nr_args];
			args[0] = size;
                	
			ospf::syscall::syscall(sysno, args);
			
			delete []args;

        	} else if (sysno == SYS_FREE) {							// free
			uint8_t *buffer = NULL;
			uint32_t len;

        		len = *word_data++;
                	buffer = new uint8_t[len];
                
                	uint32_t count = (len + sizeof(uint32_t)-1) / sizeof(uint32_t);	
                	uint32_t *pbuff = (uint32_t *)buffer;
                	for (uint32_t i = 0; i < count; ++i) {
                		pbuff[i] = *word_data++;
                	}
                	
                	nr_data += count;
			
			uint32_t *args = new uint32_t[nr_args];
			args[0] = (uint32_t)buffer;
			args[1] = len;        
                	
			ospf::syscall::syscall(sysno, args);
			if (buffer) {
				delete []buffer;
			        buffer = NULL;
			}
			delete []args;

        	} else if (sysno == SYS_CREAT) {							// creat
			uint32_t mode;
			uint8_t *buffer = NULL;
			uint32_t len;
        		
			len = *word_data++;
        		mode = *word_data++;
                	buffer = new uint8_t[len];
                
                	uint32_t count = (len + sizeof(uint32_t)-1) / sizeof(uint32_t);	
                	uint32_t *pbuff = (uint32_t *)buffer;
                	for (uint32_t i = 0; i < count; ++i) {
                		pbuff[i] = *word_data++;
                	}
                	
                	nr_data += count;
  

			uint32_t *args = new uint32_t[nr_args];
			args[0] = (uint32_t)buffer;
			args[1] = mode;
                                
			ospf::syscall::syscall(sysno, args);
			if (buffer) {
				delete []buffer;
				buffer = NULL;
			}
			delete []args;

        	} else if (sysno == SYS_LSEEK) {							// lseek
			uint32_t handle;
			uint32_t offset;
			uint32_t fromwhere;
        		
			handle = *word_data++;
        		offset = *word_data++;
                	fromwhere = *word_data++;
                
                	uint32_t count = (sizeof(uint32_t)-1) / sizeof(uint32_t);	
                	
                	nr_data += count;
                	
			uint32_t *args = new uint32_t[nr_args];
			args[0] = handle;
			args[1] = offset;
			args[2] = fromwhere;
                                
			ospf::syscall::syscall(sysno, args);
			
			delete []args;

			//...
		}// TODO else
        	
        } else {
                ospf::syscall::syscall(sysno, NULL);
        }

        // nr_data + sysno
        return (nr_data + 1) * sizeof(uint32_t);

	return true;
}

//



//}       // namespace Simulate

      // namespace ospf


