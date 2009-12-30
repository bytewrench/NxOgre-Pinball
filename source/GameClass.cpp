//#include "Viewer.h"
//#include "GameClass.h"
#include "stdafx.h"

GameEngine::GameEngine ()
    {
        
    }

GameEngine::~GameEngine ()
{
    delete mRenderer;
}



void GameEngine::pre (  LPSTR lpCmdLine )
    {
        
		mRenderer = new Renderer(lpCmdLine);

		LogManager::getSingletonPtr()->logMessage("Renderer is set up");

        Ogre::RenderWindow * window = mRenderer->createWindow("");
        LogManager::getSingletonPtr()->logMessage("Window created");
        mViewer = new Viewer(mRenderer);

        mSceneMgr = mViewer->getSceneManager();
        mCamera = mViewer->getCamera();
        mRenderer->createViewportAndBindToCamera(mCamera);
        InputHandler::getInputHandler()->start(window, mCamera);

        TextureManager::getSingleton().setDefaultNumMipmaps(1);
        MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
        MaterialManager::getSingleton().setDefaultAnisotropy(8);
        ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

			LogManager::getSingletonPtr()->logMessage("Init Lights");
        mViewer->setupLights();
		LogManager::getSingletonPtr()->logMessage("GameEngine Pre Complete");

        showFps = false;
        if(window->getWidth() != (unsigned int)640 || window->getHeight() != (unsigned int)480)
            showSlowFrameWarning=true;
        else
            showSlowFrameWarning=false;
    }

void GameEngine::post ()
    {
        mRenderer->getWindow()->removeAllViewports();
        mRenderer->getRoot()->destroySceneManager(mSceneMgr);
    }

void GameEngine::getConfig ()
    {
    }

void GameEngine::onFramePre ( float deltaTime )
{
    mViewer->onFrame(deltaTime);
}

void GameEngine::stop () { }

void GameEngine::initNxOgre ()
    {

        SceneParams sp;
        sp.setToDefault();
        sp.floor = true;
        float tableSlope = physicsVars::getSingleton()->TableSlope;
        sp.gravity = NxVec3(0, - 9, tableSlope);

        sp.max_iter = 4;
        sp.max_timestep = 1.0f / 75.0f;
        sp.defaultMaterialDynamicFriction = 0.0f;
        sp.defaultMaterialStaticFriction = 0.0f;
        sp.defaultMaterialRestitution = .5f;
        sp.time_step_method = SceneParams::CN_FIXED;

        //LogManager::getSingletonPtr()->logMessage(LML_NORMAL, "Creating Scene");
        mScene = mWorld->createScene("Main", mSceneMgr, sp);

        mCamera = mSceneMgr->createCamera("MainCamera");
        
		//mWorld->getRoot()->setFrameSmoothingPeriod(.5); -rm

        // the binary server is in your 7.2.3/bin folder//
        //mWorld->getPhysXDriver()->createDebuggerConnection("127.0.0.1");

        mWorld->getPhysXDriver()->setTimeModifier(3.0);
        mWorld->getSDK()->setParameter(NX_SKIN_WIDTH, 0.001f);
		//mWorld->getSDK()->setParameter(NX_CONTINUOUS_CD,TRUE);
		//mWorld->getSDK()->setParameter(NX_CCD_EPSILON, 0.01);
       // LogManager::getSingletonPtr()->logMessage(LML_NORMAL, "Done");
    }

World * GameEngine::getWorld ()
{
    return 0;
}

Ogre::OverlayContainer * GameEngine::createOverlay ( Ogre::String name,  Vector2 position,
                                                     Vector2 dimensions, Ogre::String material,
                                                     Ogre::String caption )
    {
        std::cout << name << std::endl;

        Ogre::String type = "Panel";

        if ( caption != "" )
        {
            type = "TextArea";
        }

        Ogre::OverlayContainer * e = static_cast<
                                         Ogre::OverlayContainer *>(Ogre::OverlayManager::getSingleton().createOverlayElement(type,
                                                                                                                             name));

        e->setMetricsMode(Ogre::GMM_PIXELS);
        e->setLeft(position.x);
        e->setTop(position.y);
        e->setWidth(dimensions.x);
        e->setHeight(dimensions.y);

        if ( material != "" )
            e->setMaterialName(material);

        if ( caption != "" )
            {
                e->setCaption(caption);
                e->setParameter("font_name", "nxogrefont");
                e->setParameter("char_height", "16");
                e->setParameter("horz_align", "left");
            }

        mOverlay->add2D(e);
        e->show();

        return e;
    }
