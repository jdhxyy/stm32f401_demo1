// Copyright 2019-2019 The TZIOT Authors. All rights reserved.
// 时钟模块
// Authors: jdh99 <jdh821@163.com>

#ifndef CLOCK_H
#define CLOCK_H

#include "drivers.h"

// ClockLoad 模块载入
void ClockLoad(void);

// GetLocalTimeUs 获得当前时间.精度:us
uint64_t GetLocalTimeUs(void);

// GetLocalTimeSecond 获取当前时间.精度:s
uint32_t GetLocalTimeSecond(void);

// DelayLoopUs us延时
// 经过计算产生的延时.本延时函数精度不及定时器延时,但可用于中断等场合
void DelayLoopUs(uint32_t us);

#endif
