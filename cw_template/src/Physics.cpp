#include "physics.h"


const double coef = 0.5;
const double rigidcoef = 0.0;

void ResolveRB(RigidBody*const b, const CollisionInfo &ci, bool which)
{

	dvec3 dv = b->position - b->prev_pos;
	dvec3 r0 = b->position - ci.position;
	dvec3 v0 = dv + cross(b->angVelocity, r0);

	// I've butchered this formula pretty bad.
	double j = -1.0 * (rigidcoef)+
		dot(dv, ci.normal) /
		(dot(ci.normal, ci.normal) * (b->inverseMass * 2.0) + dot(ci.normal, (cross(r0, ci.normal))));

	// stop sinking
	j = j - (ci.depth * 0.1);

	// linear impulse
	dvec3 newVel = dv + (b->inverseMass * ci.normal * j);
	b->AddLinearImpulse(-newVel);
	
	// angular impulse
	auto gg = cross(r0, ci.normal);
	b->angVelocity += b->worldInvInertia * cross(r0, ci.normal * j);
}


void Resolve(const CollisionInfo &ci)
{

	auto body1 = ci.c1;
	auto body2 = ci.c2;
	
	if (body1 != NULL)
	{
		ResolveRB(body1, ci, false);
	}
	if (body2 != NULL)
	{
		ResolveRB(body2, ci, true);
	}
}





void UpdatePhysics(vector<phys::Model>& physicsScene, const double t, const double dt)
{
	std::vector<CollisionInfo> collisions;
	// Check for collisions. 
	for (int i = 0; i < physicsScene.size(); i++)
		for (int j = i + 1; j < physicsScene.size(); j++)
			collision::IsColliding(physicsScene,collisions,physicsScene[i], physicsScene[j]);

	for (auto &c : collisions)
	{
		Resolve(c);
		
	}

	for (auto &e : physicsScene)
	{
		//std::cout << to_string(physicsScene[0].GetRigidBody().GetPosition()) << std::endl;
		e.GetRigidBody().Integrate(dt);
	}
}

void InitPhysics() {}

void ShutdownPhysics() {}
