#ifndef _FEARBOSS_
#define _FEARBOSS_

#include "Boss.h"
#include "j1Render.h"
#include "j1App.h"
#include <math.h>


class FearBoss : public Boss {
public:
	FearBoss(pugi::xml_node& config) : Boss(fear){
		p2SString tex_path(config.child("spritesheet").attribute("path").as_string());
		int x, y;
		x = config.parent().child("position").attribute("x").as_int(0);
		y = config.parent().child("position").attribute("y").as_int(0);
		initial_pos = { x,y };
		SDL_Rect collision = { 0,0,config.child("collisionsize").attribute("w").as_int(0),config.child("collisionsize").attribute("h").as_int(0) };
		prefab = new Prefab(x, y, tex_path.GetString(),collision);
		LoadAnimations(config);
		prefab->CreateStaticCollision(prefab->sprite.rect.w, prefab->sprite.rect.h, BOSS, PLAYER);
		movement = config.child("movement").attribute("value").as_int(0);
		increment = config.child("movement").attribute("speed").as_int(0);
		current_anim = prefab->FindAnimation(Idle);
		if (current_anim == -1) current_anim = 0;
		draw_offset.x = config.child("draw_offset").attribute("x").as_int(0);
		draw_offset.y = config.child("draw_offset").attribute("y").as_int(0);

	}

	bool Update(float dt) {
		if (move < -movement || move > movement)
			increment = -increment;

		move += increment;
		prefab->pbody->body->SetTransform(b2Vec2(prefab->pbody->body->GetPosition().x+ PIXEL_TO_METERS((int)(200*dt)), PIXEL_TO_METERS((int)(initial_pos.y+move))), 0);
		return true;
	}

	void Draw() {
		App->render->Blit(prefab->sprite.texture, prefab->GetPosition().x + draw_offset.x, prefab->GetPosition().y + draw_offset.y, &prefab->animations[current_anim]->GetCurrentFrameRect());
	}

public:
	
private:
	iPoint initial_pos;
	int movement, move = 0, increment;
private:

};

#endif