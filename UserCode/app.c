
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include "main.h"
#include "menu.h"
#include "clock.h"
#include "User_GPIO.h"
#include "LCD12864P.h"

static  OS_STK          App_TaskStartStk[128];
static  void  App_TaskStart (void        *p_arg);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

int  main (void)
{


    HAL_Init();    CPU_INT08U  os_err;

    SystemClock_Config_84M();


    /* Disable all ints until we are ready to accept them.  */
    CPU_IntDis();
    /* Initialize "uC/OS-II, The Real-Time Kernel".         */
    OSInit();
   /* Create the start task.                               */
    os_err = OSTaskCreateExt((void (*)(void *)) App_TaskStart,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[128 - 1],
                             (INT8U           ) APP_TASK_START_PRIO,
                             (INT16U          ) APP_TASK_START_PRIO,
                             (OS_STK        * )&App_TaskStartStk[0],
                             (INT32U          ) APP_TASK_START_STK_SIZE,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

   /* Start multitasking (i.e. give control to uC/OS-II).  */
    OSStart();

    return (0);
}

/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : 主进程，它是所有进程的父进程。进行相关初始化之后创建其它进程，之后被挂起，且不再唤醒。
*
* Argument(s) : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  App_TaskStart (void *p_arg)
{
    p_arg = p_arg;
    unsigned char row = 1;

    OS_CPU_SysTickInit();
    OSStatInit();

   User_GPIOEXTI(GPIOC,13,GPIO_MODE_IT_FALLING,FAST,NOPULL,5,5);
   User_GPIOInit(GPIOA,5,GPIO_MODE_OUTPUT_PP,FAST,PULLUP);

   Init_Lcd();
    while(1){
        OSTaskSuspend(APP_TASK_START_PRIO);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    };
}

void HAL_GPIO_EXTI_Callback ( uint16_t GPIO_Pin)
{
    /*选用下降沿触发键盘，就免去松手检测*/
    GPIO_Pin = GPIO_Pin;
    OSTaskResume(APP_TASK_START_PRIO);
}
