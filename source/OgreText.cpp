#include "stdafx.h"


static int init = 0;

OgreText::OgreText()
{
	olm=OverlayManager::getSingletonPtr();
	if(init==0)
	{
		panel=static_cast<OverlayContainer*>(olm->createOverlayElement("Panel","GUI"));
		//panel->setMetricsMode(Ogre::GMM_PIXELS);
		panel->setMetricsMode(Ogre::GMM_RELATIVE);
		panel->setPosition(0,0);
		//panel->setDimensions(128.0f,128.0f);
		panel->setDimensions(.125,.125);
		overlay=olm->create("GUI_OVERLAY");
		overlay->add2D(panel);
		overlay->setZOrder(510);
	}
	++init;
	szElement="element_"+StringConverter::toString(init);
	overlay=olm->getByName("GUI_OVERLAY");
	panel=static_cast<OverlayContainer*>(olm->getOverlayElement("GUI"));
	textArea=static_cast<TextAreaOverlayElement*>(olm->createOverlayElement("TextArea",szElement));
	panel->addChild(textArea);
	
	
	textArea->setMetricsMode(Ogre::GMM_RELATIVE);
	textArea->setDimensions(.1f,.1f);
	
	/*
	BlueHighway
	StarWars
	GroBold
	vino
	*/
	textArea->setFontName("BlueHighway");
	//textArea->setCharHeight(50);
	//textArea->setCaption("");
	overlay->show();
}
OgreText::~OgreText()
{
	szElement="element_"+StringConverter::toString(init);
	olm->destroyOverlayElement(szElement);
	--init;
	if(init==0)
	{
		olm->destroyOverlayElement("GUI");
		olm->destroy("GUI_OVERLAY");
	}
}
void OgreText::setText(String szString/*char *szString*/)
{
	textArea->setCaption(szString);
}
void OgreText::setHeight(Real height){
	Real _convertedHeight = getRelCoordy(height);
	try
	{
		textArea->setCharHeight(_convertedHeight);
	}
	catch (Ogre::Exception* e)
	{
		
	}

}
void OgreText::setPos(Real x,Real y)
{
	
	textArea->setPosition(x,y);

}
void OgreText::setCol(float R,float G,float B,float I)
{
	textArea->setColour(Ogre::ColourValue(R,G,B,I));
}
void OgreText::setAlign(TextAreaOverlayElement::Alignment a){
	textArea->setAlignment(a);
}
void OgreText::setVisible(bool vis){
	if (vis)
	{
		textArea->show();
	}else{
		textArea->hide();
	}

}
Real OgreText::getRelCoordx(Real inVal){

	return  inVal / 1024;
}
Real OgreText::getRelCoordy(Real inVal){
	return  inVal / 768;
}