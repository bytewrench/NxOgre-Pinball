
#ifndef __tutorial_H__
#define __tutorial_H__

/*

		Tutorial.h
		
		This is pretty much ExampleApplication and ExampleListener 
		combined into the same class, go nuts.

*/

#include "Ogre.h"
#include "OgreConfigFile.h"
#include "OgreKeyEvent.h"
#include "OgreEventListeners.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

using namespace Ogre;								


class TutorialApplication: public FrameListener {

public:
    
	TutorialApplication() {
		mRoot = 0;
	}

	~TutorialApplication() {
		if (mRoot)
		delete mRoot;
	}
	
	void go(void) {
		if (!setup())
            return;
		mRoot->startRendering();
		destroyScene();
    }

	virtual void preSetup() {}
	bool setup(void) {
		mRoot = new Root();
		
		preSetup();
		setupResources();
		if (!configure())
			return false;

		chooseSceneManager();
		createCamera();
		createViewports();
		TextureManager::getSingleton().setDefaultNumMipmaps(5);
		loadResources();
		createFrameListener();

		mShutdown = false;
		
		 mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
		mSceneMgr->setAmbientLight(ColourValue(0.0, 0.0, 0.0));
		Light* mSunLight = mSceneMgr->createLight("Sun");
		mSunLight = mSceneMgr->createLight("SunLight");
        //mSunLight->setType(Light::LT_SPOTLIGHT);
        mSunLight->setPosition(500,1250,500);
		mSunLight->setCastShadows(true);
        Vector3 dir;
        dir = -mSunLight->getPosition();
        dir.normalise();
        mSunLight->setDirection(dir);
        mSunLight->setDiffuseColour(0.35, 0.35, 0.38);
        mSunLight->setSpecularColour(0.9, 0.9, 1);
		

		SceneNode* mLightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("SunLight");
		mLightNode->attachObject(mSunLight);
		mLightNode->setPosition(0,0,0);
		mSceneMgr->setFog( FOG_LINEAR, ColourValue(0.37,0.31,0.37), 0, 500,  600);

		if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_HWRENDER_TO_TEXTURE)) {
            // In D3D, use a 1024x1024 shadow texture
            mSceneMgr->setShadowTextureSettings(1024, 2);
        }
        else {
            // Use 512x512 texture in GL since we can't go higher than the window res
            mSceneMgr->setShadowTextureSettings(512, 2);
        }
        mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));


		Plane p;
        p.normal = Vector3::UNIT_Y;
        p.d = 0;
        MeshManager::getSingleton().createPlane("FloorPlane",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
            p,128,128,1,1,true,1,16,16,Vector3::UNIT_Z);

      Entity *ent = mSceneMgr->createEntity("floor", "FloorPlane");
      ent->setMaterialName("nx.basic.floor");
      mSceneMgr->getRootSceneNode()->attachObject(ent);


		Plane axis;
        axis.normal = Vector3::UNIT_Y;
        axis.d = 0;
        MeshManager::getSingleton().createPlane("axis",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
            p,8,8,1,1,true,1,1,1,Vector3::UNIT_Z);
        Entity *entax = mSceneMgr->createEntity("axis", "axis");
        entax->setMaterialName("nx.basic.floor.axis");

		SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode("axis");
		sn->attachObject(entax);
		sn->setPosition(0,-0.01,0);
 		//mSceneMgr->setFog( FOG_LINEAR, fadeColour, 0.0, 50, 500 );
		mSceneMgr->setFog( FOG_LINEAR, ColourValue(0.37,0.31,0.37), 0, 500,  600);

		mLookupMax = 80;
		preCreateScene();
		createScene();
		
		return true;
    }

	bool configure(void) {
		//if (!mRoot->restoreConfig())
		if(mRoot->showConfigDialog())
			mWindow = mRoot->initialise(true,tutorialName);
		else
			return false;
		return true;
    }

	void chooseSceneManager(void) {
		
		// Swap these two around if your not using Dagon.

		//mSceneMgr = mRoot->getSceneManager(ST_GENERIC);
		mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "NxOgre");
	}
   
	void createCamera(void) {
		mCamera = mSceneMgr->createCamera("PlayerCam");
		mCamera->setPosition(Vector3(0,0,0));
		mCamera->lookAt(Vector3(0,0,0));
		mCamera->setNearClipDistance(0.1);

	}
    
	void createFrameListener(void) {
		/*mEventProcessor = new EventProcessor();
		mEventProcessor->initialise(mWindow);
		mEventProcessor->startProcessingEvents();
		mEventProcessor->addKeyListener(this);
		mInputDevice = mEventProcessor->getInputReader();*/
			mInputDevice = PlatformManager::getSingleton().createInputReader();
         mInputDevice->initialise(mWindow,true, true);

//		mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
		mDebugFPS = OverlayManager::getSingleton().getOverlayElement("nx.gui.stats.fps");
/*	
		mInstructions = OverlayManager::getSingleton().getOverlayElement("nx.gui.instructions");
		mInstructions->hide();
	//	mInstructions->setLeft(mWindow->getWidth() - 196 - 4);
//	mInstructions->setTop(mWindow->getHeight() - 196 - 4);

		OverlayManager::getSingleton().getOverlayElement("nx.gui.logo")->setLeft(mWindow->getWidth() - 68);
		*/
		showDebugOverlay(true);
		
		mRoot->addFrameListener(this);

	}

	void createViewports(void) {
		Viewport* vp = mWindow->addViewport(mCamera);
		
		// 96, 80 , 97
		vp->setBackgroundColour(ColourValue(0.37,0.31,0.37));
		mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
	}

	void setupResources(void) {
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
	}

	void loadResources(void) {
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}

	virtual void createScene() {}
	virtual void preCreateScene() {}
	virtual void destroyScene() {}

	void updateStats(void) {
		
		try {
			
			const RenderTarget::FrameStats& stats = mWindow->getStatistics();

			mDebugFPS->setCaption("FPS " + StringConverter::toString(stats.avgFPS));
	
	/*		OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
			guiDbg->setCaption(mWindow->getDebugText());*/
     	}
		catch(...) {}
	}

	void showDebugOverlay(bool show) {
		if (!mDebugOverlay)
      	return;

		if (show)
			mDebugOverlay->show();
		else
			mDebugOverlay->hide();
	}

	bool frameStarted(const FrameEvent& evt) {
		if(mWindow->isClosed())
			return false;
		
		mInputDevice->capture();

		if (mInputDevice->isKeyDown(KC_ESCAPE)) {
			mShutdown = true;
      }
	
		if (mInputDevice->isKeyDown(KC_F1)) {
			mWindow->writeContentsToFile("screenshot.jpg");
		}
		
		mRotX = Degree(-mInputDevice->getMouseRelativeX() * 0.13);
		mRotY = Degree(-mInputDevice->getMouseRelativeY() * 0.13);

		mCamera->yaw(mRotX);

		// Need to clean this up!

		mCamera->pitch(mRotY);
		

		//	mCamera->pitch(mRotY);
		//}
	// 	mCamera->getOrientation().getPitch().valueDegrees() < 90) {		
	
		
		//mDebugFPS->setCaption(Ogre::StringConverter::toString(mCamera->getPitch()));
		
		//mCamera->moveRelative(mTranslateVector);
	
		mMoveScale = 4 * evt.timeSinceLastFrame;
		
		mRotX = 0;
		mRotY = 0;
		mTranslateVector = Vector3::ZERO;
		
		checkInput();
		return true;
	}

	bool frameEnded(const FrameEvent& evt) {
		updateStats();
		
		if (mShutdown)
			return false;
			
		return true;
	}

	virtual void checkInput() {}

public:
	Ogre::String tutorialName;
protected:
	Root *mRoot;
	Camera* mCamera;
	SceneManager* mSceneMgr;
	RenderWindow* mWindow;
	EventProcessor* mEventProcessor;
	InputReader* mInputDevice;
	Overlay* mDebugOverlay;
	OverlayElement* mDebugFPS;
	OverlayElement* mInstructions;
	
	SceneNode* mTargetNode;
float mMoveScale;
Vector3 mTranslateVector;
	bool mShutdown;
	 Radian mRotX, mRotY;
   Radian mLookupMax;
};

#endif