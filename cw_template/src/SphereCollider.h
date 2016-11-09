#pragma once
#include <glm/glm.hpp>
namespace phys
{
	class SphereCollider
	{
	private:
		glm::vec3 center;
		float radius;

	public:
		SphereCollider();
		SphereCollider(std::vector<glm::vec3> points);
		~SphereCollider();		
		
		bool SphereSphereCollision(SphereCollider b);

		// Set and gets.

		glm::vec3 GetCenter() { return center; };
		float GetRadius() { return radius; };

		void SetCenter(glm::vec3 c) { center = c; };
		void SetRadius(float r) { radius = r; };
	};
}