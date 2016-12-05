#include "Plane.h"

int phys::Plane::SideOfPlane(glm::vec3 p)
{
	int side = 0;

	if (glm::dot((p - point), normal) > 0)
		return INFRONT;
	else if (glm::dot((p - point), normal) < 0)
		return BEHIND;
	else
		return ON;

}
