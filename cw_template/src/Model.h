#pragma once
#include "stdafx.h"
#include "BoundingBox.h"
#include "SphereCollider.h"
#include "ModelInfo.h"
#include <graphics_framework.h>
#include <vector>
#include "Plane.h"
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include "Sampler.h"
using namespace graphics_framework;
namespace phys
{
	class Model
	{
	private:

		BoundingBox box;
		SphereCollider sphere;
		info information;
		RigidBody rigidBody;
		renderInfo render;

		// splitting planes.
		std::vector<Plane> splittingPlanes = std::vector<Plane>();

	public:
		Model();
		Model(std::vector<glm::vec3> pos);
		Model(info i);
		~Model();


		// Update buffers.
		void CreateBuffers();

		// Adds a buffer of vec2 data to the geometry object
		bool add_buffer(const std::vector<glm::vec2> &buffer, GLuint index, GLenum buffer_type = GL_DYNAMIC_DRAW);
		// Adds a buffer of vec3 data to the geometry object
		bool add_buffer(const std::vector<glm::vec3> &buffer, GLuint index, GLenum buffer_type = GL_DYNAMIC_DRAW);
		// Adds a buffer of vec4 data to the geometry object
		bool add_buffer(const std::vector<glm::vec4> &buffer, GLuint index, GLenum buffer_type = GL_DYNAMIC_DRAW);
		// Adds an index buffer to the geometry object
		bool add_index_buffer(const std::vector<GLuint> &buffer);

		void Update(float deltaTime);
		void Render();



		// Set and Gets.
		BoundingBox& GetBoundingBox() { return box; }
		SphereCollider GetSphereCollider() { return sphere; }
		info& GetModelInfo() { return information; }
		RigidBody& GetRigidBody() { return rigidBody; }
		renderInfo& GetRender() { return render; }
		std::vector<Plane>& GetSplittingPlanes() { return splittingPlanes; }

		void SetBoundingBox(BoundingBox bb) { box = bb; }
		void SetSphereCollider(SphereCollider sc) { sphere = sc; }
		void SetSplittingPlanes(std::vector<Plane> pl) { splittingPlanes = pl; }
		void SetModelInfo(info i)
		{
			information = i;
			sphere = SphereCollider(information.positions);
			box = BoundingBox(information.positions);
		}
		void SetRigidBody(RigidBody ti) { rigidBody = ti; }
	};
}
