#include "stdafx.h"
#include "StatManager.h"

ScoreManager::ScoreManager ()
{
    gm.NewGame();
}

void ScoreManager::addToScore ( int addamount, ScoreType type )
    {
        gm.AddToScore(addamount);
        Vector3 col;

		StatManager::getSingleton()->upDate(type);
        switch ( type )
            {
                case _KICKER:
					gamedata::getSingleton()->logMessage("KCK");
                    col = COLOR_KICKER;
                    break;

                case _SPINNER:
                    col = COLOR_SPINNER;
                    break;

				case _TRIGGERLIGHT:
					col = COLOR_SPINNER;
					gamedata::getSingleton()->logMessage("LANE");
					break;
                case _TARGET_LIT:
                    col = COLOR_TARGET_LIT;
					gamedata::getSingleton()->logMessage("T_LIT");
                    break;

				case _TARGET_DIM:
					col = COLOR_TARGET_DIM;
					gamedata::getSingleton()->logMessage("T_DIM");
					break;

                case _RAIL:
                    col = COLOR_RAIL;
					gamedata::getSingleton()->logMessage("R");
                    break;

                case _THUMPER_BUMPER:
                    col = COLOR_THUMPER_BUMPER;
					gamedata::getSingleton()->logMessage("THMP");
                    break;

                case _HOLD:
                    col = COLOR_HOLD;
					gamedata::getSingleton()->logMessage("H");
                    break;

                case _DAUBER:
                    col = COLOR_DAUBER;
					//gamedata::getSingleton()->logMessage("DAUBER");
                    break;

                case _DRAIN:
                    col = COLOR_DRAIN;
					gamedata::getSingleton()->logMessage("D");
                    break;

                case _LETTER_SWITCHER:
                    col = COLOR_LETTER_SWITCHER;
					gamedata::getSingleton()->logMessage("LS");
                    break;

                case _BINGO_MULTIPLIER:
					gamedata::getSingleton()->logMessage("BINGO");
                    col = COLOR_BINGO_MULTIPLIER;
                    break;
            }

        recentScoreManager::getRecentScoreManager()->update(addamount, col);
    }

//==-----------------------------------------------------------------------------------
void ScoreManager::subFromScore ( int subAmount )
{
    gm.AddToScore(- subAmount);
}

//==-----------------------------------------------------------------------------------
void ScoreManager::setScore ( int score )
    {
        gm.AddToScore(- gm.GetScore());
        gm.AddToScore(score);
    }

//==-----------------------------------------------------------------------------------
int ScoreManager::getScore ()
{
    return gm.GetScore();
}

//==-----------------------------------------------------------------------------------
void ScoreManager::incLevel ()
{
    gm.NewLevel();
}

//==-----------------------------------------------------------------------------------
ScoreManager * ScoreManager::getScoreManager ()
    {
        static ScoreManager * scoreMan;

        if ( scoreMan == 0 )
        {
            scoreMan = new ScoreManager();
        }

        return scoreMan;
    }

//==-----------------------------------------------------------------------------------
GameMetrics ScoreManager::gm;
