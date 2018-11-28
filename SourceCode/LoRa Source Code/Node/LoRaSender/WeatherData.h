#ifndef __WEATHER_DATA_H__
#define __WEATHER_DATA_H__

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Define masks */
#define NodeId15_8_MASK         0xFF
#define NodeId7_0_MASK          0xFF
#define Time7_0_MASK            0xFF
#define Time10_8_MASK           0xE0
#define Battery_MASK            0x1F
#define AirDirection_MASK       0xE0
#define AirSpeed1Min9_5_MASK    0x1F
#define AirSpeed1Min4_0_MASK    0xF8
#define Temperature2_0_MASK     0x07
#define Temperature6_3_MASK     0xF0
#define AirSpeed5Min3_0_MASK    0x0F
#define AirSpeed5Min9_4_MASK    0xFC
#define Rainfall1H9_8_MASK      0x03
#define Rainfall1H7_0_MASK      0xFF
#define Rainfall24H9_2_MASK     0xFF
#define Rainfall24H1_0_MASK     0xC0
#define NodeKey6_1_MASK         0x3F
#define Humidity_MASK           0xFE
#define NodeKey0_MASK           0x01
#define Atmosphere15_8_MASK     0xFF
#define Atmosphere7_0_MASK      0xFF
#define CRC_MASK                0xFF

/* Define positions */
#define NodeId15_8_POSITION         0
#define NodeId7_0_POSITION          1
#define Time7_0_POSITION            2
#define Time10_8_POSITION           3
#define Battery_POSITION            3
#define AirDirection_POSITION       4
#define AirSpeed1Min9_5_POSITION    4
#define AirSpeed1Min4_0_POSITION    5
#define Temperature2_0_POSITION     5
#define Temperature6_3_POSITION     6
#define AirSpeed5Min3_0_POSITION    6
#define AirSpeed5Min9_4_POSITION    7
#define Rainfall1H9_8_POSITION      7
#define Rainfall1H7_0_POSITION      8
#define Rainfall24H9_2_POSITION     9
#define Rainfall24H1_0_POSITION     10
#define NodeKey6_1_POSITION         10
#define Humidity_POSITION           11
#define NodeKey0_POSITION           11
#define Atmosphere15_8_POSITION     12
#define Atmosphere7_0_POSITION      13
#define CRC_POSITION                14

/* Define offset */
#define NodeId15_8_OFFSET       0
#define NodeId7_0_OFFSET        0
#define Time7_0_OFFSET          0
#define Time10_8_OFFSET         5
#define Battery_OFFSET          0
#define AirDirection_OFFSET     5
#define AirSpeed1Min9_5_OFFSET  0
#define AirSpeed1Min4_0_OFFSET  3
#define Temperature2_0_OFFSET   0
#define Temperature6_3_OFFSET   4
#define AirSpeed5Min3_0_OFFSET  0
#define AirSpeed5Min9_4_OFFSET  2
#define Rainfall1H9_8_OFFSET    0
#define Rainfall1H7_0_OFFSET    0
#define Rainfall24H9_2_OFFSET   0
#define Rainfall24H1_0_OFFSET   6
#define NodeKey6_1_OFFSET       0
#define Humidity_OFFSET         1
#define NodeKey0_OFFSET         0
#define Atmosphere15_8_OFFSET   0
#define Atmosphere7_0_OFFSET    0
#define CRC_OFFSET              0

/* Methods */
void setNodeId(char *buffer, uint16_t nodeId);
uint16_t getNodeId(char *buffer);
void setTime(char *buffer, uint16_t Time);
uint16_t getTime(char *buffer);
void setBattery(char *buffer, uint8_t Battery);
uint8_t getBattery(char *buffer);
void setAirDirection(char *buffer, uint8_t AirDirection);
uint8_t getAirDirection(char *buffer);
void setAirSpeed1Min(char *buffer, uint16_t AirSpeed1Min);
uint16_t getAirSpeed1Min(char *buffer);
void setAirSpeed5Min(char *buffer, uint16_t AirSpeed5Min);
uint16_t getAirSpeed5Min(char *buffer);
void setTemperature(char *buffer, uint8_t temperature);
uint8_t getTemperature(char *buffer);
void setRainfall1Hour(char *buffer, uint16_t rainfall1Hour);
uint16_t getRainfall1Hour(char *buffer);
void setRainfall24Hour(char *buffer, uint16_t rainfall24Hour);
uint16_t getRainfall24Hour(char *buffer);
void setNodeKey(char *buffer, uint8_t nodeKey);
uint8_t getNodeKey(char *buffer);
void setHumidity(char *buffer, uint8_t humidity);
uint8_t getHumidity(char *buffer);
void setAtmosphere(char *buffer, uint16_t atmosphere);
uint16_t getAtmosphere(char *buffer);
void calculateCRC(char *buffer);
uint8_t getCRC(char *buffer);
uint8_t Crc8(const void *vptr, int len);

#endif /* __WEATHER_DATA_H__ */
