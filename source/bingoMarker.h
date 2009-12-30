#ifndef _BINGOMARKER
#define _BINGOMARKER

#include "stdafx.h"
class BingoMarker
{
public:
	BingoMarker(Vector3 location, float angle/*, SceneManager * sm*/);
	void hide();
	void show();
	void setScale(Vector3 scale);
	bool _visible;



protected:
private:
	SceneNode * nodeMarker;
	Entity * entMarker;
};


#endif