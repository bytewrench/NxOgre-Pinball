#ifndef _SCOREMANAGER
#define _SCOREMANAGER

#include "stdafx.h"

enum	ScoreType{
	_HOLD,
	_KICKER,
	_DRAIN,
	_RAIL,
	_TARGET_LIT,
	_TARGET_DIM,
	_BINGO_MULTIPLIER,
	_SPINNER,
	_LETTER_SWITCHER,
	_THUMPER_BUMPER,
	_DAUBER,
	_TRIGGERLIGHT,
	_ALLNUMS
};

class ScoreManager
{
public:

	static void setScore(int score);
	static int getScore();
	static void addToScore(int addamount, ScoreType type);
	static void subFromScore(int subAmount);
	static ScoreManager* getScoreManager();
	static void incLevel();
	ScoreManager();
	static GameMetrics gm;

protected:
	
private:
	
	
};


#endif