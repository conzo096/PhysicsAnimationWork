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
		// Store original points in an array.
		glm::vec3 aCorners[8] = {GetBackBottomLeft(),GetBackBottomRight(),GetBackTopLeft(),GetBackTopRight(),
								GetFrontBottomLeft(),GetFrontBottomRight(),GetFrontTopLeft(),GetFrontTopRight()};
		glm::vec3 bCorners[8] = {b.GetBackBottomLeft(),b.GetBackBottomRight(),b.GetBackTopLeft(),b.GetBackTopRight(),
								b.GetFrontBottomLeft(),b.GetFrontBottomRight(),b.GetFrontTopLeft(),b.GetFrontTopRight()};
		// Rotate using quanternion.

		for (int i = 0; i < 8; i++)
		{
			aCorners[i] = aCorners[i] * GetTransform().GetQuat();
			bCorners[i] = bCorners[i] * b.GetTransform().GetQuat();

		}
		for (int i = 0; i < 8; i++)
		{
			float shape1Min, shape1Max, shape2Min, shape2Max;
			SATtest(glm::normalize(aCorners[i]), aCorners, shape1Min, shape1Max);
			SATtest(glm::normalize(bCorners[i]), bCorners, shape2Min, shape2Max);
			if (!overlaps(shape1Min, shape1Max, shape2Min, shape2Max))
			{
				return 0; // NO INTERSECTION
			}

		}

		// No detection false.
		return false;
	}

		void SATtest(const glm::vec3& axis, const glm::vec3 ptSet[], float& minAlong, float& maxAlong)
		{
			minAlong = HUGE, maxAlong = -HUGE;
			for (int i = 0; i < 8; i++)
			{
				// just dot it to get the min/max along this axis.
				float dotVal = glm::dot(ptSet[i], axis);
				if (dotVal < minAlong)  minAlong = dotVal;
				if (dotVal > maxAlong)  maxAlong = dotVal;
			}
		}
		bool overlaps(float min1, float max1, float min2, float max2)
		{
			return isBetweenOrdered(min2, min1, max1) || isBetweenOrdered(min1, min2, max2);
		}

		inline bool isBetweenOrdered(float val, float lowerBound, float upperBound)
		{
			return lowerBound <= val && val <= upperBound;
		}

		
	void BoundingBox::PrintCorners()
	{
	/*	std::cout << "Front bottom left: ";
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

		*/
	}


	BoundingBox::~BoundingBox()
	{
	}
}