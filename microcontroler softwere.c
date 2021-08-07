/*
 * microcontroler softwere.c
 *
 * Created: 29.10.2020 16:14:07
 *  Author: Mateusz Rodak
 */ 

#include <stdbool.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "lcd.h"

static int NASTAWIONA_TEMP;

//UART DEFINE
#define F_CPU 1000000
#define BUFF_SIZE 32
#define BAUD 115200
unsigned char BUFF[BUFF_SIZE];
unsigned char START = 0;
unsigned char STOP = 0;
#define KROK(x) (x+1) & (BUFF_SIZE - 1)
char* tablica[5];

void UART_init(unsigned long baud)
{
	baud = F_CPU / (8 * BAUD) -1;
	
	UBRRH = baud >> 8;
	UBRRL =	baud & 0xFF;
	
	UCSRA = (1<<U2X); //Prêdkoœæ 
	UCSRB = (1<<RXEN) | (1<<TXEN) | (1<<RXCIE);  //w³¹czenie Rx i Tx. RXCIE aktywacja przerwania.
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<UCSZ0); // URSEL - wybór rejestru UCSZ1 - d³ugoœæ s³owa 
}	

unsigned char UART_read()
{
	STOP = KROK(STOP);
	return BUFF[STOP];	
}

void UART_write(unsigned char c) 
{
	while (!(UCSRA & (1<<UDRE)));
	UDR = c; //rejestr bufforowy Uarta
}

bool UART_read_ready() //GOTOWOSC DO WYSYLKI
{
	return STOP != START;
}

int main(void)
{
//NASTAWIENIE ADC DO PRACY
    DDRA = 0 << PA0;		//POTENCJOMETR JAKO SYMULACJA CZUJNIKA TEMPERATURY
	PORTA = 1 << PA0;
	  
	ADMUX = (1<<REFS0) | (1<<REFS1) | (1<<ADLAR) | 2; // <- wybrany multiplexer nr. 2 
	//REFS0 oraz REFS0 - ustawienie napiecia odniesienia
	//ADLAR - ustawienie rejestrow od lewej albo od prawej (ADCL, ADCH)
	
//NASTAWIENIE LCD DO PRACY	  
	LCD_init();		// Inicjacja wyœwietlacza
	LCD_clean();	// Czyszczenie wyœwietlacza

	fdevopen(LCD_putch, NULL);	// Otwarcie urz¹dzenia (wyœwietlacz bêdzie obs³ugiwa³ "printf")
	
	
//WLACZENIE DIODY
	DDRD = 1 << PD7;	
	
//WLACZENIE TIMERA	
	MCUCR = (1<<ISC01) | (1<<ISC00);
	//Interrupt 0 Sense Control
	//Zbocze narastaj¹ce z INT0 generuje impuls przerwania.
	
	GICR = (1<<INT0) | (1<<INT1); //Wlaczenie przerwania INT0 oraz INT1
	
	TCCR2 = (1<<WGM20) | (1<<WGM10) | (1<<CS20) | (1<<COM21); // - TIMER
	//WGM20 i WGM10 na 1 -> Fast PWM
	//CS20 -> ustawienie preskalera 
	//COM21 - wyczyœæ OC2 po udanym porównaniu
	
//URUCHOMIENIE KLAWISZY
	PORTD = (1 << PD2) | (1 << PD3); // PD2 - GORA, PD3 - DÓ£
	sei();	//USTAWIANIE TEMPERATURY ZA POMOC¥ KLAWISZY
	
//INICJALIZACJA UART
	DDRD = (1<<1);
	PORTD = (1<<1);
	
	UART_init(BAUD);
	
	while (1)
	{		
//POMIAR DANYCH
		ADCSRA = (1 << ADEN) | (1<<ADSC);
		//ADEN -> uruchomienie ADC
		//ADSC -> wystartowanie ADC
		
		while(bit_is_clear(ADCSRA, ADIF));
		//ADIF -> jak ma 1 to skonczyl konwersje
		
//WYŒWIETLENIE	
		LCD_printf_1("Nastawiona Temp: %d", NASTAWIONA_TEMP);		// Tekst w linii 1
		LCD_printf_2("TEMP aktualna:,  %d", ADCH);		// Tekst w linii 2
		
//STEROWANIE	
		if(ADCH-NASTAWIONA_TEMP>1 || NASTAWIONA_TEMP-ADCH>2)
		{
			OCR2 = ADCH; //Wstaw do rejestru porównawczego wartoœæ z potencjometru 
		}
		else
		{
			OCR2 = 0;
		}
		
//PRZYGOTOWANIE PACZKI DANYCH	
		tablica[0]= NASTAWIONA_TEMP; //NASTAWIONA
		tablica[1]= ADCH;			 //AKTUALNA
		tablica[2]= OCR2;			 //STAN GRZALKI
		tablica[3]= "Brak czujnika"; //Jasnosc w pomieszczeniu
		tablica[4]= OCR2/255;		 //jasnosc diody LED
		
//WYSLANIE 
		if (UART_read_ready)
		{
			UART_write(&tablica);
		}		
	}
}
ISR(INT0_vect)
{		
	NASTAWIONA_TEMP--;	
}

ISR(INT1_vect)
{
	NASTAWIONA_TEMP++;
}
	
ISR(USART_RXC_vect)
{
	START = KROK(START);
	BUFF[START] = UDR;
}