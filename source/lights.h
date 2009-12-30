#ifndef _LIGHTS

#define _LIGHTS

#include "stdafx.h"

//==-----------------------------------------------------------------------------------
const enum SWITCHPOS
    {
        lightON,
        lightOFF
    };

//==-----------------------------------------------------------------------------------
class _light
    {
        public:

            Entity * entLight;
            SceneNode * nodeLight;
            int _value;
            _light(int id,Vector3 position,float angle, SceneManager * sm);
			
            void switchLight(SWITCHPOS switchPos);
    };

//==-----------------------------------------------------------------------------------
class lightManager
    {
        public:

            static _light * lights[20];
            lightManager(SceneManager * sm);
			~lightManager();
            static void switchLight ( int ID, SWITCHPOS switchTo );
            static void setValue ( int ID, int newVal );
            void resetLightValues();
    };

//==-----------------------------------------------------------------------------------



static lightManager * lm;

#endif
