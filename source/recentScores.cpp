#include "stdafx.h"
//==-----------------------------------------------------------------------------------
recentScoreManager * recentScoreManager::getRecentScoreManager ()
    {
        if ( recentSM == 0 )
        {
            recentSM = new recentScoreManager();
        }

        return recentSM;
    }
//==-----------------------------------------------------------------------------------

recentScoreManager::recentScoreManager ()
    {
        //SCORECARD_X = 45;
		SCORECARD_X = .0439;
        //SCORECARD_Y = 120;
		SCORECARD_Y = .1562;
        MAX_SCORECARD_NUMBER = 9;

        for ( int i = 0; i < 10; ++ i )
            {
                nmItem[i] = new numberMachine();
                nmItem[i]->create(Vector2(0, 0), TextAreaOverlayElement::Alignment::Center, 20, 20, 0,
                                  "recentscores");
                nmItem[i]->setCol(1, 1, 1, 0);
                g_RecentScores[i].score = 0;
                g_RecentScores[i].alpha = 0;
                g_RecentScores[i].pos.x = SCORECARD_X;
                g_RecentScores[i].pos.y = SCORECARD_Y;
            }

        nmLastScore = new numberMachine();
        nmLastScore->create(Vector2(SCORECARD_X, SCORECARD_Y), TextAreaOverlayElement::Alignment::Center,
                            35,                                40,
                            50.0f,                             "lastscore");
        nmLastScore->UpdateValue(0);
    }

//==-----------------------------------------------------------------------------------
recentScoreManager::~recentScoreManager(){
	for ( int i = 0; i < 10; ++ i )
	{
		delete nmItem[i];
	}
	delete nmLastScore;
}
//==-----------------------------------------------------------------------------------
void recentScoreManager::display ()
    {
        float r, g, b, a;

        if ( nmLastScore->val > 0 )
            {
                r = g_RecentScores[MAX_SCORECARD_NUMBER].color.x;
                g = g_RecentScores[MAX_SCORECARD_NUMBER].color.y;
                b = g_RecentScores[MAX_SCORECARD_NUMBER].color.z;
                a = 1;

                nmLastScore->setCol(r, g, b, a);
                nmLastScore->render(0);
            }

        else
        {
            return;
        }

        for ( int i = 0; i < MAX_SCORECARD_NUMBER; i ++ ) // recent scores
            {
                if ( g_RecentScores[i].score > 0 )
                    {
                        r = g_RecentScores[i].color.x;
                        g = g_RecentScores[i].color.y;
                        b = g_RecentScores[i].color.z;
                        a = g_RecentScores[i].alpha;

                        nmItem[i]->val = g_RecentScores[i].score;

                        if ( nmItem[i]->val > 0 )
                            {
                                nmItem[i]->setColor(Vector4(r, g, b, a));
                                nmItem[i]->setPosition(g_RecentScores[i].pos);
                                nmItem[i]->render(0);
                            }
                    }
            }
    }

//==-----------------------------------------------------------------------------------
void recentScoreManager::update ( int addValue, Ogre::Vector3 col )
    {
        nmLastScore->UpdateValue(addValue);
        float alpha = .5f;

        for ( int i = 0; i < MAX_SCORECARD_NUMBER; i ++ )
            {
                g_RecentScores[i].score = g_RecentScores[i + 1].score;
                g_RecentScores[i].pos.y = g_RecentScores[i + 1].pos.y + .0195;
                g_RecentScores[i].pos.x = g_RecentScores[i + 1].pos.x;
                g_RecentScores[i].color = g_RecentScores[i + 1].color;
                g_RecentScores[i].alpha = alpha;
                alpha += .05f;
            }

        g_RecentScores[MAX_SCORECARD_NUMBER].score = addValue;
        g_RecentScores[MAX_SCORECARD_NUMBER].pos.x = SCORECARD_X;
        g_RecentScores[MAX_SCORECARD_NUMBER].pos.y = SCORECARD_Y + .0234;
        g_RecentScores[MAX_SCORECARD_NUMBER].alpha = 1;
        g_RecentScores[MAX_SCORECARD_NUMBER].color = col;
    }

//==-----------------------------------------------------------------------------------
void recentScoreManager::reset ()
    {
        for ( int i = 0; i < MAX_SCORECARD_NUMBER; i ++ )
        {
            //g_RecentScores[i].score = 0;
            nmItem[i]->reset();
        }

        nmLastScore->reset();
    }

//==-----------------------------------------------------------------------------------
recentScoreManager * recentScoreManager::recentSM;
