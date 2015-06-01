#include "mm_strc.h"
#include "mm.h"

/*
 *内存分页
 */
int  mm_page(void *mm_addr,struct page_dir** page_dir_array)
{
        int times;
        int dir_num;
	int page_num=0;
        struct page_table_entry *page_table = NULL;
 
        nums = sqrt(mm_size/page_size); //mm_size必须是页整数倍的平方
 //       printf("mm_addr %p mm_size %d nums:  %d\n",mm_addr,mm_size,nums);
        if(nums < 2){
		printf("mm_size is not enough\n");
            	exit(0);
        }

        *page_dir_array = (struct page_dir *)calloc(nums,sizeof(struct page_dir));
        if(NULL == page_dir_array){
		printf("mm_page:apply buddy dir fail\n");
            	return -1;
	}

        dir_num = 0;
        while(dir_num < nums){
//		printf("page_dir_num: %d \n",dir_num);
            	page_table = (struct page_table_entry*)calloc(nums,sizeof(struct page_table_entry));
            	if(NULL == page_table){ 
               		return -1;
            	}
            	times=0;
            	while(times < nums){ 
           		  page_table[times].page_addr = mm_addr + page_num*page_size;
//			  printf("	       page_table[%d]  %p\n",times,page_table[times].page_addr);

                	  times++;
                  	  page_num++;
	    	 }
		(*page_dir_array)[dir_num].start_addr = (void *)(dir_num * nums * page_size + 1);
		(*page_dir_array)[dir_num].start_true_addr = page_table[0].page_addr;
            	(*page_dir_array)[dir_num].page_table_addr = page_table;
//		printf("%p \n\n",(*page_dir_array)[dir_num].start_addr);
            	dir_num++;
        }
	 
        return 0;
}

