#include "Model.h"
#include "stdafx.h"
#define GLM_ENABLE_EXPERIMENTAL
namespace phys
{
	Model::Model()
	{

	}


	Model::~Model()
	{
	}

	void Model::Update(float deltaTime, const double dt)
	{
		box.Update(transformation);
		// Center is wherever object is.
		sphere.SetCenter(transformation.GetPosition());

		// calcualte velocity from current and previous position
		glm::dvec3 velocity = transformation.mPosition - transformation.prev_pos;
		// set previous position to current position
		transformation.prev_pos = transformation.mPosition;
		//transformation.mposition += v + a * (dt ^ 2);

		if (transformation.mPosition.y > -6.0f)
			transformation.mPosition += velocity + glm::dvec3(0, -10.0, 0) * pow(dt, 2);

		if (transformation.mPosition.y <= -6.0f)
		{
			if (transformation.mPosition.y = 0-6)
				transformation.mPosition += glm::dvec3(0,0,0) + glm::dvec3(0, 10.0, 0) * pow(dt, 2);

			transformation.prev_pos = transformation.mPosition + ((transformation.mPosition - transformation.prev_pos)/1.2);
			// If on floor velocity counters gravity.
		}

		if (transformation.mPosition.y >= 25.0f)
		{
			transformation.prev_pos = transformation.mPosition + ((transformation.mPosition - transformation.prev_pos)/1.2);
		}
	}


	void Model::UpdateBuffers()
	{
		// Add the buffers to the geometry
		add_buffer(information.positions, BUFFER_INDEXES::POSITION_BUFFER);
		add_buffer(information.colours, BUFFER_INDEXES::COLOUR_BUFFER);
		if (information.normals.size() != 0)
			add_buffer(information.normals, BUFFER_INDEXES::NORMAL_BUFFER);
		if (information.tex_coords.size() != 0)
			add_buffer(information.tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
		if (information.indices.size() != 0)
			add_index_buffer(information.indices);
	}

	void Model::Render()
	{
		#ifdef _DEBUG
	/*	for (int i = 0; i < information.positions.size(); i++)
		{
			glm::vec3 point = information.positions[i];
			std::cout << glm::to_string(point) << std::endl;
		}
		std::cout << "Bounding Box" << std::endl;
		box.PrintCorners();*/
		#endif

		glBindVertexArray(render._vao);
		// If there is an index buffer then use to render
		if (render._index_buffer != 0)
		{
			// Bind index buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render._index_buffer);
			// Draw elements
			glDrawElements(GL_TRIANGLES, render._indices, GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			// Draw arrays
			glDrawArrays(GL_TRIANGLES, 0, render._vertices);
		}
	}



	// Adds a buffer to the geometry object
	bool Model::add_buffer(const std::vector<glm::vec2> &buffer, GLuint index, GLenum buffer_type) {
		// Check that index is viable
		assert(index < 16);
		// Check that buffer is not empty
		assert(buffer.size() > 0);
		// Check if geometry initialised
		if (render._vao == 0)
		{
			// Create the vertex array object
			glGenVertexArrays(1, &render._vao);
		}
		// If we have no vertices yet, set the vertices to the size of this buffer
		if (render._vertices == 0)
			render._vertices = static_cast<GLuint>(buffer.size());
		// Otherwise ensure that the number of vertices matches
		else if (render._vertices != buffer.size())
		{
			std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
			std::cerr << "Buffer does not contain correct amount of vertices" << std::endl;
			return false;
		}
		// Now add buffer to the vertex array object.  Bind the vertex array object
		glBindVertexArray(render._vao);
		// Generate buffer with OpenGL
		GLuint id;
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		// Set the buffer data
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(glm::vec2), &buffer[0], buffer_type);
		// Set the vertex pointer and enable
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(index);
		// Check for OpenGL error
		if (CHECK_GL_ERROR) {
			std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
			std::cerr << "Could not create buffer with OpenGL" << std::endl;
			return false;
		}
		// Add buffer to map
		render._buffers[index] = id;
		return true;
	}

	// Adds a buffer to the geometry object
	bool Model::add_buffer(const std::vector<glm::vec3> &buffer, GLuint index, GLenum buffer_type) {
		// Check that index is viable
		assert(index < 16);
		// Check that buffer is not empty
		assert(buffer.size() > 0);
		// Check if geometry initialised
		if (render._vao == 0) {
			// Create the vertex array object
			glGenVertexArrays(1, &render._vao);
			// Check for any OpenGL error
			if (CHECK_GL_ERROR) {
				// Display error
				std::cerr << "ERROR - creating geometry" << std::endl;
				std::cerr << "Could not generate vertex array object" << std::endl;
				// Set vertex array object to 0
				render._vao = 0;
				// Throw exception
				throw std::runtime_error("Error creating vertex array object with OpenGL");
			}
		}
		// If we have no vertices yet, set the vertices to the size of this buffer
		if (render._vertices == 0)
			render._vertices = static_cast<GLuint>(buffer.size());
		// Otherwise ensure that the number of vertices matches
		else if (render._vertices != buffer.size()) {
			std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
			std::cerr << "Buffer does not contain correct amount of vertices" << std::endl;
			return false;
		}
		// Now add buffer to the vertex array object.  Bind the vertex array object
		glBindVertexArray(render._vao);
		// Generate buffer with OpenGL
		GLuint id;
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		// Set the buffer data
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(glm::vec3), &buffer[0], buffer_type);
		// Set the vertex pointer and enable
		glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(index);
		// Check for OpenGL error
		if (CHECK_GL_ERROR) {
			std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
			std::cerr << "Could not create buffer with OpenGL" << std::endl;
			return false;
		}
		// Add buffer to map
		render._buffers[index] = id;
		return true;
	}

	// Adds a buffer to the geometry object
	bool Model::add_buffer(const std::vector<glm::vec4> &buffer, GLuint index, GLenum buffer_type) {
		// Check that index is viable
		assert(index < 16);
		// Check that buffer is not empty
		assert(buffer.size() > 0);
		// Check if geometry initialised
		if (render._vao == 0) {
			// Create the vertex array object
			glGenVertexArrays(1, &render._vao);
			// Check for any OpenGL error
			if (CHECK_GL_ERROR) {
				// Display error
				std::cerr << "ERROR - creating geometry" << std::endl;
				std::cerr << "Could not generate vertex array object" << std::endl;
				// Set vertex array object to 0
				render._vao = 0;
				// Throw exception
				throw std::runtime_error("Error creating vertex array object with OpenGL");
			}
		}
		// If we have no vertices yet, set the vertices to the size of this buffer
		if (render._vertices == 0)
			render._vertices = static_cast<GLuint>(buffer.size());
		// Otherwise ensure that the number of vertices matches
		else if (render._vertices != buffer.size()) {
			std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
			std::cerr << "Buffer does not contain correct amount of vertices" << std::endl;
			return false;
		}
		// Now add buffer to the vertex array object.  Bind the vertex array object
		glBindVertexArray(render._vao);
		// Generate buffer with OpenGL
		GLuint id;
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		// Set the buffer data
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(glm::vec4), &buffer[0], buffer_type);
		// Set the vertex pointer and enable
		glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(index);
		// Check for OpenGL error
		if (CHECK_GL_ERROR) {
			std::cerr << "ERROR - adding buffer to geometry object" << std::endl;
			std::cerr << "Could not create buffer with OpenGL" << std::endl;
			return false;
		}
		// Add buffer to map
		render._buffers[index] = id;
		return true;
	}

	// Adds an index buffer to the geometry
	bool Model::add_index_buffer(const std::vector<GLuint> &buffer) {
		// Check that buffer is not empty
		assert(buffer.size() > 0);
		// Check if vertex array object is valid
		assert(render._vao != 0);
		// Set indices to buffer size
		render._indices = static_cast<GLuint>(buffer.size());
		// Bind vertex array object
		glBindVertexArray(render._vao);
		// Add buffer
		glGenBuffers(1, &render._index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render._index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(GLuint), &buffer[0], GL_STATIC_DRAW);
		// Check for error
		if (CHECK_GL_ERROR) {
			std::cerr << "Error - adding index buffer to geometry object" << std::endl;
			std::cerr << "Could not create buffer with OpenGL" << std::endl;
			return false;
		}
		return true;
	}

}