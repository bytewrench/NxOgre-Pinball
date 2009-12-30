#include "stdafx.h"


class genericScene : public GameEngine, public Ogre::FrameListener
{
public:

	genericScene();
	~genericScene();

	
	virtual void start() {
		mWorld = new World("log: html");
		mScene = mWorld->createScene("Main", mSceneMgr, "gravity: yes, floor: yes");
		mScene->createBody("cube.1m.mesh", new CubeShape(1), Vector3(0,2.5f,0), "mass: 10");
	}
	virtual void stop() {
		delete mWorld;
	}


	void onFramePre(float deltaTime);
	virtual void onFrame(float deltaTime) {}
	void startRendering(genericScene* t);

	virtual World* getWorld() {return 0;}

	World				*mWorldInstance;
	Actor*				 mTargetActor;
	Cloth*				 mTargetCloth;
	ClothVertex			 mTargetClothVertex;

protected:


	Renderer*			mRenderer;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera*		mCamera;
	std::map<Ogre::String, Ogre::String> mRenderConfiguration;
};