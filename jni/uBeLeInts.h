#ifndef __uBeLeInts_h__
#define __uBeLeInts_h__

/**
** uBeLeInts (Union of Big Endian Little Endian Ints)
**
** Union to facilitate translating sensor data from Big Endian
** (Motorola) format into Little Endian (Intel) format.
*/
// not UNICODE
//字节顺序，大端到小端转换（Motorola->Intel）
typedef union uBeLeInts
{
	unsigned short int mInt;
	struct
	{
		unsigned char leLo;
		unsigned char leHi;
	}  le;
	struct
	{
		unsigned char beHi;
		unsigned char beLo;
	}  be;
} uBeLeInts;



/**
**
*/

/**
** End of uBeLeInts.h
*/

#endif   /* if !defined ( __uBeLeInts_h__ ) */


