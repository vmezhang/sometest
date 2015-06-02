/*
 * =====================================================================================
 *
 *       Filename:  read_conf.h
 *
 *    Description:  获取配置文件的头文件
 *
 *        Version:  1.0
 *        Created:  2015年05月22日 13时06分30秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef __READ_CONF_H__
#define __READ_CONG_H__

#include <stdio.h>  
#include <string.h>   
#include <stdlib.h>   
#include <libxml/xmlmemory.h>   
#include <libxml/parser.h> 

//从配置文件中获取虚拟cpu数量
char *getCpuNum(char *docname); 
//从配置文件中获取调度算法的类型
char *getSchedType(char *docname);

//从配置文件中获取内存分配算法的类型
char *getMemType(char *docname);

#endif

