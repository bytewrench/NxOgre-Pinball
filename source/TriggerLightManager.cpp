#include "stdafx.h"


void triggerLight::create ( Ogre::String name, Ogre::String MeshName, Ogre::Vector3 pos, Body * ballBody,
			 SceneManager * sm, float onTime )
{

	_thisTime = timeGetTime();
	_lastTime = 0;
	static int id;

	_name = name + Ogre::StringConverter::toString(id);


	ent = sm->createEntity("trigLight." + Ogre::StringConverter::toString(id), MeshName);

	ent->setMaterialName("triggerlightOFF");

	SceneNode * nodeTrigLight;
	nodeTrigLight = sm->getRootSceneNode()->createChildSceneNode("triggerlightNode."
		+ Ogre::StringConverter::toString(id));

	nodeTrigLight->setPosition(pos.x, .1, pos.z);
	nodeTrigLight->attachObject(ent);
	nodeTrigLight->setVisible(true);
	_onTime = onTime;
	isOn = false;

	++ id;
}

void triggerLight::switchOn(){

 	if (isOn)
	{
		return;
	}

	_currtime = timeGetTime();

	if ( (_currtime - _lastTime)/1000 < .7 )
	{
		_lastTime = timeGetTime();
		return;
	}
	_lastTime = timeGetTime();
	ent->setMaterialName("triggerlightON");
	isOn = true;
	SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_TRIGGERLIGHT);
	ScoreManager::getScoreManager()->addToScore(SCORE_TRIGGERLIGHT,ScoreType::_TRIGGERLIGHT);
	_initTime = _lastTime;
	return;
}

void triggerLight::update ()
{
	if ( ! isOn )
	{
		return;
	}

	float now = timeGetTime();
	float diff = (now - _initTime) / 1000;

	if ( diff > _onTime )
	{
		ent->setMaterialName("triggerlightOFF");
		isOn = false;
	}
}




triggerLightManager::~triggerLightManager(){
	for ( int i = 0; i < MAX_TRIGGER_LIGHTS; i ++ )
	{
		delete _trigLight[i];
	}
}
//==------------------------------------------------------------------------------------
triggerLight::triggerLight () {
	
}

//==------------------------------------------------------------------------------------
triggerLightManager::triggerLightManager ()
{
	for ( int i = 0; i < MAX_TRIGGER_LIGHTS; i ++ )
	{
		_trigLight[i] = new triggerLight();
	}
}

//==------------------------------------------------------------------------------------
triggerLightManager * triggerLightManager::getSingleton ()
{
	static triggerLightManager * trigMan;

	if ( trigMan == NULL )
	{
		trigMan = new triggerLightManager();
	}

	return trigMan;
}



//==------------------------------------------------------------------------------------
