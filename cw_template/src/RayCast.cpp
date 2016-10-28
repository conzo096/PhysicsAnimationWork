#include "RayCast.h"



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
