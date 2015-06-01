#ifndef   MM
#define   MM

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>


extern char str[];
extern int  nums;
extern int free_size;
extern int  page_size;  //页大小
extern int  mm_size;    //总内存大小
extern void *mm_addr;
extern int  algorithm;

extern struct page_dir* page_dir_array; //页目录
extern struct buddy_dir *buddy_array_addr; //伙伴头

/*
 *分页管理:
 *         sys_list_head，user_list_head，记录系统，用户的内存头尾
 *         sys_page_list，user_page_list，分页管理，建立分页树，作为节点描述页状态
 */

/*
 *系统内存结构
 */
struct sys_list_head
{
	struct sys_page_list *s_used_head;
        struct sys_page_list *s_used_tail;
        struct sys_page_list *s_free_head;
        struct sys_page_list *s_free_tail;
        struct process_mm  *s_p_head;
        struct process_mm  *s_p_tail;
        int  sys_free_sizes;
        int  sys_used_sizes;
};

/*
 *用户内存结构
 */
struct  user_list_head
{
	struct user_page_list *u_used_head;
        struct user_page_list *u_used_tail;
        struct user_page_list *u_free_head;
        struct user_page_list *u_free_tail;
        struct process_mm *u_p_head;
        struct process_mm *u_p_tail;
        int  user_free_sizes;
        int  user_used_sizes;
};

/*
 *系统页描述
 */
struct sys_page_list
{
	struct  sys_page_list *next;
        struct  sys_page_list *prev;
        int size;
        int flag;
};

/*
 *用户页描述
 */
struct user_page_list
{
	struct user_page_list *next;
        struct user_page_list *prev;
        int size;
        int flag;
};




char*  get_n(int,char[]);
int    judge_n(int );
int    gettime(char *);
void   s_strcat(char *, const char *);
void   mm_log(char *);

void*  mm_apply (void);
int    mm_page (void *mm_addr,struct page_dir**);
void   chose_arith(void *mm_addr);

void   creat_buddy (void *);
void*  find_buddy (int );
int    free_buddy (void *);

int    init(void);
void*  smalloc(int);
int    sfree(void *);
void   *exchange_true(void *);
void   *exchange_false(void *);
#endif
