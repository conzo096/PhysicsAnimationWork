#pragma once
#include <vector>
#include "Model.h"
#include "RayCast.h"
#include <glm/glm.hpp>
#include <algorithm>

namespace phys
{
	class Sampler
	{

	public:
		Sampler();
		~Sampler();


		std::vector<glm::vec3> SamplePoints(Model model, int samples);
		std::vector<glm::vec3> SampleShell(Model model);
		std::vector<glm::vec3> SampleInterior(Model model);


	};
}
