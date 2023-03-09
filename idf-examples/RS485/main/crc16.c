#include "crc16.h"
unsigned short do_crc(unsigned char *ptr, int len)
{
    unsigned int i;
    unsigned short crc = 0xFFFF;
    while(len--)
    {
        crc ^= *ptr++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}