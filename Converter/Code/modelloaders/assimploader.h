#pragma once

#include "modelloader.h"

struct aiScene;

namespace gfx
{
	class AssimpLoader :public ModelLoader
	{
	private:
		void StoreData(const aiScene* scene, UINT modelType);
		void StoreMaterials(const aiScene* scene, UINT modelType);
		void StoreVertices(const aiScene* scene, UINT modelType);

	public:
		void LoadAssimp(LPCWSTR filename, UINT modelType);
	};
}