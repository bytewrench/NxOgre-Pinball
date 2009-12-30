#include "stdafx.h"
#include <shellapi.h>
#include "AnyOption.h"

char * * argv = NULL;

int Renderer::ParseCommandline ()
    {
        int argc, BuffSize, i;
        WCHAR * wcCommandLine;
        LPWSTR * argw;

        // Get a WCHAR version of the parsed command line
        wcCommandLine = GetCommandLineW();
        argw = CommandLineToArgvW(wcCommandLine, & argc);

        // Create the first dimension of the double array
        argv = (char * * )GlobalAlloc(LPTR, argc + 1);

        // convert each line of wcCommandeLine to MultiByte and place them
        // to the argv[] array
        for ( i = 0; i < argc; i ++ )
            {
                BuffSize = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, argw[i], - 1, NULL, 0, NULL, NULL);
                argv[i] = (char * )GlobalAlloc(LPTR, BuffSize);
                WideCharToMultiByte(CP_ACP,   WC_COMPOSITECHECK, argw[i], BuffSize * sizeof(WCHAR), argv[i],
                                    BuffSize, NULL,              NULL);
            }

        // return the number of argument
        return argc;
    }

Renderer::Renderer ( LPSTR commandLine )
    {
        char logFile[260] = {0};
        char vidConfigFile[260] = {0}; //TODO: video config used to write to program files directory.  this is bad practice and a temporary fix
        GetTempPath(sizeof(logFile),logFile);
        GetTempPath(sizeof(vidConfigFile),vidConfigFile);
        strcat_s(logFile,sizeof(logFile),"tg_game_errors.txt");
        strcat_s(vidConfigFile,sizeof(vidConfigFile),"video.cfg");
        #ifdef _DEBUG
        mRoot = new Root("plugins_d.cfg", vidConfigFile, logFile);
        #else
        mRoot = new Root("plugins.cfg", vidConfigFile, logFile);
        #endif
        videoConfigHandled=false;

        RenderSystemList* renderList = mRoot->getAvailableRenderers();
        mRoot->setRenderSystem(renderList->at(0)); //only one listed is dx9 renderer
        mRenderSystem = renderList->at(0);

        

        int argc = 0;
        argc = ParseCommandline();
        processArgs(argc, argv);
        LogManager::getSingletonPtr()->logMessage("command line processed");

        if(!videoConfigHandled)
        {
            mRoot->restoreConfig();
            LogManager::getSingletonPtr()->logMessage("video config restored");
        }
        else
        {
            LogManager::getSingletonPtr()->logMessage("restore config skipped, showed dialog");
        }


        LogManager::getSingletonPtr()->logMessage("setting up physics params");
        PhysXDriverParams params;
        params.logtype = PhysXDriverParams::LT_NONE;
        params.useFrameListener = false;
        params.shutdownOnErrors = false;

        LogManager::getSingletonPtr()->logMessage("creating NxWorld");
        mWorld = new World(params);
        LogManager::getSingletonPtr()->logMessage("NxWorld created");

        ConfigFile cf;
        cf.load("media/resources.cfg");
        LogManager::getSingletonPtr()->logMessage("loaded resources config");

        ConfigFile::SectionIterator seci = cf.getSectionIterator();
        String secName, typeName, archName;

        LogManager::getSingletonPtr()->logMessage("iterating over elements");
        while ( seci.hasMoreElements() )
            {
                secName = seci.peekNextKey();
                ConfigFile::SettingsMultiMap * settings = seci.getNext();
                ConfigFile::SettingsMultiMap::iterator i;

                for ( i = settings->begin(); i != settings->end(); ++ i )
                    {
                        typeName = i->first;
                        archName = i->second;
                        ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
                    }
            }
        LogManager::getSingletonPtr()->logMessage("parsed resources config successfully");


    }

void Renderer::processArgs ( int argc, char * argv [] )
    {
        AnyOption * opt = new AnyOption();
        opt->setFlag("standalone", 's');
        opt->setFlag("configwindow", 'c');

        LogManager::getSingletonPtr()->logMessage("processCommandArgs");
        opt->processCommandArgs(argc, argv);


        if ( opt->getFlag("configwindow") || opt->getFlag('c') )
            {
                videoConfigHandled=true;
                LogManager::getSingletonPtr()->logMessage("Show Config Window");

                if ( mRoot->showConfigDialog() )
                    {
                        
                        mRoot->saveConfig();
						LogManager::getSingletonPtr()->logMessage("Saved Config");
                    }

                else
                {
                    LogManager::getSingletonPtr()->logMessage("Error loading config");
                }
            }

        LogManager::getSingletonPtr()->logMessage("Cleaning Up Render Params");

        delete opt;
    }

void Renderer::SetRenderParam ( String paramName, String paramVal )
{
    mRenderSystem->setConfigOption(paramName, paramVal);
    mRoot->saveConfig();
}

Renderer::~Renderer ()
    {
        if ( mRoot )
            delete mRoot;
    }

Ogre::RenderWindow * Renderer::createWindow ( const Ogre::String & name )
    {
        mWindow = mRoot->initialise(true, "NxOgre Pinball");
        return mWindow;
    }

Ogre::SceneManager * Renderer::createSceneMgr ( const Ogre::String & name )
{
    return mRoot->createSceneManager(ST_GENERIC, name);
}

void Renderer::createViewportAndBindToCamera ( Ogre::Camera * c )
{
        mViewport = mWindow->addViewport(c);
        c->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));
}

void Renderer::start ()
{
        mShutdown = false;
}

void Renderer::stop ()
{
    mShutdown = true;
}

bool Renderer::frameStarted ( const Ogre::FrameEvent & evt )
{
    return true;
}

bool Renderer::frameEnded ( const Ogre::FrameEvent & evt )
    {
        if ( mShutdown )
        {
            return false;
        }

        if ( mWindow->isClosed() )
            {
                mShutdown = true;
                return true;
            }

        return true;
    }
/*
void Renderer::bind ( InputHandler * ipHand )
{
    //mGame = game;
    mInputHandler = ipHand;
}
*/

Ogre::Root * Renderer::getRoot ()
{
    return mRoot;
}

Ogre::RenderWindow * Renderer::getWindow ()
{
    return mWindow;
}

int Renderer::getWidth ()
{
    return mViewport->getActualWidth();
}

int Renderer::getHeight ()
{
    return mViewport->getActualHeight();
}
