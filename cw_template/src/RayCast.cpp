#include "RayCast.h"


namespace phys
{
	RayCast::RayCast()
	{
	}

	RayCast::RayCast(glm::vec3 startPoint, glm::vec3 endPoint)
	{
		origin = startPoint;
		direction = glm::normalize(endPoint - startPoint);
		range = glm::distance(startPoint, endPoint);


	}
	RayCast::~RayCast()
	{
	}

	bool RayCast::DetectCollision(glm::vec3 point)
	{
		return false;
	}

	void RayCast::SetRandomOrigin(glm::vec3 minBounds, glm::vec3 maxBounds)
	{
		glm::vec3 point = glm::vec3(0,0,0);

		// Find x range.
		point.x = rand() % (int)maxBounds.x + minBounds.x;
		point.y = rand() % (int)maxBounds.y + minBounds.y;
		point.z = rand() % (int)maxBounds.z + minBounds.z;

		origin = point;
	}
}