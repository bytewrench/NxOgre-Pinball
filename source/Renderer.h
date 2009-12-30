#ifndef _RENDERER

#define _RENDERER

#include "stdafx.h"

class Renderer :
    public Ogre::FrameListener
    {
        public:

            Renderer(LPSTR commandLine );
            ~ Renderer();

            Ogre::RenderWindow * createWindow ( const Ogre::String & name );
            Ogre::SceneManager * createSceneMgr ( const Ogre::String & name );
            void createViewportAndBindToCamera(Ogre::Camera *);
			void processArgs (int argc, char * argv [] );
            //void bind( InputHandler * ipHand);
            void start();
            void stop();
			int ParseCommandline();
            bool frameStarted(const Ogre::FrameEvent & evt);
            bool frameEnded(const Ogre::FrameEvent & evt);
            Ogre::Root * getRoot ();
            Ogre::RenderWindow * getWindow ();
            int getWidth();
            int getHeight();
			//InputHandler * mInputHandler;
			Ogre::RenderWindow * mWindow;
			
			void SetRenderParam(String paramName,String paramVal);
        protected:

            //GameEngine * mGame;
            //PinballGame * mPinball;
             RenderSystem * mRenderSystem;
            Ogre::Root * mRoot;
            Ogre::Viewport * mViewport;
            bool videoConfigHandled;
            
            bool mShutdown;

    };

#endif
