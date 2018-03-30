#include "touch.h"
#include "stm32f10x.h"	
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
int touchx=0,touchy=0;

void Touch_ini(void)
{	 
	tcs1;
	tclk1;
	tdin1;
	tclk1;
}
void spi3_write(u8 TxData)
{      
	u8 count=0;
	for(count=0;count<8;count++)
	{
		if((TxData&0x80)==0x80)
		{
			tdin1;
		}
		else
		{
			tdin0;
		}
		tclk0;
		tclk1;
		TxData=TxData<<1;
	}
}
u16 ADS_Read_AD(unsigned char CMD)          
{
	u8 count=0;
	u16 num=0,a=450;
	tclk0;
	tdin0;
	tcs0;
	spi3_write(CMD);
	while(a--);
	tclk0;
	a=72;
	while(a--);
	tclk1;
	tclk0;
	for(count=0;count<16;count++)
	{
		tclk0;
		num<<=1;
		tclk1;
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)==1) num++;
	}
	num>>=4;
	tcs1;
	return num;
}		   
#define READ_TIMES 15 
#define LOST_VAL 5	  
u16 ADS_Read_XY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
int Read_ADS(int *x,int *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	 									   
	if(xtemp<100||ytemp<100)return 0;
	*x=xtemp;
	*y=ytemp;
	return 1;
}
