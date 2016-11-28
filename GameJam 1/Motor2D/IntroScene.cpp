#include "Button.h"
#include "IntroScene.h"
#include "j1Scene.h"
#include "Dummy_Scene.h"
#include "Player.h"
#include "j1App.h"
#include "Prefabs.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Text.h"

IntroScene::IntroScene()
{
}

IntroScene::~IntroScene()
{
}

bool IntroScene::Start()
{
	pugi::xml_document Introconfig_doc;
	pugi::xml_node config_node;
	char* buf;
	int size = App->fs->Load("IntroScene.xml", &buf);
	Introconfig_doc.load_buffer(buf, size);
	RELEASE(buf);
	config_node = Introconfig_doc.child("config");

	pugi::xml_node buttons_node = config_node.child("buttons");

	SDL_Rect position;
	position.x = buttons_node.child("play").child("position").attribute("x").as_int(0);
	position.y = buttons_node.child("play").child("position").attribute("y").as_int(0);
	position.w = buttons_node.child("play").child("position").attribute("w").as_int(0);
	position.h = buttons_node.child("play").child("position").attribute("h").as_int(0);

	p2SString ui_path(buttons_node.child("image").attribute("path").as_string(nullptr));
	UI_tex = App->tex->Load(ui_path.GetString());

	p2SString background_path(config_node.child("background").child("image").attribute("path").as_string(nullptr));
	Background_tex = App->tex->Load(background_path.GetString());

	play = new Prefab(position.x, position.y, UI_tex, NULLRECT);
	play->LoadAnimations(buttons_node.child("play"));
	play->current_anim = play->FindAnimation(Idle);

	play_button = new Button(position.x, position.y, position.w, position.h);

	App->text->highscore = new Text(610, 525, App->text->on_meth_30, 1);
	p2SString highscore("High Score: %d", App->scene->dummy_scene->high_score);
	App->text->highscore->SetText(highscore);

	fx = App->audio->LoadFx("audio/music/StartGame.wav");

	return true;
}

bool IntroScene::Update(float dt)
{
	App->audio->PlayFx(fx);

	if (play_button->MouseDown() && !play_clicked) {
		play->current_anim = play->FindAnimation(Run);
		play_clicked = true;
	}

	if (play_clicked) {
		if (play->animations[play->current_anim]->Finished()) {
			play->animations[play->current_anim]->Reset();
			play_clicked = false;
			App->player->Start();
			App->player->active = true;
			App->scene->ChangeScene(App->scene->dummy_scene);
		}
	}
	return true;
}

bool IntroScene::PostUpdate()
{
	return true;
}

void IntroScene::Draw()
{
	if(Background_tex != nullptr)			//ALWAYS FIRST
		App->render->Blit(Background_tex, 0, 0);

	App->render->Blit(play->sprite.texture, play->sprite.pos.x, play->sprite.pos.y, &play->animations[play->current_anim]->GetCurrentFrameRect());
}

bool IntroScene::CleanUp()
{
	App->tex->UnLoad(UI_tex);
	App->tex->UnLoad(Background_tex);
	return true;
}
