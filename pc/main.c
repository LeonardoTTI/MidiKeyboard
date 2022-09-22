#include <stdio.h>
#include <string.h>
#include <fcntl.h> 
#include <errno.h> 
#include <termios.h> 
#include <unistd.h>



int main(){

    int serial_port = open("/dev/ttyACM0", O_RDWR);

    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return 1;
    }

    printf("Ciao, sei in ascolto su /dev/ttyACM0\n");
    //errori comuni errno=13 Permission denied -> $ sudo adduser $USER dialout

    ///

    struct termios tty;

    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 2;
    }

    tty.c_cflag &= ~PARENB;         //disabilitiamo bit di parità
    tty.c_cflag &= ~CSTOPB;     
    tty.c_cflag |= CS8;             // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;  // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;           // Disable echo
    tty.c_lflag &= ~ISIG;           // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;
    cfsetispeed(&tty, B9600);       //baud rate input 
    cfsetospeed(&tty, B9600);       //baud rate output

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return 3;
    }

    ///

    char read_buf [256];
    memset(&read_buf, '\0', sizeof(read_buf));

    //do {
        int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

        if (num_bytes < 0) {
            printf("Error reading: %s", strerror(errno));
            return 4;
        } else if (num_bytes > 0) {
            printf("Read %i bytes. Received message: %s", num_bytes, read_buf);
        }

    //} while(1)

    close(serial_port);

    printf("\nTerminato con successo!\n");

    return 0; // success

}