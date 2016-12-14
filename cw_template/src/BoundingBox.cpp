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
		}

			// Square and square root to ensure it is a positive number?
			float length = topX - botX;
			float width = topY - botY;
			float height = topZ - botZ;

			CalculateVolume(length,width,height);
			//SetRadius(glm::length(glm::dvec3(GetFrontTopLeft()) - glm::dvec3(length/2,width/2,height/2)));
	}


	void BoundingBox::CalculateVolume(float length, float width, float height)
	{
		volume = length * width *height;
	}

	bool BoundingBox::TestMouseCollision(glm::vec2 pos)
	{
		 // Need to add transfrom.
		glm::dvec2 topXY = glm::dvec2(topX, topY);
		glm::dvec2 botXY = glm::dvec2(botX, botY);
		glm::dvec2 topXBotY = glm::dvec2(topX, botY);
		glm::dvec2 botXTopY = glm::dvec2(botX, topY);

		// Check if it is in range.
		if (pos.x > botX || pos.x < topX) return false;
		if (pos.y > botY || pos.y < topY) return false;
		return true;
	}

	BoundingBox::~BoundingBox()
	{
	}
}