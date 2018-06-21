#include <stdio.h>
#include "OutputData.h"

int main()
{
    char *input[] = {
        "c000s000g000t...r000p000h..b10064",
        "c000s000g000t...r000p000h..b10063",
        "c000s000g000t...r000p000h..b10063",
        "c000s000g000t...r000p000h..b10063",
        "c000s000g000t...r000p000h..b10063",
        "c000s000g000t...r000p000h..b10064",
        "c000s000g000t...r000p000h..b10063",
        "c000s000g000t...r000p000h..b10062",
        "c000s000g000t...r000p000h..b10064",
        "c000s000g000t...r000p000h..b10063",
        "c000s000g000t...r000p000h..b10063",
        "c000s000g000t...r000p000h..b10063",
        "c000s000g000t...r000p000h..b10063",
        "c000s000g000t...r000p000h..b10064",
        "c000s000g000t...r000p000h..b10064",
        "c000s000g000t...r000p000h..b10064",
        "c000s000g000t...r000p000h..b10064",
        "c000s000g000t...r000p000h..b10064",
        "c000s000g000t...r000p000h..b10064",
        "c000s000g000t...r000p000h..b10064",
        "c000s000g000t...r000p000h..b10062",
        "c000s000g000t...r000p000h..b10063",
        "c000s000g000t...r000p000h..b10062",
        "c000s000g000t...r000p000h..b10063",
        "c000s000g000t...r000p000h..b10063",
        "c000s000g000t...r000p000h..b10062",
        "c000s000g000t...r000p000h..b10063",
        "c001s000g000t...r000p000h..b10063"};
    int inputSize = sizeof(input) / sizeof(input[0]);
    stcOutputData_t **structArray;
    char buffer[255];
    
    structArray = (stcOutputData_t **)malloc(sizeof(stcOutputData_t *) * inputSize);

    for (int i = 0; i < inputSize; i++)
    {
        printf("Parse element %d, result: %d\n", i, stcOutputData_Parse(input[i], &structArray[i]));
    }
    
    for (int i = 0; i < inputSize; i++)
    {
        stcOutputData_ToString(structArray[i], buffer);
        printf("%s\n", buffer);
        stcOutputData_Delete(structArray[i]);
    }

    return 0;
}
