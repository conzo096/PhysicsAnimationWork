#pragma once
#include "stdafx.h"
#include <glm/glm.hpp>
#include <vector>
#include <glm/ext.hpp>
#include <iostream>
#include "ModelInfo.h"
#include "SphereCollider.h"
namespace phys
{
	class BoundingBox
	{
	private:
		float topX = 0, topY = 0, topZ = 0;
		float botX = 0, botY = 0, botZ = 0;

		float volume;

		transformInfo transformation;

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
		transformInfo GetTransform() { return transformation; }

		// Corners using min x.
		glm::vec3 GetFrontBottomLeft() { return glm::vec3(botX, botY, botZ); };
		glm::vec3 GetFrontTopLeft() { return glm::vec3(botX, topY, botZ); };

		glm::vec3 GetBackBottomLeft() { return glm::vec3(botX, botY, topZ); };
		glm::vec3 GetBackTopLeft() { return glm::vec3(botX, topY, topZ); };

		// Corners using max x.

		glm::vec3 GetFrontBottomRight() { return glm::vec3(topX, botY, botZ); };
		glm::vec3 GetFrontTopRight() { return glm::vec3(topX, topY, botZ); };

		glm::vec3 GetBackBottomRight() { return glm::vec3(topX, botY, topZ); };
		glm::vec3 GetBackTopRight() { return glm::vec3(topX, topY, topZ); };


		bool TestOBBOBB(BoundingBox b);
		bool TestOBBSphere(SphereCollider s);
		void SATtest(const glm::vec3& axis, const glm::vec3 ptSet[], float& minAlong, float& maxAlong);
		bool overlaps(float min1, float max1, float min2, float max2);
		inline bool isBetweenOrdered(float val, float lowerBound, float upperBound);
		// Updates according to model.
		void Update(transformInfo& model) { transformation = model;};
		void PrintCorners();
	};
}
