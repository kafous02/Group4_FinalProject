/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <util/delay.h>
#define F_CPU = 8000000UL;

#define FOSC 1843200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void flash_led(void);
void flash_led_short(void);
void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
void USART_Communicate_Boardstate(void);
void USART_Flush(void);
void launch_game(void);
void gamestate_to_led(unsigned char reception1, unsigned char reception2, unsigned char reception3);


typedef enum {
	O0 = 1,
	X0 = 2,
	O1 = 4,
	X1 = 8,
	O2 = 16,
	X2 = 32,
	O3 = 64,
	X3 = 128,
	O4 = 256,
	X4 = 512,
	O5 = 1024,
	X5 = 2048,
	O6 = 4096,
	X6 = 8192,
	O7 = 16394,
	X7 = 32768,
	O8 = 65536,
	X8 = 131072
} movelist;

unsigned char PBEntry = 0;
unsigned char PCEntry = 0;
unsigned char PDEntry = 0;

unsigned int test = 1;

void flash_led(void) {
	
	DDRB = 0xff;

	int count = 0;

	//while(count < 2) {
		PORTB = (~4);
		_delay_ms(10000);
		PORTB = (~(8));
		_delay_ms(10000);
		//count++;
	//}
}

void gamestate_to_led(unsigned char reception1, unsigned char reception2, unsigned char reception3) {
	DDRB = 0xff;
	DDRC = 0xff;
	DDRD = 0xff;
	PORTB = 0xff;
	PORTC = 0xff;
	PORTD = 0xff;
	
	PBEntry = (reception1 & 63);
	PCEntry = (reception2 & 63);
	PDEntry = (((reception1 & 192) >> 2) + (reception2 & 192) + ((reception3 & 3) << 2));
	
	
	PORTB = (~PBEntry);
	PORTC = (~PCEntry);
	PORTD = (~PDEntry);
	
	int count = 0;
	//while(count < 5) {
		_delay_ms(100);
		count++;
	//}
	
	test = test*2;

}

void flash_led_short(void) {
	
	DDRB = 0xff;

	int count = 0;

	//while(count < 2) {
		PORTB = 0;
		_delay_ms(5000);
		PORTB = 255;
		_delay_ms(5000);
		//count++;
	//}
}

void USART_Init( unsigned int ubrr){
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	}

void USART_Transmit( unsigned char data ){
	
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void ){
	
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}
void USART_Flush( void )
{
	unsigned char dummy;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}

void USART_Communicate_Boardstate(void) {
	
	unsigned char reception1 = 0;
	unsigned char reception2 = 0;
	unsigned char reception3 = 0;
	
	reception1 = 0;
	reception2 = 0;
	reception3 = 0;
		
	USART_Transmit('>');
	reception1 = USART_Receive();
	reception2 = USART_Receive();
	reception3 = USART_Receive();

	gamestate_to_led(reception1, reception2, reception3);

}

void launch_game(void) {
	while(true) {
		USART_Communicate_Boardstate();
		USART_Flush();
	}
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();	
	USART_Init(MYUBRR);
	//flash_led_short();
	
	launch_game();
}
