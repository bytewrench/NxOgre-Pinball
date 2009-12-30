#ifndef _BUBBLENOTIFY
#define _BUBBLENOTIFY

#include "stdafx.h"

class BubbleNotify
{
public:
	BubbleNotify();
	~BubbleNotify();
	void init(Vector2 pos, Vector2 size, String matName, String name);
	void show();
	void hide();
	void update();
	void setMessage(Ogre::String text,Real height, float displayTime);
	static BubbleNotify * getSingleton();
protected:
private:
	static OgreText * _text;
	Overlay * _overalay;
	Vector2 _pos;
	float _displayTime;
	float _thisTime;
	float _initTime;
	bool _active;
	
};


#endif