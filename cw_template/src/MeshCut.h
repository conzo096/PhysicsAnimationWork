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
	class MeshCut
	{

	public:
		MeshCut();
		~MeshCut();


		std::vector<Model> SliceModel(Model m,Plane p);

	};
}
