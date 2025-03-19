#include<reg51.h>
#define LCD_Data P0
#define ADC_Data P2
sbit ADC_CS=P1^3;
sbit ADC_RD=P1^0;
sbit ADC_WR=P1^1;
sbit ADC_INTR=P1^2;
sbit LCD_RS=P3^4;
sbit LCD_RW=P3^5;
sbit LCD_EN=P3^6;
void delay(int cnt);
void ADC_conv();
void LCD_cmd(char cmd);
void LCD_data(char dat);
void LCD_init(void);
void main()
{
	int i,k=0;
	unsigned char adc_data;
	unsigned int adc_value;
	char dig_vol[]={'0','.','0','0','0','0','V'};
	char new_dig_vol[]={'0','.','0','0','0','0','V'};
	LCD_RW=0;
	LCD_init();
	while(1)
	{
		ADC_conv();
		adc_data=ADC_Data;
		adc_value=adc_data*196; //5*10000/255
		for(i=5;i>=0;i--)
		{
			if(i==1)
				continue;
			new_dig_vol[i]=(adc_value%10)+0x30;
			adc_value/=10;
		}
		for(i=0;i<7;i++)// to check if value has been changed
		{
			if(new_dig_vol[i]!=dig_vol[i])
			{
				k++;
				break;
			}
		}
		if(k)// if it has changed display it on lcd
		{
			LCD_cmd(0x01);
			for(i=0;i<7;i++)
			{
				LCD_data(new_dig_vol[i]);
				dig_vol[i]=new_dig_vol[i];
			}
		}
		k=0;
	}
}
void delay(int cnt)
{
	int i;
	for(i=0;i<cnt;i++);
}
void ADC_conv()
{
	ADC_CS=0;
	ADC_WR=0;
	delay(50);
	ADC_WR=1;
	ADC_RD=1;
	while(ADC_INTR);
	ADC_RD=0;
}
void LCD_init(void)
{
	LCD_cmd(0x38);
	delay(10);
	LCD_cmd(0x0c);
	delay(10);
	LCD_cmd(0x80);
	delay(10);
}
void LCD_cmd(char cmd)
{
	LCD_Data=cmd;
	LCD_RS=0;
	LCD_EN=1;
	delay(10);
	LCD_EN=0;
	delay(100);
}
void LCD_data(char dat)
{
	LCD_Data=dat;
	LCD_RS=1;
	LCD_EN=1;
	delay(10);
	LCD_EN=0;
	delay(100);
}