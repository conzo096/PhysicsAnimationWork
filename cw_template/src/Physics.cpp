#include "physics.h"
const double coef = 0.5;	// Value between 0- 1, higher value = more bounciness on collision.
const double rigidcoef = 0.0; // Use for springs.


void ResolveRB(RigidBody*const b, const CollisionInfo &ci)
{
	dvec3 dv = b->position - b->prev_pos;
	if (ci.c2 != NULL)
	{
		dvec3 dvc2 = ci.c2->position - ci.c2->prev_pos;
		// If they are moving in opposite directions already ignore.
		if (glm::dot(dv, dvc2) > 0)
			return;
	}
	dvec3 r0 = b->position - ci.position;

	dvec3 v0 = dv + cross(b->angVelocity, r0);
	// I've butchered this formula pretty bad.
	double j = -1.0 * (rigidcoef)+
		dot(dv, ci.normal) /
		(dot(ci.normal, ci.normal) * (b->inverseMass * 2.0) + dot(ci.normal, (cross(r0, ci.normal))));

	// stop sinking
	j = j - (ci.depth*0.5);
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
	// Stores all the collisions that have happened.
	std::vector<CollisionInfo> collisions;
	// Stores any new fragments that are created. Add to the physics scene after current objects have been calculated to avoid recursive loop.
	std::vector<Model> newFragments;
	// Check if objects collide with each other.
	for (int i = 0; i < physicsScene.size(); i++)
	{
		for (int j = i + 1; j < physicsScene.size(); j++)
		{
			if (collision::IsColliding(physicsScene, collisions, physicsScene[i], physicsScene[j]) == true)
			{

				// Check if the threshold is met for the model to break.
				glm::vec3 speedA = (physicsScene[i].GetRigidBody().prev_pos - physicsScene[i].GetRigidBody().position) / dt;
				speedA *= physicsScene[i].GetRigidBody().mass;
				glm::vec3 speedB = (physicsScene[j].GetRigidBody().prev_pos - physicsScene[j].GetRigidBody().position) / dt;
				speedB *= physicsScene[j].GetRigidBody().mass;

				// Find combined speed between two objects.
				if (length(speedA + speedB) > 200)
				{ 
					std::shuffle(physicsScene[i].GetSplittingPlanes().begin(), physicsScene[i].GetSplittingPlanes().end(),
						std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
					if (physicsScene.size() < 60)
					{
						// Decide if 0,1 or both objects break.
						int decision = std::rand() % 4;
						switch (decision)
						{
							case 1: break;
							case 2:
								for (int k = 0; k < physicsScene[i].GetSplittingPlanes().size(); k++)
									SliceModel(physicsScene[i], physicsScene[i].GetSplittingPlanes()[k], newFragments);
								break;
							case 3:
								for (int k = 0; k < physicsScene[j].GetSplittingPlanes().size(); k++)
									SliceModel(physicsScene[j], physicsScene[j].GetSplittingPlanes()[k], newFragments);
								break;
							case 4:
								for (int k = 0; k < physicsScene[i].GetSplittingPlanes().size(); k++)
									SliceModel(physicsScene[i], physicsScene[i].GetSplittingPlanes()[k], newFragments);
								for (int k = 0; k < physicsScene[j].GetSplittingPlanes().size(); k++)
									SliceModel(physicsScene[j], physicsScene[j].GetSplittingPlanes()[k], newFragments);
								break;
							default: break;

						}

					}
				}
			}
		}
	}


	// Check if objects collide with floor.
	for (auto &m : physicsScene)
	{
		if (collision::OnFloor(collisions, m, floor) == true)
		{
			glm::vec3 speedA = (m.GetRigidBody().prev_pos - m.GetRigidBody().position) / dt;
			speedA *= m.GetRigidBody().mass;
			// Floor is a constant value as it has infinite mass.
			if (length(speedA) > 200)
				for (int i = 0; i < m.GetSplittingPlanes().size(); i++)
					if (physicsScene.size() < 60)
						SliceModel(m, m.GetSplittingPlanes()[i], newFragments);
					else
						break;
		}
	}

	// Resolve any collisions that have occured.
	for (auto &c : collisions)
	{
		Resolve(c, floor);
	}

	for (auto &e : physicsScene)
		e.GetRigidBody().Integrate(dt);
	for (auto &f : newFragments)
		physicsScene.push_back(f);


	// Update the collider positions each tick. This is more performance costly but makes collisions more accurate.
	for (auto &e : physicsScene)
		e.Update(0);
}

void InitPhysics() {}

void ShutdownPhysics() {}