/*

	Tutorials

*/

#ifndef __character_inside_tutorial_H__
#define __character_inside_tutorial_H__


#include "Ogre.h"
#include "OgreConfigFile.h"
#include "OgreKeyEvent.h"
#include "OgreEventListeners.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

using namespace Ogre;

class CharacterOutsideTutorial : public FrameListener {

public:

	CharacterOutsideTutorial() {
		mTutorialName = "???";
		mTutorialDescription = "???";
		mShutdownNow = false;
	}

	~CharacterOutsideTutorial() {}

	bool Run() {

		getSetup();

		mRoot = new Root();



		ConfigFile cf;
		cf.load("resources.cfg");

		ConfigFile::SectionIterator seci = cf.getSectionIterator();
		String secName, typeName, archName;
		
		while (seci.hasMoreElements()) {
			secName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap *settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i) {
				typeName = i->first;
				archName = i->second;
				ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			}
		}


/*

		unsigned int width = 640, height = 400;
		bool fullscreen = false;


		Ogre::RenderSystemList *renderSystems = NULL;
		Ogre::RenderSystemList::iterator r_it;



		ConfigFile cf;
		
		cf.load("configuration.yaml",":",true);
		
		ConfigFile::SectionIterator seci = cf.getSectionIterator();
		String secName, typeName, archName;

		while (seci.hasMoreElements()) {
			secName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap *settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;
			
			for (i = settings->begin(); i != settings->end(); ++i) {

				if (i->first == "Device") {

					if (i->second == "D3D9") {
						mRoot->setRenderSystem(mRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem"));
					}
					else if (i->second == "OpenGL"){
						mRoot->setRenderSystem(mRoot->getRenderSystemByName("OpenGL Rendering Subsystem"));
					}
					else {
						mRoot->setRenderSystem(mRoot->getRenderSystemByName("OpenGL Rendering Subsystem"));
					}	
				}

				break;
			}
		}
*/
		// Gotta do it all over again now with the options.

		if(mRoot->showConfigDialog())
			mWindow = mRoot->initialise(true,mTutorialName + " - " + mTutorialDescription);
		else
			return false;
		
		// Swap these two around if your not using Dagon.

		//mSceneMgr = mRoot->getSceneManager(ST_GENERIC);
		mSceneMgr = mRoot->createSceneManager("TerrainSceneManager");
		
		mCamera = mSceneMgr->createCamera("MainCamera");
		mCamera->setPosition(Vector3(0,0,0));
		mCamera->lookAt(Vector3(0,0,0));
		mCamera->setNearClipDistance(0.1);

		mViewport = mWindow->addViewport(mCamera);
		mViewport->setBackgroundColour(ColourValue(0.37,0.31,0.37));
		mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));


		TextureManager::getSingleton().setDefaultNumMipmaps(1);
		MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
		MaterialManager::getSingleton().setDefaultAnisotropy(8); 

		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();


		mInputDevice = PlatformManager::getSingleton().createInputReader();
        mInputDevice->initialise(mWindow,true, true);
		mRoot->addFrameListener(this);


		mSceneMgr->setAmbientLight(ColourValue(0.37,0.31,0.37));
        mSceneMgr->setShadowTechnique( SHADOWTYPE_STENCIL_ADDITIVE );

		Light* mSunLight =  mSceneMgr->createLight("SunLight");
		mSunLight->setType( Light::LT_POINT );
		mSunLight->setPosition( Vector3(0, 150, 250) );
	
		std::cout << "1" << std::endl;
		ColourValue fadeColour(0.93, 0.86, 0.76);
        mSceneMgr->setFog( FOG_LINEAR, fadeColour, .001, 500, 1000);
        //mWindow->getViewport(0)->setBackgroundColour(fadeColour);

		std::cout << "2" << std::endl;
		std::string terrain_cfg("216.cfg");
		mSceneMgr->setWorldGeometry( terrain_cfg );
		std::cout << "3" << std::endl;
        // Infinite far plane?
        if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
        {
			std::cout << "3a" << std::endl;
            mCamera->setFarClipDistance(0);
			std::cout << "3b" << std::endl;
        }
		std::cout << "4" << std::endl;


		ApplicationStart();
		
		mRoot->startRendering();


		return true;
	}

	void Shutdown() {
		ApplicationStop();
		mRoot->shutdown();
		delete mRoot;
	}

protected:

	Ogre::String mTutorialName;
	Ogre::String mTutorialDescription;

	bool frameStarted(const FrameEvent& evt) {
		
		if(mWindow->isClosed())
			return false;
		
		mInputDevice->capture();

		if (mInputDevice->isKeyDown(KC_ESCAPE))
			mShutdownNow = true;
		

			
		mRotX = Degree(-mInputDevice->getMouseRelativeX() * 0.13);
		mRotY = Degree(-mInputDevice->getMouseRelativeY() * 0.13);

		mCamera->yaw(mRotX);
		mCamera->pitch(mRotY);


		if (mInputDevice->isKeyDown(KC_A))
            mTranslateVector.x = -mMoveScale;


        if (mInputDevice->isKeyDown(KC_D))
            mTranslateVector.x = mMoveScale;

        if (mInputDevice->isKeyDown(KC_UP) || mInputDevice->isKeyDown(KC_W) )
			mTranslateVector.z = -mMoveScale;

        if (mInputDevice->isKeyDown(KC_DOWN) || mInputDevice->isKeyDown(KC_S) )
            mTranslateVector.z = mMoveScale;


        if (mInputDevice->isKeyDown(KC_PGUP))
            mTranslateVector.y = mMoveScale;


        if (mInputDevice->isKeyDown(KC_PGDOWN))
            mTranslateVector.y = -mMoveScale;

		mCamera->moveRelative(mTranslateVector);
	
		mMoveScale = 16 * evt.timeSinceLastFrame;
		
		mRotX = 0;
		mRotY = 0;
		mTranslateVector = Vector3::ZERO;

		onFrame(evt.timeSinceLastFrame);
	
		return true;
	}


	bool frameEnded(const FrameEvent& evt) {
		
		if (mShutdownNow)
			return false;
			
		return true;
	}


	void ApplicationStart() {
		mCamera->setPosition(10,10,10);
		mCamera->lookAt(0,0,0);
		prestart();
		start();
	}

	void ApplicationStop() {
		prestop();
		stop();
	}

	void getSetup() {
		getTutorialSettings();
	}

	void onFrame(float _time) {
		newFrame(_time);
	}

	virtual void getTutorialSettings() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void prestart() = 0;
	virtual void prestop() = 0;

	virtual void newFrame(float _time) = 0;

	Ogre::Root* mRoot;
	Ogre::Camera* mCamera;
	Ogre::RenderWindow* mWindow;
	Ogre::Viewport* mViewport;
	Ogre::SceneManager* mSceneMgr;
	Ogre::InputReader* mInputDevice;

	bool mShutdownNow;
	Vector3 mTranslateVector;
	float mMoveScale;
	Radian mRotX;
	Radian mRotY;
};



#endif