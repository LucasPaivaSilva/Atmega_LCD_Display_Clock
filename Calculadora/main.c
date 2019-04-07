/*
 *
 * Created: 12/03/2019 16:35:41
 *  Author: Aluno
 */ 
#include <string.h>
#include "def_principais.h"
#include "LCD.h"
#define F_CPU 16000000UL
#include "avr/io.h"
#include "util/delay.h"
#define SET_BIT(p,bit)  ((p)|=(1<<(bit)))
#define CLR_BIT(p,bit)  ((p)&=~(1<<(bit)))
#define TST_BIT(p,bit)  ((p)&(1<<(bit)))
#define CPL_BIT(p,bit)  ((p)^=(1<<(bit)))
unsigned char decimal_base[8] = {'128', '64', '32', '16', '8', '4', '2', '1'};
const unsigned char ScanBytes[]= {0b11111110, 0b11111101, 0b11111011, 0b11110111};
unsigned char PadDigits[4] [4] =   {{'7', '8', '9', 'D'},
									{'4', '5', '6', 'T'},
									{'1', '2', '3', 'M'},
									{'O', '0', 'E', 'P'}};
int x = 0;
int y = 0;
char ToSend;

char binarystr[8];
unsigned char first_digits[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};
unsigned char secon_digits[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};
unsigned char final_digits[10] = {'0', '0', '0', '0', '0', '0', '0', '0'};
int k = 0;
int Carry = 0;
int VT1;
int VT2;
int VDF;
int DigitCount = 8;
int First_Second_Switch = 0;
int OperationState = 0;

char *ul2binstr(unsigned long num, char *s, size_t len)
{
	if (0 == len) {
		} else {
		s[--len] = '\0';
	}

	do {
		if (0 == len) {
			} else {
			s[--len] = ((num & 1) ? '1' : '0');
		}
	} while ((num >>= 1) != 0);

	return s + len;
}

void Calculator(char digit, int KeypadInput)
{
	if (KeypadInput == 1)
	{
		DigitCount--;
		switch (digit)
		{
			case 'D':
			cmd_LCD(0x8D, 0);
			cmd_LCD('/', 1);
			First_Second_Switch = 1;
			OperationState = 1;
			DigitCount = 8;
			break;

			case 'T':
			cmd_LCD(0x8D, 0);
			cmd_LCD('*', 1);
			First_Second_Switch = 1;
			OperationState = 2;
			DigitCount = 8;
			break;
		
			case 'M':
			cmd_LCD(0x8D, 0);
			cmd_LCD('-', 1);
			First_Second_Switch = 1;
			OperationState = 3;
			DigitCount = 8;
			break;

			case 'P':
			cmd_LCD(0x8D, 0);
			cmd_LCD('+', 1);
			First_Second_Switch = 1;
			OperationState = 4;
			DigitCount = 8;
			break;
			
			case '7':
			cmd_LCD(0x8D, 0);
			cmd_LCD('E', 1);
			First_Second_Switch = 1;
			OperationState = 5;
			DigitCount = 8;
			break;
			
			case '8':
			cmd_LCD(0x8D, 0);
			cmd_LCD('O', 1);
			First_Second_Switch = 1;
			OperationState = 6;
			DigitCount = 8;
			break;
			
			case '9':
			cmd_LCD(0x8D, 0);
			cmd_LCD('X', 1);
			First_Second_Switch = 1;
			OperationState = 7;
			DigitCount = 8;
			break;
			
			case 'E':
			cmd_LCD(1,0);
			switch (OperationState)
			{
				case 5:
				for (k=0; k<8;k++)
				{
					VT1 = first_digits[k] - '0';
					VT2 = secon_digits[k] - '0';
					if (VT1==1 && VT2==1)
					{
						final_digits[k] = '1';
					} 
					else
					{
						final_digits[k] = '0';
					}
				}
				First_Second_Switch = 3;
				break;
				
				case 6:
				for (k=0; k<8;k++)
				{
					VT1 = first_digits[k] - '0';
					VT2 = secon_digits[k] - '0';
					if (VT1==1 || VT2==1)
					{
						final_digits[k] = '1';
					}
					else
					{
						final_digits[k] = '0';
					}
				}
				First_Second_Switch = 3;
				break;
				
				case 7:
				for (k=0; k<8;k++)
				{
					VT1 = first_digits[k] - '0';
					VT2 = secon_digits[k] - '0';
					if (VT1==1 != VT2==1)
					{
						final_digits[k] = '1';
					}
					else
					{
						final_digits[k] = '0';
					}
				}
				First_Second_Switch = 3;
				break;
				
				case 4:
				VT1 = 0;
				VT2 = 0;
				for (k=0; k<8;k++)
				{
					VT1 = (first_digits[k] - '0')*(decimal_base[k] - '0')+VT1;
					VT2 = (secon_digits[k] - '0')*(decimal_base[k] - '0')+VT2;
				}
				VDF = VT1 + VT2;
				ul2binstr(VDF, binarystr, 8);
				First_Second_Switch = 4;
				break;
				
				case 2:
				VT1 = 0;
				VT2 = 0;
				for (k=0; k<8;k++)
				{
					VT1 = (first_digits[k] - '0')*(decimal_base[k] - '0')+VT1;
					VT2 = (secon_digits[k] - '0')*(decimal_base[k] - '0')+VT2;
				}
				VDF = VT1 * VT2;
				ul2binstr(VDF, binarystr, 8);
				First_Second_Switch = 4;
				break;
				
				case 1:
				VT1 = 0;
				VT2 = 0;
				for (k=0; k<8;k++)
				{
					VT1 = (first_digits[k] - '0')*(decimal_base[k] - '0')+VT1;
					VT2 = (secon_digits[k] - '0')*(decimal_base[k] - '0')+VT2;
				}
				VDF = round(VT1 / VT2);
				ul2binstr(VDF, binarystr, 8);
				First_Second_Switch = 4;
				break;
				
				case 3:
				VT1 = 0;
				VT2 = 0;
				for (k=0; k<8;k++)
				{
					VT1 = (first_digits[k] - '0')*(decimal_base[k] - '0')+VT1;
					VT2 = (secon_digits[k] - '0')*(decimal_base[k] - '0')+VT2;
				}
				VDF = (VT1 - VT2);
				ul2binstr(VDF, binarystr, 8);
				First_Second_Switch = 4;
				break;
			}
			break;

			case 'O':
			cmd_LCD(1, 0);
			for (k=0; k<8;k++)
			{
				first_digits[k]='0';
				secon_digits[k]='0';
				final_digits[k]='0';
			}
			First_Second_Switch = 0;
			DigitCount = 8;
			break;
		
			case '1':
			if (First_Second_Switch == 0)
			{
				first_digits[DigitCount] = '1';
			} 
			else
			{
				secon_digits[DigitCount] = '1';
			}
			break;

			case '0':
			if (First_Second_Switch == 0)
			{
				first_digits[DigitCount] = '0';
			}
			else
			{
				secon_digits[DigitCount] = '0';
			}
			break;
		
			default:
			break;
		
		}
	}
	if (First_Second_Switch == 0)
	{
		cmd_LCD(0x80, 0);
		cmd_LCD(first_digits[0], 1);
		cmd_LCD(first_digits[1], 1);
		cmd_LCD(first_digits[2], 1);
		cmd_LCD(first_digits[3], 1);
		cmd_LCD(first_digits[4], 1);
		cmd_LCD(first_digits[5], 1);
		cmd_LCD(first_digits[6], 1);
		cmd_LCD(first_digits[7], 1);
	}
	if (First_Second_Switch == 1)
	{
		cmd_LCD(0xC0, 0);
		cmd_LCD(secon_digits[0], 1);
		cmd_LCD(secon_digits[1], 1);
		cmd_LCD(secon_digits[2], 1);
		cmd_LCD(secon_digits[3], 1);
		cmd_LCD(secon_digits[4], 1);
		cmd_LCD(secon_digits[5], 1);
		cmd_LCD(secon_digits[6], 1);
		cmd_LCD(secon_digits[7], 1);
	}
	if (First_Second_Switch == 3)
	{
		cmd_LCD(0x80,0);
		cmd_LCD('R',1);
		cmd_LCD('e',1);
		cmd_LCD('s',1);
		cmd_LCD('u',1);
		cmd_LCD('l',1);
		cmd_LCD('t',1);
		cmd_LCD('a',1);
		cmd_LCD('d',1);
		cmd_LCD('o',1);
		cmd_LCD(':',1);
		cmd_LCD(0xC0, 0);
		cmd_LCD(final_digits[0], 1);
		cmd_LCD(final_digits[1], 1);
		cmd_LCD(final_digits[2], 1);
		cmd_LCD(final_digits[3], 1);
		cmd_LCD(final_digits[4], 1);
		cmd_LCD(final_digits[5], 1);
		cmd_LCD(final_digits[6], 1);
		cmd_LCD(final_digits[7], 1);
	}
	if (First_Second_Switch == 4)
	{
		cmd_LCD(0x80,0);
		cmd_LCD('R',1);
		cmd_LCD('e',1);
		cmd_LCD('s',1);
		cmd_LCD('u',1);
		cmd_LCD('l',1);
		cmd_LCD('t',1);
		cmd_LCD('a',1);
		cmd_LCD('d',1);
		cmd_LCD('o',1);
		cmd_LCD(':',1);
		cmd_LCD(0xC0, 0);
		cmd_LCD(binarystr[0], 1);
		cmd_LCD(binarystr[1], 1);
		cmd_LCD(binarystr[2], 1);
		cmd_LCD(binarystr[3], 1);
		cmd_LCD(binarystr[4], 1);
		cmd_LCD(binarystr[5], 1);
		cmd_LCD(binarystr[6], 1);
		cmd_LCD(binarystr[7], 1);
	}
}

int main(void)
{
	DDRD  = 0xFF;
	DDRC  = 0xFF;
	DDRB  = 0x00;
	PORTB = 0xFF;
	inic_LCD_4bits(); //inicializa o LCD
	escreve_LCD("Paivox's Calc");
	_delay_ms(400);
	cmd_LCD(1, 0);
	cmd_LCD(0x80,0);
	Calculator(ToSend, 0);
    while(1)
    {
		for (x=0;x<4;x++)
		{
			PORTD = ScanBytes[x];
			for (y=0;y<4;y++)
			{
				if (TST_BIT(PINB, y)==0)
				{
				ToSend = PadDigits[x][y];
				Calculator(ToSend, 1);
				_delay_ms(500);
				}	
			}
		}
					

	}
		
}

