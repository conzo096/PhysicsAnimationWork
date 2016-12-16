// This class is used to create the splitting planes for the models.
#pragma once
#include <vector>
#include "Plane.h"
#include "BoundingBox.h"
#include <glm/glm.hpp>
#include "stdafx.h"
#include <random>

namespace phys
{
	void CreateSplittingPlanes(BoundingBox bb, int samples, std::vector<Plane> & splittingPlanes);
}
