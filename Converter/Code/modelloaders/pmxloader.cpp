#include "pmxloader.h"

namespace gfx
{
	void ReadPMXText(std::istream& src, std::wstring& dst, int byteCount)
	{
		int length;
		wchar_t ch = 0;
		src.read((char*)(&length), 4);
		length /= byteCount;
		for (int i = 0; i < length; i++)
		{
			src.read((char*)(&ch), byteCount);
			dst += ch;
		}
	}

	struct PMXHeader
	{
		char signature[4];
		float version;
		char globalCount;
		/*
		0	Text encoding	0, 1	Byte encoding for the "text" type, 0 = UTF16LE, 1 = UTF8
		1	Additional vec4 count	0..4	Additional vec4 values are added to each vertex
		2	Vertex index size	1, 2, 4	The index type for vertices (See Index Types above)
		3	Texture index size	1, 2, 4	The index type for textures (See Index Types above)
		4	Material index size	1, 2, 4	The index type for materials (See Index Types above)
		5	Bone index size	1, 2, 4	The index type for bones (See Index Types above)
		6	Morph index size	1, 2, 4	The index type for morphs (See Index Types above)
		7	Rigidbody index size	1, 2, 4	The index type for rigid bodies (See Index Types above)
		*/
		char globals[8];
		std::wstring localName;
		std::wstring universalName;
		std::wstring localComments;
		std::wstring universalComments;

		PMXHeader() :
			signature(),
			version(0.0f),
			globalCount(0),
			globals() {}

		void Read(std::istream& src)
		{
			src.read(signature, 4);
			src.read((char*)(&version), 4);
			src.read(&globalCount, 1);
			src.read(globals, globalCount);
			ReadPMXText(src, localName, getTextByteCount());
			ReadPMXText(src, universalName, getTextByteCount());
			ReadPMXText(src, localComments, getTextByteCount());
			ReadPMXText(src, universalComments, getTextByteCount());
		}

		inline int getTextByteCount() { return 2 - globals[0]; }
	};

	struct PMXMaterial
	{
		std::wstring localName;
		std::wstring universalName;
		mth::float4 diffuseColor;
		mth::float3 specularColor;
		float specularStrength;
		mth::float3 ambientColor;
		char drawingFlags;
		mth::float4 edgeColor;
		float edgeScale;
		int textureIndex;
		int environmentIndex;
		char environmentBlendMode;
		char toonReference;
		int toonValue;
		std::wstring metaData;
		int surfaceCount;

		PMXMaterial() :
			specularStrength(0.0f),
			drawingFlags(0),
			edgeScale(0.0f),
			textureIndex(0),
			environmentIndex(0),
			environmentBlendMode(0),
			toonReference(0),
			toonValue(0),
			surfaceCount(0) {}

		void Read(std::istream& src, int textByteCount, int texIndexSize)
		{
			ReadPMXText(src, localName, textByteCount);
			ReadPMXText(src, universalName, textByteCount);
			src.read((char*)& diffuseColor, 16);
			src.read((char*)& specularColor, 12);
			src.read((char*)& specularStrength, 4);
			src.read((char*)& ambientColor, 12);
			src.read((char*)& drawingFlags, 1);
			src.read((char*)& edgeColor, 16);
			src.read((char*)& edgeScale, 4);

			switch (texIndexSize)
			{
			case 1:
			{
				char tmp;
				src.read((char*)& tmp, texIndexSize);
				textureIndex = tmp;
				src.read((char*)& tmp, texIndexSize);
				environmentIndex = tmp;
				break;
			}
			case 2:
			{
				short tmp;
				src.read((char*)& tmp, texIndexSize);
				textureIndex = tmp;
				src.read((char*)& tmp, texIndexSize);
				environmentIndex = tmp;
				break;
			}
			case 4:
			{
				int tmp;
				src.read((char*)& tmp, texIndexSize);
				textureIndex = tmp;
				src.read((char*)& tmp, texIndexSize);
				environmentIndex = tmp;
				break;
			}
			}

			src.read((char*)& environmentBlendMode, 1);
			src.read((char*)& toonReference, 1);

			src.read((char*)& toonValue, toonReference == 1 ? 1 : texIndexSize);

			ReadPMXText(src, metaData, textByteCount);
			src.read((char*)& surfaceCount, 4);
		}
	};

	struct PMXBone
	{
		std::wstring localName;
		std::wstring universalName;
		mth::float3 pos;
		int parentIndex;
		int layer;
		char flags[2];
		union TailPos
		{
			mth::float3 f3;
			int i;
		};
		TailPos tailPos;
	};

	void PMXLoader::LoadPMX(LPCWSTR filename, UINT modelType)
	{
		std::ifstream infile;
		infile.open(filename, std::ios::in | std::ios::binary);
		if (infile.good())
		{
			PMXHeader header;
			header.Read(infile);
			PMXLoadVertexData(infile, header.globals[5], header.globals[1]);
			PMXLoadIndexData(infile, header.globals[2]);
			PMXLoadTextureNames(infile, header.getTextByteCount());
			PMXLoadMaterials(infile, header.getTextByteCount(), header.globals[3]);
			PMXLoadBones(infile, header.getTextByteCount(), header.globals[5]);
		}
		else
			throw std::exception(("Could not open file: " + ToStr(filename)).c_str());
		infile.close();
	}

	void PMXLoader::PMXLoadVertexData(std::ifstream& file, int boneIndexSize, int extradata)
	{
		int vertexCount;
		file.read((char*)(&vertexCount), 4);
		struct Vertex
		{
			mth::float3 pos;
			mth::float3 normal;
			mth::float2 uv;
			std::vector<mth::float4> additional;
			char weightDeformType;
			std::vector<char> weightDeform;
			float edgeScale;

			void Read(std::ifstream& src, int boneIndexSize, int extradata)
			{
				src.read((char*)(&pos), sizeof(pos));
				src.read((char*)(&normal), sizeof(normal));
				src.read((char*)(&uv), sizeof(uv));
				if (extradata)
				{
					additional.resize(extradata);
					src.read((char*)additional.data(), sizeof(mth::float4) * extradata);
				}
				src.read((char*)(&weightDeformType), sizeof(weightDeformType));
				int deformSize[] = { boneIndexSize, 2 * boneIndexSize + 4, 4 * boneIndexSize + 16, 2 * boneIndexSize + 4 + 3 * 3 * 4, 4 * boneIndexSize + 16 };
				weightDeform.resize(deformSize[weightDeformType]);
				src.read((char*)(weightDeform.data()), deformSize[weightDeformType]);
				short ind;
				memcpy(&ind, weightDeform.data(), 2);
				src.read((char*)(&edgeScale), sizeof(edgeScale));
			}
		};

		m_modelType = ModelType::PTN;
		m_vertexSizeInBytes = ModelType::VertexSizeInBytes(m_modelType);
		m_vertices.resize(vertexCount * ModelType::VertexSizeInVertexElements(m_modelType));

		int vertexCounter = 0;
		Vertex v;
		for (int i = 0; i < vertexCount; i++)
		{
			v.Read(file, boneIndexSize, extradata);
			m_vertices[vertexCounter++] = v.pos.x;
			m_vertices[vertexCounter++] = v.pos.y;
			m_vertices[vertexCounter++] = v.pos.z;
			m_vertices[vertexCounter++] = v.uv.x;
			m_vertices[vertexCounter++] = v.uv.y;
			m_vertices[vertexCounter++] = v.normal.x;
			m_vertices[vertexCounter++] = v.normal.y;
			m_vertices[vertexCounter++] = v.normal.z;
		}
	}

	void PMXLoader::PMXLoadIndexData(std::ifstream& file, int indexSize)
	{
		int indexCount;
		file.read((char*)(&indexCount), sizeof(indexCount));
		m_indices.resize(indexCount);
		union Index
		{
			int i;
			unsigned short s[2];
			unsigned char ch[4];
		};
		Index index;
		index.i = 0;
		for (int i = 0; i < indexCount; i++)
		{
			file.read((char*)(index.ch), indexSize);
			m_indices[i] = index.i;
		}
	}

	void PMXLoader::PMXLoadTextureNames(std::ifstream& file, int textByteCount)
	{
		int textureCount;
		file.read((char*)& textureCount, 4);
		for (int i = 0; i < textureCount; i++)
		{
			std::wstring str;
			ReadPMXText(file, str, textByteCount);
			m_textureNames.push_back(str);
			m_normalmapNames.push_back(L"");
		}
		m_textureNames.push_back(L"");
		m_normalmapNames.push_back(L"");
	}

	void PMXLoader::PMXLoadMaterials(std::ifstream& file, int textByteCount, int texIndexSize)
	{
		int materialCount;
		file.read((char*)& materialCount, 4);
		int indexCounter = 0;
		for (int i = 0; i < materialCount; i++)
		{
			PMXMaterial mat;
			mat.Read(file, textByteCount, texIndexSize);

			VertexGroup vg;
			vg.startIndex = indexCounter;
			vg.indexCount = mat.surfaceCount;
			vg.materialIndex = mat.textureIndex < 0 ? (int)m_textureNames.size() - 1 : mat.textureIndex;
			m_groups.push_back(vg);
			indexCounter += vg.indexCount;
		}
	}

	void PMXLoader::PMXLoadBones(std::ifstream& file, int textByteCount, int indexSize)
	{
		int boneCount;
		file.read((char*)& boneCount, 4);

	}
}