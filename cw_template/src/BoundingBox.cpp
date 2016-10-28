#include "BoundingBox.h"

namespace phys
{

	BoundingBox::BoundingBox()
	{
	}

	BoundingBox::BoundingBox(std::vector<glm::vec3> points)
	{
		// Find top x,y,z.
		for (glm::vec3 pos : points)
		{
			// top x,y,z
			if (pos.x > topX)
				topX = pos.x;
			if (pos.y > topY)
				topY = pos.y;
			if (pos.z > topZ)
				topZ = pos.z;
			// bot x,y,z
			if (pos.x < botX)
				botX = pos.x;
			if (pos.y < botY)
				botY = pos.y;
			if (pos.z < botZ)
				botZ = pos.z;


			// Square and square root to ensure it is a positive number?
			float length = topX - botX;
			float width = topY - botY;
			float height = topZ - botZ;

			CalculateVolume(length, width, height);
		}

	}


	void BoundingBox::CalculateVolume(float length, float width, float height)
	{
		volume = length * width *height;
	}

	bool BoundingBox::TestMouseCollision(glm::vec2 pos)
	{
		glm::vec2 topXY = glm::vec2(topX, topY);
		glm::vec2 botXY = glm::vec2(botX, botY);
		glm::vec2 topXBotY = glm::vec2(topX, botY);
		glm::vec2 botXTopY = glm::vec2(botX, topY);

		// Check if it is in range.
		if (pos.x > topX | pos.x < botX) return false;
		if (pos.y > topY | pos.y < botY) return false;
		return true;
	}


	BoundingBox::~BoundingBox()
	{
	}
}