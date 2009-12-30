// GameMetrics.h: interface for the GameMetrics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEMETRICS_H__D05A97D3_937C_4CA7_BCD5_6CBA04355D5E__INCLUDED_)
#define AFX_GAMEMETRICS_H__D05A97D3_937C_4CA7_BCD5_6CBA04355D5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"


#define ARRAY_SIZEOF(_x) (sizeof(_x)/sizeof((_x)[0]))

class GameMetrics  
{
public:

	GameMetrics();
	virtual ~GameMetrics();
	void EndGame();
	void NewGame();
	void AddToScore( DWORD amount );
	void NewLevel();

	DWORD GetScore(){return score;}
protected:
	DWORD score;
};

#endif 
