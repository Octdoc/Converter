#pragma once

#include "modelloader.h"

namespace gfx
{
	class PMXLoader :public ModelLoader
	{
	private:
		void PMXLoadVertexData(std::ifstream& file, int boneIndexSize, int extradata);
		void PMXLoadIndexData(std::ifstream& file, int indexSize);
		void PMXLoadTextureNames(std::ifstream& file, int textByteCount);
		void PMXLoadMaterials(std::ifstream& file, int textByteCount, int texIndexSize);
		void PMXLoadBones(std::ifstream& file, int textByteCount, int indexSize);

	public:
		void LoadPMX(LPCWSTR filename, UINT modelType);
	};
}