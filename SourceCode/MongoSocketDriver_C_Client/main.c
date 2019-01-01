#include <stdio.h>
#include "DatabaseHandler.h"

#define nodeId "Weather_Node_01"

extern DatabaseHandler_t DatabaseHandler;

int main(int argc, char *argv[])
{
    FILE *fp;
    int i, j;
    char buffer[50];
    MyTime_t time = {
        2018,
        10,
        18,
        0,
        0};
    MyTime_t time2 = {
        2017,
        7,
        9,
        20,
        30};

    fp = fopen("out.txt", "w");

    DatabaseHandler.init();
    DatabaseHandler.writeForecastResultRequest(nodeId, time, 1, 100);
    // i = DatabaseHandler.sendReadSensorDataRequest(nodeId, time2, time);
    // if (i)
    // {
    //     j = DatabaseHandler.readSensorData(buffer, 50);
    //     while (j > 0)
    //     {
    //         // Process here
    //         printf("%s", buffer);
    //         fputs(buffer, fp);
    //         j = DatabaseHandler.readSensorData(buffer, 50);
    //     }
    // }
    // else
    // {
    //     printf("Send read request fail\n");
    // }
    DatabaseHandler.close();
    
    fclose(fp);
    
    return 0;
}

