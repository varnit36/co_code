
#include <REG52.h>
#include <avr/io.h>
#include <util/delay.h>


void write_command(unsigned char command);
void write_data(unsigned char character);
void write_string(unsigned char *s);


int main(void){

	DDRB |= (1<<DDB1 | 1<<DDB0);  //READWRITE connected to ground, write mode for enable and rs
	DDRD |= (1<<DDD4 | 1<<DDD5 | 1<<DDD6 | 1<<DDD7);  //write mode for data/command bits (4)


	_delay_ms(15);

	write_command(0x28);  //set interface - 4-Bits, 2-lines
	write_command(0x08);  //enable display
	write_command(0x01);  //clear and home
	write_command(0x06);  //move cursor right
	write_command(0x0C);  //turrn on display


	while(1){

		/write_string("VARNIT");  //send string to write
		write_command(0xC0);     //move cursor to next row
		write_string("JAIN");
		_delay_ms(2000);  
		write_command(0x01);     //clear screen


		write_string("SHREYANS"); 
		write_command(0xC0);
		write_string("MONGIA");
		_delay_ms(2000);

	}


}

void write_string(unsigned char *s){

	int i=0;

	for(;s[i]!='\0';i++){
		write_data(s[i]); //write char by char
		_delay_ms(100);   //to create a typing effect
	}    

}

void write_command(unsigned char command){

	PORTB &= ~(1<<PB0);  //en low
	PORTB &= ~(1<<PB1);  //rs low

	PORTD = (command & 0xf0); //high byte
	
	PORTB |= (1<<PB0); //en high
	_delay_us(1); // delay 450ns
	PORTB &= ~(1<<PB0);  //en low
	_delay_ms(5); // delay 5ms

	PORTD = ((command<<4) & 0xf0); //lower byte
	PORTB |= (1<<PB0); //en high 
	_delay_us(1); // delay 450ns
	PORTB &= ~(1<<PB0);  //en low
	_delay_ms(5); // delay 5ms


}

void write_data(unsigned char character){

	PORTB &= ~(1<<PB0);  //en low
	PORTB |= (1<<PB1);  //rs high

	PORTD = (character & 0xf0); //high byte
	PORTB |= (1<<PB0); //en high
	_delay_us(1); // delay 450ns
	PORTB &= ~(1<<PB0);  //en low
	_delay_us(200); // delay 5ms

	PORTD = ((character<<4) & 0xf0); //lower byte
	PORTB |= (1<<PB0); //en high  
	_delay_us(1); // delay 450ns
	PORTB &= ~(1<<PB0);  //en low
	_delay_us(200); // delay 5ms


}





