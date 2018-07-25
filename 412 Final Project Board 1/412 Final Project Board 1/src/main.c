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
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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
void test_USART(void);
void launch_game(void);

unsigned char P1 = '0';
unsigned char P2 = '0';
unsigned char P3 = '0';
unsigned char P4 = '0';

long gamestate = 0;

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
	O7 = 16384,
	X7 = 32768,
	O8 = 65536,
	X8 = 131072
} movelist;

movelist allmoves[] = { X0, X1, X2, X3, X4, X5, X6, X7, X8, O0, O1, O2, O3, O4, O5, O6, O7, O8 };
movelist allmovesinorder[] = { O0, X0, O1, X1, O2, X2, O3, X3, O4, X4, O5, X5, O6, X6, O7, X7, O8, X8 };
movelist allmovesrev[] = { O0, O1, O2, O3, O4, O5, O6, O7, O8, X0, X1, X2, X3, X4, X5, X6, X7, X8 };
movelist Xmoves[] = { X0, X1, X2, X3, X4, X5, X6, X7, X8 };
movelist Omoves[] = { O0, O1, O2, O3, O4, O5, O6, O7, O8 };
const int AllMovesLength = 18;
const int XMovesLength = 9;
const int OMovesLength = 9;

typedef enum {
	TopHorizO = O0 + O1 + O2,
	MidHorizO = O3 + O4 + O5,
	BotHorizO = O6 + O7 + O8,
	TopHorizX = X0 + X1 + X2,
	MidHorizX = X3 + X4 + X5,
	BotHorizX = X6 + X7 + X8,
	TopVertO = O0 + O3 + O6,
	MidVertO = O1 + O4 + O7,
	BotVertO = O2 + O4 + O8,
	TopVertX = X0 + X3 + X6,
	MidVertX = X1 + X4 + X7,
	BotVertX = X2 + X5 + X8,

	DownRightDiagO = O0 + O4 + O8,
	UpRightDiagO = O6 + O4 + O2,
	DownRightDiagX = X0 + X4 + X8,
	UpRightDiagX = X6 + X4 + X2,

} winlist;

winlist winlistOarray[] = { TopHorizO, MidHorizO, BotHorizO, TopVertO, MidVertO, BotVertO, DownRightDiagO, UpRightDiagO};
winlist winlistXarray[] = { TopHorizX, MidHorizX, BotHorizX, TopVertX, MidVertX, BotVertX, DownRightDiagX, UpRightDiagX };
const long WinListLength = 8;
bool OWins = false;
bool XWins = false;

void add_move(movelist ml);
void delete_move(movelist ml);
bool check_win(winlist wl);
bool check_move(movelist wl);
void check_full_board(void);
void print_win(void);
void bot_move_X_random(void);
void bot_move_O_random(void);
void bot_X_move(void);
void bot_O_move(void);
void add_turn_move(unsigned char PD2to7, unsigned char PB0to2, int turn);

void add_move(movelist ml) {
	if((gamestate & ml) != 1) {
		gamestate = gamestate + ml;
	}
	return;
}

void delete_move(movelist ml) {
	gamestate = gamestate - ml;
	return;
}

bool check_win(winlist wl) {
	if ((wl) == (wl & gamestate)) { return true; }
	return false;
}

bool check_move(movelist wl) {
	if ((wl) == (wl & gamestate)) { return true; }
	return false;
}

void check_full_board() {
	for (int iter = 0; iter < WinListLength; iter++) {
		if (check_win(winlistOarray[iter])) {
			OWins = true;
		}
		if (check_win(winlistXarray[iter])) {
			XWins = true;
		}
	}
	return;
}

void print_win() {
	if (OWins) {
		printf("O wins");
	}
	else if (XWins) {
		printf("X wins");
	}
	return;
}

void bot_move_X_random() {

	bool randmovefound = false;

	while (!randmovefound) {

		int currentrand = rand() % 9;

		movelist OMove = Omoves[currentrand];
		movelist XMove = Xmoves[currentrand];

		if (!((check_move(OMove)) && (check_move(XMove)))) {
			add_move(XMove);
			randmovefound = true;
		}
	}
	return;
}

void bot_move_O_random() {

	bool randmovefound = false;

	while (!randmovefound) {

		int currentrand = rand() % 9;

		movelist OMove = Omoves[currentrand];
		movelist XMove = Xmoves[currentrand];

		if (!((check_move(OMove)) && (check_move(XMove)))) {
			add_move(OMove);
			randmovefound = true;
		}
	}
	return;
}

void bot_X_move() {

	bool hasmoved = false;

	for (int iter1 = 0; iter1 < AllMovesLength; iter1++) {

		movelist potentialmove = allmoves[iter1];

		check_full_board();

		if (XWins) {
			add_move(potentialmove);
			hasmoved = true;
			break;
		}
		else if (OWins) {
			movelist Xoffset = allmoves[iter1 - 9];
			add_move(Xoffset);
			hasmoved = true;
			break;
		}
	}

	if (!hasmoved) {
		bot_move_X_random();
	}

	return;
}

void bot_O_move() {

	bool hasmoved = false;

	for (int iter1 = 0; iter1 < AllMovesLength; iter1++) {

		movelist potentialmove = allmovesrev[iter1];

		check_full_board();

		if (OWins) {
			add_move(potentialmove);
			hasmoved = true;
			break;
		}
		else if (XWins) {
			movelist Xoffset = allmoves[iter1 - 9];
			add_move(Xoffset);
			hasmoved = true;
			break;
		}
	}

	if (!hasmoved) {
		bot_move_O_random();
	}

	return;
}

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
void USART_Transmit( unsigned char data ) {
	
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void ) {

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

	flash_led_short();
	
	//unsigned char transmission_confirm1 = USART_Receive();
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

void collect_move(int turn) {
	
	unsigned char PD5to7 = 0;
	unsigned char PB0to5 = 0;
	
	bool hasnotentered = true;
	
	DDRB = 0x00;
	DDRD &= 0x3f;
		
	while (hasnotentered) {
		
		PD5to7 = (PIND & 192);
		PB0to5 = (PINB & 63);

		if((PD5to7 != 0) || (PB0to5 != 0)) {
			hasnotentered = false;
		}
	}
	
	add_turn_move(PD5to7, PB0to5, turn);
	
}

void add_turn_move(unsigned char PD2to7, unsigned char PB0to2, int turn) {
		if((PD2to7 & 1) == 1) {
			if(turn == 0) {
				add_move(O0);
			}
			else {
				add_move(X0);
			}
		}
		else if((PD2to7 & 2) == 1) {
			if(turn == 0) {
				add_move(O1);
			}
			else {
				add_move(X1);
			}
		}
		else if((PD2to7 & 4) == 1) {
			if(turn == 0) {
				add_move(O2);
			}
			else {
				add_move(X2);
			}
		}
		else if((PD2to7 & 8) == 1) {
			if(turn == 0) {
				add_move(O3);
			}
			else {
				add_move(X3);
			}
		}
		else if((PD2to7 & 16) == 1) {
			if(turn == 0) {
				add_move(O4);
			}
			else {
				add_move(X4);
			}
		}
		else if((PD2to7 & 32) == 1) {
			if(turn == 0) {
				add_move(O5);
			}
			else {
				add_move(X5);
			}
		}
		else if((PB0to2 & 1) == 1) {
			if(turn == 0) {
				add_move(O6);
			}
			else {
				add_move(X6);
			}
		}
		else if((PB0to2 & 2) == 1) {
			if(turn == 0) {
				add_move(O7);
			}
			else {
				add_move(X7);
			}
		}
		else if((PB0to2 & 4) == 1) {
			if(turn == 0) {
				add_move(O8);
			}
			else {
				add_move(X8);
			}
		}
}

void launch_game(void) {
	
	int moveloop = 0;
	USART_Init(MYUBRR);
	
	while(true) {
		moveloop = moveloop % 2;
		collect_move(moveloop);
		USART_Communicate_Boardstate(gamestate);
		moveloop++;
	}
}

void test_USART() {

	int moveloop = 0;
	
	while(true) {
		
		unsigned long message = allmovesinorder[(moveloop % 18)];		
		USART_Communicate_Boardstate(message);
		moveloop++;
	}
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();	
	
		flash_led_short();

	//launch_game();
	
	collect_move(0);
	
}
