#include "ik.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <phys_utils.h>
#include <vector>
using namespace std;
using namespace glm;
static unsigned int numLinks = 0;



static void CalculateRotation(int i, const vec3 &target, std::vector<Link> &const links)
{
	// our current orientation
	dquat qCur = angleAxis(links[i].m_angle, links[i].m_axis);
	// current position of this effector
	vec3 vlinkBasePos = (links[i].m_base)[3];
	vec3 vLinkEndPos = (links[i].m_end)[3];
	// current position of the effector at the end of the chain
	vec3 vEndEffPos = links[links.size() - 1].m_end[3];
	// convert our axis to world space
	vec3 vLinkAxis = links[i].m_worldaxis;
	// project target onto axis plane
	vec3 vplanetarget = projectOntoPlane(target, vLinkAxis, vlinkBasePos);
	// project vEndEffPos onto axis plane
	vec3 vplaneEndEff = projectOntoPlane(vEndEffPos, vLinkAxis, vlinkBasePos);

	// These are the two vectors we want to converge.
	vec3 vLinkBaseToEndEffDirection = normalize(vplanetarget - vlinkBasePos);
	vec3 vLinkBaseToTargetDirection = normalize(vplaneEndEff - vlinkBasePos);

	// Get Dot of the two vectors
	float cosAngle = dot(vLinkBaseToTargetDirection, vLinkBaseToEndEffDirection);
	if (abs(cosAngle) < 1.0f) {
		// *********************************
		// Get the Axis perpendicular to the two vectors
		vec3 vPerp = cross(vLinkBaseToEndEffDirection, vLinkBaseToTargetDirection);
		// Get the Angle between the two vectors
		float vAngle = angle(vLinkBaseToEndEffDirection, vLinkBaseToTargetDirection);
		// Turn into a Quat
		dquat vQuat = normalize(angleAxis(vAngle, vPerp));
		// Multply our current Quat with it
		qCur *= vQuat;
		qCur = normalize(qCur);
		// Pull out the angle and axis components, set the link params
		links[i].m_axis = axis(qCur);
		links[i].m_angle = angle(qCur);
		// *********************************
		UpdateHierarchy();
	}
}
void FabrikUpdate(const vec3 &const target, std::vector<Link> &const links, const float linkLength)
{
	// Number of tries.
	int tries =0;
	// Set numLinks
	numLinks = links.size();
	// Get distance between root and target.
	float distance = length(vec3(links[0].m_end[3]) - target);
	// Get length of entire chain.
	float chainLen = numLinks*linkLength;
	// Check if target is within reach.
	if (distance > chainLen)
	{
		for (int i = 0; i < numLinks - 1; i++)
		{

			vec3 currentPosition = (links[i].m_end)[3];
			float lambda = linkLength / length(target - currentPosition);
			vec3 pos = normalize((1 - lambda) * currentPosition + lambda * target);
	//		CalculateRotation(i, pos, links);
		}
	}
	else // Target within reach
	{

		vec3 rootInitial = links[0].m_end[3];
		float targetDelta = length(vec3(links[numLinks - 1].m_end[3])-target);

		while (targetDelta > 0.5f && tries <20)
		{
			// Forward reaching phase

			// Set position of last link to target.
			links[numLinks - 1].m_axis = target;

			for (int i = numLinks - 2; i > 0; i--)
			{
				// Calculate angle between this link and link above it.
				float lambda = linkLength / length(vec3(links[i + 1].m_end[3] - links[i].m_end[3]));
				// Calculate new position of link.
				vec3 pos = (1 - lambda) * links[i + 1].m_end[3] + lambda * links[i].m_end[3];
	//			CalculateRotation(i, pos, links);

			}
			// Backward reaching phase
			links[0].m_axis = rootInitial;
			for (int i = 0; i < numLinks - 1; i++)
			{
				// Calculate angle.
				float lambda = linkLength / length(vec3(links[i + 1].m_end[3] - links[i].m_end[3]));
				// Calculate new position.
				vec3 pos = (1 - lambda) * links[i].m_end[3] + lambda * links[i + 1].m_end[3];
	//			CalculateRotation(i, pos, links);

			}
			targetDelta = length(target-vec3(links[linkLength - 1].m_base[3]));
			tries++;
		}
	}
}
