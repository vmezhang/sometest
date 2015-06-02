// =====================================================================================
//
//       Filename:  Syscalls.cpp
//
//    Description:  系统调用相关
//
// =====================================================================================

#include <map>

#include <stdint.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Syscalls.h"
#include "Globalcall.h"

namespace ospf {

namespace syscall {

// exit 系统调用
void sys_exit(uint32_t num)
{
	printf("\n\n in exit() and num:%d \n\n", num);
        exit(num);
}

// open 系统调用
static int sys_open(uint8_t *fileno, uint32_t operate)
{
	if (fileno == NULL) {
		// error
	}
	return open((const char *)fileno, operate);
}

// close 系统调用
static int sys_close(uint32_t fileno)
{
	if (fileno == ' ') {
		// error
	}
	return close(fileno);
}

// read 系统调用
static uint8_t *sys_read(uint32_t fileno, uint8_t *buffer, uint32_t len)
{
	uint32_t temp;

        if (fileno == STDIN_FILENO) {
                temp = read(STDIN_FILENO, buffer, len);
        } else {
		temp = read(fileno, buffer, len);
	}
//	printf("\nread返回值实际长度 = %d\nread参数缓冲区长度 = %d\n", temp, len);
	
	if ((int32_t)temp - (int32_t)len > 0) {
		// 系统 read 缓冲区溢出
		printf("read 缓冲区溢出!\n");
		return 0;
	}
		

	return buffer;
}

// write 系统调用
static int sys_write(uint32_t fileno, uint8_t *buffer, uint32_t len)
{
	int temp = 0;

        if (fileno == STDOUT_FILENO) {
                temp = write(STDOUT_FILENO, buffer, len);
//		printfp(buffer);
        } else {
		temp = write(fileno, buffer, len);
	}
        
	return temp;
}
/*
// fork 系统调用
void sys_pfork(uint8_t *str, uint32_t ppid)
{

	Pfork(str, ppid);
}

// fork 系统调用
void sys_wait(uint8_t *str, uint32_t ppid)
{

	Pfork(str, ppid);
}
*/
// malloc 系统调用
void* sys_malloc(uint32_t size)
{
	if (size == 0) {
		printf("申请空间为空!\n");
	}
	return malloc(size);
}

// free 系统调用
void sys_free(void *space)
{
	if (space == NULL) {
		printf("要释放的空间不存在!\n");
		return;
	}
	free(space);
}

// kill 系统调用
void sys_kill(uint32_t pid, uint32_t op)
{
	if (pid > 0) {
		kill(pid, op);
	} else {
		printf("该进程无法删除!\n");
	}
}

// creat 系统调用
static int sys_creat(uint8_t *filename, uint32_t mode)
{
	printf("filename=%s\nmode=%d\n", filename, mode);
	return creat((const char *)filename, mode);
}

// lseek 系统调用
static int sys_lseek(uint32_t handle, uint32_t offset, uint32_t fromwhere)
{
	return (int)lseek(handle, offset, fromwhere);
}


bool syscall(uint32_t sysno, uint32_t *args)
{
	uint8_t *readbuf = (uint8_t *)args[1];
        switch (sysno) {
                case SYS_EXIT:
			sys_exit((uint32_t)args[0]);
                        break;
                case SYS_OPEN:
			Return = sys_open((uint8_t *)args[0], (uint32_t)args[1]);
			printf("fd = %d\n", Return);
                        break;
                case SYS_CLOSE:
			Return = sys_close((uint32_t)args[0]);
                        break;
                case SYS_READ:
			readbuf = sys_read((uint32_t)args[0], (uint8_t *)args[1], (uint32_t)args[2]);
                        printf("always read buf:%s\n", readbuf);
			break;
                case SYS_WRITE:
			Return = sys_write((uint32_t)args[0], (uint8_t *)args[1], (uint32_t)args[2]);
                        break;
//                case SYS_FORK:
//			sys_fork();
//                        break;
                case SYS_KILL:
			sys_kill((uint32_t)args[0], (uint32_t)args[1]);
                        break;
                case SYS_MALLOC:
			ReturnVoid = sys_malloc((uint32_t)args[0]);
			break;
                case SYS_FREE:
			sys_free((uint8_t *)args[0]);
                        break;
                case SYS_CREAT:
			Return = sys_creat((uint8_t *)args[0], (uint32_t)args[1]);
                        break;
                case SYS_LSEEK:
			sys_lseek((uint32_t)args[0], (uint32_t)args[1], (uint32_t)args[2]);
                        break;
                default:
                        // Error sysno
                        break;
        }
        
        return true;
}

// 根据系统调用号返回该系统调用参数个数
int32_t get_syscall_nr_args(uint32_t sysno)
{
        // 系统调用号和参数个数的映射关系
/*        std::map<uint32_t, int32_t> sys_nr_args_map = {
                {SYS_EXIT, 1},

                {SYS_OPEN, 2},
                {SYS_CLOSE, 1},
                {SYS_READ, 3},
                {SYS_WRITE, 3},

//                {SYS_FORK, 0},
                {SYS_KILL, 2},

                {SYS_MALLOC, 1},
                {SYS_FREE, 1},

                {SYS_CREAT, 2},
                {SYS_LSEEK, 3}
        };
*/	int sys_nr_args_map[100] = {0};
	sys_nr_args_map[SYS_EXIT] = 1;
	sys_nr_args_map[SYS_OPEN] = 2;
	sys_nr_args_map[SYS_CLOSE] = 1;
	sys_nr_args_map[SYS_READ] = 3;
	sys_nr_args_map[SYS_WRITE] = 3;
	sys_nr_args_map[SYS_KILL] = 2;
	sys_nr_args_map[SYS_MALLOC] = 1;
	sys_nr_args_map[SYS_FREE] = 1;
	sys_nr_args_map[SYS_CREAT] = 2;
	sys_nr_args_map[SYS_LSEEK] = 3;

//        if (sys_nr_args_map.find(sysno) != sys_nr_args_map.end()) {
                return sys_nr_args_map[sysno];
//        }

        return -1;
}

} // namespace syscall

} // namespace ospf
