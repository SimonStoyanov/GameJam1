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
#include "IntroScene.h"

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
	dummy_scene = new Dummy();
	intro_scene = new IntroScene();
	current_scene = intro_scene;

	if (current_scene != nullptr)
	{
		current_scene->Start();
		current_scene->SetActive(true);
	}

	// fps
	App->text->fps = new Text(5, 5, App->text->timeless_15, 1);
	App->text->fps->is_ui = true;

	// Mouse position
	App->text->position = new Text(680, 5, App->text->timeless_15, 1);
	App->text->position->is_ui = true;

	// cd
	App->text->cdQ = new Text(350, 546, App->text->timeless_15, 1);
	App->text->cdQ->is_ui = true;

	App->text->cdW = new Text(390, 546, App->text->timeless_15, 1);
	App->text->cdW->is_ui = true;

	App->text->cdE = new Text(435, 546, App->text->timeless_15, 1);
	App->text->cdE->is_ui = true;

	App->text->cdR = new Text(475, 546, App->text->timeless_15, 1);
	App->text->cdR->is_ui = true;

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
	bool ret = true;
	/*
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");
	*/

	if (current_scene != nullptr)
	{
		ret = current_scene->Update(dt);
		current_scene->Draw();
	}

	if (current_scene == dummy_scene)
	{
		App->render->camera.x -= camera_speed*dt;
	}
	
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

	// Temporal UI
	if (current_scene == dummy_scene)
	{
		SDL_Rect rect; rect.x = 340; rect.y = 545; rect.w = 170; rect.h = 50;
		App->render->DrawQuad(rect, 0, 0, 0, 150, true, false);
		rect.x = 0, rect.y = 0; rect.w = 900; rect.h = 25;
		App->render->DrawQuad(rect, 0, 0, 0, 150, true, false);

		App->text->fps->PrintText();
		App->text->position->PrintText();
		App->text->cdQ->PrintText();
		App->text->cdW->PrintText();
		App->text->cdE->PrintText();
		App->text->cdR->PrintText();
	}


	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::ChangeScene(Scene * new_scene)
{
	current_scene->CleanUp();
	current_scene->SetActive(false);
	current_scene = new_scene;
	current_scene->Start();
	current_scene->SetActive(true);
}
