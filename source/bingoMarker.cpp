#include "stdafx.h"

BingoMarker::BingoMarker ( Vector3 location, float angle/*, SceneManager * sm */)
    {
        static int markerCounter;

		Scene * _mScene = mWorld->getScenes()->getFirst();
		
        entMarker = _mScene->getSceneManager()->createEntity("bingo_marker" + Ogre::StringConverter::toString(++ markerCounter),
                                     "bingo_marker.mesh");
        nodeMarker = _mScene->getSceneManager()->getRootSceneNode()->createChildSceneNode("bingo_marker_Node"
                                                                      + Ogre::StringConverter::toString(markerCounter));
        nodeMarker->setPosition(location.x, location.y, location.z);
        entMarker->setMaterialName("BingoMarker");
        entMarker->setCastShadows(false);
        nodeMarker->attachObject(entMarker);
        nodeMarker->setVisible(false);
        nodeMarker->yaw(Degree(angle));
    }

void BingoMarker::hide ()
    {
        nodeMarker->setVisible(false);
        _visible = false;
    }

void BingoMarker::show ()
    {
        nodeMarker->setVisible(true);
        _visible = true;
    }

void BingoMarker::setScale ( Ogre::Vector3 scale )
{
    nodeMarker->setScale(scale.x, scale.y, scale.z);
}
