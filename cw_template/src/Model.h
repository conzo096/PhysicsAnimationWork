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


		// Set and Gets.
		BoundingBox GetBoundingBox() { return box; };
		
		void SetBoundingBox(BoundingBox bb) { box = bb; };
	};
}
