// Copyright 2019-2019 The TZIOT Authors. All rights reserved.
// tzbase������ͷ�ļ�
// Authors: jdh99 <jdh821@163.com>

#ifndef DRVCLOCK_H
#define DRVCLOCK_H

#include "def.h"

// DrvClockInit ��ʼ��
void DrvClockInit(void);

// DrvClockGetCounter �õ���ʱ���ĵ�ǰ����ֵ
uint32_t DrvClockGetCounter(void);

// DrvClockSetCallbackTimerOverflow ���ö�ʱ��������жϻص�����
void DrvClockSetCallbackTimerOverflow(TZEmptyFunc callback);

#endif
