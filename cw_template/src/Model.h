#pragma once
#include "..\\src\\BoundingBox.h"
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

		//Transform transform;
	//	geometry geom;
		BoundingBox box;
		info information;
		transformInfo transformation;
		renderInfo render;

		//std::vector<std::unique_ptr<Component>> components_;
	public:
		Model();
		~Model();


		void UpdateBuffers();
		// Adds a buffer of vec2 data to the geometry object
		bool add_buffer(const std::vector<glm::vec2> &buffer, GLuint index, GLenum buffer_type = GL_STATIC_DRAW);
		// Adds a buffer of vec3 data to the geometry object
		bool add_buffer(const std::vector<glm::vec3> &buffer, GLuint index, GLenum buffer_type = GL_STATIC_DRAW);
		// Adds a buffer of vec4 data to the geometry object
		bool add_buffer(const std::vector<glm::vec4> &buffer, GLuint index, GLenum buffer_type = GL_STATIC_DRAW);
		// Adds an index buffer to the geometry object
		bool add_index_buffer(const std::vector<GLuint> &buffer);

		void Update(float deltaTime);
		void Render();



		// Set and Gets.
		BoundingBox GetBoundingBox() { return box; };
		info GetModelInfo() { return information; }
		transformInfo GetTransform() { return transformation; }

		void SetBoundingBox(BoundingBox bb) { box = bb; }

		void SetModelInfo(info i) { information = i; }
		void SetTransform(transformInfo ti) { transformation = ti; }
	};
}
