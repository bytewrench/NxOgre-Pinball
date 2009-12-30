#include "stdafx.h"


#include "callback.h"

myCallback::myCallback(){

}
myCallback::~myCallback(){

}

void myCallback::onTouch(Actor* a , Actor* b) { 
	Body* bd = static_cast<Body*>(b); 
	//bd->getEntity()->setMaterialName("nx.capsule"); 
} 

void myCallback::onStartTouch(Actor* a, Actor* b)   { 

	Body* bd = static_cast<Body*>(b); 
	/*
	Vector3 direction = b->getGlobalPosition() - a->getGlobalPosition();
	direction *= (physicsVars::getSingleton()->BumperStrength[0]); // need to actual thumper [0-3]
	direction.z = 0;

	b->setLinearVelocity(Vector3(0,0,0));

	b->addForce(direction);

	int randSound = (rand() % 2) + 1;

	if ( randSound == 1 )
	{
		SoundManager::getSoundManager()->Play_Sound(SoundManager::SND_THUMPER1);
	}
	else
	{
		SoundManager::getSoundManager()->Play_Sound(SoundManager::SND_THUMPER2);
	}

	ScoreManager::getScoreManager()->addToScore(SCORE_THUMPER_BUMPER, ScoreType::_THUMPER_BUMPER);
	*/

} 

void myCallback::onEndTouch(Actor* a, Actor* b) { 
	Body* bd = static_cast<Body*>(b); 
	//b->addForce(Vector3(NxMath::rand(-10,10), NxMath::rand(-10,10), NxMath::rand(-10,10)) * b->getMass()); 
} 



