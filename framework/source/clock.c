// Copyright 2019-2019 The TZIOT Authors. All rights reserved.
// ʱ��ģ��
// Authors: jdh99 <jdh821@163.com>

#include "framework.h"

#define TAG "LOG_CLOCK"

// �ض�ʱ��.�����ǰusֵ�����ʱ����,��Ҫ���¶�ȡ��ֵ,��ֹ��Ϊ�жϽ�λ������ֵ�仯
#define READ_AGAIN_US 300
// ͳ��������
#define STATISTICS_RUN_TIME "run_time"

// ͳ����id
static int gStatisticsIdRunTime = -1;

static uint32_t gTimeSecond = 0;

static void clockIrqHandle(void);

// ClockLoad ģ������
void ClockLoad(void) {   
    //gStatisticsIdRunTime = TZStatisticsRegister(STATISTICS_RUN_TIME);
    DrvClockSetCallbackTimerOverflow(clockIrqHandle);
}

static void clockIrqHandle(void) {
    gTimeSecond++;
    //TZStatisticsAdd(gStatisticsIdRunTime);
}

// GetLocalTimeUs ��õ�ǰʱ��.����:us
uint64_t GetLocalTimeUs(void) {
    // �ȶ�ȡ�벿��,���us��λ,�����¶�ȡ�벿��
    uint32_t second = gTimeSecond;
    uint32_t us = DrvClockGetCounter();
    if (us < READ_AGAIN_US) {
        second = gTimeSecond;
    }
    
    return second * 1000000ll + us;
}

// GetLocalTimeSecond ��ȡ��ǰʱ��.����:s
uint32_t GetLocalTimeSecond(void) {
    return gTimeSecond;
}

// DelayLoopUs us��ʱ
// ���������������ʱ.����ʱ�������Ȳ�����ʱ����ʱ,���������жϵȳ���
void DelayLoopUs(uint32_t us) {
//    for (uint32_t i = 0; i < us; i++) {
//        uint32_t count = (2 * SYSTEM_CLOCK - 46) / 10;
//        for (uint8_t i = 0; i < count; i++) {}
//    }
}
