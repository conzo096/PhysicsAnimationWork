#pragma once
#include <vector>
#include "Model.h"
#include "RayCast.h"
#include <glm/glm.hpp>
#include <algorithm>
#include "stdafx.h"
#include "Plane.h"
#include "Triangle.h"
namespace phys
{
		info SliceModel(Model& m,Plane p);

}
