/*************************************************************************
	> File Name: error.c

	> Author: zhanglp

	> Mail: zhanglp92@gmail.com

	> Created Time: 2014年11月15日 星期六 19时59分43秒

	> Description: 
 ************************************************************************/

#include "pcl.h"
#include <string.h>
#include <stdio.h>

// 打印日志
unsigned char VarInfo;
//long LogFileNo;

FILE *ResFile;


#undef stderr
#undef stdout 

#define stderr ResFile
#define stdout ResFile


void CreateLogFile (void)
{
	char *filename[] = {
	
		#define LOG_FILE(s) s, 
		#include "confg/conf.h"
		#undef LOG_FILE
		NULL
	};

    ResFile = fopen (NULL == filename[0] ? "/tmp/result.txt" : filename[0], "a");
}

void CloseLogFile (void)
{
    fclose (ResFile);
}

/* 错误处理函数 */
void Error (Coord coord, const char *fmt, ...) 
{
    va_list ap;

    ErrorCount++;
    if (coord) {

        fprintf (stderr, "(%s, %d):", coord->filename, coord->ppline);
    }
    fprintf (stderr, "error:");
    
    va_start (ap, fmt);
    vfprintf (stderr, fmt, ap);
    fprintf (stderr, "\n");
    va_end (ap);
}

/* 致命的错误 */
void Fatal (const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    vfprintf (stderr, fmt, ap);
    fprintf (stderr, "\n");
    va_end (ap);
    /* 遇见此类错误直接退出 */
    exit (-1);
}

void INFO (int infoLevel, const char *fmt, ...)
{
    if (VarInfo >= infoLevel) {

        va_list ap;

        va_start (ap, fmt);
        fprintf (stdout, "INFO: ");
        vfprintf (stdout, fmt, ap);
        fprintf (stdout, "\n");
        va_end (ap);
    }
}

void PRINT (const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    fprintf (stdout, "INFO: ");
    vfprintf (stdout, fmt, ap);
    fprintf (stdout, "\n");
    va_end (ap);
}

/* 警告处理 */
void Warning (Coord coord, const char *fmt, ...) 
{
    va_list ap;

    WarningCount++;
    if (coord) {

        fprintf (stderr, "(%s, %d):", coord->filename, coord->ppline);
    }
    fprintf (stderr, "warning:");
    
    va_start (ap, fmt);
    vfprintf (stderr, fmt, ap);
    fprintf (stderr, "\n");
    va_end (ap);
}

#undef stderr
#undef stdout 
