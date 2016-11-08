#pragma once
#include "stdafx.h"
 
namespace phys
{
	struct info
	{
		// Information for model.
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> tex_coords;
		std::vector<glm::vec4> colours;
		std::vector<GLuint> indices;
		info() {}
	};

	struct renderInfo
	{
		// The OpenGL ID of the vertex array object
		GLuint _vao;
		// The OpenGL IDs of the buffers used within the vertex array object
		std::map<GLuint, GLuint> _buffers;
		// The OpenGL ID of the index buffer
		GLuint _index_buffer;
		// The number of vertices in the geometry
		GLuint _vertices;
		// The number of indices in the index buffer
		GLuint _indices;
		renderInfo() {}
	};

	struct transformInfo
	{
		// Current position of model.
		glm::vec3 mPosition;
		glm::vec3 mScale;
		glm::quat mRotation;
		
		// Creates a transform object
		transformInfo() : mScale(glm::vec3(1.0f, 1.0f, 1.0f)) { }

		// Translates the 3D object
		void translate(const glm::vec3 &translation) { mPosition += translation; }

		// Rotates the 3D object using Euler angles
		void rotate(const glm::vec3 &rotation) {
			glm::quat rot(rotation);
			rotate(rot);
		}

		// Rotate the 3D object using the given quaternion
		void rotate(const glm::quat &q) {
			mRotation = mRotation * q;
			mRotation = glm::normalize(mRotation);
		}

		// Gets the transformation matrix representing the defined transform
		glm::mat4 get_transform_matrix() {
			auto T = glm::translate(glm::mat4(1.0f), mPosition);
			auto S = glm::scale(glm::mat4(1.0f),mScale);
			auto R = glm::mat4_cast(mRotation);
			auto matrix = T * R * S;
			return matrix;
		}

		// Gets the normal matrix representing the defined transform
		glm::mat3 get_normal_matrix() { return glm::mat3_cast(mRotation); }
	};
};