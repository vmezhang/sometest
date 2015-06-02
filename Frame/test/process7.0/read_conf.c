/*
 * =====================================================================================
 *
 *       Filename:  read_conf.c
 *
 *    Description:  读取配置文件中的信息
 *
 *        Version:  1.0
 *        Created:  2015年05月22日 11时50分21秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "read_conf.h" 
 
//从配置文件中获取虚拟cpu数量
int getCpuNum(char *docname)   
{   
    xmlDocPtr doc;    
    xmlNodePtr cur;  
    char *cpuNum;
    int cpu_num;

    doc = xmlParseFile(docname);      
    if (doc == NULL )  
    {  
        fprintf(stderr,"Document not parsed successfully. \n");    
        return;   
    }     
    cur = xmlDocGetRootElement(doc);      
    if (cur == NULL)  
    {  
        fprintf(stderr,"empty document\n");     
        xmlFreeDoc(doc);  
        return;    
    }   
    while (cur != NULL)   
    {   
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"osep")))  
        {  
             cur = cur->xmlChildrenNode;    
	     while (cur != NULL)   
	     {      
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"cpu_num")))
		{
			cpuNum = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);  
			cpu_num = atoi(cpuNum);
			xmlFree(cpuNum); 
			return cpu_num;
		}
		cur = cur->next;
	     }
	}    
    }      
    xmlFreeDoc(doc);    
    return;  
}   

//从配置文件中获取调度算法的类型
char *getSchedType(char *docname)   
{   
    xmlDocPtr doc;    
    xmlNodePtr cur;  
    char *schedType;

    doc = xmlParseFile(docname);      
    if (doc == NULL )  
    {  
        fprintf(stderr,"Document not parsed successfully. \n");    
        return;   
    }     
    cur = xmlDocGetRootElement(doc);      
    if (cur == NULL)  
    {  
        fprintf(stderr,"empty document\n");     
        xmlFreeDoc(doc);  
        return;    
    }   
    while (cur != NULL)   
    {   
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"osep")))  
        {  
             cur = cur->xmlChildrenNode;    
             while (cur != NULL)   
	     {      
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"proc_sched")))
		{
			schedType = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);  
			return schedType;
			xmlFree(schedType);  
		}
		cur = cur->next;
	     }
	}    
    }      
    xmlFreeDoc(doc);    
    return;  
}   
  
//从配置文件中获取内存分配算法的类型
char *getMemType(char *docname)   
{   
    xmlDocPtr doc;    
    xmlNodePtr cur;  
    char *memType;

    doc = xmlParseFile(docname);      
    if (doc == NULL )  
    {  
        fprintf(stderr,"Document not parsed successfully. \n");    
        return;   
    }     
    cur = xmlDocGetRootElement(doc);      
    if (cur == NULL)  
    {  
        fprintf(stderr,"empty document\n");     
        xmlFreeDoc(doc);  
        return;    
    }   
    while (cur != NULL)   
    {   
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"osep")))  
        {  
	     cur = cur->xmlChildrenNode;    
	     while (cur != NULL)   
	     {      
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"mem_alloc")))
		{
			memType = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);  
			return memType;
			xmlFree(memType);  
		}
		cur = cur->next;
	     }
	}    
    }      
    xmlFreeDoc(doc);    
    return;  
}    
   

