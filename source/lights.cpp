#include "stdafx.h"
//==-----------------------------------------------------------------------------------
_light::_light ( int id, Vector3 position, float angle, SceneManager * sm )
    {
        entLight = sm->createEntity("lightEnt." + Ogre::StringConverter::toString(id), "light.mesh");
        entLight->setMaterialName("lights/lightOn");

        nodeLight = sm->getRootSceneNode()->createChildSceneNode("lightNode."
                                                                     + Ogre::StringConverter::toString(id));

        nodeLight->setPosition(position.x, position.y, position.z);
        nodeLight->pitch(Radian(angle));
        nodeLight->attachObject(entLight);
    }

//==-----------------------------------------------------------------------------------
void _light::switchLight ( SWITCHPOS switchPos )
    {
        switch ( switchPos )
            {
                case lightON:
                    entLight->setMaterialName("lights/lightOn");
                    break;

                case lightOFF:
                    entLight->setMaterialName("lights/lightOff");
                    break;
            }
    }

//==-----------------------------------------------------------------------------------
lightManager::~lightManager(){
	for ( int i = 1; i < 16; i ++ ){
		delete lights[i];
	}
}
//==-----------------------------------------------------------------------------------
lightManager::lightManager ( SceneManager * sm )
    {
        for ( int i = 1; i < 16; i ++ )
            {
                if ( i == 5 || i == 6 || i == 7 || i == 8 )
                {
                    lights[i] = new _light(i, LIGHT_POS[i], .314f, sm);
                }
                else
                {
                    lights[i] = new _light(i, LIGHT_POS[i], 0.0f, sm);
                }
            }
    }

//==-----------------------------------------------------------------------------------
void lightManager::setValue ( int ID, int newVal )
{
    lights[ID]->_value = newVal;
}

//==-----------------------------------------------------------------------------------
void lightManager::switchLight ( int ID, SWITCHPOS switchTo )
{
    lights[ID]->switchLight(switchTo);
}

//==-----------------------------------------------------------------------------------
void lightManager::resetLightValues ()
    {
        for ( int i = 1; i < 16; i ++ )
            {
                int number = NUMBERSETS[QuadManager::CurrBingoLetter - 1][i - 1];
                this->setValue(i, number);

                if ( hitNumberManager::getHitNumMan()->isHit(number) )
                {
                    switchLight(i, SWITCHPOS::lightOFF);
                }
                else
                {
                    switchLight(i, SWITCHPOS::lightON);
                }
            }
    }

//==-----------------------------------------------------------------------------------



_light * lightManager::lights[20];
