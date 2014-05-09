#include <string>
#include "orbit.h"
#include <math.h>       /* log */
#include <sstream>
#include <vector>

Orbit::Orbit() {
}

Orbit::~Orbit() {
}

void Orbit::defineOrbit(Ogre::SceneNode *systemNode, Ogre::SceneManager *sceneMgr, std::string bodyDefinition){
	int rand2 = rand()%1000;

	Ogre::Degree inclination = Ogre::Degree(Ogre::Math::RangeRandom(-20, 20));

	//"15000,x,y,false,false,gasgiant,700000,0", "300,0,0,false,false,barren,2500,1"
	std::stringstream ss;
	ss.str(bodyDefinition);
	std::vector<std::string> split;
	std::string token;

	while(std::getline(ss,token,',')){
        split.push_back(token);
	}

	int temperature = atoi(split.at(0).c_str());
	int locationx = atoi(split.at(1).c_str());
	int locationy = atoi(split.at(2).c_str());
	std::string surfaceLiquid = split.at(3);
	std::string atmosphere = split.at(4);
	std::string biome = split.at(5);
	int radius = atoi(split.at(6).c_str());

	orbit = atoi(split.at(7).c_str());
	
	orbitSpeed = 0;
	if (orbit > 0)
		orbitSpeed = 1.0/orbit;

	int orbitX = 200*orbit;
	std::string bodyNodeName = "orbitNode" + split.at(7);
	std::string rotationNodeName = "rotationNode" + split.at(7);


	if (temperature > 999){
		orbitNode = systemNode->createChildSceneNode(bodyNodeName,Ogre::Vector3((-0.5+locationx),(0+locationy),0.5));

		body.setAttributes(orbitNode, sceneMgr, temperature, radius, locationx, locationy);
	}
	else{
		orbitNode = systemNode->createChildSceneNode(bodyNodeName,Ogre::Vector3((-0.5+locationx),(0),0.5));
		rotationNode = orbitNode->createChildSceneNode(rotationNodeName,Ogre::Vector3((-0.5+orbitX),(0),0.5));
		
		Ogre::Quaternion q(Ogre::Degree(inclination),Ogre::Vector3::UNIT_Z);
		orbitNode->rotate(q);

		body.setAttributes(rotationNode, sceneMgr, radius, surfaceLiquid, atmosphere, biome);

	}
	int earthRot = 50;
	orbitNode->rotate(Ogre::Vector3::UNIT_Y,Ogre::Degree(earthRot));
}

void Orbit::orbitSun(){
	orbitNode->rotate(Ogre::Vector3::UNIT_Y,Ogre::Degree(orbitSpeed));
}