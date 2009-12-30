#include "stdafx.h"

bool timedKicker::onStartTouch ( Actor * a, Actor * b ){
	return false;
}
bool timedKicker::onEndTouch ( Actor * a, Actor * b ){
	return false;
}
bool timedKicker::onTouch ( Actor * a, Actor * b ){

	//NxContactStreamIterator i (mContactPair->stream);
	
	
	if (b->getName() != "timed_kicker.mesh")
	{
		return false;
	}

	if(_isHolding)
	{
		return false;
	}

	
	NxReal x, y, z;
	srand(timeGetTime());
	x = ((rand() % 10) + 3) * - 1;
	y = .5;
	z = ((rand() % 4) + 18) * - 1;
	x *= _strength;
	z *= _strength;



	force = NxVec3(z, y, z);
	_isHolding = true;
	_thisTime = timeGetTime();
	_lastTime = _thisTime;
	_holdingLocation.x = 6.1;//a->getGlobalPosition().x;
	_holdingLocation.y = .5f;
	_holdingLocation.z = -3.2;//a->getGlobalPosition().z + .4;
	SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_TIMED_PLUNGER);
	return true;
}



void timedKicker::update ()
{
 	if ( _isHolding )
	{
		_thisTime = timeGetTime();

		if ( (_thisTime - _lastTime) < _holdTime )
		{

			_ballbody->setLinearVelocity(Vector3(0, 0, 0));
			_ballbody->setGlobalPosition(_holdingLocation);
			_ballbody->setAngularVelocity(Vector3(0,0,0));
			//_ballbody->freezeAll();
			return;
		}
		//_ballbody->unFreezeAll();
		_isHolding = false;
		_ballbody->getNxActor()->addForce(force);
		SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_PLUNGER);
		ScoreManager::getScoreManager()->addToScore(SCORE_HOLD,_HOLD);
	}
}



timedKicker::timedKicker ( Ogre::String meshName, Ogre::SceneManager * SceneMgr, Body * ballBody, int strength,
			 DWORD HoldTime )
{
	_ballbody = ballBody;
	_holdTime = HoldTime;
	_thisTime = timeGetTime();
	_lastTime = 0;
	_isHolding = false;
	_strength = strength;

	//material * timedKickerMaterial = mScene->createMaterial("timedKickerMaterial", 0, 0, 1);
	ActorGroup * mtimedKickerGroup;
	Scene * mScene = mWorld->getScenes()->getFirst();
	mtimedKickerGroup = mScene->createActorGroup("timedKickerGroup");

	//something wrong with making createbody use the timed kickers 
	//meshname for the shape param, used sphereshape instead
	timedKickerBody = mScene->createBody(meshName,
		new CapsuleShape(.5,1),
		Vector3(6.575, .5, -2.23),
		"static:yes, group:timedKickerGroup, name:TimedKicker"
		);

	timedKickerBody->getNxActor()->setGroup(18);
	timedKickerBody->getNxActor()->setName("TimedKicker");
	mScene->createContactPair(timedKickerBody, ballBody, false, true, false);

}
