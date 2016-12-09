#pragma once
#include "stdafx.h"
#include <glm/glm.hpp>
#include <vector>
#include <glm/ext.hpp>
#include <iostream>
#include "ModelInfo.h"
#include "SphereCollider.h"
#include "Collider.h"
namespace phys
{
	class BoundingBox : public Collider
	{
	private:
		float topX = 0, topY = 0, topZ = 0;
		float botX = 0, botY = 0, botZ = 0;

		float volume;
		glm::dquat rotation;

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
		void Update(double delta) {};

		// Updates according to model.
		void Update(RigidBody& model) { rotation = model.GetQuat(); SetPosition(model.GetPosition()); };
	};
}
