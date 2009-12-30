#include "stdafx.h"

float InvSqrt(float x){
	float xhalf = 0.5f * x;
	int i = *(int*)&x; // store floating-point bits in integer
	i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
	x = *(float*)&i; // convert new bits into float
	x = x*(1.5f - xhalf*x*x); // One round of Newton's method
	return x;
}

PinBall::PinBall ( NxVec3 pos, float size, float mass, Real maxSpeed, bool boxMode )
    {
        static bool created;

        if ( created )
        {
            return;
        }
		_maxSpeed = maxSpeed;

        Scene * _mScene = mWorld->getScenes()->getFirst();

        NxOgre::Material * SteelMat = mScene->createMaterial("Steel");
        SteelMat->setStaticFriction(physicsVars::getSingleton()->BallStaticFriction);
        SteelMat->setDynamicFriction(physicsVars::getSingleton()->BallDynamicFriction);
        SteelMat->setRestitution(physicsVars::getSingleton()->BallRestitution);
		

		SphereShape * s = new SphereShape(size);
		s->mParams.mMaterialAsName = "Steel";

		String params = "Group: BallGroup, Mass: " + StringConverter::toString(physicsVars::getSingleton()->BallMass);
        myBody = _mScene->createBody("sphere.50cm.mesh", s, Vector3(0, 10, 0), params);
        myBody->setGlobalPosition(Vector3(pos.x, pos.y, pos.z));

		myBody->setMaxAngularVelocity(8);
		
        myBody->getEntity()->setMaterialName("ball");
        myBody->getEntity()->setCastShadows(true);
        myBody->getNxActor()->setName("ball");
        myBody->setName("BALL");
		

		




	
		
		
		
		
		
		created = true;
    //++ sphereCount;
    }

void PinBall::update ( Real slope )
    {
        Vector3 vec = myBody->getGlobalPosition();
        myBody->setGlobalPosition(Vector3(vec.x, .5, vec.z));

    }

void PinBall::capSpeed(int max){
	Vector3 vec = myBody->getLinearVelocity();
	int Speed = vec.length();
	if (Speed > max)
	{
		vec.normalise();
		vec *= max;
		myBody->setLinearVelocity(vec);
	}
}