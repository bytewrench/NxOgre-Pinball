#include "stdafx.h"




//NxOgre::MyScene * mScene;

void setVectorLength ( NxVec3 * vector, const Ogre::Real newLength )
{
	vector->normalize();
	( * vector) *= newLength;
}
World* getWorld(){return mWorld;}



//////////////////////////////////////////////////////////////////////////
