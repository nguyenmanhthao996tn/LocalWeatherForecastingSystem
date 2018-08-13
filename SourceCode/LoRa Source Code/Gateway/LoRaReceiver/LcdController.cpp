#include "LcdController.h"

int __getResultScreenString(char *buffer, int size, unsigned int rainfall)
{
    if ((buffer != NULL) && (size > 0) && (rainfall < 10000))
    {
        snprintf(buffer, size, RESULT_SCREEN_TEMPLATE, rainfall);

        return 1;
    }

    return 0;
}

int __getTimeScreenString(char *buffer, unsigned int size, unsigned int hour, unsigned int min, unsigned int temperature, unsigned int humidity)
{
    if ((buffer != NULL) && (size > 0) && (hour < 24) && (min < 60) && (temperature < 100) && (humidity < 100))
    {
        snprintf(buffer, size, TIME_SCREEN_TEMPLATE, hour, min, temperature, humidity);

        return 1;
    }

    return 0;
}

LcdController_t LcdController = {
    __getResultScreenString,
    __getTimeScreenString};
