// =====================================================================================
//
//       Filename:  Globalcall.h
//
//    Description:  系统调用全局变量
//
// =====================================================================================

#ifndef GLOBALCALLS_H_
#define GLOBALCALLS_H_

// 缓冲区大小
#define BUFFSIZE 1024

#include <stdint.h>

namespace ospf {
/*	typedef struct IO_mode {

		// 输入缓冲区
		uint8_t Inputbuffer[BUFFSIZE];
		
		// 当前输入缓冲区大小
		uint32_t InputSize = 0;

		// 输入标志位（1，表示请求输入数据  2，表示已经输入了数据  0，表示空闲状态）
		uint32_t Inputleap = 0;

		// 输入终端标志位（表示是哪个终端进行输入的）
		uint32_t Inputnum = 0;

		// 进程标志位（标志是哪个进程的输入 IO 支持）
		uint32_t InputPid = 0;

		// 标志是否操作完
		uint32_t Inputover = 0;



		// 输出缓冲区
		uint8_t Outputbuffer[BUFFSIZE];
		
		// 当前输出缓冲区大小
		uint32_t OutputSize = 0;

		// 输出标志位（1，表示请求输出数据  2，表示已经输出了数据  0，表示空闲状态）
		uint32_t Outputleap = 0;

		// 输出终端标志位（表示是向哪个终端进行输出）
		uint32_t Outputnum = 0;

		// 进程标志位（标志是哪个进程的 输出 IO 支持）
		uint32_t OutputPid = 0;

		// 标志是否操作完
		uint32_t Outputover = 0;

	} io_operate;

//	uint32_t NUM = 5;									// 表示终端个数

	io_operate io;

	uint32_t SLEEP_TIME = 5;								// 输入输出等待睡眠时间
*/
	uint8_t Returnbuffer[BUFFSIZE];								// 字符串返回值
	uint32_t Return;									// 整形返回值

	void *ReturnVoid;									// malloc 申请空类型
};

#endif  // GLOBALCALLS_H_
