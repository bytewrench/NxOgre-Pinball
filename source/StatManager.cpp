#include "stdafx.h"
#include "StatManager.h"


BallStats::BallStats(){
	pointsLit = 0;
	pointsDim = 0;
	pointsLane = 0;
	pointsThumper = 0;
	pointsKicker = 0;
	pointsTimedKicker = 0;
	pointsRail = 0;
	pointsBingoMultiplier = 0;
	pointsLetterSwitcher = 0;
	pointsDrain = 0;
	pointsDauber = 0;
	pointsTriggerLight = 0;
	pointsAllNums = 0;
	
}
BallStats::~BallStats(){
	
}

void BallStats::addToScore(int scoreType){
	switch(scoreType)
	{
	case _TRIGGERLIGHT:
		pointsTriggerLight += SCORE_TRIGGERLIGHT;
		break;
	case _ALLNUMS:
		pointsAllNums += SCORE_ALLNUMS;
		break;
	case _HOLD:
		pointsTimedKicker+= SCORE_HOLD;
		break;
	case _KICKER:
		pointsKicker += SCORE_KICKER;
		break;
	case _DRAIN:
		pointsDrain += SCORE_DRAIN;
		break;
	case _RAIL:
		pointsRail += SCORE_RAIL;
		break;
	case _TARGET_LIT:
		pointsLit += SCORE_TARGET_LIT;
		break;
	case _TARGET_DIM:
		pointsDim += SCORE_TARGET_DIM;
		break;
	case _BINGO_MULTIPLIER:
		pointsBingoMultiplier += SCORE_BINGO_MULTIPLIER;
		break;
	case _LETTER_SWITCHER:
		pointsLetterSwitcher += SCORE_LETTER_SWITCHER;
		break;
	case _THUMPER_BUMPER:
		pointsThumper += SCORE_THUMPER_BUMPER;
		break;
	case _DAUBER:
		pointsDauber += SCORE_DAUBER;
		break;
	}
}
int BallStats::getTotal(){
	int _pointsForBall = 0;
	_pointsForBall += pointsTimedKicker;
	_pointsForBall += pointsKicker;
	_pointsForBall += pointsDrain;
	_pointsForBall += pointsRail;
	_pointsForBall += pointsLit;
	_pointsForBall += pointsDim;
	_pointsForBall += pointsBingoMultiplier;
	_pointsForBall += pointsLetterSwitcher;
	_pointsForBall += pointsThumper;
	_pointsForBall += pointsDauber;
	_pointsForBall += pointsTriggerLight;
	_pointsForBall += pointsAllNums;
	return _pointsForBall;
}

StatManager::StatManager(){
	_currentBall=-1;
}
StatManager::~StatManager(){
	try
	{
		ballContainer.clear();
	}
	catch (int  e)
	{
		
	}
}

StatManager * StatManager::getSingleton(){
	static StatManager * _statman;
	if (_statman==0)
	{
		_statman = new StatManager();
	}
	return _statman;
}
void StatManager::addBall(){
	
	BallStats * b = new BallStats();
	ballContainer.push_back((BallStats)* b);
	_currentBall ++;
	
}
void StatManager::upDate(int scoreType){

	ballContainer[_currentBall].addToScore(scoreType);
}