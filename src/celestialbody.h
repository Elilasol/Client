#include "Ogre.h"

class CelestialBody {

public:
	CelestialBody();
	virtual ~CelestialBody();

public:
	void setAttributes(Ogre::SceneNode *systemNode, Ogre::SceneManager *sceneMgr, int temperature, double radius, int x, int y);
	void setAttributes(Ogre::SceneNode *orbitNode, Ogre::SceneManager *sceneMgr, int radius, std::string surfaceLiquid, std::string atmosphere, std::string biome);
	void setColor(double *color);
	void rotateBody();

protected:
	int kelvin;
	double radius;
	double color[4];
	std::string planetMesh;
	std::string surfaceLiquid;
	std::string atmosphere;
	std::string biome;
	Ogre::Degree starRotationDegree;
	Ogre::SceneNode *starNode;

};