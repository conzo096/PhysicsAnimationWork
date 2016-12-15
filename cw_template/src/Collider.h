// This is the base collider class which any collider created will inherit from.
#pragma once
#include <glm/glm.hpp>
#include "stdafx.h"
#include "RigidBody.h"
namespace phys
{
	class Collider
	{
	private:
		float radius;
		glm::dvec3 position;


	public:
		Collider();
		~Collider();		
		
		virtual void Update(double delta) {};

		float GetRadius() { return radius; }
		void SetRadius(float r) { radius = r; }
		glm::dvec3 GetPosition() { return position; }
		void SetPosition(glm::vec3 p) { position = p; }

	};
}