// This class represents the models that are interacting within the scene. 
#pragma once
#include "stdafx.h"
#include "BoundingBox.h"
#include "SphereCollider.h"
#include "ModelInfo.h"
#include <graphics_framework.h>
#include <vector>
#include "Plane.h"
#include <glm/glm.hpp>
#include "Sampler.h"
using namespace graphics_framework;
namespace phys
{
	class Model
	{
	private:
		// Bounding volumes.
		BoundingBox box;
		SphereCollider sphere;
		// info - holds all the visual data required to render a model. vertices, normals, texture coordinates, vertex colours.
		info information;
		// Physics based rigidbody.
		RigidBody rigidBody;
		// Holds the OpenGl buffers needed to render a model.
		renderInfo render;

		// Planes in which the model can split on.
		std::vector<Plane> splittingPlanes = std::vector<Plane>();

	public:
		Model();
		Model(std::vector<glm::vec3> pos);
		Model(info i);
		~Model();

		// Creates the buffers. Needed AFTER it is created, will fail if created in a constructor.
		void CreateBuffers();

		// Adds a buffer of vec2 data to the geometry object
		bool add_buffer(const std::vector<glm::vec2> &buffer, GLuint index, GLenum buffer_type = GL_DYNAMIC_DRAW);
		// Adds a buffer of vec3 data to the geometry object
		bool add_buffer(const std::vector<glm::vec3> &buffer, GLuint index, GLenum buffer_type = GL_DYNAMIC_DRAW);
		// Adds a buffer of vec4 data to the geometry object
		bool add_buffer(const std::vector<glm::vec4> &buffer, GLuint index, GLenum buffer_type = GL_DYNAMIC_DRAW);
		// Adds an index buffer to the geometry object
		bool add_index_buffer(const std::vector<GLuint> &buffer);
		// Updates the bounding volume positions.
		void Update(float deltaTime);
		// Renders the model.
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
