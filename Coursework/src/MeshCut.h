// This file is dedicated to any algorithms which slice a mesh.
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Plane.h"
#include "Triangle.h"
#include "Model.h"
namespace phys
{
		// Basic slice a mesh into two via a plane. Updates the model that is split and adds the fragment to the fragment list.
		void SliceModel(Model& m, Plane p, std::vector<Model>& newFragments);

}
