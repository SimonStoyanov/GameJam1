#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "j1Module.h"

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

	void CreateColliders();

private:

public:
private:
};

#endif // __PLAYER_H__