#ifndef __FFT_H__
#define __FFT_H__

#define   NPT   1024
#include "stm32f10x.h"

extern u32 lBufOutArray[NPT];
extern u32 lBufMagArray[NPT];

void GetPowerMag(void);

#endif
