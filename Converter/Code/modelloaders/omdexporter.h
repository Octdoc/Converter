#pragma once

#include "omdloader.h"

namespace gfx
{
	class OMDExporter :public ModelLoader
	{
		void WriteHeaderBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteVerticesBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteIndicesBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteGroupsBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteMaterialsBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteHitboxBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteBonesBinary(std::ofstream& outfile, OMDHeader& header);
		void WriteAnimationsBinary(std::ofstream& outfile, OMDHeader& header);

		void WriteHeaderText(std::wofstream& outfile, OMDHeader& header);
		void WriteVerticesText(std::wofstream& outfile, OMDHeader& header);
		void WriteIndicesText(std::wofstream& outfile, OMDHeader& header);
		void WriteGroupsText(std::wofstream& outfile, OMDHeader& header);
		void WriteMaterialsText(std::wofstream& outfile, OMDHeader& header);
		void WriteHitboxText(std::wofstream& outfile, OMDHeader& header);
		void WriteBonesText(std::wofstream& outfile, OMDHeader& header);
		void WriteAnimationsText(std::wofstream& outfile, OMDHeader& header);

	public:
		void ExportOMDBinary(LPCWSTR filename, UINT modelType);
		void ExportOMDText(LPCWSTR filename, UINT modelType);
	};
}