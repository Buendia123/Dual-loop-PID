/******************************************************************************
 *
 * COPYRIGHT:
 *   Copyright      Volex.      All rights reserved.
 *
 *
 * DESCRIPTION:
 *   SPI Master driver related.
 * HISTORY:
 *   @2022.09.01        AAA          Create
 *
 *****************************************************************************/
#include "drv_OLED.h"
#include "oledfont.h"
#include "math.h"

uint8_t OLED_GRAM[128][8];
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58
void OLED_Refresh(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
	   drv_oled_spim_stream(0xb0+i,OLED_CMD);
	   drv_oled_spim_stream(0x00,OLED_CMD);
	   drv_oled_spim_stream(0x10,OLED_CMD);
	   for(n=0;n<128;n++)
		 drv_oled_spim_stream(OLED_GRAM[n][i],OLED_DATA);
  }
}
void OLED_DisPlay_On(void)
{
	drv_oled_spim_stream(0x8D,OLED_CMD);
	drv_oled_spim_stream(0x14,OLED_CMD);
	drv_oled_spim_stream(0xAF,OLED_CMD);
}
void OLED_DisplayTurn(uint8_t i)
{
	if(i==0)
		{
		drv_oled_spim_stream(0xC8,OLED_CMD);
		drv_oled_spim_stream(0xA1,OLED_CMD);
		}
	if(i==1)
		{
		drv_oled_spim_stream(0xC0,OLED_CMD);
		drv_oled_spim_stream(0xA0,OLED_CMD);
		}
}

void OLED_Clear(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;
			}
  }
	OLED_Refresh();
}

void OLED_Clear_part(void)
{
	uint8_t i,n;
	for(i=0;i<4;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;
			}
  }
	OLED_Refresh();
}

void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t)
	{OLED_GRAM[x][i]|=n;}
	else
	{
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
		OLED_GRAM[x][i]|=n;
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	}
}

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode)
{
	uint8_t i,m,temp,size2,chr1;
	uint8_t x0=x,y0=y;
	if(size1==8)
		size2=6;
	else
		size2=(size1/8+((size1%8)?1:0))*(size1/2);
	chr1=chr-' ';
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];}
//		else if(size1==12)
//        {temp=asc2_1206[chr1][i];}
//		else if(size1==16)
//        {temp=asc2_1608[chr1][i];}
//		else if(size1==24)
//        {temp=asc2_2412[chr1][i];}
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)
				OLED_DrawPoint(x,y,mode);
			else
				OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}

void OLED_ShowString(uint8_t x,uint8_t y,const char *chr,uint8_t size1,uint8_t mode)
{
	while((*chr>=' ')&&(*chr<='~'))
	{
		if(x>MAX_CHAR_POSX)
		{
			x=0;
			y+=16;
		}
		if(y>MAX_CHAR_POSY)
		{
			y=x=0;
			OLED_Clear();
		}

		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)
			x+=6;
		else
			x+=size1/2;
		chr++;
  }
}

uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode)
{
	uint8_t t,temp,m=0;
	if(size1==8)m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
			else
			{
			  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
			}
  }
}

void OLED_ShowFloat(uint8_t x, uint8_t y, float num, uint8_t integer_len, uint8_t decimal_len, uint8_t size1, uint8_t mode)
{
    BOOL is_negative = (num < 0.0f);
    if (is_negative) {
        OLED_ShowChar(x, y, '-', size1, mode);
        num = -num;
        x += size1 / 2 + (size1 == 8 ? 2 : 0);
    }

    uint32_t integer_part = (uint32_t)num;
    OLED_ShowNum(x, y, integer_part, integer_len, size1, mode);

    x += integer_len * (size1 / 2 + (size1 == 8 ? 2 : 0));

    if (decimal_len>0)
    {
    	OLED_ShowChar(x, y, '.', size1, mode);
    }

    x += size1 / 2 + (size1 == 8 ? 2 : 0);

    float fractional_part = num - (float)integer_part;
    for (uint8_t i = 0; i < decimal_len; ++i)
    {
        fractional_part *= 10.0f;
        uint8_t digit = (uint8_t)fabs(fractional_part);
        OLED_ShowChar(x, y, '0' + digit, size1, mode);
        fractional_part -= (float)digit;
        x += size1 / 2 + (size1 == 8 ? 2 : 0);
    }
}

void DEV_OLED_Init(void)
{
	DRV_SPIM_RST_SET(0);
	HAL_Delay(200);
	DRV_SPIM_RST_SET(1);

	drv_oled_spim_stream(0xAE,OLED_CMD);//--turn off oled panel
	drv_oled_spim_stream(0x00,OLED_CMD);//---set low column address
	drv_oled_spim_stream(0x10,OLED_CMD);//---set high column address
	drv_oled_spim_stream(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	drv_oled_spim_stream(0x81,OLED_CMD);//--set contrast control register
	drv_oled_spim_stream(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	drv_oled_spim_stream(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	drv_oled_spim_stream(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	drv_oled_spim_stream(0xA6,OLED_CMD);//--set normal display
	drv_oled_spim_stream(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	drv_oled_spim_stream(0x3f,OLED_CMD);//--1/64 duty
	drv_oled_spim_stream(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	drv_oled_spim_stream(0x00,OLED_CMD);//-not offset
	drv_oled_spim_stream(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	drv_oled_spim_stream(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	drv_oled_spim_stream(0xD9,OLED_CMD);//--set pre-charge period
	drv_oled_spim_stream(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	drv_oled_spim_stream(0xDA,OLED_CMD);//--set com pins hardware configuration
	drv_oled_spim_stream(0x12,OLED_CMD);
	drv_oled_spim_stream(0xDB,OLED_CMD);//--set vcomh
	drv_oled_spim_stream(0x40,OLED_CMD);//Set VCOM Deselect Level
	drv_oled_spim_stream(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	drv_oled_spim_stream(0x02,OLED_CMD);//
	drv_oled_spim_stream(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	drv_oled_spim_stream(0x14,OLED_CMD);//--set(0x10) disable
	drv_oled_spim_stream(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	drv_oled_spim_stream(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
	OLED_Clear();
	drv_oled_spim_stream(0xAF,OLED_CMD);


}


