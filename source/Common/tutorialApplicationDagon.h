/*

	Tutorials

*/

#ifndef __simple_tutorial_eihort_H__
#define __simple_tutorial_eihort_H__

#include "Ogre.h"
#include "OgreConfigFile.h"
#include "OgreKeyEvent.h"
#include "OgreEventListeners.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

using namespace Ogre;
using namespace nxOgre;



class SimpleTutorialDagon : public FrameListener {

	public:

		// Enum's
		enum Key {
			QUIT,SCREENSHOT,DEBUG_MODE,RESET_SCENE, PAUSE, SLOWER, FASTER,
			OPTION_1,OPTION_2,OPTION_3,OPTION_4,
			PAN_FORWARD,PAN_BACKWARD,PAN_LEFT,PAN_RIGHT,PAN_UP,PAN_DOWN,
			X,NEG_X,Y,NEG_Y,Z,NEG_Z,
			MOUSE_PRIMARY,MOUSE_ALT,MOUSE_NONE,MOUSE_BOTH
		};

		enum mouseMode {
			CAMERA_FOLLOW,CAMERA_FORCE,CAMERA_CONTROL,PICK
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		SimpleTutorialDagon() {
			mTutorialName = "???";
			mTutorialDescription = "???";
			mShutdownNow = false;
			mMouseMode = PICK;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		~SimpleTutorialDagon() {
			std::cout << "Bye bye." << std::endl;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Run() {

				getSetup();

				resetConfig();
				getConfig();

				setupRenderSystem();
			
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
								
				Ogre::String loadingBackMat = "boot.any";

				if (!MaterialManager::getSingleton().getByName("boot." + mTutorialName).isNull())
					loadingBackMat = "boot." + mTutorialName;
				
				OverlayElement* loadingBack = createOverlay("loadBack",Vector2(0,0),Vector2(mViewport->getActualWidth(),mViewport->getActualHeight()),loadingBackMat);		
				OverlayElement* loading = createOverlay("load",Vector2((mWindow->getWidth() / 2) - 64,(mWindow->getHeight() / 2) - 16),Vector2(128,32),"gui_loading");

				mOverlay->show();


				mRoot->renderOneFrame();
				
				loading->hide();
				loadingBack->hide();
				
				createInputSys(mWindow);
				
				
				

				mPointer = createOverlay("Pointer", Vector2((mWindow->getWidth() / 2) - 16, (mWindow->getHeight() / 2) - 16), Vector2(32,32), "nx.arrow");
				mPointer->hide();		

				gui_pause = createOverlay("paused", Vector2(16, mWindow->getHeight() - 100 - 32), Vector2(128,128), "gui_pause");
				gui_pause->hide();

				gui_slowfast = createOverlay("slowfast", Vector2(16 + 100, mWindow->getHeight() - 100 - 32), Vector2(128,128), "", "0");
				gui_slowfast->hide();
				
				mCamera->lookAt(0,0,0);
				
				mCaption1 = createOverlay("caption.1",Vector2(8,128),Vector2(16,256),""," ");
				mCaption2 = createOverlay("caption.2",Vector2(8,128 + 16),Vector2(16,256),""," ");
				mCaption3 = createOverlay("caption.3",Vector2(8,mWindow->getHeight() - 16 - 8),Vector2(16,256),""," ");
				
				ApplicationStart();
				
				sg = mSceneMgr->createStaticGeometry("grid");
					CreateEntNode("nx.floor", Vector3(0,-0.01,0));
					CreateEntNode("nx.body.axis", Vector3(0,0,0));
				sg->build();

				mSceneMgr->setFog(FOG_LINEAR, ColourValue::Black, 0.0005f, 4000,4500);
				mSceneMgr->setAmbientLight(ColourValue(0.6,0.6,0.6));
		       
				mSceneMgr->setShadowTechnique( SHADOWTYPE_STENCIL_ADDITIVE );

				/*
					mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
					mSceneMgr->setShadowFarDistance(100);
					mSceneMgr->setShowDebugShadows(true);
					mSceneMgr->setShadowTexturePixelFormat(PF_X8R8G8B8);
					mSceneMgr->setShadowTextureSettings(1024, 2);
				*/

				Light* mLight =  mSceneMgr->createLight("SunLight");
				mLight->setPosition( Vector3(150, 100, -150) );
				

				if (mMouseMode == PICK)
					mPointer->show();


				mRoot->addFrameListener(this);
				
				mRoot->startRendering();

				return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void setupRenderSystem() {
			mRoot = new Root("","","ogre.graphics.log");
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

			mRenderSystem->setConfigOption("Full Screen", settings["fullscreen"]);
			mRenderSystem->setConfigOption("VSync", settings["vsync"]);

			if (rs == "Direct3D9 Rendering Subsystem") {
				mRenderSystem->setConfigOption("Video Mode", 
									   settings["width"]
									   + " x " +  settings["height"]
									   + " @ " +  settings["depth"] + "-bit colour"  
				);
				
				//mRenderSystem->setConfigOption("Anti aliasing","Level 4");

			}
			else {
				mRenderSystem->setConfigOption("Video Mode", settings["width"]   + " x " + settings["height"]
				);

				mRenderSystem->setConfigOption("Colour Depth",settings["depth"]);
				//mRenderSystem->setConfigOption("FSAA", "2");
			}

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void resetConfig() {

			settings.clear();
			settings["device"] = "Direct3D9 Rendering Subsystem";
			settings["width"] = "800";
			settings["height"] = "600";
			settings["depth"] = "32";
			settings["aa"] = "0";
			settings["vsync"] = "No";
			settings["fullscreen"] = "No";

			keys.clear();
			keys[QUIT] = KC_ESCAPE;
			keys[SCREENSHOT] = KC_F1;
			keys[DEBUG_MODE] = KC_F2;
			keys[PAUSE] = KC_F3;
			keys[SLOWER] = KC_F4;
			keys[FASTER] = KC_F5;
			keys[RESET_SCENE] = KC_F12;
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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void getConfig() {

			ConfigFile cf;
			cf.load("config.yaml",":",true);

			ConfigFile::SectionIterator seci = cf.getSectionIterator();
			String secName, typeName, archName;
			
			while (seci.hasMoreElements()) {
				secName = seci.peekNextKey();
				ConfigFile::SettingsMultiMap *csettings = seci.getNext();
				ConfigFile::SettingsMultiMap::iterator i;

				for (i = csettings->begin(); i != csettings->end(); ++i) {
					
					if (i->first == "device") {
						if (i->second == "dx9") {
							settings["device"] = "Direct3D9 Rendering Subsystem";
						}
						else {
							settings["device"] = "OpenGL Rendering Subsystem"; // Check..
						}
					}
					else if (i->first == "width") {
						settings["width"] = Ogre::StringConverter::toString(Ogre::StringConverter::parseUnsignedInt(i->second));
					}
					else if (i->first == "height") {
						settings["height"] = Ogre::StringConverter::toString(Ogre::StringConverter::parseUnsignedInt(i->second));
					}
					else if (i->first == "depth") {
						settings["depth"] = Ogre::StringConverter::toString(Ogre::StringConverter::parseUnsignedInt(i->second));
					}
					else if (i->first == "aa") {
						settings["aa"] = Ogre::StringConverter::toString(Ogre::StringConverter::parseUnsignedInt(i->second));
					}
					else if (i->first == "fullscreen") {
						settings["fullscreen"] = i->second;
					}
					else if (i->first == "vsync") {
						settings["aa"] = i->second;
					}

				}
			}
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void createInputSys(RenderWindow *_window) {
			mInputDevice = PlatformManager::getSingleton().createInputReader();
			mInputDevice->initialise(_window,true, true);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void captureInput() {
			mInputDevice->capture();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool isKeyDown(Key k) {
			return mInputDevice->isKeyDown(keys[k]);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		float getRMouseX() {
			return mInputDevice->getMouseRelativeX();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		float getRMouseY() {
			return mInputDevice->getMouseRelativeY();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		float getRMouseZ() {
			return mInputDevice->getMouseRelativeZ();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		Key getMouseButton() {

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

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Ogre::OverlayContainer* createOverlay(Ogre::String name, Vector2 position, Vector2 dimensions, Ogre::String material = "", Ogre::String caption = "") {
			std::cout << name << std::endl;

			Ogre::String type = "Panel";
			if (caption != "") {
				type = "TextArea";
			}

			Ogre::OverlayContainer* e = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement(type, name));

			e->setMetricsMode(Ogre::GMM_PIXELS);
			e->setLeft(position.x);
			e->setTop(position.y);
			e->setWidth(dimensions.x);
			e->setHeight(dimensions.y);

			if (material != "")
				e->setMaterialName(material);
			
			if (caption != "") {
				e->setCaption(caption);
				e->setParameter("font_name", "nxogrefont");
				e->setParameter("char_height", "16");
				e->setParameter("horz_align", "left");
			}

			mOverlay->add2D(e);
			e->show();

			return e;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		void CreateEntNode(Ogre::String _name, Ogre::Vector3 _pos) {
			Entity *_entity = mSceneMgr->createEntity(_name + ".entity", _name + ".mesh");
			_entity->setCastShadows(false);
			sg->addEntity(_entity, _pos);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		void Shutdown() {
			ApplicationStop();
			mRoot->shutdown();
			delete mRoot;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

				mouseMoveOrGrab(mPointer->getLeft(), mPointer->getTop(), true);
			}
			else if (mMouseMode == PICK && getMouseButton() == MOUSE_ALT) {
				mPointer->setPosition(
					mPointer->getLeft() + (getRMouseX()),
					mPointer->getTop() + (getRMouseY())
				);

				mouseMoveOrGrab(mPointer->getLeft(), mPointer->getTop(), false);

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

			// Let the tutorial have some fun.
			timeSince += evt.timeSinceLastFrame;
			
			newFrame(evt.timeSinceLastFrame);
			
			if (isKeyDown(DEBUG_MODE) && timeSince >= 0.25f) {
				world::getSingleton().showDebugOverlay(mCamera, mWindow);
				world::getSingleton().debug( world::getSingleton().isDebugRendering() ^ 1);
				timeSince = 0.0f;
			}

			if (isKeyDown(SCREENSHOT) && timeSince >= 0.25f) {
				mWindow->writeContentsToFile("screenshot." + Ogre::StringConverter::toString(Ogre::Math::Floor(mRoot->getTimer()->getMilliseconds() / 1000)) + ".png");
				timeSince = 0.0f;
			}

			if (isKeyDown(FASTER) && timeSince >= 0.25f) {
				speed *= 2;
				world::getSingleton().setTimeModifer(speed);


				timeSince = 0.0f;

				if (speed > 4)
					speed = 4;
				
				if (speed != 0) {
					gui_slowfast->show();
					gui_slowfast->setCaption(Ogre::StringConverter::toString(speed));
				} else
					gui_slowfast->hide();

			}

			if (isKeyDown(SLOWER) && timeSince >= 0.25f) {
				speed /= 2;
				world::getSingleton().setTimeModifer(speed);
				timeSince = 0.0f;

				if (speed < 0.25)
					speed = 0.25;

				if (speed != 0) {
					gui_slowfast->show();
					gui_slowfast->setCaption(Ogre::StringConverter::toString(speed));
				} else
					gui_slowfast->hide();

			}

			if (isKeyDown(PAUSE) && timeSince >= 0.5f) {

				if (nxOgre::world::getSingleton().isPaused()) {
					world::getSingleton().resume();
					gui_pause->hide();
				}
				else {
					world::getSingleton().pause();
					gui_pause->show();
				}
				timeSince = 0.0f;

			}


			if (firstFrame) {

				if (mTutorialName == "101") {
					mCamera->setPosition(10,10,10);
					mCamera->lookAt(0,1,0);
				}

				if (mMouseMode == PICK) {
					mCamera->setPosition(10,10,10); 
					mCamera->lookAt(0,1,0);
				}
				firstFrame = false;
			}

			if (hasTargetBody == false) {
				targetNode->setVisible(false);
			} else {
				Vector3 p = targetBody->getGlobalPosition();
				p.y = 0.01;
				targetNode->setPosition(p);	
				targetNode->setVisible(true);
			}
			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool frameEnded(const FrameEvent& evt) {
			
			if (mShutdownNow)
				return false;
				
			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void ApplicationStart() {

			mCamera->setPosition(10,10,10);
			mCamera->lookAt(0,0,0);

			targetBody = 0;
			hasTargetBody = false;

			targetNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("target");
			Entity *_entity =  mSceneMgr->createEntity("nx.bodyguide.ent", "nx.bodyguide.mesh");
			_entity->setCastShadows(false);
			targetNode->attachObject(_entity);
			targetNode->scale(0.5,0.5,0.5);
			targetNode->setVisible(false);
			
			firstFrame = true;
			timeSince = 0;
			speed = 1;

			prestart();
			start();

			mRayCaster = new rayCaster(Vector3::ZERO, Vector3::ZERO, world::getSingleton().getScene("Main"), 200);
			
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void ApplicationStop() {
			prestop();

			delete mRayCaster;

			stop();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void getSetup() {
			getTutorialSettings();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void mouseMoveOrGrab(float x, float y, bool move_or_grab) {

			Ogre::Ray r = mCamera->getCameraToViewportRay(
				float(x / mWindow->getWidth()),
				float(y / mWindow->getHeight())
			);

			mRayCaster->setOrigin(r.getOrigin());
			mRayCaster->setDirection(r.getDirection());

			if (!mRayCaster->cast()) {
				return;
			}

			// Move it
			if (move_or_grab) {
				
				if (hasTargetBody) {

					Ogre::Plane p;
					p.normal = r.getDirection();
					p.d = -mRayCaster->mHitPos.dotProduct(p.normal);

					std::pair<bool, Real> res;
					res = r.intersects(p);
					Vector3 objPoint;

					if (res.first) {

						Vector3 force = r.getPoint(res.second) - targetBody->getGlobalPosition();
						force *= 10;
						force -= NxTools::convert(targetBody->mActor->getPointVelocity(NxTools::convert(targetBody->getGlobalPosition())));
						targetBody->addForceAtPos(force, targetBody->getGlobalPosition());

					}

				}

				return;
			}

			// Grab it
			if (!move_or_grab) {

				body *t = mRayCaster->mHitBody;

				if (t->isStatic()) {
					targetBody = 0;
					hasTargetBody = false;
					return;
				}

				// Can't add forces to Kinematics.
				if (t->isKinematic())
					return;

				if (t == targetBody)
					return;

				targetBody = t;
				hasTargetBody = true;

			}

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void getTutorialSettings() = 0;
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void prestart() = 0;
		virtual void prestop() = 0;
		virtual void newFrame(float _time) = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// Tutorial bits
		Ogre::String mTutorialName;
		Ogre::String mTutorialDescription;
		bool mShutdownNow;
		bool firstFrame;
		float timeSince;

		// Ogre pointer bits
		Ogre::Root* mRoot;
		Ogre::RenderSystem* mRenderSystem;
		Ogre::Camera* mCamera;
		Ogre::RenderWindow* mWindow;
		Ogre::Viewport* mViewport;
		Ogre::SceneManager* mSceneMgr;
		int winHeight;
		int winWidth;

		// Gui bits
		OverlayElement* gui_pause;
		OverlayElement* gui_slowfast;

		// Settings
		std::map<Ogre::String, Ogre::String> settings;
		std::map<Key, KeyCode> keys;
		float speed;

		// Mouse movement bits.
		Vector3 mTranslateVector;
		float mMoveScale;
		Radian mRotX;
		Radian mRotY;
		mouseMode mMouseMode;

		// Overlay, Pointer and GUI bits
		Ogre::Overlay* mOverlay;
		Ogre::OverlayContainer* mPointer;
		Ogre::OverlayContainer* mTargetCaption;
		Ogre::StaticGeometry* sg;

		// Mousepicker and Raycasting bits.
		body *targetBody;
		SceneNode* targetNode;
		bool hasTargetBody;
		rayCaster* mRayCaster;
		


		// Input bits
		InputReader* mInputDevice;

		//
		Ogre::OverlayContainer* mCaption1;
		Ogre::OverlayContainer* mCaption2;
		Ogre::OverlayContainer* mCaption3;


		

};



#endif