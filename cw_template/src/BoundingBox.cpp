#include "BoundingBox.h"
namespace phys
{
	BoundingBox::BoundingBox(){}

	BoundingBox::BoundingBox(std::vector<glm::vec3> points)
	{
		// Find top x,y,z.
		for (glm::vec3 pos : points)
		{
			// top x,y,z
			if (pos.x > topX)
				topX = pos.x;
			if (pos.y > topY)
				topY = pos.y;
			if (pos.z > topZ)
				topZ = pos.z;
			// bot x,y,z
			if (pos.x < botX)
				botX = pos.x;
			if (pos.y < botY)
				botY = pos.y;
			if (pos.z < botZ)
				botZ = pos.z;
		}
		
		// Set the radius.
		glm::vec3 centre;
		for (int i = 0; i < points.size(); i++)
			centre += points[i];
		centre /= points.size();
			
		SetRadius(glm::length(centre - GetFrontTopRight()));
	}

	BoundingBox::~BoundingBox(){}
}