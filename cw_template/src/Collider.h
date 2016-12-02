#pragma once
#include <glm/glm.hpp>
#include "stdafx.h"
namespace phys
{
	class Collider
	{
	private:
		float radius;

	public:
		Collider();
		~Collider();		
		
		virtual void Update(double delta) {};

		float GetRadius() { return radius; }
		void SetRadius(float r) { radius = r; }
	};
}