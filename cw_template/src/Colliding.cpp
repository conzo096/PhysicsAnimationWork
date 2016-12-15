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
					const double sumRadius = c1.GetSphereCollider().GetRadius() + c2.GetSphereCollider().GetRadius();
					auto depth = sumRadius - distance;
					// Direction that the object is to be pushed towards.
					auto norm = -glm::normalize(d);
					// How far the object is the be moved in order to be out of the model.
					auto pos = p1 - norm * (c1.GetSphereCollider().GetRadius() - depth * 0.5f);
					// Push information back to be resolved later.
					civ.push_back({ &c1.GetRigidBody(), &c2.GetRigidBody(), pos , norm, depth });
				}
	}
	return col;
}



bool collision::OnFloor(std::vector<CollisionInfo> & civ, Model &c1, PlaneCollider pc)
{

	const dvec3 sp = c1.GetSphereCollider().GetPosition();
	// Calculate a vector from a point on the plane to the center of the sphere
	const dvec3 vecTemp(sp - pc.GetPosition());

	// Calculate the distance: dot product of the new vector with the plane's normal
	double dist = glm::dot(vecTemp, pc.GetNormal());

	if (dist <= c1.GetSphereCollider().GetRadius())
	{
		BoundingBox a = c1.GetBoundingBox();
		glm::dvec3 aCorners[8] = { a.GetBackBottomLeft(),a.GetBackBottomRight(),a.GetBackTopLeft(),a.GetBackTopRight(),
			a.GetFrontBottomLeft(),a.GetFrontBottomRight(),a.GetFrontTopLeft(),a.GetFrontTopRight() };

		const mat4 m = glm::translate(c1.GetRigidBody().position) *glm::mat4_cast(c1.GetRigidBody().orientation);
		for (int i = 0; i < 8; i++)
			aCorners[i] = dvec3(m * dvec4(aCorners[i], 1.0));

		bool isCollided = false;
		double distance;
		for (int i = 0; i < 8; i++)
		{
			distance = dot(pc.GetPosition(), pc.GetNormal()) - dot(aCorners[i], pc.GetNormal());
			if (distance > 0)
			{
				// If there is a collision between the plane that needs to be resolved. Amplify the distance it is pushed up by in order to have the cube sit on top.

				civ.push_back({ &c1.GetRigidBody(), NULL, aCorners[i] + pc.GetNormal() * distance, pc.GetNormal(), distance * 5 });
				isCollided = true;
			}
		}
		return isCollided;
	}
	return false;
}

bool collision::IsColliding(std::vector<Model>&sceneList, std::vector<CollisionInfo> &civ, Model &c1, Model &c2)
{

	// Check sphere sphere collision before box collision.
	const dvec3 p1 = c1.GetSphereCollider().GetPosition();
	const dvec3 p2 = c2.GetSphereCollider().GetPosition();
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