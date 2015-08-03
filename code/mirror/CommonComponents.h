#ifndef _COMMONCOMPONENTS_H
#define _COMMONCOMPONENTS_H

#include <stdint.h>

inline void IntToFourChar(uint32_t nSrc, uint8_t &n1, uint8_t &n2, uint8_t &n3, uint8_t &n4)
{
	n1 = nSrc >> 24;
	n2 = (nSrc >> 16) & 0xFF;
	n3 = (nSrc >> 8) & 0xFF;
	n4 = nSrc & 0xFF;
}

inline uint32_t FourCharToInt(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4)
{
	return  (c1 << 24 | c2 << 16 | c3 << 8 | c4);
}


#endif	//#ifndef _COMMONCOMPONENTS_H