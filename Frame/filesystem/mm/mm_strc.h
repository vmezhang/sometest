#ifndef  MM_STRC
#define MM_STRC


/*
 *页表项
 */
struct  page_table_entry
{
	void *page_addr;	
};

/*
 *页目录
 */
struct page_dir
{
	void * start_addr;   //页表起始地址
	void * start_true_addr;
	struct page_table_entry * page_table_addr;
};


/*
 *伙伴指针数组
 */
struct  buddy_array
{
	int flag;	
	void* page_head;
	struct buddy_array *prev;
        struct buddy_array *next;
};

/*
 *伙伴目录
 */
struct buddy_dir
{
	struct buddy_array *head;
};

#endif
