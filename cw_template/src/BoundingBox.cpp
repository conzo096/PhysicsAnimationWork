#include "BoundingBox.h"

namespace phys
{

	BoundingBox::BoundingBox()
	{

	}

	BoundingBox::BoundingBox(std::vector<glm::vec3> points)
	{
	//	glm::vec3 maxPoint, minPoint;
	//	for (auto &v : points)
	//	{
	//		minPoint = glm::min(minPoint, v);
	//		maxPoint = glm::max(maxPoint, v);
	//	}
		
	///*
		//THIS IS WRONG FIX IT.
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

	//*/
			// Square and square root to ensure it is a positive number?
			float length = topX - botX;
			float width = topY - botY;
			float height = topZ - botZ;

			CalculateVolume(length,width,height);

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
		if (pos.x > botX | pos.x < topX) return false;
		if (pos.y > botY | pos.y < topY) return false;
		return true;
	}

	bool BoundingBox::TestOBBOBB(BoundingBox b)
	{
		float ra, rb;
		glm::mat3 r, absR;

		// C

		// No detection false.
		return false;
	}

	void BoundingBox::PrintCorners()
	{
		std::cout << "Front bottom left: ";
		std::cout << glm::to_string(GetFrontBottomLeft()) << std::endl;;
		std::cout << "Front bottom right: ";
		std::cout << glm::to_string(GetFrontBottomRight()) << std::endl;;
		std::cout << "Front top right: ";
		std::cout << glm::to_string(GetFrontTopRight()) << std::endl;;
		std::cout << "Front top left: ";
		std::cout << glm::to_string(GetFrontTopLeft()) << std::endl;;

		std::cout << "Back top left: ";
		std::cout << glm::to_string(GetBackTopLeft()) << std::endl;;
		std::cout << "Back top right: ";
		std::cout << glm::to_string(GetBackTopRight()) << std::endl;;
		std::cout << "Back bottom right: ";
		std::cout << glm::to_string(GetBackBottomRight()) << std::endl;;
		std::cout << "Back bottom left: ";
		std::cout << glm::to_string(GetBackBottomLeft()) << std::endl;;


	}


	BoundingBox::~BoundingBox()
	{
	}
}