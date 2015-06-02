/*
 * =====================================================================================
 *
 *       Filename:  fileinterface.h
 *
 *    Description:  文件对外提供的接口
 *
 *        Version:  1.0
 *        Created:  05/19/2015 01:31:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  kristychen (chenluwen), eallyclw@gmail.com
 *        Company:  where there is a will, there is a way
 *
 * =====================================================================================
 */
#ifndef __FILEINTERFACE_H__
#define __FILEINTERFACE_H__

#include "mksfs.h"
#include "file.h"
struct file_desc *file[SFS_INODE_SIZE];

int linit();
int lfopen(char *pathname, int flag);
int lfread(int fd, char *buffersize, int length);
int lfwrite(int fd, char *buffersize, int length);
int lfclose(int fd);
int laccess(char *pathname, int flag);
int lfstat(int fd, int *index);  
int lfseek(int fd, int pos);

int From_Host_To_MySFS(char *pathname, char *filename);  //从宿主文件系统到MySFS的文件系统
int From_MySFS_To_Host(char *filename, char *pathname);

#endif //__FILEINTERFACE_H__
