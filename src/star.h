#include "Ogre.h"

class Star {

public:
	Star(int temperature, double radius, Ogre::SceneNode *systemNode, Ogre::SceneManager *sceneMgr, int x, int y);
	virtual ~Star();

public:
	void setColor(double *color);
	void rotateStar();

protected:
	int kelvin;
	double radius;
	double color[4];
	Ogre::Degree starRotationDegree;
	Ogre::SceneNode *starNode;
};
