#include "stdafx.h"

void flipper::wake ()
{
	this->FlipperBody->wakeUp();
}

void flipper::sleep ()
{
	this->FlipperBody->putToSleep();
}
void flipper::updateMe ( bool isActive )
{

	if ( isActive )
	{


		if ( ! flipped )
		{
			SoundManager::getSoundManager()->Play_Sound(SoundManager::SND_FLIPPER);
		}



		NxMotorDesc motorDesc;
		motorDesc.maxForce = _maxforce;

		motorDesc.freeSpin = false;

		if ( _orient == 0 )
		{
			motorDesc.velTarget = _veltarget;
		}
		else
		{
			motorDesc.velTarget = - _veltarget;
		}


		flipperJoint->setMotor(motorDesc);
		
		flipped = true;
	}

	else
	{
		NxMotorDesc motorDesc;
		motorDesc.maxForce = _maxforce;
		motorDesc.freeSpin = false;


		if ( _orient == 0 )
		{
			motorDesc.velTarget = - returnVelTarget;
		}
		else
		{
			motorDesc.velTarget = returnVelTarget;
		}


		flipperJoint->setMotor(motorDesc);
		flipped = false;
	}
}

NxRevoluteJoint * flipper::CreateRevoluteJoint ( NxActor * a1, NxVec3 globalAnchor, NxVec3 globalAxis, float velTarget, float maxForce )
{
	NxRevoluteJointDesc revDesc;

	
	revDesc.actor[1] = a1;
	
	revDesc.setGlobalAnchor(globalAnchor);
	revDesc.setGlobalAxis(globalAxis);

	revDesc.flags |= NX_RJF_LIMIT_ENABLED;
	_maxforce = maxForce;
	_veltarget = velTarget;

	if ( _orient == 0 )
	{
		revDesc.limit.high.value = 0.25 * NxPi;
		revDesc.limit.low.value = - 0.15 * NxPi;
	}

	else
	{
		revDesc.limit.high.value = 0.15 * NxPi;
		revDesc.limit.low.value = - 0.25 * NxPi;
	}

	revDesc.limit.high.restitution = 0;
	revDesc.limit.low.restitution = 0;

	revDesc.flags |= NX_RJF_MOTOR_ENABLED;



	if ( _orient == 0 )
	{
		_motorDesc.velTarget = _veltarget;
	}
	else
	{
		_motorDesc.velTarget = - _veltarget;
	}

	_motorDesc.maxForce = _maxforce;
	_motorDesc.freeSpin = true;
	revDesc.motor = _motorDesc;
	flipperActive = false;

	return (NxRevoluteJoint * ) mScene->getNxScene()->createJoint(revDesc);

}

flipper::flipper ( Vector3 pos, int orient, Ogre::SceneManager * SceneMgr, float maxForce, float velTarget )
{
	returnVelTarget = velTarget;
	_orient = orient;

	Scene * _mScene = mWorld->getScenes()->getFirst();
	
	/*We got sloppy flippers, maybe cause they were skinny and were too high on the table*/


	NxOgre::Material * RubberMat = mScene->createMaterial("Rubber");
	RubberMat->setRestitution(.8);
	RubberMat->setStaticFriction(1);
	RubberMat->setDynamicFriction(1);
	Entity * flipper_main;

	if ( _orient == 0 )
	{

		FlipperBody = mScene->createBody("flipper_band_left.mesh",
			new ConvexShape("flipper_band_left.mesh","Group:FlipperGroup, Material: Rubber"), 
			Vector3(pos.x, pos.y, pos.z),
			""
			);

		
		
		flipper_main = SceneMgr->createEntity("flipper_main_left",
			"flipper_vis_left.mesh");
		
		//flipper_main->setCastShadows(false);
		//flipper_main->setMaterialName("flipper_main");
		//FlipperBody->getNode()->attachObject(flipper_main);
		FlipperBody->getNxActor()->setName("FlipperLeft");

	}

	else
	{
		FlipperBody = mScene->createBody("flipper_band_right.mesh",
			new ConvexShape("flipper_band_right.mesh","Group:FlipperGroup, material: FlipperMat"),
			Vector3(pos.x, pos.y, pos.z),
			""
			);
		
		flipper_main = SceneMgr->createEntity("flipper_main_right",
			"flipper_vis_right.mesh");
		//flipper_main->setCastShadows(false);
		//flipper_main->setMaterialName("flipper_band");
		//FlipperBody->getNode()->attachObject(flipper_main);
		FlipperBody->getNxActor()->setName("FlipperRight");
		
	}

	
	flipper_main->setCastShadows(false);
	flipper_main->setMaterialName("flipper_main");
	FlipperBody->getNode()->attachObject(flipper_main);
	FlipperBody->getEntity()->setMaterialName("flipper_band");


	NxActor * a;
	a = FlipperBody->getNxActor();


	flipperJoint = CreateRevoluteJoint(a, NxVec3(pos.x, pos.y+.5, pos.z),
		NxVec3(0, - 1, 0),velTarget,maxForce); // y=.25

	FlipperBody->setAngularDamping(0.0f);
	FlipperBody->setLinearDamping(0.0f);

	//FlipperBody->updateMassFromShapeFromDensity(7);
	//FlipperBody->setCMassOffsetLocalPosition(pos);
	
	FlipperBody->setMass(.04);

	FlipperBody->getNxActor()->setMaxAngularVelocity(10000);
	FlipperBody->getNxActor()->setCCDMotionThreshold(50);
	flipperActive = false;
	flipTimer = 0;
	sleep();
}

