// Copyright 2019-2019 The TZIOT Authors. All rights reserved.
// 时钟模块
// Authors: jdh99 <jdh821@163.com>

#include "framework.h"

#define TAG "LOG_CLOCK"

// 重读时间.如果当前us值在这个时间内,需要重新读取秒值,防止因为中断进位导致秒值变化
#define READ_AGAIN_US 300
// 统计项名称
#define STATISTICS_RUN_TIME "run_time"

// 统计项id
static int gStatisticsIdRunTime = -1;

static uint32_t gTimeSecond = 0;

static void clockIrqHandle(void);

// ClockLoad 模块载入
void ClockLoad(void) {   
    //gStatisticsIdRunTime = TZStatisticsRegister(STATISTICS_RUN_TIME);
    DrvClockSetCallbackTimerOverflow(clockIrqHandle);
}

static void clockIrqHandle(void) {
    gTimeSecond++;
    //TZStatisticsAdd(gStatisticsIdRunTime);
}

// GetLocalTimeUs 获得当前时间.精度:us
uint64_t GetLocalTimeUs(void) {
    // 先读取秒部分,如果us进位,则重新读取秒部分
    uint32_t second = gTimeSecond;
    uint32_t us = DrvClockGetCounter();
    if (us < READ_AGAIN_US) {
        second = gTimeSecond;
    }
    
    return second * 1000000ll + us;
}

// GetLocalTimeSecond 获取当前时间.精度:s
uint32_t GetLocalTimeSecond(void) {
    return gTimeSecond;
}

// DelayLoopUs us延时
// 经过计算产生的延时.本延时函数精度不及定时器延时,但可用于中断等场合
void DelayLoopUs(uint32_t us) {
//    for (uint32_t i = 0; i < us; i++) {
//        uint32_t count = (2 * SYSTEM_CLOCK - 46) / 10;
//        for (uint8_t i = 0; i < count; i++) {}
//    }
}
