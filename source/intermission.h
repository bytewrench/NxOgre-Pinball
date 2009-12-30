#ifndef _INTERMISSION
#define _INTERMISSION
#include "ogre.h"
using namespace Ogre;

class intermission
{
public:
	intermission();
	void init();
	void show();
	void hide();
	void update(float time);
	bool visible;
protected:
private:
	OverlayContainer * hud_intermission;
	Overlay * overalay_intermission;
	ColourValue col;
};
#endif