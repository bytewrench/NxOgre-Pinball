#include "stdafx.h"

//http://www.ogre3d.org/phpBB2addons/viewtopic.php?t=1242&view=previous&sid=ce193664e1d3d7c4af509e6f4e2718c6


bool kicker::onStartTouch ( Actor * a, Actor * b ){

	return onTouch(a,b);

}
bool kicker::onEndTouch ( Actor * a, Actor * b ){
	return false;
}
bool kicker::onTouch ( Actor * a, Actor * b ){

	bool canContinue = false;

	if ( b->getName() == _name )
	{
		canContinue = true;
	}else{
		return false;
	}


	NxContactStreamIterator i(mContactPair->stream);

	while ( i.goNextPair() )
	{
		while ( i.goNextPatch() )
		{
			const NxVec3 & contactNormal = i.getPatchNormal();

			while ( i.goNextPoint() )
			{
				const NxVec3 & location = i.getPoint();
				Vector3 ballPos = a->getGlobalPosition();
				NxVec3 drive = NxVec3(ballPos.x, ballPos.y, ballPos.z);
				drive -= location;
				setVectorLength(& drive, _strength);
				a->getNxActor()->addForce(drive);
				SoundManager::getSoundManager()->Play_Sound(SoundManager::SND_KICKER);
				ScoreManager::getScoreManager()->addToScore(SCORE_KICKER, _KICKER);
				return true;
			}
		}
	}

	return true;
}


kicker::kicker ( Ogre::String meshName, Ogre::SceneManager * SceneMgr, Body * ballBody, int strength )
{
	_strength = strength;
	ActorGroup * mKickerGroup;
	Scene * _mScene = mWorld->getScenes()->getFirst();
	//mKickerGroup = _mScene->createActorGroup("KickerGroup");

	_name = meshName + ".obj";

	kickerBody = mScene->createBody(_name,
		new ConvexShape(meshName,"Material: Plastic"),
		Vector3(0, 0, 0),"static:yes"
		/*"static:yes, group:KickerGroup"*/
		);


	//kickerBody->getNxActor()->setGroup(16);
	mScene->createContactPair(kickerBody, ballBody, false, true, false);
	kickerBody->getNxActor()->setName(_name.c_str());
	
	//mScene->getNxScene()->setActorPairFlags(*kickerBody->getNxActor(),*ballBody->getNxActor(),NX_NOTIFY_ON_START_TOUCH);

	//addReporter(this);
	++ kickerCounter;
}