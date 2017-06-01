#include "aliyun_iot_platform_sem.h"

/***********************************************************
* 函数名称: aliyun_iot_sem_init
* 描       述: 线程同步信号初始化
* 输入参数: ALIYUN_IOT_SEM_S*semaphore
* 输出参数:
* 返 回  值: 0：成功，
*          FAIL_RETURN：异常
* 说       明:
************************************************************/
int32_t aliyun_iot_sem_init(aliot_platform_sem_t *semaphore)
{
    semaphore->hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);

    return SUCCESS_RETURN;
}

/***********************************************************
* 函数名称: aliyun_iot_sem_destory
* 描       述: 线程同步信号资源释放
* 输入参数: ALIYUN_IOT_SEM_S*semaphore
* 输出参数:
* 返 回  值: 0：成功，
*          FAIL_RETURN：异常
* 说       明:
************************************************************/
int32_t aliyun_iot_sem_destory(aliot_platform_sem_t *semaphore)
{
    CloseHandle(semaphore->hSemaphore);
    return SUCCESS_RETURN;
}

/***********************************************************
* 函数名称: aliyun_iot_sem_gettimeout
* 描       述: 等待同步信号
* 输入参数: ALIYUN_IOT_SEM_S*semaphore
*          INT32 timeout_ms
* 输出参数:
* 返 回  值: 0：成功，
*          ERROR_NET_TIMEOUT：等待超时，
* 说       明: 等待同步信号，超时退出
************************************************************/
int32_t aliyun_iot_sem_gettimeout(aliot_platform_sem_t *semaphore, uint32_t timeout_ms)
{
    WaitForSingleObject(semaphore->hSemaphore, timeout_ms);
    return SUCCESS_RETURN;
}

/***********************************************************
* 函数名称: aliyun_iot_sem_post
* 描       述: 发送同步信号
* 输入参数: ALIYUN_IOT_SEM_S*semaphore
* 输出参数:
* 返 回  值:
* 说       明: 发送同步信号
************************************************************/
int32_t aliyun_iot_sem_post(aliot_platform_sem_t *semaphore)
{
    ReleaseSemaphore(semaphore->hSemaphore, 1, NULL);
    return SUCCESS_RETURN;
}

