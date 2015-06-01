#include "mm_strc.h"
#include "mm.h"

/*
 *初始化平台内存伙伴结构
 */
void creat_buddy(void *mm_addr)
{
        int num;       
	int i=0,m;
	int j=0;
	char ch;
	void*  temp_mm_addr =(void *)1;
	char time_s[20],*times;
	
	struct buddy_array *p,*temp_one;
	
	time_s[20] = 0;

	buddy_array_addr = (struct buddy_dir*)calloc(9,sizeof(struct buddy_dir));
        times = get_n(nums*nums,time_s);
	for(m=0; m<9; m++){
		buddy_array_addr[m].head = NULL;
	}

        while((ch=times[i]) != '\0'){
               	num = (int)ch-48;   
		temp_one=(struct buddy_array*)malloc(sizeof(struct buddy_array));
		temp_one->flag = 0;
		temp_one->page_head =  temp_mm_addr;	
		temp_one->prev = NULL;
                temp_one->next = NULL;

		buddy_array_addr[num].head = temp_one;

		p = temp_one;
		temp_mm_addr += (int)pow(2,num)*page_size; 
		while(times[i+1] == ch){
			j++;
			temp_one=(struct buddy_array*)malloc(sizeof(struct buddy_array));
			temp_one->flag = 0;
			temp_one->page_head = temp_mm_addr;
			temp_one->prev = p; 
			temp_one->next = NULL;

			p = temp_one;
			temp_mm_addr += (int)pow(2,num)*page_size;
			i++;
 		}                               
		i++;
	}
}

void* find_buddy(int size)
{
	int i,j,num;
	void * ret = NULL;
	struct buddy_array *temp_prev;
	struct buddy_array *temp,*temp_one,*temp_two;

       //顺着伙伴算法找，找到改变结构体falg，没找到调用creat_buddy,
	num = judge_n(size);
	if(num<0 || num>8){
		return NULL;
	}

	temp = buddy_array_addr[num].head;
	while(temp != NULL && 1 == temp->flag){
		temp = temp->next;
	}
	
	if(temp != NULL && 0 == temp->flag){
		temp->flag = 1;
		ret = temp->page_head;
 	}else{  //链表没有符合大小的未使用的块，需要拆分
		for(i=num+1; i<9; i++){
			temp = buddy_array_addr[i].head;
			temp_prev = temp;

			while(temp != NULL && 1 == temp->flag){
				temp_prev = temp;
				temp = temp->next;
			}
			if(temp != NULL && 0 == temp->flag){ //分割大块
				j = i;
				while(j>num){
			 		temp_one=(struct buddy_array*)malloc(sizeof(struct buddy_array));
					if(NULL == temp_one){
						return NULL;
					}
	 				temp_one->flag = 0;
					temp_one->page_head = temp->page_head + page_size*((int)pow(2,j-1));
					temp_one->next = NULL;

					temp_two = buddy_array_addr[j-1].head;
					if(NULL == temp_two){
					
						temp->prev = NULL;
						buddy_array_addr[j-1].head = temp_one;
					}else{
						while(temp_two->next != NULL){
							temp_two = temp_two->next;
						}
						temp_one->prev = temp_two;
						temp_two->next = temp_one;	
					}
					j--;
				}
				//i链表去掉被分割的块结构体
				if( buddy_array_addr[i].head == temp){
					buddy_array_addr[i].head = temp->next;
				}else{
					temp_prev->next = temp->next;
				}
				
				//最后j == num的小块添加在 num 链表尾巴上
				temp_one->next = temp;
				temp->flag = 1;
				temp->prev = temp_one;
				temp->next = NULL;
				ret = temp->page_head;
				if(ret != NULL){
					i = 9;
				}
			}
                 }
	}

	if(ret != NULL){	
		free_size -= (int)pow(2,num)*page_size;
	}
	return ret;
}

int free_buddy(void *free_addr)
{
	int m,n;
	void *q;
	struct buddy_array *p,*temp,*temp_one;

	for(m=0; m<9; m++){
		temp = buddy_array_addr[m].head;
		while(temp != NULL){
			if(temp->page_head == free_addr){
				temp->flag = 0;
				free_size += (int)pow(2,m)*page_size; 
				n = m;				
				break;
			}
			temp = temp->next;
		}
		if(NULL == temp){
			continue;
		}
		while(n<8){
			p = buddy_array_addr[n].head;
			while(p != NULL){
				q = p->page_head;
				if ( ( q+(int)pow(2,n)*page_size == free_addr || q-(int)pow(2,n)*page_size  == free_addr)  && 0 == p->flag){
					if(q < free_addr){
						temp->page_head = q;	
					}
			
					//temp->page_head已经长大，从队伍里赶出
					if(NULL == temp->prev){
						buddy_array_addr[n].head = temp->next;
						if(temp->next != NULL){
							temp->next->prev = NULL;
						}
					}else{
						temp->prev->next = temp->next;
					}
					temp->next = temp->prev = NULL;
					//p被收购，p消亡
					if(NULL == p->prev){
						buddy_array_addr[n].head = p->next;	
						if(p->next != NULL){
							p->next->prev = NULL;
						}
					}else{
						p->prev->next = p->next;
					}
					free(p);
					break;
				}
				p = p->next;
			}
			if(NULL == p){  //没找到可以合并的块
				if(n != m){ 
					if(NULL == buddy_array_addr[n].head){
						buddy_array_addr[n].head = temp;
						temp->prev = NULL;
					}else{
						temp_one = buddy_array_addr[n].head;
						while(temp_one != NULL){
							temp_one = temp_one->next;
						}
						temp_one->next = temp;
						temp->prev = temp_one;
					}

					temp->next = NULL;
				}
				return 0;
			}else{
				n++;
			}
		 }
		if(8 == n){
			return 0;
		}
	}
	return -1;
}
