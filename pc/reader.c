#include "processing.c"
#include <ncurses.h>



int main(){
	printf("\a");
	initscr();
	//Hello
	printw("---------------------------\n");
	printw("MidiKeyboard by LeonardoTTI\n");	
	sleep(1);
	//Open communication with arduino	
	printw("Connessione con /dev/ttyACM0...");
	system("chmod 777 /dev/ttyACM0");
	int portName = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
	refresh();
	if(portName==-1){
		printw(" KO\n");
		printw("help: cd /dev/ -> sudo chmod 777 ttyACM0 or check cable\n");
		printw("Goodbye :'C\n");
		endwin();
		return -1;
	}
	printw(" OK\n");
	int ret = openCom(portName);
	if(!ret){
		printw("KO\n");
		printw("Goodbye :'C\n");
		endwin();
		return -1;
	}
	printw(" OK\n----------------------------\n");
	printw("EU(0) or US(1) convention?");
	refresh();
	int conv = getch();
	mvprintw(5,0,"Ready                          ");
	refresh();
	//printw("%d",conv);



	//Read 
	unsigned char Hb[256];
	int pos = 0;
	
	while( pos < 256 ) {
		if (read(portName, Hb+pos, 1) == 0) continue;
		//printw("pos: %d, send: %c\n",pos,Hb[pos]);
		refresh();
		if(pos%2!=0){
			printN(atoh(Hb[pos-1],Hb[pos]), conv);
		}
		pos++;
		if(pos==256){
			pos=0;
			memset(Hb, '\0', sizeof(Hb));
		}
		int c = getch();
	}
	endwin();
	//Close communication
	close(portName);
	
	printf("Goodbye World\n");

	return 0;
}