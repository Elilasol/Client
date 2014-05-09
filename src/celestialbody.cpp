#include <string>
#include "celestialbody.h"
#include <math.h>       /* log */

CelestialBody::CelestialBody() {
	planetMesh = "sphereMesh";
}

CelestialBody::~CelestialBody() {
}

void CelestialBody::rotateBody(){
	starNode->rotate(Ogre::Vector3::UNIT_Y,Ogre::Degree(starRotationDegree));
}

void CelestialBody::setAttributes(Ogre::SceneNode *orbitNode, Ogre::SceneManager *sceneMgr, int radius, std::string surfaceLiquid, std::string atmosphere, std::string biome){
	std::string bodyName = "body" + Ogre::StringConverter::toString(radius);
	Ogre::Entity *earthEntity = sceneMgr->createEntity(bodyName, planetMesh);

	//Material Gen
	earthEntity->setMaterialName(biome);

	orbitNode->attachObject(earthEntity);
	earthEntity->getParentNode()->scale(0.1,0.1,0.1);
}
void CelestialBody::setAttributes(Ogre::SceneNode *particleNode, Ogre::SceneManager *sceneMgr, int temperature, double radius, int x, int y) {
	kelvin = temperature;
	radius = radius;
	int rand2 = rand()%1000;
	double colorEvener = 0.35;

	if (kelvin >= 1000){
		std::string starNodeName = "Star" + Ogre::StringConverter::toString(temperature) + Ogre::StringConverter::toString(rand2);
		std::string starParticleName = "starParticle" + Ogre::StringConverter::toString(temperature) + Ogre::StringConverter::toString(rand2);

		//Particle Gen
		Ogre::ParticleSystem* starParticle = sceneMgr->createParticleSystem(starParticleName, "StarWhite");
		particleNode->attachObject(starParticle);
		setColor(color);
		starParticle->getEmitter(0)->setColour(Ogre::ColourValue(color[0]*colorEvener,color[1]*colorEvener,color[2]*colorEvener,color[3]));

		//Light Gen
		Ogre::Light* lightPoint = sceneMgr->createLight();
		lightPoint->setType(Ogre::Light::LT_POINT);
		lightPoint->setDiffuseColour(color[0], color[1], color[2]);
		lightPoint->setSpecularColour(color[0], color[1], color[2]);

		particleNode->attachObject(lightPoint);
	}
	else {
		
	}

	//Rotation
	starRotationDegree = 1.0;
}

void CelestialBody::setColor(double *color) {
		
	double temperature = (kelvin-1000)/100;
	double leveler = 255.0/390.0;
	double multiplier = leveler * temperature;
    
	double red;
	double green;
	double blue;

    //Calculate Red:
	red = 127.5*(cos(0.00013*(pow(multiplier,2))))+127.5;

    if (red < 0) 
		red = 0;
    if (red > 255) 
		red = 255;



    //Calculate Green:
	green = -127.5*(sin(0.00011*(pow(multiplier,2))))+127.5;
	
    if (green < 0)
		green = 0;
    if (green > 255)
		green = 255;

	

    //Calculate Blue:
	blue = -127.5*(cos(0.00015*(pow(multiplier,2))))+127.5;

    if (blue < 0)
		blue = 0;
    if (blue > 255)
		blue = 255;

	color[0] = (1.0/255) * (red);
	color[1] = (1.0/255) * (green);
	color[2] = (1.0/255) * (blue);
	color[3] = 1.0;
}