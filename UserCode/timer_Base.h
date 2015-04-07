#ifndef __TIMER_BASE_H__
#define __TIMER_BASE_H__
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"

#define TimerBaseInit(TIMx,_Prescaler,_ClockDivision,_Period,_CounterMode) \
do{\
  TIMBase_GetTimHandle()->Instance = TIMx;\
  TIMBase_GetTimHandle()->Init.Period        = _Period;\
  TIMBase_GetTimHandle()->Init.Prescaler     = _Prescaler;\
  TIMBase_GetTimHandle()->Init.ClockDivision = _ClockDivision;\
  TIMBase_GetTimHandle()->Init.CounterMode   = TIM_COUNTERMODE_##_CounterMode;\
  HAL_TIM_Base_Init(TIMBase_GetTimHandle());\
  HAL_TIM_Base_Start_IT(TIMBase_GetTimHandle());\
}while(0)

extern TIM_HandleTypeDef *TIMBase_GetTimHandle(void);
#endif