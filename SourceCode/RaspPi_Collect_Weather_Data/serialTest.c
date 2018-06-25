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
#include <time.h>

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
void saveToText(stcOutputData_t **objectArray, int count);
int getMostAppear(uint8_t *counterArray, int size);
void getCurrentTimeString(char *buffer, int size);
void getFilename(char *buffer, int size);

/********************* MAIN *********************/
int main(int argc, char *argv[])
{
  char buffer[40];
  // char consoleBuffer[1024];

  stcOutputData_t *dataObject = NULL;
  stcOutputData_t *dataObjectArray[60] = {NULL};
  int dataObjectCounter = 0;

  setupVariables();
  setupSerialPort(&oldTSetting);

  printf("Setting up complete!\n");

  while (true)
  {
    if (myBuffer_Read(buffer))
    {
      if (stcOutputData_Parse(buffer, &dataObject))
      {
        // stcOutputData_ToString(dataObject, consoleBuffer);
        // printf("%s\n", consoleBuffer);

        // printf("dataObjectCounter: %d\n", dataObjectCounter + 1);
        dataObjectArray[dataObjectCounter] = dataObject;
        dataObjectCounter++;
        if (dataObjectCounter >= 60)
        {
          // Process
          saveToText(dataObjectArray, dataObjectCounter);

          // Clean memory
          dataObjectCounter = 0;
          for (int i = 0; i < 60; i++)
          {
            stcOutputData_Delete(dataObjectArray[i]);
          }
        }

        dataObject = NULL;
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

void saveToText(stcOutputData_t **objectArray, int count)
{
  char filename[255];
  FILE *f;

  uint16_t airDirection = 0;
  uint8_t airDirectionCounterArray[8] = {0};
  uint16_t outputAirDirectionValue[] = {0, 45, 90, 135, 180, 225, 270, 315};

  char currentTimeString[255];
  uint32_t airSpeed1Min = 0;
  uint32_t airSpeed5Min = 0;
  uint32_t temperature = 0;
  uint32_t rainfall1Hour = 0;
  uint32_t rainfall24Hour = 0;
  uint32_t humidity = 0;
  uint32_t atmosphere = 0;

  char buffer[255];

  stcOutputData_t *currentObject = NULL;

  for (int i = 0; i < count; i++)
  {
    currentObject = objectArray[i];

    // airDirection
    switch (currentObject->airDirection)
    {
    case 0:
      airDirectionCounterArray[0]++;
      break;
    case 45:
      airDirectionCounterArray[1]++;
      break;
    case 90:
      airDirectionCounterArray[2]++;
      break;
    case 135:
      airDirectionCounterArray[3]++;
      break;
    case 180:
      airDirectionCounterArray[4]++;
      break;
    case 225:
      airDirectionCounterArray[5]++;
      break;
    case 270:
      airDirectionCounterArray[6]++;
      break;
    case 315:
      airDirectionCounterArray[7]++;
      break;
    default:
      break;
    }

    // airSpeed1Min
    airSpeed1Min += currentObject->airSpeed1Min;

    // airSpeed5Min
    airSpeed5Min += currentObject->airSpeed5Min;

    // temperature
    temperature += currentObject->temperature;

    // rainfall1Hour
    rainfall1Hour += currentObject->rainfall1Hour;

    // rainfall24Hour
    rainfall24Hour += currentObject->rainfall24Hour;

    // humidity
    humidity += currentObject->humidity;

    // atmosphere
    atmosphere += currentObject->atmosphere;
  }

  airDirection = outputAirDirectionValue[getMostAppear(airDirectionCounterArray, 8)];
  airSpeed1Min /= count;
  airSpeed5Min /= count;
  temperature /= count;
  rainfall1Hour /= count;
  rainfall24Hour /= count;
  humidity /= count;
  atmosphere /= count;

  getCurrentTimeString(currentTimeString, 255);

  // Write to file
  snprintf(buffer, 255, "%s %d %d %d %d %d %d %d %d", currentTimeString, airDirection, airSpeed1Min, airSpeed5Min, temperature, rainfall1Hour, rainfall24Hour, humidity, atmosphere);
  // printf("%s\n", buffer);
  getFilename(filename, 255);
  f = fopen(filename, "a");
  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }

  fprintf(f, "%s\n", buffer);

  fclose(f);
}

int getMostAppear(uint8_t *counterArray, int size)
{
  int i = 0;
  uint8_t maxValue = counterArray[0];
  int maxIndex = 0;

  printf("Value: %d ", counterArray[0]);

  for (i = 1; i < size; i++)
  {
    printf("%d ", counterArray[i]);
    if (counterArray[i] >= maxValue)
    {
      maxValue = counterArray[i];
      maxIndex = i;
    }
  }

  printf("| Max: %d | Pos: %d\n", maxValue, maxIndex);

  return maxIndex;
}

void getCurrentTimeString(char *buffer, int size)
{
  time_t now;
  time(&now);

  strftime(buffer, size, "%FT%TZ", gmtime(&now));
}

void getFilename(char *buffer, int size)
{
  time_t now;
  time(&now);

  strftime(buffer, size, "%FT.txt", gmtime(&now));
}
