#include "timer_IC.h"
#define    Error_Handler()  while(1)

/***************************************************************************
                         defines
**************************************************************************/
#define TIMx                           TIM4
#define TIMx_CLK_ENABLE()              __TIM4_CLK_ENABLE()

/* Definition for TIMx Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __GPIOB_CLK_ENABLE()
#define GPIO_PORT                      GPIOB
#define GPIO_PIN_CHANNEL2              GPIO_PIN_7
#define GPIO_AF_TIMx                   GPIO_AF2_TIM4

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                      TIM4_IRQn
#define TIMx_IRQHandler                TIM4_IRQHandler

/*******************************************************************
                       Local
******************************************************************/
  static TIM_HandleTypeDef    TimHandle;
  static TIM_IC_InitTypeDef   sICConfig;

  TIM_HandleTypeDef        PWMTimHandle;
  TIM_IC_InitTypeDef       PWMsConfig;
  TIM_SlaveConfigTypeDef   sSlaveConfig;

/*****************************************************************
                       Functions
******************************************************************/

  TIM_HandleTypeDef *TIMIC_GetTimHandle(void)
  {
     return &TimHandle;
  }

  TIM_IC_InitTypeDef *TIMIC_GetsICConfig(void)
  {
     return &sICConfig;
  }



  void PWMICInit(void)
  {
      PWMTimHandle.Instance = TIMx;
      PWMTimHandle.Init.Period = 0xFFFF;
      PWMTimHandle.Init.Prescaler = 0;
      PWMTimHandle.Init.ClockDivision = 0;
      PWMTimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
      if(HAL_TIM_IC_Init(&PWMTimHandle) != HAL_OK)
      {
        Error_Handler();
      }

      PWMsConfig.ICPrescaler = TIM_ICPSC_DIV1;
      PWMsConfig.ICFilter = 0;

      PWMsConfig.ICPolarity = TIM_ICPOLARITY_FALLING;
      PWMsConfig.ICSelection = TIM_ICSELECTION_INDIRECTTI;
      if(HAL_TIM_IC_ConfigChannel(&PWMTimHandle, &PWMsConfig, TIM_CHANNEL_1) != HAL_OK)
      {
        Error_Handler();
      }


      PWMsConfig.ICPolarity = TIM_ICPOLARITY_RISING;
      PWMsConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
      if(HAL_TIM_IC_ConfigChannel(&PWMTimHandle, &PWMsConfig, TIM_CHANNEL_2) != HAL_OK)
      {
        /* Configuration Error */
        Error_Handler();
      }

      sSlaveConfig.SlaveMode     = TIM_SLAVEMODE_RESET;
      sSlaveConfig.InputTrigger  = TIM_TS_TI2FP2;
      if(HAL_TIM_SlaveConfigSynchronization(&PWMTimHandle, &sSlaveConfig) != HAL_OK)
      {

        Error_Handler();
      }
/*
      if(HAL_TIM_IC_Start_IT(&PWMTimHandle, TIM_CHANNEL_2) != HAL_OK)
      {
        Error_Handler();
      }

      if(HAL_TIM_IC_Start_IT(&PWMTimHandle, TIM_CHANNEL_1) != HAL_OK)
      {
        Error_Handler();
      }
*/
  }




void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  __TIM2_CLK_ENABLE();

  /* Enable GPIO channels Clock */
   __GPIOA_CLK_ENABLE();

  /* Configure  (TIMx_Channel) in Alternate function, push-pull and 100MHz speed */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*##-2- Configure the NVIC for TIMx #########################################*/

  HAL_NVIC_SetPriority(TIM2_IRQn, 2, 1);

  /* Enable the TIM4 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM2_IRQn);


    TIMx_CLK_ENABLE();

  /* Enable GPIO channels Clock */
  TIMx_CHANNEL_GPIO_PORT();

  /* Configure  (TIMx_Channel) in Alternate function, push-pull and 100MHz speed */
  GPIO_InitStruct.Pin = GPIO_PIN_CHANNEL2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF_TIMx;
  HAL_GPIO_Init(GPIO_PORT, &GPIO_InitStruct);

  /*##-2- Configure the NVIC for TIMx #########################################*/
  /* Sets the priority grouping field */
  HAL_NVIC_SetPriority(TIMx_IRQn, 8, 1);

  /* Enable the TIM4 global Interrupt */
  HAL_NVIC_EnableIRQ(TIMx_IRQn);
}


