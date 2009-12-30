#ifndef _SKINNABLEQUAD

#define _SKINNABLEQUAD

#include "stdafx.h"



//---------------------------------------------------
class SkinnableQuadLetter
    {
        public:

            Entity * entQuad;
            SceneNode * nodeQuad;
            SkinnableQuadLetter();
            static int id;
            void create(int id, Ogre::String filename, Ogre::String texture, Vector3 location,
                        SceneManager * sm);
            void setAngle(float angle);
            void setTexture(Ogre::String textureName);
            void show();
            void hide();
            void destroy();
    };

//---------------------------------------------------
class SkinnableQuad
    {
        public:

            Entity * entQuad;
            SceneNode * nodeQuad;
            SkinnableQuad();
            static int id;
            bool pitched;
            void create(SceneManager * sm);
            void setLocation(Vector3 * location);
            void setAngle(float angle);
            void setTexture(Ogre::String textureName);
            void show();
            void hide();
            void destroy();
    };


//---------------------------------------------------
class SkinnableQuadSet
    {
        public:

            SkinnableQuadSet();

            void init(int position, int val, Vector3 BasePosition, int LightNum);
            void show(int position);
            void swapColor(int colorset, int position, int val);
    };

//---------------------------------------------------
class SkinnableQuadSetBingo
    {
        public:

            SkinnableQuadSetBingo();
            Entity * entDauber;
            SceneNode * nodeDauber;
            Entity * entQSBingo;
            Node * nodeQSBingo;
            static int id;
            void daub();
            void create(SceneManager * sm);
            void hide(int position);
            void init(int position, int val, Vector3 BasePosition, int CardPos);
            void show(int position);
    };

class QuadManager
    {
        public:

            static SkinnableQuadSet quadSet[16];
            static SkinnableQuadSetBingo BingoCardQuadSet[27];
            static SkinnableQuadLetter BingoLetters[6];
            static SkinnableQuad numberQuad[30];
            static Ogre::String textureNums[3][10];
            static SkinnableQuad BingoCardQuad[50];
            static int CurrBingoLetter;
			static Entity * ballEnt;
			
            //=--------------------------------------
            static void SwitchNumberSet ( int CurrentBingoLetter )
                {
                    for ( int x = 1; x < 6; x ++ )
                    {
                        BingoLetters[x].hide();
                    }

                    CurrBingoLetter = CurrentBingoLetter;
                    BingoLetters[CurrBingoLetter].show();
                    int positionCounter = 1;

                    for ( int i = 1; i < 16; i ++ )
                        {
                            quadSet[i].init(positionCounter, NUMBERSETS[CurrentBingoLetter - 1][i - 1],
                                            LIGHT_POS[i],    i);
                            positionCounter += 2;
                        }
					if (!ballEnt)
					{
						return;
					}
					switch(CurrentBingoLetter)
					{
					case B_:
						ballEnt->setMaterialName("ball_b");
						break;
					case I_:
						ballEnt->setMaterialName("ball_i");
						break;
					case N_:
						ballEnt->setMaterialName("ball_n");
						break;
					case G_:
						ballEnt->setMaterialName("ball_g");
						break;
					case O_:
						ballEnt->setMaterialName("ball_o");
						break;
					}

                }
            //=--------------------------------------

            QuadManager ( SceneManager * sm )
                {
                    Ogre::String tmpString = "";
					
					LogManager::getSingletonPtr()->logMessage("Loading Green");

                    for ( int i = 0; i < 10; i ++ )
                        {
                            tmpString = "numbers/green/" + Ogre::StringConverter::toString(i);
                            textureNums[NUM_TEXTURE_GREEN][i] = tmpString;
                            tmpString = "";
                        }
					LogManager::getSingletonPtr()->logMessage("Loading Black");
                    for ( int i = 0; i < 10; i ++ )
                        {
                            tmpString = "numbers/black_2/" + Ogre::StringConverter::toString(i);
                            textureNums[NUM_TEXTURE_BLACK][i] = tmpString;
                            tmpString = "";
                        }
					/*
					LogManager::getSingletonPtr()->logMessage("Loading White");
                    for ( int i = 0; i < 10; i ++ )
                        {
                            tmpString = "numbers/white/" + Ogre::StringConverter::toString(i);
                            textureNums[NUM_TEXTURE_WHITE][i] = tmpString;
                            tmpString = "";
                        }
					*/
					LogManager::getSingletonPtr()->logMessage("Loading numberQuad");
                    for ( int i = 0; i < 30; i ++ )
                    {
                        numberQuad[i].create(sm);
                    }
					LogManager::getSingletonPtr()->logMessage("Loading BingoCardQuad");
                    for ( int i = 0; i < 50; i ++ )
                    {
                        BingoCardQuad[i].create(sm);
                    }
					LogManager::getSingletonPtr()->logMessage("Loading BingoCardQuadSet");
                    for ( int i = 1; i < 27; i ++ )
                    {
                        BingoCardQuadSet[i].create(sm);
                    }
					LogManager::getSingletonPtr()->logMessage("Loading BingoLetters");
                    BingoLetters[1].create(1, "_OBJ_B.mesh", "quads/b", Vector3(- 4.1f, .1f, 1.25f), sm);
                    BingoLetters[2].create(2, "_OBJ_B.mesh", "quads/i", Vector3(- 2.5f, .1f, 1.25f), sm);
                    BingoLetters[3].create(3, "_OBJ_B.mesh", "quads/n", Vector3(- .8f, .1f, 1.25f), sm);
                    BingoLetters[4].create(4, "_OBJ_B.mesh", "quads/g", Vector3(.7f, .1f, 1.25f), sm);
                    BingoLetters[5].create(5, "_OBJ_B.mesh", "quads/o", Vector3(2.27f, .1f, 1.25f), sm);

					LogManager::getSingletonPtr()->logMessage("SwitchNumberSet");
                    SwitchNumberSet(B_);

					

                    //ResetBingoCard2(- 2);
					LogManager::getSingletonPtr()->logMessage("resetCard");
					BingoManager::getSingleton()->resetCard(-2);
                }

			void setBallEnt(Entity * ent){
				ballEnt = ent;
			}

        protected:
        private:
    };

//---------------------------------------------------

#endif
