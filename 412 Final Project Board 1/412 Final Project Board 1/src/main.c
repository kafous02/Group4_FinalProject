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
unsigned char USART_Receive(void);
void USART_Communicate_Boardstate(unsigned long input);
void Split_Long (unsigned long input);
void Test_Split_Long(void);
void USART_Flush(void);

unsigned char P1 = '0';
unsigned char P2 = '0';
unsigned char P3 = '0';
unsigned char P4 = '0';


void flash_led(void) {
	
	DDRB = 0xff;

	int count = 0;

	while(count < 5) {
		PORTB = 255;
		_delay_ms(1000);
		PORTB = 0;
		_delay_ms(1000);
		count++;
	}
}

void flash_led_short(void) {
	
	DDRB = 0xff;

	int count = 0;

	while(count < 2) {
		PORTB = 255;
		_delay_ms(5000);
		PORTB = 0;
		_delay_ms(5000);
		count++;
	}
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

void USART_Communicate_Boardstate(unsigned long input) {

	P1 = 0;
	P2 = 0;
	P3 = 0;
	
	Split_Long(input);

	if((P1 == 80) && (P2 == 105) && (P3 == 96)) {
		flash_led_short();
	}
	
	unsigned char transmission_confirm1 = USART_Receive();
	USART_Transmit(P1);
	USART_Transmit(P2);
	USART_Transmit(P3);
	
	
}

void Split_Long (unsigned long input) {
	
	P1 = (unsigned char)((input & 0x000000ffUL)      );
	P2 = (unsigned char)((input & 0x0000ff00UL) >>  8);
	P3 = (unsigned char)((input & 0x00ff0000UL) >> 16);
	P4 = (unsigned char)((input & 0xff000000UL) >> 24);
	
	return;
}

void Test_Split_Long(void) {
	
	unsigned long testLong = 16843009;
	
	Split_Long(testLong);
	
	if(P1 == 1) {
		flash_led_short();
	}
	_delay_ms(10000);

	if(P2 == 1) {
		flash_led_short();
	}
	_delay_ms(10000);

	if(P3 == 1) {
		flash_led_short();
	}
	_delay_ms(10000);

	if(P4 == 1) {
		flash_led_short();
	}
}

void launch_game(void) {
	while(true) {
		
		long message = 0;
		USART_Communicate_Boardstate(message);
	}
}

void test_USART() {
	while(true) {
	USART_Communicate_Boardstate(6318416);
	}
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();	
	USART_Init(MYUBRR);
	flash_led_short();

	test_USART();
}
