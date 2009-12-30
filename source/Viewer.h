#ifndef _VIEWER

#define _VIEWER

#include "stdafx.h"

class HighlightQueueListener :
	public Ogre::RenderQueueListener
{
public:

	virtual void renderQueueStarted ( Ogre::uint8 queueGroupId, const Ogre::String & invocation,
		bool & skipThisInvocation )
	{
		//RenderQueue containing the object to be highlighted
		if ( queueGroupId == 90 )
		{
			Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem();

			rendersys->clearFrameBuffer(Ogre::FBT_STENCIL);
			rendersys->setStencilCheckEnabled(true);
			rendersys->setStencilBufferParams(Ogre::CMPF_ALWAYS_PASS, 1,
				0xFFFFFFFF,             Ogre::SOP_KEEP,
				Ogre::SOP_KEEP,         Ogre::SOP_REPLACE,
				false);
		}

		//RenderQueue containing the outline
		if ( queueGroupId == 91 )
		{
			Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem();

			rendersys->setStencilCheckEnabled(true);
			rendersys->setStencilBufferParams(Ogre::CMPF_NOT_EQUAL, 1,
				0xFFFFFFFF,           Ogre::SOP_KEEP,
				Ogre::SOP_KEEP,       Ogre::SOP_KEEP,
				false);
		}
	}

	virtual void renderQueueEnded ( Ogre::uint8 queueGroupId, const Ogre::String & invocation,
		bool & repeatThisInvocation )
	{
		if ( (queueGroupId == 90) || (queueGroupId == 91) )
		{
			Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem();
			rendersys->setStencilCheckEnabled(false);
			rendersys->setStencilBufferParams();
		}
	}
};

class Viewer 
    {
        public:

            Viewer(Renderer * r);
            virtual void preStart(World *);
            virtual void setupLights();
            virtual void cleanup();
            virtual void onFrame(float deltaTime);
            

            Actor * cast ( Ogre::Vector3 & hitpos );
            Ogre::Camera * getCamera ();
            Ogre::SceneManager * getSceneManager ();

            World * mWorld;
            Scene * mScene;

        protected:


            Ogre::SceneManager * mSceneMgr;
            Ogre::Camera * mCamera;
            Renderer * mRenderer;
            InputHandler * mInputHandler;
            //PinballGame * mGame;

            // Grid
            Ogre::StaticGeometry * mStaticGeom;


            Ogre::OverlayContainer * mCursor;

            // Raycasting.
            Ray mOgreRay;
            RayCaster * mRay;
            ClothRayCaster * mClothRay;

            int mHalfRenderWidth;
            int mHalfRenderHeight;
            HighlightQueueListener * mHighLightListener;

            Ogre::Vector2 worldToScreen(const Ogre::Vector3 & worldPos);

            // Mouse and Frame
            //OIS::MouseState ms;
            NxReal timeSince;
            bool RMB;

            // Camera movement.		
            Ogre::Vector3 mTranslateVector;
            float mMoveScale;

            // Building stuff
            enum buildmode
                {
                    BM_NONE,
                    BM_MENU,
                    BM_GUI,
                    BM_ACTOR_MENU,
                    BM_ACTOR,
                    BM_JOINT_MENU,
                    BM_JOINT,
                    BM_FX_MENU,
                    BM_FX
                };

            buildmode mBuildMode;
            SceneNode * mBuildNode;
            SceneNode * mGlowNode;
            Ogre::String mBuildMesh;
            Camera * mReflectCam;
            std::map<unsigned int, Ogre::String> mBuildMaterials;
            int mCurrentBuildMaterial;

            bool mBuildScaleModeAllOnly;
            int mBuildRotMode;
            int mBuildScaleMode;

            // Screenshot System.
            RenderTexture * mRTTTex;
    };

#endif
