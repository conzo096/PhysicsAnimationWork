#include "RigidBody.h"

namespace phys
{
	
	static glm::dvec3 GRAVITY = glm::dvec3(0, -10, 0);
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
		localInvInertia = glm::inverse(glm::dmat3(1.0));
		worldInvInertia = mat4_cast(orientation) * glm::dmat4(localInvInertia) * glm::transpose(mat4_cast(orientation));
		if (inverseMass == 0)
		{
			localInvInertia = glm::dmat3(0.0);
			worldInvInertia = localInvInertia;
		}

		// hard coded need to add radius to this?
		const double x2 = 4.0 * (5 * 5);
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

		forces = glm::dvec3(0);

		angVelocity += worldInvInertia * torques * dt;
		angVelocity *= glm::min(pow(angularDamping, dt), 1.0);

		orientation += glm::dquat(angVelocity * dt) * orientation;
		orientation = normalize(orientation);

		torques = glm::dvec3(0);

	}

}