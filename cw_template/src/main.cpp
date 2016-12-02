/*
		CURRENT ISSUE. 
		Need to be able to change camera and mvp matrix. Might have to rewrite that segment as well.
*/


#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <graphics_framework.h>
#include <phys_utils.h>
#include "Model.h"
#include "ModelInfo.h"
#include "Game.h"


using namespace std;
using namespace graphics_framework;
using namespace glm;
using namespace phys;
#define physics_tick 1.0 / 60.0

//static vector<unique_ptr<Model>> sceneList;
static vector<Model> sceneList;
effect eff;
target_camera cam;
glm::mat4 PV;
directional_light light;
material mat;

info LoadCube(const glm::vec3 &dims)
{
	info cube;

	glm::vec3 box_position[] =
	{
		glm::vec3(-0.5f, 0.5f, -0.5f),  // a -0
		glm::vec3(0.5f, 0.5f, -0.5f),   // b - 1
		glm::vec3(-0.5f, -0.5f, -0.5f), // c - 2
		glm::vec3(0.5f, -0.5f, -0.5f),  // d - 3

		glm::vec3(-0.5f, 0.5f, 0.5f),  // g - 4
		glm::vec3(0.5f, 0.5f, 0.5f),   // h - 5
		glm::vec3(-0.5f, -0.5f, 0.5f), // e - 6
		glm::vec3(0.5f, -0.5f, 0.5f),  // f - 7
	};
	int box_indices[] =
	{
		// front ABC
		0, 1, 2,
		// BDC
		1, 3, 2,
		// bottom CDE
		2, 3, 6,
		// DFE
		3, 7, 6,
		// back EFG
		6, 7, 4,
		// FHG
		7, 5, 4,
		// top GHA
		4, 5, 0,
		// HBA
		5, 1, 0,
		// left GAE
		4, 0, 6,
		// ACE
		0, 2, 6,
		// rightBHD
		1, 5, 3,
		// HFD
		5, 7, 3
	};

	// Normals for the box geometry
	glm::vec3 box_normals[6] = { glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) };

	// Box texture coordinates
	glm::vec2 box_texcoords[4] = { glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f) };

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> tex_coords;
	std::vector<glm::vec4> colours;

	// Iterate through each position and add to buffer
	for (unsigned int i = 0; i < 36; i++)
	{
		// Calculate position
		glm::vec3 pos = box_position[box_indices[i]] * dims;
		// Add the position data.  Multiply by dimension
		positions.push_back(pos);
		// Normal is one of the six defined.  Divide index by 4 to get the value
		normals.push_back(box_normals[i / 4]);
		// Set the colour to be light grey
		colours.push_back(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
	}
	// Texture coordinates based on side
	// Front
	for (unsigned int i = 0; i < 4; ++i)
		tex_coords.push_back(box_texcoords[i] * glm::vec2(dims));
	// Right
	for (unsigned int i = 0; i < 4; ++i)
		tex_coords.push_back(box_texcoords[i] * glm::vec2(dims.z, dims.y));
	// Back
	for (unsigned int i = 0; i < 4; ++i)
		tex_coords.push_back(box_texcoords[i] * glm::vec2(dims));
	// Left
	for (unsigned int i = 0; i < 4; ++i)
		tex_coords.push_back(box_texcoords[i] * glm::vec2(dims.z, dims.y));
	// Top
	for (unsigned int i = 0; i < 4; ++i)
		tex_coords.push_back(box_texcoords[i] * glm::vec2(dims.x, dims.z));
	// Bottom
	for (unsigned int i = 0; i < 4; ++i)
		tex_coords.push_back(box_texcoords[i] * glm::vec2(dims.x, dims.z));

	cube.positions = positions;
	cube.normals = normals;
	cube.colours = colours;
	cube.tex_coords = tex_coords;

	return cube;
}

bool load_content()
{
	phys::Init();
	/*unique_ptr<Model> test(new Model());
	test->SetModelInfo(LoadCube(glm::dvec3(1, 1, 1)));
	test->SetBoundingBox(BoundingBox(test->GetModelInfo().positions));
	test->SetSphereCollider(SphereCollider(test->GetModelInfo().positions));
	test->UpdateBuffers();
	sceneList.push_back(move(test));
	
	unique_ptr<Model> test1(new Model());
	test1->SetBoundingBox(BoundingBox(test1->GetModelInfo().positions));
	test1->SetModelInfo(LoadCube(glm::dvec3(1, 1, 1)));
	test1->SetSphereCollider(SphereCollider(test1->GetModelInfo().positions));	
	test1->GetRigidBody().translate(glm::vec3(0, 2, 0));
	test1->UpdateBuffers();
	sceneList.push_back(move(test1));*/

	Model test;
	test.SetModelInfo(LoadCube(glm::dvec3(1, 1, 1)));
	test.SetBoundingBox(BoundingBox(test.GetModelInfo().positions));
	test.SetSphereCollider(SphereCollider(test.GetModelInfo().positions));
	//test.GetRigidBody().GetPosition() = glm::dvec3(0,2,0);
	test.UpdateBuffers();
	sceneList.push_back(test);

	Model test1;
	test1.SetBoundingBox(BoundingBox(test1.GetModelInfo().positions));
	test1.SetModelInfo(LoadCube(glm::dvec3(1, 1, 1)));
	test1.SetSphereCollider(SphereCollider(test1.GetModelInfo().positions));
	test1.GetRigidBody().translate(glm::vec3(0, 0, 0));
	test1.UpdateBuffers();
	sceneList.push_back(test1); 

	eff = effect();
	eff.add_shader("shaders/phys_phong.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/phys_phong.frag", GL_FRAGMENT_SHADER);
	eff.build();
	cam.set_position(vec3(10.0f, 10.0f, 10.0f));
	cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
	light.set_ambient_intensity(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	light.set_direction(vec3(0.0f, 1.0f, 0.0f));
	mat = material(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), 25.0f);
	return true;
}


bool update(float delta_time)
{
	
	static double t = 0.0;
	static double accumulator = 0.0;
	accumulator += delta_time;

//	for (auto &e : SceneList)
//	{
//		e->Update(delta_time);
//	}


	static float rot = 0.0f;
	rot += 0.2f * delta_time;
	phys::SetCameraPos(rotate(vec3(15.0f, 12.0f, 15.0f), rot, vec3(0, 1.0f, 0)));
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W))
		//sceneList[0]->GetRigidBody().translate(glm::vec3(0.0f, 1, 0.0f)*delta_time);
		sceneList[0].GetRigidBody().translate(glm::vec3(0.0f, 1, 0.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S))
	//	sceneList[0]->GetRigidBody().translate(glm::vec3(0.0f, -1, 0.0f)*delta_time);
		sceneList[0].GetRigidBody().translate(glm::vec3(0.0f, -1, 0.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A))
		sceneList[0].GetRigidBody().translate(glm::vec3(1.0f, 0, 0.0f)*delta_time);
		//sceneList[0]->GetRigidBody().translate(glm::vec3(1.0f, 0, 0.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D))
		sceneList[0].GetRigidBody().translate(glm::vec3(-1.0f, 0, 0.0f)*delta_time);

		//sceneList[0]->GetRigidBody().translate(glm::vec3(-1.0f, 0, 0.0f)*delta_time);
	

	
	PV = cam.get_projection() * cam.get_view();
	cam.update(static_cast<float>(delta_time));
	renderer::setClearColour(0, 0, 0);
	

	while (accumulator > physics_tick)
	{
		UpdatePhysics(sceneList,t, physics_tick);
		accumulator -= physics_tick;
		t += physics_tick;
	}

	for (auto &e : sceneList)
	{
		e.Update(delta_time);
	}
	std::cout << to_string(sceneList[0].GetRigidBody().GetPosition()) << std::endl;
	phys::Update(delta_time);
	return true;
}

bool render()
{
	renderer::clear();
	renderer::bind(eff);
	for (auto &e : sceneList)
	{
		RGBAInt32 col = GREY;
		mat.set_diffuse(col.tovec4());
		renderer::bind(mat, "mat");
		renderer::bind(light, "light");
		mat4 M = e.GetRigidBody().get_transform_matrix();
		mat3 N = e.GetRigidBody().get_normal_matrix();
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(PV * M));
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));
		e.Render();
	}
	return true;
}

void main()
{
	// Create application
	app application;
	// Set load content, update and render methods
	application.set_load_content(load_content);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}