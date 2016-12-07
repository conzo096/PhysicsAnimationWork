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
		// List of new model objects. - Only two.
		std::vector<Model> models = std::vector<Model>();
		
		std::vector<glm::vec3> meshFrag;
		std::vector<glm::vec3> meshold;
		glm::vec3 curPos = m.GetModelInfo().positions[0];
		int side;
	
		// For each triangle in the mesh.
		for (int i = 0; i < m.GetModelInfo().positions.size()-2; i++)
		{
			// Get mesh triangle.
			Triangle tri = Triangle(m.GetModelInfo().positions[i], m.GetModelInfo().positions[i+1], m.GetModelInfo().positions[i+2]);
			// Check how many points of the triangle are on the correct side of the plane.
			int pointsInfront = 0;
			if (glm::dot(p.GetNormal(), tri.GetA()-p.GetPoint()) > 0)
			{
				pointsInfront++;
				if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) > 0)
				{
					pointsInfront++;
					if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) > 0)
					{
						pointsInfront++;
					}
				}
			}
			// If no points are on the correct side ignore.
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
					
						ray.SetDirection(tri.GetC() - ray.GetOrigin());
						meshFrag.push_back(RayPlaneResult(ray, p));
					}
					if (glm::dot(p.GetNormal(), tri.GetB() - p.GetPoint()) + p.GetDistance() > 0)
					{
						ray.SetOrigin(tri.GetB());
						ray.SetDirection(tri.GetA() - ray.GetOrigin());
						meshFrag.push_back(RayPlaneResult(ray, p));

						meshFrag.push_back(tri.GetB());
						
						ray.SetDirection(tri.GetC() - ray.GetOrigin());
						meshFrag.push_back(RayPlaneResult(ray, p));
					}
					if (glm::dot(p.GetNormal(), tri.GetC() - p.GetPoint()) + p.GetDistance() > 0)
					{
						
						ray.SetOrigin(tri.GetC());
						ray.SetDirection(tri.GetA() - ray.GetOrigin());
						meshFrag.push_back(RayPlaneResult(ray, p));
					
						ray.SetDirection(tri.GetB() - ray.GetOrigin());
						meshFrag.push_back(RayPlaneResult(ray, p));

						meshFrag.push_back(tri.GetC());
					}
				}
				// Find intersection point. - could find what point is on the incorrect side then correct it?
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
				}
			}
		}


		std::cout << "Frag:"  + std::to_string(meshFrag.size()) << std::endl;
		std::cout << "old:" + std::to_string(meshold.size()) << std::endl;
		std::cout << "Tot:" + std::to_string(meshFrag.size() + meshold.size()) << std::endl;

		for (int i = 0; i < meshFrag.size() - 1; i++)
			std::cout << glm::to_string(meshFrag[i]) << std::endl;

		info newFragment = info(meshFrag);

		return newFragment;
	}
}