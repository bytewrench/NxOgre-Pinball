#ifndef _LOADINGANIMATION
#define _LOADINGANIMATION

#include "stdafx.h"

class loadinganimation
{
public:
	loadinganimation();
	static loadinganimation * getSingleton();
	void increment(String action = ""/*Root * _root*/);
	void setRoot(Root * _root);
	void hide();
	void setCaption(String str);
protected:
private:
	OverlayContainer * hud_loading;
	Overlay * overalay_loading;
	static Root * _rootref;
	OgreText otCaption ;

};

#endif