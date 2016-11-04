#pragma once
#include "..\\src\\BoundingBox.h"
#include "ModelInfo.h"
#include <graphics_framework.h>
#include <glm/ext.hpp>
using namespace graphics_framework;
namespace phys
{
	class Model
	{
	private:

		//Transform transform;
	//	geometry geom;
		BoundingBox box;
		info information;
		transformInfo transformation;


		//std::vector<std::unique_ptr<Component>> components_;
	public:
		Model();
		~Model();


		// Set and Gets.
		BoundingBox GetBoundingBox() { return box; };
	//	geometry GetGeometry() { return geom; };
		info GetModelInfo() { return information; }
		void SetBoundingBox(BoundingBox bb) { box = bb; };
	//	void SetGeometry(geometry g) { geom = g; };
		void SetModelInfo(info i) { information = i; }

		void Update(double deltaTime);
		void Render();
		
	};
}
