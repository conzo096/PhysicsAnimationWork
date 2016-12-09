#include "Colliding.h"


bool CheckCorner(float val, float minBound, float maxBound)
{
	return minBound <= val && val <= maxBound;
}


bool CheckObbObb(std::vector<CollisionInfo> &civ, Model& c1, Model& c2)
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
		aCorners[i] = glm::dvec3(aCorners[i] + a.GetPosition());// *c1.GetRigidBody().orientation;
		bCorners[i] = glm::dvec3(bCorners[i] + b.GetPosition());// *c2.GetRigidBody().orientation;
	}

	float xMin = b.GetPosition().x + b.GetBackBottomLeft().x;
	float xMax = b.GetPosition().x + b.GetBackBottomRight().x;
	float yMin = b.GetPosition().y + b.GetBackBottomLeft().y;
	float yMax = b.GetPosition().y + b.GetBackTopLeft().y;
	float zMin = b.GetPosition().z + b.GetFrontBottomLeft().z;
	float zMax = b.GetPosition().z + b.GetBackBottomLeft().z;

	bool col = false;

	// For each corner of the box b.
	for (int i = 0; i < 8; i++)
	{
		if (CheckCorner(aCorners[i].x, xMin, xMax))
			if (CheckCorner(aCorners[i].y, yMin, yMax))
				if (CheckCorner(aCorners[i].z, zMin, zMax))
				{
					col = true;
				}
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
					col = true;
				}
	}

	if (col = true)
	{
		const dvec3 p1 = a.GetPosition();
		const dvec3 p2 = b.GetPosition();
		const dvec3 d = p2 - p1;
		// Calculate distance between two objects.
		const double distance = glm::length(d);
		// Calculate how far inside the sphere the colliding object is.
		const double sumRadius = a.GetRadius() + b.GetRadius();
		auto depth = sumRadius - distance;
		// Direction that the object is to be pushed towards.
		auto norm = glm::normalize(d);
		// How far the object is the be moved in order to be out of the model.
		auto pos = p1 - norm * (c1.GetBoundingBox().GetRadius() - depth);
		civ.push_back({ &c1.GetRigidBody(), &c2.GetRigidBody(), pos, norm, depth});
	}
	return col;
}



bool collision::OnFloor(std::vector<CollisionInfo> & civ, Model &c1, PlaneCollider pc)
{
	BoundingBox a = c1.GetBoundingBox();
	
	glm::dvec3 aCorners[8] = { a.GetBackBottomLeft(),a.GetBackBottomRight(),a.GetBackTopLeft(),a.GetBackTopRight(),
		a.GetFrontBottomLeft(),a.GetFrontBottomRight(),a.GetFrontTopLeft(),a.GetFrontTopRight() };

	for (int i = 0; i < 8; i++)
		aCorners[i] = glm::dvec3(aCorners[i] + a.GetPosition());// *c1.GetRigidBody().orientation;

	bool isCollided = false;
	dvec3 planeNormal = pc.GetNormal();
	for (int i = 0; i < 8; i++)
	{
		double distance = dot(pc.GetPosition(), planeNormal) - dot(aCorners[i], planeNormal);
		if (distance >= 0)
		{
			civ.push_back({ &c1.GetRigidBody(), NULL, aCorners[i] + planeNormal * distance, planeNormal, distance *1.1f });
			isCollided = true;
			return isCollided;
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
