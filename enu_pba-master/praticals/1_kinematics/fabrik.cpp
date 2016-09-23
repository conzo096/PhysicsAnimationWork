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



static void SetLastLinkToTarget(int i, const vec3 &target, std::vector<Link> &const links)
{
	// our current orientation
	dquat qCur = angleAxis(links[i].m_angle, links[i].m_axis);
	// current position of this effector
	vec3 vlinkBasePos = (links[i].m_base)[3];
	// current position of the effector at the end of the chain
	vec3 vEndEffPos = links[links.size() - 1].m_end[3];
	// convert our axis to world space
	vec3 vLinkAxis = links[i].m_worldaxis;
	// project target onto axis plane
	vec3 vplanetarget = projectOntoPlane(target, vLinkAxis, vlinkBasePos);
	// project vEndEffPos onto axis plane
	vec3 vplaneEndEff = projectOntoPlane(vEndEffPos, vLinkAxis, vlinkBasePos);

	// These are the two vectors we want to converge.
	vec3 vLinkBaseToEndEffDirection = normalize(vplaneEndEff - vlinkBasePos);
	vec3 vLinkBaseToTargetDirection = normalize(vplanetarget - vlinkBasePos);

	// Get Dot of the two vectors
	float cosAngle = dot(vLinkBaseToTargetDirection, vLinkBaseToEndEffDirection);
	if (abs(cosAngle) < 1.0f)
	{
		// *********************************
		// Get the Angle between the two vectors
		float vAngle = angle(vLinkBaseToTargetDirection, vLinkBaseToEndEffDirection);

		// Turn into a Quat with our axis
		dquat vQuat = angleAxis(vAngle, vLinkAxis);

		// Multply our current Quat with it
		qCur *= vQuat;
		qCur = normalize(qCur);
		// Pull out the angle component, set the link params
		links[i].m_angle = angle(qCur);

		// *********************************
	}
}

static void Reach(int i, const vec3 &target, std::vector<Link> &const links)
{
	// our current orientation
	dquat qCur = angleAxis(links[i].m_angle, links[i].m_axis);
	// current position of this effector
	vec3 vlinkBasePos = (links[i].m_base)[3];
	// end effector of next link in chain. 
	vec3 vEndEffPos = links[i+1].m_end[3];
	// convert our axis to world space
	vec3 vLinkAxis = links[i].m_worldaxis;
	// project target onto axis plane
	vec3 vplanetarget = projectOntoPlane(target, vLinkAxis, vlinkBasePos);
	// project vEndEffPos onto axis plane
	vec3 vplaneEndEff = projectOntoPlane(vEndEffPos, vLinkAxis, vlinkBasePos);

	// These are the two vectors we want to converge.
	vec3 vLinkBaseToEndEffDirection = normalize(vplaneEndEff - vlinkBasePos);
	vec3 vLinkBaseToTargetDirection = normalize(vplanetarget - vlinkBasePos);

	// Get Dot of the two vectors
	float cosAngle = dot(vLinkBaseToTargetDirection, vLinkBaseToEndEffDirection);
	if (abs(cosAngle) < 1.0f)
	{
		// *********************************
		// Get the Angle between the two vectors
		float vAngle = angle(vLinkBaseToTargetDirection, vLinkBaseToEndEffDirection);

		// Turn into a Quat with our axis
		dquat vQuat = angleAxis(vAngle, vLinkAxis);

		// Multply our current Quat with it
		qCur *= vQuat;
		qCur = normalize(qCur);
		// Pull out the angle component, set the link params
		links[i].m_angle = angle(qCur);

		// *********************************
	}
}

void FabrikUpdate(const vec3 &const target, std::vector<Link> &const links, const float linkLength)
{
	// Number of tries. Used to prevent stuck beng in while loop.
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
			// Length of link/ Distance between joint and target.
			float lambda = linkLength / length(target - currentPosition);
			// New world position of where the link should be?.
			vec3 pos = (1 - lambda) * currentPosition + (lambda * target);
			//UpdateHierarchy();
			Reach(i+1, pos, links);
		}
	}
	else // Target within reach
	{
		// Get the initial root position.
		vec3 rootInitial = links[0].m_end[3];
		
		// Find distance between last effector and target. 
		float targetDelta = length(vec3(links[numLinks - 1].m_end[3])-target);

		while (targetDelta > 0.5f && tries <20)
		{
			// Forward reaching phase

			// Set position of last link to target.
			UpdateHierarchy();
			SetLastLinkToTarget(numLinks - 1, target, links);

			// For rest of links.
			for (int i = numLinks - 1; i >= 1; i--)
			{
				// Calculate lambda between this link and link above it.
				float lambda = linkLength / (length(vec3(links[i].m_end[3] - links[i-1].m_end[3])));
				// Calculate new target of link.
				vec3 pos = (1 - lambda) * links[i].m_end[3] + lambda * links[i-1].m_end[3];
				UpdateHierarchy();
				Reach(i-1, pos, links);
			}
			// Backward reaching phase
			UpdateHierarchy();
			Reach(0, rootInitial, links);
			for (int i = 0; i < numLinks - 2; i++)
			{
				// Calculate angle.
				float lambda = linkLength / (length(vec3(links[i + 1].m_end[3] - links[i].m_end[3])));
				// Calculate new position.
				vec3 pos = (1 - lambda) * links[i].m_end[3] + lambda * links[i + 1].m_end[3];
				UpdateHierarchy();
				Reach(i+1, pos, links);

			}
			targetDelta = length(target-vec3(links[numLinks - 1].m_end[3]));
			tries++;
		}
		UpdateHierarchy();
		return;
	}
}
