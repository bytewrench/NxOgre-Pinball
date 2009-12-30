// GameMetrics.cpp: implementation of the GameMetrics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GameMetrics::GameMetrics()
{
	score = 0;

}

GameMetrics::~GameMetrics()
{

}

void GameMetrics::NewLevel()
{
	DWORD temp;
	score = 0;

}


void GameMetrics::AddToScore(DWORD amount)
{
	score += amount;

}






void GameMetrics::NewGame()
{
	
	score = 0;

}


void GameMetrics::EndGame()
{

}



