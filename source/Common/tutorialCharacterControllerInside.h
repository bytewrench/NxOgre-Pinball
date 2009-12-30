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

class CharacterInsideTutorial : public FrameListener {

public:

	CharacterInsideTutorial() {
		mTutorialName = "???";
		mTutorialDescription = "???";
		mShutdownNow = false;
	}

	~CharacterInsideTutorial() {}

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
		mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "NxOgre");
		
		mCamera = mSceneMgr->createCamera("MainCamera");
		mCamera->setPosition(Vector3(0,0,0));
		mCamera->lookAt(Vector3(0,0,0));
		mCamera->setNearClipDistance(0.1);

		mViewport = mWindow->addViewport(mCamera);
		mViewport->setBackgroundColour(ColourValue(1,1,1));
		mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));


		TextureManager::getSingleton().setDefaultNumMipmaps(1);
		MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
		MaterialManager::getSingleton().setDefaultAnisotropy(8); 

		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();


		mInputDevice = PlatformManager::getSingleton().createInputReader();
        mInputDevice->initialise(mWindow,true, true);
		mRoot->addFrameListener(this);


		mSceneMgr->setAmbientLight(ColourValue(0.6,0.6,0.6));

		mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE );

		Light* mSunLight =  mSceneMgr->createLight("SunLight");
		mSunLight->setType( Light::LT_POINT );
		mSunLight->setPosition( Vector3(-300, 160, 0) );

		ApplicationStart(); 
		
		mRoot->startRendering();


		return true;
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
	float mRotZ;
	float mLastRotZ;
};



#endif