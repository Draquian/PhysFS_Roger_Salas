#include <iostream> 
#include "Defs.h"
#include "Log.h"
#include "Window.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "AssetsManager.h"

#include "App.h"



// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(pTimer);

	wantToSave = wantToLoad = false;

	//modules
	assetManager = new ModuleAssetsManager();
	win = new ModuleWindow();
	input = new ModuleInput();
	render = new ModuleRender();
	tex = new ModuleTextures();
	audio = new ModuleAudio();
	scene = new ModuleScene();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(assetManager);
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(scene);

	// render last to swap buffer
	AddModule(render);
	

	PERF_PEEK(pTimer);
}

// Destructor
App::~App()
{
	// release modules
	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(pTimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;

	config = LoadConfig(config_file);


	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());

		frameRate = app_config.attribute("framerate_cap").as_int();
		capFrames = true;
	}


	if (ret == true)
	{
		int numModules = modules.size();


		for (int i = 0; i < numModules; i++)
		{
			ret = modules[i]->Awake(config.child(modules[i]->name.GetString()));
		}


	}


	loadGame = config.first_child().child("load").attribute("fileName").as_string();
	saveGame = config.first_child().child("load").attribute("fileName").as_string();


	PERF_PEEK(pTimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	int numModules = modules.size();


	for (int i = 0; i < numModules; i++)
	{
		ret = modules[i]->Start();
	}


	return ret;
}

// Called each loop iteration
bool App::Update()
{

	bool ret = true;
	PrepareUpdate();


	if (input->GetWindowEvent(EVENT_WINDOW::WE_QUIT) == true)
		ret = false;


	if (ret == true)
		ret = PreUpdate();


	if (ret == true)
		ret = DoUpdate();


	if (ret == true)
		ret = PostUpdate();


	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");


	if (result != NULL)
		ret = config_file.child("config");


	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	dt = frameTime.ReadSec() * TIME_CONST;


	//just to when we debug, the player doesnt trespass the floor
	if (dt > MAX_DT)
		dt = MAX_DT;


	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	PERF_START(pTimer);


	if (wantToSave == true)
		SavegameNow();


	if (wantToLoad == true)
		LoadGameNow();


	// Framerate calculations --


	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}


	float avgFps = float(frameCount) / startupTime.ReadSec();
	float secondsSinceStartup = startupTime.ReadSec();
	uint32 lastFrameMs = frameTime.Read();
	uint32 framesOnLastUpdate = prevLastSecFrameCount;

	static char title[256];

	app->win->SetTitle(title);

	int delay;
	int averageFrame = ((1.0f / frameRate) * 1000);
	delay = averageFrame - lastFrameMs;



}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	int numModules = modules.size();


	for (int i = 0; i < numModules && ret == true; i++)
	{


		if (modules[i]->active == false)
			continue;


		ret = modules[i]->PreUpdate();
	}


	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	int numModules = modules.size();


	for (int i = 0; i < numModules && ret == true; i++)
	{


		if (modules[i]->active == false)
		{
			continue;
		}


		ret = modules[i]->Update(dt);
	}


	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	int numModules = modules.size();

	for (int i = 0; i < numModules && ret == true; i++)
	{
		if (modules[i]->active == false) {
			continue;
		}

		ret = modules[i]->PostUpdate();
	}

	frames++;		
	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	int numModules = modules.size();


	for (int i = numModules - 1; i >= 0 && ret == true; i--)
	{
		ret = modules[i]->CleanUp();
	}


	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{


	if (index < argc)
		return args[index];
	else
		return NULL;


}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGame()
{

	wantToLoad = true;
}

// ---------------------------------------
void App::SaveGame() const
{

	wantToSave = true;
}

// ---------------------------------------


bool App::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;


	pugi::xml_parse_result result = data.load_file(loadGame.GetString());

	if (result != NULL)
	{

		root = data.child("game_state");

		int numModules = modules.size();
		ret = true;

		for (int i = 0; i < numModules && ret == true; i++)
		{
			ret = modules[i]->Load(root.child(modules[i]->name.GetString()));
		}

		data.reset();
	}


	wantToLoad = false;
	return ret;
}

bool App::SavegameNow() const
{
	bool ret = true;

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	int numModules = modules.size();

	for (int i = 0; i < numModules && ret == true; i++)
	{
		ret = modules[i]->Save(root.append_child(modules[i]->name.GetString()));
	}

	if (ret == true)
	{
		data.save_file(saveGame.GetString());
	}

	data.reset();
	wantToSave = false;
	return ret;
}