#pragma once
#include <glm/glm.hpp>
#include <vector>
namespace phys
{
	class BoundingBox
	{
	private:
		float topX, topY, topZ;
		float botX, botY, botZ;

		float volume;

		glm::vec3 position;

		// USE TRANSFORM?
	public:
		BoundingBox();
		BoundingBox(std::vector<glm::vec3> points);
		~BoundingBox();

		//void  CalculateVolume();
		void  CalculateVolume(float length, float width, float height);

		//bool TestCollision(BoundingBox bb);
		bool TestMouseCollision(glm::vec2 pos);

		// Set and gets.
		float GetVolume() { return volume; };
		glm::vec3 GetPosition() { return position; };

		void SetPosition(glm::vec3 pos) { position = pos; };
	};
}
