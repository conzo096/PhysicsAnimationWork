// Holds some of the structures needed for the central model class. 
#pragma once
#include "stdafx.h"
#include "RigidBody.h"
#include "Plane.h"
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

		info(std::vector <glm::vec3> pos);
		glm::vec3 GetMidPoint() { return midPoint; };

	private:
		glm::vec3 midPoint;
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
		renderInfo() throw(...) : _vao(0), _index_buffer(0), _vertices(0), _indices(0) {}
	};

		//class Collider;
		const struct CollisionInfo
		{
			RigidBody *c1;
			RigidBody *c2;
			glm::dvec3 position;
			glm::dvec3 normal;
			double depth;
		};
	};
