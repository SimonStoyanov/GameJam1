#ifndef __ENEMIES_H__
#define __ENEMIES_H__

#include "j1Module.h"

class Enemies : public j1Module
{
public:
	Enemies();

	// Destructor
	virtual ~Enemies();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

private:

public:
private:
};

#endif // __ENEMIES_H__