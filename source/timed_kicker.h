#ifndef _timedKicker

#define _timedKicker

#include "stdafx.h"

class timedKicker : public customContactReporter
    {
        private:

            Body * _ballbody;
            DWORD _holdTime;
            int _strength;
            NxVec3 force;
            bool _isHolding;
            DWORD _thisTime;
            DWORD _lastTime;
            Vector3 _holdingLocation;
			bool onStartTouch ( Actor * a, Actor * b );
			bool onEndTouch ( Actor * a, Actor * b );
			bool onTouch ( Actor * a, Actor * b );
	
	public:

            Body * timedKickerBody;
			void update ();
			timedKicker ( Ogre::String meshName, Ogre::SceneManager * SceneMgr, Body * ballBody, int strength,
				DWORD HoldTime );

    };

#endif
