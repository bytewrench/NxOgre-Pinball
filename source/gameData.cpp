#include "stdafx.h"
#include "StatManager.h"

gamedata::gamedata ()
    {
        setToDefault();
        _datapacket = "";
    }

gamedata * gamedata::getSingleton ()
    {
        static gamedata * gamedat;

        if ( gamedat == NULL )
        {
            gamedat = new gamedata();
        }

        return gamedat;
    }
//------------------------------------------------------------

//------------------------------------------------------------

void gamedata::endGame ( float _fps )
    {

		LogManager::getSingletonPtr()->logMessage("Ending App");

        endTime = time(NULL);
        gameTime = (endTime - startTime);
   
    }

void gamedata::beginGame ()
    {
        _datapacket += "INIT:" + getCurrentTime() + ",\n";
        LoadGameDetailsFromServer();
        LogManager::getSingletonPtr()->logMessage("Loaded Details");
        startTime = time(NULL);
    }

void gamedata::logMessage ( String message )
{
    _datapacket += message + ",";
}

String gamedata::getLog ()
    {
        String retVal = _datapacket;
        return retVal;
    }

void gamedata::sendToServer ( String PlayerInitials )
    {

    }

String gamedata::getCurrentTime ()
    {
        String _szTime;
        char _Time[9];
        _strtime(_Time);
        _szTime = _Time;
        String ret = _szTime;
        return ret;
    }

void gamedata::LoadGameDetailsFromServer ()
    {
        ScoreManager::getScoreManager()->gm.NewGame();
        LoadStaticSettings();
    }

void gamedata::LoadStaticSettings ()
    {
        ScoreManager::getScoreManager()->gm.NewGame();
        NUMBALLS = 5;
        MAXBUMPS = 5;
        NUMBUMPS = MAXBUMPS;
        CURRENTBALL = 0;
        
    }


void gamedata::setToDefault ()
    {
        // set this to false for the release version
        BETAMODE = true;
        fileSent = false;
        NUMBALLS = 4;
        CURRENTBALL = 0;
        ENDSCORE = - 1;
        ENDLEVEL = - 1;
        HIGHSCORE = 0;
        NUMBUMPS = 5;
        MAXBUMPS = 5;
        CURRENTBUMP = 0;

        ALLNUMCOUNT = 0;
        LOWSCORE = 0;
        LASTSCORE = 0;
        PlayerInitials = "";
        sendLogToServer = true;
        szPlayerInitials = "";
    }
