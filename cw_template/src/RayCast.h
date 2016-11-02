#pragma once
#include <glm/glm.hpp>
class RayCast
{
private:
	glm::vec3 direction;
	glm::vec3 origin;
	float range;

public:
	RayCast();
	RayCast(glm::vec3 startPoint, glm::vec3 endPoint);
	~RayCast();

	bool DetectCollision(glm::vec3 point);

	// Set and gets.
	
	glm::vec3 GetDirection() { return direction; };
	glm::vec3 GetOrigin() { return origin; };
	float GetRange() { return range; };

	void SetDirection(glm::vec3 dir) { direction = dir; };
	void SetOrigin(glm::vec3 ori) { origin = ori; };
	void SetRange(float r) { range = r; };
};
