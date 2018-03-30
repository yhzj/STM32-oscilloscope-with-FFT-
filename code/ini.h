#ifndef __INI_H__
#define __INI_H__

extern u16 ADC_Value[20480];

void Driver_MCU_Init(void);
void GPIOA_ini(void);
void GPIOB_ini(void);
void GPIOC_ini(void);
void GPIOD_ini(void);
void uart_ini(void);
void send_data(u8 a);
void print(char *z);
void putnum(long int x);
void Adc1_Init(void);
void Adc2_Init(void);
void sys_ini(void);
void DMA_Configuration(void);
void TIM1_Int_Init(u16 arr,u16 psc);
#endif
