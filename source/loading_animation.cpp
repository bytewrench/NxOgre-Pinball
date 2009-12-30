#include "stdafx.h"


loadinganimation::loadinganimation(){

	
	otCaption.setHeight(18);
	//otCaption.setPos(957,640);
	otCaption.setPos(.9345,.8333);
	otCaption.setCol(1,1,1,1);
	otCaption.setAlign(TextAreaOverlayElement::Alignment::Center);

	OverlayManager & overlay_man = OverlayManager::getSingleton();
	hud_loading = static_cast<OverlayContainer *>(overlay_man.createOverlayElement("Panel","loading_animation"));
	hud_loading->setMetricsMode(GMM_RELATIVE);
	
	//hud_loading->setPosition(893, 640);
	hud_loading->setPosition(.872, .8333);
	//hud_loading->setDimensions(128, 128);
	hud_loading->setDimensions(.125, .1666);
	
	hud_loading->setMaterialName("la.1");
	overalay_loading = overlay_man.create("loading_overlay");
	overalay_loading->add2D(hud_loading);
	overalay_loading->setZOrder(650);
	overalay_loading->show();
	otCaption.setText("Preparing");
	

}
void loadinganimation::setCaption(String str){
	otCaption.setText(str);
}

loadinganimation * loadinganimation::getSingleton(){
	static loadinganimation * la;
	if (la == NULL)
	{
		la = new loadinganimation();
	}
	return la;
}
void loadinganimation::setRoot(Root * _root){
	if (_rootref == NULL)
	{
		_rootref = _root;
	}
	
}
void loadinganimation::increment(String action /*Root * root = NULL*/){

	if (!hud_loading->isVisible())
	{
		hud_loading->show();
	}

	if (action != "")
	{
		otCaption.setText(action);
	}
	
	static int deg = 360;

	static TextureUnitState * texture;
	if (texture == NULL)
	{
		texture = 
			hud_loading->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0);

	}
	texture->setTextureRotate(Degree(deg));
	deg-=5;
	if (deg < 0)
	{
		deg = 360;
	}
	if (_rootref != NULL)
	{
		_rootref->renderOneFrame();
	}
}
void loadinganimation::hide(){
	otCaption.setText("");
	hud_loading->hide();

}

Root * loadinganimation::_rootref;