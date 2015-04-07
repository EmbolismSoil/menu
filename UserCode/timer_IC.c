#include "timer_IC.h"

  static TIM_HandleTypeDef    TimHandle;
  static TIM_IC_InitTypeDef      sICConfig;

  TIM_HandleTypeDef *TIMIC_GetTimHandle(void)
  {
     return &TimHandle;
  }

  TIM_IC_InitTypeDef *TIMIC_GetsICConfig(void)
  {
     return &sICConfig;
  }

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  __TIM1_CLK_ENABLE();

  /* Enable GPIO channels Clock */
   __GPIOA_CLK_ENABLE();

  /* Configure  (TIMx_Channel) in Alternate function, push-pull and 100MHz speed */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*##-2- Configure the NVIC for TIMx #########################################*/

  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 2, 1);

  /* Enable the TIM4 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);


}


