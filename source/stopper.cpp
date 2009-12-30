#include "stdafx.h"

stopper::stopper(){

	pos = Vector3(0,0,0);
	
	_name = "stopper";
	Scene * _mScene = mWorld->getScenes()->getFirst();
	SceneManager * _scnmgr = _mScene->getSceneManager();

	stopperBody = _mScene->createBody("stopper", new ConvexShape("stopper.mesh"), pos,	"static: yes");
	stopperBody->setName(_name);
	stopperBody->getNxActor()->setName(_name.c_str());
	
	
	Entity * stopperEnt = _scnmgr->createEntity("stopper_ent","stopper.mesh");
	stopper_node = _scnmgr->getRootSceneNode()->createChildSceneNode("stopper_node");

	stopperEnt->setMaterialName("stopper");
	stopper_node->attachObject(stopperEnt);

	stopperEntElectric = _scnmgr->createEntity("stopper_ent_electric","stopper_electric.mesh");
	stopperEntElectric->setMaterialName("stopper_electric");
	
	stopper_node->attachObject(stopperEntElectric);
	stopperEntElectric->setVisible(false);


	_thisTime = 0;
	_lastTime = 0;
	_holdTime = 1000;
	retract(false);
}
stopper::~stopper(){
	//delete stopper_node;
	//delete stopperBody;
	
}
stopper * stopper::getSingleton(){
	static stopper * _stopper;
	if (_stopper == 0)
	{
		_stopper = new stopper();
	}
	return _stopper;
}
void stopper::popout(){
	stopperEntElectric->setVisible(true);
	stopperBody->setGlobalPosition(Vector3(pos.x,0, pos.z));
	stopper_node->setPosition(pos.x,0, pos.z);
	_thisTime = timeGetTime();
	_lastTime = _thisTime;
	SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_STOPPER);
	active = true;
}
void stopper::retract(bool playsound){
	stopperEntElectric->setVisible(false);
	stopperBody->setGlobalPosition(Vector3(pos.x,-1, pos.z));
	stopper_node->setPosition(pos.x,-1, pos.z);
	_thisTime = 0;
	if (playsound)
	{
		SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_SWITCH);
	}
	
	active = false;
}
void stopper::update(float timeDiff, SceneNode * node){
	
	if (!active)
	{
		return;
	}
	_thisTime = timeGetTime();
	Vector3 scale = node->getScale();
	scale.x += 1*timeDiff;
	scale.z += 1*timeDiff;

	node->setScale(scale.x,1,scale.z);
	
	node->setVisible(true);
	if ( (_thisTime - _lastTime) > _holdTime )
	{
		node->setScale(2,1,2);
		node->setVisible(false);
		retract();
		
	}

}