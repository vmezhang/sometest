/*
 * =====================================================================================
 *
 *       Filename:  string.h
 *
 *    Description:  string库函数的定义
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
 
#ifndef __STRING_H__
#define __STRING_H__

#include "types.h"
#include "defs.h"

size_t strlen(const char *s);
size_t strnlen(const char *s, size_t len);

char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, size_t len);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

void *lmemset(void *s, char c, size_t n);

#endif /* !__STRING_H__ */

