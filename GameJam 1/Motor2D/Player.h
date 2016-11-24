#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "j1Module.h"
#include "SDL\include\SDL.h"
#include "p2Point.h"

class Prefab;
class Player : public j1Module
{
public:
	Player();

	// Destructor
	virtual ~Player();

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	bool Start();

	//
	bool Update(float dt);
	// Called before quitting
	bool CleanUp();

	void LoadTextures();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	bool IsGoingUp();
	PhysBody* GetClosestPlat();

private:
	void LoadAnimations(pugi::xml_node config);

public:
	Prefab* player = nullptr;

private:
	bool on_ground = false;

	SDL_Rect Sprite_rect;
	p2SString texture_path;

	int jump_force;
	iPoint start_pos;

	int last_pos = 0;
	PhysBody* curr_platform = nullptr;

	int current_animation = 0;
};

#endif // __PLAYER_H__