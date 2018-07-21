
/*
 * Assembler1.s
 *
 * Created: 7/18/2018 4:49:42 PM
 *  Author: Kevin Fou
 */ 

 .section ".data"					//student comment here
.equ	DDRB,0x04					//student comment here
.equ	DDRD,0x0A					//student comment here
.equ	PORTB,0x05					//student comment here
.equ	PORTD,0x0B					//student comment here
.equ	U2X0,1						//student comment here
.equ	UBRR0L,0xC4					//student comment here
.equ	UBRR0H,0xC5					//student comment here
.equ	UCSR0A,0xC0					//student comment here
.equ	UCSR0B,0xC1					//student comment here
.equ	UCSR0C,0xC2					//student comment here
.equ	UDR0,0xC6					//student comment here
.equ	RXC0,0x07					//student comment here
.equ	UDRE0,0x05					//student comment here
.equ	ADCSRA,0x7A					//student comment here
.equ	ADMUX,0x7C					//student comment here
.equ	ADCSRB,0x7B					//student comment here
.equ	DIDR0,0x7E					//student comment here
.equ	DIDR1,0x7F					//student comment here
.equ	ADSC,6						//student comment here
.equ	ADIF,4						//student comment here
.equ	ADCL,0x78					//student comment here
.equ	ADCH,0x79					//student comment here
.equ	EECR,0x1F					//student comment here
.equ	EEDR,0x20					//student comment here
.equ	EEARL,0x21					//student comment here
.equ	EEARH,0x22					//student comment here
.equ	EERE,0						//student comment here
.equ	EEPE,1						//student comment here
.equ	EEMPE,2						//student comment here
.equ	EERIE,3						//student comment here

.global Init
Init:
	ldi	r16,0xFF
	out DDRB,r16
	out	PORTB,r16
	ret

 .global LedTestOne
 LedTestOne:
	ldi	r16,0xFE
	out	PORTB,r16
	ret

.global	LedTestTwo
LedTestTwo:
	ldi r16,0xFD
	out PORTB,r16
	ret
