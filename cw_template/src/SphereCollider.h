#pragma once
#include <glm/glm.hpp>
#include "stdafx.h"
#include "Collider.h"
namespace phys
{
	class SphereCollider : public Collider
	{
	private:
		glm::dvec3 center;
	public:
		SphereCollider();
		SphereCollider(std::vector<glm::vec3> points);
		~SphereCollider();		
		
		bool SphereSphereCollision(SphereCollider b);

		// Set and gets.

		glm::vec3 GetCenter() { return center; };
		void SetCenter(glm::vec3 c) { center = c; };

		void Update(double delta) {};
	};
}