#include <string>
#include "planet.h"
#include <math.h>       /* log */


Planet::Planet(double temperature, double radius, Ogre::SceneManager *sceneMgr) {
	
}

Planet::~Planet() {
}

void Planet::setLook(double *color) {
		{	
			/*
			Ogre::MaterialPtr lMaterial = lMaterialManager.create("Earth",lNameOfResourceGroup);
			Ogre::Technique* lFirstTechnique = lMaterial->getTechnique(0);
			Ogre::Pass* lFirstPass = lFirstTechnique->getPass(0);
			
			// Lighting is allowed on this pass.
			lFirstPass->setLightingEnabled(true);

			// Emissive / self illumination is the color 'produced' by the object.
			// Color values vary between 0.0(minimum) to 1.0 (maximum).
			Ogre::ColourValue lSelfIllumnationColour(0.0f, 0.0f, 0.1f, 0.3f);
			lFirstPass->setSelfIllumination(lSelfIllumnationColour);

			// diffuse color is the traditionnal color of the lit object.
			Ogre::ColourValue lDiffuseColour(0.1f, 0.2f, 0.8f, 1.0f);
			lFirstPass->setDiffuse(lDiffuseColour);

			// ambient colour is linked to ambient lighting.
			// If there is no ambient lighting, then this has no influence.
			// It the ambient lighting is at 1, then this colour is fully added.
			// This is often use to change the general feeling of a whole scene.
			Ogre::ColourValue lAmbientColour(0.0f, 0.1f, 0.4f, 1.0f);
			lFirstPass->setAmbient(lAmbientColour);

			// specular colour, is the colour of the 'little light reflection'
			// that you can see on some object. For example, my bald head skin
			// reflect the sun. This make a 'round of specular lighting'.
			// Set this to black if you don't want to see it.
			Ogre::ColourValue lSpecularColour(0.0f, 0.3f, 1.0f, 1.0f);
			lFirstPass->setSpecular(lSpecularColour);

			// Shininess is the 'inverse of specular color splattering' coefficient.
			// If this is big (e.g : 64) you get a very tiny dot with a quite strong color (on round surface).
			// If this is 0, you get a simple color layer (the dot has become very wide).
			Ogre::Real lShininess = 34.0f;
			lFirstPass->setShininess(lShininess);
			*/
	}
}