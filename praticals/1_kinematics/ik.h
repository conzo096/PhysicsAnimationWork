#pragma once
#include <glm/glm.hpp>
#include <vector>

class Link
{
public:
	Link(glm::vec3 &axis, float angle)
	{
		m_axis = axis;
		 m_angle = angle;
	}
	// Note- we could store this as a quaternion
	glm::vec3 m_axis; // local axis
	float m_angle;    // local angle


	// Converts the local axis into world space.
	glm::vec3 m_worldaxis;
	// Start point of the link.
	glm::mat4 m_base;
	// End point of the link.
	glm::mat4 m_end;

};

void FabrikUpdate(const glm::vec3 &const target, std::vector<Link> &const links, const float linkLength);
void ik_1dof_Update(const glm::vec3 &const target, std::vector<Link> &const links, const float linkLength);
void ik_3dof_Update(const glm::vec3 &const target, std::vector<Link> &const links, const float linkLength);
void ik_jacob_Update(const glm::vec3 &const target, std::vector<Link> &const links, const float linkLength);
void UpdateHierarchy();