#include "stdafx.h"

struct_NUMVALS getIndividualNumberValues ( int number )
    {
        struct_NUMVALS nv;

        if ( number > 9 )
            {
                nv.num_1 = number / 10;
                number -= (nv.num_1 * 10);
                nv.num_2 = number;
            }

        else
            {
                nv.num_1 = number;
                nv.num_2 = - 1;
            }

        return nv;
    }

//------------------------------------
SkinnableQuadLetter::SkinnableQuadLetter ()
    {
        entQuad = NULL;
        nodeQuad = NULL;
    }

void SkinnableQuadLetter::create ( int id, Ogre::String filename, Ogre::String texture, Vector3 location,
                                   SceneManager * sm )
    {
        entQuad = sm->createEntity("quadLetterEnt."
                                       + Ogre::StringConverter::toString(++ SkinnableQuadLetter::id),
                                   filename);
        entQuad->setMaterialName(texture);

        nodeQuad = sm->getRootSceneNode()->createChildSceneNode("nodeLetterEnt."
                                                                    + Ogre::StringConverter::toString(SkinnableQuadLetter::id));

        nodeQuad->setPosition(location.x, location.y, location.z);
        nodeQuad->attachObject(entQuad);
    }

void SkinnableQuadLetter::setAngle ( float angle )
{
    nodeQuad->pitch(Radian(angle));
}

void SkinnableQuadLetter::setTexture ( Ogre::String texture )
{
    entQuad->setMaterialName(texture);
}

void SkinnableQuadLetter::show ()
{
    nodeQuad->setVisible(true);
}

void SkinnableQuadLetter::hide ()
{
    nodeQuad->setVisible(false);
}

void SkinnableQuadLetter::destroy () { }
//////////////////////////////////////////////////////////////////////////

SkinnableQuad::SkinnableQuad ()
    {
        entQuad = NULL;
        nodeQuad = NULL;
    }

void SkinnableQuad::create ( SceneManager * sm )
    {
        int id = ++ SkinnableQuad::id;
        entQuad = sm->createEntity("quadEnt." + Ogre::StringConverter::toString(id), "_OBJ_QUAD.mesh");
        nodeQuad = sm->getRootSceneNode()->createChildSceneNode("SkinnableQuad."
                                                                    + Ogre::StringConverter::toString(id));
        nodeQuad->attachObject(entQuad);
    }

void SkinnableQuad::setLocation ( Vector3 * location )
{
    nodeQuad->setPosition(location->x, location->y, location->z);
}

void SkinnableQuad::setAngle ( float angle )
{
    nodeQuad->pitch(Radian(angle));
}

void SkinnableQuad::setTexture ( Ogre::String texture )
{
    entQuad->setMaterialName(texture);
}

void SkinnableQuad::show ()
{
    nodeQuad->setVisible(true);
}

void SkinnableQuad::hide ()
{
    nodeQuad->setVisible(false);
}

void SkinnableQuad::destroy () { }

//--------------------------------------------------------------------

SkinnableQuadSet::SkinnableQuadSet () { };

void SkinnableQuadSet::swapColor ( int colorset, int position, int val )
    {

        if ( val < 10 )
        {
            QuadManager::numberQuad[position].entQuad->setMaterialName(QuadManager::textureNums[colorset][val]);
        }
        else
            {
                struct_NUMVALS nv;
                nv = getIndividualNumberValues(val);
                QuadManager::numberQuad[position
                                            - 1].entQuad->setMaterialName(QuadManager::textureNums[NUM_USE_COLOR][nv.num_1]);
                QuadManager::numberQuad[position].entQuad->setMaterialName(QuadManager::textureNums[NUM_USE_COLOR][nv.num_2]);
            }
    }

//--------------------------------------------------------------------
void SkinnableQuadSet::init ( int position, int val, Vector3 BasePosition, int LightNum )
    {
        Vector3 pos;
        BasePosition.y += .02f;
        pos = BasePosition;

        QuadManager::numberQuad[position - 1].hide();
        QuadManager::numberQuad[position].hide();

        if ( val < 10 )
            {
                QuadManager::numberQuad[position].entQuad->setMaterialName(QuadManager::textureNums[NUM_TEXTURE_BLACK][val]);
                QuadManager::numberQuad[position].nodeQuad->setPosition(pos.x, pos.y, pos.z);
                QuadManager::numberQuad[position].nodeQuad->setVisible(true);
            }

        else
            {
                struct_NUMVALS nv;
                nv = getIndividualNumberValues(val);
                QuadManager::numberQuad[position
                                            - 1].entQuad->setMaterialName(QuadManager::textureNums[NUM_TEXTURE_BLACK][nv.num_1]);
                pos.x -= .45f;
                QuadManager::numberQuad[position - 1].nodeQuad->setPosition(pos.x, pos.y, pos.z);
                QuadManager::numberQuad[position - 1].nodeQuad->setVisible(true);
                pos = BasePosition;
                pos.x += .27f;
                pos.y += .01f;
                QuadManager::numberQuad[position].entQuad->setMaterialName(QuadManager::textureNums[NUM_TEXTURE_BLACK][nv.num_2]);
                QuadManager::numberQuad[position].nodeQuad->setPosition(pos.x, pos.y, pos.z);
                QuadManager::numberQuad[position].nodeQuad->setVisible(true);
            }

        if ( (LightNum > 4) && (LightNum < 9) )
            {
                if ( ! QuadManager::numberQuad[position - 1].pitched )
                    {
                        QuadManager::numberQuad[position - 1].nodeQuad->pitch(Radian(LANE_ANGLE));
                        QuadManager::numberQuad[position - 1].pitched = true;
                    }

                if ( ! QuadManager::numberQuad[position].pitched )
                    {
                        QuadManager::numberQuad[position].nodeQuad->pitch(Radian(LANE_ANGLE));
                        QuadManager::numberQuad[position].pitched = true;
                    }
            }

        else
            {
                QuadManager::numberQuad[position - 1].nodeQuad->pitch(Radian(0));
                QuadManager::numberQuad[position].nodeQuad->pitch(Radian(0));
            }
    }

//------------------------------------------------------------------
void SkinnableQuadSet::show ( int position )
    {
        QuadManager::numberQuad[position - 1].nodeQuad->setVisible(true);
        QuadManager::numberQuad[position].nodeQuad->setVisible(true);
    }
//------------------------------------------------------------------

SkinnableQuadSetBingo::SkinnableQuadSetBingo ()
    {
        entDauber = NULL;
        nodeDauber = NULL;
    };

void SkinnableQuadSetBingo::daub ()
{
    nodeDauber->setVisible(true);
}

//////////////////////////////////////////////////////////////////////////

void SkinnableQuadSetBingo::hide ( int position )
    {
        QuadManager::BingoCardQuad[position - 1].nodeQuad->setVisible(false);
        QuadManager::BingoCardQuad[position].nodeQuad->setVisible(false);
    }
//////////////////////////////////////////////////////////////////////////

void SkinnableQuadSetBingo::create ( SceneManager * sm )
    {
        int id = ++ SkinnableQuadSetBingo::id;
        entDauber = sm->createEntity("quadEntBingoCard." + Ogre::StringConverter::toString(id),
                                     "_OBJ_DAUBER.mesh");

        entDauber->setMaterialName("dauber");
        nodeDauber = sm->getRootSceneNode()->createChildSceneNode("Dauber."
                                                                      + Ogre::StringConverter::toString(id));

        nodeDauber->attachObject(entDauber);
    }

void SkinnableQuadSetBingo::init ( int position, int val, Vector3 BasePosition, int CardPos )
    {
        Vector3 pos, pos2;

        pos2 = BasePosition;
        pos2.y = .04f;
        nodeDauber->setPosition(pos2.x, pos2.y, pos2.z);
        nodeDauber->setVisible(false);
		
		
        
		QuadManager::BingoCardQuad[position - 1].hide();

		
        QuadManager::BingoCardQuad[position].hide();
        pos = BasePosition;

        int UseNumberSet = NUM_TEXTURE_GREEN;

        if ( val < 10 )
            {
				
                QuadManager::BingoCardQuad[position].entQuad->setMaterialName(QuadManager::textureNums[UseNumberSet][val]);
                QuadManager::BingoCardQuad[position].nodeQuad->setPosition(pos.x, pos.y, pos.z);
                QuadManager::BingoCardQuad[position].nodeQuad->setVisible(true);
            }

        else
            {
			
                struct_NUMVALS nv;
                nv = getIndividualNumberValues(val);
			    QuadManager::BingoCardQuad[position- 1].entQuad->setMaterialName(QuadManager::textureNums[UseNumberSet][nv.num_1]);
                pos.x -= .39f;
			    QuadManager::BingoCardQuad[position - 1].nodeQuad->setPosition(pos.x, pos.y, pos.z);
			    QuadManager::BingoCardQuad[position - 1].nodeQuad->setVisible(true);
			    pos = BasePosition;
                pos.y += .009f;
                pos.x += .3f;
			    QuadManager::BingoCardQuad[position].entQuad->setMaterialName(QuadManager::textureNums[UseNumberSet][nv.num_2]);
			    QuadManager::BingoCardQuad[position].nodeQuad->setPosition(pos.x, pos.y, pos.z);
			    QuadManager::BingoCardQuad[position].nodeQuad->setVisible(true);
				
            }
    }

//-------------------------------------------------------
void SkinnableQuadSetBingo::show ( int position )
{
    QuadManager::BingoCardQuad[position].nodeQuad->setVisible(true);
}
//////////////////////////////////////////////////////////////////////////

Ogre::String QuadManager::textureNums[3][10];
Entity * QuadManager::ballEnt;
SkinnableQuadLetter QuadManager::BingoLetters[6];
SkinnableQuad QuadManager::numberQuad[30];
SkinnableQuad QuadManager::BingoCardQuad[50];
SkinnableQuadSet QuadManager::quadSet[16];
SkinnableQuadSetBingo QuadManager::BingoCardQuadSet[27];

int SkinnableQuad::id = 0;
int SkinnableQuadLetter::id = 0;
int SkinnableQuadSetBingo::id = 0;
int QuadManager::CurrBingoLetter = 0;
//---------------------------------------------------------------  
