#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"

class ModuleScene : public Module
{
public:

	ModuleScene();

	// Destructor
	virtual ~ModuleScene();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:

	//load the texture
	void LoadTexFile(const pugi::xml_document& dataFile);

	//load the fx
	void LoadFxFile(const pugi::xml_document& dataFile);

	//load and play the music
	void LoadMusFile(const pugi::xml_document& dataFile);

	SDL_Texture* texture1 = nullptr;
	SDL_Texture* texture2 = nullptr;

	bool img = 1;
};

#endif // __MODULESCENE_H__
