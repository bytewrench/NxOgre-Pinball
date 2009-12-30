#include "stdafx.h"
//---------------------------------------------------------------
numberMachine::numberMachine ()
{
	val = 0;
	pos = NULL;
	size = 0;
	size_current = 0;
	size_exaggerated = 0;
	cyclespeed = NULL;
	effectLast = NULL;
	effectNow = NULL;
	spacing = 0.0f;
	justify = 1; /* 1=LEFT 2=CENTER 3=RIGHT */
	
}
numberMachine::~numberMachine(){
	delete txtItem;
}
void numberMachine::UpdateValue ( int Amount )
{
	val = Amount;
	effectNow = (DWORD)timeGetTime();
	effectLast = (DWORD)timeGetTime();
	size_current = size_exaggerated;
}

void numberMachine::setPosition(Vector2 pos){
	txtItem->setPos(pos.x,pos.y);
}
void numberMachine::setColor(Vector4 col){
	txtItem->setCol(col.x,col.y,col.z,col.w);
}
void numberMachine::create ( Vector2 position,TextAreaOverlayElement::Alignment just, Real size_,  float size_ex,
							float cycspd,String name, String preText )
{
	pos = position;
	size = size_;
	size_exaggerated = size_ex;
	size_current = size_;
	cyclespeed = cycspd;
	_name = name;
	txtItem = new OgreText();
	txtItem->setHeight(size);
	txtItem->setCol(1,1,1,1);
	txtItem->setPos(position.x,position.y);
	txtItem->setAlign(just);
	_preText = preText;
}

void numberMachine::render (float diff)
{

	if ( size_current > size )
	{
		size_current -= cyclespeed * diff;
	}
	else
	{
		size_current = size;
	}

	if (_name == "balls")
	{
		int remain = (gamedata::getSingleton()->NUMBALLS) - val;
		if (remain <= gamedata::getSingleton()->NUMBALLS)
		{
			String DisplayText = Ogre::StringConverter::toString(remain+1) + " of " + StringConverter::toString(gamedata::getSingleton()->NUMBALLS);
			txtItem->setText(DisplayText);
		}else{
			txtItem->setText("");
		}
		
		
	}else{
		if (_preText != "")
		{
			txtItem->setText(_preText + " " + Ogre::StringConverter::toString(val));
		}else{
			txtItem->setText(Ogre::StringConverter::toString(val));
		}
		
	}
	
	txtItem->setHeight(size_current);

}
void numberMachine::kill(){
	delete txtItem;
}
void numberMachine::reset(){
	txtItem->setText("");
}