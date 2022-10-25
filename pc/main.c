

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h> 
#include <errno.h> 
#include <termios.h> 
#include <unistd.h>
#include <inttypes.h>


char* avr_ports[3]={"/dev/ttyACM0",
                    "/dev/ttyAMC1",
                    "/dev/ttyACM2"};


int main(void){
    printf("Inizio ascolto sulle porte....\n");
    int serial_port;
    int i;
    for(i = 0; i <= 2; i++){
        serial_port = open(avr_ports[i], O_RDWR | O_NOCTTY | O_NDELAY);
        if (serial_port < 0) {
            printf("Error %i from open: %s\n", errno, strerror(errno));
        } else {
            break;
        }
    };
    if (serial_port < 0) {
        printf("Nessun dispositivo trovato, Bye...\n");  
        return 1; 
    }
    


    printf("Ciao, sei in ascolto su %s \n--------------------------------------------\n",avr_ports[i]);
    //errori comuni errno=13 Permission denied -> $ sudo adduser $USER dialout

    ///

    struct termios tty;

    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 2;
    }
    printf("No2\n");

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag |= PARENB;  // Set parity bit, enabling parity
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication
    tty.c_cflag |= CS8; // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    cfsetspeed(&tty, B115200);

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return 3;
    }
    printf("No3\n");

    ///

    char read_buf [256];
    memset(&read_buf, '\0', sizeof(read_buf));
    int num_bytes;
    int j = 0;
    do {
        num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
        //diouxXfeEgGcs
        if (num_bytes < 0) {
            printf("Error reading: %s\n", strerror(errno));
            break;
        } else if (num_bytes > 0) {
            printf("Read %i bytes. Received message: %s" "\n", num_bytes, read_buf);
        } else {
            printf("Read %i bytes.\n", num_bytes);
        }
        j++;
    } while(j<100);

    close(serial_port);

    printf("\n--------------------------------------------\nTerminato con successo!\n");

    return 0; // success

}