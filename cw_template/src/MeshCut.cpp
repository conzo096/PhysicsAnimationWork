#include "MeshCut.h"
namespace phys
{

	//glm::vec3  RayPlaneResult(RayCast ray, Plane pl)
	//{
	//	float scale = -(glm::dot(ray.GetOrigin(), pl.GetNormal() + pl.GetDistance()));
	//	scale /= glm::dot(ray.GetDirection(), pl .GetNormal());
	//	return ray.GetOrigin() + (scale*ray.GetDirection());
	//	
	//}

	void SliceModel(Model& m,Plane p, std::vector<Model> & newFragments)
	{
	
		std::vector<glm::vec3> meshFrag;
		std::vector<glm::vec3> updateOrigMesh;


		glm::dvec3 currentMid = m.GetModelInfo().GetMidPoint();


		// For each triangle in the mesh.
		for (int i = 0; i < m.GetModelInfo().positions.size(); i+=3)
		{
			// Get mesh triangle.
			Triangle tri = Triangle(m.GetModelInfo().positions[i], m.GetModelInfo().positions[i+1], m.GetModelInfo().positions[i+2]);
			// Check how many points of the triangle are on the correct side of the plane.
			int pointsInfront = 0;
			if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint())> 0)
				pointsInfront++;
			if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint())> 0)
				pointsInfront++;
			if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint())> 0)
				pointsInfront++;

			// Now what about if the point lies on the plane?
			int onPlane = 0;
			if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint())== 0)
				onPlane++;
			if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint())== 0)
				onPlane++;
			if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint())== 0)
				onPlane++;



			// If no points are on the correct side add points which would lie on the plane.
			if (pointsInfront == 0)
			{
				meshFrag.push_back(p.ClosestPointOnPlane(tri.GetA()));
				meshFrag.push_back(p.ClosestPointOnPlane(tri.GetB()));
				meshFrag.push_back(p.ClosestPointOnPlane(tri.GetC()));
			}
			else if (pointsInfront != 0)
			{
				// If 3 points are on the correct side add complete triangle.
				if (pointsInfront == 3 || onPlane == 3)
				{
					meshFrag.push_back(tri.GetA());
					meshFrag.push_back(tri.GetB());
					meshFrag.push_back(tri.GetC());
				}
				// Other cases have an intersection, find out where.

				// One point on correct side - Two intersections.
				else if (pointsInfront == 1)
				{
					if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint())> 0)
					{
						meshFrag.push_back(tri.GetA());
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetB()));
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetC()));

					}
					if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint())> 0)
					{
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetA()));
						meshFrag.push_back(tri.GetB());
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}


					if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint())> 0)
					{
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetA()));
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetB()));
						meshFrag.push_back(tri.GetC());
					}
				}
				// Two points are on the correct side - One intersection.
				else if (pointsInfront == 2)
				{
					if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint())< 0)
					{
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetA()));
						meshFrag.push_back(tri.GetB());
						meshFrag.push_back(tri.GetC());
					}
					else if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) < 0)
					{
						meshFrag.push_back(tri.GetA());
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetB()));
						meshFrag.push_back(tri.GetC());
					}
					else if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint())< 0)
					{
						meshFrag.push_back(tri.GetA());
						meshFrag.push_back(tri.GetB());
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}
				}
			}
		}

		// To find the vertices for the original mesh. Just flip the calculations.
		

		for (int i = 0; i < m.GetModelInfo().positions.size(); i += 3)
		{
			// Get mesh triangle.
			Triangle tri = Triangle(m.GetModelInfo().positions[i], m.GetModelInfo().positions[i + 1], m.GetModelInfo().positions[i + 2]);
			// Check how many points of the triangle are on the correct side of the plane.
			int pointsBehind = 0;
			if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint())< 0)
				pointsBehind++;
			if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint())< 0)
				pointsBehind++;
			if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint())< 0)
				pointsBehind++;

			// Now what about if the point lies on the plane?
			int onPlane = 0;
			if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) == 0)
				onPlane++;
			if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) == 0)
				onPlane++;
			if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) == 0)
				onPlane++;

			// If no points are on the correct side add points which would lie on the plane.
			if (pointsBehind == 0)
			{
				updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetA()));
				updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetB()));
				updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetC()));
			}
			else if (pointsBehind != 0)
			{
				// If 3 points are on the correct side add complete triangle.
				if (pointsBehind == 3 || onPlane == 3)
				{
					updateOrigMesh.push_back(tri.GetA());
					updateOrigMesh.push_back(tri.GetB());
					updateOrigMesh.push_back(tri.GetC());
				}
			//	// Other cases have an intersection, find out where.

			//	// One point behind the plane.
				else if (pointsBehind == 1)
				{
					 //Check if a is behind plane.
					if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint())< 0)
					{
						updateOrigMesh.push_back(tri.GetA());
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetB()));
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}
					 // Check if b is behind plane.
					if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) < 0)
					{
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetA()));
						updateOrigMesh.push_back(tri.GetB());
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}

					// Check if c is behind plane.
					if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint())< 0)
					{
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetA()));
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetB()));
						updateOrigMesh.push_back(tri.GetC());

					}
				}
			//	// Find intersection point. - THIS IS WHAT IS BORKEN
				else if (pointsBehind == 2)
				{
					if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint())> 0)
					{
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetA()));
						updateOrigMesh.push_back(tri.GetB());
						updateOrigMesh.push_back(tri.GetC());
					}
					if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint())> 0)
					{
						updateOrigMesh.push_back(tri.GetA());
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetB()));
						updateOrigMesh.push_back(tri.GetC());
					}
					else if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint())> 0)
					{
						updateOrigMesh.push_back(tri.GetA());
						updateOrigMesh.push_back(tri.GetB());
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}
				}
			}
		}



		// If points lie on the plane then they are added to both models.

		Model test = Model(meshFrag);
		glm::dvec3 centre = m.GetModelInfo().GetMidPoint();
		glm::dvec3 newMPos = test.GetModelInfo().GetMidPoint();
		glm::dvec3 diff = newMPos - centre;
		CreateSplittingPlanes(test.GetBoundingBox(), 4, test.GetSplittingPlanes());
		test.SetRigidBody(m.GetRigidBody());
		test.GetRigidBody().SetInitialPosition(test.GetRigidBody().GetPosition() + (diff));
		test.CreateBuffers();
		test.Update(0);

		Model newMod = Model(updateOrigMesh);
		newMod.SetRigidBody(m.GetRigidBody());
		newMPos = newMod.GetModelInfo().GetMidPoint();
		diff = newMPos - currentMid;
		CreateSplittingPlanes(newMod.GetBoundingBox(), 4, newMod.GetSplittingPlanes());
		newMod.GetRigidBody().SetInitialPosition(m.GetRigidBody().GetPosition() + (diff));
		m = newMod;
		m.CreateBuffers();
		m.Update(0);

		newFragments.push_back(test);

	}
}