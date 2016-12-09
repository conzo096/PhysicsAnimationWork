#include "physics.h"


const double coef = 0.5;
const double rigidcoef = 0.0;

void ResolveFloorCollision(RigidBody* const b, const CollisionInfo &ci, PlaneCollider pc)
{
	// If the angle the cube is 90 degree to the plane normal then there is no anglular rotation.
	// This is to fix the rotation problem that occurs when the box tries to lie on a plane.
	glm::dquat rot = b->GetQuat();
	if (glm::dot(glm::axis(rot), pc.GetNormal()) == 90)
		std::cout << "90" << std::endl;

}
void ResolveRB(RigidBody*const b, const CollisionInfo &ci)
{

	dvec3 dv = b->position - b->prev_pos;
	dvec3 r0 = b->position - ci.position;
	dvec3 v0 = dv + cross(b->angVelocity, r0);

	// I've butchered this formula pretty bad.
	double j = -1.0 * (rigidcoef)+
		dot(dv, ci.normal) /
		(dot(ci.normal, ci.normal) * (b->inverseMass * 2.0) + dot(ci.normal, (cross(r0, ci.normal))));

	// stop sinking
	j = j - (ci.depth * 0.05);

	// linear impulse
	dvec3 newVel = dv + (b->inverseMass * ci.normal * j);
	b->AddLinearImpulse(-newVel);
	
	// angular impulse
	auto gg = cross(r0, ci.normal);
	b->angVelocity += b->worldInvInertia * cross(r0, ci.normal * j);
}


void Resolve(const CollisionInfo &ci, PlaneCollider& pc)
{

	auto body1 = ci.c1;
	auto body2 = ci.c2;
	

	// If there is only collision then it is because the other is the floor collider, which does not get changed.
	if (body1 != NULL && body2 == NULL)
	{
	//	ResolveFloorCollision(ci.c1, ci, pc);
	}

	if (body1 != NULL)
	{
		ResolveRB(body1, ci);
	}
	if (body2 != NULL)
	{
		ResolveRB(body2, ci);
	}
}

void UpdatePhysics(vector<phys::Model>& physicsScene, const double t, const double dt, phys::PlaneCollider floor)
{
	std::vector<CollisionInfo> collisions;

	// Check if objects collide with each other.
	for (int i = 0; i < physicsScene.size(); i++)
		for (int j = i + 1; j < physicsScene.size(); j++)
			collision::IsColliding(physicsScene,collisions,physicsScene[i], physicsScene[j]);

	for (auto &c : collisions)
	{
		Resolve(c, floor);
	}
	collisions.clear();
	// Check if any object collide with the plane.
	for (int i = 0; i < physicsScene.size(); i++)
		collision::OnFloor(collisions, physicsScene[i], floor);

	for (auto &c : collisions)
	{
		Resolve(c,floor);		
	}

	for (auto &e : physicsScene)
	{

		e.GetRigidBody().Integrate(dt);
	}
}

void InitPhysics() {}

void ShutdownPhysics() {}
