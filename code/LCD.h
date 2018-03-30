#ifndef __LCD_H__
#define __LCD_H__

//LCD Pin
#define  RS1 GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define  RS0 GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define  WR1 GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define  WR0 GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define  RD1 GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define  RD0 GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define  REST1 GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define  REST0 GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define  CS1 GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define  CS0 GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define  LCD_W 240
#define  LCD_H 320
extern   u32 BACK_COLOR,POINT_COLOR;
void LCD_Writ_Bus(u16 dat); 
void LCD_WR_DATA(u16 dat);
void LCD_WR_REG(u16 dat);	 
void Lcd_Init(void);
void LCD_Writ_Bus(u16 dat);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_Clear(u16 Color);
void LCD_DrawPoint(u16 x,u16 y);
void LCD_ShowChar(u16 x,u16 y,u8 num);
void LCD_ShowString(u16 x,int y,u8 *p);
void Draw_Circle(u16 x0,u16 y0,u8 r);
void putnums(u16 x,u16 y,int num);
void putfloatnums(u16 x,u16 y,float num);
void show_line1(int x0,int x1,int y);
void show_line2(int y0,int y1,int x);
#endif
