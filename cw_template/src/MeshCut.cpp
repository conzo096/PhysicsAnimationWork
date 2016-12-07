#include "MeshCut.h"
namespace phys
{

	glm::vec3  RayPlaneResult(RayCast ray, Plane pl)
	{
		float scale = -(glm::dot(ray.GetOrigin(), pl.GetNormal() + pl.GetDistance()));
		scale /= glm::dot(ray.GetDirection(), pl .GetNormal());
		return ray.GetOrigin() + (scale*ray.GetDirection());
		
	}

	info SliceModel(Model m,Plane p)
	{
		
		std::vector<glm::vec3> meshFrag;
		std::vector<glm::vec3> meshold;
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

					/*std::cout << "T" << to_string(tri.GetA()) << std::endl;
					std::cout << "M" << to_string(meshFrag[meshFrag.size() - 3]) << std::endl;
					std::cout << "T" << to_string(tri.GetB()) << std::endl;
					std::cout << "M" << to_string(meshFrag[meshFrag.size() - 2]) << std::endl;
					std::cout << "T" << to_string(tri.GetC()) << std::endl;
					std::cout << "M" << to_string(meshFrag[meshFrag.size()-1]) << std::endl;*/

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
					//std::cout << "T" << to_string(tri.GetA()) << std::endl;
					//std::cout << "M" << to_string(meshFrag[meshFrag.size()-3]) << std::endl;
					//std::cout << "T" << to_string(tri.GetB()) << std::endl;
					//std::cout << "M" << to_string(meshFrag[meshFrag.size() - 2]) << std::endl;
					//std::cout << "T" << to_string(tri.GetC()) << std::endl;
					//std::cout << "M" << to_string(meshFrag[meshFrag.size()-1]) << std::endl;
				}
			}
		}


		for (int i = 0; i < m.GetModelInfo().positions.size() - 1; i++)
		{
			std::cout << "OLD: " << glm::to_string(m.GetModelInfo().positions[i]) << std::endl;
			std::cout <<"NEW: " << glm::to_string(meshFrag[i]) << std::endl;
		}
			info newFragment = info(meshFrag);

		return newFragment;
	}
}