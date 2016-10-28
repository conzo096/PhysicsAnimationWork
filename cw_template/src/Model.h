#pragma once
#include "..\\src\\BoundingBox.h"
namespace phys
{
	class Model
	{
	private:

		//Transform transform;
		//geometry stuff;
		BoundingBox box;			// WORK. also BRB LUNCH

	public:
		Model();
		~Model();
	};
}
