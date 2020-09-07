#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"


void TIM2_Init(u16 arr,u16 psc);

void TIM3_Cap_Init(u16 ifreq,u16 psc);


#endif


