#ifndef __LCD_CONTROLLER_H__
#define __LCD_CONTROLLER_H__

#include <stdio.h>

/* CONSTANT */
#define RESULT_SCREEN_TEMPLATE "C01R%.4d\r\n"
#define TIME_SCREEN_TEMPLATE "C02H%.2dM%.2dT%.2dH%.2d\r\n"

/* DATA TYPE */
typedef struct {
int (*getResultScreenString)(char *buffer, int size, unsigned int rainfall);
int (*getTimeScreenString)(char *buffer, unsigned int size, unsigned int hour, unsigned int min, unsigned int temperature, unsigned int humidity);    
} LcdController_t;

/* METHOD */
int __getResultScreenString(char *buffer, int size, unsigned int rainfall);
int __getTimeScreenString(char *buffer, unsigned int size, unsigned int hour, unsigned int min, unsigned int temperature, unsigned int humidity);

/* GLOBAL VARIABLE */
extern LcdController_t LcdController;

#endif /* __LCD_CONTROLLER_H__ */
