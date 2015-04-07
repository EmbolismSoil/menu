
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
#include "fifo_ALLType.h"

/********************************************************************************************************
                                            Defines
*********************************************************************************************************/
#define Menu_Manager_Prio 5
#define Key_Manager_Prio 12
#define Frequency_Prio  8
#define Duty_Prio 9

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
static  OS_STK          FrequencyStk[200];
static  OS_STK          DutySTK[200];

 uint32_t            uwIC2Value = 0;
 uint32_t            uwDutyCycle = 0;
 uint32_t            uwFrequency = 0;
 uint32_t            tmpDuty = 0;

static double  Frequency;
static double freAry[20];
static double  tmpFre = 0;
static int flag = 0;
static int dflag = 0;
OS_EVENT *KeyBox;
OS_EVENT *FreBox;
fifo_t *pFIFO = NULL;
/***********************************************************************************************
                                    Declaretion
***********************************************************************************************/
static  void  App_TaskStart (void        *p_arg);
static void App_MenuManger(void *parg);
static void App_KeyManager(void *p_arg);
static void App_Frequency(void *parg);
static void App_DutyMeasure(void*);

void *FreinFunc(void* p)
{
   p = p;
   Menu_cursorOFF();
   Menu_CurMenu()->Buffer->opt = Dis;
   Menu_CurMenu()->Buffer->Next->opt = Dis;
   return NULL;
}

void *FreoutFunc(void *p)
{
    p = p;
    Menu_cursorON();
    Menu_CurMenu()->Buffer->opt = NODis;
    Menu_CurMenu()->Buffer->Next->opt = NODis;
    return NULL;
}

void *inFunc(void* p)
{
   p = p;
   Menu_cursorOFF();
   Menu_CurMenu()->Buffer->opt = Dis;
   Menu_CurMenu()->Buffer->Next->opt = Dis;
   HAL_TIM_IC_Start_IT(&PWMTimHandle, TIM_CHANNEL_2);
   HAL_TIM_IC_Start_IT(&PWMTimHandle, TIM_CHANNEL_1);
   return NULL;
}

void *outFunc(void *p)
{
    p = p;
    HAL_TIM_IC_Stop_IT(&PWMTimHandle, TIM_CHANNEL_2);
    HAL_TIM_IC_Stop_IT(&PWMTimHandle, TIM_CHANNEL_1);
    Menu_cursorON();
    Menu_CurMenu()->Buffer->opt = NODis;
    Menu_CurMenu()->Buffer->Next->opt = NODis;
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

static char Frequency_Buf[35];
static char DutyBuf[4];

printBuffer_t *bp = Menu_GenPrintBufferList("Frequency : ",NODis,0,0,
                                  Menu_GenPrintBufferList(Frequency_Buf,NODis,1,0,NULL));

printBuffer_t *dutyBuffer = Menu_GenPrintBufferList("Duty : ",NODis,0,0,Menu_GenPrintBufferList(DutyBuf,NODis,1,0,NULL));

actFuncAndArg_t *pIN = Menu_GenActList(inFunc,NULL,NULL);
actFuncAndArg_t *pOUT = Menu_GenActList(outFunc,NULL,NULL);
actFuncAndArg_t *pFREIN = Menu_GenActList(FreinFunc,NULL,NULL);
actFuncAndArg_t *pFREOUT = Menu_GenActList(FreoutFunc,NULL,NULL);

int  main (void)
{

      HAL_Init();
      SystemClock_Config_84M();


      menu_t *mainMenu = Menu_NewMenu("Root",0,0,0,NULL,NULL,NULL,Updata,NULL);
      menu_t *Signals = Menu_NewMenu("1 Signals ", 0,1,1, mainMenu,
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
      Menu_NewMenu("1 Freq Measure",0,1, 5, Signals,
                     pFREIN,
                     pFREOUT,
                     Updata,bp);
      Menu_NewMenu("2 Duty Measure",1,1,6, Signals,pIN,pOUT,Updata,dutyBuffer);


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

     OSTaskCreateExt((void (*)(void *)) App_Frequency,
                             (void          * ) 0,
                             (OS_STK        * )&FrequencyStk[200 - 1],
                             (INT8U           ) Frequency_Prio,
                             (INT16U          ) Frequency_Prio,
                             (OS_STK        * )&FrequencyStk[0],
                             (INT32U          ) 200,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

          OSTaskCreateExt((void (*)(void *)) App_DutyMeasure,
                             (void          * ) 0,
                             (OS_STK        * )&DutySTK[200 - 1],
                             (INT8U           ) Duty_Prio,
                             (INT16U          ) Duty_Prio,
                             (OS_STK        * )&DutySTK[0],
                             (INT32U          ) 200,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

   /* Start multitasking (i.e. give control to uC/OS-II).  */

    KeyBox = OSMboxCreate(NULL);
    FreBox = OSMboxCreate(NULL);
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
    Timer_ICInit(TIM2,1,0xFFFF,0,0,UP,RISING,1);
    TimerBaseInit(TIM1,(8400-1),0,2500 - 1 ,UP);
    PWMICInit();

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
    User_GPIOEXTI(GPIOA,15,GPIO_MODE_IT_FALLING,FAST,PULLUP,5,1);
    User_GPIOEXTI(GPIOA,4, GPIO_MODE_IT_FALLING,FAST,PULLUP,5,2);
    User_GPIOEXTI(GPIOB,0, GPIO_MODE_IT_FALLING,FAST,PULLUP,5,3);
    User_GPIOEXTI(GPIOC,2, GPIO_MODE_IT_FALLING,FAST,PULLUP,5,4);

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
            OSTimeDly(30);
            if (!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)){
                 Key = PRE;
                 OSMboxPost(KeyBox,&Key);
             }
        }else if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)){
              OSTimeDly(30);
             if (!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)){
                    Key = NEXT;
                   OSMboxPost(KeyBox,&Key);
             }
        }else if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)){
               OSTimeDly(30);
             if (!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)){
                    Key = ENTER;
                    OSMboxPost(KeyBox,&Key);
             }
        }else if (!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)){
                OSTimeDly(30);
              if (!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)){
                    Key = BACK;
                   OSMboxPost(KeyBox,&Key);
             }
        }
    }


}

/*
*********************************************************************************************************
*                                          App_Frequency()
*
* Description : 频率测量进程 该进程只能被菜单条目Frequency唤醒，唤醒后即可读取消息邮箱取得中断发送的原始频率
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
void App_Frequency(void *parg)
{
/***********************************************
                误差拟合系数
************************************************
     static const  double p1 =   -9.16e-09;
     static const double p2 =       1.013;
     static const double p3 =      0.2876 ;

     static const double p1 =  -1.316e-05  ;
     static const double p2 =       1.281  ;(1.254, 1.309)
     static const double p3  =      -933.7 ; (-1208, -659.4)
******************************************************/


    static const double a =       1.015 ;// (1.005, 1.025)
    static const double b =      0.9998  ;//(0.9988, 1.001)
    static const double c =     -0.4801 ;// (-5.049, 4.089)
     while(1)
      {
          OSTaskSuspend(Frequency_Prio);
        if (flag){
          tmpFre *= 4;
          tmpFre = a*pow(tmpFre,b) + c;
          sprintf(Frequency_Buf,"%d",(int)tmpFre);
          Menu_RefreshBuffer();
          flag = 0;
        }
      }
}


/*********************************************************************************************************
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
static  void  App_DutyMeasure (void *p_arg)
{
    p_arg = p_arg;

         while(1)
      {
          OSTaskSuspend(Duty_Prio);
        if (dflag){
           sprintf(DutyBuf,"%d %",(int)uwDutyCycle + 1);
           OSTimeDly(800);
          Menu_RefreshBuffer();
          dflag = 0;
        }
      }
}



/**************************************************************************************
                                  中断响应函数
*************************************************************************************/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
  {
    uwIC2Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
    if (uwIC2Value != 0){
      uwDutyCycle = ((HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1)) * 100) / uwIC2Value;
    }
    else
      uwDutyCycle = 0;
      OSTaskResume(Duty_Prio);
    dflag = 1;
  }else  Frequency++;

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
   static int num = 0;

    if (num <= 19)
    {
        freAry[num] = Frequency;
        num++;

    }else{
       tmpFre = 0;
       QuickSort(freAry,0,19);
       for (num = 5;num <= 14;num++)
           tmpFre += freAry[num];
       tmpFre /= 10.0;
       num = 0;
       flag =  1;
      OSTaskResume(Frequency_Prio);
    }
    Frequency = 0;
}

void HAL_GPIO_EXTI_Callback ( uint16_t GPIO_Pin)
{
   OSTaskResume(Key_Manager_Prio);
}
