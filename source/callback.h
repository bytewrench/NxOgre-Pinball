#include "stdafx.h"

class myCallback : public GroupCallback::InheritedCallback 
{ 

public: 
	myCallback();
	~myCallback();
	void onTouch(Actor* a , Actor* b);
	void onStartTouch(Actor* a, Actor* b);
	void onEndTouch(Actor* a, Actor* b);


}; 


