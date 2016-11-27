#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "j1Module.h"
#include "SDL\include\SDL.h"
#include "p2Point.h"

#include "j1App.h"
#include "p2Point.h"

class Prefab;
struct SDL_Rect;
struct SDL_Texture;

enum Shape {
	Human, Cat
};

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

	// Platforms ---
	bool IsGoingUp();
	bool IsGoingDown();
	int DistanceToPlayer(PhysBody* obj);
	bool isTouching(PhysBody* body1, PhysBody* body2);
	// -------------

	// Shapeshift
	void ChangeShape(Shape newshape);

	//Spell E ghost
	void Ghost();
	void UnGhost();


private:

public:
	Prefab* player = nullptr;

	int curr_hp = 3;

	// Player UI --
	SDL_Texture* UI_texture;
	p2List<SDL_Rect> UI_spells_rects;
	// ------------

	//Spell E Ghost
	j1Timer ghost_timer;
	bool ghost;
	//----------


private:
	bool on_ground = false;

	SDL_Rect Sprite_rect;
	p2SString texture_path;

	int jump_force;
	iPoint start_pos;

	// Platforms --
	int last_pos = 0;
	PhysBody* curr_platform = nullptr;
	// ------------

	int current_animation = 0;
	iPoint draw_offset;

	Prefab* cat_anims;

	Shape shape = Human;
	j1Timer shape_time;

};

#endif // __PLAYER_H__