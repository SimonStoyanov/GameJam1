#ifndef _BOSS_
#define _BOSS_

#include "Prefabs.h"

enum EnemiesTypes {
	unknow,
};

class Boss {
public:
	Boss(){}
	Boss(EnemiesTypes type) : type(type) {	}
public:
	Prefab prefab;
	EnemiesTypes type;

private:
private:

};

#endif // !_BOSS_

