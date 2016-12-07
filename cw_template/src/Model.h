#pragma once
#include "stdafx.h"
#include "BoundingBox.h"
#include "SphereCollider.h"
#include "ModelInfo.h"
#include <graphics_framework.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
using namespace graphics_framework;
namespace phys
{
	class Model
	{
	private:

		BoundingBox box;
		SphereCollider sphere;
		info information;
		RigidBody transformation;
		renderInfo render;
	public:
		Model();
		Model(std::vector<glm::vec3> pos);
		~Model();


		void UpdateBuffers();
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
		RigidBody& GetRigidBody() { return transformation; }

		void SetBoundingBox(BoundingBox bb) { box = bb; }
		void SetSphereCollider(SphereCollider sc) { sphere = sc; }
		void SetModelInfo(info i) { information = i; }
		void SetRigidBody(RigidBody ti) { transformation = ti; }
	};
}
