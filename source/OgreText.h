#ifndef _OGRETEXT
#define _OGRETEXT


#include "stdafx.h"


class OgreText
{
public:

	OgreText();
	~OgreText();
	void setText(String szString/*char *szString*/);
	void setHeight(Real height);
	void setPos(Real x,Real y);
	void setCol(float R,float G,float B,float I);
	void setAlign(TextAreaOverlayElement::Alignment a);
	void setVisible(bool vis);
	Real getRelCoordx(Real inVal);
	Real getRelCoordy(Real inVal);
	
private:
	OverlayManager *olm;
	OverlayContainer *panel ;
	Overlay *overlay;
	TextAreaOverlayElement *textArea;
	
	String szElement;




};



#endif