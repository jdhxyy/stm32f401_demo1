// Copyright 2019-2019 The TZIOT Authors. All rights reserved.
// ʱ��ģ��
// Authors: jdh99 <jdh821@163.com>

#ifndef CLOCK_H
#define CLOCK_H

#include "drivers.h"

// ClockLoad ģ������
void ClockLoad(void);

// GetLocalTimeUs ��õ�ǰʱ��.����:us
uint64_t GetLocalTimeUs(void);

// GetLocalTimeSecond ��ȡ��ǰʱ��.����:s
uint32_t GetLocalTimeSecond(void);

// DelayLoopUs us��ʱ
// ���������������ʱ.����ʱ�������Ȳ�����ʱ����ʱ,���������жϵȳ���
void DelayLoopUs(uint32_t us);

#endif
