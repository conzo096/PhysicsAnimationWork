#pragma once
#include <vector>
#include "Model.h"
#include "RayCast.h"
#include <glm/glm.hpp>
#include <algorithm>
#include "stdafx.h"
#include "Plane.h"

namespace phys
{
		std::vector<Model> SliceModel(Model m,Plane p);

}
