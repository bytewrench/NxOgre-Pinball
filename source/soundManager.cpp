#include "stdafx.h"
//==-----------------------------------------------------------------------------------
SoundManager::SoundManager ()
    {
        _result = FMOD::System_Create(& _system);
        _result = _system->init(34, FMOD_INIT_NORMAL, 0);
        _result = _system->createSound("Media/sounds/Mechanisms/thumper_1.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SND_THUMPER1]);
        _result = _system->createSound("Media/sounds/Mechanisms/thumper_1.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SND_THUMPER1]);
        _result = _system->createSound("Media/sounds/Mechanisms/thumper_2.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SND_THUMPER2]);
        _result = _system->createSound("Media/sounds/Mechanisms/flipper.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SND_FLIPPER]);
        _result = _system->createSound("Media/sounds/Mechanisms/switch.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_SWITCH]);
        _result = _system->createSound("Media/sounds/mechanisms/plunger_.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_PLUNGER]);

        _result = _system->createSound("Media/sounds/specials/bingo.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_BINGO]);
        _result = _system->createSound("Media/sounds/specials/special_5.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_DAUBER]);
        _result = _system->createSound("Media/sounds/specials/on2.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_SWITCH_LETTER]);
        _result = _system->createSound("Media/sounds/specials/allnumbers.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_ALLNUMBERS]);
		_result = _system->createSound("Media/sounds/specials/eventup.ogg", FMOD_SOFTWARE,
										0, & _sound[SOUND_TRIGGERLIGHT]);


        _result = _system->createSound("Media/sounds/bells/sound_bell_buzz.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SND_KICKER]);
        _result = _system->createSound("Media/sounds/bells/Sound_Bell_5_High.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_TIMED_PLUNGER]);
        _result = _system->createSound("Media/sounds/bells/Sound_Bell_1_High.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_SPINNER]);
        _result = _system->createSound("Media/sounds/bells/Sound_Bell_Dual_Low.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_TARGET_LIT]);
        _result = _system->createSound("Media/sounds/bells/Sound_Bell_2_High.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_TARGET_DIM]);

        _result = _system->createSound("Media/sounds/action/sound_shoot_ball.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_SHOOT_BALL]);
        _result = _system->createSound("Media/sounds/action/ErrorAlert.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_DRAIN]);
        _result = _system->createSound("Media/sounds/action/NewBall.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_NEWBALL]);
        _result = _system->createSound("Media/sounds/action/blip.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_BUMP]);
        _result = _system->createSound("Media/sounds/action/tilt.ogg", FMOD_SOFTWARE,
                                       0, & _sound[SOUND_TILT]);
		_result = _system->createSound("Media/sounds/action/deactivate.ogg", FMOD_SOFTWARE,
			0, & _sound[SOUND_DEACTIVATE]);

		_result = _system->createSound("Media/sounds/action/stopper.ogg", FMOD_SOFTWARE,
			0, & _sound[SOUND_STOPPER]);

        _system->set3DSettings(1.0, 1.0, 1.0);

        for ( int i = 0; i < 20; i ++ )
        {
            _channel[i]->setPaused(false);
        }
    }

//==-----------------------------------------------------------------------------------
SoundManager * SoundManager::getSoundManager ()
    {
        static SoundManager * sndMgr;
        static int num = 0;

        try
            {
                if ( sndMgr == 0 )
                    {
                        sndMgr = new SoundManager();
                        num ++;
                    }
            }
        catch( char * str )
        {
            LogManager::getSingletonPtr()->logMessage(LogMessageLevel::LML_NORMAL, "Audio Error: "
                                                                                       + String(str));
        }

        return sndMgr;
    }

//==-----------------------------------------------------------------------------------
void SoundManager::Play_Sound ( soundTypes sound )
    {
        try
            {
 
                FMOD_VECTOR vec =
                    {
                    ballBody->getNxActor()->getGlobalPosition().x * 10, 0,
                    ballBody->getNxActor()->getGlobalPosition().z * 10
                    };

                FMOD_VECTOR vel =
                    {
                    0, 0, 0
                    };

                _channel[sound]->set3DAttributes(& vec, & vel);
				
                _result = _system->playSound(FMOD_CHANNEL_REUSE, _sound[sound], false, & _channel[sound]);

                if ( _result != FMOD_OK )
                {
                    LogManager::getSingletonPtr()->logMessage(LogMessageLevel::LML_CRITICAL, "Error on Sound",
                                                              false);
                }
            }
        catch( char * str )
        {
            LogManager::getSingletonPtr()->logMessage(LogMessageLevel::LML_NORMAL, "Audio Error: "
                                                                                       + String(str));
        }
    }
