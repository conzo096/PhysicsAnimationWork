#include "Game.h"



bool collision::IsColliding(std::vector<CollisionInfo> &civ, Model &c1,Model &c2)
{
	enum shape { UNKOWN = 0, PLANE, SPHERE, BOX };
	shape s1 = UNKOWN;
	shape s2 = UNKOWN;
	if (dynamic_cast<const SphereCollider *>(&c1.GetSphereCollider()))
	{
		s1 = SPHERE;
	}
	else if (dynamic_cast<const BoundingBox *>(&c1.GetSphereCollider()))
	{
		s1 = BOX;
	}

//	else if (dynamic_cast<const cPlaneCollider *>(&c1)) {
//		s1 = PLANE;
//	}
//

	if (dynamic_cast<const SphereCollider *>(&c2.GetSphereCollider()))
	{
		s2 = SPHERE;
	}
	else if (dynamic_cast<const BoundingBox *>(&c2.GetSphereCollider()))
	{
		s2 = BOX;
	}
//	else if (dynamic_cast<const cPlaneCollider *>(&c2)) {
//		s2 = PLANE;
//	}
//
	if (!s1 || !s2)
	{
		std::cout << "Routing Error" << std::endl;
		return false;
	}
//	if (s1 == PLANE) {
//		if (s2 == PLANE) {
//			return IsCollidingCheck(civ, dynamic_cast<const cPlaneCollider &>(c1), dynamic_cast<const cPlaneCollider &>(c2));
//		}
//		else if (s2 == SPHERE) {
//			return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c1), dynamic_cast<const cPlaneCollider &>(c2));
//		}
//		else if (s2 == BOX) {
//			return IsCollidingCheck(civ, dynamic_cast<const cPlaneCollider &>(c1), dynamic_cast<const cBoxCollider &>(c2));
//		}
//		else {
//			cout << "Routing Error" << endl;
//			return false;
//		}
//	}
	if (s1 == SPHERE)
	{
		if (s2 == PLANE)
		{
		//	return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c1), dynamic_cast<const cPlaneCollider &>(c2));
		}
		else if (s2 == SPHERE)
		{
			const dvec3 p1 = c1.GetSphereCollider().GetCenter();
			const dvec3 p2 = c2.GetSphereCollider().GetCenter();
			const dvec3 d = p2 - p1;
			const double distance = glm::length(d);
			const double sumRadius = c1.GetSphereCollider().GetRadius() + c2.GetSphereCollider().GetRadius();
			if (distance < sumRadius)
			{
				auto depth = sumRadius - distance;
				auto norm = -glm::normalize(d);
				auto pos = p1 - norm * (c1.GetSphereCollider().GetRadius() - depth * 0.5f);
				civ.push_back({ &c1.GetRigidBody(), &c2.GetRigidBody(), pos, norm, depth });
				return true;
			}
			//return IsCollidingCheck(civ,c1.GetSphereCollider(), c2.GetSphereCollider());
			return false;
		}
		else if (s2 == BOX)
		{
		//	return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c1), dynamic_cast<const cBoxCollider &>(c2));
		}
		else
		{
			std::cout << "Routing Error" << std::endl;
			return false;
		}
	}
	else if (s1 == BOX) {
//		if (s2 == PLANE) {
//			return IsCollidingCheck(civ, dynamic_cast<const cPlaneCollider &>(c2), dynamic_cast<const cBoxCollider &>(c1));
//		}
//		else if (s2 == SPHERE) {
//			return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c2), dynamic_cast<const cBoxCollider &>(c1));
//		}
		if (s2 == BOX)
		{
			// CODE FOR BOX TO BOX COLLISIONS.

			return false;
		}
		else
		{
			cout << "Routing Error" << endl;
			return false;
		}
	}
	return false;
}
