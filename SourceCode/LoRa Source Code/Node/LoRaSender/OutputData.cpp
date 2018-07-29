#include "OutputData.h"

int stcOutputData_Create(stcOutputData_t **stcOutputDataStructure)
{
  int successFlag = 1; // Suppose that it successful already
  if ((*stcOutputDataStructure) == NULL)
  {
    (*stcOutputDataStructure) = (stcOutputData_t *)malloc(sizeof(stcOutputData_t));
  }

  if ((*stcOutputDataStructure) != NULL)
  {
    (*stcOutputDataStructure)->airDirection = 0;
    (*stcOutputDataStructure)->airSpeed1Min = 0;
    (*stcOutputDataStructure)->airSpeed5Min = 0;
    (*stcOutputDataStructure)->temperature = 0;
    (*stcOutputDataStructure)->rainfall1Hour = 0;
    (*stcOutputDataStructure)->rainfall24Hour = 0;
    (*stcOutputDataStructure)->humidity = 0;
    (*stcOutputDataStructure)->atmosphere = 0;
  }
  else
  {
    successFlag = 0;
  }

  return successFlag;
}

int stcOutputData_Parse(char *inputString, stcOutputData_t **stcOutputDataStructure)
{
  int successFlag = 1; // Suppose that it successful already

  char airDirectionStr[4];
  char airSpeed1MinStr[4];
  char airSpeed5MinStr[4];
  char temperatureStr[4];
  char rainfall1HourStr[4];
  char rainfall24HourStr[4];
  char humidityStr[3];
  char atmosphereStr[6];

  airDirectionStr[3] = '\0';
  airSpeed1MinStr[3] = '\0';
  airSpeed5MinStr[3] = '\0';
  temperatureStr[3] = '\0';
  rainfall1HourStr[3] = '\0';
  rainfall24HourStr[3] = '\0';
  humidityStr[2] = '\0';
  atmosphereStr[5] = '\0';

  if ((inputString != NULL) && ((strlen(inputString) == 33) || (strlen(inputString) == 35))) // If input string contain carriage return and new line character, the lengeth will be 35. Otherwise, it will be 33
  {
    if (stcOutputData_Create(stcOutputDataStructure) == 1)
    {
      strncpy(airDirectionStr, inputString + 1, sizeof(airDirectionStr) - 1);
      strncpy(airSpeed1MinStr, inputString + 5, sizeof(airSpeed1MinStr) - 1);
      strncpy(airSpeed5MinStr, inputString + 9, sizeof(airSpeed5MinStr) - 1);
      strncpy(temperatureStr, inputString + 13, sizeof(temperatureStr) - 1);
      strncpy(rainfall1HourStr, inputString + 17, sizeof(rainfall1HourStr) - 1);
      strncpy(rainfall24HourStr, inputString + 21, sizeof(rainfall24HourStr) - 1);
      strncpy(humidityStr, inputString + 25, sizeof(humidityStr) - 1);
      strncpy(atmosphereStr, inputString + 28, sizeof(atmosphereStr) - 1);

      (*stcOutputDataStructure)->airDirection = (uint16_t)atoi(airDirectionStr);
      (*stcOutputDataStructure)->airSpeed1Min = (uint16_t)atoi(airSpeed1MinStr);
      (*stcOutputDataStructure)->airSpeed5Min = (uint16_t)atoi(airSpeed5MinStr);
      (*stcOutputDataStructure)->temperature = (uint16_t)atoi(temperatureStr);
      (*stcOutputDataStructure)->rainfall1Hour = (uint16_t)atoi(rainfall1HourStr);
      (*stcOutputDataStructure)->rainfall24Hour = (uint16_t)atoi(rainfall24HourStr);
      (*stcOutputDataStructure)->humidity = (uint8_t)atoi(humidityStr);
      (*stcOutputDataStructure)->atmosphere = (uint32_t)atoi(atmosphereStr);
    }
    else
    {
      successFlag = 0;
    }
  }
  else
  {
    successFlag = 0;
  }

  return successFlag;
}

void stcOutputData_ToString(stcOutputData_t *stcOutputDataStructure, char *buffer)
{
  sprintf(buffer, "AirDirection: %d\r\nAirSpeed1Min: %d\r\nAirSpeed5Min: %d\r\nTemperature: %d\r\nRainfall1Hour: %d\r\nRainfall24Hour: %d\r\nHumidity: %d\r\nAtmosphere: %d",
          stcOutputDataStructure->airDirection,
          stcOutputDataStructure->airSpeed1Min,
          stcOutputDataStructure->airSpeed5Min,
          stcOutputDataStructure->temperature,
          stcOutputDataStructure->rainfall1Hour,
          stcOutputDataStructure->rainfall24Hour,
          stcOutputDataStructure->humidity,
          stcOutputDataStructure->atmosphere);
}

void stcOutputData_ToDataString(stcOutputData_t *stcOutputDataStructure, char *buffer)
{
  sprintf(buffer, "c%.3ds%.3dg%.3dt%.3dr%.3dp%.3dh%.2db%.5d\r\n\0",
          stcOutputDataStructure->airDirection,
          stcOutputDataStructure->airSpeed1Min,
          stcOutputDataStructure->airSpeed5Min,
          stcOutputDataStructure->temperature,
          stcOutputDataStructure->rainfall1Hour,
          stcOutputDataStructure->rainfall24Hour,
          stcOutputDataStructure->humidity,
          stcOutputDataStructure->atmosphere);
}

void stcOutputData_Delete(stcOutputData_t *stcOutputDataStructure)
{
  if (stcOutputDataStructure != NULL)
  {
    free(stcOutputDataStructure);
  }
}
