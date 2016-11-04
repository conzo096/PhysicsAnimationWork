#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
struct info
{
	// Information for model.
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> tex_coords;
	std::vector<glm::vec4> colours;
	std::vector<int> indices;
};

struct transformInfo
{
	// Current position of model.
	glm::vec3 mPosition;
	glm::quat mRotation;
	// Transformation matrix.
	glm::mat4 mTransform;

};