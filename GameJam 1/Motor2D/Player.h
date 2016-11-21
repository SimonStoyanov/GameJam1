#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "j1Module.h"

class Prefab;
class Player : public j1Module
{
public:
	Player();

	// Destructor
	virtual ~Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	void LoadTextures();
	void CreateColliders();

private:

public:
	Prefab* player = nullptr;

private:
};

#endif // __PLAYER_H__