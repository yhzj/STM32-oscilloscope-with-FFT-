#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "ini.h"
#include "lcd.h"
u16 ADC_Value[20480];
//UART 2
//RCC
void Driver_MCU_Init(void)
{
	ErrorStatus HSEStartUpStatus;
	RCC_DeInit();
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
	RCC_HSICmd(DISABLE);
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);  
	if(HSEStartUpStatus == SUCCESS)
	{
      
		/* HCLK = SYSCLK 72 */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
		/* PCLK2 = HCLK   72*/
		RCC_PCLK2Config(RCC_HCLK_Div1); 
 
		/* PCLK1 = HCLK/2   36*/
		RCC_PCLK1Config(RCC_HCLK_Div2);
	
		RCC_PREDIV2Config(RCC_PREDIV2_Div5); //PLL2CLK = (HSE / 5) * 8 = 40 MHz
    
		RCC_PLL2Config(RCC_PLL2Mul_8);  //PLL2CLK = (HSE / 5) * 8 = 40 MHz
    
		RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2,RCC_PREDIV1_Div5);  // PREDIV1CLK = PLL2 / 5 = 8 MHz
    
		// Enable PLL2
		RCC_PLL2Cmd(ENABLE);
		while((RCC->CR & RCC_CR_PLL2RDY) == 0)
		{
		}
      
		/* PLL configuration: PLLCLK = PREDIV1 * 9 = 72 MHz */ 
		RCC_PLLConfig(RCC_PLLSource_PREDIV1,0x00200000);//超频了RCC_PLLMul_10

		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
 
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
 
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	
	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}
//GPIO
void GPIOB_ini()
{
	//LCD set and LED
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_DeInit(GPIOB);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure SPI2 pins:, SCK and MOSI */   //VS1053 SPI2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14 ;//vsMISO
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//VS_DREQ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//PB10 VS_REST
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//PB12 pin: VS_XCS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure SPI3 pins:, SCK and MOSI */    //touch SPI3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//T_MISO
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}
void GPIOC_ini()
{
	//LCD DATA Line
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_DeInit(GPIOC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void GPIOA_ini()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_DeInit(GPIOA);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//VS_XDCS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//UART
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure SPI1 pins:, SCK, MISO and MOSI */  //SD_SPI1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 ;//MISO应该要初始化为带上拉的输入 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	//PA4 pin:SD_CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //GPIO_Pin_0|GPIO_Pin_1| //dac使用
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//PA1 pin:T_IRQ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
}
void GPIOD_ini()
{
	//SPI3 TOUCH_CS
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_DeInit(GPIOD);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
//UART
void uart_ini()
{	
	USART_InitTypeDef USART_InitStrue;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_InitStrue.USART_BaudRate=115200;
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStrue.USART_Parity=USART_Parity_No;
	USART_InitStrue.USART_StopBits=USART_StopBits_1;
	USART_InitStrue.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART2,&USART_InitStrue);
	USART_Cmd(USART2,ENABLE);
	
}
void send_data(u8 a)
{ 
	int flag1=0;
	while(flag1==0)
	{
		flag1=USART_GetFlagStatus(USART2,USART_FLAG_TXE);
	}
	    
	USART_SendData(USART2,a);
	USART_ClearFlag(USART2,USART_FLAG_TXE);						
}
void print(char *z)
{ 
	while(*z)
	{
		send_data(*z);
		z++;
	}
}
void putnum(long int x)
{
	int ge=0,shi=0,bai=0,qian=0,wan=0,shiwan=0,baiwan=0;
	ge=(x%10);
	shi=((((x%100000)%10000)%1000)%100)/10;
	bai=(((x%100000)%10000)%1000)/100;
	qian=((x%100000)%10000)/1000;
	wan=(x%100000)/10000;
	shiwan=x/100000;
	baiwan=x/1000000;
	send_data(baiwan+48);
	send_data(shiwan+48);
	send_data(wan+48);
	send_data(qian+48);
	send_data(bai+48);
	send_data(shi+48);
	send_data(ge+48);
}
//ADC
void  Adc2_Init(void)
{ 
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC2	, ENABLE );	
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   
           
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//deita_E_DAC
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC2);  

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);	

	ADC_Cmd(ADC2, ENABLE);
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);	
	ADC_RegularChannelConfig(ADC2,ADC_Channel_1,1, ADC_SampleTime_1Cycles5 );
	
	//ADC_DMACmd(ADC2, ENABLE);
	
	ADC_ResetCalibration(ADC2);	 
	while(ADC_GetResetCalibrationStatus(ADC2));	
	ADC_StartCalibration(ADC2);	
	while(ADC_GetCalibrationStatus(ADC2));	
}
void  Adc1_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   
	            
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1); //将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC 工作模式:ADC1 和 ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //模数转换工作在扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //模数转换工作在连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ADC_ExternalTrigConv_None;//ADC_ExternalTrigConv_T1_CC1; //Timer1触发转换开启
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC 数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1; //顺序进行规则转换的 ADC 通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_DMACmd(ADC1, ENABLE);// 开启 ADC 的 DMA 支持（要实现 DMA 功能，还需独立配置 DMA 通道等参数）
	ADC_Cmd(ADC1, ENABLE);	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1, ADC_SampleTime_1Cycles5 );	
	//ADC_ExternalTrigConvCmd(ADC1, ENABLE);   //外部触发
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
	
	ADC_ResetCalibration(ADC1);	 
	while(ADC_GetResetCalibrationStatus(ADC1));	
	ADC_StartCalibration(ADC1);	
	while(ADC_GetCalibrationStatus(ADC1));	
}

void DMA_Configuration(void)
{
		
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Channel1); //将 DMA 的通道 1 寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA 外设 ADC 基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Value; //DMA 内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = 20480; //DMA 通道的 DMA 缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度为 16 位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16 位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA 通道 x 拥有高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMA 通道 x 没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //根据 DMA_InitStruct 中指定的参数初始DMA 的通道
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

void TIM1_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	/* Time Base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
	TIM_TimeBaseStructure.TIM_Period = arr;          
	TIM_TimeBaseStructure.TIM_Prescaler = psc;       
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	/* TIM1 channel1 configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
	TIM_OCInitStructure.TIM_Pulse = arr; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}
//sys
void sys_ini(void)
{
	Driver_MCU_Init();
	GPIOA_ini();
	GPIOB_ini();
	GPIOC_ini();
	GPIOD_ini();
	//uart_ini();
}
