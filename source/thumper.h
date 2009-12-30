#ifndef _THUMPER

#define _THUMPER

#include "stdafx.h"

#include <Mmsystem.h>

class thumper : public customContactReporter
    {
        private:

            int _strength;
            String _name;
			//void onContactNotify ( NxContactPair & pair, NxU32 events );

		public:


			bool onStartTouch ( Actor * a, Actor * b );
			bool onEndTouch ( Actor * a, Actor * b );
			bool onTouch ( Actor * a, Actor * b );
			bool propel(Actor * a);
            Body * thumperBody;
            SceneNode * stroker_node;
            Vector3 _origPos, _stroked_pos;
            Entity * thumper_cover;
            SceneNode * thumper_node;
            float stroker_y;
            bool isHit;
			thumper ( Vector3 pos, Ogre::SceneManager * SceneMgr, Body * ballBody, int strength );
			void update ( float _time );
        protected:
        private:
    };

#endif
