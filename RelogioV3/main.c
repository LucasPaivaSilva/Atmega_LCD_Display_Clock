#include "def_principais.h"
#include "LCD.h"
#include <avr/io.h>
#include <avr/interrupt.h>
const unsigned char carac0[] PROGMEM = {0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b11111};
const unsigned char carac1[] PROGMEM = {0b11111, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000};
const unsigned char carac2[] PROGMEM = {0b11111, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001};
const unsigned char carac3[] PROGMEM = {0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001};
const unsigned char carac4[] PROGMEM = {0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111};
const unsigned char carac5[] PROGMEM = {0b11111, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b11111};
const unsigned char carac6[] PROGMEM = {0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};
const unsigned char carac7[] PROGMEM = {0b11111, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11111};
unsigned char Nr_Grande[11] [4] =  {{0x01, 0x02, 0x4C, 0x00}, //nr. 0
									{0x20, 0x7C, 0x20, 0x7C}, //nr. 1
									{0x04, 0x05, 0x4C, 0x5F}, //nr. 2
									{0x06, 0x05, 0x5F, 0x00}, //nr. 3
									{0x4C, 0x00, 0x20, 0x03}, //nr. 4
									{0x07, 0x04, 0x5F, 0x00}, //nr. 5
									{0x07, 0x04, 0x4C, 0x00}, //nr. 6
									{0x06, 0x02, 0x20, 0x03}, //nr. 7
									{0x07, 0x05, 0x4C, 0x00}, //nr. 8
									{0x07, 0x05, 0x20, 0x03}, //nr. 9
									{0xA5, 0x20, 0xA5, 0x20}};
unsigned char dot_Grande[2][2] ={{0xA5, 0xA5},
								 {0x20, 0x2F}};
volatile unsigned int flag = 0;
unsigned char contSegundo = 0;	//variável de contagem para os segundos

void Nr_Grande_Print(char digit, char offset)
{
	unsigned int intToDisplay;
	cmd_LCD(0x80+offset, 0);
	intToDisplay = (digit) - '0';
	cmd_LCD(Nr_Grande[intToDisplay][0],1);
	cmd_LCD(Nr_Grande[intToDisplay][1],1);
	cmd_LCD(0xC0+offset, 0);
	cmd_LCD(Nr_Grande[intToDisplay][2],1);
	cmd_LCD(Nr_Grande[intToDisplay][3],1);
}
void dot_Grande_Print(char digit, char offset)
{
	unsigned int intToDisplay;
	cmd_LCD(0x80+offset, 0);
	intToDisplay = (digit) - '0';
	cmd_LCD(dot_Grande[intToDisplay][0],1);
	cmd_LCD(0xC0+offset, 0);
	cmd_LCD(dot_Grande[intToDisplay][1],1);
}
ISR(PCINT1_vect)
{
	if (flag == 0)
	{
		flag = 1;
	}
}
ISR(TIMER1_COMPA_vect) //interrupção do TC1
{
	contSegundo = contSegundo + 1;
}

int main()
{
	DDRD = 0xFF; //PORTD como saída
	DDRB = 0xFF; //PORTB como saída
	DDRC  = 0x00;
	PORTC = 0xFF;
	PCICR = 1<<PCIE1;
	PCMSK1 = (1<<PCINT8);
	TCNT1 = 0;
	OCR1A = 15624;
	//TCCR1A = (1<<COM1A0);
	TCCR1B = (1<<CS10) | (1<<CS12) | (1<<WGM12);//TC1 com prescaler de 1024
	TIMSK1 = (1<<OCIE1A); //habilita a interrupção do T1
	sei();
	unsigned char k;
	unsigned char digitos[3];//variável para os digitos individuais do segundo
	unsigned char contMinuto = 0;	//variável de contagem para os minutos
	unsigned char contHora = 0;	//variável de contagem para as horas
	unsigned char contDia = 2;		//variável de contagem para os dias
	unsigned char contMes = 4;		//variável de contagem para os meses
	unsigned int contAno = 19;	//variável de contagem para os anos
	unsigned int displaySwitch = 0;
	unsigned int x=0;
	#define bot1 PC0
	set_bit(PORTB, PB2);
	inic_LCD_4bits(); //inicializa o LCD
	/////////////////
	cmd_LCD(0x40, 0);
	for(k=0;k<7;k++){
		cmd_LCD(pgm_read_byte(&carac0[k]), 1);
	}
	cmd_LCD(0x00,1);
	//////////////////
	cmd_LCD(0x48,0);
	for(k=0;k<7;k++){
		cmd_LCD(pgm_read_byte(&carac1[k]), 1);
	}
	cmd_LCD(0x00,1);
	//////////////////
	cmd_LCD(0x50,0);
	for(k=0;k<7;k++){
		cmd_LCD(pgm_read_byte(&carac2[k]), 1);
	}
	cmd_LCD(0x00,1);
	/////////////////
	cmd_LCD(0x58, 0);
	for(k=0;k<7;k++){
		cmd_LCD(pgm_read_byte(&carac3[k]), 1);
	}
	cmd_LCD(0x00,1);
	//////////////////
	cmd_LCD(0x60,0);
	for(k=0;k<7;k++){
		cmd_LCD(pgm_read_byte(&carac4[k]), 1);
	}
	cmd_LCD(0x00,1);
	//////////////////
	cmd_LCD(0x68,0);
	for(k=0;k<7;k++){
		cmd_LCD(pgm_read_byte(&carac5[k]), 1);
	}
	cmd_LCD(0x00,1);
	//////////////////
	cmd_LCD(0x70,0);
	for(k=0;k<7;k++){
		cmd_LCD(pgm_read_byte(&carac6[k]), 1);
	}
	cmd_LCD(0x00,1);
	//////////////////
	cmd_LCD(0x78,0);
	for(k=0;k<7;k++){
		cmd_LCD(pgm_read_byte(&carac7[k]), 1);
	}
	cmd_LCD(0x00,1);
	/////////////////
	cmd_LCD(1, 0);
		
	while(1)
	{
		if (displaySwitch == 0)
		{
			cmd_LCD(1, 0);
			ident_num(contHora,digitos);
			Nr_Grande_Print(digitos[1], 0x01);
			Nr_Grande_Print(digitos[0], 0x03);
			dot_Grande_Print('0', 0x05);
			ident_num(contMinuto,digitos);
			Nr_Grande_Print(digitos[1], 0x06);
			Nr_Grande_Print(digitos[0], 0x08);
			dot_Grande_Print('0', 0x0A);
			ident_num(contSegundo,digitos);
			Nr_Grande_Print(digitos[1], 0x0B);
			Nr_Grande_Print(digitos[0], 0x0D);
		}
		else
		{
			cmd_LCD(1, 0);
			ident_num(contDia,digitos);
			Nr_Grande_Print(digitos[1], 0x01);
			Nr_Grande_Print(digitos[0], 0x03);
			dot_Grande_Print('1', 0x05);
			ident_num(contMes,digitos);
			Nr_Grande_Print(digitos[1], 0x06);
			Nr_Grande_Print(digitos[0], 0x08);
			dot_Grande_Print('1', 0x0A);
			ident_num(contAno,digitos);
			Nr_Grande_Print(digitos[1], 0x0B);
			Nr_Grande_Print(digitos[0], 0x0D);
		}
		
// 		for (x = 0; x<5; x++)
// 		{
// 			if (!tst_bit(PINC, bot1))
// 			{
// 				if (displaySwitch == 0){displaySwitch = 1;}
// 				else{displaySwitch = 0;}
// 			}
// 			_delay_ms(199);
// 		}
		_delay_ms(1000);
		if (flag == 1)
		{
			flag = 0;
			if (displaySwitch == 1)
			{
				displaySwitch = 0;
			}
			else
			{
				displaySwitch = 1;	
			}
		}
	
		if (contSegundo == 60){contSegundo = 0;contMinuto++;};
		if (contMinuto == 60){contMinuto = 0;contHora++;};
		if (contHora == 24){contHora = 0; contDia++;};
		if (contDia == 30){contDia = 0; contMes++;};
		if (contMes == 12){contMes = 0; contAno++;};
		
	}
}

	


