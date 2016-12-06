#include "MeshCut.h"
namespace phys
{

	glm::vec3  RayPlaneResult(RayCast ray, Plane pl)
	{
		float scale = -(glm::dot(ray.GetOrigin(), pl.GetNormal() + pl.GetDistance()));
		scale /= glm::dot(ray.GetDirection(), pl .GetNormal());
		return ray.GetOrigin() + (scale*ray.GetDirection());
		
	}

	std::vector<Model> SliceModel(Model m,Plane p)
	{
		// List of new model objects. - Only two.
		std::vector<Model> models = std::vector<Model>();
		
		std::vector<glm::vec3> meshFrag;

		glm::vec3 curPos = m.GetModelInfo().positions[0];
		int side;
		// Find starting point.
		if (glm::dot(p.GetNormal(), curPos - p.GetPoint()) > 0)
		{
			side = INFRONT;
			meshFrag.push_back(curPos);
		}
		// If meshFrag is null here then starting position must be on plane.
		else
		{
			side = BEHIND;
			meshFrag.push_back(p.ClosestPointOnPlane(curPos));
		}

		// For rest of vertices in the model.
		for (int i = 1; i < m.GetModelInfo().positions.size(); i++)
		{
			glm::vec3 curPos = m.GetModelInfo().positions[i];

		
			if (glm::dot(p.GetNormal(), curPos - p.GetPoint()) > 0)
			{
				// Same side.
				if (side == INFRONT)
				{
					meshFrag.push_back(curPos);
				}
				// IF ON ANOTHER SIDE CALCULATE WHERE IT INTERSECTS WITH THE PLANE.
				else
				{
					RayCast ray; 
					ray.SetOrigin(curPos);
					ray.SetDirection(curPos - m.GetModelInfo().positions[i - 1]);
					meshFrag.push_back(RayPlaneResult(ray,p));
				}
			}
			else
			{
				side = BEHIND;
			}


			std::cout << meshFrag.size() << std::endl;

		}


		return models;
	}
}