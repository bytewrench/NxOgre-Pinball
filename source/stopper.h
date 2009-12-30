#ifndef _STOPPER
#define _STOPPER
#include "stdafx.h"
class stopper
{
public:
	stopper();
	~stopper();
	static stopper * getSingleton();
	void popout();
	void retract(bool playsound = true);
	void update(float timeDiff, SceneNode * node);
protected:
	Body * stopperBody;
	Vector3 pos;
	String _name;
	SceneNode * stopper_node;
	Entity * stopperEntElectric;
	bool active;
	DWORD _thisTime;
	DWORD _lastTime;
	DWORD _holdTime;
private:
};

#endif