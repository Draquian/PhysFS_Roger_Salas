#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "AssetsManager.h"

ModuleScene::ModuleScene() : Module()
{
	name.create("scene");
}

ModuleScene::~ModuleScene()
{}

// Called before the first frame
bool ModuleScene::Start()
{
	
	// TODO 5: Uncomment all of this, resolve how to load the document from the memory with the link

	char* buffer;
	pugi::xml_document dataFile;

	int bytesFile = app->assetManager->LoadData("data.xml", &buffer);

	// Loading from memory with PUGI: https://pugixml.org/docs/manual.html#loading.memory
	pugi::xml_parse_result result = dataFile.load_buffer(buffer, bytesFile);

	RELEASE_ARRAY(buffer);

	LoadTexFile(dataFile);
	LoadFxFile(dataFile);
	LoadMusFile(dataFile);

	return true;
}


// Called each loop iteration
bool ModuleScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		app->audio->PlayFx(1);

	if (app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		img = !img;

	return true;
}

// Called each loop iteration
bool ModuleScene::PostUpdate()
{
	bool ret = true;

	if (img)
		app->render->Blit(texture1, 529, 0, 0.5f);
	else if (!img)
		app->render->Blit(texture2, 529, 0, 0.5f);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool ModuleScene::CleanUp()
{
	return true;
}

void ModuleScene::LoadTexFile(const pugi::xml_document& dataFile)
{
	pugi::xml_node tex_node = dataFile.child("data").child("texture");
	texture1 = app->tex->Load(tex_node.attribute("file").as_string());
	texture2 = app->tex->Load(tex_node.attribute("file2").as_string());
}

void ModuleScene::LoadFxFile(const pugi::xml_document& dataFile)
{
	pugi::xml_node fx_node = dataFile.child("data").child("fx");
	app->audio->LoadFx(fx_node.attribute("file").as_string());
}

void ModuleScene::LoadMusFile(const pugi::xml_document& dataFile)
{
	pugi::xml_node mus_node = dataFile.child("data").child("mus");
	app->audio->PlayMusic(mus_node.attribute("file").as_string());
}