#include "myBuffer.h"

static char **__buffer;
static int __bufferWritePosition, __bufferReadPosition;
static unsigned int __bufferSize, __bufferElementLength;

int myBuffer_Init(unsigned int bufferSize, unsigned int elementLength)
{
    int i, initSuccessfulFlag = 1;
    __bufferSize = bufferSize;
    __bufferElementLength = elementLength;

    __bufferWritePosition = 0;
    __bufferReadPosition = 0;

    __buffer = (char **)malloc(sizeof(char *) * __bufferSize);
    if (__buffer != NULL)
    {
        for (i = 0; i < __bufferSize; i++)
        {
            __buffer[i] = (char *)malloc(__bufferElementLength);
            if (__buffer[i] == NULL)
            {
                initSuccessfulFlag = 0;
                break;
            }
        }
    }
    else
    {
        initSuccessfulFlag = 0;
    }

    return initSuccessfulFlag;
}

void myBuffer_Delete(void)
{
    int i;
    if (__buffer != NULL)
    {
        for (i = 0; i < __bufferSize; i++)
        {
            if (__buffer[i] != NULL)
            {
                free(__buffer[i]);
            }
        }
        free(__buffer);
    }
}

int myBuffer_Write(char *dataToWrite)
{
    int successFlag = __myBuffer_isAvailableToWrite();

    if (successFlag)
    {
        strncpy(__buffer[__bufferWritePosition], dataToWrite, __bufferElementLength);
        __bufferWritePosition++;
        __bufferWritePosition %= __bufferSize;
    }

    return successFlag;
}

int myBuffer_Read(char *buffer)
{
    int successFlag = __myBuffer_isAvailableToRead();

    if (successFlag)
    {
        strncpy(buffer, __buffer[__bufferWritePosition], __bufferElementLength);
        __bufferReadPosition++;
        __bufferReadPosition %= __bufferSize;
    }

    return successFlag;
}

int __myBuffer_isAvailableToWrite(void)
{
    int flag = 1; // Suppose that buffer available

    if ((__bufferWritePosition == (__bufferSize - 1)) && (__bufferReadPosition == 0))
    {
        flag = 0;
    }

    if (__bufferWritePosition == (__bufferReadPosition - 1))
    {
        flag = 0;
    }

    return flag;
}

int __myBuffer_isAvailableToRead(void)
{
    int flag = 1; // Suppose that buffer available

    if (__bufferReadPosition == __bufferWritePosition)
    {
        flag = 0;
    }

    return flag;
}
