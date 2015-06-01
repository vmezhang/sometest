#include "mm_strc.h"
#include "mm.h"

/*
 *从系统获取平台从内存
 */
void*  mm_apply(void)
{    

        if(mm_size <= 0){
                strcpy(str,"error:the 'mm_size' <= 0.\n ");
 //               mm_log(str);  
                return NULL;
         }
        mm_addr= (void *)malloc(mm_size);
	if(NULL == mm_addr) {
		strcpy(str,"error: apply system memory fail.\n ");
//		mm_log(str);		
		return NULL;
	}else{
		strcpy(str,"\nsuccess:apply system memory success\n\n ");
//		mm_log(str);
 		printf("mm_addr:  %p\n",mm_addr);
		free_size = mm_size;
     		return mm_addr;
	}
	
}
