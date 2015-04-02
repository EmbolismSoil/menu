#ifndef __USER_GPIO_H__
#define __USER_GPIO_H__

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"

#define User_GPIOInit(GPIOx,Pinx,_Mode,_Speed,PULL)\
do{\
    __##GPIOx##_CLK_ENABLE();\
   GPIO_InitTypeDef GPIO_InitStruct = \
   {\
       .Pin = GPIO_PIN_##Pinx,\
       .Mode = _Mode,\
       .Pull = GPIO_##PULL,\
       .Speed = GPIO_SPEED_##_Speed,\
   };\
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);\
}while(0)


#define User_GPIOEXTI(GPIOx,Pinx,_Mode,_Speed,PULL,PrmptPrio,SubPrio)\
do{\
    __##GPIOx##_CLK_ENABLE();\
   GPIO_InitTypeDef GPIO_InitStruct = \
   {\
       .Pin = GPIO_PIN_##Pinx,\
       .Mode = _Mode,\
       .Pull = GPIO_##PULL,\
       .Speed = GPIO_SPEED_##_Speed,\
   };\
   HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);\
  if (Pinx >= 5 && Pinx <= 9){ \
      HAL_NVIC_SetPriority(EXTI9_5_IRQn,PrmptPrio,SubPrio);\
       HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);\
  }else if (Pinx >= 10 && Pinx <= 15){\
      HAL_NVIC_SetPriority(EXTI15_10_IRQn,PrmptPrio,SubPrio);\
       HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);\
  }else{ \
      switch(Pinx){ \
      case 0: \
          HAL_NVIC_SetPriority(EXTI0_IRQn,PrmptPrio,SubPrio);\
          HAL_NVIC_EnableIRQ(EXTI0_IRQn);\
          break;\
      case 1:\
          HAL_NVIC_SetPriority(EXTI1_IRQn,PrmptPrio,SubPrio);\
          HAL_NVIC_EnableIRQ(EXTI1_IRQn);\
          break;\
      case 2:\
          HAL_NVIC_SetPriority(EXTI1_IRQn,PrmptPrio,SubPrio);\
          HAL_NVIC_EnableIRQ(EXTI1_IRQn);\
          break;\
      case 3:\
          HAL_NVIC_SetPriority(EXTI1_IRQn,PrmptPrio,SubPrio);\
          HAL_NVIC_EnableIRQ(EXTI1_IRQn);\
          break;\
      case 4:\
          HAL_NVIC_SetPriority(EXTI1_IRQn,PrmptPrio,SubPrio);\
          HAL_NVIC_EnableIRQ(EXTI1_IRQn);\
          break;\
      default:\
          break;\
      }\
  }\
}while(0);
#endif