#ifndef _PINBALLGAME

#define _PINBALLGAME

#include "stdafx.h"

static QuadManager * quadManager;


static int currentVelocity = 100;

static int DAUBERCOUNT = 0;
static DWORD CurTime, LastTime;
static String myBoard;
static int thisNumber;

#include "physicsVars.h"

#include "flipper.h"
#include "sphere.h"
#include "thumper.h"
#include "kicker.h"
#include "timed_kicker.h"
#include "target.h"
#include "stopper.h"
#include "table.h"
#include "trigger.h"


class GameEngine
    {
        public:

            GameEngine();
            ~GameEngine();
            World * mWorldInstance;
            void initNxOgre();
            virtual World * getWorld ();
            void setup();
            void pre( LPSTR lpCmdLine);
            void post();
			void processargs(int argc, char * argv [] );
            virtual void stop ();
            Ogre::Overlay * mOverlay;
            void onFramePre(float deltaTime);
            Ogre::OverlayContainer * createOverlay ( Ogre::String name,  Vector2 position,
                                                     Vector2 dimensions, Ogre::String material = "",
                                                     Ogre::String caption = "" );

			void GameEngine::processArgs (int argc, char * argv [] );
            bool showSlowFrameWarning;

        protected:
            Ogre::Timer* slowFrameWarning;
            static const int slowFrameWarningDelay = 5000; //milliseconds until slow frame warning
            void getConfig();
            Viewer * mViewer;
            Renderer * mRenderer;
            Ogre::SceneManager * mSceneMgr;
            Ogre::Camera * mCamera;
            //std::map<Ogre::String, Ogre::String> mRenderConfiguration;
            bool showFps;
    };

class PinballGame :
    public GameEngine, public Ogre::FrameListener, public OIS::KeyListener
    {
        public:

            float _time;
            Ogre::String mTutorialName;
            Ogre::String mTutorialDescription;
            bool mShutdownNow;
            bool firstFrame;
            float timeSince;
            Real tableSlope;

            OgreText * otFPS;
            OgreText * otUserMessage;
            //OgreText * otLoadingStat;
            numberMachine * nmBingos;
            numberMachine * nmScore;
            numberMachine * nmPower;
            numberMachine * nmBalls;
            numberMachine * nmBumps;
            numberMachine * nmLastScore;
            OgreText * otHighScore;
            OgreText * otPlayerInitials;
            OgreText * otIntermissionHeader;
            OgreText * otIntermissionSubText;
            OgreText * otSoftwareVersion;
            //OgreText * otBingoScores[11];
            OgreText * otAllNumbers[6];
            hitNumberManager * hnm;

            std::vector<std::string> szVecStandingsGuid;

            int _plungerState;
            float _plungerStrength;
            bool _ballRunning;
            int _ballsRemaining;
            int _maxBumps;
            bool _ballDeducted;
            bool _canBump;
            DWORD _thisBumpTime, _lastBumpTime;
            DWORD _thisKeyTime, _lastKeyTime;
            Vector3 _target_LastPosition;
	
			TablePin * _pin[3];


			SceneNode * nodeShock;
			


            intermission * _intermission_screen;
            PinBall * _pinBall[1];
            thumper * _thumper[4];
            kicker * _rearKickers[2];
            kicker * _midKickers[2];
            stopper * _stopper;
            timedKicker * _timed_kicker;
            flipper * _flipper[2];
            table * _mainTable;
            target * _targetbank_1;
            target * _targetbank_2;
            target * _targetbank_lanes;
            target * _targetbank_3;
            target * _targetbank_4;
            target * _targetbank_center;
            target * _targetbank_drains;

            Body * _targetBody;
            Body * _oldTargetBody;
            SceneNode * _mTargetNode;

			myTrigger * trigger[25];


            float delayTimerStart;

            DWORD ballResetTime;
            bool InetOK;

            bool allNumbers[5];
            bool DoEnterInitials;
            int initialsInputCounter;
            bool scoreposted;
            char * guidString;

            OgreText * txtUsernames[15];
            OgreText * txtScores[15];

            float bingoBallMarkerPos;

            CubeShape * cube;

            PinballGame();
            ~PinballGame();

            void RunGame();

            void pinballFrame(float deltaTime);

            bool frameStarted(const Ogre::FrameEvent & evt);
            bool frameEnded(const Ogre::FrameEvent & evt);

            void createInputSys(RenderWindow * _window);
            void captureInput();
            bool isKeyDown(Keys k);
            bool keyReleased(const KeyEvent & arg);
            bool keyPressed(const KeyEvent & arg);

            void spinBingo();

            void initPinball();
            void createTriggers();
            void initBoard();
            void preloadText();
            void buildTable();
            void ResetObject(Ogre::Real x, Ogre::Real y, Ogre::Real z, NxForceMode _forceMode = NX_FORCE);

            void addTextOverlays();
            void addOverlays();
            void prestop();
            void initGui();

            void displayStandings();
            void resetHudText();
            void hideStandings();
            void updateKickers();
            void updateBalls(float _time);
            void updateFlippers();
            void updatePlunger(float _time);
            void updateGui(float diff);
            void fireBall(int ballID);
            void checkIntermissionState();
            void resetBall(int ballID);
            void logMes(String txt);
            void bumpBall(int ballID);

            void stop();

        protected:
            
        private:

            std::map<Keys, OIS::KeyCode> mKeys;
    };

#endif
