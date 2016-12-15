#pragma once
#include <glm/glm.hpp>
#include "stdafx.h"
namespace phys
{
	struct RigidBody
	{
		// Current position of model.
		glm::dvec3 position;
		glm::dvec3 scale;
		glm::dquat orientation;

		// Needed for physics
		glm::dvec3 prev_pos;
		double mass;
		double inverseMass;
		glm::dvec3 forces;
		glm::dvec3 prevForces;
		glm::dmat3 localInvInertia;
		glm::dmat3 worldInvInertia;
		glm::dvec3 angVelocity;
		glm::dvec3 torques;
		double angularDamping;

		// Creates a transform object
		RigidBody() : scale(glm::vec3(1.0f, 1.0f, 1.0f)), angularDamping(0.05), position(glm::vec3(0, 0, 0)), mass(5), inverseMass(0.2) { }

		// Translates the 3D object
		void translate(const glm::vec3 &translation) { position += translation; }

		// Rotates the 3D object using Euler angles
		void rotate(const glm::vec3 &rotation)
		{
			glm::quat rot(rotation);
			rotate(rot);
		}

		// Rotate the 3D object using the given quaternion
		void rotate(const glm::dquat &q)
		{
			orientation = orientation * q;
			orientation = glm::normalize(orientation);
		}

		// Gets the transformation matrix representing the defined transform
		glm::mat4 get_transform_matrix()
		{
			auto T = glm::translate(glm::dmat4(1.0f), position);
			auto S = glm::scale(glm::dmat4(1.0f), scale);
			auto R = glm::mat4_cast(orientation);
			auto matrix = T * R * S;
			return matrix;
		}

		void SetInitialPosition(glm::vec3 pos)
		{
			position = pos;
			prev_pos = pos;
		}
		// Gets the normal matrix representing the defined transform
		glm::dmat3 get_normal_matrix() { return glm::mat3_cast(orientation); }
		glm::dquat& GetQuat() { return orientation; }
		glm::dvec3& GetPosition() { return position; }

		void SetMass(float m) { mass = m; inverseMass = 1 / mass; }
		float GetMass() { return mass; }

		void AddForceAt(const glm::dvec3 &force, const glm::dvec3 &point);
		void AddAngularForce(const glm::dvec3 &i);
		void AddLinearImpulse(const glm::dvec3 &v);
		virtual void ComputeLocalInvInertiaTensor();
		void Integrate(const double dt);
	};



}