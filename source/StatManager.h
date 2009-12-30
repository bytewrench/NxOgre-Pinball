#ifndef _STATMAN
#define _STATMAN

#include "stdafx.h"

class BallStats
{
public:
	BallStats();
	~BallStats();
	int pointsLit;
	int pointsDim;
	int pointsLane;
	int pointsThumper;
	int pointsKicker;
	int pointsTimedKicker;
	int pointsRail;
	int pointsBingoMultiplier;
	int pointsLetterSwitcher;
	int pointsDrain;
	int pointsDauber;
	int pointsAllNums;
	int pointsTriggerLight;
	int getTotal();
	void addToScore(int scoreType);
protected:
private:
};

class StatManager
{
public:
	StatManager();
	~StatManager();
	static StatManager * getSingleton ();
	vector <BallStats> ballContainer;
	void addBall();
	int _currentBall;
	void upDate(int scoreType);

protected:
private:
};

#endif