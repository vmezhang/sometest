// =====================================================================================
//
//       Filename:  byte_code.h
//
//    Description:  字节码解释
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


#ifndef __BYTE_CODE_H__
#define __BYTE_CODE_H__


#define OP_STACK 100  
#define LV_TABLE 100  
#define FRAME_NUM 20  

//#define true 1
//#define false 0

#include "proc.h"

typedef struct operatestack_node
{
    int element[OP_STACK];
    int top;
}operate_stack;
//typedef operate_stack *OperateStack;

typedef struct  stackframe_node
{
    int pc;
    operate_stack *OpStack;   
    struct stackframe_node *next;
}stack_frame;
//typedef stack_frame *StackFrame;

typedef struct vmstack_node
{
    stack_frame *StackFrame;  
    struct vmstack_node *next;
}LinkVMstackNode;
typedef LinkVMstackNode *LinkVMstack;

typedef struct array_node
{
    int *array_data; 
    char *carray_data;
    int ret;  
    struct array_node *next;
}ArrayRef;

typedef struct heap_node
{

    int localvariable_table[LV_TABLE]; 
    ArrayRef *arrayref;
    struct heap_node *next;
}heap_area;
typedef heap_area *HeapArea;


extern struct ProcStruct *PCB;

typedef struct byte_codes
{
    int code;
    int arg1;
    int arg2;
}ByteCode;

extern ByteCode *byte_code;

#endif //__BYTE_CODE_H__
