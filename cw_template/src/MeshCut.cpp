#include "MeshCut.h"
namespace phys
{

	// This class cuts a mesh in half updates the model that has broken. 
	void SliceModel(Model& m,Plane p, std::vector<Model>& newFragments)
	{

		// Stores the vertex positions of the new fragment.
		std::vector<glm::vec3> meshFrag;
		// Stores the vertex positions of the updated original model..
		std::vector<glm::vec3> updateOrigMesh;
		// Current center point of the model. - used to calculate the displacement.
		glm::dvec3 currentMid = m.GetModelInfo().GetMidPoint();
		// For each triangle in the mesh.
		for (int i = 0; i < m.GetModelInfo().positions.size(); i += 3)
		{
			// Get mesh triangle.
			Triangle tri = Triangle(m.GetModelInfo().positions[i], m.GetModelInfo().positions[i + 1], m.GetModelInfo().positions[i + 2]);
			// Check how many points of the triangle are on the correct side of the plane.
			int pointsInfront = 0;
			if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) >= 0)
				pointsInfront++;
			if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) >= 0)
				pointsInfront++;
			if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) >= 0)
				pointsInfront++;


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
				if (pointsInfront == 3)
				{
					meshFrag.push_back(tri.GetA());
					meshFrag.push_back(tri.GetB());
					meshFrag.push_back(tri.GetC());
				}
		
				// One point on correct side - Two intersections.
				else if (pointsInfront == 1)
				{
					if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) >= 0)
					{
						meshFrag.push_back(tri.GetA());
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetB()));
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetC()));

					}
					if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) >= 0)
					{
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetA()));
						meshFrag.push_back(tri.GetB());
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}


					if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) >= 0)
					{
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetA()));
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetB()));
						meshFrag.push_back(tri.GetC());
					}
				}
				// Two points are on the correct side - One intersection.
				else if (pointsInfront == 2)
				{
					if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) < 0)
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
					else if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) < 0)
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
			if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) <= 0)
				pointsBehind++;
			if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) <= 0)
				pointsBehind++;
			if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) <= 0)
				pointsBehind++;

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
				if (pointsBehind == 3)
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
					if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) <= 0)
					{
						updateOrigMesh.push_back(tri.GetA());
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetB()));
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}
					// Check if b is behind plane.
					if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) <= 0)
					{
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetA()));
						updateOrigMesh.push_back(tri.GetB());
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}

					// Check if c is behind plane.
					if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) <= 0)
					{
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetA()));
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetB()));
						updateOrigMesh.push_back(tri.GetC());

					}
				}
				//	// Find intersection point.
				else if (pointsBehind == 2)
				{
					if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) > 0)
					{
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetA()));
						updateOrigMesh.push_back(tri.GetB());
						updateOrigMesh.push_back(tri.GetC());
					}
					if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) > 0)
					{
						updateOrigMesh.push_back(tri.GetA());
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetB()));
						updateOrigMesh.push_back(tri.GetC());
					}
					else if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) > 0)
					{
						updateOrigMesh.push_back(tri.GetA());
						updateOrigMesh.push_back(tri.GetB());
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}
				}
			}
		}


		// Reduce number of splits possible on new fragments..
		int numSplit = m.GetSplittingPlanes().size();
		if (numSplit > 1)
			numSplit -= 1;

		Model test = Model(meshFrag);
		glm::dvec3 centre = m.GetModelInfo().GetMidPoint();
		glm::dvec3 newMPos = test.GetModelInfo().GetMidPoint();
		glm::dvec3 diff = (newMPos - centre);
		CreateSplittingPlanes(test.GetBoundingBox(),numSplit , test.GetSplittingPlanes());
		test.GetRigidBody().SetInitialPosition(m.GetRigidBody().GetPosition() + (diff));
		test.GetRigidBody().AddLinearImpulse((diff + glm::dvec3(0,1,0))*0.05);
		test.CreateBuffers();
		test.Update(0);

		Model newMod = Model(updateOrigMesh);
		newMod.GetRigidBody().orientation = m.GetRigidBody().orientation;
		newMPos = newMod.GetModelInfo().GetMidPoint();
		diff = newMPos - currentMid;
		CreateSplittingPlanes(newMod.GetBoundingBox(), numSplit, newMod.GetSplittingPlanes());
		newMod.GetRigidBody().SetInitialPosition(m.GetRigidBody().GetPosition() + (diff));
		newMod.GetRigidBody().AddLinearImpulse((diff + glm::dvec3(0, 1, 0))*0.05);
		m = newMod;
		m.CreateBuffers();
		m.Update(0);

		// Push back the new fragment to the vector which hold all of them.
		newFragments.push_back(test);
	}
}