
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

		tutorialName = "NxTutorial";
		instructions = "";
		mHideGraphical = false;

		preSetup();

		setupResources();

		if (!configure())
			return false;

		chooseSceneManager();
		createCamera();
		createViewports();

		TextureManager::getSingleton().setDefaultNumMipmaps(1);
		MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
		MaterialManager::getSingleton().setDefaultAnisotropy(8); 
		loadResources();

		noStats = false;
		mShutdown = false;

		createFrameListener();
		for (int i=0;i < 4;i++) {

			mSceneMgr[i]->setAmbientLight(ColourValue(0.37,0.31,0.37));

			Light* mSunLight =  mSceneMgr[i]->createLight("SunLight");
			mSunLight->setType(Light::LT_POINT);
			mSunLight->setPosition(100,100,100);
		
			mSunLight->setCastShadows(true);
			

			createPlane("grid","nx.grid",Vector3(0,0,0),Vector2(1024,1024),i,Vector2(100,100));
			createPlane("axis","nx.axis",Vector3(0,0.-0.04,0),Vector2(8,8),i);

		}
		preCreateScene();
		createScene();
		
		return true;
    }

	bool configure(void) {
		if(mRoot->showConfigDialog())
			mWindow = mRoot->initialise(true,tutorialName);
		else
			return false;
		return true;
    }

	void chooseSceneManager(void) {
		for (int i=0; i < 4;i++) {
			mSceneMgr[i] = mRoot->createSceneManager(ST_GENERIC, "NxOgre." + StringConverter::toString(i));
		}
	}
   
	void createCamera(void) {
		for (int i=0; i < 4;i++) {
			mCamera[i] = mSceneMgr[i]->createCamera("Camera." + StringConverter::toString(i));
			mCamera[i]->setNearClipDistance(0.1);
		}
	}
    
	void createFrameListener(void) {

		mInputDevice = PlatformManager::getSingleton().createInputReader();
        mInputDevice->initialise(mWindow,true, true);

		mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
		mDebugFPS = OverlayManager::getSingleton().getOverlayElement("nx.gui.stats.fps");
		
		OverlayManager::getSingleton().getOverlayElement("nx.gui.logo")->setLeft(mWindow->getWidth() - 68);
		
		showDebugOverlay(true);
		
		mRoot->addFrameListener(this);
	}

	void createPlane(Ogre::String _name, Ogre::String _material, Ogre::Vector3 _pos, Ogre::Vector2 _size, int _scene, Ogre::Vector2 _subdivisions = Ogre::Vector2(1,1)) {
		Plane _plane;
        _plane.normal = Vector3::UNIT_Y;
        _plane.d = 0;


        MeshManager::getSingleton().createPlane(_name + ".plane",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
            _plane,_size.x,_size.y,16,16,true,1,_subdivisions.x,_subdivisions.y,Vector3::UNIT_Z);
        
		Entity *_entity = mSceneMgr[_scene]->createEntity(_name + ".entity", _name + ".plane");
        _entity->setMaterialName(_material);
		_entity->setCastShadows(false);
		
		
		SceneNode* _node = mSceneMgr[_scene]->getRootSceneNode()->createChildSceneNode(_name + ".node");
		_node->attachObject(_entity);
		_node->setPosition(_pos);


	}

	void createViewports(void) {

		mViewport[0] = mWindow->addViewport(mCamera[0],0);
		mViewport[1] = mWindow->addViewport(mCamera[1],1);
		mViewport[2] = mWindow->addViewport(mCamera[2],2);
		mViewport[3] = mWindow->addViewport(mCamera[3],3);

		mViewport[0]->setDimensions(0,0,0.5,0.5);
		mViewport[1]->setDimensions(0.5,0,0.5,0.5);
		mViewport[2]->setDimensions(0,0.5,0.5,0.5);
		mViewport[3]->setDimensions(0.5,0.5,0.5,0.5);
	
		mCamera[0]->setAspectRatio(Real(mViewport[0]->getActualWidth()) / Real(mViewport[0]->getActualHeight()));
		mCamera[1]->setAspectRatio(Real(mViewport[1]->getActualWidth()) / Real(mViewport[1]->getActualHeight()));
		mCamera[2]->setAspectRatio(Real(mViewport[2]->getActualWidth()) / Real(mViewport[2]->getActualHeight()));
		mCamera[3]->setAspectRatio(Real(mViewport[3]->getActualWidth()) / Real(mViewport[3]->getActualHeight()));

		mViewport[0]->setBackgroundColour(ColourValue(0.37,0.31,0.37));
		mViewport[1]->setBackgroundColour(ColourValue(0.37,0.31,0.37));
		mViewport[2]->setBackgroundColour(ColourValue(0.37,0.31,0.37));
		mViewport[3]->setBackgroundColour(ColourValue(0.37,0.31,0.37));

		mViewport[0]->setOverlaysEnabled(false);
		mViewport[1]->setOverlaysEnabled(false);
		mViewport[2]->setOverlaysEnabled(true);
		mViewport[3]->setOverlaysEnabled(false);
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
		if (noStats)
			return;

		try {
			
			const RenderTarget::FrameStats& stats = mWindow->getStatistics();

			mDebugFPS->setCaption("FPS " + StringConverter::toString(stats.avgFPS));
	
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

		if (mInputDevice->isKeyDown(KC_ESCAPE))
			mShutdown = true;

	
		if (mInputDevice->isKeyDown(KC_F1))
			mWindow->writeContentsToFile("screenshot." + Ogre::StringConverter::toString(Ogre::Math::Floor(mRoot->getTimer()->getMilliseconds() / 1000)) + ".jpg");

		
		mRotX = Degree(-mInputDevice->getMouseRelativeX() * 0.13);
		mRotY = Degree(-mInputDevice->getMouseRelativeY() * 0.13);

		for (int i=0;i < 4;i++) {
			mCamera[i]->yaw(mRotX);
			mCamera[i]->pitch(mRotY);
		}


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

		for (int i=0;i < 4;i++) {
			mCamera[i]->moveRelative(mTranslateVector);
		}
		mMoveScale = 4 * evt.timeSinceLastFrame;
		
		mRotX = 0;
		mRotY = 0;
		mTranslateVector = Vector3::ZERO;

		updateStats();
		checkInput();
		return true;
	}

	bool frameEnded(const FrameEvent& evt) {
		
		
		if (mShutdown)
			return false;
			
		return true;
	}

	virtual void checkInput() {}

public:
	Ogre::String tutorialName;
	Ogre::String instructions;
	bool mHideGraphical;
protected:
	Root *mRoot;
	Camera* mCamera[4];
	SceneManager* mSceneMgr[4];
	Viewport* mViewport[4];

	RenderWindow* mWindow;
	
	EventProcessor* mEventProcessor;
	InputReader* mInputDevice;
	Overlay* mDebugOverlay;
	OverlayElement* mDebugFPS;
	OverlayElement* mInstructions;
	OverlayElement* mTextInstructions;
	
	float mMoveScale;
	Vector3 mTranslateVector;
	bool mShutdown;
	Radian mRotX, mRotY;
	bool noStats;

};

#endif