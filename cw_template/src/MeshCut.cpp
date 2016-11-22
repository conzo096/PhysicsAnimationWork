#include "MeshCut.h"
namespace phys
{
	MeshCut::MeshCut()
	{
	}
	MeshCut::~MeshCut()
	{
	}

	std::vector<Model> MeshCut::SliceModel(Model m,Plane p)
	{
		// List of new model objects.
		std::vector<Model> models = std::vector<Model>();
		

		// For each triangle.
		for (int i = 0; i < m.GetModelInfo().indices.size(); i++)
		{

		}

		return models;
	}
}