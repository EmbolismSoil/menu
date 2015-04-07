#include "timer_Base.h"

  static TIM_HandleTypeDef    TimHandle;

  TIM_HandleTypeDef *TIMBase_GetTimHandle(void)
  {
     return &TimHandle;
  }


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  __TIM1_CLK_ENABLE();

  /*##-2- Configure the NVIC for TIMx #########################################*/

  HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 1, 1);

  /* Enable the TIM4 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}


