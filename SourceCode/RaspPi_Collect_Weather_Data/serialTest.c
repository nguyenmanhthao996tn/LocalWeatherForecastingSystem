/*************************************************************
 * @author ThaoNguyen                                        *
 * @modified 2018/04/17 14:21                                *
 * @desciption This program write every package data         *
 *             received from serial port (ttyUSB0) and time  *
 *             that package received to text file.           *
 *************************************************************/

/********************* LIBRARIES *********************/
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

#include "OutputData.h"
#include "myBuffer.h"

/********************* CONFIGURATIONS *********************/
#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define MAX_DATA_RECEIVE_PACKAGE_COUNTER 26
#define MAX_BYTE_EACH_PACKAGE 5

#define BUFFER_ARRAY_SIZE 32
#define BUFFER_ELEMENT_LENGTH 40

/********************* GLOBAL VARIABLES *********************/
volatile int fd;
volatile struct timespec timeReceived;
volatile struct termios oldTSetting;

/********************* METHODS *********************/
void setupSerialPort(struct termios *oldTSetting);
void signalIO_handler(int status); /* definition of signal handler */
void setupVariables(void);

/********************* MAIN *********************/
int main(int argc, char *argv[])
{
  char buffer[40];
  stcOutputData_t *dataObject = NULL;
  char consoleBuffer[1024];

  setupVariables();
  setupSerialPort(&oldTSetting);

  printf("Setting up complete!\n");

  while (true)
  {
    if (myBuffer_Read(buffer))
    {
      if (stcOutputData_Parse(buffer, &dataObject))
      {
        stcOutputData_ToString(dataObject, consoleBuffer);
        printf("%s\n", consoleBuffer);
      }
    }
    else
    {
      // printf("Buffer empty\n");
    }
  }

  return EXIT_SUCCESS;
}

void setupSerialPort(struct termios *oldTSetting)
{
  struct termios newTSetting;
  struct sigaction saio; /* definition of signal action */

  /* open the device to be non-blocking (read will return immediatly) */
  fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd < 0)
  {
    perror(MODEMDEVICE);
    exit(EXIT_FAILURE);
  }

  /* install the signal handler before making the device asynchronous */
  saio.sa_handler = signalIO_handler;
  sigemptyset(&saio.sa_mask); // saio.sa_mask = 0;
  saio.sa_flags = 0;
  saio.sa_restorer = NULL;
  sigaction(SIGIO, &saio, NULL);

  /* allow the process to receive SIGIO */
  fcntl(fd, F_SETOWN, getpid());

  /* Make the file descriptor asynchronous (the manual page says only 
           O_APPEND and O_NONBLOCK, will work with F_SETFL...) */
  fcntl(fd, F_SETFL, FASYNC);

  /* save current port settings */
  tcgetattr(fd, oldTSetting);

  /* set new port settings for canonical input processing */
  newTSetting.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
  newTSetting.c_iflag = IGNPAR | ICRNL;
  newTSetting.c_oflag = 0;
  newTSetting.c_lflag = ICANON;
  newTSetting.c_cc[VMIN] = 1;
  newTSetting.c_cc[VTIME] = 0;
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &newTSetting);
}

void signalIO_handler(int status)
{
  int res;
  char buffer[40];

  // Get serial data
  res = read(fd, buffer, 40);
  buffer[res] = 0; // Put ENDL

  if (myBuffer_Write(buffer))
  {
    // printf("Buffer wrote\n");
  }
  else
  {
    // printf("Buffer full\n");
  }

  // Print to console
  // printf("received SIGIO signal");
}

void setupVariables(void)
{
  myBuffer_Init(BUFFER_ARRAY_SIZE, BUFFER_ELEMENT_LENGTH);
}