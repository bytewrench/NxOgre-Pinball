#include "stdafx.h"

intermission::intermission(){
}

void intermission::init(){
	OverlayManager & overlay_man = OverlayManager::getSingleton();
	hud_intermission = static_cast<OverlayContainer *>(overlay_man.createOverlayElement("Panel","intermission"));
	hud_intermission->setMetricsMode(GMM_RELATIVE);
	hud_intermission->setPosition(0, 0);
	hud_intermission->setDimensions(1, 1);
	hud_intermission->setMaterialName("intermission_material");
	overalay_intermission = overlay_man.create("intermission_overlay");
	overalay_intermission->add2D(hud_intermission);
	overalay_intermission->setZOrder(500);
}
void intermission::show(){
	overalay_intermission->show();
	visible = true;
}
void intermission::hide(){
	overalay_intermission->hide();
	visible = false;
}
void intermission::update(float time){
	col.a += INTERMISSION_FADE_RATE * time;
	if (col.a > 1)
	{
		col.a = 1;
	}
	hud_intermission->setColour(ColourValue(col.a,col.r,col.g,col.b));
}