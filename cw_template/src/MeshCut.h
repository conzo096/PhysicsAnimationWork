#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "stdafx.h"
#include "Plane.h"
#include "Triangle.h"
#include "Model.h"
namespace phys
{
		void SliceModel(Model& m, std::vector<Plane>pl, std::vector<Model>& newFragments);

}
