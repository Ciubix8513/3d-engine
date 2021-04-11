#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_

#include "Scene.h"
#include <fstream>

namespace Engine
{
	class FileManager
	{
	public:
		static MeshComponent::Mesh* LoadMesh(std::string FileName);

	};
};


#endif
