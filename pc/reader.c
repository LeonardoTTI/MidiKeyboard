#include "processing.c"
#include <ncurses.h>

unsigned char Hb[256];
int pos = 0;

int main(){
	int win_cols = system("tput cols");
	int win_rows = system("tput lines");
	initscr();
	cbreak();
	noecho();
	win = newwin(win_rows,win_cols,0,0);
	refresh();
	int topbottom = '-';
	int leftright = '|';
	box(win, (int)leftright, (int)topbottom);
	wrefresh(win);

	//Hello
	mvwprintw(win,rows++,1,"Hi, this is MidiKeyboard by LeonardoTTI");
	wrefresh(win);	
	wtimeout(win, 5000);
	getch();
	mvwprintw(win,rows,1,"Opening com on /dev/ttyACM0...");
	wrefresh(win);
	sleep(1);
	int ret ;
	system("sudo chmod 777 /dev/ttyACM0 -f");
	system("sudo modprobe pcspkr");
	int portName = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);//Open communication with arduino	
	if(portName==-1){
		mvwprintw(win, rows++, 33," KO");
		mvwprintw(win,rows++,1,"help: sudo chmod 777 /dev/ttyACM0 or check cable");
		mvwprintw(win,rows++,1,"Goodbye, press any key to end");
		wrefresh(win);
		getch();
		endwin();
		return -1;
	}
	mvwprintw(win, rows++, 33,"OK");
	wrefresh(win);



	ret = openCommu(portName);
	if(!ret){
		mvwprintw(win, rows++, 30," KO");
		mvwprintw(win,rows++,1,"Goodbye, press any key to end");
		wrefresh(win);
		getch();
		endwin();
		return -1;
	}
	mvwprintw(win, rows++, 30,"OK                                     ");
	mvwprintw(win,rows,1,"EU(0) or US(1) convention?");
	wrefresh(win);
	int conv = getch();
	mvwprintw(win,rows++,1,"Menu: (p)=pause; (q)=quit; (c)=switch convention           ");
	//mvwprintw(win,rows,1,"        |        |        |        |        |        |");
	//mvwprintw(win,rows+1,1,"       0|       0|       0|       0|       0|       0|");
	wrefresh(win);

	int com = 0;
	timeout(50);
	//Read 
	while( com!=113 ) {
		com = getch();
		if(com==99){if(conv==48){conv++;}else{conv--;}}
		printT();
		if (read(portName, Hb+pos, 1) == 0) continue;
		printN((uint8_t)Hb[pos], conv);
		mvwprintw(win, rows+2, 1,"buffer position: %d, send: %u\n",pos,Hb[pos]);
		if(pos>=255){pos=0;memset(Hb, '\0', sizeof(Hb));}
		pos++;
	}
	printw(" ");
	wrefresh(win);
	//Close communication and free win;
	endwin();
	close(portName);
	
	printf("Goodbye World\n");

	return 0;
}