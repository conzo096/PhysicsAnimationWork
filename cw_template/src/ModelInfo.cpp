#include "ModelInfo.h"

void phys::RigidBody::AddForceAt(const glm::dvec3 & force, const glm::dvec3 & point)
{
	forces += force;
	torques += cross(point - position, force);
}

void phys::RigidBody::AddAngularForce(const glm::dvec3 & i)
{
	torques += i;
}

void phys::RigidBody::AddLinearImpulse(const glm::dvec3& v)
{
	const glm::dvec3 dv = position - prev_pos;
	const glm::dvec3 newdv = dv + v;
	prev_pos = position - newdv;
}

void phys::RigidBody::ComputeLocalInvInertiaTensor()
{
	 //localInvInertia = glm::inverse(glm::dmat3(1.0));
	//worldInvInertia = mat4_cast(orientation) * glm::dmat4(localInvInertia) * glm::transpose(mat4_cast(orientation));
	if (inverseMass == 0)
	{
		localInvInertia = glm::dmat3(0.0);
		worldInvInertia = localInvInertia;
	}

	// hard coded need to add radius to this?
	const double x2 = 4.0 * ( 5*5);
	const double ix = (x2 + x2) / (inverseMass * 12.0);

	glm::dmat3 localInvInertia = glm::dmat3(0.0);
	localInvInertia[0][0] = 1.0 / ix;
	localInvInertia[1][1] = 1.0 / ix;
	localInvInertia[2][2] = 1.0 / ix;
	worldInvInertia = mat4_cast(orientation) * glm::dmat4(localInvInertia) * glm::transpose(mat4_cast(orientation));
}

void phys::RigidBody::Integrate(const double dt)
{
	// calcualte velocity from current and previous position
	glm::dvec3 velocity = position - prev_pos;
	// set previous position to current position
	prev_pos = position;
	// position += v + a * (dt^2)
	position += velocity + (forces + GRAVITY * inverseMass) * pow(dt, 2);
	//position += velocity + (forces + GRAVITY) * pow(dt, 2);
	
	
	// Remove once floor plane in.
	if (position.y <= -6.0f)
	{
		prev_pos = position + (glm::dvec3(velocity) / 1.2);
		position += glm::dvec3(0, 6.0, 0) * pow(dt, 2);
	}
	
	
	forces = glm::dvec3(0);

	angVelocity += worldInvInertia * torques * dt;
	angVelocity *= glm::min(pow(angularDamping, dt), 1.0);

	orientation += glm::dquat(angVelocity * dt) * orientation;
	orientation = normalize(orientation);

	torques = glm::dvec3(0);
	
}

#pragma once
#include "Plane.h"

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
		colours.push_back(glm::vec4(1.0f, 0,0,1.0f));
		normals.push_back(box_normals[i/4]);
	}
	// NEED TO FIX NORMALS.
	for (int i = 0; i < pos.size(); i += 3)
	{
		phys::Plane tri = phys::Plane(pos[i], pos[i + 1], pos[i + 2]);
		normals.push_back(tri.GetNormal());
		normals.push_back(tri.GetNormal());
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


}
