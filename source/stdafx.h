// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)

#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000

#pragma once

#endif                      // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers


#include "Ogre.h"
using namespace Ogre;




static int CurNumSet = 1;
static int GAME_STATE;
static int INTERMISSION_STATE;


const static int NUMBERSETS[5][15] = {
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
	{ 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 },
	{ 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45 },
	{ 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60 },
	{ 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75 } };

	const static Vector3 LIGHT_POS[16] =
	{
		Vector3(- 50.2f, - 50.2f, - 50.2f), Vector3(- 7.0f, 0.2f, 1.0f),     Vector3(- 5.7f, 0.2f, - .9f),
		Vector3(- 3.8f, 0.2f, - 8.67f),     Vector3(- 2.7f, 0.2f, - 10.45f), Vector3(- 2.03f, 1.55f, - 13.04f),
		Vector3(- .05f, 1.55f, - 13.04f),   Vector3(1.9f, 1.55f, - 13.04f),  Vector3(3.88f, 1.55f, - 13.04f),
		Vector3(4.59f, 0.2f, - 10.25f),     Vector3(5.5f, 0.2f, - 8.65f),    Vector3(5.6f, 0.2f, - 6.95f),
		Vector3(5.0f, 0.2f, - 5.0f),        Vector3(4.0f, 0.2f, - 1.08f),    Vector3(4.5f, 0.2f, .8f),
		Vector3(5.5f, 0.2f, 2.37f)
	};

	const static Vector3 TRIGLIGHT_POS[5] = 
	{

		Vector3(-6.25 - .2, .009, -6.5),
		Vector3(-7.09 - .2, .009, -8.7),
		Vector3(-7.32 - .2, .009, -11.1),
		Vector3(-6.87 - .2, .009, -13.4)

	};

	struct struct_NUMVALS
	{
		int num_1;
		int num_2;
	};

	enum Keys
	{
		POPUP,
		RETRACT,
		QUIT,
		SCREENSHOT,
		DEBUG_MODE,
		RESET_SCENE,
		PAUSE,
		SLOWER,
		FASTER,
		OPTION_1,
		OPTION_2,
		OPTION_3,
		OPTION_4,
		PAN_FORWARD,
		PAN_BACKWARD,
		PAN_LEFT,
		PAN_RIGHT,
		PAN_UP,
		PAN_DOWN,
		X,
		NEG_X,
		Y,
		NEG_Y,
		Z,
		NEG_Z,
		MOUSE_PRIMARY,
		MOUSE_ALT,
		MOUSE_NONE,
		MOUSE_BOTH,
		FIRE_BALL1,
		FIRE_BALL2,
		RESET_BALL,
		RESET_ALL,
		PLUS_VEL,
		MINUS_VEL,
		NEW_GAME,
		FLIP_LEFT1,
		FLIP_LEFT2,
		FLIP_LEFT3,
		FLIP_LEFT4,
		FLIP_LEFT5,
		FLIP_LEFT6,
		FLIP_LEFT7,
		FLIP_LEFT8,
		FLIP_LEFT9,
		FLIP_LEFT10,
		FLIP_RIGHT1,
		FLIP_RIGHT2,
		FLIP_RIGHT3,
		FLIP_RIGHT4,
		FLIP_RIGHT5,
		FLIP_RIGHT6,
		FLIP_RIGHT7,
		FLIP_RIGHT8,
		FLIP_RIGHT9,
		FLIP_RIGHT10,
		SEND_SCORE,
		POST_SCORE,
		NO_POST_SCORE,
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT
	};






#include "OgreFontManager.h"
#include "OgreStringConverter.h"
#include "OgreTextAreaOverlayElement.h"
#include "OgreText.h"



#include "NxOgre.h"
using namespace NxOgre;

extern NxOgre::World * mWorld;
extern NxOgre::Scene* mScene;




#include <Windows.h>
#include <direct.h>
#include <stdio.h>
#include <time.h>
#include <WinInet.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <string.h>



void setVectorLength ( NxVec3 * vector, const Ogre::Real newLength );



#include "NxOgreSceneContactController.h"
#include "NxOgreSceneTriggerController.h"
#include "OIS.h"
using namespace OIS;
#include "InputHandler.h"



#include <vector>
using namespace std;



#include "defines.h"


void loadNumTextures ();
void SpinBingo ();
bool ResetBingoCard2 ( int lastHit );
void resetHitNumbers ();





#include "web.h"

#include "BingoBoard.h"
#include "bingomarker.h"

#include "bingoballmarker.h"
#include "BingoManager.h"

#include "TriggerLightManager.h"
#include "HitNumberManager.h"

#include "soundManager.h"
#include "SkinnableQuad.h"
#include "numberMachine.h"

#include "lights.h"

#include "codec.h"
#include "GameMetrics.h"
#include "scoreManager.h"
#include "recentScores.h"

#include "bubble_notify.h"
#include "loading_animation.h"






#include "intermission.h"
#include "uploader.h"
#include "codec.h"

#include "BingoBoard.h"

#include "Renderer.h"
#include "Viewer.h"
#include "gameData.h"
#include "pin.h"
#include "GameClass.h"









using namespace openutils;




// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
