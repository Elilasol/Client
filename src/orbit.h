#include "Ogre.h"
#include "celestialbody.h"

class Orbit {

public:
	Orbit();
	virtual ~Orbit();

public:
	void getColor(double *color);
	void defineOrbit(Ogre::SceneNode *systemNode, Ogre::SceneManager *sceneMgr, std::string bodyDefinition);
	void setOrbit(int orbitLocation);
	void orbitSun();

protected:	
	int orbit;
	double orbitSpeed;
	int inclination;
	Ogre::SceneNode *orbitNode;
	Ogre::SceneNode *rotationNode;
	CelestialBody body;
	
};
