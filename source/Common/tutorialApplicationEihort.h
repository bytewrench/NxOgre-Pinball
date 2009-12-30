//
//	Tutorials
//

#ifndef __simple_tutorial_eihort_H__

#define __simple_tutorial_eihort_H__

#define NOMINMAX
#include "windows.h"
#include "resource.h"

#include "nxOgre.h"
#include "Ogre.h"
#include "OgreConfigFile.h"

#include "OgreStringConverter.h"
#include "OgreException.h"


using namespace Ogre;

using namespace nxOgre;

using namespace std;

#ifdef _DEBUG

#pragma comment(lib, "OIS_d.lib")

#else

#pragma comment(lib, "OIS.lib")

#endif

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"

using namespace OIS;


/// Betajaen's GUI 015
/// Written by Robin "Betajaen" Southern 07-Nov-2006, http://www.ogre3d.org/wiki/index.php/BetaGUI
/// This code is under the Whatevar! licence. Do what you want; but keep the original copyright header.
/// This code is not meant to be readable, if you base your future source on this, I will laugh at you.

#include "Ogre.h"

using namespace Ogre;

using namespace std;

namespace BetaGUI
    {
        class GUI;
        class Window;
        class Button;
        class TextInput;
        class Callback;

        enum wt
            {
                NONE = 0,
            MOVE = 1,
            RESIZE = 2,
            RESIZE_AND_MOVE = 3
            };

        class GUI
            {
                public: friend class Window;friend class Button;friend class TextInput;

                    GUI(String font, uint fontSize);
                    ~GUI();
                    bool injectMouse(uint x, uint y, bool LMB);
                    bool injectKey(String key, uint x, uint y);

                    void injectBackspace ( uint x, uint y )
                    {
                        injectKey("!b", x, y);
                    }

                    Window * createWindow ( Vector4, String, wt, String c = "" );

                    void destroyWindow ( Window * w )
                    {
                        mXW = w;
                    }

                    OverlayContainer * createOverlay ( String, Vector2, Vector2, String m = "", String = "",
                                                       bool a = true );
                    OverlayContainer * createMousePointer ( Vector2 size, String material );

                protected:

                    uint wc, bc, tc;
                    Overlay * mO;
                    vector<Window *> WN;
                    Window * mXW;
                    OverlayContainer * mMP;
                    String mFont;
                    uint mFontSize;
            };

        class Window
            {
                public: friend class Button;friend class TextInput;friend class GUI;

                    Button * createButton ( Vector4 Dimensions, String Material, String Text,
                                            Callback callback );
                    TextInput * createTextInput ( Vector4 Dimensions, String Material, String Value,
                                                  uint length );
                    OverlayContainer * createStaticText ( Vector4 Dimensions, String Text );

                    void hide ()
                    {
                        mO->hide();
                    }

                    void show ()
                    {
                        mO->show();
                    }

                    bool isVisible ()
                    {
                        return mO->isVisible();
                    }

                protected:

                    Window(Vector4 Dimensions, String Material, wt type, String caption, GUI *gui);

                    ~Window();
                    bool check(uint x, uint y, bool LMB);
                    bool checkKey(String key, uint x, uint y);
                    TextInput * mATI;
                    Button * mRZ, * mAB, * mTB;
                    uint x, y, w, h;
                    GUI * mGUI;
                    OverlayContainer * mO;
                    vector<BetaGUI::Button *> mB;
                    vector<BetaGUI::TextInput *> mT;
            };

        class BetaGUIListener
            {
                public:

                    virtual void onButtonPress ( Button * ref ) = 0;
            };

        class Callback
            {
                public: friend class Window;friend class Button;

                    Callback ()
                    {
                        t = 0;
                    }

                    Callback ( void (* functionPointer) ( Button * ref ) )
                        {
                            t = 1;
                            fp = functionPointer;
                        }

                    Callback ( BetaGUIListener * L )
                        {
                            t = 2;
                            LS = L;
                        }

                protected:

                    uchar t;void(*fp)(Button *r);

                    BetaGUIListener * LS;
            };

        class Button
            {
                public: friend class Window;

                    OverlayContainer * mO, * mCP;
                    String mmn, mma;

                protected:

                    Button(Vector4 Dimensions, String Material, String Text, Callback callback,
                           Window * parent);

                    ~Button ()
                        {
                            mO->getParent()->removeChild(mO->getName());
                            mCP->getParent()->removeChild(mCP->getName());
                        }

                    void activate ( bool a )
                        {
                            if ( ! a && mmn != "" )
                                mO->setMaterialName(mmn);

                            if ( a && mma != "" )
                                mO->setMaterialName(mma);
                        }

                    bool in ( uint mx, uint my, uint px, uint py )
                    {
                        return (! (mx >= x + px && my >= y + py))
                                   || (! (mx <= x + px + w && my <= y + py + h));
                    }

                    Callback callback;
                    uint x, y, w, h;
            };

        class TextInput
            {
                public: friend class Window;

                    TextInput(Vector4 Dimensions, String Material, String Value, uint length, Window *parent);

                    ~TextInput () { }

                    bool in ( uint mx, uint my, uint px, uint py )
                    {
                        return (! (mx >= x + px && my >= y + py))
                                   || (! (mx <= x + px + w && my <= y + py + h));
                    }

                    OverlayContainer * mO, * mCP;
                    String mmn, mma, value;
                    uint x, y, w, h, length;

                public:

                    String getValue ()
                    {
                        return value;
                    }

                    void setValue ( String v )
                    {
                        mCP->setCaption(value = v);
                    }
            };
    } /*** Optional Seperate Code that goes into a .cpp part : Cut downwards and paste  ***/

using namespace Ogre;

using namespace std;

namespace BetaGUI
    {
        GUI::GUI ( String font, uint fontSize ):
            mXW(0),
            mMP(0),
            mFont(font),
            mFontSize(fontSize),
            wc(0),
            bc(0),
            tc(0)
            {
                mO = OverlayManager::getSingleton().create("BetaGUI");
                mO->show();
            }

        GUI::~GUI ()
            {
                for ( uint i = 0; i < WN.size(); i ++ )
                    delete WN[i];

                WN.clear();
            }

        bool GUI::injectMouse ( uint x, uint y, bool LMB )
            {
                if ( mMP )
                    mMP->setPosition(x, y);

                if ( mXW )
                    {
                        for ( vector<Window *>::iterator i = WN.begin(); i != WN.end(); i ++ )
                            {
                                if ( mXW == (* i) )
                                    {
                                        delete mXW;
                                        WN.erase(i);
                                        mXW = 0;
                                        return false;
                                    }
                            }
                    }

                for ( uint i = 0; i < WN.size(); i ++ )
                    {
                        if ( WN[i]->check(x, y, LMB) )
                        {
                            return true;
                        }
                    }

                return false;
            }

        bool GUI::injectKey ( String key, uint x, uint y )
            {
                for ( uint i = 0; i < WN.size(); i ++ )
                    {
                        if ( WN[i]->checkKey(key, x, y) )
                        {
                            return true;
                        }
                    }

                return false;
            }

        OverlayContainer * GUI::createOverlay ( String N, Vector2 P, Vector2 D, String M, String C, bool A )
            {
                String t = "Panel";

                if ( C != "" )
                    t = "TextArea";

                OverlayContainer * e = static_cast<
                                           OverlayContainer *>(OverlayManager::getSingleton().createOverlayElement(t,
                                                                                                                   N));
                e->setMetricsMode(GMM_PIXELS);
                e->setDimensions(D.x, D.y);
                e->setPosition(P.x, P.y);

                if ( M != "" )
                    e->setMaterialName(M);

                if ( C != "" )
                    {
                        e->setCaption(C);
                        e->setParameter("font_name", mFont);
                        e->setParameter("char_height", StringConverter::toString(mFontSize));
                    }

                if ( A )
                    {
                        mO->add2D(e);
                        e->show();
                    }

                return e;
            }

        OverlayContainer * GUI::createMousePointer ( Vector2 d, String m )
            {
                Overlay * o = OverlayManager::getSingleton().create("BetaGUI.MP");
                o->setZOrder(500);
                mMP = createOverlay("bg.mp", Vector2(0, 0), d, m, "", false);
                o->add2D(mMP);
                o->show();
                mMP->show();
                return mMP;
            }

        Window * GUI::createWindow ( Vector4 D, String M, wt T, String C )
            {
                Window * w = new Window(D, M, T, C, this);
                WN.push_back(w);
                return w;
            }

        Window::Window ( Vector4 D, String M, wt t, String C, GUI * G ):
            x(D.x),
            y(D.y),
            w(D.z),
            h(D.w),
            mGUI(G),
            mTB(0),
            mRZ(0),
            mATI(0),
            mAB(0)
            {
                mO = G->createOverlay("BetaGUI.w" + StringConverter::toString(G->wc ++), Vector2(D.x, D.y),
                                      Vector2(D.z, D.w),                                 M);

                if ( t >= 2 )
                    {
                        Callback c;
                        c.t = 4;
                        mRZ = createButton(Vector4(D.z - 16, D.w - 16, 16, 16), M + ".resize", "", c);
                    }

                if ( t == 1 || t == 3 )
                    {
                        Callback c;
                        c.t = 3;
                        mTB = createButton(Vector4(0, 0, D.z, 22), M + ".titlebar", C, c);
                    }
            }

        Window::~Window ()
            {
                for ( uint i = 0; i < mB.size(); i ++ )
                    delete mB[i];

                for ( uint i = 0; i < mT.size(); i ++ )
                    delete mT[i];

                mGUI->mO->remove2D(mO);
            }

        Button * Window::createButton ( Vector4 D, String M, String T, Callback C )
            {
                Button * x = new Button(D, M, T, C, this);
                mB.push_back(x);
                return x;
            }

        Button::Button ( Vector4 D, String M, String T, Callback C, Window * P ):
            x(D.x),
            y(D.y),
            w(D.z),
            h(D.w),
            mmn(M),
            mma(M)
            {
                ResourcePtr ma = MaterialManager::getSingleton().getByName(mmn + ".active");

                if ( ! ma.isNull() )
                    mma += ".active";

                mO = P->mGUI->createOverlay(P->mO->getName() + "b"
                                                + StringConverter::toString(P->mGUI->bc ++),
                                            Vector2(x, y),
                                            Vector2(w, h),
                                            M,
                                            "",
                                            false);
                mCP = P->mGUI->createOverlay(mO->getName() + "c", Vector2(4, (h - P->mGUI->mFontSize) / 2),
                                             Vector2(w, h),       "",
                                             T,                   false);
                P->mO->addChild(mO);
                mO->show();
                mO->addChild(mCP);
                mCP->show();
                callback = C;
            }

        TextInput * Window::createTextInput ( Vector4 D, String M, String V, uint L )
            {
                TextInput * x = new TextInput(D, M, V, L, this);
                mT.push_back(x);
                return x;
            }

        OverlayContainer * Window::createStaticText ( Vector4 D, String T )
            {
                OverlayContainer * x = mGUI->createOverlay(mO->getName()
                                                               + StringConverter::toString(mGUI->tc ++),
                                                           Vector2(D.x, D.y),
                                                           Vector2(D.z, D.w),
                                                           "",
                                                           T,
                                                           false);
                mO->addChild(x);
                x->show();
                return x;
            }

        TextInput::TextInput ( Vector4 D, String M, String V, uint L, Window * P ):
            x(D.x),
            y(D.y),
            w(D.z),
            h(D.w),
            value(V),
            mmn(M),
            mma(M),
            length(L)
            {
                ResourcePtr ma = Ogre::MaterialManager::getSingleton().getByName(mmn + ".active");

                if ( ! ma.isNull() )
                    mma += ".active";

                mO = P->mGUI->createOverlay(P->mO->getName() + "t"
                                                + StringConverter::toString(P->mGUI->tc ++),
                                            Vector2(x, y),
                                            Vector2(w, h),
                                            M,
                                            "",
                                            false);
                mCP = P->mGUI->createOverlay(mO->getName() + "c", Vector2(4, (h - P->mGUI->mFontSize) / 2),
                                             Vector2(w, h),       "",
                                             V,                   false);
                P->mO->addChild(mO);
                mO->show();
                mO->addChild(mCP);
                mCP->show();
            }

        bool Window::checkKey ( String k, uint px, uint py )
            {
                if ( mATI == 0 )
                    return false;

                if ( ! mO->isVisible() )
                    return false;

                if ( ! (px >= x && py >= y) || ! (px <= x + w && py <= y + h) )
                    return false;

                if ( k == "!b" )
                    {
                        mATI->setValue(mATI->value.substr(0, mATI->value.length() - 1));
                        return true;
                    }

                if ( mATI->value.length() >= mATI->length )
                    return true;

                mATI->mCP->setCaption(mATI->value += k);
                return true;
            }

        bool Window::check ( uint px, uint py, bool LMB )
            {
                if ( ! mO->isVisible() )
                    return false;

                if ( ! (px >= x && py >= y) || ! (px <= x + w && py <= y + h) )
                    {
                        if ( mAB )
                        {
                            mAB->activate(false);
                        }

                        return false;
                    }

                if ( mAB )
                {
                    mAB->activate(false);
                }

                for ( uint i = 0; i < mB.size(); i ++ )
                    {
                        if ( mB[i]->in(px, py, x, y) )
                            continue;

                        if ( mAB )
                        {
                            mAB->activate(false);
                        }

                        mAB = mB[i];
                        mAB->activate(true);

                        if ( ! LMB )
                            return true;

                        if ( mATI )
                            {
                                mATI->mO->setMaterialName(mATI->mmn);
                                mATI = 0;
                            }

                        switch ( mAB->callback.t )
                            {
                                default:
                                    return true;

                                case 1:
                                    mAB->callback.fp(mAB);
                                    return true;

                                case 2:
                                    mAB->callback.LS->onButtonPress(mAB);
                                    return true;

                                case 3:
                                    mO->setPosition(x = px - (mAB->w / 2), y = py - (mAB->h / 2));
                                    return true;

                                case 4:
                                    mO->setDimensions(w = px - x + 8, h = py - y + 8);
                                    mRZ->mO->setPosition(mRZ->x = w - 16, mRZ->y = h - 16);

                                    if ( mTB )
                                    {
                                        mTB->mO->setWidth(mTB->w = w);
                                    }

                                    return true;
                            }
                    }

                if ( ! LMB )
                    return true;

                for ( uint i = 0; i < mT.size(); i ++ )
                    {
                        if ( mT[i]->in(px, py, x, y) )
                            continue;

                        mATI = mT[i];
                        mATI->mO->setMaterialName(mATI->mma);
                        return true;
                    }

                if ( mATI )
                    {
                        mATI->mO->setMaterialName(mATI->mmn);
                        mATI = 0;
                    }

                return true;
            }
    } // End of Betajaen's GUI. Normal programming can resume now.

class SimpleTutorialEihort :
    public FrameListener, public OIS::KeyListener, public BetaGUI::BetaGUIListener
    {
        public:

            // Enum's

            enum Key
                {
                    QUIT,
                    SCREENSHOT,
                    DEBUG_MODE,
                    RESET_SCENE,
                    PAUSE,
                    SLOWER,
                    FASTER,
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
                    MOUSE_BOTH,
                    FIRE_BALL1,
                    FIRE_BALL2,
                    RESET_BALL,
                    RESET_ALL,
                    PLUS_VEL,
                    MINUS_VEL,
                    NEW_GAME,
                    FLIP_LEFT1,
                    FLIP_LEFT2,
                    FLIP_LEFT3,
                    FLIP_LEFT4,
                    FLIP_LEFT5,
                    FLIP_LEFT6,
                    FLIP_LEFT7,
                    FLIP_RIGHT1,
                    FLIP_RIGHT2,
                    FLIP_RIGHT3,
                    FLIP_RIGHT4,
                    FLIP_RIGHT5,
                    FLIP_RIGHT6,
                    FLIP_RIGHT7,
                    POST_SCORE
                };

            enum mouseMode
                {
                    CAMERA_FOLLOW,
                    CAMERA_FORCE,
                    CAMERA_CONTROL,
                    PICK
                };


            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            SimpleTutorialEihort ()
                {
                    mTutorialName = "???";
                    mTutorialDescription = "???";
                    mShutdownNow = false;
                    mMouseMode = PICK;
                    mConstruct = false;
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            HINSTANCE hInst;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void setHInst ( HINSTANCE hi )
            {
                hInst = hi;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            ~SimpleTutorialEihort ()
            {
                std::cout << "Bye bye." << std::endl;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            bool Run ()
                {
                    getSetup();

                    resetConfig();
                    getConfig();

                    setupRenderSystem();

                    ConfigFile cf;
                    cf.load("resources.cfg");

                    ConfigFile::SectionIterator seci = cf.getSectionIterator();
                    String secName, typeName, archName;

                    while ( seci.hasMoreElements() )
                        {
                            secName = seci.peekNextKey();
                            ConfigFile::SettingsMultiMap * settings = seci.getNext();
                            ConfigFile::SettingsMultiMap::iterator i;

                            for ( i = settings->begin(); i != settings->end(); ++ i )
                                {
                                    typeName = i->first;
                                    archName = i->second;
                                    ResourceGroupManager::getSingleton().addResourceLocation(archName,
                                                                                             typeName,
                                                                                             secName);
                                }
                        }

                    mWindow = mRoot->initialise(true, "NxOgre: " + mTutorialName + " - "
                                                          + mTutorialDescription);

                    mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "OgreBall");
                    mCamera = mSceneMgr->createCamera("MainCamera");
                    mCamera->setPosition(Vector3(0, 0, 0));

                    mCamera->setNearClipDistance(1.0f);
                    mCamera->setFarClipDistance(50.0f);

                    mViewport = mWindow->addViewport(mCamera);

                    if ( ! mConstruct )
                        mViewport->setBackgroundColour(ColourValue::Black);
                    else
                        mViewport->setBackgroundColour(ColourValue::White);

                    mCamera->setAspectRatio(Real(mViewport->getActualWidth())
                                                / Real(mViewport->getActualHeight()));

                    TextureManager::getSingleton().setDefaultNumMipmaps(1);
                    MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
                    MaterialManager::getSingleton().setDefaultAnisotropy(8);
                    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

                    mOverlay = Ogre::OverlayManager::getSingleton().create("GUI");
                    mOverlay->setZOrder(549);

                    Ogre::String loadingBackMat = "boot.any";

                    if ( ! MaterialManager::getSingleton().getByName("boot." + mTutorialName).isNull() )
                        loadingBackMat = "boot." + mTutorialName;

                    OverlayElement * loadingBack = createOverlay("loadBack",
                                                                 Vector2(0, 0),
                                                                 Vector2(mViewport->getActualWidth(),
                                                                         mViewport->getActualHeight()),
                                                                 loadingBackMat);
                    OverlayElement * loading = createOverlay("load", Vector2((mWindow->getWidth() / 2)
                                                                                 - 64, (mWindow->getHeight() / 2)
                                                                                           - 16),
                                                             Vector2(128, 32),
                                                             "gui_loading");

                    mOverlay->show();

                    mRoot->renderOneFrame();

                    loading->hide();
                    loadingBack->hide();
                    createInputSys(mWindow);

                    
                    mGUI = new BetaGUI::GUI("nxogrefont", 16);
					/*
                    mGUI_ExitWindow = mGUI->createWindow(Vector4(mWindow->getWidth() - 128 - 4, 4, 128, 22),
                                                         "",
                                                         BetaGUI::NONE);
                    mGUI_ExitButton = mGUI_ExitWindow->createButton(Vector4(128 - 22, 0, 22, 22),
                                                                    "bgui.exit",
                                                                    "",
                                                                    BetaGUI::Callback(this));
                    mGUI_PauseButton = mGUI_ExitWindow->createButton(Vector4(128 - (22 * 2) - 4, 0, 22, 22),
                                                                     "bgui.pause",
                                                                     "",
                                                                     BetaGUI::Callback(this));
                    mGUI_DebugButton = mGUI_ExitWindow->createButton(Vector4(128 - (22 * 3) - 4, 0, 22, 22),
                                                                     "bgui.debug",
                                                                     "",
                                                                     BetaGUI::Callback(this));
                    */
                    mPointer = createOverlay("Pointer", Vector2((mWindow->getWidth() / 2) - 16, (mWindow->getHeight() / 2) - 16), Vector2(32,32), "nx.arrow");
                    mPointer->hide();		
                    mPointer = mGUI->createMousePointer(Vector2(24, 24), "bgui.pointer");
                    mGUI->injectMouse((mWindow->getWidth() / 2) - 16, (mWindow->getHeight() / 2) - 16, false);
                    mPointer->hide();

                    //gui_pause = createOverlay("paused",          Vector2(16, mWindow->getHeight() - 100 - 32),
                    //                          Vector2(128, 128), "gui_pause");
                    //gui_pause->hide();

                    //gui_slowfast = createOverlay("slowfast",
                    //                             Vector2(16 + 100, mWindow->getHeight() - 100 - 32),
                    //                             Vector2(128, 128),
                    //                             "",
                    //                             "0");
                    //gui_slowfast->hide();

                    gui_score = createOverlay("gui_score", Vector2(1, 1), Vector2(128, 128), "", "0");
                    gui_score->setColour(ColourValue::Green);
                    gui_score->setParameter("char_height", "25");

                    gui_power = createOverlay("gui_power", Vector2(1, 20), Vector2(128, 128), "", "0");
                    gui_power->setColour(ColourValue::Green);
                    gui_power->setParameter("char_height", "25");

                    gui_fps = createOverlay("gui_fps", Vector2(1, 40), Vector2(128, 128), "", "0");
                    gui_fps->setColour(ColourValue::Green);
                    gui_fps->setParameter("char_height", "25");

                    gui_balls = createOverlay("gui_balls", Vector2(1, 60), Vector2(128, 128), "", "0");
                    gui_balls->setColour(ColourValue::Green);
                    gui_balls->setParameter("char_height", "25");

                    gui_bumps = createOverlay("gui_bumps", Vector2(1, 80), Vector2(128, 128), "", "0");
                    gui_bumps->setColour(ColourValue::Green);
                    gui_bumps->setParameter("char_height", "25");

                    //mCaption1 = createOverlay("caption.1", Vector2(8, 128), Vector2(16, 256), "", " ");
                    //mCaption2 = createOverlay("caption.2", Vector2(8, 128 + 16), Vector2(16, 256), "", " ");
                    //mCaption3 = createOverlay("caption.3",      Vector2(8, mWindow->getHeight() - 16 - 8),
                    //                          Vector2(16, 256), "",
                    //                          " ");

                    ApplicationStart();

                    if ( ! mConstruct )
                        {
                            sg = mSceneMgr->createStaticGeometry("grid");
                            CreateEntNode("nx.floor2", Vector3(0, - 0.05, 0));
                            CreateEntNode("nx.body.axis", Vector3(0, 0, 0));
                            sg->build();
                        }

                    if ( ! mConstruct )
                        {
                            mSceneMgr->setFog(FOG_LINEAR, ColourValue::Black, 0.0005f, 4000, 4500);
                            mSceneMgr->setAmbientLight(ColourValue(0.6, 0.6, 0.6));
                        }

                    else
                        {
                            mSceneMgr->setFog(FOG_LINEAR, ColourValue::White, 0.0005f, 4000, 4500);
                            mSceneMgr->setAmbientLight(ColourValue(0.9, 0.9, 0.9));
                        }

                    mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

                    Light * mLight = mSceneMgr->createLight("SunLight");
                    mLight->setPosition(Vector3(0, 100, - 25));

                    if ( mMouseMode == PICK )
                        mPointer->show();

                    mRoot->addFrameListener(this);

                    mRoot->startRendering();

                    return true;
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void setupRenderSystem ()
                {
                    mRoot = new Root("", "", "ogre.graphics.log");
                    mRoot->loadPlugin("RenderSystem_Direct3D9");
                    //mRoot->loadPlugin("RenderSystem_GL");
                    mRoot->loadPlugin("Plugin_ParticleFX");

                    Ogre::String rs = "Direct3D9 Rendering Subsystem";

                    Ogre::RenderSystemList * pRenderSystemList;
                    pRenderSystemList = mRoot->getAvailableRenderers();
                    Ogre::RenderSystemList::iterator pRenderSystem;
                    pRenderSystem = pRenderSystemList->begin();
                    Ogre::RenderSystem * pSelectedRenderSystem;
                    pSelectedRenderSystem = * pRenderSystem;

                    while ( pRenderSystem != pRenderSystemList->end() )
                        {
                            if ( (* pRenderSystem)->getName() == rs )
                                {
                                    mRenderSystem = * pRenderSystem;
                                    break;
                                }

                            pRenderSystem ++;
                        }

                    mRoot->setRenderSystem(mRenderSystem);

                    mRenderSystem->setConfigOption("Full Screen", settings["fullscreen"]);
                    mRenderSystem->setConfigOption("VSync", settings["vsync"]);

                    if ( rs == "Direct3D9 Rendering Subsystem" )
                    {
                        mRenderSystem->setConfigOption("Video Mode", settings["width"] + " x "
                                                                         + settings["height"] + " @ " + settings["depth"]
                                                                         + "-bit colour");

                    //mRenderSystem->setConfigOption("Anti aliasing","Level 4");

                    }
                    else
                        {
                            mRenderSystem->setConfigOption("Video Mode", settings["width"] + " x "
                                                                             + settings["height"]);

                            mRenderSystem->setConfigOption("Colour Depth", settings["depth"]);
                        //mRenderSystem->setConfigOption("FSAA", "2");
                        }
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void resetConfig ()
                {
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

                    keys[FLIP_LEFT1] = KC_LSHIFT;
                    keys[FLIP_LEFT2] = KC_Q;
                    keys[FLIP_LEFT3] = KC_A;
                    keys[FLIP_LEFT4] = KC_Z;
                    keys[FLIP_LEFT5] = KC_W;
                    keys[FLIP_LEFT6] = KC_D;
                    keys[FLIP_LEFT7] = KC_X;

                    keys[FLIP_RIGHT1] = KC_RSHIFT;
                    keys[FLIP_RIGHT2] = KC_L;
                    keys[FLIP_RIGHT3] = KC_P;
                    keys[FLIP_RIGHT4] = KC_M;
                    keys[FLIP_RIGHT5] = KC_O;
                    keys[FLIP_RIGHT6] = KC_K;
                    keys[FLIP_RIGHT7] = KC_J;

                    keys[FIRE_BALL1] = KC_SPACE;
                    keys[FIRE_BALL2] = KC_RETURN;
                    //keys[RESET_BALL] = KC_R;
                    //keys[MINUS_VEL] = KC_SUBTRACT;
                    //keys[PLUS_VEL] = KC_ADD;
                    //keys[RESET_ALL] = KC_G;

                    keys[NEW_GAME] = KC_S;
                    keys[POST_SCORE] = KC_Y;

                //CUSTOMKEYS
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void getConfig ()
                {
                    ConfigFile cf;
                    cf.load("config.yaml", ":", true);

                    ConfigFile::SectionIterator seci = cf.getSectionIterator();
                    String secName, typeName, archName;

                    while ( seci.hasMoreElements() )
                        {
                            secName = seci.peekNextKey();
                            ConfigFile::SettingsMultiMap * csettings = seci.getNext();
                            ConfigFile::SettingsMultiMap::iterator i;

                            for ( i = csettings->begin(); i != csettings->end(); ++ i )
                                {
                                    if ( i->first == "device" )
                                        {
                                            if ( i->second == "dx9" )
                                            {
                                                settings["device"] = "Direct3D9 Rendering Subsystem";
                                            }
                                            else
                                            {
                                                settings["device"] = "OpenGL Rendering Subsystem"; // Check..
                                            }
                                        }
                                    else if ( i->first == "width" )
                                    {
                                        settings["width"] =
                                            Ogre::StringConverter::toString(Ogre::StringConverter::parseUnsignedInt(i->second));
                                    }
                                    else if ( i->first == "height" )
                                    {
                                        settings["height"] =
                                            Ogre::StringConverter::toString(Ogre::StringConverter::parseUnsignedInt(i->second));
                                    }
                                    else if ( i->first == "depth" )
                                    {
                                        settings["depth"] =
                                            Ogre::StringConverter::toString(Ogre::StringConverter::parseUnsignedInt(i->second));
                                    }
                                    else if ( i->first == "aa" )
                                    {
                                        settings["aa"] =
                                            Ogre::StringConverter::toString(Ogre::StringConverter::parseUnsignedInt(i->second));
                                    }
                                    else if ( i->first == "fullscreen" )
                                    {
                                        settings["fullscreen"] = i->second;
                                    }
                                    else if ( i->first == "vsync" )
                                    {
                                        settings["aa"] = i->second;
                                    }
                                }
                        }
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void createInputSys ( RenderWindow * _window )
                {
                    LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
                    ParamList pl;
                    size_t windowHnd = 0;
                    std::ostringstream windowHndStr;

                    _window->getCustomAttribute("WINDOW", & windowHnd);
                    windowHndStr << windowHnd;
                    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

                    InputManager & im = * OIS::InputManager::createInputSystem(pl);

                    mKeyboard = static_cast<Keyboard *>(im.createInputObject(OISKeyboard, true));
                    mMouse = static_cast<Mouse *>(im.createInputObject(OISMouse, true));
                    mKeyboard->setEventCallback(this);
                    mKeyboard->setBuffered(true);
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void onButtonPress ( BetaGUI::Button * ref ) {
            /*
            if ( ref == mGUI_ExitButton )
                mShutdownNow = true;

            if ( ref == mGUI_PauseButton && timeSince >= 0.5f )
                {
                    if ( nxOgre::world::getSingleton().isPaused() )
                        {
                            world::getSingleton().resume();
                            mGUI_PauseButton->mmn = "bgui.pause";
                            mGUI_PauseButton->mma = "bgui.pause.active";
                        }

                    else
                        {
                            world::getSingleton().pause();
                            mGUI_PauseButton->mmn = "bgui.play";
                            mGUI_PauseButton->mma = "bgui.play.active";
                        }

                    timeSince = 0.0f;
                }

            if ( ref == mGUI_DebugButton && timeSince >= 0.25f )
                {
                    world::getSingleton().showDebugOverlay(mCamera, mWindow);
                    world::getSingleton().debug(world::getSingleton().isDebugRendering() ^ 1);

                    if ( world::getSingleton().isDebugRendering() )
                        {
                            mGUI_DebugButton->mmn = "bgui.debugon";
                            mGUI_DebugButton->mma = "bgui.debugon.active";
                        }

                    else
                        {
                            mGUI_DebugButton->mmn = "bgui.debug";
                            mGUI_DebugButton->mma = "bgui.debug.active";
                        }

                    timeSince = 0.0f;
                }

            GUIbuttonPressed(ref);
            */
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void captureInput ()
            {
                mKeyboard->capture();
				mMouse->capture();
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            bool isKeyDown ( Key k )
            {
                return mKeyboard->isKeyDown(keys[k]);
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            bool keyPressed ( const KeyEvent & arg )
                //bool keyPressed ( const OIS::KeyEvent &evt )
                {
                    
                    if ( arg.text == 8 )
                        {
                            mGUI->injectBackspace(mPointer->getLeft(), mPointer->getTop());
                            return true;
                        }
                    
                    if ( arg.text < 32 || arg.text > 126 )
                        return true;

                    string k;
                    k = static_cast<char>(arg.text);
                    mGUI->injectKey(k, mPointer->getLeft(), mPointer->getTop());
                    return true;
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            bool keyReleased ( const KeyEvent & arg )
            {
                mGUI->injectKey(mKeyboard->getAsString((OIS::KeyCode) arg.text), mPointer->getLeft(), mPointer->getTop());
                return true;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            float getRMouseX ()
                {
                    const MouseState & ms = mMouse->getMouseState();
                    return ms.X.rel;
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            float getRMouseY ()
                {
                    const MouseState & ms = mMouse->getMouseState();
                    return ms.Y.rel;
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            float getRMouseZ ()
                {
                    const MouseState & ms = mMouse->getMouseState();
                    return ms.Z.rel;
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            Key getMouseButton ()
                {
                    const MouseState & ms = mMouse->getMouseState();

                    if ( ms.buttons == 1 )
                        return MOUSE_PRIMARY;

                    else if ( ms.buttons == 2 )
                        return MOUSE_ALT;

                    else if ( ms.buttons == 3 )
                        return MOUSE_BOTH;

                    else
                        return MOUSE_NONE;
                }
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            Ogre::OverlayContainer * createOverlay ( Ogre::String name,         Vector2 position,
                                                     Vector2 dimensions,        Ogre::String material = "",
                                                     Ogre::String caption = "", bool autoAdd = true )
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

                    if ( autoAdd )
                        {
                            mOverlay->add2D(e);
                            e->show();
                        }

                    return e;
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void createPlane ( Ogre::String _name,  Ogre::String _material, Ogre::Vector3 _pos,
                               Ogre::Vector2 _size, Ogre::Vector2 _subdivisions = Ogre::Vector2(1, 1) )
                {
                    Plane _plane;
                    _plane.normal = Vector3::UNIT_Y;
                    _plane.d = 0;

                    MeshManager::getSingleton().createPlane(_name + ".plane",
                                                            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                            _plane,
                                                            _size.x,
                                                            _size.y,
                                                            1,
                                                            1,
                                                            true,
                                                            1,
                                                            _subdivisions.x,
                                                            _subdivisions.y,
                                                            Vector3::UNIT_Z);

                    Entity * _entity = mSceneMgr->createEntity(_name + ".entity", _name + ".plane");
                    _entity->setMaterialName(_material);
                    _entity->setCastShadows(false);

                    sg->addEntity(_entity, _pos);
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void CreateEntNode ( Ogre::String _name, Ogre::Vector3 _pos )
                {
                    Entity * _entity = mSceneMgr->createEntity(_name + ".entity", _name + ".mesh");
                    _entity->setCastShadows(false);
                    sg->addEntity(_entity, _pos);
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            SceneNode * CreateNodeEntity ( Ogre::String _name, Ogre::String ent, Ogre::Vector3 _pos )
                {
                    SceneNode * n = mSceneMgr->getRootSceneNode()->createChildSceneNode(_name);
                    Entity * e = mSceneMgr->createEntity(_name + ".ent", ent);
                    n->attachObject(e);
                    n->setPosition(_pos);
                    return n;
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void Shutdown ()
                {
                    delete mGUI;
                    ApplicationStop();
                    mRoot->shutdown();
                    delete mRoot;

#ifdef _DEBUG

                    std::ifstream ifs("OgreLeaks.log");
                    string line;
                    unsigned int i = 0;

                    cout << "\n\n\nLeak Report" << std::endl;
                    cout << "-----------" << std::endl;

                    while ( ! ifs.eof() )
                        {
                            i ++;
                            std::getline(ifs, line);

                            if ( i < 4 )
                                continue;

                            if ( line.length() == 0 )
                                continue;

                            if ( line.substr(0, 1) == "-" )
                                continue;

                            std::cout << "   ->" << line << std::endl;
                        }

                    cout << "\n\n\n";

#endif

                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            bool frameStarted ( const FrameEvent & evt )
                {
                    if ( mWindow->isClosed() )
                        return false;

                    captureInput();

                    if ( isKeyDown(QUIT) )
                        mShutdownNow = true;

                    
                    if ( mMouseMode == CAMERA_CONTROL )
                        {
                            mRotX = Degree(- getRMouseX() * 0.13);
                            mRotY = Degree(- getRMouseY() * 0.13);
                            mCamera->yaw(mRotX);
                            mCamera->pitch(mRotY);
                        }

                    else if ( mMouseMode == CAMERA_FORCE )
                        {
                            mRotX = Degree(- getRMouseX() * 0.13);
                            mRotY = Degree(- getRMouseY() * 0.13);
                            mCamera->yaw(mRotX);
                            mCamera->pitch(mRotY);
                        }

                    else if ( mMouseMode == PICK && getMouseButton() == MOUSE_PRIMARY )
                        {
                            mPointer->setPosition(mPointer->getLeft() + (getRMouseX()),
                                                  mPointer->getTop() + (getRMouseY()));

                            if ( ! mGUI->injectMouse(mPointer->getLeft(), mPointer->getTop(), true) )
                                mouseMoveOrGrab(mPointer->getLeft(), mPointer->getTop(), true);
                        }

                    else if ( mMouseMode == PICK && getMouseButton() == MOUSE_ALT )
                        {
                            mPointer->setPosition(mPointer->getLeft() + (getRMouseX()),
                                                  mPointer->getTop() + (getRMouseY()));

                            mouseMoveOrGrab(mPointer->getLeft(), mPointer->getTop(), false);
                        }

                    else if ( mMouseMode == PICK && getMouseButton() == MOUSE_BOTH )
                        {
                            mRotX = Degree(- getRMouseX() * 0.13);
                            mRotY = Degree(- getRMouseY() * 0.13);
                            mCamera->yaw(mRotX);
                            mCamera->pitch(mRotY);
                        }

                    else if ( mMouseMode == PICK )
                        {
                            mPointer->setPosition(mPointer->getLeft() + (getRMouseX()),
                                                  mPointer->getTop() + (getRMouseY()));

                            mGUI->injectMouse(mPointer->getLeft(), mPointer->getTop(), false);
                        }

                    if ( mMouseMode == PICK || mMouseMode == CAMERA_CONTROL )
                        {
                            if ( isKeyDown(PAN_LEFT) )
                                mTranslateVector.x = - mMoveScale;

                            if ( isKeyDown(PAN_RIGHT) )
                                mTranslateVector.x = mMoveScale;

                            if ( isKeyDown(PAN_FORWARD) )
                                mTranslateVector.z = - mMoveScale;

                            if ( isKeyDown(PAN_BACKWARD) )
                                mTranslateVector.z = mMoveScale;

                            if ( isKeyDown(PAN_UP) )
                                mTranslateVector.y = mMoveScale;

                            if ( isKeyDown(PAN_DOWN) )
                                mTranslateVector.y = - mMoveScale;

                            mCamera->moveRelative(mTranslateVector);
                        }

                    mMoveScale = 5 * evt.timeSinceLastFrame;

                    mRotX = 0;
                    mRotY = 0;
                    mTranslateVector = Vector3::ZERO;
                   
                    // Let the tutorial have some fun.
                    timeSince += evt.timeSinceLastFrame;
                    newFrame(evt.timeSinceLastFrame);

                    //
                    //		Pausing NxOgre
                    //
                    /*
                    if ( isKeyDown(DEBUG_MODE) && timeSince >= 0.25f )
                        {

                            // Technically this only needs to be done once, and it should be in your setup code.
                            world::getSingleton().showDebugOverlay(mCamera, mWindow);

                            // Pause or unpause the DebugRenderer
                            world::getSingleton().debug(world::getSingleton().isDebugRendering() ^ 1);
                    
                            if ( world::getSingleton().isDebugRendering() )
                                {
                                   // mGUI_DebugButton->mmn = "bgui.debugon";
                                   // mGUI_DebugButton->mma = "bgui.debugon.active";
                                }

                            else
                                {
                                    //mGUI_DebugButton->mmn = "bgui.debug";
                                    //mGUI_DebugButton->mma = "bgui.debug.active";
                                }
                    
                            timeSince = 0.0f;
                        }
                    */
                    if ( isKeyDown(SCREENSHOT) && timeSince >= 0.25f )
                        {
                            mWindow->writeContentsToFile("screenshot."
                                                             + Ogre::StringConverter::toString(Ogre::Math::Floor(mRoot->getTimer()->getMilliseconds()
                                                                                                                     / 1000))
                                                             + ".png");
                            timeSince = 0.0f;
                        }
                    /*
                    if ( isKeyDown(FASTER) && timeSince >= 0.25f )
                        {
                            speed *= 2;
                            world::getSingleton().setTimeModifer(speed);

                            timeSince = 0.0f;

                            if ( speed > 10 )
                                speed = 10;

                            if ( speed != 0 )
                                {
                                    gui_slowfast->show();
                                    gui_slowfast->setCaption(Ogre::StringConverter::toString(speed));
                                }

                            else
                                gui_slowfast->hide();
                        }

                    if ( isKeyDown(SLOWER) && timeSince >= 0.25f )
                        {
                            speed /= 2;
                            world::getSingleton().setTimeModifer(speed);
                            timeSince = 0.0f;

                            if ( speed < 0.25 )
                                speed = 0.25;

                            if ( speed != 0 )
                                {
                                    gui_slowfast->show();
                                    gui_slowfast->setCaption(Ogre::StringConverter::toString(speed));
                                }

                            else
                                gui_slowfast->hide();
                        }

                    if ( isKeyDown(PAUSE) && timeSince >= 0.5f )
                        {
                            if ( nxOgre::world::getSingleton().isPaused() )
                                {
                                    world::getSingleton().resume();
                                    gui_pause->hide();
                                }

                            else
                                {
                                    world::getSingleton().pause();
                                    gui_pause->show();
                                }

                            timeSince = 0.0f;
                        }
                    
                    if ( firstFrame )
                        {
                            if ( mMouseMode == PICK ) {
                            //mCamera->setPosition(10,10,10); 
                            //mCamera->lookAt(0, 1, 0);
                            }

                            firstFrame = false;
                        }
                    
                    if ( hasTargetBody == false )
                    {
                        targetNode->setVisible(false);
                    }
                    else
                        {
                            Vector3 p = targetBody->getGlobalPosition();
                            p.y = 0.01;
                            targetNode->setPosition(p);
                            targetNode->setVisible(true);
                        }
                    */
                    return true;
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            Ogre::String mmToString ( mouseMode mm )
                {
                    switch ( mm )
                        {
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

            bool frameEnded ( const FrameEvent & evt )
                {
                    if ( mShutdownNow )
                        return false;

                    return true;
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void ApplicationStart ()
                {


                    //flipperSound = OpenSound(SoundDevice,"Media/sounds/Mechanisms/flipper.ogg",false);
                    //
                    //mCamera->setPosition(- 1, 14, 24);

					Quaternion q;
					q.w = .90;
					q.x = -.377;
					q.y = 0;
					q.z = 0;
					mCamera->setPosition(0, 15.95, 21.55);
					mCamera->setOrientation(q);
                    //mCamera->lookAt(- 1, 0, 7);

                    //targetBody = 0;
                    //hasTargetBody = false;

                    //targetNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("target");
                    //Entity * _entity = mSceneMgr->createEntity("nx.bodyguide.ent", "nx.bodyguide.mesh");
                    //_entity->setCastShadows(false);
                    //targetNode->attachObject(_entity);
                    //targetNode->scale(0.5, 0.5, 0.5);
                    //targetNode->setVisible(false);

                    firstFrame = true;
                    timeSince = 0;
                    speed = 1;

                    prestart();
                    start();

              //  mRayCaster = world::getSingleton().getScene("Main")->createRayCaster("MousePicker",
                //                                                                     Vector3::ZERO,
                  //                                                                   Vector3::ZERO,
                    //                                                                 200);
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void ApplicationStop ()
                {
                    prestop();

                    stop();
                }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void getSetup ()
            {
                getTutorialSettings();
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            void mouseMoveOrGrab ( float x, float y, bool move_or_grab )
            {
                Ogre::Ray r = mCamera->getCameraToViewportRay(float(x / mWindow->getWidth()),
                                                              float(y / mWindow->getHeight()));

            //mRayCaster->setOrigin(r.getOrigin());
            //mRayCaster->setDirection(r.getDirection());

            //if ( ! mRayCaster->cast() )
            //{
            //    return;
            //}

            // Move it
            /*
            if ( move_or_grab )
                {
                    if ( hasTargetBody )
                        {
                            Ogre::Plane p;
                            p.normal = r.getDirection();
                            p.d = - mRayCaster->mHitPos.dotProduct(p.normal);

                            std::pair<bool, Real> res;
                            res = r.intersects(p);
                            Vector3 objPoint;

                            if ( res.first )
                                {
                                    Vector3 force = r.getPoint(res.second)
                                                        - targetBody->getGlobalPosition();
                                    force *= 10;
                                    force -=
                                        NxTools::convert(targetBody->mActor->getPointVelocity(NxTools::convert(targetBody->getGlobalPosition())));
                                    targetBody->addForceAtPos(force, targetBody->getGlobalPosition());
                                }
                        }

                    return;
                }
            */
            /*
            // Grab it
            if ( ! move_or_grab )
                {
                    body * t = mRayCaster->mHitBody;

                    if ( t->isStatic() )
                        {
                            targetBody = 0;
                            hasTargetBody = false;
                            return;
                        }

                    // Can't add forces to Kinematics.
                    if ( t->isKinematic() )
                        return;

                    if ( t == targetBody )
                        return;

                    targetBody = t;
                    hasTargetBody = true;
                }
                */
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            virtual void getTutorialSettings () = 0;
            virtual void start () = 0;
            virtual void stop () = 0;
            virtual void prestart () = 0;
            virtual void prestop () = 0;
            virtual void newFrame ( float _time ) = 0;

            virtual void GUIbuttonPressed ( BetaGUI::Button * ref ) { };

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            //Sound Stuff

            //OutputStreamPtr thumperSound;

            // Tutorial bits
            Ogre::String mTutorialName;
            Ogre::String mTutorialDescription;
            bool mShutdownNow;
            bool firstFrame;
            float timeSince;
            bool mConstruct;

            // Ogre pointer bits
            Ogre::Root * mRoot;
            Ogre::RenderSystem * mRenderSystem;
            Ogre::Camera * mCamera;
            Ogre::RenderWindow * mWindow;
            Ogre::Viewport * mViewport;
            Ogre::SceneManager * mSceneMgr;
            int winHeight;
            int winWidth;

            // Gui bits
            OverlayElement * gui_pause;
            OverlayElement * gui_slowfast;
            OverlayElement * gui_fps;
            OverlayElement * gui_score;
            OverlayElement * gui_power;
            OverlayElement * gui_balls;
            OverlayElement * gui_bumps;


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
            Ogre::Overlay * mOverlay;

            Ogre::OverlayContainer * mPointer;
            Ogre::OverlayContainer * mTargetCaption;
            Ogre::StaticGeometry * sg;
            /*
                        // Mousepicker and Raycasting bits.
                        body * targetBody;
                        SceneNode * targetNode;
                        bool hasTargetBody;
                        rayCaster * mRayCaster;
                        */

            // Input bits
            OIS::Mouse * mMouse;
            OIS::Keyboard * mKeyboard;


    //
    /*
    Ogre::OverlayContainer * mCaption1;
    Ogre::OverlayContainer * mCaption2;
    Ogre::OverlayContainer * mCaption3;
 */

    // GUI
    BetaGUI::GUI * mGUI;
    BetaGUI::Button * mGUI_ExitButton;
    BetaGUI::Button * mGUI_PauseButton;
    BetaGUI::Button * mGUI_DebugButton;
    BetaGUI::Window * mGUI_ExitWindow;
   
    };

#endif

#if 0

/// Betajaen's GUI 015 Uncompressed
/// Written by Robin "Betajaen" Southern 07-Nov-2006, http://www.ogre3d.org/wiki/index.php/BetaGUI
/// This code is under the Whatevar! licence. Do what you want; but keep the original copyright header.
#include "Ogre.h"
using namespace Ogre;

using namespace std;

namespace BetaGUI
    {

        /// This code is not meant to be readable, if you base your future source on this, I will laugh at you.

        class GUI;
        class Window;
        class Button;
        class TextInput;
        class Callback;

        enum wt
            { /* Window Type */
                NONE = 0,
            MOVE = 1,
            RESIZE = 2,
            RESIZE_AND_MOVE = 3
            };

        class GUI
            {
                public:
            friend class Window;
            friend class Button;
            friend class TextInput;

                    GUI(String font, uint fontSize);
                    ~GUI();
                    bool injectMouse(uint x, uint y, bool LMB);
                    bool injectKey(String key, uint x, uint y);
                    void injectBackspace(uint x, uint y);
                    Window * createWindow ( Vector4 Dimensions, String Material, wt type,
                                            String caption = "" );

                    void destroyWindow ( Window * window )
                    {
                        mXW = window;
                    }

                    OverlayContainer * createOverlay ( Ogre::String name,         Vector2 position,
                                                       Vector2 dimensions,        Ogre::String material = "",
                                                       Ogre::String caption = "", bool autoAdd = true );
                    OverlayContainer * createMousePointer ( Vector2 dimensions, Ogre::String material );

                protected:

                    Overlay * mO;           // Main sheet overlay
                    vector<Window *> WN;    // Windows
                    Window * mXW;           // Window to destroy
                    OverlayContainer * mMP; // Mouse Pointer Overlay
                    String mFont;
                    uint mFontSize;
                    uint wc, bc, tc;
            };

        class Window
            {
                public:
            friend class Button;
            friend class TextInput;
            friend class GUI;

                    Button * createButton ( Vector4 Dimensions, String Material, String Text,
                                            Callback callback );
                    TextInput * createTextInput ( Vector4 Dimensions, String Material, String Value,
                                                  uint length );
                    void createStaticText(Vector4 Dimensions, String Text);

                    void hide ()
                    {
                        mO->hide();
                    }

                    void show ()
                    {
                        mO->show();
                    }

                    bool isVisible ()
                    {
                        return mO->isVisible();
                    }

                protected:

                    Window(Vector4 Dimensions, String Material, wt type, String caption, GUI *gui);
                    ~Window();

                    bool check(uint x, uint y, bool LMB);
                    bool checkKey(String key, uint x, uint y);
                    TextInput * mATI;                // Active TextInput
                    Button * mRZ, * mAB, * mTB;      // Resizer, ActiveButton, Titlebar
                    uint x, y, w, h;                 // Dimensions
                    GUI * mGUI;                      // mGUI pointer
                    OverlayContainer * mO;           // Overlay
                    vector<BetaGUI::Button *> mB;    // Buttons
                    vector<BetaGUI::TextInput *> mT; // TextInputs
            };

        class BetaGUIListener
            {
                public:

                    virtual void onButtonPress ( Button * ref ) = 0;
            };

        class Callback
            {
                public:
            friend class Window;
            friend class Button;

                    Callback ()
                    {
                        t = 0;
                    }

                    Callback ( void (* functionPointer) ( Button * ref ) )
                        {
                            t = 1;
                            fp = functionPointer;
                        }

                    Callback ( BetaGUIListener * l )
                        {
                            t = 2;
                            LS = l;
                        }

                protected:

                    uchar
                        t;                // Type of callback: 0 - None, 1 - FunctionPointer 2 - GUIListener, 3 - Move Window, 4 - Resize
            void(*fp)(Button *ref);       // Function pointer (if 1)

                    BetaGUIListener * LS; // GuiListener (if 2)
            };

        class Button
            {
                public:
            friend class Window;

                    Button(Vector4 Dimensions, String Material, String Text, Callback callback, Window *parent)
                        ;

                    ~Button ()
                        {
                            mO->getParent()->removeChild(mO->getName());
                            mCP->getParent()->removeChild(mCP->getName());
                        }

                protected:

                    void activate ( bool a )
                        {
                            if ( ! a && mmn != "" )
                                mO->setMaterialName(mmn);

                            if ( a && mma != "" )
                                mO->setMaterialName(mma);
                        }

                    bool in ( uint mx, uint my, uint px, uint py )
                    {
                        return (! (mx >= x + px && my >= y + py))
                                   || (! (mx <= x + px + w && my <= y + py + h));
                    }

                    OverlayContainer * mO, * mCP; // Main Overlay and Caption
                    String mmn, mma;              // MaterialName Normal, MaterialName Active
                    BetaGUI::Callback callback;   // Callback to use
                    uint x, y, w, h;              // Dimensions.
            };

        class TextInput
            {
                public:
            friend class Window;

                    TextInput(Vector4 Dimensions, String Material, String Value, uint length, Window *parent);

                    ~TextInput () { }

                    String getValue ()
                    {
                        return value;
                    }

                    void setValue ( String v )
                    {
                        mCP->setCaption(value = v);
                    }

                protected:

                    bool in ( uint mx, uint my, uint px, uint py )
                    {
                        return (! (mx >= x + px && my >= y + py))
                                   || (! (mx <= x + px + w && my <= y + py + h));
                    }

                    OverlayContainer * mO, * mCP;
                    String mmn, mma, value;
                    uint x, y, w, h, length;
            };
    } // End of Namespace


/*** Optional Seperate Code that goes into a .cpp part : Cut downwards and paste  ***/
using namespace Ogre;

using namespace std;

namespace BetaGUI
    {
        GUI::GUI ( String font, uint fontSize ):
            mXW(0),
            mMP(0),
            mFont(font),
            mFontSize(fontSize),
            wc(0),
            bc(0),
            tc(0)
            {
                mO = OverlayManager::getSingleton().create("BetaGUI");
                mO->show();
            }

        GUI::~GUI ()
            {
                for ( uint i = 0; i < WN.size(); i ++ )
                    delete WN[i];

                WN.clear();
            }

        bool GUI::injectMouse ( uint x, uint y, bool LMB )
            {
                if ( mMP )
                    mMP->setPosition(x, y);

                if ( mXW )
                    {
                        for ( vector<Window *>::iterator i = WN.begin(); i != WN.end(); i ++ )
                            {
                                if ( mXW == (* i) )
                                    {
                                        delete mXW;
                                        WN.erase(i);
                                        mXW = 0;
                                        return false;
                                    }
                            }
                    }

                for ( uint i = 0; i < WN.size(); i ++ )
                    {
                        if ( WN[i]->check(x, y, LMB) )
                        {
                            return true;
                        }
                    }

                return false;
            }

        bool GUI::injectKey ( String key, uint x, uint y )
            {
                for ( uint i = 0; i < WN.size(); i ++ )
                    {
                        if ( WN[i]->checkKey(key, x, y) )
                        {
                            return true;
                        }
                    }

                return false;
            }

        void GUI::injectBackspace ( uint x, uint y )
        {
            injectKey("!b", x, y);
        }

        OverlayContainer * GUI::createOverlay ( String name,     Vector2 position, Vector2 dimensions,
                                                String material, String caption,   bool autoAdd )
            {
                String type = "Panel";

                if ( caption != "" )
                    type = "TextArea";

                OverlayContainer * e = static_cast<
                                           OverlayContainer *>(OverlayManager::getSingleton().createOverlayElement(type,
                                                                                                                   name));

                e->setMetricsMode(Ogre::GMM_PIXELS);
                e->setDimensions(dimensions.x, dimensions.y);
                e->setPosition(position.x, position.y);

                if ( material != "" )
                    e->setMaterialName(material);

                if ( caption != "" )
                    {
                        e->setCaption(caption);
                        e->setParameter("font_name", mFont);
                        e->setParameter("char_height", StringConverter::toString(mFontSize));
                    }

                if ( autoAdd )
                    {
                        mO->add2D(e);
                        e->show();
                    }

                return e;
            }

        OverlayContainer * GUI::createMousePointer ( Vector2 d, String m )
            {
                Overlay * o = OverlayManager::getSingleton().create("BetaGUI.MP");
                o->setZOrder(649);
                mMP = createOverlay("bg.mp", Vector2(0, 0), d, m, "", false);
                o->add2D(mMP);
                o->show();
                mMP->show();
                return mMP;
            }

        Window * GUI::createWindow ( Vector4 Dimensions, String Material, wt type, String caption )
            {
                Window * window = new BetaGUI::Window(Dimensions, Material, type, caption, this);
                WN.push_back(window);
                return window;
            }

        Window::Window ( Vector4 Dimensions, String Material, wt t, String caption, GUI * gui ):
            x(Dimensions.x),
            y(Dimensions.y),
            w(Dimensions.z),
            h(Dimensions.w),
            mGUI(gui),
            mTB(0),
            mRZ(0),
            mATI(0),
            mAB(0)
            {
                mO = gui->createOverlay("BetaGUI.w" + StringConverter::toString(gui->wc ++),
                                        Vector2(Dimensions.x, Dimensions.y),
                                        Vector2(Dimensions.z, Dimensions.w),
                                        Material);

                if ( t >= 2 )
                    {
                        Callback c;
                        c.t = 4;
                        mRZ = createButton(Vector4(Dimensions.z - 16, Dimensions.w - 16, 16, 16),
                                           Material + ".resize",
                                           "",
                                           c);
                    }

                if ( t == 1 || t == 3 )
                    {
                        Callback c;
                        c.t = 3;
                        mTB = createButton(Vector4(0, 0, Dimensions.z, 22), Material + ".titlebar", caption,
                                           c);
                    }
            }

        Window::~Window ()
            {
                for ( uint i = 0; i < mB.size(); i ++ )
                    delete mB[i];

                for ( uint i = 0; i < mT.size(); i ++ )
                    delete mT[i];

                mGUI->mO->remove2D(mO);
            }

        Button * Window::createButton ( Vector4 D, String M, String T, Callback C )
            {
                Button * x = new Button(D, M, T, C, this);
                mB.push_back(x);
                return x;
            }

        Button::Button ( Vector4 Dimensions, String m, String Text, Callback cb, Window * parent ):
            x(Dimensions.x),
            y(Dimensions.y),
            w(Dimensions.z),
            h(Dimensions.w),
            mmn(m),
            mma(m)
            {
                NXD("Button=" << mmn << ":" << mma);

                Ogre::ResourcePtr ma = Ogre::MaterialManager::getSingleton().getByName(mmn + ".active");

                if ( ! ma.isNull() )
                    mma += ".active";

                mO = parent->mGUI->createOverlay(parent->mO->getName() + "b"
                                                     + StringConverter::toString(parent->mGUI->bc ++),
                                                 Vector2(x, y),
                                                 Vector2(w, h),
                                                 m,
                                                 "",
                                                 false);
                mCP = parent->mGUI->createOverlay(mO->getName() + "c",
                                                  Vector2(4, (h - parent->mGUI->mFontSize) / 2),
                                                  Vector2(w, h),
                                                  "",
                                                  Text,
                                                  false);
                parent->mO->addChild(mO);
                mO->show();
                mO->addChild(mCP);
                mCP->show();
                callback = cb;
            }

        TextInput * Window::createTextInput ( Vector4 D, String M, String V, uint L )
            {
                TextInput * x = new TextInput(D, M, V, L, this);
                mT.push_back(x);
                return x;
            }

        void Window::createStaticText ( Vector4 D, String T )
            {
                OverlayContainer * x = mGUI->createOverlay(mO->getName()
                                                               + StringConverter::toString(mGUI->tc ++),
                                                           Vector2(D.x, D.y),
                                                           Vector2(D.z, D.w),
                                                           "",
                                                           T,
                                                           false);
                mO->addChild(x);
                x->show();
            }

        TextInput::TextInput ( Vector4 D, String M, String V, uint L, Window * P ):
            x(D.x),
            y(D.y),
            w(D.z),
            h(D.w),
            value(V),
            mmn(M),
            mma(M),
            length(L)
            {
                ResourcePtr ma = Ogre::MaterialManager::getSingleton().getByName(mmn + ".active");

                if ( ! ma.isNull() )
                    mma += ".active";

                mO = P->mGUI->createOverlay(P->mO->getName() + "t"
                                                + StringConverter::toString(P->mGUI->tc ++),
                                            Vector2(x, y),
                                            Vector2(w, h),
                                            M,
                                            "",
                                            false);
                mCP = P->mGUI->createOverlay(mO->getName() + "c", Vector2(4, (h - P->mGUI->mFontSize) / 2),
                                             Vector2(w, h),       "",
                                             V,                   false);
                P->mO->addChild(mO);
                mO->show();
                mO->addChild(mCP);
                mCP->show();
            }

        bool Window::checkKey ( String k, uint px, uint py )
            {
                if ( ! mO->isVisible() )
                    return false;

                if ( ! (px >= x && py >= y) || ! (px <= x + w && py <= y + h) )
                    return false;

                if ( mATI == 0 )
                    return false;

                if ( k == "!b" )
                    {
                        mATI->setValue(mATI->value.substr(0, mATI->value.length() - 1));
                        return true;
                    }

                if ( mATI->value.length() >= mATI->length )
                    return true;

                mATI->mCP->setCaption(mATI->value += k);
                return true;
            }

        bool Window::check ( uint px, uint py, bool LMB )
            {
                if ( ! mO->isVisible() )
                    return false;

                if ( ! (px >= x && py >= y) || ! (px <= x + w && py <= y + h) )
                    {
                        if ( mAB )
                        {
                            mAB->activate(false);
                        }

                        return false;
                    }

                if ( mAB )
                {
                    mAB->activate(false);
                }

                for ( uint i = 0; i < mB.size(); i ++ )
                    {
                        if ( mB[i]->in(px, py, x, y) )
                            continue;

                        if ( mAB )
                        {
                            mAB->activate(false);
                        }

                        mAB = mB[i];
                        mAB->activate(true);

                        if ( ! LMB )
                            return true;

                        if ( mATI )
                            {
                                mATI->mO->setMaterialName(mATI->mmn);
                                mATI = 0;
                            }

                        switch ( mAB->callback.t )
                            {
                                default:
                                    return true;

                                case 1:
                                    mAB->callback.fp(mAB);
                                    return true;

                                case 2:
                                    mAB->callback.LS->onButtonPress(mAB);
                                    return true;

                                case 3:
                                    mO->setPosition(x = px - (mAB->w / 2), y = py - (mAB->h / 2));
                                    return true;

                                case 4:
                                    mO->setDimensions(w = px - x + 8, h = py - y + 8);
                                    mRZ->mO->setPosition(mRZ->x = w - 16, mRZ->y = h - 16);

                                    if ( mTB )
                                    {
                                        mTB->mO->setWidth(mTB->w = w);
                                    }

                                    return true;
                            }
                    }

                if ( ! LMB )
                    return true;

                for ( uint i = 0; i < mT.size(); i ++ )
                    {
                        if ( mT[i]->in(px, py, x, y) )
                            continue;

                        mATI = mT[i];
                        mATI->mO->setMaterialName(mATI->mma);
                        return true;
                    }

                if ( mATI )
                    {
                        mATI->mO->setMaterialName(mATI->mmn);
                        mATI = 0;
                    }

                return true;
            }
    } // End of Betajaen's GUI. Normal programming can resume now.

#endif
