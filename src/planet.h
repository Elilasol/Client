#include "Ogre.h"

class Planet {

public:
	Planet(double temperature, double radius, Ogre::SceneManager *sceneMgr);
	virtual ~Planet();

public:
	void setLook(double *color);

protected:
	double kelvin;
	double radius;
};
