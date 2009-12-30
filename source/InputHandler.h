#ifndef InputManager_H
#define InputManager_H


class InputHandler {

public:

	InputHandler();
	~InputHandler();

	void start(Ogre::RenderWindow*, Ogre::Camera*);
	void stop();
	void onFrame(float deltaTime);

	static InputHandler * getInputHandler();
	OIS::MouseState getMouseState() const {return mMouse->getMouseState();}
	
	OIS::Keyboard* mKeyboard;

	bool isKeyDown(Keys k) {
		return mKeyboard->isKeyDown(mKeys[k]);
	}

protected:

	void __resetKeys();

	Ogre::Root*    mRoot;
	Ogre::Camera*  mCamera;
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	
	std::map<Keys, OIS::KeyCode> mKeys;
};


#endif
