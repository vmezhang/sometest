#include "mm_strc.h"
#include "mm.h"

int init(void)
{
	mm_addr = mm_apply( );
	if(NULL == mm_addr){
                return -1;
         }

	mm_page(mm_addr,&page_dir_array);
	chose_arith(mm_addr);

	return 0;
}

void *smalloc(int size)
{
   	 void* addr = NULL;

   	 if(size > free_size){
		printf("\n平台内存不足\n");
		exit(0);
	   }
	 switch(algorithm){
	
		/*case 1:  addr = allocate_mem(size);	break;
		case 2:  addr = allocate_mem(size);	break;
 		case 3:  addr = allocate_mem(size);	break;*/
		case 4:  addr = find_buddy(size);       break;
		default: algorithm = 4;
	        addr = find_buddy(size);       break;
   	 }
  	 addr = exchange_true(addr);
  	 return addr;
}

int sfree(void *free_addr)
{
     	 free_addr = exchange_false(free_addr);
  	 switch(algorithm){
	
		/*case 1:  free_allocate_mem(ab);	        break;
		case 2:  free_allocate_mem(ab);  	break;
	 	case 3:  free_allocate_mem(ab);	        break;*/
        	case 4:  free_buddy(free_addr);         break;
   	}
  	 return 0;
}

void  chose_arith(void *mm_addr)
{                            //  默认配置0,伙伴算法 chose_num 为全局变量
        switch(algorithm){
	     case 4:	 creat_buddy(mm_addr);   break;
	     default:    creat_buddy(mm_addr);   break;  
	}
}

void *exchange_true(void *change)
{
	int m;
	int n=0;
	while(n<(nums-1) && page_dir_array[n+1].start_addr <= change){
		n++;
	}
	
	m = (change - page_dir_array[n].start_addr)/page_size;
	return page_dir_array[n].page_table_addr[m].page_addr;
}

void *exchange_false(void *change)
{
	int n=0;
	void *temp;

	while(n<(nums-1) && page_dir_array[n+1].start_true_addr <= change){
		n++;
	}
	temp = (change - page_dir_array[n].start_true_addr) + page_dir_array[n].start_addr;
	return temp;
}


