/**
   @file OutputData.h
   @brief Library for managing data output string received from STC15
   @author Thao Nguyen Manh
   @modified June 21th 2018, 09:57
*/

#ifndef __OUTPUT_DATA_H__
#define __OUTPUT_DATA_H__

/**************** INCLUDE ****************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**************** DATA STRUCTURE ****************/
typedef struct
{
  uint16_t airDirection;
  uint16_t airSpeed1Min;
  uint16_t airSpeed5Min;
  uint16_t temperature;
  uint16_t rainfall1Hour;
  uint16_t rainfall24Hour;
  uint8_t humidity;
  uint32_t atmosphere;
} stcOutputData_t;

/**************** METHODS ****************/
/**
   @brief Create a new struct of stcOutputData_t
   @param stcOutputDataStructure Pointer to stcOutputData_t to be create
   @return Return 1 on creating successfully
*/
int stcOutputData_Create(stcOutputData_t **stcOutputDataStructure);

/**
   @brief Get data from input string and put it in stcOutputData_t struct that stcOutputDataStructure point to
   @param inputString Input data string to get data
   @param stcOutputDataStructure Pointer point to struct that storage data
   @return Return 1 on parsing successfully
*/
int stcOutputData_Parse(char *inputString, stcOutputData_t **stcOutputDataStructure);

/**
   @brief Convert a struct of stcOutputData_t to string for printing on console
   @param stcOutputDataStructure Structure to convert
   @param buffer Character array to print
   @return Void
*/
void stcOutputData_ToString(stcOutputData_t *stcOutputDataStructure, char *buffer);

/**
   @brief Convert a struct of stcOutputData_t to data string same as input string from value of object
   @param stcOutputDataStructure Structure to convert
   @param buffer Character array to print
   @return Void
*/
void stcOutputData_ToDataString(stcOutputData_t *stcOutputDataStructure, char *buffer);

/**
   @brief Free and delete stcOutputDataStructure pointer
   @param Pointer to be delete
   @return Void
*/
void stcOutputData_Delete(stcOutputData_t *stcOutputDataStructure);

#endif /* __OUTPUT_DATA_H__ */
