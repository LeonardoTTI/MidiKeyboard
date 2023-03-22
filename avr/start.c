#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "./avr_common/uart.h" // this includes the printf and initializes it

#define MASK 0xFF //11111111
volatile uint8_t mss = 0; //interrupt occured;
volatile uint8_t new_keys = 0;
volatile uint8_t old_keys = 0;
ISR(PCINT2_vect){
  old_keys = new_keys;
  new_keys = PINK & MASK;
  mss = 1;
}

int main(void){

  //setup arduino server
  printf_init();
  DDRK &= ~MASK;//and bab per modificare solo i bit della maschera 00000000 0 input,1 output
  PORTK |= MASK;//pull-up dei pin in input per metterli a 5v 11111111
  
  PCICR |= (1<<PCIE2); //set interrupt on change, looking up PCMSK2
  SREG |= (1<<7);

  PCMSK2 |= MASK;

  while(1){
    _delay_ms(10);
    if(mss){
      printf("%c", new_keys);//STAMPARE HEX IN MINUSCOLO
      mss=0;
    }
  }
  
}
