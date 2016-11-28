#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics()
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	// creating staic shape for ground

	return true;
}

// 
bool ModulePhysics::PreUpdate()
{
	world->Step(App->Getdt(), 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureB()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return true;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, float rest, int cat, int mask)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.filter.categoryBits = cat;
	fixture.filter.maskBits = mask;
	fixture.restitution = rest;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody * ModulePhysics::CreateStaticCircle(int x, int y, int radius, float rest, int cat, int mask)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.filter.categoryBits = cat;
	fixture.filter.maskBits = mask;
	fixture.restitution = rest;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody * ModulePhysics::CreateCircleSensor(int x, int y, int radius, float rest, int cat, int mask)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.isSensor = true;
	fixture.filter.categoryBits = cat;
	fixture.filter.maskBits = mask;
	fixture.restitution = rest;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, float rest, int cat, int mask, int angle)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = DEGTORAD*angle;

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.filter.categoryBits = cat;
	fixture.filter.maskBits = mask;
	fixture.restitution = rest;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody * ModulePhysics::CreateStaticRectangle(int x, int y, int width, int height, float rest, int cat, int mask, int angle)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = DEGTORAD*angle;

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.filter.categoryBits = cat;
	fixture.filter.maskBits = mask;
	fixture.restitution = rest;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody * ModulePhysics::CreatePolygon(int x, int y, int* points, int size, float rest, int cat, int mask, int angle)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = DEGTORAD*angle;

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	box.Set(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.filter.categoryBits = cat;
	fixture.filter.maskBits = mask;
	fixture.restitution = rest;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->height = pbody->width = 0;

	return pbody;
}

PhysBody * ModulePhysics::CreateStaticPolygon(int x, int y, int* points, int size, float rest, int cat, int mask, int angle)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = DEGTORAD*angle;

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	box.Set(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.filter.categoryBits = cat;
	fixture.filter.maskBits = mask;
	fixture.restitution = rest;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->height = pbody->width = 0;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, float rest, int cat, int mask, int angle)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = DEGTORAD*angle;

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;
	fixture.filter.categoryBits = cat;
	fixture.filter.maskBits = mask;
	fixture.restitution = rest;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, float rest, int cat, int mask, int angle)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = DEGTORAD*angle;

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.filter.categoryBits = cat;
	fixture.filter.maskBits = mask;
	fixture.restitution = rest;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody * ModulePhysics::CreateStaticChain(int x, int y, int * points, int size, float rest, int cat, int mask, int angle)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = DEGTORAD*angle;

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.filter.categoryBits = cat;
	fixture.filter.maskBits = mask;
	fixture.restitution = rest;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

void ModulePhysics::CleanBodies()
{
	while (world->GetBodyList() != nullptr) {
		world->DestroyBody(world->GetBodyList());
	}
}

void ModulePhysics::DeleteObject(PhysBody * object)
{
	world->DestroyBody(object->body);
}

b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(PhysBody * anchor, PhysBody * body, iPoint anchor_offset, iPoint body_offset, bool enable_limit,
													float max_angle, float min_angle, bool enable_motor, int motor_speed, int max_torque)
{
	b2RevoluteJointDef rev_joint;
	rev_joint.bodyA = anchor->body;
	rev_joint.bodyB = body->body;
	rev_joint.collideConnected = false;
	rev_joint.type = e_revoluteJoint;
	rev_joint.enableLimit = enable_limit;
	rev_joint.enableMotor = enable_motor;
	b2Vec2 anchor_center_diff(PIXEL_TO_METERS(anchor_offset.x), PIXEL_TO_METERS(anchor_offset.y));
	rev_joint.localAnchorA = anchor_center_diff;
	b2Vec2 body_center_diff(PIXEL_TO_METERS(body_offset.x), PIXEL_TO_METERS(body_offset.y));
	rev_joint.localAnchorB = body_center_diff;
	if (enable_limit) {
		rev_joint.lowerAngle = DEGTORAD*min_angle;
		rev_joint.upperAngle = DEGTORAD*max_angle;
	}
	if (enable_motor) {
		rev_joint.motorSpeed = motor_speed;
		rev_joint.maxMotorTorque = max_torque;
	}

	b2RevoluteJoint* rev = (b2RevoluteJoint*)world->CreateJoint(&rev_joint);

	return rev;
}

b2PrismaticJoint * ModulePhysics::CreatePrismaticJoint(PhysBody * anchor, PhysBody * body, iPoint anchor_offset, iPoint body_offset, bool enable_limit, float max_trans, float min_trans, bool enable_motor, int motor_speed, int max_force)
{
	b2PrismaticJointDef def;
	def.bodyA = anchor->body;
	def.bodyB = body->body;
	def.collideConnected = false;
	def.enableLimit = enable_limit;
	def.enableMotor = enable_motor;
	b2Vec2 localA(PIXEL_TO_METERS(anchor_offset.x), PIXEL_TO_METERS(anchor_offset.y));
	def.localAnchorA = localA;
	b2Vec2 localB(PIXEL_TO_METERS(body_offset.x), PIXEL_TO_METERS(body_offset.y));
	def.localAnchorB = localB;
	b2Vec2 localAxis(0,1);
	def.localAxisA = localAxis;
	if (enable_limit) {
		def.lowerTranslation = PIXEL_TO_METERS(min_trans);
		def.upperTranslation = PIXEL_TO_METERS(max_trans);
	}
	if (enable_motor) {
		def.motorSpeed = motor_speed;
		def.maxMotorForce = max_force;
	}

	return (b2PrismaticJoint*)world->CreateJoint(&def);
}

b2MotorJoint* ModulePhysics::CreateMotorJoint(b2Body* body, b2Vec2 target)
{
	target.x = PIXEL_TO_METERS(target.x);
	target.y = PIXEL_TO_METERS(target.y);

	b2MotorJointDef def;
	def.bodyA = ground;
	def.bodyB = body;
	def.maxTorque = 100.0f * body->GetMass();
	def.maxForce = 100.0f * body->GetMass();

	

	return (b2MotorJoint*)world->CreateJoint(&def);
}

path_joint * ModulePhysics::CreatePathJoint(b2Body * body, int * path, int path_size, int x_offset, int y_offset)
{
	path_joint* aux = new path_joint;

	b2Vec2* p = new b2Vec2[path_size / 2];

	for (uint i = 0; i < path_size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS((path[i * 2 + 0] + x_offset));
		p[i].y = PIXEL_TO_METERS((path[i * 2 + 1] + y_offset));
	}

	aux->path = p;
	aux->points = path_size / 2;

	b2MouseJointDef def;
	def.bodyA = ground;
	def.bodyB = body;
	def.target = p[0];
	def.dampingRatio = 1.0f;
	def.frequencyHz = 2.0f;
	def.maxForce = 100.0f * body->GetMass();

	b2MouseJoint* test = (b2MouseJoint*)world->CreateJoint(&def);
	aux->joint = test;

	return aux;
}

void ModulePhysics::DeleteJoint(b2MouseJoint* joint)
{
	world->DestroyJoint(joint);
}

// 
bool ModulePhysics::PostUpdate()
{
	/*
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return true;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->render->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 0, 0, 255);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 0, 0, 255);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			If mouse button 1 is pressed ...
			if (App->input->GetKey(SDL_SCANCODE_F12) == KEY_DOWN) {
				int mouse_x, mouse_y;
				App->input->GetMousePosition(mouse_x, mouse_y);
				b2Vec2 mouse(PIXEL_TO_METERS(mouse_x), PIXEL_TO_METERS(mouse_y));
				if (f->TestPoint(mouse)) {
					selected = f->GetBody();
					break;
				}
			}
			// test if the current body contains mouse position
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	If a body was selected, create a mouse joint
	// using mouse_joint class property
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && selected != nullptr) {
		int mouse_x, mouse_y;
		App->input->GetMousePosition(mouse_x, mouse_y);
		b2Vec2 mouse(PIXEL_TO_METERS(mouse_x), PIXEL_TO_METERS(mouse_y));

		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = selected;
		def.target = mouse;
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f * selected->GetMass();

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);

	}


	If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && selected != nullptr) {
		int mouse_x, mouse_y;
		App->input->GetMousePosition(mouse_x, mouse_y);
		b2Vec2 mouse(PIXEL_TO_METERS(mouse_x), PIXEL_TO_METERS(mouse_y));
		b2Vec2 mouse_pix(mouse_x, mouse_y);

		mouse_joint->SetTarget(mouse);
		App->render->DrawLine(mouse_pix.x, mouse_pix.y, METERS_TO_PIXELS(selected->GetPosition().x), METERS_TO_PIXELS(selected->GetPosition().y), 255, 0, 0, 255);
	}
	If the player releases the mouse button, destroy the joint
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && selected != nullptr) {
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
		selected = nullptr;
	}

	*/
	return true;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}