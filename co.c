
#include <reg51.h>
#include <avr/io.h>
#include <util/delay.h>

#define LCDPORT P2
sbit RS=LCDPORT^0;
sbit RW=LCDPORT^1;
sbit E =LCDPORT^2;

int ch; 

void write_command(unsigned char command);
void write_data(unsigned char character);
void write_string(unsigned char *s);


int main(void){

	
	_delay_ms(15);

	write_command(0x28);  //set interface - 4-Bits, 2-lines
	write_command(0x08);  //enable display
	write_command(0x01);  //clear and home
	write_command(0x06);  //move cursor right
	write_command(0x0C);  //turrn on display


	while(1){

		initial_welcome_screen();
		_delay_ms(1500);
		game_select_screen();
		bbye_screen();
		_delay_ms(3000);

	}

	/*while(1){

		write_string("VARNIT");  //send string to write
		write_command(0xC0);     //move cursor to next row
		write_string("JAIN");
		_delay_ms(2000);  
		write_command(0x01);     //clear screen


		write_string("SHREYANS"); 
		write_command(0xC0);
		write_string("MONGIA");
		_delay_ms(2000);

	}*/


}

void bbye_screen(){

	write_command(0x01);     //clear screen
	write_string("Thank you");
	write_command(0xC0);
	write_string("Adios");

}

void initial_welcome_screen(){

	write_command(0x01);     //clear screen
	write_string("Welcome");
	write_command(0xC0);
	write_string("Media Centre");

}

void game_select_screen(){

	write_command(0x01);     //clear screen
	write_string("B1 for Timer");
	write_command(0xC0);
	write_string("B2 for Game (bang)");

	ch=0;

	while(ch!=3){

		if(b1==high){
			ch=1;
		}
		else if(b2==high){
			ch=2;
		}

		if(ch==1){
			timer();
		}
		else if(ch==2){
			bang();
		}
	
	}

	

}

void timer(){
	write_command(0x01);     //clear screen
	write_string("Press B3 to count up the timer");

	//button timer with ISR code


	write_command(0x01);     //clear screen
	write_string("B1(Timer), B2(Bang)");
	write_command(0xC0);
	write_string("B3 for exit");

	pressed=0;

	while(!pressed){
		if(b1==high){
			pressed=1;
			ch=1;
		}
		else if(b2==high){
			pressed=1;
			ch=2;
		}
		else if(b3==high){
			pressed=1;
			ch=3;
		}
	}
}



void bang(){

	write_command(0x01);     //clear screen
	write_string("Player 1 presses B1");
	write_command(0xC0);
	write_string("Player 2 presser B2");
	_delay_ms(1000);
	write_command(0x01);     //clear screen
	write_string("The player who presses first wins");
	write_command(0xC0);
	write_string("This is a best of 5 contest");

	int score_p1=0,score_p2=0;

	while(score_p1<5 && score_p2<5){

		write_command(0x01);
		write_string_without_delay("Ready");
		_delay_ms(200);
		write_command(0x01);
		_delay_ms(1000);
		write_command(0x01);
		write_string_without_delay("Steady");
		_delay_ms(200);
		write_command(0x01);
		_delay_ms(1000);
		write_string_without_delay("*****BANG*****");


		int pressed=0;

		while(!pressed){
			if(b1==high){
				pressed=1;
				score_p1++;
			}
			else if(b2==high){
				pressed=1;
				score_p2++;
			}
		}


	}

	if(score_p1==5){
		write_command(0x01);     //clear screen
		write_string("Player 1 wins");
	}else if(score_p2==5){
		write_command(0x01);     //clear screen
		write_string("Player 2 wins");
	}


	write_command(0x01);     //clear screen
	write_string("B1(Timer), B2(Bang)");
	write_command(0xC0);
	write_string("B3 for exit");

	pressed=0;

	while(!pressed){
		if(b1==high){
			pressed=1;
			ch=1;
		}
		else if(b2==high){
			pressed=1;
			ch=2;
		}
		else if(b3==high){
			pressed=1;
			ch=3;
		}
	}

}



void write_string(unsigned char *s){

	int i=0;

	for(;s[i]!='\0';i++){
		write_data(s[i]); //write char by char
		_delay_ms(100);   //to create a typing effect
	}    

}


void write_string_without_delay(unsigned char *s){

	int i=0;

	for(;s[i]!='\0';i++){
		write_data(s[i]); //write char by char
		//_delay_ms(100);   //to create a typing effect
	}    

}


void write_command(unsigned char command){

	E=0;//PORTB &= ~(1<<PB0);  //en low
	RS=0;//PORTB &= ~(1<<PB1);  //rs low
	
	
	LCDPORT = (command & 0xf0);  //PORTD = (command & 0xf0); //high byte
	

	E=1; //PORTB |= (1<<PB0); //en high
	_delay_us(1); // delay 450ns
	E=0; //PORTB &= ~(1<<PB0);  //en low
	_delay_ms(5); // delay 5ms

	LCDPORT = ((command<<4) & 0xf0); //PORTD = ((command<<4) & 0xf0); //lower byte
	E=1; //PORTB |= (1<<PB0); //en high 
	_delay_us(1); // delay 450ns
	E=0; //PORTB &= ~(1<<PB0);  //en low
	_delay_ms(5); // delay 5ms


}

void write_data(unsigned char character){

	E=0; //PORTB &= ~(1<<PB0);  //en low
	RS=1; //PORTB |= (1<<PB1);  //rs high

	LCDPORT = (command & 0xf0); //PORTD = (character & 0xf0); //high byte
	//PORTB |= (1<<PB0); //en high
	_delay_us(1); // delay 450ns
	//PORTB &= ~(1<<PB0);  //en low
	_delay_us(200); // delay 5ms

	LCDPORT = ((command<<4) & 0xf0); //PORTD = ((character<<4) & 0xf0); //lower byte
	E=1; //PORTB |= (1<<PB0); //en high  
	_delay_us(1); // delay 450ns
	E=0; //PORTB &= ~(1<<PB0);  //en low
	_delay_us(200); // delay 5ms


}





