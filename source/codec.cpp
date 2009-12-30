// Codec.cpp: implementation of the CCodec class.
//
//////////////////////////////////////////////////////////////////////
//#include "Codec.h"
#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCodec::CCodec()
{

}

CCodec::~CCodec()
{

}

void CCodec::InitCodeBuffer()
{

	srand(time(NULL));
	
	BOOL Used[32];
	int shifttemp[32];

	int x;
	for(x=0; x < 32; x++)
		Used[x] = false;

	int		shiftindex = 0;
	int		shifts = 0;
	int	nextshift = 0;

	while(shifts < 32)
	{
		nextshift = rand() % 32;
		if(!Used[nextshift])
		{
			Used[nextshift] = true;
			shifttemp[shifts] = nextshift;
			shifts++;
		}
	}

	for(x=0; x<32; x++)
	{
		ShiftMapEnc[x] = shifttemp[x]-x;
		ShiftMapDec[shifttemp[x]]  = ShiftMapEnc[x] * -1;
	}

}

DWORD CCodec::Encode(DWORD val)
{
	DWORD bitmask =		0x80000000;
	DWORD thisbit =		0x00000000;
	DWORD retval =		0x00000000;
	for(int x=0; x < 32; x++)
	{
		thisbit = val & bitmask;
		if( thisbit)
		{
			if( ShiftMapEnc[x] < 0)
				thisbit <<= abs(ShiftMapEnc[x]);
			else
				thisbit >>= ShiftMapEnc[x];

			retval |= thisbit;
		}
		bitmask >>= 1;
	}
	return retval;

}

DWORD CCodec::Decode(DWORD val)
{
	DWORD bitmask =		0x80000000;
	DWORD thisbit =		0x00000000;
	DWORD retval =		0x00000000;
	for(int x=0; x < 32; x++)
	{
		thisbit = val & bitmask;
		if( thisbit )
		{
			if( ShiftMapDec[x] < 0)
				thisbit <<= abs(ShiftMapDec[x]);
			else
				thisbit >>= ShiftMapDec[x];

			retval |= thisbit;			
		}


		bitmask >>= 1;
	}
	return retval;

}
