/*
 * graphicalLCD128X64.c
 *
 * Created: 01/08/2018 10:43:49 p. m.
 *  Author: https://www.youtube.com/watch?v=DZ1EH_06IAc
  modified by sencillamente charles
 */ 
#define  F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "glcd-font.h"
/* QUICK PIN MAP
PORT C: |7|6|5 CS2|4 CS1|3 RST|2 E|1 RW|0 RS
PORTD : DB7 TO DB0 GLCD
*/
#define GLCD_DATA_PORT PORTD
#define GLCD_CTRL_PORT PORTC
#define GLCD_DPORT_DIR DDRD
#define GLCD_CPORT_DIR DDRC

#define RS PC0
#define RW PC1
#define E  PC2
#define RST PC3
#define CS1 PC4
#define CS2 PC5

enum{leftPage,rigthPage,bothPages,none}page;

void glcd_cmd(unsigned char cmd);
void glcd_dta(unsigned char dta);
void glcd_PageSelect(char pge);
void glcd_writeChar(char * chr);
void glcd_SetCursor(uint8_t line,uint8_t y);
void glcd_writeStr(char pge,char* str);
void glcd_writeImg(char* pxl);
void glcd_int(void);

int main(void)
{
	//char xharA[5]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF};// A

	glcd_int();
	//glcd_PageSelect(leftPage);
	glcd_SetCursor(0,0);
    while(1)
    {
       // glcd_writeChar(xharA);
	    glcd_SetCursor(0,0);
		glcd_writeStr(leftPage,"-aber tus    senos bebe  ");
		glcd_SetCursor(4,0);
		glcd_writeStr(leftPage,"-Ay wakala   eres hombre  cortate el    pelo gei");
		glcd_SetCursor(7,0);
		glcd_writeStr(leftPage,"");
		_delay_ms(100);
		
		
    }
}
void glcd_cmd(unsigned char cmd)
{
	GLCD_DATA_PORT=cmd;// displaying command
	GLCD_CTRL_PORT&=~(1<<RS);// rs on zero means that command is being writed
	GLCD_CTRL_PORT&=~(1<<RW);// GCLD on  write mode
	GLCD_CTRL_PORT|=1<<E;
	_delay_ms(1);
	GLCD_CTRL_PORT&=~(1<<E);//GLCD eats command
}
void glcd_dta(unsigned char dta)
{
	GLCD_DATA_PORT=dta;// displaying command
	GLCD_CTRL_PORT|=(1<<RS);// rs on one means that data is being writed
	GLCD_CTRL_PORT&=~(1<<RW);// GCLD on  write mode
	GLCD_CTRL_PORT|=1<<E;
	_delay_ms(1);
	GLCD_CTRL_PORT&=~(1<<E);//GLCD eats command
}
void glcd_PageSelect(char pge)
{
	switch(pge)
	{
		case 0:
			GLCD_CTRL_PORT|=1<<CS1;   // enable left middle part
			GLCD_CTRL_PORT&=~(1<<CS2);
			break;
		case 1:
		    GLCD_CTRL_PORT&=~(1<<CS1);
			GLCD_CTRL_PORT|=1<<CS2;	 // enable right middle part
			break;
		case 2:
			GLCD_CTRL_PORT|=1<<CS1;// enable both parts
			GLCD_CTRL_PORT|=1<<CS2;	
			break;
		case 3:                       //disable both parts
			GLCD_CTRL_PORT&=~(1<<CS1);
			GLCD_CTRL_PORT&=~(1<<CS2);
			break;
		default:
			GLCD_CTRL_PORT|=1<<CS1;
			GLCD_CTRL_PORT&=~(1<<CS2);
		
	}
}
void glcd_SetCursor(uint8_t line,uint8_t y)
{
	glcd_cmd(0xB8+line);
	_delay_ms(1);
	glcd_cmd(0x40+y);
}
void glcd_writeChar( char * chr)
{
	uint8_t i;
	//glcd_SetCursor(0,0);
	for (i=0;i<5;i++)
	{
		glcd_dta(*(chr+i));
	}
}
void glcd_writeStr(char pge,char* str)
{
	int i=0;
	int startfont=0,endfont=0;
	glcd_PageSelect(pge);
	//glcd_SetCursor(1,10);
	while (*str)
	{
		startfont=(*str-32)*5;
		endfont=startfont + 5;
		for (i=startfont;i<endfont;i++)
		{
			glcd_dta(sys5x7[i]);
			_delay_ms(10);
		}
		str++;
	}
}
void glcd_writeImg(char* pxl){}
void glcd_int(void)
{
	GLCD_CPORT_DIR=255;//setting data and control ports as outputs
	GLCD_DPORT_DIR=255;
	GLCD_CTRL_PORT&=~(1<<RST);//making a reset
	_delay_ms(10);
	GLCD_CTRL_PORT|=1<<RST;
	_delay_ms(1);
	glcd_PageSelect(2);
	_delay_ms(1);
	glcd_cmd(0x3F);
	glcd_cmd(0xC0);
	_delay_ms(1);
	glcd_PageSelect(3);
	_delay_ms(1);
}
