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

int main(){

	//Hello
	printf("MidiKeyboard by DeucoTTI\n");	
	sleep(1);

	//Open communication with arduino	
	printf("Connessione con /dev/ttyACM0...");
	system("chmod 777 /dev/ttyACM0");
	int portName = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);

	if(portName==-1){
		printf(" KO\n");
		printf("help: cd /dev/ -> sudo chmod 777 ttyACM0 or check cable\n");
		printf("Goodbye :'C\n");
		return -1;
	}

	printf(" OK\n");

	//Setup serial port
	printf("Setup serial port ttyACM0...");
	struct termios options;
	tcgetattr(portName, &options);
	cfsetispeed(&options, B19200);
	cfsetospeed(&options, B19200);
	options.c_cflag |= (CLOCAL | CREAD);
	tcsetattr(portName, TCSANOW, &options);
	//8 bit characters	
	options.c_cflag &= ~CSIZE; /* Mask the character size bits */
	options.c_cflag |= CS8;    /* Select 8 data bits */
	//No parity
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	sleep(2); //required to make flush work, for some reason
	printf(" OK\n\n----------------------------\n");
    tcflush(portName,TCIOFLUSH);
	sleep(4);
	//Read 
	unsigned char Hb[256];
	int pos = 0;
	while( pos < 256 ) {
		if (read(portName, Hb+pos, 1) == 0) continue;		
		printf("%c",Hb[pos]);
		pos++;
		if(pos==256){
			pos=0;
			Hb[0]='\0';
		}
	}
	
	//Close communication
	close(portName);
	
	printf("Goodbye World\n");

	return 0;
}