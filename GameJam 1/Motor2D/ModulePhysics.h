#pragma once
#include "j1Module.h"
#include "p2Point.h"
//#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -20.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum BodyType
{
	null,
	platform,
	wall,
	fireb,
	player,
	death_detec
};

struct path_joint {
	b2MouseJoint* joint;
	b2Vec2* path;
	int points = 0;
	int cur_point = 0;
};

enum Layers {
	PLAYER = 1, WORLD = 2, BOSS = 4  //CHANGE FOR LAYERS NEEDED AND REPLACE THE METHODS!!!!
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	j1Module* listener;
	BodyType type = BodyType::null;
};

// Module --------------------------------------
class ModulePhysics : public j1Module, public b2ContactListener // TODO
{
public:
	ModulePhysics();
	~ModulePhysics();

	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, float rest = 0.0f, int cat = WORLD, int mask = PLAYER);
	PhysBody* CreateStaticCircle(int x, int y, int radius, float rest = 0.0f, int cat = WORLD, int mask = PLAYER);
	PhysBody* CreateCircleSensor(int x, int y, int radius, float rest = 0.0f, int cat = WORLD, int mask = PLAYER);
	PhysBody* CreateRectangle(int x, int y, int width, int height, float rest = 0.0f, int cat = WORLD, int mask = PLAYER, int angle = 0);
	PhysBody* CreateStaticRectangle(int x, int y, int width, int height, float rest = 0.0f, int cat = WORLD, int mask = PLAYER, int angle = 0);
	PhysBody* CreatePolygon(int x, int y, int* points, int size, float rest = 0.0f, int cat = WORLD, int mask = PLAYER, int angle = 0);
	PhysBody* CreateStaticPolygon(int x, int y, int* points, int size, float rest = 0.0f, int cat = WORLD, int mask = PLAYER, int angle = 0);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, float rest = 0.0f, int cat = WORLD, int mask = PLAYER, int angle = 0);
	PhysBody* CreateChain(int x, int y, int* points, int size, float rest = 0.0f, int cat = WORLD, int mask = PLAYER, int angle = 0);
	PhysBody* CreateStaticChain(int x, int y, int* points, int size, float rest = 0.0f, int cat = WORLD, int mask = PLAYER, int angle = 0);

	void CleanBodies();

	void DeleteObject(PhysBody* object);

	b2RevoluteJoint* CreateRevoluteJoint(PhysBody* anchor, PhysBody* body, iPoint anchor_offset, iPoint body_offset, bool enable_limit, float max_angle, float min_angle, bool enable_motor, int motor_speed, int max_torque);
	b2PrismaticJoint* CreatePrismaticJoint(PhysBody* anchor,PhysBody* body, iPoint anchor_offset, iPoint body_offset, bool enable_limit, float max_trans, float min_trans, bool enable_motor, int motor_speed, int max_force);
	b2MotorJoint* CreateMotorJoint(b2Body* body, b2Vec2 target);

	path_joint* CreatePathJoint(b2Body* body, int *path, int path_size, int x_offset = 0, int y_offset = 0);

	void DeleteJoint(b2MouseJoint* joint);

	void SetGround(b2Body* body) {
		ground = body;
	}

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint = nullptr;
	b2Body* ground;
	b2Body* selected = nullptr;

};