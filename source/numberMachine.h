#ifndef _NUMBERMACHINE
#define _NUMBERMACHINE

#include "stdafx.h"


class numberMachine : public OgreText
{
public:

	void UpdateValue(int Amount);
	void create(Vector2 position, TextAreaOverlayElement::Alignment just,Real size_,   float size_ex, float cycspd, String name, String preText = "");
	void render(float diff);
	void setPosition(Vector2 pos);
	void setColor(Vector4 col);
	void reset();
	numberMachine();
	~numberMachine();
	int val;
	String _name;
	void kill();
private:
	OgreText * txtItem;
	Vector2 pos;
	Real size;
	Real size_exaggerated;
	Real size_current;
	float cyclespeed;
	DWORD effectLast;
	DWORD effectNow;
	float spacing;
	int justify;
	String _preText;
};
#endif