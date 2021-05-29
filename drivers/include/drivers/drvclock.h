// Copyright 2019-2019 The TZIOT Authors. All rights reserved.
// tzbase适配器头文件
// Authors: jdh99 <jdh821@163.com>

#ifndef DRVCLOCK_H
#define DRVCLOCK_H

#include "def.h"

// DrvClockInit 初始化
void DrvClockInit(void);

// DrvClockGetCounter 得到定时器的当前计数值
uint32_t DrvClockGetCounter(void);

// DrvClockSetCallbackTimerOverflow 设置定时器溢出的中断回调函数
void DrvClockSetCallbackTimerOverflow(TZEmptyFunc callback);

#endif
