#include "stdafx.h"

#include "callback.h"

#include "StatManager.h"

PinballGame::PinballGame () { }

PinballGame::~PinballGame () { }


/*
//////////////////////////////////////////////////////////////////////////
OUTERMOST
//////////////////////////////////////////////////////////////////////////
*/

void PinballGame::RunGame ()
{
    //const NxReal simulationTimeStep = (float)1.0 / 50.0f; 
    const NxReal simulationTimeStep = (float)1.0 / 60.0f;
    NxReal timeToSimulate = 0.0f;
    NxReal deltaTime;
    
    Vector3 lastPosition=_pinBall[0]->myBody->getGlobalPosition();
    Vector3 interpolated;

    Ogre::Timer* s = new Timer();
    slowFrameWarning = new Ogre::Timer();

    while ( !mShutdownNow ) 
    {
        deltaTime = s->getMicroseconds()*1.0e-6;
        s->reset();
        timeToSimulate += deltaTime;

        _pinBall[0]->myBody->setGlobalPosition(lastPosition);
        interpolated = _pinBall[0]->myBody->getGlobalPosition();
        
        while (timeToSimulate>=simulationTimeStep)
        {
            mWorld->getPhysXDriver()->simulate(simulationTimeStep);
            updateFlippers();
            timeToSimulate -= simulationTimeStep;
        }
        pinballFrame(deltaTime);

        lastPosition = _pinBall[0]->myBody->getGlobalPosition();
        interpolated.x=(interpolated.x+lastPosition.x)*.5f;
        interpolated.z=(interpolated.z+lastPosition.z)*.5f;
        _pinBall[0]->myBody->setGlobalPosition(interpolated);
        
        mWorld->getPhysXDriver()->render(deltaTime);
        mRenderer->getRoot()->renderOneFrame();
    }
}

bool PinballGame::frameStarted ( const Ogre::FrameEvent & evt )
    {
        InputHandler::getInputHandler()->onFrame(evt.timeSinceLastFrame);

        if ( isKeyDown(QUIT) )
            {
				mShutdownNow=true;
                return false;
            }

        onFramePre(evt.timeSinceLastFrame);
        if(showFps)
            otFPS->setText("fps: " + Ogre::StringConverter::toString(mRenderer->getWindow()->getAverageFPS()));
        else
            otFPS->setText("");
        return true;
    }

bool PinballGame::frameEnded ( const Ogre::FrameEvent & evt )
    {
        if ( mShutdownNow )
            {
                mRenderer->stop();
                return false;
            }

        mRenderer->frameEnded(evt);
        return true;
    }

void PinballGame::stop ()
    {
        gamedata::getSingleton()->endGame(mRenderer->getWindow()->getAverageFPS());

        delete _pinBall[0];
        delete _flipper[0];
        delete _flipper[1];
        delete _thumper[0];
        delete _thumper[1];
        delete _thumper[2];
        delete _thumper[3];
        delete _rearKickers[0];
        delete _rearKickers[1];
        delete _midKickers[0];
        delete _midKickers[1];
        delete _timed_kicker;

        delete _mainTable;

        delete nmBingos;
        delete nmBalls;
        delete nmBumps;
        delete nmScore;
        delete nmPower;

        for ( int i = 0; i < 3; ++ i )
        {
            delete _pin[i];
        }

        for ( int i = 0; i < 5; i ++ )
        {
            delete otAllNumbers[i];
        }

        if ( otHighScore != NULL )
        {
            delete otHighScore;
        }

        if ( otPlayerInitials != NULL )
        {
            delete otPlayerInitials;
        }

        if ( otIntermissionHeader != NULL )
        {
            delete otIntermissionHeader;
        }

        if ( otSoftwareVersion != NULL )
        {
            delete otSoftwareVersion;
        }

        for ( int i = 0; i < 15; i ++ )
            {
                delete txtUsernames[i];
                delete txtScores[i];
            }

        for ( int i = 0; i < 24; i ++ )
        {
            delete trigger[i];
        }

        delete BingoManager::getSingleton();

        delete otIntermissionSubText;
        delete _intermission_screen;
        delete otFPS;
        delete otUserMessage;
        delete SoundManager::getSoundManager();
        delete webManager::getWebmanager();
        delete ScoreManager::getScoreManager();
        delete loadinganimation::getSingleton();
        delete gamedata::getSingleton();
        delete bingoballmarker::getSingleton();
        delete BubbleNotify::getSingleton();
        delete physicsVars::getSingleton();
        delete recentScoreManager::getRecentScoreManager();
        delete triggerLightManager::getSingleton();
        delete hitNumberManager::getHitNumMan();
        delete stopper::getSingleton();
        delete StatManager::getSingleton();
        delete lm;
        delete quadManager;
		
        delete InputHandler::getInputHandler();
		LogManager::getSingletonPtr()->logMessage("Almost Out");
        delete mViewer;
        delete mWorld;
    }

void PinballGame::prestop () { }

/*
//////////////////////////////////////////////////////////////////////////
INIT STUFF
//////////////////////////////////////////////////////////////////////////
*/

void PinballGame::initPinball ()
    {
        InetOK = true;
        mShutdownNow = false;
		physicsVars::getSingleton();

        gamedata::getSingleton()->setToDefault();
        LogManager::getSingletonPtr()->logMessage(LML_NORMAL, "Start Pinball");
        InputHandler::getInputHandler()->mKeyboard->setEventCallback(this);
        InputHandler::getInputHandler()->mKeyboard->setBuffered(true);


        otHighScore = new OgreText();
        otIntermissionHeader = new OgreText();
        otIntermissionSubText = new OgreText();
        otSoftwareVersion = new OgreText();

        otFPS = new OgreText();
        otFPS->setHeight(15);
        //otFPS->setPos(890, 758);
        otFPS->setPos(.869, .9869);
        otFPS->setCol(0, 1, 1, 1);

        otUserMessage = new OgreText();
        otUserMessage->setText("");
        otUserMessage->setHeight(15);
        //otUserMessage->setPos(2, 758);
        otUserMessage->setPos(.01, .9869);

		otSoftwareVersion->setText(StringConverter::toString(Real(GAME_VER)));
        otSoftwareVersion->setHeight(15);
        //otSoftwareVersion->setPos(1000, 758);
        otSoftwareVersion->setPos(.976, .9869);

        _plungerState = 0;
        _plungerStrength = 0;
        _canBump = false;

        FMOD_VECTOR mUp, mVel, mForward;
        FMOD_VECTOR pos =
            {
            mCamera->getPosition().x, mCamera->getPosition().y, mCamera->getPosition().z
            };

        mUp.x = 0;
        mUp.y = 0;
        mUp.z = 1;

        mVel.x = 0;
        mVel.y = 0;
        mVel.z = 0;

        mForward.x = 0;
        mForward.y = - 1;
        mForward.z = 0;


        //.51
        loadinganimation::getSingleton()->increment();
        ActorGroup * mBallGroup = mScene->createActorGroup("BallGroup");

			
        _pinBall[0] = new PinBall(
			NxVec3(8.7, .5, 12.7),
			.51f, 
			physicsVars::getSingleton()->BallMass,
			1,
			false);
		
		_pinBall[0]->myBody->putToSleep();
		LogManager::getSingletonPtr()->logMessage("sleep ball");

        SoundManager::getSoundManager()->ballBody = _pinBall[0]->myBody;
        loadinganimation::getSingleton()->increment();

        SoundManager::getSoundManager()->_system->set3DListenerAttributes(0, & pos, & mVel, & mForward, & mUp);
        loadinganimation::getSingleton()->increment();

        BingoManager::getSingleton()->initBoard();
		LogManager::getSingletonPtr()->logMessage("quad manager");
        quadManager = new QuadManager(mSceneMgr);
		LogManager::getSingletonPtr()->logMessage("getball");
        quadManager->setBallEnt(_pinBall[0]->myBody->getEntity());
		LogManager::getSingletonPtr()->logMessage("preload text");
        preloadText();
		LogManager::getSingletonPtr()->logMessage("light manager");
        if ( lm == NULL )
        {
            lm = new lightManager(mSceneMgr);
        }

        loadinganimation::getSingleton()->increment("Please Wait");
		
		LogManager::getSingletonPtr()->logMessage("Build Table");
        buildTable();
		LogManager::getSingletonPtr()->logMessage("Table Built");
        addOverlays();
        tableSlope = physicsVars::getSingleton()->TableSlope;

		
        gamedata::getSingleton()->beginGame();
	
        _intermission_screen = new intermission();
        _intermission_screen->init();
        _intermission_screen->show();

        Entity * entShock = mSceneMgr->createEntity("Shockwave", "light.mesh");
        entShock->setMaterialName("shockwave");

        nodeShock = mSceneMgr->getRootSceneNode()->createChildSceneNode("shockNode");
        nodeShock->setPosition(- .725, .5, 12.842);
        nodeShock->attachObject(entShock);
        nodeShock->setScale(2, 1, 2);
        nodeShock->setVisible(false);

        mRenderer->getRoot()->addFrameListener(this);
        createInputSys(mRenderer->getWindow());
        mOverlay->hide();

        _pinBall[0]->myBody->addForce(Vector3(.5, 0, 0));
        _maxBumps = gamedata::getSingleton()->NUMBUMPS;
        SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_NEWBALL);
		LogManager::getSingletonPtr()->logMessage("Initialize Game");
		_pinBall[0]->myBody->wakeUp();
    //loadinganimation::getSingleton()->hide();
	    }

void PinballGame::buildTable ()
    {
        sphereCount = 1;

        ShapeGroup * mGroupFlippers = mScene->createShapeGroup("FlipperGroup");
        ShapeGroup * mGroupTable = mScene->createShapeGroup("TableGroup");

        ActorGroup * mThumperGroup = mScene->createActorGroup("ThumperGroup");
        ActorGroup * mKickerGroup = mScene->createActorGroup("KickerGroup");
        ActorGroup * mTriggerGroup = mScene->createActorGroup("TriggerGroup");

        _flipper[0] = new flipper(physicsVars::getSingleton()->FlipperPos[0],
                                  0,
                                  mSceneMgr,
                                  physicsVars::getSingleton()->FlipperPower,
                                  physicsVars::getSingleton()->FlipperSpeed);

        _flipper[1] = new flipper(physicsVars::getSingleton()->FlipperPos[1],
                                  1,
                                  mSceneMgr,
                                  physicsVars::getSingleton()->FlipperPower,
                                  physicsVars::getSingleton()->FlipperSpeed);


        //--------------------------------------------------------THUMPERS
        _thumper[0] = new thumper(physicsVars::getSingleton()->ThumperPos[0], mSceneMgr, _pinBall[0]->myBody,
                                  physicsVars::getSingleton()->BumperStrength[0]);
        _thumper[1] = new thumper(physicsVars::getSingleton()->ThumperPos[1], mSceneMgr, _pinBall[0]->myBody,
                                  physicsVars::getSingleton()->BumperStrength[1]);
        _thumper[2] = new thumper(physicsVars::getSingleton()->ThumperPos[2], mSceneMgr, _pinBall[0]->myBody,
                                  physicsVars::getSingleton()->BumperStrength[2]);
        _thumper[3] = new thumper(physicsVars::getSingleton()->ThumperPos[3], mSceneMgr, _pinBall[0]->myBody,
                                  physicsVars::getSingleton()->BumperStrength[3]);


        //--------------------------------------------------------KICKERS
        _rearKickers[0] = new kicker("kickers_rear_left.mesh", mSceneMgr, _pinBall[0]->myBody,
                                     physicsVars::getSingleton()->KickerRearStrength);

        _rearKickers[1] = new kicker("kickers_rear_right.mesh", mSceneMgr, _pinBall[0]->myBody,
                                     physicsVars::getSingleton()->KickerRearStrength);

        _midKickers[0] = new kicker("kicker_mid_left.mesh", mSceneMgr, _pinBall[0]->myBody,
                                    physicsVars::getSingleton()->KickerMidStrength);

        _midKickers[1] = new kicker("kicker_mid_right.mesh", mSceneMgr, _pinBall[0]->myBody,
                                    physicsVars::getSingleton()->KickerMidStrength);

        _timed_kicker = new timedKicker("timed_kicker.mesh",
                                        mSceneMgr,
                                        _pinBall[0]->myBody,
                                        physicsVars::getSingleton()->Timed_plungerStrength,
                                        1000);


        //http://www.ogre3d.org/phpBB2addons/viewtopic.php?p=27130#27130

        _mainTable = new table(mSceneMgr);

        for ( int i = 0; i < 4; ++ i )
        {
            mScene->mSceneContactController->addReporter(_thumper[i]);
        }

        mScene->mSceneContactController->addReporter(_rearKickers[0]);
        mScene->mSceneContactController->addReporter(_rearKickers[1]);

        mScene->mSceneContactController->addReporter(_midKickers[0]);
        mScene->mSceneContactController->addReporter(_midKickers[1]);
        mScene->mSceneContactController->addReporter(_timed_kicker);

        loadinganimation::getSingleton()->increment();

        BingoManager::getSingleton()->initBingoMarkers();

        loadinganimation::getSingleton()->increment();

        //--------------------------------------------------------TRIGGERLIGHTS
        for ( int i = 0; i < MAX_TRIGGER_LIGHTS; i ++ )
            {
                loadinganimation::getSingleton()->increment();
                triggerLightManager::getSingleton()->_trigLight[i]->create("triglight",
                                                                           "trigger_light.mesh",
                                                                           TRIGLIGHT_POS[i],
                                                                           _pinBall[0]->myBody,
                                                                           mSceneMgr,
                                                                           1.5);
            }



        mGroupFlippers->setCollisionResponse(mGroupTable, ShapeGroup::CR_No_Collision);

        _stopper = stopper::getSingleton();

        createTriggers();

		
        for ( int i = 0; i < 3; ++ i )
        {
            _pin[i] = new TablePin(physicsVars::getSingleton()->PinPos[i]);
		
        }
		
    }

void PinballGame::createTriggers ()
    {
        NxVec3 TriggerSize = NxVec3(.5, .5, .5);
        NxVec3 CenterTargetTriggerSize = NxVec3(1, .5, .1);

        //---------------------------------------------------CENTER
        trigger[0] = new myTrigger();
        trigger[0]->CreateTriggerBox(NxVec3(- 0.9, .0, - .98), CenterTargetTriggerSize, "0", false);

        //---------------------------------------------------BANK 1
        trigger[1] = new myTrigger();
        trigger[1]->CreateTriggerBox(NxVec3(- 7.89, .0, - .05), TriggerSize, "1");

        trigger[2] = new myTrigger();
        trigger[2]->CreateTriggerBox(NxVec3(- 6.877, .0, - 1.44), TriggerSize, "2");

        //---------------------------------------------------BANK 2
        trigger[3] = new myTrigger();
        trigger[3]->CreateTriggerBox(NxVec3(- 4.99, .0, - 9.67), TriggerSize, "3");

        trigger[4] = new myTrigger();
        trigger[4]->CreateTriggerBox(NxVec3(- 4.087, .0, - 10.986), TriggerSize, "4");


        //----------------------------------------------------------------LANES AT TOP
        trigger[5] = new myTrigger();
        trigger[5]->CreateTriggerBox(NxVec3(- 1.99, .0, - 13), TriggerSize, "5");
        trigger[6] = new myTrigger();
        trigger[6]->CreateTriggerBox(NxVec3(0.01, .0, - 13), TriggerSize, "6");
        trigger[7] = new myTrigger();
        trigger[7]->CreateTriggerBox(NxVec3(1.9, .0, - 13), TriggerSize, "7");
        trigger[8] = new myTrigger();
        trigger[8]->CreateTriggerBox(NxVec3(3.8, .0, - 13), TriggerSize, "8");


        //---------------------------------------------------BANK 3
        trigger[9] = new myTrigger();
        trigger[9]->CreateTriggerBox(NxVec3(5.86, .0, - 10.79), TriggerSize, "9");

        trigger[10] = new myTrigger();
        trigger[10]->CreateTriggerBox(NxVec3(6.755, .0, - 9.53), TriggerSize, "10");

        trigger[11] = new myTrigger();
        trigger[11]->CreateTriggerBox(NxVec3(7.036, .0, - 7.881), TriggerSize, "11");

        trigger[12] = new myTrigger();
        trigger[12]->CreateTriggerBox(NxVec3(7, .0, - 6.302), TriggerSize, "12");

        //---------------------------------------------------BANK 4
        trigger[13] = new myTrigger();
        trigger[13]->CreateTriggerBox(NxVec3(5.229, .0, - 1.565), TriggerSize, "13");

        trigger[14] = new myTrigger();
        trigger[14]->CreateTriggerBox(NxVec3(5.983, .0, - .039), TriggerSize, "14");

        trigger[15] = new myTrigger();
        trigger[15]->CreateTriggerBox(NxVec3(6.7, .0, 1.383), TriggerSize, "15");


        //----------------------------------------------------------------LANE LIGHTS
        trigger[16] = new myTrigger();
        Vector3 _pos = TRIGLIGHT_POS[0];
        trigger[16]->CreateTriggerBox(NxVec3(_pos.x, _pos.y, _pos.z), TriggerSize, "16");

        trigger[17] = new myTrigger();
        _pos = TRIGLIGHT_POS[1];
        trigger[17]->CreateTriggerBox(NxVec3(_pos.x, _pos.y, _pos.z), TriggerSize, "17");

        trigger[18] = new myTrigger();
        _pos = TRIGLIGHT_POS[2];
        trigger[18]->CreateTriggerBox(NxVec3(_pos.x, _pos.y, _pos.z), TriggerSize, "18");

        trigger[19] = new myTrigger();
        _pos = TRIGLIGHT_POS[3];
        trigger[19]->CreateTriggerBox(NxVec3(_pos.x, _pos.y, _pos.z), TriggerSize, "19");


        //----------------------------------------------------------------DRAINS
        trigger[20] = new myTrigger();
        trigger[20]->CreateTriggerBox(NxVec3(- 8.6, .0, 9), TriggerSize, "20");
        trigger[21] = new myTrigger();
        trigger[21]->CreateTriggerBox(NxVec3(7, .0, 9), TriggerSize, "21");
        trigger[22] = new myTrigger();
        trigger[22]->CreateTriggerBox(NxVec3(- 7, .0, 9), TriggerSize, "30");
        trigger[23] = new myTrigger();
        trigger[23]->CreateTriggerBox(NxVec3(5.5, .0, 9), TriggerSize, "31");
    }

void PinballGame::pinballFrame ( float deltaTime )
    {
        _time = deltaTime;

		

        _stopper->update(deltaTime, nodeShock);

        for ( int i = 0; i < MAX_TRIGGER_LIGHTS; i ++ )
        {
            triggerLightManager::getSingleton()->_trigLight[i]->update();
        }

        switch ( GAME_STATE )
            {
                case GAMESTATE_GOTO_INTERMISSION:
                    loadinganimation::getSingleton()->hide();

                    //scoreposted = false;
                    if ( InetOK )
                        {
                            LogManager::getSingletonPtr()->logMessage(LogMessageLevel::LML_NORMAL,
                                                                      "Getting Standings-Pre Intermission");
                            //gamedata::getSingleton()->getStandings();
                            //displayStandings();
                        }

                    resetHudText();
                    GAME_STATE = GAMESTATE_INTERMISSION;
                    BubbleNotify::getSingleton()->hide();
   
                    hnm = hitNumberManager::getHitNumMan();
                    hnm->resetHitList();

                    break;

                case GAMESTATE_INIT:

					resetHudText();
                    loadinganimation::getSingleton()->hide();

                    ballResetTime = timeGetTime();
                    hnm = hitNumberManager::getHitNumMan();
                    hnm->resetHitList();

                    scoreposted = false;

                    hideStandings();

                    if ( _intermission_screen )
                    {
                        _intermission_screen->hide();
                    }

                    BingoManager::getSingleton()->initBoard();
                    BingoManager::getSingleton()->resetCard(- 2);

                    _ballsRemaining = gamedata::getSingleton()->NUMBALLS;
                    LogManager::getSingletonPtr()->logMessage("NumBallsInit:"
                                                                  + StringConverter::toString(_ballsRemaining));
                    StatManager::getSingleton()->addBall();
                    nmBumps->setText("");
                    nmScore->setText("");
                    otIntermissionHeader->setText("");
                    otIntermissionSubText->setText("");

                    otHighScore->setAlign(TextAreaOverlayElement::Alignment::Right);
                    otHighScore->setHeight(30);
                    otHighScore->setPos(.2373, .0559);
                    otHighScore->setText(Ogre::StringConverter::toString(gamedata::getSingleton()->HIGHSCORE));

                    for ( int i = 0; i < 12; i ++ )
                    {
                        BingoManager::getSingleton()->bingo_marker[i]->hide();
                    }

                    ScoreManager::setScore(0);
                    _ballRunning = false;
                    GAME_STATE = GAMESTATE_RUNNING;

                    BingoManager::getSingleton()->BINGOCOUNT = 0;

                    break;

                case GAMESTATE_RUNNING:
                    updateKickers();
                    updateBalls(_time);
                    updateFlippers();
                    updatePlunger(_time);
                    updateGui(_time);

                    BubbleNotify::getSingleton()->update();

                    for ( int i = 0; i < 4; i ++ )
                    {
                        _thumper[i]->update(_time);
                    }

                    if ( ! _ballRunning )
                        {
                            if ( _ballsRemaining == 0 )
                                {

                                    GAME_STATE = GAMESTATE_GOTO_INTERMISSION;
                                    LogManager::getSingletonPtr()->logMessage(LogMessageLevel::LML_NORMAL,
                                                                              "Going to Intermission");

                                    int lowScore = gamedata::getSingleton()->LOWSCORE;
                                    int myScore = ScoreManager::getScoreManager()->getScore();

                                    if ( myScore > lowScore )
                                    {
                                        INTERMISSION_STATE = INTERMISSION_STATE_QUESTION_SEND_SCORE;
                                    }
                                    else
                                    {
                                        INTERMISSION_STATE = INTERMISSION_STATE_PREGAME;
                                    }
                                        
                                }

                            spinBingo();
                            DWORD thisTime = timeGetTime();
                            DWORD diff;
                            diff = (thisTime - ballResetTime) / 1000;

                            if ( diff > 30 )
                                {
                                    _plungerStrength = 100;
                                    fireBall(0);
                                }
                        }

                    else
                    {
                        _ballDeducted = false;
                    }

                    break;

                case GAMESTATE_RESETBALL:
                    break;

                case GAMESTATE_BALLRESET:
                    break;

                case GAMESTATE_INTERMISSION:
                    if ( ! _intermission_screen->visible )
                    {
                        _intermission_screen->show();
                    }

                    ballResetTime = timeGetTime();

                    updateBalls(_time);

                    spinBingo();

                    if ( INTERMISSION_STATE != INTERMISSION_STATE_ENTER_INITIALS )
                    {
                        updateFlippers();
                    }

                    checkIntermissionState();

                    break;

                case GAMESTATE_GETSTANDINGSERROR:
                    break;
            }
    }

void PinballGame::initBoard () { }

void PinballGame::preloadText ()
    {
        for ( int i = 0; i < 15; i ++ )
            {
                loadinganimation::getSingleton()->increment();
                txtScores[i] = new OgreText();
                txtScores[i]->setHeight(26);
                txtUsernames[i] = new OgreText();
                txtUsernames[i]->setHeight(26);
            }

        for ( int i = 0; i < 5; i ++ )
            {
                loadinganimation::getSingleton()->increment();
                otAllNumbers[i] = new OgreText();
                otAllNumbers[i]->setHeight(18);
                otAllNumbers[i]->setCol(1, 1, 1, .3);
                otAllNumbers[i]->setPos(.0166,.45 + i*.0195);

                otAllNumbers[i]->setVisible(false);
            }

        otAllNumbers[0]->setText("B...20k");
        otAllNumbers[1]->setText("I.....20k");
        otAllNumbers[2]->setText("N...20k");
        otAllNumbers[3]->setText("G...20k");
        otAllNumbers[4]->setText("O...20k");
    }


/*
//////////////////////////////////////////////////////////////////////////
GAME STUFF
//////////////////////////////////////////////////////////////////////////
*/
void PinballGame::ResetObject ( Ogre::Real x, Ogre::Real y, Ogre::Real z, NxForceMode _forceMode )
{
    _oldTargetBody->addForce(Vector3(x, y, z), _forceMode);
}

void PinballGame::updateBalls ( float _time )
    {
        Vector3 ballPos;

        for ( int i = 0; i < sphereCount; i ++ )
            {
                _pinBall[i]->update(tableSlope * _time);

                ballPos = _pinBall[i]->myBody->getGlobalPosition();

                if ( (ballPos.x > 10) || (ballPos.x < - 10) )
                {
					LogManager::getSingletonPtr()->logMessage("reset ball x");
					resetBall(i);

                }

                if ( (ballPos.z > 20) || (ballPos.z < - 20) )
                {
					LogManager::getSingletonPtr()->logMessage("reset ball z1");
                    resetBall(i);
                }

                if ( ballPos.z > 16.4 )
                    {
                        if ( _pinBall[i]->ballState != BALLSTATE_RESET )
                            {
                                LogManager::getSingletonPtr()->logMessage(LML_NORMAL,
                                                                          "Reset Ball from UpdateBalls");
                                _pinBall[i]->myBody->setGlobalPosition(Vector3(8, .5, 12.7));
                                _pinBall[i]->myBody->setLinearVelocity(Vector3(0, 0, 0));
                                _pinBall[i]->myBody->addForce(Vector3(4, 0, 0));
                                _pinBall[i]->ballState = BALLSTATE_RESET;
								LogManager::getSingletonPtr()->logMessage("reset ball z2");
                                resetBall(i);
                            }
                    }
            }
    }

void PinballGame::resetBall ( int ballID )
    {
        if ( mShutdownNow )
        {
            return;
        }

        if ( ballID > - 1 )
            {
                delayTimerStart = timeGetTime();
                _pinBall[ballID]->ballState = BALLSTATE_PRIMED;
                SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_NEWBALL);
                LogManager::getSingletonPtr()->logMessage(LML_NORMAL, "Reset Ball");
                _plungerStrength = 0;
                _ballRunning = false;
                _maxBumps = gamedata::getSingleton()->NUMBUMPS;

                if ( ! _ballDeducted )
                    {
                        -- _ballsRemaining;
                        StatManager::getSingleton()->addBall();
                        LogManager::getSingletonPtr()->logMessage("BallsRemaining:"
                                                                      + StringConverter::toString(_ballsRemaining));
                        _ballDeducted = true;
                    }

                if ( _ballsRemaining > 0 )
                {
                    BubbleNotify::getSingleton()->setMessage("Get Ready!", 25, 2);
                }

                ballResetTime = timeGetTime();
            }
    }

void PinballGame::updateKickers ()
{
    _timed_kicker->update();
}

void PinballGame::displayStandings ()
    {
		/*
        Real yPos = .3164;
        Real initX = .278;
        Real initX_2 = .483;

        for ( int i = 0; i < (int)gamedata::getSingleton()->szVecStandingsUserNames.size(); i ++ )
            {
                txtUsernames[i]->setPos(initX, yPos);
                txtUsernames[i]->setText(gamedata::getSingleton()->szVecStandingsUserNames[i].c_str());
                txtUsernames[i]->setAlign(TextAreaOverlayElement::Alignment::Left);
                txtScores[i]->setPos(initX_2, yPos);
                txtScores[i]->setText(gamedata::getSingleton()->szVecStandingsScores[i].c_str());
                txtScores[i]->setAlign(TextAreaOverlayElement::Alignment::Right);
                yPos += .02343;
            }
			*/
    }

void PinballGame::updateFlippers ()
    {
        if ( _flipper[0]->FlipperBody != 0 )
            {
                if ( isKeyDown(FLIP_LEFT1) || isKeyDown(FLIP_LEFT2) || isKeyDown(FLIP_LEFT3)
                         || isKeyDown(FLIP_LEFT4) || isKeyDown(FLIP_LEFT5) || isKeyDown(FLIP_LEFT6)
                         || isKeyDown(FLIP_LEFT7) || isKeyDown(FLIP_LEFT8) || isKeyDown(FLIP_LEFT9)
                         || isKeyDown(FLIP_LEFT10) )
                {

                    _flipper[0]->updateMe(true);
					_pinBall[0]->capSpeed(20);
                }
                else
                {
                    _flipper[0]->updateMe(false);
                }
            }

        if ( _flipper[1]->FlipperBody != 0 )
            {
                if ( isKeyDown(FLIP_RIGHT1) || isKeyDown(FLIP_RIGHT2) || isKeyDown(FLIP_RIGHT3)
                         || isKeyDown(FLIP_RIGHT4) || isKeyDown(FLIP_RIGHT5) || isKeyDown(FLIP_RIGHT6)
                         || isKeyDown(FLIP_RIGHT7) || isKeyDown(FLIP_RIGHT8) || isKeyDown(FLIP_RIGHT9)
                         || isKeyDown(FLIP_RIGHT10) )
                {
                    _flipper[1]->updateMe(true);
					_pinBall[0]->capSpeed(20);
                }
                else
                {
                    _flipper[1]->updateMe(false);
                }
            }
    }

void PinballGame::logMes ( String txt )
{
    LogManager::getSingleton().logMessage(LogMessageLevel::LML_NORMAL, txt);
}

void PinballGame::spinBingo ()
    {
        CurTime = timeGetTime();

        if ( CurTime - LastTime > 200 )
            {
                LastTime = CurTime;

                if ( QuadManager::CurrBingoLetter > 4 )
                {
                    QuadManager::CurrBingoLetter = 1;
                }
                else
                {
                    QuadManager::CurrBingoLetter ++;
                }

                QuadManager::SwitchNumberSet(QuadManager::CurrBingoLetter);
                lm->resetLightValues();
            }
    }
/*
//////////////////////////////////////////////////////////////////////////
GUI / HUD
//////////////////////////////////////////////////////////////////////////
*/
void PinballGame::addTextOverlays ()
    {
        OverlayManager & overlayManager = OverlayManager::getSingleton();

        // Create a panel
        OverlayContainer *
            panel = static_cast<OverlayContainer *>(overlayManager.createOverlayElement("Panel",
                                                                                        "PanelName"));

        panel->setMetricsMode(Ogre::GMM_PIXELS);
        panel->setPosition(10, 10);
        panel->setDimensions(100, 100);
    }

void PinballGame::addOverlays ()
    {
        OverlayManager & overlay_man = OverlayManager::getSingleton();

        OverlayContainer *
            HUD_Bumps = static_cast<OverlayContainer *>(overlay_man.createOverlayElement("Panel",
                                                                                         "Bumps"));

        HUD_Bumps->setMetricsMode(GMM_RELATIVE);
        HUD_Bumps->setPosition(- .03, .833);
        HUD_Bumps->setDimensions(.25, .166);
        HUD_Bumps->setMaterialName("HudBumpsOverlayMaterial");
        Overlay * overalayBumps = overlay_man.create("HudBumpsOverlay");
        overalayBumps->add2D(HUD_Bumps);

        OverlayContainer *
            HUD_Power = static_cast<OverlayContainer *>(overlay_man.createOverlayElement("Panel",
                                                                                         "Power"));

        HUD_Power->setMetricsMode(GMM_RELATIVE);
        HUD_Power->setPosition(.78, .833);
        HUD_Power->setDimensions(.25, .166);

        HUD_Power->setMaterialName("HudPowerOverlayMaterial");
        Overlay * overalayPower = overlay_man.create("HudPOWEROverlay");
        overalayPower->add2D(HUD_Power);

        OverlayContainer *
            panelLeft = static_cast<OverlayContainer *>(overlay_man.createOverlayElement("Panel",
                                                                                         "Score"));

        panelLeft->setMetricsMode(GMM_RELATIVE);
        panelLeft->setPosition(0, 0);
        panelLeft->setDimensions(.5, 1.33);

        panelLeft->setMaterialName("LeftOverlayMaterial");
        Overlay * overalayLeft = overlay_man.create("LeftOverlay");
        overalayLeft->add2D(panelLeft);

        OverlayContainer *
            panelRight = static_cast<OverlayContainer *>(overlay_man.createOverlayElement("Panel",
                                                                                          "Right"));

        panelRight->setMetricsMode(GMM_RELATIVE);
        panelRight->setPosition(.5, 0);
        panelRight->setDimensions(.5, 1.33);

        panelRight->setMaterialName("RightOverlayMaterial");
        Overlay * overalayRight = overlay_man.create("RightOverlay");
        overalayRight->add2D(panelRight);
        overalayBumps->show();
        overalayPower->show();
        overalayLeft->show();
        overalayRight->show();
    }

void PinballGame::initGui ()
    {
        
		

        mOverlay = Ogre::OverlayManager::getSingleton().create("SPLASH");

        Ogre::String loadingBackMat = "boot.any";

        OverlayElement * loadingBack = createOverlay("loadBack",
                                                     Vector2(0, 0),
                                                     Vector2(mRenderer->getWidth(), mRenderer->getHeight()),
                                                     loadingBackMat);

        mOverlay->show();

        loadinganimation::getSingleton()->setRoot(mRenderer->getRoot());
        loadinganimation::getSingleton()->increment("Init");

        nmBingos = new numberMachine();
        nmBingos->create(Vector2(.957, .026), TextAreaOverlayElement::Alignment::Center, 40, 50, 10,
                         "bingos");

        loadinganimation::getSingleton()->increment("Displays");
        nmScore = new numberMachine();
        nmScore->create(Vector2(.242, .0078), TextAreaOverlayElement::Alignment::Right, 38, 42, 50.0f,
                        "score");

        nmBalls = new numberMachine();
        nmBalls->create(Vector2(.1689, .095), TextAreaOverlayElement::Alignment::Center, 35, 40, 50.0f,
                        "balls");

        loadinganimation::getSingleton()->increment();
        nmBumps = new numberMachine();
        nmBumps->create(Vector2(.0927, .89), TextAreaOverlayElement::Alignment::Center, 60, 90, 25.0f,
                        "bumps");

        loadinganimation::getSingleton()->increment();
        nmPower = new numberMachine();
        nmPower->create(Vector2(.908, .892), TextAreaOverlayElement::Alignment::Center, 60, 60, 50.0f,
                        "power");

        otPlayerInitials = new OgreText();
        otPlayerInitials->setHeight(35);
        otPlayerInitials->setPos(.244, .244);
        otPlayerInitials->setText("");
        mRenderer->getRoot()->renderOneFrame();

        firstFrame = true;
        timeSince = 0;

        loadinganimation::getSingleton()->increment();
    }

void PinballGame::resetHudText ()
    {
        for ( int i = 1; i < 6; i ++ )
            {
                allNumbers[i - 1] = false;
                otAllNumbers[i - 1]->setCol(1, 1, 1, .3);
            }

        bingoballmarker::getSingleton()->step(0);

        for ( int i = 0; i < 5; i ++ )
        {
            otAllNumbers[i]->setVisible(true);
        }

        otHighScore->setText("");
        nmBalls->reset();
        nmPower->reset();
        nmBumps->reset();
        recentScoreManager::getRecentScoreManager()->reset();
    }

void PinballGame::updateGui ( float diff )
    {
        Real fps = mRenderer->getWindow()->getAverageFPS();

        if ( fps < 40 && showSlowFrameWarning)
        {
            if(slowFrameWarning->getMilliseconds() > slowFrameWarningDelay)
            otUserMessage->setText("Press F1 to improve choppy gameplay. (FPS < 40)");
        }
        else
        {
            otUserMessage->setText("");
        }

        int bc = BingoManager::getSingleton()->BINGOCOUNT;

        if ( nmBingos->val != bc )
            {
                nmBingos->UpdateValue(bc);
                bingoballmarker::getSingleton()->step(bc);
            }

        bingoballmarker::getSingleton()->update(diff);

        if ( nmScore->val != ScoreManager::getScore() )
        {
            nmScore->UpdateValue(ScoreManager::getScore());
        }

        if ( nmBumps->val != _maxBumps )
        {
            nmBumps->UpdateValue(_maxBumps);
        }

        if ( nmBalls->val != _ballsRemaining )
            {
                if ( (_ballsRemaining <= gamedata::getSingleton()->NUMBALLS) && (_ballsRemaining > 0) )
                {
                    nmBalls->UpdateValue(_ballsRemaining);
                }
            }

        nmPower->val = _plungerStrength;

        nmScore->render(diff);
        nmBumps->render(diff);
        nmPower->render(diff);
        nmBalls->render(diff);

        recentScoreManager::getRecentScoreManager()->display();

        for ( int i = 1; i < 6; i ++ )
            {
                if ( hitNumberManager::getHitNumMan()->allNumbers[i] )
                    {
                        if ( ! allNumbers[i - 1] )
                            {
                                allNumbers[i - 1] = true;
                                otAllNumbers[i - 1]->setCol(1, 1, 1, 1);
                                gamedata::getSingleton()->logMessage("ALLNUMBERS="
                                                                         + StringConverter::toString(i));
                            }
                    }
            }
    }


/*
//////////////////////////////////////////////////////////////////////////
INPUT 
//////////////////////////////////////////////////////////////////////////
*/
bool PinballGame::isKeyDown ( Keys k )
{
    return InputHandler::getInputHandler()->isKeyDown(k);
}

bool PinballGame::keyReleased ( const KeyEvent & arg )
{
    return true;
}

bool PinballGame::keyPressed ( const KeyEvent & arg )
    {
        if (arg.key == KC_F2)
        {
            showFps = !showFps;
        }

        if (arg.key == KC_F1)
        {
            showSlowFrameWarning=false;
            if(mRenderer->getWindow()->isFullScreen())
            {
                mRenderer->getWindow()->setFullscreen(true,640,480);
            }
            else
            {
                mRenderer->getWindow()->resize(640,480);
            }
            mRenderer->getWindow()->windowMovedOrResized();
            mRenderer->SetRenderParam("Video Mode","640 x 480 @ 16-bit colour");
        }

        if ( INTERMISSION_STATE != INTERMISSION_STATE_ENTER_INITIALS )
        {
            return true;
        }

        if ( arg.text == 8 )
            {
                if ( gamedata::getSingleton()->szPlayerInitials.size() > 0 )
                    {
                        gamedata::getSingleton()->szPlayerInitials.erase(gamedata::getSingleton()->szPlayerInitials.size()
                                                                             - 1);

                        return true;
                    }
            }

        if ( arg.text < 48 )
        {
            return true;
        }

        if ( arg.text > 122 )
        {
            return true;
        }

        if ( (arg.text > 57) && (arg.text < 65) )
        {
            return true;
        }

        if ( (arg.text > 90) && (arg.text < 97) )
        {
            return true;
        }

        if ( arg.text < 48 || arg.text > 122 )
            return true;
		/*
        string k;
        k = static_cast<char>(arg.text);

        if ( ! scoreposted )
            {
                if ( gamedata::getSingleton()->szPlayerInitials.length() < 3 )
                {
                    gamedata::getSingleton()->szPlayerInitials += k;
                }
            }
		*/
        return true;
    }

void PinballGame::createInputSys ( RenderWindow * _window ) { }

void PinballGame::checkIntermissionState ()
    {
        _thisKeyTime = timeGetTime();
        float diff = _thisKeyTime - _lastKeyTime;

        if ( diff < 1 )
            {
                _lastKeyTime = timeGetTime();
                return;
            }

        bool writeDebug = false;

        switch ( INTERMISSION_STATE )
            {
                case INTERMISSION_STATE_PREGAME:
                    otIntermissionHeader->setPos(.5078, .24479);
                    otIntermissionHeader->setText("Pinball Bingo Standings");
                    otIntermissionHeader->setHeight(50);
                    otIntermissionHeader->setAlign(TextAreaOverlayElement::Alignment::Center);
                    otIntermissionSubText->setText("");

                    if ( isKeyDown(NEW_GAME) )
                        {
                            GAME_STATE = GAMESTATE_INIT;
                            _lastKeyTime = timeGetTime();
                            return;
                        }

                    break;

                case INTERMISSION_STATE_QUESTION_SEND_SCORE:
                    otIntermissionHeader->setPos(.5078, .24479);
                    otIntermissionHeader->setText("Send Score? Y/N, Esc=Exit");
                    otIntermissionHeader->setHeight(50);
                    otIntermissionHeader->setAlign(TextAreaOverlayElement::Alignment::Center);

                    if ( isKeyDown(POST_SCORE) )
                        {
                            INTERMISSION_STATE = INTERMISSION_STATE_ENTER_INITIALS;
                            logMes("Going to Enter Initials");
                            _lastKeyTime = timeGetTime();
                            return;
                        }

                    else if ( isKeyDown(NO_POST_SCORE) )
                        {
                            GAME_STATE = GAMESTATE_INIT;

                            _lastKeyTime = timeGetTime();
                            return;
                        }

                    break;

                case INTERMISSION_STATE_ENTER_INITIALS:
                    otIntermissionHeader->setPos(.247, .24869);
                    otIntermissionHeader->setText("Enter Your Initials....:"
                                                      + gamedata::getSingleton()->szPlayerInitials);
                    otIntermissionHeader->setHeight(50);
                    otIntermissionHeader->setAlign(TextAreaOverlayElement::Alignment::Left);
                    otIntermissionSubText->setText("Press Enter to Send");
                    otIntermissionSubText->setHeight(35);
                    otIntermissionSubText->setPos(.5195, .315);
                    otIntermissionSubText->setAlign(TextAreaOverlayElement::Alignment::Left);

                    if ( isKeyDown(SEND_SCORE) )
                        {
                            String fps = StringConverter::toString(mRenderer->getWindow()->getAverageFPS());


                            INTERMISSION_STATE = INTERMISSION_STATE_SCORE_POSTED;
                            LogManager::getSingletonPtr()->logMessage(LogMessageLevel::LML_NORMAL,
                                                                      "Posted Score to Web");
                            //gamedata::getSingleton()->getStandings();
                            //displayStandings();

                            _lastKeyTime = timeGetTime();
                            return;
                        }

                    break;

                case INTERMISSION_STATE_SCORE_POSTED:
                    otIntermissionHeader->setPos(.5078, .24479);
                    otIntermissionHeader->setText("Score Posted!");
                    otIntermissionHeader->setHeight(50);
                    otIntermissionHeader->setAlign(TextAreaOverlayElement::Alignment::Center);
                    otIntermissionSubText->setText("");

                    if ( isKeyDown(NEW_GAME) )
                        {
                            GAME_STATE = GAMESTATE_INIT;

                            _lastKeyTime = timeGetTime();
                            return;
                        }

                    break;

                case INTERMISSION_STATE_INET_ERROR_STANDINGS:
                    otIntermissionHeader->setPos(.5078, .24479);
                    otIntermissionHeader->setText("Internet Problem");
                    otIntermissionHeader->setHeight(50);
                    otIntermissionHeader->setAlign(TextAreaOverlayElement::Alignment::Center);
                    otIntermissionSubText->setText("Could not retrieve the Standings");
                    otIntermissionSubText->setPos(.5078, .289);
                    otIntermissionSubText->setAlign(TextAreaOverlayElement::Alignment::Center);

                    if ( isKeyDown(NEW_GAME) )
                        {
                            GAME_STATE = GAMESTATE_INIT;

                            _lastKeyTime = timeGetTime();
                            return;
                        }

                    break;

                case INTERMISSION_STATE_INET_ERROR_POSTSCORE:
                    otIntermissionHeader->setPos(.5078, .23698);
                    otIntermissionHeader->setText("Internet Problem");
                    otIntermissionHeader->setHeight(50);
                    otIntermissionHeader->setAlign(TextAreaOverlayElement::Alignment::Center);
                    otIntermissionSubText->setText("Problem posting score. Enter to Retry");
                    otIntermissionSubText->setPos(.5078, .289);
                    otIntermissionSubText->setAlign(TextAreaOverlayElement::Alignment::Center);

                    if ( isKeyDown(NEW_GAME) || isKeyDown(NO_POST_SCORE) )
                        {
                            GAME_STATE = GAMESTATE_INIT;
                            _lastKeyTime = timeGetTime();
                            return;
                        }

                    else if ( isKeyDown(SEND_SCORE) )
                        {
                            otIntermissionSubText->setText("Attempting to post score...");
                            _lastKeyTime = timeGetTime();
                            return;
                        }

                    break;
            }
    }

void PinballGame::captureInput () { }

void PinballGame::updatePlunger ( float _time )
    {
        float delayTimerCurrent = timeGetTime();

        if ( delayTimerStart != - 1 && ((delayTimerCurrent - delayTimerStart) / 1000 < 2)
                                      && _ballsRemaining < gamedata::getSingleton()->NUMBALLS )
        {
            return;
        }

        if ( isKeyDown(FIRE_BALL1) || isKeyDown(FIRE_BALL2) )
            {
                if ( _pinBall[0]->ballState == BALLSTATE_INPLAY )
                    {
                        if ( _ballRunning )
                            {
                                bumpBall(0);
                                return;
                            }
                    }

                _plungerState = 1;

                if ( _plungerStrength < 100 )
                    {
                        _plungerStrength += 100 * _time;

                        if ( _plungerStrength > 100 )
                        {
                            _plungerStrength = 100;
                        }
                    }
            }

        else
            {
                if ( _plungerState == 1 )
                    {
                        if ( _plungerStrength > 100 )
                        {
                            _plungerStrength = 100;
                        }

                        if ( _plungerStrength < 50 )
                        {
                            _plungerStrength = 55;
                        }

                        fireBall(0);
                    }
            }
    }

void PinballGame::bumpBall ( int ballID )
    {
        _thisBumpTime = timeGetTime();
        float diff = _thisBumpTime - _lastBumpTime;

        if ( diff < 100 )
            {
                _lastBumpTime = timeGetTime();
                return;
            }

        if ( _maxBumps <= 0 )
            {
                SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_TILT);
                _lastBumpTime = timeGetTime();
                return;
            }

        if ( diff < BUMP_DELAY )
            {
                SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_TILT);
                _lastBumpTime = timeGetTime();
                return;
            }

        _pinBall[ballID]->myBody->addForce(Vector3(0, 0, - physicsVars::getSingleton()->BumpStrength));
        SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_BUMP);
        gamedata::getSingleton()->logMessage("BUMP=" + StringConverter::toString(_maxBumps));
        _lastBumpTime = timeGetTime();
        -- _maxBumps;
    }

void PinballGame::fireBall ( int ballID )
    {
        if ( ballID > - 1 )
            {
                Real modifiedPlungerPower = _plungerStrength + physicsVars::getSingleton()->PlungerPower;
                _pinBall[ballID]->myBody->addForce(Vector3(0, 0, - (modifiedPlungerPower)));
				_pinBall[0]->capSpeed(20);
                SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_PLUNGER);
                _plungerState = 0;
                _ballRunning = true;
                GAME_STATE = GAMESTATE_RUNNING;
                //_pinBall[ballID]->ballState = BALLSTATE_SHOT;
				_pinBall[ballID]->ballState = BALLSTATE_INPLAY;

                gamedata::getSingleton()->logMessage("SHOOTBALL="
                                                         + StringConverter::toString(_ballsRemaining));
            }
    }


/*
//////////////////////////////////////////////////////////////////////////
SERVER COMM 
//////////////////////////////////////////////////////////////////////////
*/
void PinballGame::hideStandings ()
    {
        LogManager::getSingletonPtr()->logMessage("Hiding Standings");

        if ( gamedata::getSingleton()->szVecStandingsUserNames.size() > 0 )
            {
                for ( int i = 0; i < (int)gamedata::getSingleton()->szVecStandingsUserNames.size(); i ++ )
                    {
                        txtScores[i]->setText("");
                        txtUsernames[i]->setText("");
                    }
            }

        LogManager::getSingletonPtr()->logMessage("Done Hiding Standings");
    }
