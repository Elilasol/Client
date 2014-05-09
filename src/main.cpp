#include "input.h"
#include "simulation.h"
#include "spheremesh.h"
#include "starsystem.h"

#include "Ogre.h"

//Particle define?
 #include "OgreParticleAffectorFactory.h"
 #include "OgreParticleSystem.h"
 #include "OgreParticle.h"
#include "OgreParticleEmitterFactory.h"
#include "OgreParticleAffectorFactory.h"
#include "OgreParticle.h"
#include "OgreQuaternion.h"
#include "OgreParticleEmitter.h"


// needed to be able to create the CEGUI renderer interface
#include "RendererModules/Ogre/CEGUIOgreRenderer.h" 

// CEGUI includes
#include "CEGUISystem.h"
#include "CEGUIInputEvent.h"
#include "CEGUIWindow.h"
#include "CEGUIWindowManager.h"
#include "CEGUISchemeManager.h"
#include "CEGUIFontManager.h"
#include "elements/CEGUIFrameWindow.h"

// GUI sheet handlers
#include "MainMenuDlg.h"

#if defined(WIN32)
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main (int argc, char *argv[]) {
#endif

	#define OGRE_STATIC_ParticleFX 1

	Ogre::Root *ogre;
	Ogre::RenderWindow *window;
	Ogre::SceneManager *sceneMgr;
	Ogre::Camera *camera;

	// fire up an Ogre rendering window. Clearing the first two (of three) params will let us 
	// specify plugins and resources in code instead of via text file
	ogre = new Ogre::Root("", "");


	// This is a VERY minimal rendersystem loading example; we are hardcoding the OpenGL 
	// renderer, instead of loading GL and D3D9. We will add renderer selection support in a 
	// future article.

	// I separate the debug and release versions of my plugins using the same "_d" suffix that
	// the Ogre main libraries use; you may need to remove the "_d" in your code, depending on the
	// naming convention you use 
	ogre->loadPlugin("RenderSystem_GL_d");
	ogre->loadPlugin("Plugin_ParticleFX_d");
	const Ogre::RenderSystemList &renderSystems = ogre->getAvailableRenderers();
	Ogre::RenderSystemList::const_iterator r_it;

	// we do this step just to get an iterator that we can use with setRenderSystem. In a future article
	// we actually will iterate the list to display which renderers are available. 
	// renderSystems = ogre->getAvailableRenderers();
	r_it = renderSystems.begin();
	ogre->setRenderSystem(*r_it);
	ogre->initialise(false);

	// load common plugins
	ogre->loadPlugin("Plugin_CgProgramManager_d");		
	ogre->loadPlugin("Plugin_OctreeSceneManager_d");

	// setup main window; hardcode some defaults for the sake of presentation
	Ogre::NameValuePairList opts;
	opts["resolution"] = "1024x768";
	opts["fullscreen"] = "false";
	opts["vsync"] = "true";

	// create a rendering window with the title "CDK"
	window = ogre->createRenderWindow("CDK", 1024, 768, false, &opts);

		// load the basic resource location(s)
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		"resource", "FileSystem", "General");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		"resource/gui.zip", "Zip", "GUI");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		"resource/textures", "FileSystem", "Textures");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		"resource/particles", "FileSystem", "Particles");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		"resource/materials", "FileSystem", "Materials");
#if defined(WIN32)
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		"c:\\windows\\fonts", "FileSystem", "GUI"); 
#endif
	//Must initialize resource groups after window if using particle effects.
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("GUI");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Textures");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Materials");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Particles");

	// since this is basically a CEGUI app, we can use the ST_GENERIC scene manager for now; in a later article 
	// we'll see how to change this
	sceneMgr = ogre->createSceneManager(Ogre::ST_GENERIC);
	camera = sceneMgr->createCamera("camera");
	camera->setNearClipDistance(1);
    Ogre::Viewport* vp = window->addViewport(camera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	// most examples get the viewport size to calculate this; for now, we'll just 
	// set it to 4:3 the easy way
	camera->setAspectRatio((Ogre::Real)1.333333);

// with a scene manager and window, we can create a the GUI renderer
	
	// new way to instantiate a CEGUIOgreRenderer (Ogre 1.7)
	Ogre::RenderTarget *mRenderTarget = window;
	CEGUI::OgreRenderer* pGUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem(*mRenderTarget);
 
	// create the root CEGUI class
	CEGUI::System* pSystem = CEGUI::System::getSingletonPtr();
 
	// tell us a lot about what is going on (see CEGUI.log in the working directory)
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
 
	// use this CEGUI scheme definition (see CEGUI docs for more)
	CEGUI::SchemeManager::getSingleton().create((CEGUI::utf8*)"TaharezLookSkin.scheme", (CEGUI::utf8*)"GUI");
 
	// show the CEGUI mouse cursor (defined in the look-n-feel)
	pSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
 
	// use this font for text in the UI
	CEGUI::FontManager::getSingleton().create("Tahoma-8.font", (CEGUI::utf8*)"GUI");
	pSystem->setDefaultFont((CEGUI::utf8*)"Tahoma-8");
 
	// load a layout from the XML layout file (you'll find this in resources/gui.zip), and 
	// put it in the GUI resource group
	CEGUI::Window* pLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout("katana.layout", "", "GUI");
 
	// you need to tell CEGUI which layout to display. You can call this at any time to change the layout to
	// another loaded layout (i.e. moving from screen to screen or to load your HUD layout). Note that this takes
	// a CEGUI::Window instance -- you can use anything (any widget) that serves as a root window.
	pSystem->setGUISheet(pLayout);

	// this next bit is for the sake of the input handler
	unsigned long hWnd;
	// WINDOW is generic to all platforms now as of Eihort
	window->getCustomAttribute("WINDOW", &hWnd);

	// set up the input handlers
	Simulation *sim = new Simulation();

	// since the input handler deals with pushing input to CEGUI, we need to give it a pointer
	// to the CEGUI System instance to use
	InputHandler *handler = new InputHandler(pSystem, sim, hWnd);

	// put us into our "main menu" state
	sim->requestStateChange(GUI);

	// make an instance of our GUI sheet handler class
	MainMenuDlg* pDlg = new MainMenuDlg(pSystem, pLayout, sim);
	
	//testing shit
	Ogre::SceneNode *systemNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,0));
	

	
	SphereMesh *sphere = NULL;
	sphere->createSphere("sphereMesh", 80, 64, 64);


	/*
	// Now I can create several entities using that mesh
	Ogre::Entity *MoonEntity = sceneMgr->createEntity("Moon", planetMesh);
	// Now I attach it to a scenenode, so that it becomes present in the scene.
	Ogre::SceneNode *EarthOrbitNode = systemNode->createChildSceneNode("Earth Orbit", Ogre::Vector3(0,0,0));
	Ogre::SceneNode *EarthNode = EarthOrbitNode->createChildSceneNode("Earth", Ogre::Vector3(200,0,0));
	Ogre::SceneNode *MoonNode = EarthNode->createChildSceneNode("Moon", Ogre::Vector3(150,0,0));
	MoonNode->attachObject(MoonEntity);
	MoonEntity->getParentNode()->scale(0.5,0.5,0.5);
	*/
	//Material Tests
	Ogre::MaterialManager& lMaterialManager = Ogre::MaterialManager::getSingleton();
	Ogre::String lNameOfResourceGroup = "Mission 1 : Deliver Tom";
	
	Ogre::ResourceGroupManager& lRgMgr = Ogre::ResourceGroupManager::getSingleton();
	lRgMgr.createResourceGroup(lNameOfResourceGroup);

	{
			Ogre::MaterialPtr lMaterial = lMaterialManager.create("M_Lighting+OneTexture",lNameOfResourceGroup);
			Ogre::Technique* lFirstTechnique = lMaterial->getTechnique(0);
			Ogre::Pass* lFirstPass = lFirstTechnique->getPass(0);
 
			lFirstPass->setDiffuse(0.8f, 0.8f, 0.8f,1.0f);
			lFirstPass->setAmbient(0.3f, 0.3f, 0.3f);
			lFirstPass->setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
			lFirstPass->setShininess(64.0f);
			lFirstPass->setSelfIllumination(0.1f, 0.1f, 0.1f);

			Ogre::TextureUnitState* lTextureUnit = lFirstPass->createTextureUnitState();
			lTextureUnit->setTextureName("SimpleTexture.bmp", Ogre::TEX_TYPE_2D);
			lTextureUnit->setTextureCoordSet(0);
	}
	
	
	// 3/ Lighting color.
	// To have the feeling of '3D', the lighting is good feeling.
		// It often requires the object to have correct normals (see my manual object construction), 
		// and some lights in the scene.
		
	{
			Ogre::MaterialPtr lMaterial = lMaterialManager.create("Sun",lNameOfResourceGroup);
			Ogre::Technique* lFirstTechnique = lMaterial->getTechnique(0);
			Ogre::Pass* lFirstPass = lFirstTechnique->getPass(0);
			
			// Lighting is allowed on this pass.
			lFirstPass->setLightingEnabled(true);

			// Emissive / self illumination is the color 'produced' by the object.
			// Color values vary between 0.0(minimum) to 1.0 (maximum).
			Ogre::ColourValue lSelfIllumnationColour(0.5f, 0.0f, 0.0f, 1.0f);
			lFirstPass->setSelfIllumination(lSelfIllumnationColour);

			// diffuse color is the traditionnal color of the lit object.
			Ogre::ColourValue lDiffuseColour(1.0f, 0.4f, 0.4f, 1.0f);
			lFirstPass->setDiffuse(lDiffuseColour);

			// ambient colour is linked to ambient lighting.
			// If there is no ambient lighting, then this has no influence.
			// It the ambient lighting is at 1, then this colour is fully added.
			// This is often use to change the general feeling of a whole scene.
			Ogre::ColourValue lAmbientColour(0.4f, 0.1f, 0.1f, 1.0f);
			lFirstPass->setAmbient(lAmbientColour);

			// specular colour, is the colour of the 'little light reflection'
			// that you can see on some object. For example, my bald head skin
			// reflect the sun. This make a 'round of specular lighting'.
			// Set this to black if you don't want to see it.
			Ogre::ColourValue lSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
			lFirstPass->setSpecular(lSpecularColour);

			// Shininess is the 'inverse of specular color splattering' coefficient.
			// If this is big (e.g : 64) you get a very tiny dot with a quite strong color (on round surface).
			// If this is 0, you get a simple color layer (the dot has become very wide).
			Ogre::Real lShininess = 64.0f;
			lFirstPass->setShininess(lShininess);
	}
		{
			Ogre::MaterialPtr lMaterial = lMaterialManager.create("mars",lNameOfResourceGroup);
			Ogre::Technique* lFirstTechnique = lMaterial->getTechnique(0);
			Ogre::Pass* lFirstPass = lFirstTechnique->getPass(0);
			
			// Lighting is allowed on this pass.
			lFirstPass->setLightingEnabled(true);

			// Emissive / self illumination is the color 'produced' by the object.
			// Color values vary between 0.0(minimum) to 1.0 (maximum).
			Ogre::ColourValue lSelfIllumnationColour(0.6f, 0.1f, 0.1f, 0.0f);
			lFirstPass->setSelfIllumination(lSelfIllumnationColour);

			// diffuse color is the traditionnal color of the lit object.
			Ogre::ColourValue lDiffuseColour(1.0f, 0.2f, 0.0f, 0.0f);
			lFirstPass->setDiffuse(lDiffuseColour);

			// ambient colour is linked to ambient lighting.
			// If there is no ambient lighting, then this has no influence.
			// It the ambient lighting is at 1, then this colour is fully added.
			// This is often use to change the general feeling of a whole scene.
			Ogre::ColourValue lAmbientColour(1.0f, 0.2f, 0.0f, 0.0f);
			lFirstPass->setAmbient(lAmbientColour);

			// specular colour, is the colour of the 'little light reflection'
			// that you can see on some object. For example, my bald head skin
			// reflect the sun. This make a 'round of specular lighting'.
			// Set this to black if you don't want to see it.
			Ogre::ColourValue lSpecularColour(1.0f, 0.3f, 0.3f, 0.3f);
			lFirstPass->setSpecular(lSpecularColour);

			// Shininess is the 'inverse of specular color splattering' coefficient.
			// If this is big (e.g : 64) you get a very tiny dot with a quite strong color (on round surface).
			// If this is 0, you get a simple color layer (the dot has become very wide).
			Ogre::Real lShininess = 34.0f;
			lFirstPass->setShininess(lShininess);
		}

	{
			Ogre::MaterialPtr lMaterial = lMaterialManager.create("gaia",lNameOfResourceGroup);
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
		}
		{
			Ogre::MaterialPtr lMaterial = lMaterialManager.create("barren",lNameOfResourceGroup);
			Ogre::Technique* lFirstTechnique = lMaterial->getTechnique(0);
			Ogre::Pass* lFirstPass = lFirstTechnique->getPass(0);
			
			// Lighting is allowed on this pass.
			lFirstPass->setLightingEnabled(true);

			// Emissive / self illumination is the color 'produced' by the object.
			// Color values vary between 0.0(minimum) to 1.0 (maximum).
			Ogre::ColourValue lSelfIllumnationColour(0.1f, 0.1f, 0.1f, 0.1f);
			lFirstPass->setSelfIllumination(lSelfIllumnationColour);

			// diffuse color is the traditionnal color of the lit object.
			Ogre::ColourValue lDiffuseColour(0.3f, 0.3f, 0.3f, 0.3f);
			lFirstPass->setDiffuse(lDiffuseColour);

			// ambient colour is linked to ambient lighting.
			// If there is no ambient lighting, then this has no influence.
			// It the ambient lighting is at 1, then this colour is fully added.
			// This is often use to change the general feeling of a whole scene.
			Ogre::ColourValue lAmbientColour(0.3f, 0.3f, 0.3f, 0.3f);
			lFirstPass->setAmbient(lAmbientColour);

			// specular colour, is the colour of the 'little light reflection'
			// that you can see on some object. For example, my bald head skin
			// reflect the sun. This make a 'round of specular lighting'.
			// Set this to black if you don't want to see it.
			Ogre::ColourValue lSpecularColour(0.7f, 0.7f, 0.7f, 0.7f);
			lFirstPass->setSpecular(lSpecularColour);

			// Shininess is the 'inverse of specular color splattering' coefficient.
			// If this is big (e.g : 64) you get a very tiny dot with a quite strong color (on round surface).
			// If this is 0, you get a simple color layer (the dot has become very wide).
			Ogre::Real lShininess = 34.0f;
			lFirstPass->setShininess(lShininess);
		}
	{
			Ogre::MaterialPtr lMaterial = lMaterialManager.create("gasgiant",lNameOfResourceGroup);
			Ogre::Technique* lFirstTechnique = lMaterial->getTechnique(0);
			Ogre::Pass* lFirstPass = lFirstTechnique->getPass(0);
			
			// Lighting is allowed on this pass.
			lFirstPass->setLightingEnabled(true);

			// Emissive / self illumination is the color 'produced' by the object.
			// Color values vary between 0.0(minimum) to 1.0 (maximum).
			Ogre::ColourValue lSelfIllumnationColour(0.0f, 0.3f, 0.0f, 0.1f);
			lFirstPass->setSelfIllumination(lSelfIllumnationColour);

			// diffuse color is the traditionnal color of the lit object.
			Ogre::ColourValue lDiffuseColour(0.1f, 0.7f, 0.1f, 0.3f);
			lFirstPass->setDiffuse(lDiffuseColour);

			// ambient colour is linked to ambient lighting.
			// If there is no ambient lighting, then this has no influence.
			// It the ambient lighting is at 1, then this colour is fully added.
			// This is often use to change the general feeling of a whole scene.
			Ogre::ColourValue lAmbientColour(0.1f, 0.7f, 0.3f, 0.3f);
			lFirstPass->setAmbient(lAmbientColour);

			// specular colour, is the colour of the 'little light reflection'
			// that you can see on some object. For example, my bald head skin
			// reflect the sun. This make a 'round of specular lighting'.
			// Set this to black if you don't want to see it.
			Ogre::ColourValue lSpecularColour(0.2f, 1.0f, 0.6f, 0.6f);
			lFirstPass->setSpecular(lSpecularColour);

			// Shininess is the 'inverse of specular color splattering' coefficient.
			// If this is big (e.g : 64) you get a very tiny dot with a quite strong color (on round surface).
			// If this is 0, you get a simple color layer (the dot has become very wide).
			Ogre::Real lShininess = 34.0f;
			lFirstPass->setShininess(lShininess);
		}

	//end material tests

	//System Creator
	std::string systemDefinition[10] = {"15000,0,0,false,false,false,700000,0", "300,0,0,false,false,barren,2500,1",
		"300,x,y,false,co2,barren,6000,2", "300,x,y,h2o,n2/o2,gaia,6300,3", "300,x,y,false,co2,mars,3400,4",
		"300,x,y,false,h2/he,gasgiant,71000,5", "300,x,y,false,h2/he,gasgiant,60000,6", "300,x,y,false,h2/he,gasgiant,25001,7",
		"300,x,y,false,h2/he,gasgiant,25000,8", "300,x,y,false,n-ice,ice,1100,9"};

	StarSystem sol = StarSystem::StarSystem(sceneMgr, systemDefinition);

	//Star Creator
	/*
	{
	int x = 0;
	int y = 0;
	int kelvin = 15000;
	CelestialBody newstar = CelestialBody::CelestialBody(kelvin, 100, systemNode, sceneMgr, x, y);
	}

	// Spectrum testing and multi star generation.
	
	int time = 200;
	int x = -500;
	int y = 500;

	for ( int count = 1; count <= time; count++){
	
		int kelvin = count * (40000/time);
		Star newstar = Star::Star(kelvin, 100, systemNode, sceneMgr, x, y);
		x += 20;

		if (count % 10 == 0) {
			y -= 20;
			x = -500;
		}
	}
	*/

	// I move the SceneNode back 15 so that it is visible to the camera.
	float PositionOffset = 0.0;
	float distance = -2000.0;

	systemNode->translate(0, PositionOffset, distance);
	
	camera->lookAt(Ogre::Vector3(0,0,distance));

	while (sim->getCurrentState() != SHUTDOWN) {
		
		sol.rotateOrbits();
		
		handler->capture();

		ogre->renderOneFrame();

		// run the message pump (uncomment for Eihort)
		Ogre::WindowEventUtilities::messagePump();
	}

	{
		window->removeAllViewports();
	}
	{
		sceneMgr->destroyAllCameras();
		sceneMgr->destroyAllManualObjects();
		sceneMgr->destroyAllEntities();
		sceneMgr->destroyAllLights();
		systemNode->removeAndDestroyAllChildren();
	}

	{
		Ogre::ResourceGroupManager& lRgMgr = Ogre::ResourceGroupManager::getSingleton();
		lRgMgr.destroyResourceGroup(lNameOfResourceGroup);
	}

	// clean up after ourselves
	delete pDlg;
	delete handler;
	delete sim;
	delete ogre;

	return 0;
}