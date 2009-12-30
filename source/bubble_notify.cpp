#include "stdafx.h"

BubbleNotify::BubbleNotify(){
	//init(Vector2(59,132),Vector2(159,125),"bubble","myBubble");
	init(Vector2(.05761,.1718),Vector2(.15527,.1627),"bubble","myBubble");
}
BubbleNotify::~BubbleNotify(){
	delete _text;
}
BubbleNotify * BubbleNotify::getSingleton(){
	static BubbleNotify*  _bubble;
	if (_bubble == NULL)
	{
		_bubble = new BubbleNotify();
	}
	return _bubble;
}
void BubbleNotify::init(Vector2 pos, Vector2 size, String matName, String name){

	if (_text == NULL)
	{
		_text = new OgreText();
	}
	
	_pos = pos;
	OverlayManager & overlay_man = OverlayManager::getSingleton();

	static int id = 1;
	OverlayContainer *
		HUD_Item =
		static_cast<OverlayContainer *>(overlay_man.createOverlayElement("Panel","item." + StringConverter::toString(id)));
	
	HUD_Item->setMetricsMode(GMM_RELATIVE);
	HUD_Item->setPosition(pos.x, pos.y);
	HUD_Item->setDimensions(size.x, size.y);
	HUD_Item->setMaterialName(matName);
	_overalay = overlay_man.create(name + StringConverter::toString(id));
	_overalay->add2D(HUD_Item);
	_overalay->setZOrder(509);
	_active = false;
	++id;
}

void BubbleNotify::hide(){
	_overalay->hide();
	_text->setText(" ");
	_active = false;
}
void BubbleNotify::setMessage(Ogre::String text,Real height, float displayTime){
	
	_text->setText(text);
	_text->setHeight(height);
	//_text->setPos(_pos.x+79, _pos.y + 51);
	_text->setPos(_pos.x+.0771, _pos.y + .0664);
	_text->setCol(0,0,0,1);
	_text->setAlign(TextAreaOverlayElement::Alignment::Center);
	_overalay->show();
	_displayTime = displayTime;
	_initTime = timeGetTime();
	_active = true;
}
void BubbleNotify::show(){
	
}
void BubbleNotify::update(){
	if (!_active)
	{
		return;
	}
	_thisTime = timeGetTime();
	float diff = (_thisTime - _initTime) / 1000;
	if (diff >= _displayTime)
	{
		_overalay->hide();
		_text->setText(" ");
		//SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_DEACTIVATE);
		_active = false;
	}
}
OgreText * BubbleNotify::_text;