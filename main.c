#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART.h"
#include <stdio.h>

// TEMPERATURA
unsigned int temp = 0;
unsigned char digitos [tam_vetor];

// COMANDOS
char comando;
unsigned char novo_comando = 0;

#define CMD_PISCA "pisca"


// SHIFT
#define HC595_PORT   PORTB
#define HC595_DDR    DDRB
#define HC595_DS_POS PB5      //Data pin (DS) pin location
#define HC595_SH_CP_POS PB3      //Shift Clock (SH_CP) pin location 
#define HC595_ST_CP_POS PB4      //Store Clock (ST_CP) pin location
void HC595Init()
{
   //Make the Data(DS), Shift clock (SH_CP), Store Clock (ST_CP) lines output
   HC595_DDR|=((1<<HC595_SH_CP_POS)|(1<<HC595_ST_CP_POS)|(1<<HC595_DS_POS));
}

//Low level macros to change data (DS)lines
#define HC595DataHigh() (HC595_PORT|=(1<<HC595_DS_POS))

#define HC595DataLow() (HC595_PORT&=(~(1<<HC595_DS_POS)))


//Sends a clock pulse on SH_CP line
void HC595Pulse()
{
   //Pulse the Shift Clock

   HC595_PORT|=(1<<HC595_SH_CP_POS);//HIGH

   HC595_PORT&=(~(1<<HC595_SH_CP_POS));//LOW

}

//Sends a clock pulse on ST_CP line
void HC595Latch()
{
   //Pulse the Store Clock

   HC595_PORT|=(1<<HC595_ST_CP_POS);//HIGH
   _delay_loop_1(1);

   HC595_PORT&=(~(1<<HC595_ST_CP_POS));//LOW
   _delay_loop_1(1);
}

void HC595Write(uint8_t data)
{
   //Send each 8 bits serially

   //Order is MSB first
   uint8_t i ;
   for( i=0;i<8;i++)
   {
      //Output the data on DS line according to the
      //Value of MSB
      if(data & 0b10000000)
      {
         //MSB is 1 so output high

         HC595DataHigh();
      }
      else
      {
         //MSB is 0 so output high
         HC595DataLow();
      }

      HC595Pulse();  //Pulse the Clock line
      data=data<<1;  //Now bring next bit at MSB position

   }

   //Now all 8 bits have been transferred to shift register
   //Move them to output latch at one
   HC595Latch();
}


// Inicia os parametros
void init()
{

   // PWM
   DDRB =  0b00000000;
   //PORTB = 0b00000000;
   set_bit(TCCR1A, COM1A0);
   set_bit(TCCR1A, WGM12);
   set_bit(TCCR1B, CS11);
   OCR1A = 8;

   // COLOCA AS PORTAS COMO LEITURA
   DDRB = 0xff;
   PORTB = 0x00;   
   
   clr_bit(DDRB, PB1); // INFRAVERMELHO
   
   DDRC = 0xff;
   // CONFIGURANDO LEITURA ANALOGICA TEMPERATURA
   clr_bit(DDRC, PC1);
   PORTC = 0x00;   
   clr_bit(DDRC, PC2);

    // CONFIGURANDO ADC
   ADMUX  = 0b11000001;  //Tensão interna de ref (1.1v) canal 0
   ADCSRA = 0b11101110; /* Habilita o AD, habilita interrupção, modo de conversão contínua, 
                           prescalar =  128*/

   ADCSRB = 0x00;       // Modo de conversão contínuo
   clr_bit(DIDR0,0);    //desabilita pino PD0 como I/0, entrada do ADC0
   sei();
   

   // INICIALIZA PORTAS DO SHIFT
   HC595Init();
   
   // INICIALIZA USART
   USART_Inic(MYUBRR);
   
   // INTERRUPÇÃO LEITURA
   set_bit(UCSR0B, RXCIE0);   
  
}

void showPirSensor()
{

    // PIR SENSOR
   clr_bit(DDRD, PD2);
   //set_bit(PORTD, PD2);
   
   if ( tst_bit(PIND, PD2) ) {
      escreve_USART("0001 p");
   }
   else
   {
      escreve_USART("0000 p");
   }
   USART_Transmite('\r');
   USART_Transmite('\n');  
}

void showTemperature()
{
   ident_num(temp,digitos);
   USART_Transmite(digitos[3]);
   USART_Transmite(digitos[2]);
   USART_Transmite(digitos[1]);
//   USART_Transmite(',');
   USART_Transmite(digitos[0]);
   USART_Transmite(' ');
   USART_Transmite('c');
   USART_Transmite('\r');
   USART_Transmite('\n');  ;
}

void activeLamp(){
   set_bit(DDRB, PB2);
   set_bit(PORTB, PB2);
   escreve_USART("Liga lampada");
   USART_Transmite('\r');
   USART_Transmite('\n'); 
  
}

void desactiveLamp(){
   set_bit(DDRB, PB2);   
   clr_bit(PORTB, PB2);
   escreve_USART("Desliga lampada");
   USART_Transmite('\r');
   USART_Transmite('\n');   
}

void showLamp(){
   if ( tst_bit(PORTB, PB2) ){
       escreve_USART("0001 x");
   }
   else
   {
       escreve_USART("0000 x");   
   }
   USART_Transmite('\r');
   USART_Transmite('\n');
}

void ligaPwm()
{
  //DDRB =  0b00000010;
  set_bit(DDRB, PB1);
  //PORTD = 0b10011111;
   //PORTD = 0b10011111;
  // set_bit(TCCR0A, COM0A0);
  // set_bit(TCCR0A, WGM01);
  // set_bit(TCCR0B, CS01);
  // OCR0A = 8;
}

void desligaPwm()
{
   clr_bit(DDRB, PB1);
  //DDRB =  0b00000000;
  //PORTD = 0b00000000;
}

void ligaTv(){
 ligaPwm(); 
  _delay_us(8711.6345); // 1 - 146
  desligaPwm(); 
  _delay_us(4694.4776); // 0 - 71
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1719.1045); // 0 - 26
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1719.1045); // 0 - 26
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1652.9851); // 0 - 25
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1719.1045); // 0 - 26
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1719.1045); // 0 - 26
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(1652.9851); // 0 - 25
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(1785.2239); // 0 - 27
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1719.1045); // 0 - 26
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(8463.2836); // 0 - 128
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1719.1045); // 0 - 26
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1719.1045); // 0 - 26
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(1785.2239); // 0 - 27
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1652.9851); // 0 - 25
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(1719.1045); // 0 - 26
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1719.1045); // 0 - 26
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(462.8358); // 0 - 7
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(462.8358); // 0 - 7
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(462.8358); // 0 - 7
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1719.1045); // 0 - 26
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1652.9851); // 0 - 25
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(462.8358); // 0 - 7
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(1652.9851); // 0 - 25
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(8397.1642); // 0 - 127
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(462.8358); // 0 - 7
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(462.8358); // 0 - 7
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(462.8358); // 0 - 7
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(1652.9851); // 0 - 25
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(462.8358); // 0 - 7
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(462.8358); // 0 - 7
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(477.3498); // 1 - 8
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(1652.9851); // 0 - 25
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(595.0746); // 0 - 9
  ligaPwm(); 
  _delay_us(537.0186); // 1 - 9
  desligaPwm(); 
  _delay_us(528.9552); // 0 - 8
  ligaPwm(); 
  _delay_us(596.6873); // 1 - 10
  desligaPwm(); 
  _delay_us(224277.0149); // 0 - 3392
  _delay_us(66.1194); // 0 - 1
  
  escreve_USART("Liga Ar condicionado");
  USART_Transmite('\r');
  USART_Transmite('\n');    

}

void showPortSensor()
{
   clr_bit(DDRD, PD3);
   set_bit(PORTD, PD3);
   if ( tst_bit(PIND, PD3)   ){
      escreve_USART("0000 s");
   }
   else
   {
      escreve_USART("0001 s");         
   }
   USART_Transmite('\r');
   USART_Transmite('\n');     
}

void showLuminosidade()
{  
    set_bit(PORTC, PC2);
    _delay_ms(10);
    ADMUX  = 0b11000010;  //Tensão interna de ref (1.1v) canal 2
    _delay_ms(50);
   ident_num(temp,digitos);
   USART_Transmite(digitos[3]);
   USART_Transmite(digitos[2]);
   USART_Transmite(digitos[1]);
   USART_Transmite(digitos[0]);
   USART_Transmite(' ');
   USART_Transmite('l');
   USART_Transmite('\r');
   USART_Transmite('\n');  
  
    ADMUX  = 0b11000001;  //Tensão interna de ref (1.1v) canal 1    
    clr_bit(PORTC, PC2);
    comando = ' ';
}


int main (){
 init();
 uint8_t led_pattern[8]={
                        0b10000000,                        
                        0b11000000,
                        0b11100000,
                        0b11110000,
                        0b11111000,
                        0b11111100,
                        0b11111110,                        
                        0b11111111};


  escreve_USART("CENTRAL DE MONITORAMENTO VERSÃO 1.0");

   while ( 1 ) 
   {
      //PORTC ^= _BV( PC5 );	

      //escreve_USART("HELLO WORD");
      /*for(s = 0; s < 8; s++ ){
	      HC595Write(led_pattern[s]);    
	      _delay_ms(100);
	   }*/
	   
	   switch ( temp ) 
	   {
	      case 100 ... 150: 
	         HC595Write(led_pattern[0]);
	         break;
	     case 151 ... 180:
	         HC595Write(led_pattern[1]);
	         break;
	     case 181 ... 200:
	         HC595Write(led_pattern[2]);
	         break;
        case 201 ... 220:
	         HC595Write(led_pattern[3]);
	         break; 	         
	     case 221 ... 240:
	         HC595Write(led_pattern[4]);
	         break;    
	      case 241 ... 260:
	         HC595Write(led_pattern[5]);
	         break;
	      case 261 ... 280:
	         HC595Write(led_pattern[6]);
	         break;   	      
	      default:
	         HC595Write(led_pattern[7]);
	      
	   }
	   
	   if ( novo_comando == 1 ){
         int i ;

         

	      switch ( comando )
	      {
	         case 'p':   
	            for ( i = 0; i < 5 ; i++ ){
	               HC595Write(led_pattern[0]);
	               _delay_ms(1000);
	               HC595Write(led_pattern[7]);
	               _delay_ms(1000);
	            }
	            break;
	         case 't':
	            showTemperature();
	            break;
	         case 'l':
	            showLuminosidade();
	            break;
	         case '1':
	            activeLamp();
	            break;
	         case '0':
	            desactiveLamp();
	            break;
	         case 's':
	            showLamp();
	            break;
	         case 'o':
	            ligaTv();
	            break;
	         case 'r':
	            showPirSensor();
	            break;
	         case 'q':
	            showPortSensor();
	            break;
	            
	      }
	      novo_comando = 0;
	   }
	   
     //escreve_USART("CENTRAL DE MONITORAMENTO VERSÃO 1.0\n\r");
     //showTemperature();
    //_delay_ms(1000);
     
   }	
}

ISR(USART_RX_vect){
   static char buffer_rx;

   if ( UDR0 == '\n' ){
      comando = buffer_rx;                  
      buffer_rx = ' ';           
      novo_comando = 1;        
   }
   else{
      buffer_rx = UDR0;      
   }
  
   
}

ISR(ADC_vect)
{
   static char contador = 0;
   static int buffer = 0;
   //static int iDelay;
   if ( comando == 'l'){
      temp = ADC;
      contador = 0;
      buffer = 0;
   }
   else
   {
      
      if ( contador < 10 )
      {
         buffer +=  ADC * (1049 / 1024);
         contador++;
      }
      else
      {
         temp = buffer /10;
         contador = 0;
         buffer = 0;
      }
      
      
      //iDelay = ADCH;
      //iDelay += (ADCL << 8);
      //iDelay = ADCW;
      //temp = iDelay * ( 1049 / 1023 );
   }
   //temp = (ADC + (ADC * 19)/256);
   //temp = ADC * 1024 / 110;
}
