#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#define BAUD 19600
#define MYUBRR (F_CPU/16/BAUD-1)


#define MAX_BUF 256
int main(void){
  UART_init();
  UART_putString((uint8_t*)"write something, i'll repeat it\n");
  uint8_t buf[MAX_BUF];
  while(1) {
    UART_getString(buf);
    UART_putString((uint8_t*)"received\n");
    UART_putString(buf);
  }

}

 
