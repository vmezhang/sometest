#include "mm_strc.h"

char str[30];
int  nums;
int free_size;
int  page_size = 4096;  //页大小
int  mm_size = 25*4096;    //总内存大小
void *mm_addr;
int  algorithm = 4;



struct page_dir* page_dir_array; //页目录
struct buddy_dir*buddy_array_addr;//伙伴结构首地址
