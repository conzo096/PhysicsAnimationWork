#include "MeshCut.h"
namespace phys
{
	MeshCut::MeshCut()
	{
	}
	MeshCut::~MeshCut()
	{
	}

	std::vector<Model> MeshCut::SliceModel(Model m,Plane p)
	{
		// List of new model objects. - Only two.
		std::vector<Model> models = std::vector<Model>();
		
		info oldModel;
		info meshFrag;

		glm::vec3 pointA, pointB, pointC;
		// For each triangle.
		for (int i = 0; i < m.GetModelInfo().positions.size()-3; i++)
		{
			pointA = m.GetModelInfo().positions[i];
			pointB = m.GetModelInfo().positions[i+1];
			pointC = m.GetModelInfo().positions[i+2];
			// Keep track of what side the previous point was on.
			int prevSide;

			// Check the first point of triangle.
			if (p.SideOfPlane(pointA) == BEHIND)
			{
				oldModel.positions.push_back(pointA);
				prevSide == BEHIND;
			}
			else if (p.SideOfPlane(pointA) == INFRONT)
			{
				meshFrag.positions.push_back(pointA);
				prevSide == INFRONT;
			}
			// If there is an intersection add the point to both.
			else if (p.SideOfPlane(pointA) == ON)
			{

				oldModel.positions.push_back(pointA);
				prevSide == ON;
			}
			// Check second point.
			if (p.SideOfPlane(pointB) == BEHIND && prevSide == BEHIND)
			{
				oldModel.positions.push_back(pointB);
				prevSide = BEHIND;
			}
			else if (p.SideOfPlane(pointB) == BEHIND && prevSide == INFRONT)
			{
				// INTERSECTION.
				oldModel.positions.push_back(pointB);
				prevSide = INFRONT;
			}
			else if (p.SideOfPlane(pointB) == INFRONT && prevSide == BEHIND)
			{
				// INTERSECTION.
				oldModel.positions.push_back(pointB);
				prevSide = INFRONT;
			}
			else if (p.SideOfPlane(pointB) == BEHIND && prevSide == INFRONT)
			{
				// INTERSECTION.
				oldModel.positions.push_back(pointB);
				prevSide = INFRONT;
			}
			else if (p.SideOfPlane(pointB) == BEHIND && prevSide == INFRONT)
			{
				// INTERSECTION.
				oldModel.positions.push_back(pointB);
				prevSide = INFRONT;
			}
		}

		return models;
	}
}