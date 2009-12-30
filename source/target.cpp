#include "stdafx.h"


//-----------------------------------------------------------------------------targetContactController
targetContactController::targetContactController(TARGETNAME bankname){
	_bankname = bankname;
}


bool targetContactController::onStartTouch ( Actor * a, Actor * b ){
	return false;
}
bool targetContactController::onEndTouch ( Actor * a, Actor * b ){
	return false;
}
bool targetContactController::onTouch ( Actor * a, Actor * b ){



	NxContactStreamIterator i(mContactPair->stream);

	while ( i.goNextPair() )
	{
		while ( i.goNextPatch() )
		{
			while ( i.goNextPoint() )
			{
				const NxVec3 & location = i.getPoint();
				int target_hit = getHitTarget(location);

				if ( target_hit > 0 && target_hit < 16 )
				{
					lm->lights[target_hit]->switchLight(SWITCHPOS::lightOFF);
					bool wasHit =
						hitNumberManager::getHitNumMan()->isHit(lm->lights[target_hit]->_value);

					if ( ! wasHit )
					{
						if ( BingoManager::getSingleton()->resetCard((lm->lights[target_hit]->_value) ))
						{
							OutputDebugStr("New target Hit\n");
						}

						hitNumberManager::getHitNumMan()->recordHit(lm->lights[target_hit]->_value);
						ScoreManager::getScoreManager()->addToScore(SCORE_TARGET_DIM, ScoreType::_TARGET_DIM);
						SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_TARGET_DIM);
					}

					else
					{
						ScoreManager::getScoreManager()->addToScore(SCORE_TARGET_LIT, ScoreType::_TARGET_LIT);
						SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_TARGET_LIT);
					}
				}

				else if ( target_hit == 0 )
				{
					if ( QuadManager::CurrBingoLetter >= 5 )
					{
						QuadManager::CurrBingoLetter = 1;
					}
					else
					{
						++ QuadManager::CurrBingoLetter;
					}

					QuadManager::SwitchNumberSet(QuadManager::CurrBingoLetter);
					lm->resetLightValues();
					SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_SWITCH_LETTER);
					ScoreManager::getScoreManager()->addToScore(SCORE_LETTER_SWITCHER, ScoreType::_LETTER_SWITCHER);
				}

				else if ( target_hit == 20 )
				{
					SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_DRAIN);
					ScoreManager::getScoreManager()->addToScore(SCORE_DRAIN,ScoreType::_DRAIN);
				}

				else if ( target_hit == 30 )
				{
					SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_SWITCH);
					ScoreManager::getScoreManager()->addToScore(SCORE_RAIL,ScoreType::_RAIL);
				}

				return false;
			}
		}
	}

	return true;
}




int targetContactController::getHitTarget ( NxVec3 pos )
{
	char posHit[128];

	switch ( _bankname )
	{
	case TARGETBANK_1:
		if ( (pos.z >= - .99f) && (pos.z <= .3f) )
		{
			return 1;
		}
		else if ( (pos.z >= - 1.98f) && (pos.z <= - 1.0f) )
		{
			return 2;
		}
		else
		{
			sprintf(posHit, "MIS BANK 1 %f\n", pos.z);
			OutputDebugStr(posHit);
		}

		return - 1;
		break;

	case TARGETBANK_2:
		if ( (pos.z >= - 10.48f) && (pos.z <= - 8.97f) )
		{
			return 3;
		}
		else if ( (pos.z >= - 11.59f) && (pos.z <= - 10.49f) )
		{
			return 4;
		}
		else
		{
			sprintf(posHit, "MIS BANK 2 %f\n", pos.z);
			OutputDebugStr(posHit);
		}

		return - 2;
		break;

	case TARGETBANK_3:
		if ( (pos.z >= - 11) && (pos.z <= - 10.18f) )
		{
			return 9;
		}
		else if ( (pos.z >= - 10.17f) && (pos.z <= - 8.74f) )
		{
			return 10;
		}
		else if ( (pos.z >= - 8.73f) && (pos.z <= - 7.1f) )
		{
			return 11;
		}
		else if ( (pos.z >= - 7.0f) && (pos.z <= - 5.7f) )
		{
			return 12;
		}
		else
		{
			sprintf(posHit, "MIS BANK 3 %f\n", pos.z);
			OutputDebugStr(posHit);
		}

		return - 3;
		break;

	case TARGETBANK_4:
		if ( (pos.z >= - 2.16f) && (pos.z <= - .62f) )
		{
			return 13;
		}
		else if ( (pos.z >= - .61f) && (pos.z <= .93f) )
		{
			return 14;
		}
		else if ( (pos.z >= .92f) && (pos.z <= 2.3f) )
		{
			return 15;
		}
		else
		{
			sprintf(posHit, "MIS BANK 4 %f\n", pos.z);
			OutputDebugStr(posHit);
		}

		return - 4;
		break;
	case TARGETBANK_CENTER:
		return 0;
		break;


	default:
		sprintf(posHit, "MIS EVERYTHING! %f-%f\n", pos.x, pos.z);
		OutputDebugStr(posHit);
		return - 50;
		break;
	}

	return - 60;
}



//-----------------------------------------------------------------------------target
int target::getHitTarget ( NxVec3 pos )
{
	char posHit[128];

	switch ( _bankname )
	{
	case TARGETBANK_1:
		if ( (pos.z >= - .99f) && (pos.z <= .3f) )
		{
			return 1;
		}
		else if ( (pos.z >= - 1.98f) && (pos.z <= - 1.0f) )
		{
			return 2;
		}
		else
		{
			sprintf(posHit, "MIS BANK 1 %f\n", pos.z);
			OutputDebugStr(posHit);
		}

		return - 1;
		break;

	case TARGETBANK_2:
		if ( (pos.z >= - 10.48f) && (pos.z <= - 8.97f) )
		{
			return 3;
		}
		else if ( (pos.z >= - 11.59f) && (pos.z <= - 10.49f) )
		{
			return 4;
		}
		else
		{
			sprintf(posHit, "MIS BANK 2 %f\n", pos.z);
			OutputDebugStr(posHit);
		}

		return - 2;
		break;

	case TARGETBANK_3:
		if ( (pos.z >= - 11) && (pos.z <= - 10.18f) )
		{
			return 9;
		}
		else if ( (pos.z >= - 10.17f) && (pos.z <= - 8.74f) )
		{
			return 10;
		}
		else if ( (pos.z >= - 8.73f) && (pos.z <= - 7.1f) )
		{
			return 11;
		}
		else if ( (pos.z >= - 7.0f) && (pos.z <= - 5.7f) )
		{
			return 12;
		}
		else
		{
			sprintf(posHit, "MIS BANK 3 %f\n", pos.z);
			OutputDebugStr(posHit);
		}

		return - 3;
		break;

	case TARGETBANK_4:
		if ( (pos.z >= - 2.16f) && (pos.z <= - .62f) )
		{
			return 13;
		}
		else if ( (pos.z >= - .61f) && (pos.z <= .93f) )
		{
			return 14;
		}
		else if ( (pos.z >= .92f) && (pos.z <= 2.3f) )
		{
			return 15;
		}
		else
		{
			sprintf(posHit, "MIS BANK 4 %f\n", pos.z);
			OutputDebugStr(posHit);
		}

		return - 4;
		break;
	case TARGETBANK_CENTER:
		return 0;
		break;


	default:
		sprintf(posHit, "MIS EVERYTHING! %f-%f\n", pos.x, pos.z);
		OutputDebugStr(posHit);
		return - 50;
		break;
	}

	return - 60;
}




bool target::onStartTouch ( Actor * a, Actor * b ){
	return false;
}
bool target::onEndTouch ( Actor * a, Actor * b ){
	return false;
}
bool target::onTouch ( Actor * a, Actor * b ){

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
			while ( i.goNextPoint() )
			{
				const NxVec3 & location = i.getPoint();
				int target_hit = getHitTarget(location);

				if ( target_hit > 0 && target_hit < 16 )
				{
					lm->lights[target_hit]->switchLight(SWITCHPOS::lightOFF);
					bool wasHit =
						hitNumberManager::getHitNumMan()->isHit(lm->lights[target_hit]->_value);

					if ( ! wasHit )
					{
						if ( BingoManager::getSingleton()->resetCard(lm->lights[target_hit]->_value) )
						{
							OutputDebugStr("New target Hit\n");
						}

						hitNumberManager::getHitNumMan()->recordHit(lm->lights[target_hit]->_value);
						ScoreManager::getScoreManager()->addToScore(SCORE_TARGET_DIM, ScoreType::_TARGET_DIM);
						SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_TARGET_DIM);
					}

					else
					{
						ScoreManager::getScoreManager()->addToScore(SCORE_TARGET_LIT, ScoreType::_TARGET_LIT);
						SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_TARGET_LIT);
					}
				}

				else if ( target_hit == 0 )
				{
					if ( QuadManager::CurrBingoLetter >= 5 )
					{
						QuadManager::CurrBingoLetter = 1;
					}
					else
					{
						++ QuadManager::CurrBingoLetter;
					}

					QuadManager::SwitchNumberSet(QuadManager::CurrBingoLetter);
					lm->resetLightValues();
					SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_SWITCH_LETTER);
					ScoreManager::getScoreManager()->addToScore(SCORE_LETTER_SWITCHER, ScoreType::_LETTER_SWITCHER);
				}

				else if ( target_hit == 20 )
				{
					SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_DRAIN);
					ScoreManager::getScoreManager()->addToScore(SCORE_DRAIN,ScoreType::_DRAIN);
				}

				else if ( target_hit == 30 )
				{
					SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_SWITCH);
					ScoreManager::getScoreManager()->addToScore(SCORE_RAIL,ScoreType::_RAIL);
				}

				return false;
			}
		}
	}

	return true;
}



target::target ( Ogre::String meshName,   Ogre::SceneManager * SceneMgr, Body * ballBody,
		Ogre::String targetName, TARGETNAME BankName )
{
	_ballbody = ballBody;
	_thisTime = timeGetTime();
	_lastTime = 0;
	_name = targetName;
	_bankname = BankName;



	//material * targetMaterial = mScene->createMaterial("targetMaterial", 0, 0, 1);



	ActorGroup * mTargetGroup;
	Scene * _mScene = mWorld->getScenes()->getFirst();


	NxOgre::Material * targetMat = _mScene->createMaterial("TargetMat");

	targetMat->setRestitution(1);
	targetMat->setStaticFriction(.5);
	targetMat->setStaticFrictionV(.5);

	mTargetGroup = _mScene->createActorGroup("TargetGroup");

	_targetBody = _mScene->createBody("target",
		new ConvexShape(meshName),
		Vector3(0, 0, 0),
		"static:yes, group:TargetGroup, material: TargetMat"
		);

	_targetBody->getNxActor()->setGroup(15);
	_targetBody->getNxActor()->setName("Target");

	mScene->createContactPair(_targetBody, ballBody, false, true, false);
	
}



