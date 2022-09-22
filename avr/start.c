#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "./avr_common/uart.h" // this includes the printf and initializes it

typedef struct {
  uint_8_t status:  1;//1 premuto, 0 rilasciato (1 bit)
  uint_8_t key:     7;//numero tra 0 e 7 (3 bit)
} KeyEvent;

uint_8_t KeyStatus; //do re mi fa sol la si do


int main(void){
  printf_init(); 
  
  conts uint_8_t mask = 0X0F; 
  printf("%x", mask);
  DDRF = mask; //00001111 bit 7-4 input bit 3-0 output

  PORTF = ~mask; //~ è alt gr + ì //11110000

}
 
