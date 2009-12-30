#include "stdafx.h"

InputHandler::InputHandler() 
: mRoot(0),
mCamera(0),
mKeyboard(0),
mMouse(0)
{
	__resetKeys();
}

///////////////////////////////////////////////////////////////////////////////

InputHandler::~InputHandler() {
	
}


InputHandler * InputHandler::getInputHandler(){
	static InputHandler * _inputHandler;
	if(_inputHandler == 0){
		_inputHandler = new InputHandler();
	}
	return _inputHandler;
}

///////////////////////////////////////////////////////////////////////////////

void InputHandler::__resetKeys() {

	mKeys.clear();

	// #KEYCONFIG
	mKeys[QUIT] = KC_ESCAPE;
	mKeys[SCREENSHOT] = KC_F1;
	mKeys[DEBUG_MODE] = KC_F2;
	mKeys[POPUP] = KC_F3;
	mKeys[RETRACT] = KC_F4;

	mKeys[FLIP_LEFT1] = KC_LSHIFT;
	mKeys[FLIP_LEFT2] = KC_Q;
	mKeys[FLIP_LEFT3] = KC_A;
	mKeys[FLIP_LEFT4] = KC_Z;
	mKeys[FLIP_LEFT5] = KC_W;
	mKeys[FLIP_LEFT6] = KC_D;
	mKeys[FLIP_LEFT7] = KC_X;
	mKeys[FLIP_LEFT8] = KC_E;
	mKeys[FLIP_LEFT9] = KC_R;
	mKeys[FLIP_LEFT10] = KC_F;

	mKeys[FLIP_RIGHT1] = KC_RSHIFT;
	mKeys[FLIP_RIGHT2] = KC_L;
	mKeys[FLIP_RIGHT3] = KC_P;
	mKeys[FLIP_RIGHT4] = KC_M;
	mKeys[FLIP_RIGHT5] = KC_O;
	mKeys[FLIP_RIGHT6] = KC_K;
	mKeys[FLIP_RIGHT7] = KC_J;
	mKeys[FLIP_RIGHT8] = KC_I;
	mKeys[FLIP_RIGHT9] = KC_N;
	mKeys[FLIP_RIGHT10] = KC_U;

	mKeys[FIRE_BALL1] = KC_SPACE;
	mKeys[FIRE_BALL2] = KC_RETURN;

	mKeys[NEW_GAME] = KC_S;
	mKeys[POST_SCORE] = KC_Y;
	mKeys[NO_POST_SCORE] = KC_N;
	mKeys[SEND_SCORE] = KC_RETURN;
}

///////////////////////////////////////////////////////////////////////////////

void InputHandler::start(Ogre::RenderWindow* window, Ogre::Camera*) {
	// create mMouse

	ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	window->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = InputManager::createInputSystem( pl );

	//Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
	mKeyboard = static_cast<Keyboard*>(mInputManager->createInputObject( OISKeyboard, false ));
	mMouse = static_cast<Mouse*>(mInputManager->createInputObject( OISMouse, false));

	std::cout << mKeyboard << std::endl;
	std::cout << mMouse << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

void InputHandler::stop() {
	mInputManager->destroyInputObject(mMouse);
	mInputManager->destroyInputObject(mKeyboard);
	InputManager::destroyInputSystem(mInputManager);

	//	OIS::InputManager::destroyInputObject(mMouse);
	//	OIS::InputManager::destroyInputObject(mKeyboard);
}

///////////////////////////////////////////////////////////////////////////////

void InputHandler::onFrame(float deltaTime) {
	mKeyboard->capture();
	mMouse->capture();
}

///////////////////////////////////////////////////////////////////////////////
