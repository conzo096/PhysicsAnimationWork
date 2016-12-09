#pragma once
#include <glm/glm.hpp>
#include "stdafx.h"
namespace phys
{
	class PlaneCollider : Collider
	{
	private:
		glm::vec3 normal;


	public:
		PlaneCollider();
		~PlaneCollider();		
		
		virtual void Update(double delta) {};

	
	};
}