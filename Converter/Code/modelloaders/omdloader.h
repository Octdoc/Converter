#pragma once

#include "modelloader.h"

namespace gfx
{
	struct OMDHeader
	{
		char fileFormat;	//'t' for text, 'b' for binary
		char extension[3];	//"OMD"
		UINT modelType;
		UINT vertexCount;
		UINT indexCount;
		UINT groupCount;
		UINT materialCount;
		UINT boundingVolumePrimitive;
		UINT hitboxTriangleCount;
		UINT boneCount;
		UINT animationCount;
	};

	class OMDLoader :public ModelLoader
	{
	private:
		void ReadHeaderBinary(std::ifstream& infile, OMDHeader& header, UINT modelType);
		void ReadVerticesBinary(std::ifstream& infile, OMDHeader& header);
		void ReadIndicesBinary(std::ifstream& infile, OMDHeader& header);
		void ReadGroupsBinary(std::ifstream& infile, OMDHeader& header);
		void ReadMaterialsBinary(std::ifstream& infile, OMDHeader& header);
		void ReadHitboxBinary(std::ifstream& infile, OMDHeader& header);
		void ReadBonesBinary(std::ifstream& infile, OMDHeader& header);
		void ReadAnimationsBinary(std::ifstream& infile, OMDHeader& header);

		void ReadHeaderText(std::wifstream& infile, OMDHeader& header, UINT modelType);
		void ReadVerticesText(std::wifstream& infile, OMDHeader& header);
		void ReadIndicesText(std::wifstream& infile, OMDHeader& header);
		void ReadGroupsText(std::wifstream& infile, OMDHeader& header);
		void ReadMaterialsText(std::wifstream& infile, OMDHeader& header);
		void ReadHitboxText(std::wifstream& infile, OMDHeader& header);
		void ReadBonesText(std::wifstream& infile, OMDHeader& header);
		void ReadAnimationsText(std::wifstream& infile, OMDHeader& header);

	public:
		void LoadOMD(LPCWSTR filename, UINT modelType);

		void LoadOMDText(LPCWSTR filename, UINT modelType);
		void LoadOMDBinary(LPCWSTR filename, UINT modelType);
	};
}