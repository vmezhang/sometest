// =====================================================================================
//
//       Filename:  Simulate.h
//
//    Description:  字节码解释器
//
//        Version:  1.0
//        Created:  2014年12月30日 14时56分28秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Qianyi.lh (liuhuan), qianyi.lh@alibaba-inc.com
//        Company:  Alibaba-Inc Aliyun
//
// =====================================================================================

#ifndef SIMULATE_H_
#define SIMULATE_H_

#include "byte_code.h"

// 字节码定义
/*enum byte_code_t
 * {
 * SIMU_ADD = 0x1,
 * SIMU_SUB = 0x2,
 * SIMU_SYSCALL = 0x3
 * };
 * */

#define BYTE_CODE_MAX 0xff
#define _USE_GNU         //启用CPU_ZERO等相关的宏

//#define _GNU_SOURSCE

bool simulate(LinkVMstack _VMstack,ByteCode *bytecode, int mm);
  
/*ThreadPool threadPool1("OSPF Runtime")
 * ThreadPool threadPool2("OSPF Runtime");
 * ThreadPool threadPool3("OSPF Runtime");
 * ThreadPool threadPool4("OSPF Runtime");
 * ThreadPool threadPool5("OSPF Runtime");
 * */

#endif // SIMULATE_H_
