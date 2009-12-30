#ifndef __mousePicker_H__
#define __mousePicker_H__


#include "nxOgre.h"
#include "Ogre.h"

using namespace Ogre;
using namespace nxOgre;

class mousePicker {

protected:

	body *targetBody;
	SceneNode* targetNode;
	bool hasTargetBody;
	rayCaster* mRayCaster;
	Ogre::SceneManager	*__mgr;
	Ogre::Camera *__cam;

	int winHeight;
	int winWidth;

	bool firstFrame;

	void pickerSetup(Ogre::SceneManager *mgr, Ogre::Camera *cam, scene *_scene, int w, int h) {
		__mgr = mgr;
		__cam = cam;
		winHeight = h;
		winWidth = w;
		targetBody = 0;
		hasTargetBody = false;
		targetNode = __mgr->getRootSceneNode()->createChildSceneNode("target");
		Entity *_entity = __mgr->createEntity("nx.bodyguide.ent", "nx.bodyguide.mesh");
		_entity->setCastShadows(false);
		targetNode->attachObject(_entity);
		targetNode->scale(0.5,0.5,0.5);
		targetNode->setVisible(false);
		mRayCaster = new rayCaster(Vector3::ZERO, Vector3::ZERO, _scene, 200);
		firstFrame = true;
	}

	void mouseMoveOrGrab(float x, float y, bool move_or_grab) {

		Ogre::Ray r = __cam->getCameraToViewportRay(
			float(x / winWidth),
			float(y / winHeight)
		);

		mRayCaster->setOrigin(r.getOrigin());
		mRayCaster->setDirection(r.getDirection());

		if (!mRayCaster->cast()) {
			return;
		}

		// Move it
		if (move_or_grab) {
			
			if (hasTargetBody) {

				Ogre::Plane p;
				p.normal = r.getDirection();
				p.d = -mRayCaster->mHitPos.dotProduct(p.normal);

				std::pair<bool, Real> res;
				res = r.intersects(p);
				Vector3 objPoint;

				if (res.first) {

					Vector3 force = r.getPoint(res.second) - targetBody->getGlobalPosition();
					force *= 10;
					force -= NxTools::convert(targetBody->mActor->getPointVelocity(NxTools::convert(targetBody->getGlobalPosition())));
					targetBody->addForceAtPos(force, targetBody->getGlobalPosition());

				}

			}

			return;
		}

		// Grab it
		if (!move_or_grab) {

			body *t = mRayCaster->mHitBody;

			if (t->isStatic()) {
				targetBody = 0;
				hasTargetBody = false;
				return;
			}

			// Can't add forces to Kinematics.
			if (t->isKinematic())
				return;

			if (t == targetBody)
				return;

			targetBody = t;
			hasTargetBody = true;

		}

	}

	void pickerFrame() {

#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 3
		if (firstFrame) {
			__cam->setPosition(10,10,10); 
			__cam->lookAt(0,0,0);
			firstFrame = false;
		}
#endif

		if (hasTargetBody == false)
			targetNode->setVisible(false);
		else {
			Vector3 p = targetBody->getGlobalPosition();
			p.y = 0.01;
			targetNode->setPosition(p);	
			targetNode->setVisible(true);
		}
	}

};




class mouseKineticPicker {

protected:

	body *targetBody;
	SceneNode* targetNode;
	bool hasTargetBody;
	rayCaster* mRayCaster;
	Ogre::SceneManager	*__mgr;
	Ogre::Camera *__cam;

	int winHeight;
	int winWidth;

	void pickerSetup(Ogre::SceneManager *mgr, Ogre::Camera *cam, scene *_scene, int w, int h) {
		__mgr = mgr;
		__cam = cam;
		winHeight = h;
		winWidth = w;
		targetBody = 0;
		hasTargetBody = false;
		targetNode = __mgr->getRootSceneNode()->createChildSceneNode("target");
		Entity *_entity = __mgr->createEntity("nx.bodyguide.ent", "nx.bodyguide.mesh");
		_entity->setCastShadows(false);
		targetNode->attachObject(_entity);
		targetNode->scale(0.5,0.5,0.5);
		targetNode->setVisible(false);
		mRayCaster = new rayCaster(Vector3::ZERO, Vector3::ZERO, _scene, 200);
	}

	void mouseMoveOrGrab(float x, float y, bool move_or_grab) {

		Ogre::Ray r = __cam->getCameraToViewportRay(
			float(x / winWidth),
			float(y / winHeight)
		);

		mRayCaster->setOrigin(r.getOrigin());
		mRayCaster->setDirection(r.getDirection());

		if (!mRayCaster->cast()) {
			return;
		}

		// Move it
		if (move_or_grab) {
			
			if (hasTargetBody) {
				mRayCaster->mHitPos.y = 0.5;
				targetBody->setGlobalPosition(mRayCaster->mHitPos);
			}

			return;
		}

		// Grab it
		if (!move_or_grab) {

			body *t = mRayCaster->mHitBody;

			if (t->isStatic()) {
				targetBody = 0;
				hasTargetBody = false;
				return;
			}

			// Can't add forces to Kinematics.
			//if (t->isKinematic())
			//	return;

			if (t == targetBody)
				return;

			targetBody = t;
			hasTargetBody = true;

		}

	}

	void pickerFrame() {
		if (hasTargetBody == false)
			targetNode->setVisible(false);
		else {
			Vector3 p = targetBody->getGlobalPosition();
			p.y = 0.01;
			targetNode->setPosition(p);	
			targetNode->setVisible(true);
		}
	}

};
#endif