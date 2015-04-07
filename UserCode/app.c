
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
#include "timer_IC.h"
#include "timer_Base.h"
#include "QuickSort.h"
#include <ucos_ii.h>
#include <math.h>


/********************************************************************************************************
                                            Defines
*********************************************************************************************************/
#define Menu_Manager_Prio 5
#define Key_Manager_Prio 12

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

/**********************************************************************************************
                                   Local variable
**********************************************************************************************/
static  OS_STK          App_TaskStartStk[128];
static  OS_STK          MenuManagerStk[200];
static  OS_STK          KeyManagerStk[200];

static double  Frequency;
static double freAry[20];
static int num = 0;
static double tmpFre = 0;
static int flag = 0;
OS_EVENT *KeyBox;

/***********************************************************************************************
                                    Declaretion
***********************************************************************************************/
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


printBuffer_t *bp = Menu_GenPrintBufferList("mesure 10kHZ....",Dis,0,0,Menu_GenPrintBufferList("0KHZ",Dis,2,0,NULL));
actFuncAndArg_t *pIN = Menu_GenActList(inFunc,NULL,NULL);
actFuncAndArg_t *pOUT = Menu_GenActList(outFunc,NULL,NULL);
int Line = 0;
int  main (void)
{

      HAL_Init();
      SystemClock_Config_168M();
      Timer_ICInit(TIM1,1,0xFFFF,0,0,UP,RISING,1);
      TimerBaseInit(TIM2,(8400-1),0,2500-1 ,UP);


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

    KeyBox = OSMboxCreate(NULL);
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
    User_GPIOEXTI(GPIOA,15,GPIO_MODE_IT_FALLING,FAST,PULLUP,5,5);
    User_GPIOEXTI(GPIOB,7, GPIO_MODE_IT_FALLING,FAST,PULLUP,5,5);
    User_GPIOEXTI(GPIOB,0, GPIO_MODE_IT_FALLING,FAST,PULLUP,5,5);
    User_GPIOEXTI(GPIOC,2, GPIO_MODE_IT_FALLING,FAST,PULLUP,5,5);

    Init_Lcd();
    Menu_Enter();
    int *Key;;
    INT8U ERR;

    while(1){
         Key = OSMboxPend(KeyBox,10000,&ERR);
         KEY_RESOLVE(*Key);
    };
}

/*
*********************************************************************************************************
*                                          App_KeyManager()
*
* Description :
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
static void App_KeyManager(void *parg)
{
    parg = parg;


    static  int Key = -1;

    while(1){
         OSTaskSuspend(Key_Manager_Prio);
        if (!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)){
            OSTimeDly(2);
            if (!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)){
                 Key = PRE;
                 OSMboxPost(KeyBox,&Key);
             }
        }else if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)){
              OSTimeDly(2);
             if (!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)){
                    Key = NEXT;
                   OSMboxPost(KeyBox,&Key);
             }
        }else if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)){
               OSTimeDly(2);
             if (!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)){
                    Key = ENTER;
                    OSMboxPost(KeyBox,&Key);
             }
        }else if (!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)){
                OSTimeDly(2);
              if (!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)){
                    Key = BACK;
                   OSMboxPost(KeyBox,&Key);
             }
        }
    }


}


void APP_Fre(void *parg)
{
   //  static const  double p1 =   -9.16e-09;
    //static const double p2 =       1.013;
    //static const double p3 =      0.2876 ;

    static const double a =       1.015 ;// (1.005, 1.025)
    static const double b =      0.9998  ;//(0.9988, 1.001)
    static const double c =     -0.4801 ;// (-5.049, 4.089)

   //    static const double p1 =  -1.316e-05  ;
   //    static const double p2 =       1.281  ;//(1.254, 1.309)
   //    static const double p3  =      -933.7 ;// (-1208, -659.4)
      while(1)
        if (flag){
                tmpFre *= 4;
                tmpFre = a*pow(tmpFre,b) + c;
                User_printf(0,0,Align,"%d ",(int)tmpFre);
                flag = 0;
        }
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
      /*couter*/
    Frequency++;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if (num <= 19)
    {
        freAry[num] = Frequency;
        num++;

    }else{
       tmpFre = 0;
    //   quickSort(freAry,6);
       QuickSort(freAry,0,19);
       for (num = 5;num <= 14;num++)
           tmpFre += freAry[num];
       tmpFre /= 10.0;
       flag = 1;
       num = 0;
    }

    Frequency = 0;
}

void HAL_GPIO_EXTI_Callback ( uint16_t GPIO_Pin)
{
   OSTaskResume(Key_Manager_Prio);
}

