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
			SetRadius(glm::length(glm::dvec3(GetFrontTopLeft()) - GetPosition()));
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

	bool BoundingBox::TestOBBOBB(BoundingBox b)
	{
		// Store original corners in an array.
		glm::dvec3 aCorners[8] = {GetBackBottomLeft(),GetBackBottomRight(),GetBackTopLeft(),GetBackTopRight(),
								GetFrontBottomLeft(),GetFrontBottomRight(),GetFrontTopLeft(),GetFrontTopRight()};
		glm::dvec3 bCorners[8] = {b.GetBackBottomLeft(),b.GetBackBottomRight(),b.GetBackTopLeft(),b.GetBackTopRight(),
								b.GetFrontBottomLeft(),b.GetFrontBottomRight(),b.GetFrontTopLeft(),b.GetFrontTopRight()};

		for (int i = 0; i < 8; i++)
		{

			// Need to apply rotation to this.
			aCorners[i] = glm::dvec3(aCorners[i] + GetPosition());
			bCorners[i] = glm::dvec3(bCorners[i] + GetPosition());

		}

		float xMin = b.GetPosition().x + b.GetBackBottomLeft().x,
			  xMax = b.GetPosition().x + b.GetBackBottomRight().x,
			  yMin = b.GetPosition().y + b.GetBackBottomLeft().y,
			  yMax = b.GetPosition().y + b.GetBackTopLeft().y,
			  zMin = b.GetPosition().z + b.GetFrontBottomLeft().z,
			  zMax = b.GetPosition().z + b.GetBackBottomLeft().z;
		
		float col = false;

		// For each corner of the box b.
		for (int i = 0; i < 8; i++)
		{
			if (CheckCorner(aCorners[i].x, xMin, xMax))
				if (CheckCorner(aCorners[i].y, yMin, yMax))
					if (CheckCorner(aCorners[i].z, zMin, zMax))
						return true;
		}
				

		//Find effect of b on a.
			xMin = GetPosition().x + GetBackBottomLeft().x,
			xMax = GetPosition().x + GetBackBottomRight().x,
			yMin = GetPosition().y + GetBackBottomLeft().y,
			yMax = GetPosition().y + GetBackTopLeft().y,
			zMin = GetPosition().z + GetFrontBottomLeft().z,
			zMax = GetPosition().z + GetBackBottomLeft().z;

		for (int i = 0; i < 8; i++)
		{
			// Check x axis. 
			if (CheckCorner(bCorners[i].x, xMin, xMax))
				if (CheckCorner(bCorners[i].y, yMin, yMax))
					if (CheckCorner(bCorners[i].z, zMin, zMax))
						return true;
		}

		// Intersects on all 3 axis.
		return false;
	}

		//void BoundingBox::SATtest(const glm::vec3& axis, const glm::vec3 ptSet[], float& minAlong, float& maxAlong)
		bool BoundingBox::CheckCorner(float val, float minBound, float maxBound)
		{
			return minBound <= val && val <= maxBound;
		}
		bool BoundingBox::overlaps(float min1, float max1, float min2, float max2)
		{
			return isBetweenOrdered(min2, min1, max1) || isBetweenOrdered(min1, min2, max2);
		}

		inline bool BoundingBox::isBetweenOrdered(float val, float lowerBound, float upperBound)
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