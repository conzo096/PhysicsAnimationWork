// This is the orientated bounding box. It inherits from the base collider class.
#pragma once
#include <glm/glm.hpp>
#include "Collider.h"
namespace phys
{
	class BoundingBox : public Collider
	{
	private:
		// Top corner values of cube.
		float topX = 0, topY = 0, topZ = 0;
		// Bottom corner values of cube.
		float botX = 0, botY = 0, botZ = 0;
		// Rotation of the cube. This is the same as the rigid body it is attached too.
		glm::dquat rotation;
	public:
		BoundingBox();
		BoundingBox(std::vector<glm::vec3> points);
		~BoundingBox();

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
