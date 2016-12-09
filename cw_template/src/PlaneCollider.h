#pragma once
#include <glm/glm.hpp>
#include "stdafx.h"
namespace phys
{
	class PlaneCollider :public Collider
	{
	private:
		glm::dvec3 normal;


	public:
		PlaneCollider();
		~PlaneCollider();		
		
		void SetNormal(glm::dvec3 n) { normal = n; }
		glm::dvec3 GetNormal() { return normal; }

		virtual void Update(double delta) {};

	
	};
}