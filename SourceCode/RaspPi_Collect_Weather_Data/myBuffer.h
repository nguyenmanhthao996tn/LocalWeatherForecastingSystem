#ifndef __MY_BUFFER_H__
#define __MY_BUFFER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int myBuffer_Init(unsigned int bufferSize, unsigned int elementLength);
void myBuffer_Delete(void);
int myBuffer_Write(char * dataToWrite);
int myBuffer_Read(char * buffer);

int __myBuffer_isAvailableToWrite(void);
int __myBuffer_isAvailableToRead(void);

#endif /* __MY_BUFFER_H__ */
