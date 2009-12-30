#include "stdafx.h"

physicsVars::physicsVars ()
    {


		BallMass=.06;
		BallStaticFriction=.2;
		BallDynamicFriction=.2;
		BallRestitution=.5;
		/*
		BumperStrength[0]=9;
		BumperStrength[1]=8;
		BumperStrength[2]=9.5;
		BumperStrength[3]=8;
		*/
		BumperStrength[0]=8.5;
		BumperStrength[1]=8;
		BumperStrength[2]=9;
		BumperStrength[3]=7.5;

		FlipperSpeed=10;
		FlipperPower=9;

		Timed_plungerStrength=1;
		KickerRearStrength=2;
		KickerMidStrength=2;
		BumpStrength=6;
		TableSlope= 2;
		PlungerPower=-10;

		ThumperPos[0] = Vector3(-3.0,0,-3.7);
		ThumperPos[1] = Vector3(-1.0,0,-7.2);
		ThumperPos[2] = Vector3(3.2,0,-7.2);
		ThumperPos[3] = Vector3(1.7,0,-3.7);

		FlipperPos[0] = Vector3(-4.15,0,13.43);		
		FlipperPos[1] = Vector3(2.65,0,13.43);

		PinPos[0] = Vector3(-8.8,0,6.2);
		PinPos[1] = Vector3(7,0,6.6);
		PinPos[2] = Vector3(-50,0,0);
		
    }

physicsVars * physicsVars::getSingleton ()
    {
        static physicsVars * physvars;

        if ( physvars == NULL )
        {
            physvars = new physicsVars();
        }

        return physvars;
    }

void physicsVars::setVariable ( Ogre::String varname, Ogre::Real val )
    {

		String message = "LOAD:";
		message += varname;
		message += "=";
		message += StringConverter::toString(val);
		LogManager::getSingletonPtr()->logMessage(message);

        if ( varname == BALL_MAX_SPEED )
        {
            BallMaxSpeed = val;
        }

        if ( varname == BALL_MASS )
        {
            BallMass = val;
        }
        else if ( varname == BALL_STATICFRICTION )
        {
            BallStaticFriction = val;
        }
        else if ( varname == BALL_DYNAMICFRICTION )
        {
            BallDynamicFriction = val;
        }
        else if ( varname == BALL_RESTITUTION )
        {
            BallRestitution = val;
        }
        else if ( varname == BUMPER_STRENGTH_1 )
        {
            BumperStrength[0] = val;
        }
        else if ( varname == BUMPER_STRENGTH_2 )
        {
            BumperStrength[1] = val;
        }
        else if ( varname == BUMPER_STRENGTH_3 )
        {
            BumperStrength[2] = val;
        }
        else if ( varname == BUMPER_STRENGTH_4 )
        {
            BumperStrength[3] = val;
        }
        else if ( varname == FLIPPER_VELOCITY_TARGET )
        {
            FlipperSpeed = val;
        }
        else if ( varname == FLIPPER_MAX_FORCE )
        {
            FlipperPower = val;
        }
        else if ( varname == TIMED_KICKER_STRENGTH )
        {
            Timed_plungerStrength = val;
        }
        else if ( varname == REAR_KICKER_STRENGTH )
        {
            KickerRearStrength = val;
        }
        else if ( varname == MID_KICKER_STRENGTH )
        {
            KickerMidStrength = val;
        }
        else if ( varname == BUMP_POWER )
        {
            BumpStrength = val;
        }
        else if ( varname == PLUNGER_POWER )
        {
            PlungerPower = val;
        }
        else if ( varname == TABLE_SLOPE )
        {
            TableSlope = val;
        }
        else if ( varname == FLIPPERPOS_LEFT_X )
        {
            FlipperPos[0].x = val;
        }
        else if ( varname == FLIPPERPOS_LEFT_Z )
        {
            FlipperPos[0].z = val;
        }
        else if ( varname == FLIPPERPOS_RIGHT_X )
        {
            FlipperPos[1].x = val;
        }
        else if ( varname == FLIPPERPOS_RIGHT_Z )
        {
            FlipperPos[1].z = val;
        }
        else if ( varname == THUMPERPOS_1_X )
        {
            ThumperPos[0].x = val;
        }
        else if ( varname == THUMPERPOS_2_X )
        {
            ThumperPos[1].x = val;
        }
        else if ( varname == THUMPERPOS_3_X )
        {
            ThumperPos[2].x = val;
        }
        else if ( varname == THUMPERPOS_4_X )
        {
            ThumperPos[3].x = val;
        }
        else if ( varname == THUMPERPOS_1_Z )
        {
            ThumperPos[0].z = val;
        }
        else if ( varname == THUMPERPOS_2_Z )
        {
            ThumperPos[1].z = val;
        }
        else if ( varname == THUMPERPOS_3_Z )
        {
            ThumperPos[2].z = val;
        }
        else if ( varname == THUMPERPOS_4_Z )
        {
            ThumperPos[3].z = val;
        }
        else if ( varname == PIN_1_X )
        {
            PinPos[0].x = val;
        }
        else if ( varname == PIN_1_Z )
        {
            PinPos[0].z = val;
        }
        else if ( varname == PIN_2_X )
        {
            PinPos[1].x = val;
        }
        else if ( varname == PIN_2_Z )
        {
            PinPos[1].z = val;
        }
        else if ( varname == PIN_3_X )
        {
            PinPos[2].x = val;
        }
        else if ( varname == PIN_3_Z )
        {
            PinPos[2].z = val;
        }
    }
