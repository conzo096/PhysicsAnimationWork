#include "Game.h"


bool CheckCorner(float val, float minBound, float maxBound)
{
	return minBound <= val && val <= maxBound;
}


bool CheckObbObb(std::vector<CollisionInfo> &civ, Model c1, Model c2)
{
	BoundingBox a = c1.GetBoundingBox();
	BoundingBox b = c2.GetBoundingBox();

	// Store original corners in an array.
	glm::dvec3 aCorners[8] = { a.GetBackBottomLeft(),a.GetBackBottomRight(),a.GetBackTopLeft(),a.GetBackTopRight(),
		a.GetFrontBottomLeft(),a.GetFrontBottomRight(),a.GetFrontTopLeft(),a.GetFrontTopRight() };
	glm::dvec3 bCorners[8] = { b.GetBackBottomLeft(),b.GetBackBottomRight(),b.GetBackTopLeft(),b.GetBackTopRight(),
		b.GetFrontBottomLeft(),b.GetFrontBottomRight(),b.GetFrontTopLeft(),b.GetFrontTopRight() };

	for (int i = 0; i < 8; i++)
	{

		// Need to apply rotation to this.
		aCorners[i] = glm::dvec3(aCorners[i] + a.GetPosition());
		bCorners[i] = glm::dvec3(bCorners[i] + b.GetPosition());

	}

	float xMin = b.GetPosition().x + b.GetBackBottomLeft().x,
		xMax = b.GetPosition().x + b.GetBackBottomRight().x,
		yMin = b.GetPosition().y + b.GetBackBottomLeft().y,
		yMax = b.GetPosition().y + b.GetBackTopLeft().y,
		zMin = b.GetPosition().z + b.GetFrontBottomLeft().z,
		zMax = b.GetPosition().z + b.GetBackBottomLeft().z;

	float col = false;

	// For each corner of the box b.
	for (int i = 0; i < 8; i++)
	{
		if (CheckCorner(aCorners[i].x, xMin, xMax))
			if (CheckCorner(aCorners[i].y, yMin, yMax))
				if (CheckCorner(aCorners[i].z, zMin, zMax))
					return true;
	}


	//Find effect of b on a.
	xMin = a.GetPosition().x + a.GetBackBottomLeft().x,
		xMax = a.GetPosition().x + a.GetBackBottomRight().x,
		yMin = a.GetPosition().y + a.GetBackBottomLeft().y,
		yMax = a.GetPosition().y + a.GetBackTopLeft().y,
		zMin = a.GetPosition().z + a.GetFrontBottomLeft().z,
		zMax = a.GetPosition().z + a.GetBackBottomLeft().z;

	for (int i = 0; i < 8; i++)
	{
		// Check x axis. 
		if (CheckCorner(bCorners[i].x, xMin, xMax))
			// Check y axis.
			if (CheckCorner(bCorners[i].y, yMin, yMax))
				// Check z axis.
				if (CheckCorner(bCorners[i].z, zMin, zMax))
				{
					const dvec3 p1 = c1.GetBoundingBox().GetPosition();
					const dvec3 p2 = c1.GetBoundingBox().GetPosition();
					const dvec3 d = p2 - p1;
					const double distance = glm::length(d);
					const double sumRadius = a.GetRadius() + b.GetRadius();
					auto depth = sumRadius - distance;
					auto norm = -glm::normalize(d);
					auto pos = p1 - norm * (c1.GetBoundingBox().GetRadius() - depth * 0.5f);
					civ.push_back({ &c1.GetRigidBody(), &c2.GetRigidBody(), pos, norm, depth });
					return true;

				}
	}

}


bool collision::IsColliding(std::vector<Model>&sceneList, std::vector<CollisionInfo> &civ, Model &c1,Model &c2)
{
	
	// Check sphere sphere collision before box collision.
	const dvec3 p1 = c1.GetSphereCollider().GetCenter();
	const dvec3 p2 = c2.GetSphereCollider().GetCenter();
	const dvec3 d = p2 - p1;
	const double distance = glm::length(d);
	const double sumRadius = c1.GetSphereCollider().GetRadius() + c2.GetSphereCollider().GetRadius();
	if (distance < sumRadius)
	{
		//CheckObbObb(civ, c1, c2);
		auto depth = sumRadius - distance;
		auto norm = -glm::normalize(d);
		auto pos = p1 - norm * (c1.GetSphereCollider().GetRadius() - depth * 0.5f);
		civ.push_back({ &c1.GetRigidBody(), &c2.GetRigidBody(), pos, norm, depth });
		
		// BAD CONNER - infinite loop.
		//std::cout << "HERE WE GO" << std::endl;
		//Plane testPlane(glm::vec3(0, -0.5, 0.5), glm::vec3(0, 0.5, -0.5), glm::vec3(0, 0.5, 0.5));
		////Plane testPlane(glm::vec3(-10, -10, 10), glm::vec3(-10, 10, -10), glm::vec3(-10, 10, 10));
		////Plane testPlane(glm::vec3(-0.5, 0,0.5), glm::vec3(0.5, 0, -0.5), glm::vec3(0.5, 0, 0.5));
		//Model test;
		//test.SetModelInfo(SliceModel(sceneList[1], testPlane));
		//test.GetRigidBody().SetInitialPosition(sceneList[1].GetRigidBody().GetPosition() + glm::dvec3(-3, 0, 0));
		//std::cout << to_string(test.GetRigidBody().position) << std::endl;
		//test.SetBoundingBox(BoundingBox(test.GetModelInfo().positions));
		//test.SetSphereCollider(SphereCollider(test.GetModelInfo().positions));
		//test.CreateBuffers();
		//sceneList.push_back(test);
		//return true;
	}


//	enum shape { UNKOWN = 0, PLANE, SPHERE, BOX };
//	shape s1 = UNKOWN;
//	shape s2 = UNKOWN;
//	if (dynamic_cast<const SphereCollider *>(&c1.GetSphereCollider()))
//	{
//		s1 = SPHERE;
//	}
//	else if (dynamic_cast<const BoundingBox *>(&c1.GetSphereCollider()))
//	{
//		s1 = BOX;
//	}
//
////	else if (dynamic_cast<const cPlaneCollider *>(&c1)) {
////		s1 = PLANE;
////	}
////
//
//	if (dynamic_cast<const SphereCollider *>(&c2.GetSphereCollider()))
//	{
//		s2 = SPHERE;
//	}
//	else if (dynamic_cast<const BoundingBox *>(&c2.GetSphereCollider()))
//	{
//		s2 = BOX;
//	}
////	else if (dynamic_cast<const cPlaneCollider *>(&c2)) {
////		s2 = PLANE;
////	}
////
//	if (!s1 || !s2)
//	{
//		std::cout << "Routing Error" << std::endl;
//		return false;
//	}
////	if (s1 == PLANE) {
////		if (s2 == PLANE) {
////			return IsCollidingCheck(civ, dynamic_cast<const cPlaneCollider &>(c1), dynamic_cast<const cPlaneCollider &>(c2));
////		}
////		else if (s2 == SPHERE) {
////			return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c1), dynamic_cast<const cPlaneCollider &>(c2));
////		}
////		else if (s2 == BOX) {
////			return IsCollidingCheck(civ, dynamic_cast<const cPlaneCollider &>(c1), dynamic_cast<const cBoxCollider &>(c2));
////		}
////		else {
////			cout << "Routing Error" << endl;
////			return false;
////		}
////	}
//	if (s1 == SPHERE)
//	{
//		if (s2 == PLANE)
//		{
//		//	return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c1), dynamic_cast<const cPlaneCollider &>(c2));
//		}
//		else if (s2 == SPHERE)
//		{
//			const dvec3 p1 = c1.GetSphereCollider().GetCenter();
//			const dvec3 p2 = c2.GetSphereCollider().GetCenter();
//			const dvec3 d = p2 - p1;
//			const double distance = glm::length(d);
//			const double sumRadius = c1.GetSphereCollider().GetRadius() + c2.GetSphereCollider().GetRadius();
//			if (distance < sumRadius)
//			{
//				auto depth = sumRadius - distance;
//				auto norm = -glm::normalize(d);
//				auto pos = p1 - norm * (c1.GetSphereCollider().GetRadius() - depth * 0.5f);
//				civ.push_back({ &c1.GetRigidBody(), &c2.GetRigidBody(), pos, norm, depth });
//				return true;
//			}
//			//return IsCollidingCheck(civ,c1.GetSphereCollider(), c2.GetSphereCollider());
//			return false;
//		}
//		else if (s2 == BOX)
//		{
//		//	return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c1), dynamic_cast<const cBoxCollider &>(c2));
//		}
//		else
//		{
//			std::cout << "Routing Error" << std::endl;
//			return false;
//		}
//	}
//	else if (s1 == BOX) {
////		if (s2 == PLANE) {
////			return IsCollidingCheck(civ, dynamic_cast<const cPlaneCollider &>(c2), dynamic_cast<const cBoxCollider &>(c1));
////		}
////		else if (s2 == SPHERE) {
////			return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c2), dynamic_cast<const cBoxCollider &>(c1));
////		}
//		if (s2 == BOX)
//		{
//			// CODE FOR BOX TO BOX COLLISIONS.
//
//			return false;
//		}
//		else
//		{
//			cout << "Routing Error" << endl;
//			return false;
//		}
//	}
	return false;
}
