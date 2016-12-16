// Ray casting class. Was first used to try and split meshes. Currently it is not used in any classes or methods.
#pragma once
#include <glm/glm.hpp>
#include "Model.h"
#include "stdafx.h"
namespace phys
{
	class RayCast
	{
	private:
		glm::vec3 direction;
		glm::vec3 origin;
		float range = 150000;

	public:
		RayCast();
		RayCast(glm::vec3 startPoint, glm::vec3 endPoint);
		~RayCast();

		bool DetectCollision(glm::vec3 point);
		bool TestBoundingBoxHit(BoundingBox bb);
		
		
		
		// Set and gets.

		glm::vec3 GetDirection() { return direction; };
		glm::vec3 GetOrigin() { return origin; };
		float GetRange() { return range; };

		void SetDirection(glm::vec3 dir) { glm::normalize(direction = dir); };
		void SetOrigin(glm::vec3 ori) { origin = ori; };
		void SetRange(float r) { range = r; };
		void SetRandomOrigin(glm::vec3 minBounds, glm::vec3 maxBounds);

	};
}