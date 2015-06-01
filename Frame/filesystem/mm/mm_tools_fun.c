#include "mm.h"

/*
 *根据平台内存总大小，页大小得到分页的总页数（为页整数倍）
 */
char*  get_n(int num,char times[])
{
	int i;
	int rem,res;
	char ch[1];
	  
        strcpy(times,"\0");
        if(num<=0){
		return NULL;
	}
	rem = num%2;
        num = num-rem;

	for(i=8; i>0&&num>0; i--){
		res = num%((int)pow(2,i));
		if(res == num){
                     continue;
		}
		ch[0] = i+'0';
		strcat(times,ch);
                num = res;
	}
        if(1==rem){
		ch[0] = '0';
		strcat(times,ch);
	}
	ch[0] = '\0';
        strcat(times,ch);
	return times;
}

/*
 *size = pow(2,n),返回 n
 *size < pow(2,n),返回 n
 *size > pow(2,n).返回 n+1
 */
int judge_n(int size)
{
	int rec=0;
	int res;
	if(size<=0){
		strcpy(str,"error:apply memory size <= 0\n");
		return -1;
	}

	if(size<page_size){
		return rec;
	}else{
		res = size%page_size;
		if(res==0){
			res= size/page_size;    //需要的页数
		}else{
			res= size/page_size+1;
		}
		while(res>pow(2,rec)){
			rec++;	
		}
		
	}	 
	return rec;
}

