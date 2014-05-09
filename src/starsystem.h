#include "Ogre.h"
#include "orbit.h"

class StarSystem {

public:
	StarSystem(Ogre::SceneManager *sceneMgr, std::string *bodyDefinition);
	virtual ~StarSystem();

public:
	void createOrbits(Ogre::SceneNode *systemNode, Ogre::SceneManager *sceneMgr, std::string *bodyDefinition);
	void rotateOrbits();

protected:
	Orbit orbits[10];
};
