// Simple plane class, is mainly used to split the split meshes up.
#pragma once
#include "stdafx.h"

namespace phys
{
	class Plane
	{
		glm::vec3 point;
		glm::vec3 normal;
		float distance;
		public:
		Plane() {};
		Plane(glm::vec3 a, glm::vec3 b, glm::vec3 c);
		Plane(glm::vec3 pos, glm::vec3 norm) { point = pos; normal = glm::normalize(norm); }
		~Plane() {}

		int SideOfPlane(glm::vec3 p);
		glm::vec3 ClosestPointOnPlane(glm::vec3 p);

		void SetPoint(glm::vec3 p){point = p;}
		void SetNormal(glm::vec3 n){normal = n;}
		void UpdateDistance() { distance = glm::dot(normal, point); }
		glm::vec3 GetPoint(){return point;}
		glm::vec3 GetNormal(){	return normal; }
		float GetDistance() { return distance;}
		
	};
}