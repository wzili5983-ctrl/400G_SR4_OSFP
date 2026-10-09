/***************************************************************************//**
 *   @file   basedef.h
 *   @brief  Base defination of variable & status.
 *   @author Oliver Wang (zili.wang@terabitcom.com)
********************************************************************************
 * Copyright 2024~2026(c) Terabitcom, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Terabitcom, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Terabitcom, INC. “AS IS” AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL Terabitcom, INC. BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/
#ifndef BASEDEF_H
#define BASEDEF_H

#include <stdint.h>
#include <stdbool.h>
#define STATIC static
#define CONST const

typedef unsigned char            BYTE;
typedef unsigned short           WORD;
typedef unsigned int            DWORD;
typedef unsigned long long     DDWORD;

typedef signed char*             PBYTE;
typedef signed short*            PWORD;
typedef signed int*             PDWORD;
typedef signed long long*      pDDWORD;

typedef signed int                  INT;
typedef float                     FLOAT;
typedef bool                       BOOL;
typedef void                       VOID;

#ifndef NULL
#define NULL  0
#endif

#define OK   0
#define ERR  1

#define FALSE   0
#define TRUE    1

#define IO_LOW   0
#define IO_HIGH  1

#define MAX_UIT16_VALUE 0XFFFF

#define SETBIT(x,i)  ((x) |= 1 << (i))
#define CLRBIT(x,i)  ((x) &= ~(1 << (i)))
#define GETBIT(x,i)  (((x)>> (i)) & 0x01)
#define SWAP16(x)   ((x) >> 8 | (x) << 8)


#define MAKEWORD(l, h)   (UIT16)(((UIT16)(h)<<8) | (l))
#define GETWORDB(p,i)  MAKEWORD(p[i+1], p[i])

#define WORD2BIG(p, x)   *p = (x) >> 8; \
					  *(p+1) = (x)&0xff;

#define GETDWORDB(p, i) 	(UIT32)((p[i +3]) | (p[i+2] << 8) | (p[i+1] << 16) | (p[i] << 24))

#define DWORD2BIG(p, x)   *p = (x) >> 24; \
					  *(p+1) = (x) >> 16; \
		              *(p+2) = (x) >> 8; \
				      *(p+3) = (x)&0xff; \

#define FLOAT2INT(x)   (INT)(((x) > 0.0f) ? ((x) + 0.5f ) :((x) - 0.5f ))

#define DWORD2FLOAT(x)   (*(FLOAT*)(&(x)))

typedef enum {
	BIT0=0,
	BIT1,
	BIT2,
	BIT3,
	BIT4,
	BIT5,
	BIT6,
	BIT7,
	BIT8,
	BIT9,
	BIT10,
	BIT11,
	BIT12,
	BIT13,
	BIT14,
	BIT15
} Bit_t;

#endif