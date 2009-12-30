#ifndef _BINGOBALLMARKER
#define _BINGOBALLMARKER

#include "stdafx.h"


class bingoballmarker
{
public:
	bingoballmarker();
	void update(float _time);
	void hide();
	void step(int bingoCount);
	static bingoballmarker * getSingleton();
protected:
private:
	Real _currentPos;
	Real _destPos;
	OverlayContainer * hud_ball;
	Overlay * overalay_ball;
};


#endif