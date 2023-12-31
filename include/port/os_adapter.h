/* os_adapter.h
*
* 操作系统适配层，在此定义与目标操作系统相关的接口函数
*
* Neo-T, 创建于2022.03.15 13:53
* 版本: 1.0
*
*/
#ifndef OS_ADAPTER_H
#define OS_ADAPTER_H

#include <rtdef.h>

#ifdef SYMBOL_GLOBALS
#define OS_ADAPTER_EXT
#else
#define OS_ADAPTER_EXT extern
#endif //* SYMBOL_GLOBALS


//* 一些协议栈要用到的与OS适配层相关的全局变量、函数、结构体定义
//* ==============================================================================================
typedef struct _STCB_PSTACKTHREAD_ { //* 协议栈内部工作线程控制块，其用于线程建立
  void(*pfunThread)(void *pvParam);
  void *pvParam;
  const CHAR *pszThreadName; //* 线程名称
  UINT unStackSize; //* 栈大小
  UCHAR ubPrio;     //* 线程优先级
  UINT unTimeSlice; //* 单次调度该线程能够运行的最长时间片
} STCB_PSTACKTHREAD, *PSTCB_PSTACKTHREAD;

//* ==============================================================================================

//* 一些协议栈要用到的需要OS提供的支撑函数
//* ==============================================================================================
OS_ADAPTER_EXT void os_sleep_secs(UINT unSecs);                       //* 休眠，单位：秒
OS_ADAPTER_EXT void os_sleep_ms(UINT unMSecs);                        //* 休眠，单位：毫秒
OS_ADAPTER_EXT UINT os_get_system_secs(void);                         //* 获取系统启动以来已运行的秒数（从0开始）
OS_ADAPTER_EXT UINT os_get_system_msecs(void);                        //* 获取系统启动以来已运行的毫秒数（从0开始）

OS_ADAPTER_EXT HMUTEX os_thread_mutex_init(void);                     //* 线程同步锁初始化，成功返回同步锁句柄，失败则返回INVALID_HMUTEX
OS_ADAPTER_EXT void os_thread_mutex_lock(HMUTEX hMutex);              //* 线程同步区加锁
OS_ADAPTER_EXT void os_thread_mutex_unlock(HMUTEX hMutex);            //* 线程同步区解锁
OS_ADAPTER_EXT void os_thread_mutex_uninit(HMUTEX hMutex);            //* 删除线程同步锁，释放该资源
OS_ADAPTER_EXT HSEM os_thread_sem_init(UINT unInitVal, UINT unCount); //* 信号量初始化，参数unInitVal指定初始信号量值， unCount指定信号量最大数值
OS_ADAPTER_EXT void os_thread_sem_post(HSEM hSem);                    //* 投递信号量
OS_ADAPTER_EXT INT os_thread_sem_pend(HSEM hSem, INT nWaitSecs);      //* 等待信号量到达，参数nWaitSecs指定要等待的超时时间（单位为秒）：0，一直等下去直至信号量到达，收到信号则返回值为0，出错则返回值为-1；其它，等待指定时间，如果指定时间内信号量到达，则返回值为0，超时则返回值为1，出错则返回值为-1
OS_ADAPTER_EXT void os_thread_sem_uninit(HSEM hSem);                  //* 信号量去初始化，释放该资源
OS_ADAPTER_EXT void os_thread_onpstack_start(void *pvParam);          //* 启动协议栈内部工作线程

extern rt_base_t rt_hw_interrupt_disable(void);
extern void rt_hw_interrupt_enable(register rt_base_t temp);
#define os_critical_init()    register rt_base_t temp;          //* 临界区初始化
#define os_enter_critical()   temp = rt_hw_interrupt_disable(); //* 进入临界区（关中断）
#define os_exit_critical()    rt_hw_interrupt_enable(temp);     //* 退出临界区（开中断）

#ifndef PKG_ONPS_BUDDY_MMU_EN
    extern void *rt_malloc(rt_size_t size);
    extern void rt_free(void *rmem);
    #define buddy_free(pvStart)     rt_free(pvStart)

#ifdef __RT_THREAD_H__
    #include "onps_errors.h"
    OS_ADAPTER_EXT void *buddy_alloc(UINT unSize, EN_ONPSERR *penErr);  //* 使用OS内核提供的内存
#endif //* #ifdef __RT_THREAD_H__
#endif //* #ifndef PKG_ONPS_BUDDY_MMU_EN

#ifdef PKG_ONPS_SUPPORT_PRINTF
  #define printf rt_kprintf
#endif //* #ifdef PKG_ONPS_SUPPORT_PRINTF

#endif

