#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Text.h"
#include "j1Scene.h"
#include "Scene.h"
#include "Dummy_Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	bool ret = true;

	camera_speed = node.child("camera_speed").attribute("value").as_int(2);

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if(App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	dummy_scene = new Dummy();
	current_scene = dummy_scene;

	if (current_scene != nullptr)
	{
		current_scene->Start();
	}

	// fps
	App->text->fps = new Text(5, 5, App->text->timeless_15, 1);
	App->text->fps->is_ui = true;

	// Mouse position
	App->text->position = new Text(690, 5, App->text->timeless_15, 1);
	App->text->position->is_ui = true;

	// cdQ
	App->text->cdQ = new Text(350,546, App->text->timeless_15, 1);
	App->text->cdQ->is_ui = true;
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	/*
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");
	*/

	if (current_scene != nullptr)
	{
		current_scene->Update(dt);
		current_scene->Draw();
	}

	if (current_scene == dummy_scene)
	{
		App->render->camera.x -= camera_speed*dt;
	}

	
	SDL_Rect rect; rect.x = 340; rect.y = 545; rect.w = 100; rect.h = 50;
	App->render->DrawQuad(rect, 0, 0, 0, 150, true, false);
	App->text->fps->PrintText();
	App->text->position->PrintText();
	App->text->cdQ->PrintText();
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (current_scene != nullptr)
	{
		current_scene->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
