#ifndef _SOUNDMGR

#define _SOUNDMGR


#include "stdafx.h"
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"




#define MAX_SOUNDS 23
//==-----------------------------------------------------------------------------------
class SoundManager
    {
        public:

            const enum soundTypes
                {
                    SND_THUMPER1,
                    SND_THUMPER2,
                    SND_FLIPPER,
                    SND_KICKER,
                    SOUND_BUMP,
                    SOUND_TILT,
                    SOUND_FLIPPER,
                    SOUND_SWITCH,
                    SOUND_BINGO,
                    SOUND_DAUBER,
                    SOUND_SWITCH_LETTER,
                    SOUND_TIMED_PLUNGER,
                    SOUND_SPINNER,
                    SOUND_TARGET_DIM,
                    SOUND_TARGET_LIT,
                    SOUND_SHOOT_BALL,
                    SOUND_PLUNGER,
                    SOUND_DRAIN,
                    SOUND_NEWBALL,
                    SOUND_ALLNUMBERS,
					SOUND_DEACTIVATE,
					SOUND_TRIGGERLIGHT,
					SOUND_STOPPER
                };

            Body * ballBody;
            static SoundManager * getSoundManager ();
            SoundManager();
            void Play_Sound(soundTypes sound);
            FMOD::System * _system;

        private:

            FMOD::Sound * _sound[MAX_SOUNDS];
            FMOD::Channel * _channel[MAX_SOUNDS];
            FMOD_RESULT _result;
    };

#endif
