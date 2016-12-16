#include "RayCast.h"
#include "SphereCollider.h"


namespace phys
{
	
	SphereCollider::SphereCollider()
	{
	}
	SphereCollider::SphereCollider(std::vector<glm::vec3> points)
	{
		// Center point.
		glm::vec3 t = glm::dvec3(0, 0, 0);
		// Furthest point from center.
		glm::vec3 fp = t;
		float dist = 0;
		// DOES NOT WORK FOR IRREGULAR SHAPES.
		for (int i = 0; i < points.size(); i++)
			t += points[i];
		t /= points.size();
		for (int i = 0; i < points.size(); i++)
			if (dist < glm::distance(t, points[i]))
				dist = glm::distance(t, points[i]);


		SetPosition(t);
		SetRadius(dist);

	}
	SphereCollider::~SphereCollider()
	{
	}
}