
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
#include <stdio.h>

#define Menu_Manager_Prio 12

static  OS_STK          App_TaskStartStk[128];
static  OS_STK          MenuManagerStk[200];


static  void  App_TaskStart (void        *p_arg);
static void App_MenuManger(void *parg);
void *act(void* a)
{
  a = a;
  Init_Lcd();
  User_printf(0,0,Align,"Hello act");
  return NULL;
}

actFuncAndArg_t Act =
{
    .Func = act,
    .Arg = NULL,
    .NextFunc = NULL,
};


static void Updata(unsigned char x, unsigned char y,uchar align, Menu_Opt_t opt, char *printString)
{
    y += 1;
    if (opt == CLEAR)
        Init_Lcd();
    else if (opt == Dis)
        User_printf(x,y,align,printString);
};
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


      HAL_Init();

      //  SystemClock_Config_84M();


      menu_t *mainMenu = Menu_NewMenu("Root",0,NULL,NULL,NULL,Updata,NULL);
      menu_t *freMenu = Menu_NewMenu("1 Frequency", 1, mainMenu,
                     &Act,
                     NULL,
                     Updata,NULL);
      Menu_NewMenu("2 DC Output", 2,mainMenu,
                     NULL,
                     NULL,
                     Updata,NULL);
      Menu_NewMenu("3 Volatage",3, mainMenu,
                     NULL,
                     NULL,
                     Updata,NULL);
      Menu_NewMenu("4 Power +-12V", 4,mainMenu,
                     NULL,
                     NULL,
                     Updata,NULL);
      Menu_NewMenu("1 10KHZ", 5, freMenu,
                     NULL,
                     NULL,
                     Updata,NULL);
      Menu_NewMenu("2 20KHZ", 6, freMenu,NULL,NULL,Updata,NULL);
    /* Disable all ints until we are ready to accept them.  */
    CPU_IntDis();
    /* Initialize "uC/OS-II, The Real-Time Kernel".         */
    OSInit();
   /* Create the start task.                               */
    OSTaskCreateExt((void (*)(void *)) App_TaskStart,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[128 - 1],
                             (INT8U           ) APP_TASK_START_PRIO,
                             (INT16U          ) APP_TASK_START_PRIO,
                             (OS_STK        * )&App_TaskStartStk[0],
                             (INT32U          ) APP_TASK_START_STK_SIZE,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    OSTaskCreateExt((void (*)(void *)) App_MenuManger,
                             (void          * ) 0,
                             (OS_STK        * )&MenuManagerStk[128 - 1],
                             (INT8U           ) Menu_Manager_Prio,
                             (INT16U          ) Menu_Manager_Prio,
                             (OS_STK        * )&MenuManagerStk[0],
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

    OS_CPU_SysTickInit();
    OSStatInit();
        while(1){
            OSTaskSuspend(APP_TASK_START_PRIO);
        }
}

static void App_MenuManger(void *parg)
{
    parg = parg;
    User_GPIOEXTI(GPIOC,13,GPIO_MODE_IT_FALLING,FAST,NOPULL,5,5);
    User_GPIOInit(GPIOA,5,GPIO_MODE_OUTPUT_PP,FAST,PULLUP);

    Init_Lcd();
    Menu_Enter();
    Menu_Enter();
    Menu_Back();
    Menu_Enter();
    Menu_Back();
    Menu_Next();
    Menu_Pre();
   // Menu_Enter();
    while(1){
        OSTaskSuspend(Menu_Manager_Prio);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    };
}

void HAL_GPIO_EXTI_Callback ( uint16_t GPIO_Pin)
{
    /*选用下降沿触发键盘，就免去松手检测*/
    GPIO_Pin = GPIO_Pin;
    OSTaskResume(Menu_Manager_Prio);

}
