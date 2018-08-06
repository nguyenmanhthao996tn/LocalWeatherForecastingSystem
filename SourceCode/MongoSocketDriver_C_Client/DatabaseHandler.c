#include "DatabaseHandler.h"

/* Global Variable */
DatabaseHandler_t DatabaseHandler = {
    __init,
    __sendReadSensorData,
    __readSensorData,
    __writeForecastResult,
    __close};

static int __sockfd;

int __init(void)
{
    struct hostent *server;
    struct sockaddr_in serv_addr;

    /* Create a socket point */
    __sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (__sockfd < 0)
    {
        perror("ERROR opening socket");
        return 0; // FAIL
    }

    server = gethostbyname(SERVER_NAME);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        return 0; // FAIL
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(SERVER_PORT);

    /* Now connect to the server */
    if (connect(__sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        return 0; // FAIL
    }

    return 1; // Success
}

int __sendReadSensorData(MyTime_t begin, MyTime_t end)
{
    int n;
    char buffer[255];
    const char *requestTemplate = "{\"code\":0, \"from\":\"%.4d-%.2d-%.2d %.2d:%.2d\", \"to\":\"%.4d-%.2d-%.2d %.2d:%.2d\"}";

    // Parse template
    bzero(buffer, 255);
    snprintf(buffer, 255, requestTemplate, begin.year, begin.month, begin.day, begin.hour, begin.minute, end.year, end.month, end.day, end.hour, end.minute);

    /* Send request to the server */
    n = write(__sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        return 0; // FAIL
    }

    /* Read server response */
    bzero(buffer, 255);
    n = read(__sockfd, buffer, 15);

    if (n > 0)
    {
        if (strcmp("{\"status\":\"OK\"}", buffer) == 0)
        {
            return 1; // Success
        }
        else
        {
            return 0; // FAIL
        }
    }
    else
    {
        return 0; // FAIL
    }
}

int __readSensorData(char *buffer)
{
    int n;
    bzero(buffer, 50);
    n = read(__sockfd, buffer, 50);

    if (n > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int __writeForecastResult(MyTime_t date, int isRain, int amountOfRain)
{
    int n;
    const char *requestTemplate = "{\"code\":2, \"date\":\"%.4d-%.2d-%.2d %.2d:%.2d\", \"rain\":%s, \"AmountOfRain\":%d}";
    char buffer[255];

    // Parse template
    bzero(buffer, 255);
    snprintf(buffer, 255, requestTemplate, date.year, date.month, date.day, date.hour, date.minute, ((isRain == 0) ? "false" : "true"), amountOfRain);

    /* Send request to the server */
    n = write(__sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        return 0; // FAIL
    }

    /* Read server response */
    bzero(buffer, 255);
    n = read(__sockfd, buffer, 255);

    if (n > 0)
    {
        if (strcmp("{\"status\":\"OK\"}", buffer) == 0)
        {
            return 1; // Success
        }
        else
        {
            return 0; // FAIL
        }
    }
    else
    {
        return 0; // FAIL
    }
}

void __close(void)
{
    close(__sockfd);
}
