#ifndef _HITNUMBERMANAGER
#define _HITNUMBERMANAGER


#include "stdafx.h"

class hitNumberManager
{
public:

	void recordHit(int ID);
	bool isHit(int ID);
	void resetHitList();
	bool allNumbers[6];
	hitNumberManager();
	static hitNumberManager * getHitNumMan ();

protected:
private:
private:

	struct HIT_STRUCT
	{
		int Value;
		bool IsHit;
	};

	HIT_STRUCT g_HitNumbers[76];
};
#endif