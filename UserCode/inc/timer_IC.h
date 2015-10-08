#ifndef __TIMER_IC_H__
#define __TIMER_IC_H__
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"



#define Timer_ICInit(TIMx,Channel,_Period,_Prescaler,_ClockDivision,\
       _CounterMode,_ICPolarity,_ICPrescaler)\
do{\
  TIMIC_GetTimHandle()->Instance = TIMx;\
  TIMIC_GetTimHandle()->Init.Period        = _Period;\
  TIMIC_GetTimHandle()->Init.Prescaler     = _Prescaler;\
  TIMIC_GetTimHandle()->Init.ClockDivision = _ClockDivision;\
  TIMIC_GetTimHandle()->Init.CounterMode   = TIM_COUNTERMODE_##_CounterMode;\
  HAL_TIM_IC_Init(TIMIC_GetTimHandle());\
  TIMIC_GetsICConfig()->ICPolarity  = TIM_ICPOLARITY_##_ICPolarity;\
  TIMIC_GetsICConfig()->ICSelection = TIM_ICSELECTION_DIRECTTI;\
  TIMIC_GetsICConfig()->ICPrescaler = TIM_ICPSC_DIV##_ICPrescaler;\
  TIMIC_GetsICConfig()->ICFilter    = 0;\
  HAL_TIM_IC_ConfigChannel(TIMIC_GetTimHandle(),  TIMIC_GetsICConfig(), TIM_CHANNEL_##Channel);\
      HAL_TIM_IC_Start_IT( TIMIC_GetTimHandle(), TIM_CHANNEL_##Channel);\
}while(0)

extern void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim);
extern   TIM_HandleTypeDef *TIMIC_GetTimHandle(void);
extern   TIM_IC_InitTypeDef *TIMIC_GetsICConfig(void);
extern  void PWMICInit(void);
  extern TIM_HandleTypeDef        PWMTimHandle;
  extern TIM_IC_InitTypeDef       PWMsConfig;
  extern TIM_SlaveConfigTypeDef   sSlaveConfig;

#endif