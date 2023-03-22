#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <time.h>
#include <sys/types.h>
//gcc -o mainGPT main_gpy.c
#include <stdlib.h> 
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>
#include <ncurses.h>

long durations[7] = {0,0,0,0,0,0,0};
char* thenoteUS[] = {" C", " D", " E", " F", " G", " A", " B"};
char* thenoteEU[] = {" DO", " RE", " MI", " FA", "SOL", " LA", " SI"};
int noteFreq4[] = {262, 294, 330, 349, 392, 440, 494};
void* playNote(void *var_arg){
	beep();
	return NULL;
}

void printN(uint8_t notes, uint8_t conv){
	uint8_t n = notes;
	//printf("%x\n", n);
	move(5,0);
	printw("                 ");
	for(int i = 0; i < 7; i++){
		if(n%2==0){
			if(conv==48){
				mvprintw(5,5*i,"%s| ", thenoteEU[i]);
			} else {
				mvprintw(5,5*i,"%s| ", thenoteUS[i]);
			}
			n=n>>1;
			if(durations[i]==0) durations[i]=clock();
			//mvprintw(6,i*4,"%ld | ",clock() - durations[i]);
		} else {
			mvprintw(5,i*5,"  | ");
			//mvprintw(6,i*4,"  |");
			n=n>>1;
			durations[i]=0;
		}
		
	}
	refresh();
}

uint8_t atoh(unsigned char c1, unsigned char c2){
	uint8_t val = 0x0;
	if(c1>=97 && c1 <=102){
		val += (c1-87)<<4;
	} else {
		val += (c1-48)<<4;
	}
	if(c2>=97 && c2 <=102){
		val += (c2-87);
	} else {
		val += (c2-48);
	}
	return val;
}

int openCom(int fd){
    //Setup serial port
	printw("Setup serial port ttyACM0...");
	refresh();
	struct termios options;
	tcgetattr(fd, &options);
	cfsetispeed(&options, B19200);
	cfsetospeed(&options, B19200);
	options.c_cflag |= (CLOCAL | CREAD);
	tcsetattr(fd, TCSANOW, &options);
	//8 bit characters	
	options.c_cflag &= ~CSIZE; /* Mask the character size bits */
	options.c_cflag |= CS8;    /* Select 8 data bits */
	//No parity
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	sleep(2); //required to make flush work, for some reason
    tcflush(fd,TCIOFLUSH);
    return 1;
}