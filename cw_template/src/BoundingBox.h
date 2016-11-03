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


		// Corners using min x.
		glm::vec3 GetFrontBottomLeft() { return glm::vec3(botX, botY, botZ); };
		glm::vec3 GetFrontTopLeft() { return glm::vec3(botX, topY, botZ); };

		glm::vec3 GetBackBottomLeft() { return glm::vec3(botX, botY, topZ); };
		glm::vec3 GetBackTopLeft() { return glm::vec3(botX, topY, topZ); };

		// Corners using max x.

		glm::vec3 GetFrontBottomRight() { return glm::vec3(topX, botY, botZ); };
		glm::vec3 GetFrontTopRight() { return glm::vec3(topX, botY, topZ); };

		glm::vec3 GetBackBottomRight() { return glm::vec3(topX, botY, topZ); };
		glm::vec3 GetBackTopRight() { return glm::vec3(topX, topY, topZ); };


		void SetPosition(glm::vec3 pos) { position = pos; };
	};
}
