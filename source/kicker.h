#ifndef _kicker

#define _kicker

#include "stdafx.h"


static int kickerCounter = 0;


class kicker  : 
	public customContactReporter
    {
        private:
			String _name;
            int _strength;
			
			
			bool onStartTouch ( Actor * a, Actor * b );
			bool onEndTouch ( Actor * a, Actor * b );
			bool onTouch ( Actor * a, Actor * b );
			
			
        public:

			
            Body * kickerBody;
			kicker ( Ogre::String meshName, Ogre::SceneManager * SceneMgr, Body * ballBody, int strength );
            
    };

#endif
