#ifndef _PHYSICSVARS
#define _PHYSICSVARS

#include "stdafx.h"

#define BALL_MAX_SPEED "BALL_MAX_SPEED"
#define BALL_MASS  "BALL_MASS"
#define BUMPER_STRENGTH_1  "BUMPER_STRENGTH_1"
#define BUMPER_STRENGTH_2   "BUMPER_STRENGTH_2"
#define BUMPER_STRENGTH_3   "BUMPER_STRENGTH_3"
#define BUMPER_STRENGTH_4   "BUMPER_STRENGTH_4"
#define FLIPPER_VELOCITY_TARGET  "FLIPPER_VELOCITY_TARGET"
#define FLIPPER_MAX_FORCE  "FLIPPER_MAX_FORCE"
#define TIMED_KICKER_STRENGTH  "TIMED_KICKER_STRENGTH"
#define REAR_KICKER_STRENGTH  "REAR_KICKER_STRENGTH"
#define MID_KICKER_STRENGTH   "MID_KICKER_STRENGTH"
#define BUMP_POWER   "BUMP_POWER"
#define PLUNGER_POWER "PLUNGER_POWER"
#define TABLE_SLOPE "TABLE_SLOPE"
#define BALL_STATICFRICTION "BALL_STATICFRICTION"
#define BALL_DYNAMICFRICTION "BALL_DYNAMICFRICTION"
#define BALL_RESTITUTION "BALL_RESTITUTION"

#define THUMPERPOS_1_X	"THUMPERPOS_1_X"
#define THUMPERPOS_2_X	"THUMPERPOS_2_X"
#define THUMPERPOS_3_X	"THUMPERPOS_3_X"
#define THUMPERPOS_4_X	"THUMPERPOS_4_X"
#define FLIPPERPOS_LEFT_X "FLIPPERPOS_LEFT_X"
#define FLIPPERPOS_RIGHT_X "FLIPPERPOS_RIGHT_X"

#define THUMPERPOS_1_Z	"THUMPERPOS_1_Z"
#define THUMPERPOS_2_Z	"THUMPERPOS_2_Z"
#define THUMPERPOS_3_Z	"THUMPERPOS_3_Z"
#define THUMPERPOS_4_Z	"THUMPERPOS_4_Z"
#define FLIPPERPOS_LEFT_Z "FLIPPERPOS_LEFT_Z"
#define FLIPPERPOS_RIGHT_Z "FLIPPERPOS_RIGHT_Z"

#define PIN_1_X "PIN_1_X"
#define PIN_1_Z "PIN_1_Z"

#define PIN_2_X "PIN_2_X"
#define PIN_2_Z "PIN_2_Z"

#define PIN_3_X "PIN_3_X"
#define PIN_3_Z "PIN_3_Z"




class physicsVars
{
public:
	physicsVars();
	static physicsVars * getSingleton();
	void setVariable(String varname, Real val);
	Real BumperStrength[4];
	Real KickerRearStrength;
	Real KickerMidStrength;
	Real Timed_plungerStrength;
	Real MinPlungerPower;
	Real MaxPlungerPower;
	Real TableSlope;
	Real Restitution;
	Real FlipperSpeed;
	Real FlipperPower;
	Real FlipperMass;
	Real BallMass;
	Real BallStaticFriction;
	Real BallDynamicFriction;
	Real BallRestitution;
	Real BumpStrength;
	Real PlungerPower;
	Real BallMaxSpeed;
	Vector3 ThumperPos[4];
	Vector3 FlipperPos[2];
	Vector3 PinPos[3];
	

protected:
private:

};


#endif