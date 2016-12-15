#pragma once
#include <glm/glm.hpp>
#include "stdafx.h"
#include "Collider.h"
namespace phys
{
	class SphereCollider : public Collider
	{
	private:
	public:
		SphereCollider();
		SphereCollider(std::vector<glm::vec3> points);
		~SphereCollider();		


		void Update(double delta) {};
	};
}