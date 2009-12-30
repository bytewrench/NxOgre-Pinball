#include "stdafx.h"


table::table ( Ogre::SceneManager * SceneMgr )
{

	Scene * _mScene = mWorld->getScenes()->getFirst();

	
	NxOgre::Material * RubberMat = mScene->createMaterial("Wood");
	RubberMat->setRestitution(.3);
	RubberMat->setStaticFriction(.1);
	RubberMat->setDynamicFriction(.1);
	
	
	_mainTable = _mScene->createBody("table_collision",
		new NxOgre::TriangleMeshShape("table_collision.mesh","Group:TableGroup, material: Wood"),
		Vector3(0,0,0),"static: yes");
	
	_mainTable->getNxActor()->setName("Table");

	
	loadinganimation::getSingleton()->increment("Building(10)");
	Entity * Posts = SceneMgr->createEntity("posts", "posts.mesh");
	loadinganimation::getSingleton()->increment("Building(9)");
	Entity * Bands = SceneMgr->createEntity("bands", "Bands.mesh");
	loadinganimation::getSingleton()->increment("Building(8)");
	Entity * Covers = SceneMgr->createEntity("covers", "covers.mesh");
	loadinganimation::getSingleton()->increment("Building(7)");
	Entity * Floor = SceneMgr->createEntity("floor", "floor.mesh");
	loadinganimation::getSingleton()->increment("Building(6)");
	Entity * Walls = SceneMgr->createEntity("walls", "walls.mesh");
	loadinganimation::getSingleton()->increment("Building(5)");
	Entity * ScrewsRails = SceneMgr->createEntity("screws_rails", "metal.mesh");
	loadinganimation::getSingleton()->increment("Building(4)");
	Entity * LaneCovers = SceneMgr->createEntity("lanecovers", "lane_covers.mesh");
	loadinganimation::getSingleton()->increment("Building(3)");
	Entity * Targets = SceneMgr->createEntity("targets", "targets.mesh");

	
	//Entity * LightMap = SceneMgr->createEntity("light_map","fulltable_lightmap3.mesh");
	//LightMap->setMaterialName("tablelightbake.material");

	loadinganimation::getSingleton()->increment("Building(2)");
	
	
	Posts->setMaterialName("Gile[s]_posts");
	Bands->setMaterialName("Gile[s]_bands");
	Covers->setMaterialName("Gile[s]_covers");
	Floor->setMaterialName("Gile[s]_floor");
	Walls->setMaterialName("Gile[s]_walls");
	ScrewsRails->setMaterialName("Gile[s]_metals");
	LaneCovers->setMaterialName("Gile[s]_lanecovers");
	Targets->setMaterialName("Gile[s]_targets");
	
	Posts->setCastShadows(false);
	Covers->setCastShadows(false);
	Floor->setCastShadows(false);
	
	Bands->setCastShadows(false);
	Walls->setCastShadows(false);
	ScrewsRails->setCastShadows(false);
	LaneCovers->setCastShadows(false);
	Targets->setCastShadows(false);
	
	const Vector3 _default(0,0,0);

	StaticGeometry * sg = SceneMgr->createStaticGeometry("TableParts");
	const int size=50;
	const int amount = 8;
	sg->setRegionDimensions(Vector3(20,10,60));
	sg->setOrigin(Vector3(10,0,30));
	sg->addEntity(Posts,_default);
	sg->addEntity(Bands,_default);
	sg->addEntity(Covers,_default);
	sg->addEntity(Floor,_default);
	sg->addEntity(Walls,_default);
	sg->addEntity(ScrewsRails,_default);
	sg->addEntity(LaneCovers,_default);
	sg->addEntity(Targets,_default);
	//sg->addEntity(LightMap,_default);
	sg->build();
	

	loadinganimation::getSingleton()->increment("Building(1)");



}