#include "stdafx.h"


void hitNumberManager::recordHit ( int ID )
{
	g_HitNumbers[ID].IsHit = true;

	if ( (ID < 16) && (! allNumbers[1]) )
	{
		for ( int i = 1; i < 16; i ++ )
		{
			if ( ! g_HitNumbers[i].IsHit )
			{
				allNumbers[1] = false;
				return;
			}
		}

		allNumbers[1] = true;
		ScoreManager::getScoreManager()->addToScore(SCORE_ALLNUMS,ScoreType::_ALLNUMS);
		SoundManager::getSoundManager()->Play_Sound(SoundManager::soundTypes::SOUND_ALLNUMBERS);
		BubbleNotify::getSingleton()->setMessage("20,000",30,10);
	}

	else if ( (ID > 15) && (ID < 31) && (! allNumbers[2]) )
	{
		for ( int i = 16; i < 31; i ++ )
		{
			if ( ! g_HitNumbers[i].IsHit )
			{
				allNumbers[2] = false;
				return;
			}
		}

		allNumbers[2] = true;
		ScoreManager::getScoreManager()->addToScore(SCORE_ALLNUMS,ScoreType::_ALLNUMS);
		SoundManager::getSoundManager()->Play_Sound(SoundManager::soundTypes::SOUND_ALLNUMBERS);
		BubbleNotify::getSingleton()->setMessage("20,000",30,10);
	}

	else if ( (ID > 30) && (ID < 46) && (! allNumbers[3]) )
	{
		for ( int i = 31; i < 46; i ++ )
		{
			if ( ! g_HitNumbers[i].IsHit )
			{
				allNumbers[3] = false;
				return;
			}
		}

		allNumbers[3] = true;
		ScoreManager::getScoreManager()->addToScore(SCORE_ALLNUMS,ScoreType::_ALLNUMS);
		SoundManager::getSoundManager()->Play_Sound(SoundManager::soundTypes::SOUND_ALLNUMBERS);
		BubbleNotify::getSingleton()->setMessage("20,000",30,10);
	}

	else if ( (ID > 45) && (ID < 61) && (! allNumbers[4]) )
	{
		for ( int i = 46; i < 61; i ++ )
		{
			if ( ! g_HitNumbers[i].IsHit )
			{
				allNumbers[4] = false;
				return;
			}
		}

		allNumbers[4] = true;
		ScoreManager::getScoreManager()->addToScore(SCORE_ALLNUMS,ScoreType::_ALLNUMS);
		SoundManager::getSoundManager()->Play_Sound(SoundManager::soundTypes::SOUND_ALLNUMBERS);
		BubbleNotify::getSingleton()->setMessage("20,000",30,10);
	}

	else if ( (ID > 60) && (! allNumbers[5]) )
	{
		for ( int i = 61; i < 76; i ++ )
		{
			if ( ! g_HitNumbers[i].IsHit )
			{
				allNumbers[5] = false;
				return;
			}
		}

		allNumbers[5] = true;
		ScoreManager::getScoreManager()->addToScore(SCORE_ALLNUMS,ScoreType::_ALLNUMS);
		SoundManager::getSoundManager()->Play_Sound(SoundManager::soundTypes::SOUND_ALLNUMBERS);
		BubbleNotify::getSingleton()->setMessage("20,000",30,10);
	}
}

//==-----------------------------------------------------------------------------------
bool hitNumberManager::isHit ( int ID )
{
	return g_HitNumbers[ID].IsHit;
}

//==-----------------------------------------------------------------------------------
void hitNumberManager::resetHitList ()
{
	for ( int i = 0; i < 76; ++ i )
	{
		g_HitNumbers[i].IsHit = false;
		g_HitNumbers[i].Value = i;
	}

	for ( int i = 1; i < 6; i ++ )
	{
		allNumbers[i] = false;
	}
}

//==-----------------------------------------------------------------------------------
hitNumberManager * hitNumberManager::getHitNumMan ()
{
	static hitNumberManager * hitMan;

	if ( hitMan == 0 )
	{
		hitMan = new hitNumberManager();
	}

	return hitMan;
}

//==-----------------------------------------------------------------------------------
hitNumberManager::hitNumberManager () { }

