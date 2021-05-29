// Copyright 2019-2019 The TZIOT Authors. All rights reserved.
// ʱ������
// Authors: jdh99 <jdh821@163.com>

#include "drivers.h"

// ��ʱ��
#define CLOCK_TIM TIM2
#define CLOCK_TIM_IRQn TIM2_IRQn
#define CLOCK_TIM_IRQHandler TIM2_IRQHandler
#define CLOCK_RCC_APBPeriphClockCmd RCC_APB1PeriphClockCmd
#define CLOCK_RCC_APBPeriph_TIM RCC_APB1Periph_TIM2

// ���ֵ,��Ӧ��������.��λ: s
#define CLOCK_CYCLE 1

// ��ʱ����������ص�����
static TZEmptyFunc gCallbackTimer = 0;

static void initTimer(void);

// DrvClockInit ��ʼ��
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
    // Ԥ��Ƶ
    timInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  
    // ����Ƶ��Ϊ1us��ת1�� 			
    timInitStruct.TIM_Prescaler = SystemCoreClock / 2 / 1000000 - 1;         
    // ���ϼ���  				
    timInitStruct.TIM_CounterMode = TIM_CounterMode_Up; 	
    timInitStruct.TIM_RepetitionCounter = 0;
    // ���ֵʵ���Ͼ���TIMX->ARR����ʱ��ʼʱ�����趨����	
    timInitStruct.TIM_Period = ((uint32_t)CLOCK_CYCLE * 1000000) - 1; 								
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(CLOCK_TIM, &timInitStruct);
    // ������жϱ�־  
    TIM_ClearFlag(CLOCK_TIM, TIM_FLAG_Update);  
    // ��ֹARRԤװ�ػ�����  
    TIM_ARRPreloadConfig(CLOCK_TIM, DISABLE);  
    // �������ʱ�����ж�
    TIM_ITConfig(CLOCK_TIM, TIM_IT_Update, ENABLE); 					
    // ����������
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

// DrvClockGetCounter �õ���ʱ���ĵ�ǰ����ֵ
uint32_t DrvClockGetCounter(void) {
    return TIM_GetCounter(CLOCK_TIM);
}

// DrvClockSetCallbackTimerOverflow ���ö�ʱ��������жϻص�����
void DrvClockSetCallbackTimerOverflow(TZEmptyFunc callback) {
    gCallbackTimer = callback;
}
