#include "PhysicsSystem.h"

bool PhysicsSystem::Init(float ppm, float gx, float gy) {
	m_PPM = ppm;
	m_World = new b2World({ gx,gy });
	return true;
}

void PhysicsSystem::Shutdown() {
	delete m_World;
	m_World = nullptr;
}

void PhysicsSystem::Step(float dt) {
	if (!m_World) return;
	m_World->Step(dt, 8, 3);
}

b2Body* PhysicsSystem::CreateBox(float px, float py, float w, float h, bool dynamic, float density, float rest) {
	b2BodyDef bd;
	bd.type = dynamic ? b2_dynamicBody : b2_staticBody;
	bd.position.Set(px / m_PPM, py / m_PPM);

	b2PolygonShape shape;
	shape.SetAsBox((w / 2) / m_PPM, (h / 2) / m_PPM);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = density;
	fd.restitution = rest;

	b2Body* body = m_World->CreateBody(&bd);
	body->CreateFixture(&fd);
	return body;

}

b2Body* PhysicsSystem::CreateCircle(float px, float py, float radius, bool dynamic, float density, float rest) {
	b2BodyDef bd;
	bd.type = dynamic ? b2_dynamicBody : b2_staticBody;
	bd.position.Set(px / m_PPM, py / m_PPM);

	b2CircleShape shape;
	shape.m_radius = radius / m_PPM;

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = density;
	fd.restitution = rest;

	b2Body* body = m_World->CreateBody(&bd);
	body->CreateFixture(&fd);
	return body;
}