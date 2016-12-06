#include "Plane.h"

phys::Plane::Plane(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 ba = b - a;
	glm::vec3 ca = c - a;

	point = a;
	normal = glm::normalize(glm::cross(ba, ca));
	distance = glm::dot(point, normal);
}

int phys::Plane::SideOfPlane(glm::vec3 p)
{
	int side = 0;

	if (glm::dot((p - point), normal) > 0)
		return INFRONT;
	else if (glm::dot((p - point), normal)  < 0)
		return BEHIND;
	else
		return ON;

}

glm::vec3 phys::Plane::ClosestPointOnPlane(glm::vec3 p)
{
	float scalerDist = glm::dot(normal, p) + GetDistance();
	glm::vec3 closestPoint = p - (scalerDist*normal);
	return closestPoint;
}
