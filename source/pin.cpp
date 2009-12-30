#include "stdafx.h"

TablePin::TablePin ( Vector3 pos )
    {
        Scene * _mScene = mWorld->getScenes()->getFirst();

        NxOgre::Material * PinMat = mScene->createMaterial("Pin");
        PinMat->setRestitution(.8);
        String params = "static:yes";

        CapsuleShape * c = new CapsuleShape(.17, 1);
        c->mParams.setToDefault();
        c->mParams.mMaterialAsName = "Steel";

        _pinBody = _mScene->createBody("pin.mesh", c, pos, params);
		_pinBody->getEntity()->setMaterialName("Gile[s]_thumper_metal");
        
    }

TablePin::~TablePin ()
{
    delete _pinBody;
}
