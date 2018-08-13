#ifndef __DATABASE_HANDLER_H__
#define __DATABASE_HANDLER_H__

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

/* Define */
#define SERVER_NAME "localhost"
#define SERVER_PORT 5001

/* Data Types */
typedef struct
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
} MyTime_t;
typedef struct
{
    int (*init)(void);
    int (*sendReadSensorDataRequest)(MyTime_t begin, MyTime_t end);
    int (*readSensorData)(char *buffer, int size);
    int (*writeForecastResultRequest)(MyTime_t date, int isRain, int amountOfRain);
    void (*close)(void);
} DatabaseHandler_t;

/* Methods */
int __init(void);
int __sendReadSensorData(MyTime_t begin, MyTime_t end);
int __readSensorData(char *buffer, int size);
int __writeForecastResult(MyTime_t date, int isRain, int amountOfRain);
void __close(void);

#endif /* __DATABASE_HANDLER_H__ */
