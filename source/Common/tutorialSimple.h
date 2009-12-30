#ifndef __tutorial_H__
#define __tutorial_H__


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
		
	}
	
	void go(void) {
		if (!setup())
            return;
		mRoot->startRendering();
		destroyScene();
		std::cout << "shutting down" << std::endl;
		mRoot->shutdown();
		delete mRoot;
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
		
		mSceneMgr->setAmbientLight(ColourValue(0.37,0.31,0.37));

        mSceneMgr->setShadowTechnique( SHADOWTYPE_STENCIL_ADDITIVE );

		Light* mSunLight =  mSceneMgr->createLight("SunLight");
		mSunLight->setType( Light::LT_POINT );
		mSunLight->setPosition( Vector3(0, 150, 250) );

		createPlane("grid","nx.grid",Vector3(0,0,0),Vector2(1024,1024),Vector2(100,100));
		createPlane("axis","nx.axis",Vector3(0,0.-0.04,0),Vector2(8,8));

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

		mInputDevice = PlatformManager::getSingleton().createInputReader();
        mInputDevice->initialise(mWindow,true, true);

		mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
		mDebugFPS = OverlayManager::getSingleton().getOverlayElement("nx.gui.stats.fps");

		mLogo = OverlayManager::getSingleton().getOverlayElement("nx.gui.logo");
		mLogo->setLeft(mWindow->getWidth() - 128 - 4);
		mLogo->setTop(mWindow->getHeight() - 128 - 4);

		//if (mHideGraphical == true)
		//	mInstructions->hide();

		mTextInstructions = OverlayManager::getSingleton().getOverlayElement("nx.gui.text-instructions.text");
		mTextInstructions->setCaption(instructions);


		//OverlayManager::getSingleton().getOverlayElement("nx.gui.logo")->setLeft(mWindow->getWidth() - 68);
		
		showDebugOverlay(true);

		
		mRoot->addFrameListener(this);
	}

	void createPlane(Ogre::String _name, Ogre::String _material, Ogre::Vector3 _pos, Ogre::Vector2 _size, Ogre::Vector2 _subdivisions = Ogre::Vector2(1,1)) {
		Plane _plane;
        _plane.normal = Vector3::UNIT_Y;
        _plane.d = 0;


        MeshManager::getSingleton().createPlane(_name + ".plane",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
            _plane,_size.x,_size.y,16,16,true,1,_subdivisions.x,_subdivisions.y,Vector3::UNIT_Z);
        
		Entity *_entity = mSceneMgr->createEntity(_name + ".entity", _name + ".plane");
        _entity->setMaterialName(_material);
		_entity->setCastShadows(true);
		
		
		SceneNode* _node = mSceneMgr->getRootSceneNode()->createChildSceneNode(_name + ".node");
		_node->attachObject(_entity);
		_node->setPosition(_pos);
		
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
		if (noStats)
			return;

		try {
			
			const RenderTarget::FrameStats& stats = mWindow->getStatistics();

			mDebugFPS->setCaption(StringConverter::toString(stats.avgFPS));
	
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
	Camera* mCamera;
	SceneManager* mSceneMgr;
	RenderWindow* mWindow;
	EventProcessor* mEventProcessor;
	InputReader* mInputDevice;
	Overlay* mDebugOverlay;
	OverlayElement* mDebugFPS;
	OverlayElement* mLogo;
	OverlayElement* mTextInstructions;
	Viewport *mShadowVp;
	CompositorInstance* mShadowCompositor;

	float mMoveScale;
	Vector3 mTranslateVector;
	bool mShutdown;
	Radian mRotX, mRotY;
	bool noStats;

};

#endif