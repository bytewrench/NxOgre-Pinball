#ifndef _TRIGGERLIGHTMAN
#define _TRIGGERLIGHTMAN

#include "stdafx.h"


class triggerLight

{
public:

	triggerLight();
	void create ( Ogre::String name, Ogre::String MeshName, Ogre::Vector3 pos, Body * ballBody,
		SceneManager * sm, float onTime );
	void switchOn();
	void update ();

protected:
private:

	Entity * ent;
	float _thisTime;
	float _lastTime;
	float _currtime;
	float _onTime;
	float _initTime;
	String _name;
	bool isOn;
};

//==------------------------------------------------------------------------------------

#define MAX_TRIGGER_LIGHTS 4

class triggerLightManager
{
public:

	triggerLightManager();
	~triggerLightManager();
	static triggerLightManager * getSingleton ();

	triggerLight * _trigLight[MAX_TRIGGER_LIGHTS];

protected:
private:

	static SceneManager * _sm;
	static Scene * _scn;
};



#endif