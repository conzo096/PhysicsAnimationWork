#include "Colliding.h"


bool CheckCorner(float val, float minBound, float maxBound)
{
	return minBound <= val && val <= maxBound;
}


bool CheckObbObb(std::vector<CollisionInfo> &civ, Model& c1, Model& c2)
{
	BoundingBox a = c1.GetBoundingBox();
	BoundingBox b = c2.GetBoundingBox();
	glm::dvec3 collidingCorner;
	// Store original corners in an array.
	glm::dvec3 aCorners[8] = { a.GetBackBottomLeft(),a.GetBackBottomRight(),a.GetBackTopLeft(),a.GetBackTopRight(),
		a.GetFrontBottomLeft(),a.GetFrontBottomRight(),a.GetFrontTopLeft(),a.GetFrontTopRight() };
	glm::dvec3 bCorners[8] = { b.GetBackBottomLeft(),b.GetBackBottomRight(),b.GetBackTopLeft(),b.GetBackTopRight(),
		b.GetFrontBottomLeft(),b.GetFrontBottomRight(),b.GetFrontTopLeft(),b.GetFrontTopRight() };

	const mat4 m = glm::translate(c1.GetRigidBody().position) * glm::mat4_cast(c1.GetRigidBody().orientation);
	const mat4 m2 = glm::translate(c2.GetRigidBody().position) * glm::mat4_cast(c2.GetRigidBody().orientation);
	for (int i = 0; i < 8; i++)
	{
		aCorners[i] = dvec3(m * dvec4(aCorners[i], 1.0));
		bCorners[i] = dvec3(m2 * dvec4(bCorners[i], 1.0));

	}

	float xMin = b.GetPosition().x + b.GetBackBottomLeft().x;
	float xMax = b.GetPosition().x + b.GetBackBottomRight().x;
	float yMin = b.GetPosition().y + b.GetBackBottomLeft().y;
	float yMax = b.GetPosition().y + b.GetBackTopLeft().y;
	float zMin = b.GetPosition().z + b.GetFrontBottomLeft().z;
	float zMax = b.GetPosition().z + b.GetBackBottomLeft().z;


	for (int i = 0; i < 8; i++)
	{
		if (bCorners[i].x < xMin)
			xMin = bCorners[i].x;
		else if (bCorners[i].x > xMax)
			xMax = bCorners[i].x;
		if (bCorners[i].y < yMin)
			yMin = bCorners[i].y;
		else if (bCorners[i].y > yMax)
			yMax = bCorners[i].y;
		if (bCorners[i].z < zMin)
			zMin = bCorners[i].z;
		else if (bCorners[i].z > zMax)
			zMax = bCorners[i].z;
	}

	bool col = false;

	// For each corner of  box b.
	for (int i = 0; i < 8; i++)
	{
		if (CheckCorner(aCorners[i].x, xMin, xMax))
			if (CheckCorner(aCorners[i].y, yMin, yMax))
				if (CheckCorner(aCorners[i].z, zMin, zMax))
				{
					col = true;
					const dvec3 p1 = a.GetPosition();
					const dvec3 p2 = b.GetPosition();
					const dvec3 d = p2 - p1;
					// Calculate distance between two objects.
					const double distance = glm::length(d);
					// Find the distance between colliding centre and colliding point
					auto depth =  (length(p1 - aCorners[i]) + length(p2 - bCorners[i])) - distance;
					
				
					// Direction that the object is to be pushed towards.
					//auto t = aCorners[i] - p1;
					auto norm = glm::normalize(d);
					// How far the object is the be moved in order to be out of the model.
					auto pos = p2 - norm *	(length(p2 - bCorners[i]) - depth);
					civ.push_back({ &c1.GetRigidBody(), &c2.GetRigidBody(), pos, norm, depth});
				}
	}

	//if (col == false)
	//if (glm::length (c2.GetRigidBody().prev_pos - c2.GetRigidBody().position) < 1 || glm::length(c1.GetRigidBody().prev_pos - c1.GetRigidBody().position) < 1)
	//{
	//	xMin = a.GetPosition().x + a.GetBackBottomLeft().x;
	//	xMax = a.GetPosition().x + a.GetBackBottomRight().x;
	//	yMin = a.GetPosition().y + a.GetBackBottomLeft().y;
	//	yMax = a.GetPosition().y + a.GetBackTopLeft().y;
	//	zMin = a.GetPosition().z + a.GetFrontBottomLeft().z;
	//	zMax = a.GetPosition().z + a.GetBackBottomLeft().z;


	//	for (int i = 0; i < 8; i++)
	//	{
	//		if (aCorners[i].x < xMin)
	//			xMin = aCorners[i].x;
	//		else if (aCorners[i].x > xMax)
	//			xMax = aCorners[i].x;
	//		if (aCorners[i].y < yMin)
	//			yMin = aCorners[i].y;
	//		else if (aCorners[i].y > yMax)
	//			yMax = aCorners[i].y;
	//		if (aCorners[i].z < zMin)
	//			zMin = aCorners[i].z;
	//		else if (aCorners[i].z > zMax)
	//			zMax = aCorners[i].z;
	//	}

	//	 //For each corner of the box b.
	//	for (int i = 0; i < 8; i++)
	//	{
	//		if (CheckCorner(bCorners[i].x, xMin, xMax))
	//			if (CheckCorner(bCorners[i].y, yMin, yMax))
	//				if (CheckCorner(bCorners[i].z, zMin, zMax))
	//				{
	//					const dvec3 p1 = a.GetPosition();
	//					const dvec3 p2 = b.GetPosition();
	//					const dvec3 d = p1 - p2;
	//					// Calculate distance between two objects.
	//					const double distance = glm::length(d);
	//					// Find the distance between colliding centre and colliding point
	//					auto depth = (length(p1 - aCorners[i]) + length(p2 - bCorners[i]) - distance);
	//					// Direction that the object is to be pushed towards.
	//					auto norm = glm::normalize(d);
	//					// How far the object is the be moved in order to be out of the model.
	//					auto pos = p1 - norm * (length(p1 - aCorners[i]) - depth);
	//					civ.push_back({ &c2.GetRigidBody(), &c1.GetRigidBody(), pos, norm, depth });
	//				}
	//	}
	//}
	return col;
	}



bool collision::OnFloor(std::vector<CollisionInfo> & civ, Model &c1, PlaneCollider pc)
{	
	
	BoundingBox a = c1.GetBoundingBox();
	glm::dvec3 aCorners[8] = { a.GetBackBottomLeft(),a.GetBackBottomRight(),a.GetBackTopLeft(),a.GetBackTopRight(),
		a.GetFrontBottomLeft(),a.GetFrontBottomRight(),a.GetFrontTopLeft(),a.GetFrontTopRight() };

	const mat4 m = glm::translate(c1.GetRigidBody().position) *glm::mat4_cast(c1.GetRigidBody().orientation);
	for (int i = 0; i < 8; i++)
		aCorners[i] = dvec3(m * dvec4(aCorners[i], 1.0));

	bool isCollided = false;
	double distances[8];
	for (int i = 0; i < 8; i++)
	{
		distances[i] = dot(pc.GetPosition(), pc.GetNormal()) - dot(aCorners[i], pc.GetNormal());
		//distances[i] = glm::dot(pc.GetNormal(), aCorners[i] - pc.GetPosition());
		if (distances[i] > 0)
		{
			// If there is a collision between the plane that needs to be resolved. Amplify the distance it is pushed up by in order to have the cube sit on top.
			distances[i] * 1.3;
			civ.push_back({ &c1.GetRigidBody(), &c1.GetRigidBody(), aCorners[i] + pc.GetNormal() * distances[i], pc.GetNormal(), distances[i]});
			isCollided = true;
		}
	}
	return isCollided;
}

bool collision::IsColliding(std::vector<Model>&sceneList, std::vector<CollisionInfo> &civ, Model &c1,Model &c2)
{
	
	// Check sphere sphere collision before box collision.
	const dvec3 p1 = c1.GetSphereCollider().GetCenter();
	const dvec3 p2 = c2.GetSphereCollider().GetCenter();
	const dvec3 d = p2 - p1;
	const double distance = glm::length(d);
	const double sumRadius = c1.GetSphereCollider().GetRadius() + c2.GetSphereCollider().GetRadius();
	// If it is in the sphere, check the bounding box collision.
	if (distance < sumRadius)
	{
		CheckObbObb(civ, c1, c2);
			return true;
	}
	return false;
}
