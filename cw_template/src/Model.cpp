#include "Model.h"


namespace phys
{
	Model::Model()
	{
	}


	Model::~Model()
	{
	}

	void Model::Update(double deltaTime)
	{
		box.Update(transformation);
	}

	void Model::Render()
	{
		#ifdef _DEBUG
		for (int i = 0; i < information.positions.size(); i++)
		{
			glm::vec3 point = information.positions[i];
			std::cout << glm::to_string(point) << std::endl;
		}
		std::cout << "Bounding Box" << std::endl;
		box.PrintCorners();
		#endif
	}

}