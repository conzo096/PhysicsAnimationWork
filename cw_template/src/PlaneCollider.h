// Plane collider class, inherits from the collider class. This is used for the floor in the scene.
#pragma once
#include <glm/glm.hpp>
#include "Collider.h"
namespace phys
{
	class PlaneCollider :public Collider
	{
	private:
		const glm::dvec3 normal = glm::dvec3(0,1,0);


	public:
		PlaneCollider();
		PlaneCollider(glm::dvec3 pos) { SetPosition(pos); }
		~PlaneCollider();		
		const glm::dvec3 GetNormal() { return normal; }

		void Update(double delta) {};

	
	};
}