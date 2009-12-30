#include "stdafx.h"
#include "generic.h"



genericScene::genericScene() {


}




///////////////////////////////////////////////////////////////////////////////

genericScene::~genericScene() {
	// delete input handler
	//delete mInputHandler;
	// delete renderer
	delete mRenderer;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

void genericScene::onFramePre(float deltaTime) {
	//OgreProfileBegin("MAIN");
	//if (mInputHandler->isKeyDown(QUIT)) {
	//	mRenderer->stop();
	//	return;
	//}

	
	//onFrame(deltaTime);
}

///////////////////////////////////////////////////////////////////////////////

void genericScene::startRendering(genericScene *t) {
	
	mTargetActor = 0;
	//mRenderer->bind(t, mInputHandler);
	mRenderer->start();
}
