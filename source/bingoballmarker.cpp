
#include "stdafx.h"

//#define MARKERINIT_POSY 258
#define MARKERINIT_POSY .3359
//#define MARKERINIT_POSX 988
#define MARKERINIT_POSX .9648
//#define MARKER_INCREMENT 21.5
#define MARKER_INCREMENT .02799

#define MARKER_ANIM_SPEED 50
bingoballmarker::bingoballmarker(){

	OverlayManager & overlay_man = OverlayManager::getSingleton();
	hud_ball = static_cast<OverlayContainer *>(overlay_man.createOverlayElement("Panel","hudball_animation"));
	hud_ball->setMetricsMode(GMM_RELATIVE);
	hud_ball->setPosition(MARKERINIT_POSX, MARKERINIT_POSY);
	//hud_ball->setDimensions(32, 32);
	hud_ball->setDimensions(.03125, .03125);
	hud_ball->setMaterialName("bingoballmarker");
	overalay_ball = overlay_man.create("hudball_overlay");
	overalay_ball->add2D(hud_ball);
	overalay_ball->setZOrder(499);
	overalay_ball->show();
	_currentPos = MARKERINIT_POSY;
	_destPos = _currentPos;
}
bingoballmarker * bingoballmarker::getSingleton(){
	static bingoballmarker * bbm;
	if (bbm == NULL)
	{
		bbm = new bingoballmarker();
	}
	return bbm;
}
void bingoballmarker::hide(){
	hud_ball->hide();
}
void bingoballmarker::step(int bingoCount){

	_destPos =  MARKERINIT_POSY - (bingoCount* MARKER_INCREMENT);
	//LogManager::getSingletonPtr()->logMessage(StringConverter::toString(_destPos));
}
void bingoballmarker::update(float _time){
	
	
	if (_currentPos > _destPos)
	{
		_currentPos -= MARKER_ANIM_SPEED * _time;
		if (_currentPos < _destPos)
		{
			_currentPos = _destPos;
		}
	}else if (_currentPos < _destPos){
		_currentPos+= MARKER_ANIM_SPEED * _time;
		if (_currentPos > _destPos)
		{
			_currentPos = _destPos;
		}
	}
	hud_ball->setPosition(MARKERINIT_POSX, _currentPos);
}