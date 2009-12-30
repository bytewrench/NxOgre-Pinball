/*

	Tutorials

*/

#ifndef __simple_tutorial_H__
#define __simple_tutorial_H__


#include "Ogre.h"
#include "OgreConfigFile.h"

#include "OgreStringConverter.h"
#include "OgreException.h"

using namespace Ogre;


// (If Eihort or newer)
#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 3

	// Load in the OIS.lib manually. This allows this code to 
	// use both Input System without any changes to the linker
	// settings.
#ifndef _DEBUG
	#pragma comment(lib, "OIS.lib")
#else
	#pragma comment(lib, "OIS_d.lib")
#endif
	#define OIS_DYNAMIC_LIB
	#include "OIS/OIS.h"
	using namespace OIS;

// (Any versions older than Eihort)
#else

	#include "OgreKeyEvent.h"
	#include "OgreEventListeners.h"

#endif


class FancyInput {

	public:

		enum Key {
			QUIT,
			SCREENSHOT,
			DEBUG_MODE,
			RESET_SCENE,

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
			MOUSE_BOTH
		};

#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 3		
		OIS::Mouse*    mMouse;
		OIS::Keyboard* mKeyboard;
#else
		Ogre::InputReader* mInputDevice;
#endif
		std::map<Key, KeyCode> keys;
	
		FancyInput() {
			// Defaults
			keys[QUIT] = KC_ESCAPE;
			keys[SCREENSHOT] = KC_F1;
			keys[DEBUG_MODE] = KC_F2;
			keys[RESET_SCENE] = KC_F5;
			keys[OPTION_1] = KC_1;
			keys[OPTION_2] = KC_2;
			keys[OPTION_3] = KC_3;
			keys[OPTION_4] = KC_4;
			keys[PAN_FORWARD] = KC_W;
			keys[PAN_BACKWARD] = KC_S;
			keys[PAN_LEFT] = KC_A;
			keys[PAN_RIGHT] = KC_D;
			keys[PAN_UP] = KC_Q;
			keys[PAN_DOWN] = KC_Z;
			keys[X] = KC_J;
			keys[NEG_X] = KC_L;
			keys[Y] = KC_U;
			keys[NEG_Y] = KC_M;
			keys[Z] = KC_I;
			keys[NEG_Z] = KC_K;
		}

		void createInputSys(RenderWindow *_window) {

#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 3
			LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
			ParamList pl;	
			size_t windowHnd = 0;
			std::ostringstream windowHndStr;

			_window->getCustomAttribute("WINDOW", &windowHnd);
			windowHndStr << windowHnd;
			pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

			InputManager &im = *OIS::InputManager::createInputSystem( pl );

			//Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
			mKeyboard = static_cast<Keyboard*>(im.createInputObject( OISKeyboard, true ));
			mMouse = static_cast<Mouse*>(im.createInputObject( OISMouse, true ));
#else
			mInputDevice = Ogre::PlatformManager::getSingleton().createInputReader();
			mInputDevice->initialise(_window,true, true);
#endif
		}

		void captureInput() {
#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 3
			mKeyboard->capture();
			mMouse->capture();
#else
			mInputDevice->capture();
#endif
		}

		// TODO
		void fromConfig(std::map<std::string, std::string> config) {
			// Loaded Yaml configuration file.
			// 
		}

		bool isKeyDown(Key k) {
#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 3
			return mKeyboard->isKeyDown(keys[k]);
#else
			return mInputDevice->isKeyDown(keys[k]);
#endif
		}

		float getRMouseX() {
#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 3
			const MouseState &ms = mMouse->getMouseState();
			return ms.relX;
#else
			return mInputDevice->getMouseRelativeX();
#endif			
		}

		float getRMouseY() {
#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 3
			const MouseState &ms = mMouse->getMouseState();
			return ms.relY;
#else
			return mInputDevice->getMouseRelativeY();
#endif
		}

		Key getMouseButton() {

#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 3
			const MouseState &ms = mMouse->getMouseState();
			
			if (ms.buttons == 1)
				return MOUSE_PRIMARY;
			else if (ms.buttons == 2)
				return MOUSE_ALT;
			else if (ms.buttons == 3)
				return MOUSE_BOTH;
			else
				return MOUSE_NONE;
#else
		Ogre::MouseState st;
		mInputDevice->getMouseState(st);
		
		if (st.Buttons == 1)
			return MOUSE_PRIMARY;
		else if (st.Buttons == 2)
			return MOUSE_ALT;
		else if (st.Buttons == 3)
			return MOUSE_BOTH;
		else
			return MOUSE_NONE;
#endif		
		}

};





class SimpleTutorial : public FrameListener, public FancyInput {

public:

	SimpleTutorial() {
		mTutorialName = "???";
		mTutorialDescription = "???";
		mShutdownNow = false;
	}

	~SimpleTutorial() {}

	bool Run() {

		getSetup();

		mRoot = new Root("","ogre.cfg","ogre.graphics.log");
		mRoot->loadPlugin("RenderSystem_Direct3D9");
		mRoot->loadPlugin("RenderSystem_GL");
		mRoot->loadPlugin("Plugin_ParticleFX");
		
		Ogre::String rs = "Direct3D9 Rendering Subsystem";

		Ogre::RenderSystemList *pRenderSystemList; 
		pRenderSystemList = mRoot->getAvailableRenderers(); 
		Ogre::RenderSystemList::iterator pRenderSystem; 
		pRenderSystem = pRenderSystemList->begin(); 
		Ogre::RenderSystem *pSelectedRenderSystem; 
		pSelectedRenderSystem = *pRenderSystem; 
	
		while (pRenderSystem != pRenderSystemList->end())  {
			
			if ((*pRenderSystem)->getName() == rs) {
				mRenderSystem = *pRenderSystem;
				break;
			}
			
			pRenderSystem++;
		}

		mRoot->setRenderSystem(mRenderSystem);

		

		mRenderSystem->setConfigOption("Full Screen", "No");
		mRenderSystem->setConfigOption("VSync", "No");

		if (rs == "Direct3D9 Rendering Subsystem") {
			mRenderSystem->setConfigOption("Video Mode", 
								   Ogre::StringConverter::toString(1024)
								   + " x " + Ogre::StringConverter::toString(768) 
								   + " @ " + Ogre::StringConverter::toString(32) + "-bit colour"  
			);
			
			mRenderSystem->setConfigOption("Anti aliasing","Level 4");

		}
		else {
			mRenderSystem->setConfigOption("Video Mode", 
								   Ogre::StringConverter::toString(1024)
								   + " x " + Ogre::StringConverter::toString(768) 
			);

			mRenderSystem->setConfigOption("Colour Depth",Ogre::StringConverter::toString(32));
			mRenderSystem->setConfigOption("FSAA", "2");
		}
	
		ConfigFile cf;
		cf.load("media/resources.cfg");

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

		
		mWindow = mRoot->initialise(true, "NxOgre: " + mTutorialName + " - " + mTutorialDescription);

		mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "NxOgre");
		mCamera = mSceneMgr->createCamera("MainCamera");
		mCamera->setPosition(Vector3(0,0,0));
		mCamera->lookAt(Vector3(0,0,0));
		mCamera->setNearClipDistance(0.1);

		mViewport = mWindow->addViewport(mCamera);
		mViewport->setBackgroundColour(ColourValue::Black);
		mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));


		TextureManager::getSingleton().setDefaultNumMipmaps(1);
		MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
		MaterialManager::getSingleton().setDefaultAnisotropy(8); 
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		mOverlay = Ogre::OverlayManager::getSingleton().create("GUI");
		mTargetCaption = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement(
                  "Panel", "Mode"));
		mTargetCaption->setMetricsMode(Ogre::GMM_PIXELS);
		mTargetCaption->setLeft(0);
		mTargetCaption->setTop(0);
		mTargetCaption->setWidth(mViewport->getActualWidth());
		mTargetCaption->setHeight(mViewport->getActualHeight());
		
		if (!MaterialManager::getSingleton().getByName("boot." + mTutorialName).isNull())
			mTargetCaption->setMaterialName("boot." + mTutorialName);
		else
			mTargetCaption->setMaterialName("boot.any");

		mOverlay->add2D(mTargetCaption);
		mTargetCaption->show();
		mOverlay->show();
		mRoot->renderOneFrame();

	
		createInputSys(mWindow);
		
		mMouseMode = PICK;

		mPointer = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement(
                  "Panel", "Pointer"));
		mPointer->setMetricsMode(Ogre::GMM_PIXELS);
		mPointer->setLeft(100);
		mPointer->setTop(100);
		mPointer->setWidth(32);
		mPointer->setHeight(32);
		mPointer->setMaterialName("nx.arrow");
		mOverlay->add2D(mPointer);
		mPointer->hide();
		

		
		mCamera->lookAt(0,0,0);
		
		ApplicationStart();

		mTargetCaption->setCaption("");
		mTargetCaption->hide();
		
		sg = mSceneMgr->createStaticGeometry("grid");
		CreateEntNode("nx.floor2", Vector3(0,-0.01,0));
		CreateEntNode("nx.body.axis", Vector3(0,0,0));
		sg->build();

		mSceneMgr->setFog(FOG_LINEAR, ColourValue::Black, 0.0005f, 4000,4500);
		mSceneMgr->setAmbientLight(ColourValue(0.6,0.6,0.6));
        //mSceneMgr->setShadowTechnique( SHADOWTYPE_STENCIL_ADDITIVE );
		mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
		mSceneMgr->setShadowFarDistance(100);
		mSceneMgr->setShowDebugShadows(true);
		mSceneMgr->setShadowTexturePixelFormat(PF_X8R8G8B8);
		mSceneMgr->setShadowTextureSettings(1024, 2);

		Light* mLight =  mSceneMgr->createLight("SunLight");
		mLight->setPosition( Vector3(150, 100, -150) );
		

		if (mMouseMode == PICK)
			mPointer->show();

		mCamera->setPosition(10,10,10);
		mCamera->lookAt(0,1,0);

		mRoot->addFrameListener(this);
		mRoot->startRendering();

		return true;
	}


	void createPlane(Ogre::String _name, Ogre::String _material, Ogre::Vector3 _pos, Ogre::Vector2 _size, Ogre::Vector2 _subdivisions = Ogre::Vector2(1,1)) {
		Plane _plane;
        _plane.normal = Vector3::UNIT_Y;
        _plane.d = 0;


        MeshManager::getSingleton().createPlane(_name + ".plane",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
            _plane,_size.x,_size.y,1,1,true,1,_subdivisions.x,_subdivisions.y,Vector3::UNIT_Z);
        
		Entity *_entity = mSceneMgr->createEntity(_name + ".entity", _name + ".plane");
        _entity->setMaterialName(_material);
		_entity->setCastShadows(false);
		
		sg->addEntity(_entity, _pos);
		
		
	}

	void CreateEntNode(Ogre::String _name, Ogre::Vector3 _pos) {
		Entity *_entity = mSceneMgr->createEntity(_name + ".entity", _name + ".mesh");
		_entity->setCastShadows(false);
		sg->addEntity(_entity, _pos);
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

		captureInput();

		if (isKeyDown(QUIT))
			mShutdownNow = true;

	
		if (mMouseMode == CAMERA_CONTROL) {
			mRotX = Degree(-getRMouseX() * 0.13);
			mRotY = Degree(-getRMouseY() * 0.13);
			mCamera->yaw(mRotX);
			mCamera->pitch(mRotY);
		}
		else if (mMouseMode == CAMERA_FORCE) {
			mRotX = Degree(-getRMouseX() * 0.13);
			mRotY = Degree(-getRMouseY() * 0.13);
			mCamera->yaw(mRotX);
			mCamera->pitch(mRotY);
		}
		else if (mMouseMode == PICK && getMouseButton() == MOUSE_PRIMARY) {
			mPointer->setPosition(
				mPointer->getLeft() + (getRMouseX()),
				mPointer->getTop() + (getRMouseY())
			);

			mousePick(mPointer->getLeft(), mPointer->getTop());
		}
		else if (mMouseMode == PICK && getMouseButton() == MOUSE_ALT) {
			mPointer->setPosition(
				mPointer->getLeft() + (getRMouseX()),
				mPointer->getTop() + (getRMouseY())
			);
			mousePick(mPointer->getLeft(), mPointer->getTop());
		}
		else if (mMouseMode == PICK && getMouseButton() == MOUSE_BOTH) {

			mRotX = Degree(-getRMouseX() * 0.13);
			mRotY = Degree(-getRMouseY() * 0.13);
			mCamera->yaw(mRotX);
			mCamera->pitch(mRotY);
		}
		else if (mMouseMode == PICK) {
			mPointer->setPosition(
				mPointer->getLeft() + (getRMouseX()),
				mPointer->getTop() + (getRMouseY())
			);

		}

		if (mMouseMode == PICK || mMouseMode == CAMERA_CONTROL) {

			if (isKeyDown(PAN_LEFT))
				mTranslateVector.x = -mMoveScale;

			if (isKeyDown(PAN_RIGHT))
				mTranslateVector.x = mMoveScale;

			if (isKeyDown(PAN_FORWARD))
				mTranslateVector.z = -mMoveScale;

			if (isKeyDown(PAN_BACKWARD))
				mTranslateVector.z = mMoveScale;

			if (isKeyDown(PAN_UP))
				mTranslateVector.y = mMoveScale;

			if (isKeyDown(PAN_DOWN))
				mTranslateVector.y = -mMoveScale;

			mCamera->moveRelative(mTranslateVector);
		}
	
		mMoveScale = 16 * evt.timeSinceLastFrame;
		
		mRotX = 0;
		mRotY = 0;
		mTranslateVector = Vector3::ZERO;


		onFrame(evt.timeSinceLastFrame);
	
		return true;
	}

	enum mouseMode {
		CAMERA_FOLLOW,
		CAMERA_FORCE,
		CAMERA_CONTROL,
		PICK
	};

	void setMouseMode(mouseMode mm) {

		
	}

	Ogre::String mmToString(mouseMode mm) {
		switch(mm) {
			case CAMERA_FOLLOW:
				return "Camera (Following)";
			break;

			case CAMERA_CONTROL:
				return "Camera (Free)";
			break;

			case PICK:
				return "Mouse picking";
			break;
		};

		return "?";
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
	virtual void mousePick(float x, float y) = 0;

	virtual void newFrame(float _time) = 0;

	Ogre::Root* mRoot;
	Ogre::RenderSystem* mRenderSystem;
	Ogre::Camera* mCamera;
	Ogre::RenderWindow* mWindow;
	Ogre::Viewport* mViewport;
	Ogre::SceneManager* mSceneMgr;

	bool mShutdownNow;
	Vector3 mTranslateVector;
	float mMoveScale;
	Radian mRotX;
	Radian mRotY;

	mouseMode mMouseMode;
	Ogre::Overlay* mOverlay;
	Ogre::OverlayContainer* mPointer;
	Ogre::OverlayContainer* mTargetCaption;
	Ogre::StaticGeometry* sg;

};



#endif