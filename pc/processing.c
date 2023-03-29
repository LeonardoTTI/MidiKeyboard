#include <stdio.h>	 /* Standard input/output definitions */
#include <string.h>	 /* String function definitions */
#include <unistd.h>	 /* UNIX standard function definitions */
#include <fcntl.h>	 /* File control definitions */
#include <errno.h>	 /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>	 /* Standard types */
#include <string.h>	 /* String function definitions */
#include <unistd.h>	 /* UNIX standard function definitions */
#include <fcntl.h>	 /* File control definitions */
#include <errno.h>	 /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>
#include <ncurses.h>

int rows = 1;

WINDOW *win;
clock_t durations[7] = {0, 0, 0, 0, 0, 0, 0};
char *thenoteUS[] = {" C", " D", " E", " F", " G", " A", " B"};
char *thenoteEU[] = {" DO", " RE", " MI", " FA", "SOL", " LA", " SI"};
char *note[] = {"aplay -q ./notes/piano-c_C_major.wav",
				"aplay -q ./notes/piano-d_D_major.wav",
				"aplay -q ./notes/piano-e_E_major.wav",
				"aplay -q ./notes/piano-f_F_major.wav",
				"aplay -q ./notes/piano-g_G_major.wav",
				"aplay -q ./notes/piano-a_A_major.wav",
				"aplay -q ./notes/piano-b_B_major.wav"};

void *playNote(void *var_arg)
{
	system(note[*((int *)var_arg)]);
	return NULL;
}

void printN(uint8_t notes, uint8_t conv)
{
	pthread_t thread_id[7];
	uint8_t n = notes;
	mvwprintw(win, rows, 1, "                                   ");
	for (int i = 0; i < 7; i++)
	{
		if (n % 2 == 0)
		{
			if (conv == 48)
			{
				mvwprintw(win, rows, (9 * i) + 1, "     %s|", thenoteEU[i]);
				if (durations[i] == 0)
				{
					int press2 = i;
					if (pthread_create(&(thread_id[i]), NULL, playNote, &press2) != 0)
					{
						mvwprintw(win, rows + 3, 1, "Error: failed to play the note %s", thenoteEU[i]);
						break;
					}
					else
					{
						pthread_detach(thread_id);
					}
				}
			}
			else
			{
				mvwprintw(win, rows, (9 * i) + 1, "      %s|", thenoteUS[i]);
				if (durations[i] == 0)
				{
					int press1 = i;
					if (pthread_create(&(thread_id[i]), NULL, playNote, &press1) != 0)
					{
						mvwprintw(win, rows + 3, 1, "Error: failed to play the note %s", thenoteUS[i]);
						break;
					}
					else
					{
						pthread_detach(thread_id);
					}
				}
			}
			if (durations[i] == 0)
				durations[i] = clock();
			n = n >> 1;
		}
		else
		{
			durations[i] = 0;
			mvwprintw(win, rows, (9 * i) + 1, "        |");
			n = n >> 1;
		}
	}
	wrefresh(win);
}
void printT()
{
	for (int i = 0; i < 7; i++)
	{
		if (durations[i] != 0)
			mvwprintw(win, rows + 1, (9 * i) + 1, "  %f|", ((double)(clock() - durations[i]) / CLOCKS_PER_SEC) * 550);
		else
			mvwprintw(win, rows + 1, (9 * i) + 1, "        0 ");
	}
	wrefresh(win);
}

uint8_t atoh(unsigned char c1, unsigned char c2)
{
	uint8_t val = 0x0;
	if (c1 >= 97 && c1 <= 102)
	{
		val += (c1 - 87) << 4;
	}
	else
	{
		val += (c1 - 48) << 4;
	}
	if (c2 >= 97 && c2 <= 102)
	{
		val += (c2 - 87);
	}
	else
	{
		val += (c2 - 48);
	}
	return val;
}

int openCommu(int fd)
{

	mvwprintw(win, rows, 1, "Setup serial port ttyACM0...");
	wrefresh(win);

	struct termios options; // Setup serial port
	tcgetattr(fd, &options);
	cfsetispeed(&options, B19200);
	cfsetospeed(&options, B19200);
	options.c_cflag |= (CLOCAL | CREAD);
	tcsetattr(fd, TCSANOW, &options);
	// 8 bit characters
	options.c_cflag &= ~CSIZE; /* Mask the character size bits */
	options.c_cflag |= CS8;	   /* Select 8 data bits */
	// No parity
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	sleep(2); // required to make flush work, for some reason
	tcflush(fd, TCIOFLUSH);
	return 1;
}