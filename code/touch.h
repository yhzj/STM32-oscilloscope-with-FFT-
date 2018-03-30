#ifndef __TOUCH_H__
#define __TOUCH_H__
//T_IRQ PA1  T_DIN PB5 T_DO PB4 T_CLK PB3 T_CS PD2
#define  tcs1 GPIO_SetBits(GPIOD,GPIO_Pin_2)
#define  tcs0 GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define  tclk1  GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define  tclk0  GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define  tdin1  GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define  tdin0  GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define  CMD_RDX  0xD0	 
#define  CMD_RDY  0x90
extern int touchx,touchy;
void Touch_ini(void);
int Read_ADS(int *x,int *y);
#endif
