#include "stdafx.h"


static int thumperCounter = 0;

thumper::thumper ( Vector3 pos, Ogre::SceneManager * SceneMgr, Body * ballBody, int strength )
{
	_origPos = _stroked_pos = pos;
	_stroked_pos.y = - .5;
	_strength = strength;
	_name = "thumper." + Ogre::StringConverter::toString(thumperCounter);

	ActorGroup * mThumperGroup;
	Scene * _mScene = mWorld->getScenes()->getFirst();

	NxOgre::Material * plasticMat = mScene->createMaterial("Plastic");
	plasticMat->setStaticFriction(.0);
	plasticMat->setDynamicFriction(.0);
	plasticMat->setRestitution(.0);

	thumperBody = _mScene->createBody("thumper", new CapsuleShape(.8,.5,"material: Plastic"), pos,
		"static: yes, group: ThumperGroup");

	
	
	thumperBody->setName(_name);
	thumperBody->getNxActor()->setName(_name.c_str());

	thumper_node = SceneMgr->getRootSceneNode()->createChildSceneNode("thumper"
		+ Ogre::StringConverter::toString(thumperCounter));

	thumper_cover = SceneMgr->createEntity("thumper_cover"
		+ Ogre::StringConverter::toString(thumperCounter),
		"thumper_cover.mesh");
	SceneNode * cover_node = SceneMgr->getRootSceneNode()->createChildSceneNode("cover_node"
		+ Ogre::StringConverter::toString(thumperCounter));

	pos.y=0;

	cover_node->setPosition(pos.x, pos.y, pos.z);
	thumper_cover->setMaterialName("Gile[s]_thumper_cover");
	thumper_cover->setCastShadows(false);
	cover_node->attachObject(thumper_cover);

	Entity * thumper_shaft = SceneMgr->createEntity("thumper_plastic"
		+ Ogre::StringConverter::toString(thumperCounter),
		"thumper_plastic.mesh");
	SceneNode * shaft_node = cover_node->createChildSceneNode("shaft_node"
		+ Ogre::StringConverter::toString(thumperCounter));

	thumper_shaft->setMaterialName("Gile[s]_thumper_plastic");
	thumper_shaft->setCastShadows(false);
	shaft_node->attachObject(thumper_shaft);

	Entity * thumper_stroker = SceneMgr->createEntity("thumper_metal"
		+ Ogre::StringConverter::toString(thumperCounter),
		"thumper_metal.mesh");
	stroker_node = SceneMgr->getRootSceneNode()->createChildSceneNode("stroker_node"
		+ Ogre::StringConverter::toString(thumperCounter));

	thumper_stroker->setMaterialName("Gile[s]_thumper_metal");
	thumper_stroker->setCastShadows(false);
	stroker_y = stroker_node->getPosition().y;
	stroker_node->setPosition(pos.x, pos.y, pos.z);
	stroker_node->attachObject(thumper_stroker);

	thumperBody->getNxActor()->setGroup(20 + thumperCounter);

	mScene->createContactPair(thumperBody, ballBody,true, true, true);

	//mScene->getNxScene()->setActorPairFlags(*thumperBody->getNxActor(),*ballBody->getNxActor(),NX_NOTIFY_ON_START_TOUCH);
	//mScene->getNxScene()->setUserContactReport(this);
	


	++ thumperCounter;
}




bool thumper::onStartTouch ( Actor * a, Actor * b ){
	bool canContinue = false;

	if ( b->getName() == _name )
	{
		canContinue = true;
	}else{
		return false;
	}

	return propel(a);
}
bool thumper::onEndTouch ( Actor * a, Actor * b ){
	return false;
}
bool thumper::propel(NxOgre::Actor *a){

	NxContactStreamIterator i(mContactPair->stream);

	while ( i.goNextPair() )
	{
		while ( i.goNextPatch() )
		{
			while ( i.goNextPoint() )
			{
				const NxVec3 & location = i.getPoint();
				Vector3 ballPos = a->getGlobalPosition();
				NxVec3 drive = NxVec3(ballPos.x, ballPos.y, ballPos.z);
				drive -= location;
				setVectorLength(& drive, _strength);
				a->setLinearVelocity(Vector3(0, 0, 0));
				a->addForce( Vector3 (drive.x, drive.y, drive.z));

				int max = 20;
				Vector3 vec = a->getLinearVelocity();
				int Speed = vec.length();
				if (Speed > max)
				{
					vec.normalise();
					vec *= max;
					a->setLinearVelocity(vec);
				}

				stroker_node->setPosition(_stroked_pos.x, _stroked_pos.y,_stroked_pos.z);

				int randSound = (rand() % 2) + 1;

				if ( randSound == 1 )
				{
					SoundManager::getSoundManager()->Play_Sound(SoundManager::SND_THUMPER1);
				}
				else
				{
					SoundManager::getSoundManager()->Play_Sound(SoundManager::SND_THUMPER2);
				}

				ScoreManager::getScoreManager()->addToScore(SCORE_THUMPER_BUMPER,
					ScoreType::_THUMPER_BUMPER);
				return true;
			}
		}
	}
}
bool thumper::onTouch ( Actor * a, Actor * b ){

	bool canContinue = false;

	if ( b->getName() == _name )
	{
		canContinue = true;
	}else{
		return false;
	}

	return propel(a);
}




void thumper::update ( float _time )
    {
        const static float max_node_height=.32f;

        if ( stroker_node->getPosition().y < 0 )
        {
            Vector3 currPOS = stroker_node->getPosition();
            currPOS.y += (float)(8 * _time);
            if(currPOS.y>max_node_height)
                currPOS.y=max_node_height;
            stroker_node->setPosition(currPOS);
        }
    }

