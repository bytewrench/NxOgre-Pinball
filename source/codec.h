// Codec.h: interface for the CCodec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODEC_H__1AF9154E_EA2B_448B_840E_6AA71471A574__INCLUDED_)
#define AFX_CODEC_H__1AF9154E_EA2B_448B_840E_6AA71471A574__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

class CCodec  
{
public:
	DWORD Decode(DWORD val);
	DWORD Encode(DWORD val);
	void InitCodeBuffer();
	CCodec();
	virtual ~CCodec();

protected:
	SHORT ShiftMapEnc[32];
	SHORT ShiftMapDec[32];
};

#endif // !defined(AFX_CODEC_H__1AF9154E_EA2B_448B_840E_6AA71471A574__INCLUDED_)
