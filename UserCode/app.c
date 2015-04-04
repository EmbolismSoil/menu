
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
#include "fifo_AllType.h"
#include "Key.h"

#define Menu_Manager_Prio 3
#define Key_Manager_Prio 8

#define KEY_RESOLVE(key) \
do{\
            switch(key) \
            {\
                case ENTER:\
                    Menu_Enter();\
                    break;\
                case BACK:\
                    Menu_Back();\
                    break;\
                case NEXT:\
                    Menu_Next();\
                     break;\
                case PRE:\
                    Menu_Pre();\
                    break;\
            }\
}while(0)

static  OS_STK          App_TaskStartStk[128];
static  OS_STK          MenuManagerStk[200];
static  OS_STK          KeyManagerStk[200];

static  void  App_TaskStart (void        *p_arg);
static void App_MenuManger(void *parg);
static void App_KeyManager(void *p_arg);

void *inFunc(void* p)
{
   p = p;
   Menu_cursorOFF();
   return NULL;
}

void *outFunc(void *p)
{
    p = p;
    Menu_cursorON();
    return NULL;
}

static void Updata(unsigned char x, unsigned char y,uchar align, Menu_Opt_t opt, char *printString)
{
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

fifo_t *FIFO;

printBuffer_t *bp = Menu_GenPrintBufferList("mesure 10kHZ....",Dis,0,0,Menu_GenPrintBufferList("0KHZ",Dis,2,0,NULL));
actFuncAndArg_t *pIN = Menu_GenActList(inFunc,NULL,NULL);
actFuncAndArg_t *pOUT = Menu_GenActList(outFunc,NULL,NULL);
int Line = 0;
int  main (void)
{

      HAL_Init();
      SystemClock_Config_84M();
      FIFO = FIFO_NewFIFO(100);

      menu_t *mainMenu = Menu_NewMenu("Root",0,0,0,NULL,NULL,NULL,Updata,NULL);
      menu_t *freMenu = Menu_NewMenu("1 Frequency", 0,1,1, mainMenu,
                     NULL,
                     NULL,
                     Updata,NULL);
     menu_t *DC =  Menu_NewMenu("2 DC Output",1,1, 2,mainMenu,
                     NULL,
                     NULL,
                     Updata,NULL);
      Menu_NewMenu("1 DC 3V",0,1, 7,DC,
                     NULL,
                     NULL,
                     Updata,NULL);
      Menu_NewMenu("3 Volatage",2,1,3, mainMenu,
                     NULL,
                     NULL,
                     Updata,NULL);
      Menu_NewMenu("4 Power +-12V", 3,1,4,mainMenu,
                     NULL,
                     NULL,
                     Updata,NULL);
      menu_t *_10KHZ = Menu_NewMenu("1 10KHZ",0,1, 5, freMenu,
                     pIN,
                     pOUT,
                     Updata,bp);
      menu_t *_20KHZ = Menu_NewMenu("2 20KHZ",1,1,6, freMenu,NULL,NULL,Updata,NULL);


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
                             (OS_STK        * )&MenuManagerStk[200 - 1],
                             (INT8U           ) Menu_Manager_Prio,
                             (INT16U          ) Menu_Manager_Prio,
                             (OS_STK        * )&MenuManagerStk[0],
                             (INT32U          ) 200,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
     OSTaskCreateExt((void (*)(void *)) App_KeyManager,
                             (void          * ) 0,
                             (OS_STK        * )&KeyManagerStk[200 - 1],
                             (INT8U           ) Key_Manager_Prio,
                             (INT16U          ) Key_Manager_Prio,
                             (OS_STK        * )&KeyManagerStk[0],
                             (INT32U          ) 200,
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


/*
*********************************************************************************************************
*                                          App_MenuManger()
*
* Description : 菜单管理进程 用于响应键盘操作
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
static void App_MenuManger(void *parg)
{
    parg = parg;
    User_GPIOInit(GPIOA,15,GPIO_MODE_INPUT,FAST,PULLUP);
    User_GPIOInit(GPIOB,7, GPIO_MODE_INPUT,FAST,PULLUP);
    User_GPIOInit(GPIOB,0, GPIO_MODE_INPUT,FAST,PULLUP);
    User_GPIOInit(GPIOC,2, GPIO_MODE_INPUT,FAST,PULLUP);

    Init_Lcd();
    Menu_Enter();
    int key = 0;
    while(1){
        OSTaskSuspend(Menu_Manager_Prio);
        if (!FIFO_isNull(FIFO)){
            FIFO_POP(FIFO,key);
            KEY_RESOLVE(key);
        }
    };
}

static void App_KeyManager(void *parg)
{
    parg = parg;
     /*选用下降沿触发键盘，就免去松手检测*/
    int Key = 0;
    while(1){

        if (!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)){
            OSTimeDly(2);
            if (!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)){
                 while(1){
                   if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2))
                       break;
                   OSTimeDly(100);
                 }
                    Key = PRE;
                  //  KEY_RESOLVE(Key);
                    FIFO_PUSH(FIFO,Key);
                    OSTaskResume(Menu_Manager_Prio);
             }
        }else if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)){
              OSTimeDly(2);
             if (!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)){
                 while(1){
                   if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0))
                       break;
                   OSTimeDly(100);
                 }
                    Key = NEXT;
                 //   KEY_RESOLVE(Key);
                  FIFO_PUSH(FIFO,Key);
                    OSTaskResume(Menu_Manager_Prio);
             }
        }else if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)){
               OSTimeDly(2);
             if (!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)){
                 while(1){
                   if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15))
                       break;
                   OSTimeDly(100);
                 }
                    Key = ENTER;
                   // KEY_RESOLVE(Key);
                    FIFO_PUSH(FIFO,Key);
                    OSTaskResume(Menu_Manager_Prio);
             }
        }else if (!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)){
                OSTimeDly(2);
              if (!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)){
                 while(1){
                   if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7))
                       break;
                   OSTimeDly(100);
                 }
                    Key = BACK;
                   // KEY_RESOLVE(Key);
                    FIFO_PUSH(FIFO,Key);
                   OSTaskResume(Menu_Manager_Prio);
             }
        }

       // OSTaskSuspend(Key_Manager_Prio);
    }


}

void HAL_GPIO_EXTI_Callback ( uint16_t GPIO_Pin)
{
   OSTaskResume(Key_Manager_Prio);
}
