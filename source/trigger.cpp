#include "stdafx.h"

#include "trigger.h"
#include "stopper.h"


NxActor *  myTrigger::CreateTriggerBox(NxVec3 pos, NxVec3 size, const char* name, bool capsule){

	_name = name;
	NxActorDesc actorDesc;
	actorDesc.name = name;

	if (capsule)
	{
		NxCapsuleShapeDesc capsuleShape;
		capsuleShape.height=size.y;
		capsuleShape.radius = size.x;
		capsuleShape.shapeFlags|= NX_TRIGGER_ENABLE;
		actorDesc.shapes.pushBack(&capsuleShape);

	}else{
		NxBoxShapeDesc boxShape;
		boxShape.dimensions.y = size.y;
		boxShape.dimensions.x = size.x;
		boxShape.dimensions.z = .25;
		boxShape.shapeFlags |= NX_TRIGGER_ENABLE;
		actorDesc.shapes.pushBack(&boxShape);
	}


	actorDesc.globalPose.t = pos + NxVec3(0, size.y, 0);
	mScene->getNxScene()->setUserTriggerReport(this);
	return mScene->getNxScene()->createActor(actorDesc);
}

void myTrigger::onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status){

	if ( ! triggerShape.getActor().userData && ! otherShape.getActor().userData )
	{
		return;
	}
	
	if (status & NX_TRIGGER_ON_ENTER)
	{
		// A body just entered the trigger area
		NxActor* triggerActor = &triggerShape.getActor();
		String name = triggerActor->getName();
		int thisTarget = StringConverter::parseInt(name);
		creditTarget(thisTarget);
	}
	
}




void myTrigger::creditTarget (int target_hit){


	if ( target_hit == 0 )
	{
		//----------------------------------------------------LETTER SWITCHER
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
		//SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_STOPPER);
		ScoreManager::getScoreManager()->addToScore(SCORE_LETTER_SWITCHER, ScoreType::_LETTER_SWITCHER);
		stopper::getSingleton()->popout();
	}
	else if (target_hit > 0 && target_hit < 16)
	{

		//---------------------------------------------------------MAIN TARGETS
		lm->lights[target_hit]->switchLight(SWITCHPOS::lightOFF);
		bool wasHit = hitNumberManager::getHitNumMan()->isHit(lm->lights[target_hit]->_value);

		if ( ! wasHit )
		{
			if ( BingoManager::getSingleton()->resetCard(lm->lights[target_hit]->_value) )
			{
			}
			hitNumberManager::getHitNumMan()->recordHit(lm->lights[target_hit]->_value);
			ScoreManager::getScoreManager()->addToScore(SCORE_TARGET_DIM, ScoreType::_TARGET_DIM);
			SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_TARGET_DIM);
			return;
		}else{
			ScoreManager::getScoreManager()->addToScore(SCORE_TARGET_LIT,ScoreType::_TARGET_LIT);
			SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_TARGET_LIT);
		}
	}

	if ((target_hit > 15) && (target_hit < 20))
	{
		triggerLightManager::getSingleton()->_trigLight[(target_hit-16)]->switchOn();
		return;
	}
	if ( (target_hit == 20) || (target_hit == 21) )
	{
		SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_DRAIN);
		ScoreManager::getScoreManager()->addToScore(SCORE_DRAIN,ScoreType::_DRAIN);
		return;
	}

	if ( (target_hit == 30) || (target_hit == 31) )
	{
		SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_SWITCH);
		ScoreManager::getScoreManager()->addToScore(SCORE_RAIL,ScoreType::_RAIL);
		return;
	}

}


_TargetTrigger::_TargetTrigger(String name, ShapeBlueprint *shape, const NxOgre::Pose& pose, ActorParams params){
	//void _createActor() 
	_thisTrigger = new Trigger(name,mScene,shape,pose,params);
	_thisTrigger->setCallback(this);
}

void _TargetTrigger::onEnter(Trigger* _trigger, Character* _char){
	String myName;
	myName = _char->getName();

}
void _TargetTrigger::onEnter(Trigger* _trigger, Actor* _actor){
	
	String myName;
	myName = _actor->getName();
}
/*
void _TargetTrigger::onTrigger(NxShape & ts, NxShape & as, NxTriggerFlag tf ){
	if ( ! ts.getActor().userData && ! as.getActor().userData )
	{
		return;
	}
	int thisTarget = StringConverter::parseInt(ts.getName());
	creditTarget(thisTarget);
}
*/
void _TargetTrigger::creditTarget ( int target_hit )
{
	if ( target_hit > 4 && target_hit < 9 )
	{
		lm->lights[target_hit]->switchLight(SWITCHPOS::lightOFF);
		bool wasHit = hitNumberManager::getHitNumMan()->isHit(lm->lights[target_hit]->_value);

		if ( ! wasHit )
		{
			if ( BingoManager::getSingleton()->resetCard(lm->lights[target_hit]->_value) )
			{
				OutputDebugStr("New target Hit\n");
			}

			hitNumberManager::getHitNumMan()->recordHit(lm->lights[target_hit]->_value);
			ScoreManager::getScoreManager()->addToScore(SCORE_TARGET_DIM,ScoreType::_TARGET_DIM);
			SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_TARGET_DIM);
		}

		else
		{
			ScoreManager::getScoreManager()->addToScore(SCORE_TARGET_LIT,ScoreType::_TARGET_LIT);
			SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_TARGET_LIT);
		}

		return;
	}

	if ( (target_hit == 20) || (target_hit == 21) )
	{
		SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_DRAIN);
		ScoreManager::getScoreManager()->addToScore(SCORE_DRAIN,ScoreType::_DRAIN);
		return;
	}

	if ( (target_hit == 30) || (target_hit == 31) )
	{
		SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_SWITCH);
		ScoreManager::getScoreManager()->addToScore(SCORE_RAIL,ScoreType::_RAIL);
		return;
	}

	if ((target_hit == 0) || (target_hit == 1) || (target_hit == 2) || (target_hit == 3))
	{
		triggerLightManager::getSingleton()->_trigLight[target_hit]->switchOn();
	}

}