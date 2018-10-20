#include "WeatherData.h"

void setNodeId(char *buffer, uint16_t nodeId)
{
    buffer[NodeId15_8_POSITION] |= ((nodeId >> 8) << NodeId15_8_OFFSET) & NodeId15_8_MASK;
    buffer[NodeId7_0_POSITION] |= (nodeId << NodeId7_0_OFFSET) & NodeId7_0_MASK;
}

uint16_t getNodeId(char *buffer)
{
    uint16_t result, highByte, lowByte;

    highByte = (buffer[NodeId15_8_POSITION] & NodeId15_8_MASK) >> NodeId15_8_OFFSET;
    lowByte = (buffer[NodeId7_0_POSITION] & NodeId7_0_MASK) >> NodeId7_0_OFFSET;

    result = lowByte;
    result |= ((uint16_t)highByte << 8);

    return result;
}

void setTime(char *buffer, uint16_t Time)
{
    buffer[Time10_8_POSITION] |= ((Time >> 8) << Time10_8_OFFSET) & Time10_8_MASK;
    buffer[Time7_0_POSITION] |= (Time << Time7_0_OFFSET) & Time7_0_MASK;
}

uint16_t getTime(char *buffer)
{
    uint16_t result, highByte, lowByte;

    highByte = (buffer[Time10_8_POSITION] & Time10_8_MASK) >> Time10_8_OFFSET;
    lowByte = (buffer[Time7_0_POSITION] & Time7_0_MASK) >> Time7_0_OFFSET;

    result = lowByte;
    result |= ((uint16_t)highByte << 8);

    return result;
}

void setBattery(char *buffer, uint8_t Battery)
{
    buffer[Battery_POSITION] |= (Battery << Battery_OFFSET) & Battery_MASK;
}

uint8_t getBattery(char *buffer)
{
    uint8_t result;

    result = (buffer[Battery_POSITION] & Battery_MASK) >> Battery_OFFSET;

    return result;
}

void setAirDirection(char *buffer, uint8_t AirDirection)
{
    buffer[AirDirection_POSITION] |= (AirDirection << AirDirection_OFFSET) & AirDirection_MASK;
}

uint8_t getAirDirection(char *buffer)
{
    uint8_t result;

    result = (buffer[AirDirection_POSITION] & AirDirection_MASK) >> AirDirection_OFFSET;

    return result;
}

void setAirSpeed1Min(char *buffer, uint16_t AirSpeed1Min)
{
    buffer[AirSpeed1Min9_5_POSITION] |= ((AirSpeed1Min >> 5) << AirSpeed1Min9_5_OFFSET) & AirSpeed1Min9_5_MASK;
    buffer[AirSpeed1Min4_0_POSITION] |= (AirSpeed1Min << AirSpeed1Min4_0_OFFSET) & AirSpeed1Min4_0_MASK;
}

uint16_t getAirSpeed1Min(char *buffer)
{
    uint16_t result, highByte, lowByte;

    highByte = (buffer[AirSpeed1Min9_5_POSITION] & AirSpeed1Min9_5_MASK) >> AirSpeed1Min9_5_OFFSET;
    lowByte = (buffer[AirSpeed1Min4_0_POSITION] & AirSpeed1Min4_0_MASK) >> AirSpeed1Min4_0_OFFSET;

    result = lowByte;
    result |= ((uint16_t)highByte << 5);

    return result;
}

void setAirSpeed5Min(char *buffer, uint16_t AirSpeed5Min)
{
    buffer[AirSpeed5Min9_4_POSITION] |= ((AirSpeed5Min >> 4) << AirSpeed5Min9_4_OFFSET) & AirSpeed5Min9_4_MASK;
    buffer[AirSpeed5Min3_0_POSITION] |= (AirSpeed5Min << AirSpeed5Min3_0_OFFSET) & AirSpeed5Min3_0_MASK;
}

uint16_t getAirSpeed5Min(char *buffer)
{
    uint16_t result, highByte, lowByte;

    highByte = (buffer[AirSpeed5Min9_4_POSITION] & AirSpeed5Min9_4_MASK) >> AirSpeed5Min9_4_OFFSET;
    lowByte = (buffer[AirSpeed5Min3_0_POSITION] & AirSpeed5Min3_0_MASK) >> AirSpeed5Min3_0_OFFSET;

    result = lowByte;
    result |= ((uint16_t)highByte << 4);

    return result;
}

void setTemperature(char *buffer, uint8_t temperature)
{
    buffer[Temperature6_3_POSITION] |= ((temperature >> 3) << Temperature6_3_OFFSET) & Temperature6_3_MASK;
    buffer[Temperature2_0_POSITION] |= (temperature << Temperature2_0_OFFSET) & Temperature2_0_MASK;
}

uint8_t getTemperature(char *buffer)
{
    uint8_t result, highByte, lowByte;

    highByte = (buffer[Temperature6_3_POSITION] & Temperature6_3_MASK) >> Temperature6_3_OFFSET;
    lowByte = (buffer[Temperature2_0_POSITION] & Temperature2_0_MASK) >> Temperature2_0_OFFSET;

    result = lowByte;
    result |= ((uint8_t)highByte << 3);

    return result;
}

void setRainfall1Hour(char *buffer, uint16_t rainfall1Hour)
{
    buffer[Rainfall1H9_8_POSITION] |= ((rainfall1Hour >> 8) << Rainfall1H9_8_OFFSET) & Rainfall1H9_8_MASK;
    buffer[Rainfall1H7_0_POSITION] |= (rainfall1Hour << Rainfall1H7_0_OFFSET) & Rainfall1H7_0_MASK;
}

uint16_t getRainfall1Hour(char *buffer)
{
    uint16_t result, highByte, lowByte;

    highByte = (buffer[Rainfall1H9_8_POSITION] & Rainfall1H9_8_MASK) >> Rainfall1H9_8_OFFSET;
    lowByte = (buffer[Rainfall1H7_0_POSITION] & Rainfall1H7_0_MASK) >> Rainfall1H7_0_OFFSET;

    result = lowByte;
    result |= ((uint16_t)highByte << 8);

    return result;
}

void setRainfall24Hour(char *buffer, uint16_t rainfall24Hour)
{
    buffer[Rainfall24H9_2_POSITION] |= ((rainfall24Hour >> 2) << Rainfall24H9_2_OFFSET) & Rainfall24H9_2_MASK;
    buffer[Rainfall24H1_0_POSITION] |= (rainfall24Hour << Rainfall24H1_0_OFFSET) & Rainfall24H1_0_MASK;
}

uint16_t getRainfall24Hour(char *buffer)
{
    uint16_t result, highByte, lowByte;

    highByte = (buffer[Rainfall24H9_2_POSITION] & Rainfall24H9_2_MASK) >> Rainfall24H9_2_OFFSET;
    lowByte = (buffer[Rainfall24H1_0_POSITION] & Rainfall24H1_0_MASK) >> Rainfall24H1_0_OFFSET;

    result = lowByte;
    result |= ((uint16_t)highByte << 2);

    return result;
}

void setNodeKey(char *buffer, uint8_t nodeKey)
{
    buffer[NodeKey6_1_POSITION] |= ((nodeKey >> 1) << NodeKey6_1_OFFSET) & NodeKey6_1_MASK;
    buffer[NodeKey0_POSITION] |= (nodeKey << NodeKey0_OFFSET) & NodeKey0_MASK;
}

uint8_t getNodeKey(char *buffer)
{
    uint8_t result, highByte, lowByte;

    highByte = (buffer[NodeKey6_1_POSITION] & NodeKey6_1_MASK) >> NodeKey6_1_OFFSET;
    lowByte = (buffer[NodeKey0_POSITION] & NodeKey0_MASK) >> NodeKey0_OFFSET;

    result = lowByte;
    result |= ((uint8_t)highByte << 1);

    return result;
}

void setHumidity(char *buffer, uint8_t humidity)
{
    buffer[Humidity_POSITION] |= (humidity << Humidity_OFFSET) & Humidity_MASK;
}

uint8_t getHumidity(char *buffer)
{
    uint8_t result = 0;
    result = (buffer[Humidity_POSITION] & Humidity_MASK) >> Humidity_OFFSET;

    return result;
}

void setAtmosphere(char *buffer, uint16_t atmosphere)
{
    buffer[Atmosphere15_8_POSITION] |= ((atmosphere >> 8) << Atmosphere15_8_OFFSET) & Atmosphere15_8_MASK;
    buffer[Atmosphere7_0_POSITION] |= (atmosphere << Atmosphere7_0_OFFSET) & Atmosphere7_0_MASK;
}

uint16_t getAtmosphere(char *buffer)
{
    uint16_t result, highByte, lowByte;

    highByte = (buffer[Atmosphere15_8_POSITION] & Atmosphere15_8_MASK) >> Atmosphere15_8_OFFSET;
    lowByte = (buffer[Atmosphere7_0_POSITION] & Atmosphere7_0_MASK) >> Atmosphere7_0_OFFSET;

    result = lowByte;
    result |= ((uint16_t)highByte << 8);

    return result;
}

void calculateCRC(char *buffer)
{
    uint8_t crc = Crc8(buffer, 14);
    buffer[CRC_POSITION] |= (crc << CRC_OFFSET) & CRC_MASK;
}

uint8_t getCRC(char *buffer)
{
    uint8_t result = 0;
    result = (buffer[CRC_POSITION] & CRC_MASK) >> CRC_OFFSET;

    return result;
}

uint8_t Crc8(const void *vptr, int len)
{
    const uint8_t *data = vptr;
    unsigned crc = 0;
    int i, j;
    for (j = len; j; j--, data++)
    {
        crc ^= (*data << 8);
        for (i = 8; i; i--)
        {
            if (crc & 0x8000)
                crc ^= (0x1070 << 3);
            crc <<= 1;
        }
    }
    return (uint8_t)(crc >> 8);
}
