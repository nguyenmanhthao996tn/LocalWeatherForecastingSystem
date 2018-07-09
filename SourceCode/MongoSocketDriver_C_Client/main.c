#include <stdio.h>
#include "DatabaseHandler.h"

extern DatabaseHandler_t DatabaseHandler;

int main(int argc, char *argv[])
{
    int i, j, k;
    char buffer[35];
    MyTime_t time = {
        2018,
        7,
        9,
        20,
        30};
    MyTime_t time2 = {
        2017,
        7,
        9,
        20,
        30};

    DatabaseHandler.init();
    // DatabaseHandler.writeForecastResult(time, 1, 100);
    i = DatabaseHandler.sendReadSensorDataRequest(time2, time);
    if (i)
    {
        j = DatabaseHandler.readSensorData(buffer);
        while (j > 0)
        {
            // Process here
            printf("%s", buffer);
            j = DatabaseHandler.readSensorData(buffer);
        }
    }
    else
    {
        printf("Send read request fail\n");
    }
    DatabaseHandler.close();

    return 0;
}
