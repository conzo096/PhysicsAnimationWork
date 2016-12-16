#include "ModelInfo.h"

phys::info::info(std::vector<glm::vec3> pos)
{
	// Find corners. 
	// Use corners to calulate normals. 
	// Use corners to work out texture range?
	// Assume default colour cba with passing old value in.
	// Box texture coordinates
	glm::vec2 box_texcoords[4] = { glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f) };

	glm::vec3 box_normals[6] = { glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) };

	positions = pos;

	for (int i = 0; i < pos.size(); i++)
	{
		colours.push_back(glm::vec4(1.0f, 1.0f,0,1.0f));
		normals.push_back(box_normals[i/6]);
	}
	// NEED TO FIX NORMALS.
	for (int i = 0; i < pos.size(); i += 3)
	{
		phys::Plane tri = phys::Plane(pos[i], pos[i + 1], pos[i + 2]);
		normals.push_back(tri.GetNormal());

	}
	for (unsigned int i = 0; i < 4; ++i)
		tex_coords.push_back(box_texcoords[i] * glm::vec2(1));
	// Right
	for (unsigned int i = 0; i < 4; ++i)
		tex_coords.push_back(box_texcoords[i] * glm::vec2(1));
	// Back
	for (unsigned int i = 0; i < 4; ++i)
		tex_coords.push_back(box_texcoords[i] * glm::vec2(1));
	// Left
	for (unsigned int i = 0; i < 4; ++i)
		tex_coords.push_back(box_texcoords[i] * glm::vec2(1));
	// Top
	for (unsigned int i = 0; i < 4; ++i)
		tex_coords.push_back(box_texcoords[i] * glm::vec2(1));
	// Bottom
	for (unsigned int i = 0; i < 4; ++i)
		tex_coords.push_back(box_texcoords[i] * glm::vec2(1));


	// Calcluate centre point of object.

	for (int i = 0; i < positions.size(); i++)
		midPoint += positions[i];
	midPoint /= positions.size();

}

