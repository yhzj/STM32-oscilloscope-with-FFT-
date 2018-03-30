#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
#include "stm32_dsp.h"
#include "ini.h"
#include "fft.h"
#include "LCD.h"
#include "math.h"
#include "touch.h"
int  wav_data [640];
int  cacefft_1[320];
int  cacewav_1[320];
int  cacefft_2[320];
int  cacewav_2[320];
u32  fre_data[1024];
//delay
void delay(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;  
      while(i--) ;    
   }
}
//MAIN
int main(void)
{
	int i=0,k=0,x=0,y=0,touchx=0,touchy=0,tune=0,fft_max=0,fft_num=0;
	char fft_fs=1,wav_fs=1;
	sys_ini();
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	delay(30000);
	
	Lcd_Init();
	LCD_Clear(0x0000);
	delay(3000);
	LCD_Clear(0x0000);
	delay(3000);
	LCD_ShowString(0,312,"LCD ...............OK!");
	delay(3000);
	
  	Adc1_Init();
 	LCD_ShowString(32,312,"adc ..............OK!");
 	delay(1000);
	//TIM1_Int_Init(1,1);    
	DMA_Configuration();    //DMA≥ı ºªØ
	LCD_Clear(0x0000);
	delay(3000);
	
	LCD_ShowString(224,128,"wav_fs=");
	putnums(224,64,wav_fs);
	LCD_ShowString(224,312,"fft_fs=");
	putnums(224,257,fft_fs);
	
	ADC_GetConversionValue(ADC1);
		
	while(1)
	{
		
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)==0)
		{
			Read_ADS(&touchx,&touchy);
			if((touchy<2000)&&(touchy>0))
			{
				touchx=4000;
				touchy=4000;
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)!=0);
				fft_fs++;
				if(fft_fs>20)
				{
					fft_fs=1;
				}
				LCD_ShowString(224,312,"fft_fs=");
				putnums(224,257,fft_fs);
			}
			if((touchy<3400)&&(touchy>2000))
			{
				touchx=4000;
				touchy=4000;
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)!=0);
				wav_fs++;
				if(wav_fs>32)
				{
					wav_fs=1;
				}
				LCD_ShowString(224,128,"wav_fs=");
				putnums(224,64,wav_fs);
			}
		}
		
		//print part 1
		while(DMA_GetFlagStatus(DMA1_FLAG_TC1)!=1);
	
		DMA_Cmd(DMA1_Channel1, DISABLE);
		for(i=0;i<640;i++)
		{
			wav_data[i]=ADC_Value[i*wav_fs];
		}
		for(i=0;i<1024;i++)
		{
			fre_data[i]=ADC_Value[i*fft_fs]<<16;
		}
		cr4_fft_1024_stm32(lBufOutArray,fre_data,NPT);    
		GetPowerMag();
		DMA_Cmd(DMA1_Channel1, ENABLE);
		DMA_ClearFlag(DMA1_FLAG_TC1);
		
		//FFT
		fft_max=lBufMagArray[3];
		for(i=4;i<320;i++)
		{
			if(lBufMagArray[i]>fft_max)
			{
				fft_max=lBufMagArray[i];
				fft_num=i;
			}
		}
		putnums(0,64,fft_max);
		fft_num=1000*fft_num*(500/fft_fs)/512-100;
		putnums(0,312,fft_num);
		LCD_ShowString(0,256,"Hz");
		y=319;
		for(k=0;k<320;k++)
		{
			x=x-lBufMagArray[k]/2-20;
			cacefft_2[k]=x;
			//write add
			Address_set(x,y,x,y);
			//write data
			RS1;
			LCD_Writ_Bus(0x07e0); 
			//
			x=239;
			y--;	
		}
		//
		
		//wave
		for(tune=1;tune<255;tune++)
		{
			if((wav_data[tune]<2080)&&(wav_data[tune]>2020))
			{
				if(wav_data[tune]>wav_data[tune-1])
				{
					break;
				}
			}
		}
		y=319;
		for(k=tune;k<320+tune;k++)
		{
			x=x-wav_data[k]/10+100;
			cacewav_2[k-tune]=x;
			//write add
			Address_set(x,y,x,y);
			//write data
			RS1;
			LCD_Writ_Bus(0x07e0);
			//
			x=180;
			y--;	
		}
		//

		//clear part 2
		y=319;
		for(k=0;k<320;k++)
		{
			//FFT
			Address_set(cacefft_1[k],y,cacefft_1[k],y);
			RS1;
			LCD_Writ_Bus(0x0000);
 			//wave	
			Address_set(cacewav_1[k],y,cacewav_1[k],y);
			RS1;
			LCD_Writ_Bus(0x0000);
			y--;	
		}
		//
		//
		
		//
		//print part 2
		while(DMA_GetFlagStatus(DMA1_FLAG_TC1)!=1);
	
		DMA_Cmd(DMA1_Channel1, DISABLE);
		for(i=0;i<640;i++)
		{
			wav_data[i]=ADC_Value[i*wav_fs];
		}
		for(i=0;i<1024;i++)
		{
			fre_data[i]=ADC_Value[i*fft_fs]<<16;
		}
		cr4_fft_1024_stm32(lBufOutArray,fre_data,NPT);    
		GetPowerMag();
		DMA_Cmd(DMA1_Channel1, ENABLE);
		DMA_ClearFlag(DMA1_FLAG_TC1);
		
		//FFT
		fft_max=lBufMagArray[3];
		for(i=4;i<320;i++)
		{
			if(lBufMagArray[i]>fft_max)
			{
				fft_max=lBufMagArray[i];
				fft_num=i;
			}
		}
		putnums(0,64,fft_max);
		fft_num=1000*fft_num*(500/fft_fs)/512-100;
		putnums(0,312,fft_num);
		LCD_ShowString(0,256,"Hz");
		y=319;
		for(k=0;k<320;k++)
		{
			x=x-lBufMagArray[k]/2-20;
			cacefft_1[k]=x;
			//write add
			Address_set(x,y,x,y);
			//write data
			RS1;
			LCD_Writ_Bus(0x07e0); 
			//
			x=239;
			y--;	
		}
		//
		
		//wave
		for(tune=1;tune<255;tune++)
		{
			if((wav_data[tune]<2080)&&(wav_data[tune]>2020))
			{
				if(wav_data[tune]>wav_data[tune-1])
				{
					break;
				}
			}
		}
		y=319;
		for(k=tune;k<320+tune;k++)
		{
			x=x-wav_data[k]/10+100;
			cacewav_1[k-tune]=x;
			//write add
			Address_set(x,y,x,y);
			//write data
			RS1;
			LCD_Writ_Bus(0x07e0);
			//
			x=180;
			y--;	
		}
		//
	
		//clear part 1
		y=319;
		for(k=0;k<320;k++)
		{
			//FFT
			Address_set(cacefft_2[k],y,cacefft_2[k],y);
			RS1;
			LCD_Writ_Bus(0x0000);
 			//wave
			Address_set(cacewav_2[k],y,cacewav_2[k],y);
			RS1;
			LCD_Writ_Bus(0x0000);
			y--;	
		}
		//
	}
}
