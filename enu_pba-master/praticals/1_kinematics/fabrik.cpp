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


void FabrikUpdate(const vec3 &const target, std::vector<Link> &const links, const float linkLength)
{
	// Set numLinks.
	numLinks = links.size();


	// Get distance between root link and target.
	float dist = length(vec3(links[0].m_end[3]) - target);
	float totDist =0;
	// Check if target is within reach.
	for (int i = 0; i < numLinks-1; i++)
	{
		// The distance between the links.
		totDist += linkLength;
	}

	if (dist > totDist)
	{
		for (int i = 0; i < numLinks-1; i++)
		{
			float r = length(target - vec3(links[i].m_end[3]));
			float diff = linkLength / r;
			// Find the new joint position.
			vec3 newPosition = (1 - diff)*links[i].m_end[3] + vec4((diff*target),0);
		//	links[i+1].m_end[3] = vec4(newPosition,0);
			links[i + 1].m_angle = angle(newPosition, vec3(links[i + 1].m_end[3]));
			links[i + 1].m_axis = newPosition;
		}
	}
	else
	{
		for (int i = 0; i < numLinks; i++)
		{
			vec3 b = vec3(links[i].m_end[3]);
			// Check dist between end effector and target is greater than tolerance.
		//	float diffA = length(vec3(links[numLinks - 1].m_end[3] - vec4(target,0)));
			float diffA = length(vec3(links[numLinks - 1].m_axis - target));
			while (diffA > 0.5f)
			{
				// Stage 1: Forward reaching.
				// Set end effector as target.
			//	links[numLinks - 1].m_end[3] = vec4(target,0);
				links[numLinks - 1].m_angle = angle(vec3(links[numLinks-1].m_end[3]), target);
				links[numLinks - 1].m_axis = target;
				for (int i = numLinks-2 ; i >= 1; i--)
				{
					// Find distance between new position and joint position.
					float r = length(vec3(links[i+1].m_end[3] - links[i].m_end[3]));
					float diff = linkLength / r;
				//	links[i].m_end[3] = (1 - diff)*links[i + 1].m_end[3] + (diff*links[i].m_end[3]);
					links[i].m_axis = (1 - diff)*links[i + 1].m_end[3] + (diff*links[i].m_end[3]);
				}
				// Stage 2: Backward reaching.
				// Set the root initial position.
				//links[0].m_end[3] = vec4(b,0);
				links[0].m_axis = b;
				for (int i = 0; i < numLinks - 1; i++)
				{
					// Distance between links and target.
					float x = length(vec3(vec4(target,0) - links[i].m_end[3]));
					
					// Find the distance between new joint position and the next joint in the chain.
					float dist = length(vec3(links[i + 1].m_end[3] - links[i].m_end[3]));
					float diff = linkLength / dist;
					// Find new joint position.
				//	links[i + 1].m_end[3] = (1 - diff)*links[i].m_end[3] + (diff*links[i + 1].m_end[3]);
					links[i + 1].m_axis = (1 - diff)*links[i].m_end[3] + (diff*links[i + 1].m_end[3]);
				}
				//diffA = length(vec3(links[numLinks - 1].m_end[3] - vec4(target,0)));
				UpdateHierarchy();
				diffA = length(vec3(links[numLinks - 1].m_axis - target));
			}

		}
	}
	
	return;
}