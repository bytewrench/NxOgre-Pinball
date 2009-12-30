
#include "stdafx.h"

class flipper
    {
        public:

            bool flipperActive;
            Body * FlipperAnchor;
			Body * FlipperBody;
			NxMotorDesc _motorDesc;
            NxRevoluteJoint * flipperJoint;
			NxActor * _myActor;
            float flipperYPOS;
            int _orient;
            bool testMode;
            float flipTimer;
            bool flipped;
			float _maxforce;
			float _veltarget;
			Vector3 _globalAxis;
			Vector3 _globalAnchor;
			int returnVelTarget;
			void wake ();
			void sleep ();
			void updateMe ( bool isActive );
			NxRevoluteJoint * CreateRevoluteJoint ( NxActor * a1, NxVec3 globalAnchor, NxVec3 globalAxis, float velTarget, float maxForce );
            flipper ( Vector3 pos, int orient, Ogre::SceneManager * SceneMgr, float maxForce, float velTarget );


        protected:
        private:
    };
