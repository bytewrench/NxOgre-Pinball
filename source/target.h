#ifndef _TARGET

#define _TARGET

#include "stdafx.h"

//==-----------------------------------------------------------------------------------
const enum TARGETNAME
{
	TARGETBANK_1,
	TARGETBANK_2,
	TARGETBANK_LANES,
	TARGETBANK_3,
	TARGETBANK_4,
	TARGETBANK_CENTER,
	TARGETBANK_DRAINS
};
class targetContactController : customContactReporter
{
public:
	NxOgre::Scene* mScene;
	
	bool onStartTouch ( Actor * a, Actor * b );
	bool onEndTouch ( Actor * a, Actor * b );
	bool onTouch ( Actor * a, Actor * b );

	TARGETNAME _bankname;
	targetContactController(TARGETNAME bankname);
	int getHitTarget ( NxVec3 pos );
	//void onContactNotify(NxContactPair& pair, NxU32 events);

	
};

//==-----------------------------------------------------------------------------------

class target : public customContactReporter
    
    {
        private:

            Body * _ballbody;
            DWORD _thisTime;
            DWORD _lastTime;
            DWORD _currtime;

            String _name;

        
        public:

            Body * _targetBody;

			bool onStartTouch ( Actor * a, Actor * b );
			bool onEndTouch ( Actor * a, Actor * b );
			bool onTouch ( Actor * a, Actor * b );
			int getHitTarget ( NxVec3 pos );
			TARGETNAME _bankname;
			target ( Ogre::String meshName,   Ogre::SceneManager * SceneMgr, Body * ballBody,
				Ogre::String targetName, TARGETNAME BankName );

            
    };





#endif
