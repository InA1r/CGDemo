#pragma once

#ifndef _WINDEF_
typedef struct _POINT
{
	long x;
	long y;
} POINT;
#endif // !_WINDEF_

typedef struct _EDGE
{
	float xOfMinY;
	float deltaX;
	int yMax;
	int yMin;
}EDGE;

#ifdef _DEBUG
	#define DBG_PRNT(fmt, ...) \
	printf(fmt, __VA_ARGS__)
#else
	#define DBG_PRNT(fmt, ...)  
#endif