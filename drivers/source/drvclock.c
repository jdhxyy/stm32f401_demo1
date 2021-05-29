// Copyright 2019-2019 The TZIOT Authors. All rights reserved.
// 时钟驱动
// Authors: jdh99 <jdh821@163.com>

#include "drivers.h"

// 定时器
#define CLOCK_TIM TIM2
#define CLOCK_TIM_IRQn TIM2_IRQn
#define CLOCK_TIM_IRQHandler TIM2_IRQHandler
#define CLOCK_RCC_APBPeriphClockCmd RCC_APB1PeriphClockCmd
#define CLOCK_RCC_APBPeriph_TIM RCC_APB1Periph_TIM2

// 溢出值,对应的是周期.单位: s
#define CLOCK_CYCLE 1

// 定时器周期溢出回调函数
static TZEmptyFunc gCallbackTimer = 0;

static void initTimer(void);

// DrvClockInit 初始化
void DrvClockInit(void) {
    initTimer();
}

static void initTimer(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = CLOCK_TIM_IRQn;              
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TZ_IRQ_PRIORITY_HIGHEST;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             
    NVIC_Init(&NVIC_InitStructure);                             

    CLOCK_RCC_APBPeriphClockCmd(CLOCK_RCC_APBPeriph_TIM, ENABLE);
    TIM_DeInit(CLOCK_TIM);
    TIM_InternalClockConfig(CLOCK_TIM);

    TIM_TimeBaseInitTypeDef timInitStruct;
    // 预分频
    timInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  
    // 计数频率为1us跳转1次 			
    timInitStruct.TIM_Prescaler = SystemCoreClock / 2 / 1000000 - 1;         
    // 向上计数  				
    timInitStruct.TIM_CounterMode = TIM_CounterMode_Up; 	
    timInitStruct.TIM_RepetitionCounter = 0;
    // 这个值实际上就是TIMX->ARR，延时开始时重新设定即可	
    timInitStruct.TIM_Period = ((uint32_t)CLOCK_CYCLE * 1000000) - 1; 								
    // 初始化定时器
    TIM_TimeBaseInit(CLOCK_TIM, &timInitStruct);
    // 清溢出中断标志  
    TIM_ClearFlag(CLOCK_TIM, TIM_FLAG_Update);  
    // 禁止ARR预装载缓冲器  
    TIM_ARRPreloadConfig(CLOCK_TIM, DISABLE);  
    // 计数溢出时触发中断
    TIM_ITConfig(CLOCK_TIM, TIM_IT_Update, ENABLE); 					
    // 开启计数器
    TIM_Cmd(CLOCK_TIM, ENABLE); 				 						
}

void CLOCK_TIM_IRQHandler(void) {  
    if (TIM_GetITStatus(CLOCK_TIM, TIM_IT_Update) == SET) {  
        TIM_ClearITPendingBit(CLOCK_TIM, TIM_FLAG_Update);
        if (gCallbackTimer != 0) {
            gCallbackTimer();
        }
    }
}

// DrvClockGetCounter 得到定时器的当前计数值
uint32_t DrvClockGetCounter(void) {
    return TIM_GetCounter(CLOCK_TIM);
}

// DrvClockSetCallbackTimerOverflow 设置定时器溢出的中断回调函数
void DrvClockSetCallbackTimerOverflow(TZEmptyFunc callback) {
    gCallbackTimer = callback;
}
