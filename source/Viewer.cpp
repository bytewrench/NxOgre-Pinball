#include "stdafx.h"



Viewer::Viewer ( Renderer * r)
    {
        mSceneMgr = r->createSceneMgr("Main");
        mCamera = mSceneMgr->createCamera("Main");
        mCamera->setNearClipDistance(5.0f);
        mCamera->setFarClipDistance(100.0f);
		mCamera->setRenderingDistance(100);
		Quaternion q;
        q.w = .90;
        q.x = - .377;
        q.y = 0;
        q.z = 0;
        mCamera->setPosition(0, 15.95, 21.55);
        mCamera->setOrientation(q);
		
		
    }

void Viewer::setupLights ()
    {
        mSceneMgr->setAmbientLight(ColourValue(0.8, 0.8, 0.8));

		
		if ( Ogre::Root::getSingletonPtr()->getRenderSystem()->getCapabilities()->hasCapability(RSC_HWRENDER_TO_TEXTURE) )
		{
			mSceneMgr->setShadowTextureSettings(1024, 2);
		}
		else
		{
			mSceneMgr->setShadowTextureSettings(512, 2);
		}
		
		
		mSceneMgr->setShadowColour(ColourValue(0.5,0.5,0.5));

        Light * mSunLight = mSceneMgr->createLight("SunLight");
		
		mSunLight->setCastShadows(true);

        mSunLight->setType(Light::LT_SPOTLIGHT);
        mSunLight->setPosition(500, 500, 500);
        mSunLight->setSpotlightRange(Degree(30), Degree(50));
        Vector3 dir;
        dir = - mSunLight->getPosition();
        dir.normalise();
        mSunLight->setDirection(dir);
        mSunLight->setDiffuseColour(0.35, 0.35, 0.38);
        mSunLight->setSpecularColour(0.9, 0.9, 1);

		timeSince = 0.0f;
        mBuildMode = BM_NONE;
		
#if 0

        mReflectCam = mSceneMgr->createCamera("ReflectCam");

        mRTTTex = mRenderer->getRoot()->getRenderSystem()->createRenderTexture("RttTex",    512, 384,
                                                                               TEX_TYPE_2D, PF_R8G8B8);

        {
        mReflectCam = mSceneMgr->createCamera("ReflectCam");
        mReflectCam->setPosition(mCamera->getPosition());
        mReflectCam->setOrientation(mCamera->getOrientation());
        mReflectCam->setNearClipDistance(mCamera->getNearClipDistance());
        mReflectCam->setFarClipDistance(mCamera->getFarClipDistance());

        Viewport * v = mRTTTex->addViewport(mReflectCam);
        mReflectCam->setAspectRatio(Real(v->getWidth()) / Real(v->getHeight()));
        v->setOverlaysEnabled(false);
        v->setClearEveryFrame(true);
        v->setBackgroundColour(ColourValue::Black);
        }

        MaterialPtr mat = MaterialManager::getSingleton().create("RttMat",
                                                                 ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        TextureUnitState * t = mat->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex");
        t->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
        //t->setProjectiveTexturing(true, mReflectCam);

        mat->clone("RttMat.over");

        //mGui->createWindow(Vector4(16, 16, 512, 384), "RttMat", BetaGUI::WFT_NONE, "");

#endif

    }


void Viewer::preStart ( World * w )
    {
        mWorld = w;
        mScene = mWorld->getScenes()->getFirst();
        //mRay = new RayCaster(mCamera->getPosition(), Vector3(0, 0, 0), 100, RayCaster::RCT_CLOSEST, mScene);
        //mClothRay = new ClothRayCaster(mScene);
        mHalfRenderWidth = mRenderer->getWindow()->getWidth() / 2;
        mHalfRenderHeight = mRenderer->getWindow()->getHeight() / 2;
        //mGame->mTargetActor = 0;
        //mGame->mTargetCloth = 0;
    }

void Viewer::cleanup ()
    {
        // Grid

        mSceneMgr->removeRenderQueueListener(mHighLightListener);
        delete mHighLightListener;
        mHighLightListener = 0;

    }

void Viewer::onFrame ( float deltaTime )
    {
        //ms = mInputHandler->getMouseState();
		mInputHandler->getInputHandler()->onFrame(deltaTime);
        timeSince += deltaTime;
    }


Actor * Viewer::cast ( Ogre::Vector3 & hitpos )
    {
        mOgreRay = mCamera->getCameraToViewportRay(float(mCursor->getLeft()
                                                             / mRenderer->getWindow()->getWidth()),
                                                   float(mCursor->getTop()
                                                             / mRenderer->getWindow()->getHeight()));

        mRay->setOrigin(mOgreRay.getOrigin());
        mRay->setDirection(mOgreRay.getDirection());

        if ( mRay->castShape(RayCaster::AF_NONE) )
            {
                hitpos = mRay->getClosestRaycastHit().mWorldImpact;
                return mRay->getClosestActor();
            }

        else
        {
            return 0;
        }
    }

Ogre::Vector2 Viewer::worldToScreen ( const Ogre::Vector3 & worldPos )
    {
        Ogre::Vector3 hcsPosition = mCamera->getProjectionMatrix() * (mCamera->getViewMatrix() * worldPos);
        Ogre::Vector2 p;

        if ( hcsPosition.x < - 1.0f )
            p.x = 0;

        else if ( hcsPosition.x > 1.0f )
        p.x = (mHalfRenderWidth*2)+1;

        else
            p.x = mHalfRenderWidth + (mHalfRenderWidth * hcsPosition.x);

        if ( hcsPosition.y < - 1.0f )
            p.y = 0;

        else if ( hcsPosition.y > 1.0f )
        p.y = (mHalfRenderHeight*2)+1;

        else
        p.y = mHalfRenderHeight + (mHalfRenderHeight * -hcsPosition.y);

        return p;
    }

Ogre::Camera * Viewer::getCamera ()
{
    return mCamera;
}

Ogre::SceneManager * Viewer::getSceneManager ()
{
    return mSceneMgr;
}
