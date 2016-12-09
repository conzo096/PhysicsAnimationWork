#include "MeshCut.h"
namespace phys
{

	glm::vec3  RayPlaneResult(RayCast ray, Plane pl)
	{
		float scale = -(glm::dot(ray.GetOrigin(), pl.GetNormal() + pl.GetDistance()));
		scale /= glm::dot(ray.GetDirection(), pl .GetNormal());
		return ray.GetOrigin() + (scale*ray.GetDirection());
		
	}

	info SliceModel(Model& m,Plane p)
	{
		
		std::vector<glm::vec3> meshFrag;
		std::vector<glm::vec3> updateOrigMesh;
		glm::vec3 curPos;

		// For each triangle in the mesh.
		for (int i = 0; i < m.GetModelInfo().positions.size(); i+=3)
		{
			// Get mesh triangle.
			Triangle tri = Triangle(m.GetModelInfo().positions[i], m.GetModelInfo().positions[i+1], m.GetModelInfo().positions[i+2]);
			// Check how many points of the triangle are on the correct side of the plane.
			int pointsInfront = 0;
			if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) + p.GetDistance() > 0)
				pointsInfront++;
			if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) + p.GetDistance() > 0)
				pointsInfront++;
			if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) + p.GetDistance() > 0)
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
				RayCast ray;
				// If 3 points are on the correct side add complete triangle.
				if (pointsInfront == 3)
				{
					meshFrag.push_back(tri.GetA());
					meshFrag.push_back(tri.GetB());
					meshFrag.push_back(tri.GetC());
				}
				// Other cases have an intersection, find out where.
				// One intersection.
				else if (pointsInfront == 1)
				{
					if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) + p.GetDistance() > 0)
					{
						meshFrag.push_back(tri.GetA());

						ray.SetOrigin(tri.GetA());
						ray.SetDirection(tri.GetB() - ray.GetOrigin());
						meshFrag.push_back(RayPlaneResult(ray, p));
					
						ray.SetOrigin(meshFrag[meshFrag.size() - 1]);
						ray.SetDirection(tri.GetC() - ray.GetOrigin());
						meshFrag.push_back(RayPlaneResult(ray, p));
					}
					// fix this.
					if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) + p.GetDistance() > 0)
					{
						ray.SetOrigin(tri.GetB());
						ray.SetDirection(tri.GetA() - ray.GetOrigin());
						meshFrag.push_back(RayPlaneResult(ray, p));

						meshFrag.push_back(tri.GetB());

						// THIS IS INCORRECT.
						
						//ray.SetDirection(ray.GetOrigin() - tri.GetC());
						//meshFrag.push_back(RayPlaneResult(ray, p));

						// This should not work?
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}


					if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) + p.GetDistance() > 0)
					{
						
						ray.SetOrigin(tri.GetC());
						ray.SetDirection(tri.GetA() - ray.GetOrigin());
						meshFrag.push_back(RayPlaneResult(ray, p));
					
						ray.SetOrigin(meshFrag[meshFrag.size() - 1]);
						ray.SetDirection(tri.GetB() - ray.GetOrigin());
						meshFrag.push_back(RayPlaneResult(ray, p));

						meshFrag.push_back(tri.GetC());
					}

					//std::cout << "T" << to_string(tri.GetA()) << std::endl;
					//std::cout << "M" << to_string(meshFrag[meshFrag.size() - 3]) << std::endl;
					//std::cout << "T" << to_string(tri.GetB()) << std::endl;
					//std::cout << "M" << to_string(meshFrag[meshFrag.size() - 2]) << std::endl;
					//std::cout << "T" << to_string(tri.GetC()) << std::endl;
					//std::cout << "M" << to_string(meshFrag[meshFrag.size()-1]) << std::endl;
					//std::cout << "" << std::endl;
				}
				// Find intersection point. - THIS IS WHAT IS BORKEN
				else if (pointsInfront == 2)
				{
					if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) + p.GetDistance() < 0)
					{
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetA()));
						meshFrag.push_back(tri.GetB());
						meshFrag.push_back(tri.GetC());
					}
					else if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) +p.GetDistance() < 0)
					{
						meshFrag.push_back(tri.GetA());
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetB()));
						meshFrag.push_back(tri.GetC());
					}
					else if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) + p.GetDistance() < 0)
					{
						meshFrag.push_back(tri.GetA());
						meshFrag.push_back(tri.GetB());
						meshFrag.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}
				/*	std::cout << "T" << to_string(tri.GetA()) << std::endl;
					std::cout << "M" << to_string(meshFrag[meshFrag.size()-3]) << std::endl;
					std::cout << "T" << to_string(tri.GetB()) << std::endl;
					std::cout << "M" << to_string(meshFrag[meshFrag.size() - 2]) << std::endl;
					std::cout << "T" << to_string(tri.GetC()) << std::endl;
					std::cout << "M" << to_string(meshFrag[meshFrag.size()-1]) << std::endl;*/
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
			if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) + p.GetDistance() <= 0)
				pointsBehind++;
			if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) + p.GetDistance() <= 0)
				pointsBehind++;
			if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) + p.GetDistance() <= 0)
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
				RayCast ray;
				// If 3 points are on the correct side add complete triangle.
				if (pointsBehind == 3)
				{
					updateOrigMesh.push_back(tri.GetA());
					updateOrigMesh.push_back(tri.GetB());
					updateOrigMesh.push_back(tri.GetC());
				}
				// Other cases have an intersection, find out where.
				
				// One point behind the plane.
				else if (pointsBehind == 1)
				{
					// Check if a is behind plane.
					if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) + p.GetDistance() < 0)
					{
						updateOrigMesh.push_back(tri.GetA());

						ray.SetOrigin(tri.GetA());
						ray.SetDirection(tri.GetB() - ray.GetOrigin());
						updateOrigMesh.push_back(RayPlaneResult(ray, p));

						ray.SetOrigin(tri.GetB());
						ray.SetDirection(tri.GetC() - ray.GetOrigin());
						updateOrigMesh.push_back(RayPlaneResult(ray, p));
					}
					// Check if b is behind plane.
					if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) + p.GetDistance() < 0)
					{
						ray.SetOrigin(tri.GetB());
						ray.SetDirection(tri.GetA() - ray.GetOrigin());
						updateOrigMesh.push_back(RayPlaneResult(ray, p));

						updateOrigMesh.push_back(tri.GetB());

						// THIS IS INCORRECT.

						//ray.SetDirection(ray.GetOrigin() - tri.GetC());
						//updateOrigMesh.push_back(RayPlaneResult(ray, p));

						// This should not work?
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}

					// Check if c is behind plane.
					if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) + p.GetDistance() < 0)
					{
						// Find line intersect of CA to the plane.
						ray.SetOrigin(tri.GetA());
						ray.SetDirection(tri.GetC() - tri.GetA());
						updateOrigMesh.push_back(RayPlaneResult(ray, p));
						std::cout << "T" << to_string(tri.GetA()) << std::endl;
						std::cout << "M" << to_string(updateOrigMesh[updateOrigMesh.size() - 1]) << std::endl;
						// Find location of 2nd point of triangle.
						ray.SetOrigin(tri.GetB());

						ray.SetDirection(tri.GetC() - tri.GetB());
						updateOrigMesh.push_back(RayPlaneResult(ray, p));
						std::cout << "T" << to_string(tri.GetB()) << std::endl;
						std::cout << "M" << to_string(updateOrigMesh[updateOrigMesh.size() - 1]) << std::endl;
						updateOrigMesh.push_back(tri.GetC());
						std::cout << "T" << to_string(tri.GetC()) << std::endl;
						std::cout << "M" << to_string(updateOrigMesh[updateOrigMesh.size() - 1]) << std::endl;
						std::cout << "" << std::endl;

					}
				}
				// Find intersection point. - THIS IS WHAT IS BORKEN
				else if (pointsBehind == 2)
				{
					if (glm::dot(p.GetNormal(), tri.GetA() - p.GetPoint()) + p.GetDistance() > 0)
					{
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetA()));
						updateOrigMesh.push_back(tri.GetB());
						updateOrigMesh.push_back(tri.GetC());
					}
					else if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) + p.GetDistance() > 0)
					{
						updateOrigMesh.push_back(tri.GetA());
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetB()));
						updateOrigMesh.push_back(tri.GetC());
					}
					else if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) + p.GetDistance() > 0)
					{
						updateOrigMesh.push_back(tri.GetA());
						updateOrigMesh.push_back(tri.GetB());
						updateOrigMesh.push_back(p.ClosestPointOnPlane(tri.GetC()));
					}
				}
			}
		}


		// Create mesh fragment.
//		Model newMod = Model(meshFrag);
//		newMod.GetRigidBody().SetInitialPosition(m.GetRigidBody().GetPosition());
	
		// Update original model.
		Model updateMod = Model(meshFrag);
	//	updateMod.GetRigidBody().SetInitialPosition(m.GetRigidBody().GetPosition()+glm::dvec3(0,2,0));
		m = updateMod;

		return info(updateOrigMesh);
	}
}