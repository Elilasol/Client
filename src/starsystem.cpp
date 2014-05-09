#include <string>
#include <math.h>       /* log */
#include "starsystem.h"

	/*
	"15000,700000,x,y,0", "false,false,barren,2500,1",
	"false,co2,barren,6000,2", "h2o,n2/o2,gaia,6300,3", "false,co2,barren,3400,4",
	"false,h2/he,gasgiant,71000,5", "false,h2/he,gasgiant,60000,6", "false,h2/he,gasgiant,25000,7",
	"false,h2/he,gasgiant,25000,8", "false,n-ice,ice,1100,9"
	*/
StarSystem::StarSystem(Ogre::SceneManager *sceneMgr, std::string *bodyDefinition) {
	Ogre::SceneNode *systemNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,0));

	createOrbits(systemNode, sceneMgr, bodyDefinition);

	float PositionOffset = 0.0;
	float distance = -1000.0;

	systemNode->translate(0, PositionOffset, distance);
}

StarSystem::~StarSystem() {
}

void StarSystem::createOrbits(Ogre::SceneNode *systemNode, Ogre::SceneManager *sceneMgr, std::string *bodyDefinition) {
	for (int repitition = 0;repitition < 10;repitition++){
		orbits[repitition].defineOrbit(systemNode, sceneMgr, bodyDefinition[repitition]);
	}
}

void StarSystem::rotateOrbits(){
	for (int repitition = 0;repitition < 10;repitition++){
		orbits[repitition].orbitSun();
	}
}