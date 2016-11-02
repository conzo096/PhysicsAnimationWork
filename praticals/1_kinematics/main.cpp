#include "ik.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <graphics_framework.h>
#include <phys_utils.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

// Simulation constants
int numLinks = 5;
float linkLength = 2.0f; // Length of each link
std::vector<Link> links; // link chain that finds new solution.
bool foundSolution = false; // true when the chain finds a solution.
bool moveBall = false; // true when the ball is allowed to be moved.
int order = 0; // flips between 0-1 for the 
float lerpTimer = 0.0f; // Timer for lerp.
std::array <std::vector<Link>,2> pastLinks; // Flips between last contact (link found solution) and current link chain.
std::vector<Link> lerpLink; // Lerp result of pastLinks. Used for render.
vec3 target = vec3(6.0f, 4.0f, 0); // Target position of the ball.

void MoveTarget()
{
  target = glm::ballRand((static_cast<float>(numLinks) * linkLength) * 0.6f);
  target.y = abs(target.y);
   target.x = abs(target.x);
  cout << "boop" << endl;
}

void UpdateHierarchy()
{

	for (int i = 0; i < (int)links.size(); ++i)
	{
		// Calculate rotation matrix.
		mat4 R1 = mat4_cast(angleAxis(links[i].m_angle, links[i].m_axis));
		// Calculate Translation matrix.
		mat4 T1 = translate(mat4(1.0f), vec3(linkLength, 0, 0));
		// Set the start of link.
		links[i].m_base = mat4(1.0) * R1;
		// Set the end of link.
		links[i]. m_end = links[i].m_base * T1;
		// Translate axis into world space.
		links[i].m_worldaxis = links[i].m_axis;
		// If it is not the root node.
		if (i > 0)
		{
			// Don't move the root link.

			// Base of link is end of previous link.
			links[i].m_base = links[i - 1].m_end * links[i].m_base;
			// Calculate end of link.
			links[i].m_end = links[i].m_base * links[i].m_end;
			// Rotation axis in world space. Considers the previous link rotation.
			links[i].m_worldaxis = normalize(mat3(links[i - 1].m_end) * links[i].m_axis);
		}
	}
}

void UpdateLerp()
{
	for (int i = 0; i < (int)lerpLink.size(); ++i)
	{
		lerpLink[i].m_angle = lerp(pastLinks[1 - order][i].m_angle, pastLinks[order][i].m_angle, lerpTimer);
		lerpLink[i].m_axis = lerp(pastLinks[1 - order][i].m_axis, pastLinks[order][i].m_axis, lerpTimer);
		lerpLink[i].m_worldaxis = normalize(lerp(pastLinks[1 - order][i].m_worldaxis, pastLinks[order][i].m_worldaxis, lerpTimer));

		mat4 R1 = mat4_cast(angleAxis(lerpLink[i].m_angle, lerpLink[i].m_axis));
		mat4 T1 = translate(mat4(1.0f), vec3(linkLength, 0, 0));
		lerpLink[i].m_base = mat4(1.0) * R1;
		lerpLink[i].m_end = lerpLink[i].m_base * T1;
		lerpLink[i].m_worldaxis = lerpLink[i].m_axis;
		if (i > 0)
		{
			// Don't move the root link.
			lerpLink[i].m_base = lerpLink[i - 1].m_end * lerpLink[i].m_base;
			lerpLink[i].m_end = lerpLink[i].m_base * lerpLink[i].m_end;
			lerpLink[i].m_worldaxis = normalize(mat3(lerpLink[i - 1].m_end) * lerpLink[i].m_axis);
		}
	}
}

bool load_content()
{
  phys::Init();
  for (int i = 0; i < numLinks; ++i)
  {
    int ax = (i + 0) % 3 == 0;
    int ay = (i + 1) % 3 == 0;
    int az = (i + 2) % 3 == 0;
    links.push_back(Link(vec3(ax, ay, az), 0.0 + (float)i * 0.0f));
	pastLinks[0].push_back(Link(vec3(ax, ay, az), 0.0 + (float)i * 0.0f));
	pastLinks[1].push_back(Link(vec3(ax, ay, az), 0.0 + (float)i * 0.0f));
	lerpLink.push_back(Link(vec3(ax, ay, az), 0.0 + (float)i * 0.0f));

    cout << vec3(ax, ay, az) << endl;
  }
  UpdateHierarchy();
  return true;
}

void UpdateIK()
{
	UpdateHierarchy();
	// distance of current link chain to target.
	const float distance = length(vec3(links[links.size() - 1].m_end[3]) - target);
 
	// Check if link is in reach.
	if (distance < 0.5f)
	{
		// switch order of links.
		order = 1 - order;
		// Set current link to past link.
		pastLinks[order] = links;
		foundSolution = true;
		moveBall = true;
	} 
	if (moveBall == true)
	{
		MoveTarget();
		moveBall = false;
	}
		// no solution found so do not update.
	//if (foundSolution == false)
	{
		FabrikUpdate(target, links, linkLength);
		//ik_1dof_Update(target, links, linkLength);
		// ik_3dof_Update(target, links, linkLength);
	}
}

void RenderIK()
{
  phys::DrawSphere(target, 0.2f, RED);
  
  for (int i = 0; i < (int)links.size(); ++i)
  {
    vec3 base = links[i].m_base[3];
    vec3 end = links[i].m_end[3];
    phys::DrawCube(links[i].m_base * glm::scale(mat4(1.0f), vec3(0.5f)), GREEN);
    phys::DrawCube(links[i].m_end * glm::scale(mat4(1.0f), vec3(0.5f)), ORANGE);
    phys::DrawLine(base, end);
    phys::DrawPlane(base, links[i].m_worldaxis, vec3(0.01f));
  }


  //if(foundSolution == true)
  //{
	 // for (int i = 0; i < (int)lerpLink.size(); ++i)
	 // {
		//  vec3 base = lerpLink[i].m_base[3];
		//  vec3 end = lerpLink[i].m_end[3];
		//  phys::DrawCube(lerpLink[i].m_base * glm::scale(mat4(1.0f), vec3(0.5f)), GREEN);
		//  phys::DrawCube(lerpLink[i].m_end * glm::scale(mat4(1.0f), vec3(0.5f)), ORANGE);
		//  phys::DrawLine(base, end);
		//  phys::DrawPlane(base, lerpLink[i].m_worldaxis, vec3(0.01f));
	 // }
  //}
  //else
  //{
	 //  for (int i = 0; i < (int)pastLinks[order].size(); ++i)
	 // {
		//  vec3 base = pastLinks[order][i].m_base[3];
		//  vec3 end = pastLinks[order][i].m_end[3];
		//  phys::DrawCube(pastLinks[order][i].m_base * glm::scale(mat4(1.0f), vec3(0.5f)), GREEN);
		//  phys::DrawCube(pastLinks[order][i].m_end * glm::scale(mat4(1.0f), vec3(0.5f)), ORANGE);
		//  phys::DrawLine(base, end);
		//  phys::DrawPlane(base, pastLinks[order][i].m_worldaxis, vec3(0.01f));
	 // }
  //}

}

bool update(float delta_time)
{
  static float rot = 0.0f;
  rot += 0.2f * delta_time;
  phys::SetCameraPos(rotate(vec3(15.0f, 12.0f, 15.0f), rot, vec3(0, 1.0f, 0)));
  
  if (foundSolution == true)
  {
	  lerpTimer += delta_time;
	  lerpTimer = clamp(lerpTimer, 0.0f, 1.0f);
	  UpdateLerp();
  }
  else
	  lerpTimer = 0.0f;
  if (lerpTimer == 1.0f)
  {
	  foundSolution = false;
	  moveBall = false;
  }
  UpdateIK();
  phys::Update(delta_time);
  return true;
}

bool render() {
  RenderIK();
 // phys::DrawScene();
  return true;
}

void main() {
  // Create application
  app application;
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}