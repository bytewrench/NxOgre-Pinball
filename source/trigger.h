#ifndef _TRIGGER

#define _TRIGGER

#include "stdafx.h"


class myTrigger : public NxUserTriggerReport
{
public:
	String _name;
	NxActor * CreateTriggerBox(NxVec3 pos, NxVec3 size,const char* name, bool capsule = true);
	void creditTarget (int target_hit);
	virtual void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);
protected:
private:
};


class _TargetTrigger: public NxOgre::TriggerCallback::InheritedCallback /*customTriggerReporter*/
{
public:

	Trigger * _thisTrigger;
	void onEnter(Trigger*, Actor* a);
	void onEnter(Trigger*, Character*);
	void creditTarget ( int target_hit );

	_TargetTrigger(String name,ShapeBlueprint *shape, const NxOgre::Pose& pose, ActorParams params);
	~_TargetTrigger(){};
protected:
private:
};

#endif
