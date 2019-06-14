#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "modelloader.h"

#pragma comment (lib, "Code/assimp/lib/assimp.lib")

namespace gfx
{
	void ModelLoader::OrganizeMaterials()
	{
		std::vector<std::wstring> textureNames, normalmapNames;
		for (UINT i = 0; i < (UINT)m_groups.size(); i++)
		{
			textureNames.push_back(m_textureNames[m_groups[i].materialIndex]);
			normalmapNames.push_back(m_normalmapNames[m_groups[i].materialIndex]);
			m_groups[i].materialIndex = i;
		}
		m_textureNames.swap(textureNames);
		m_normalmapNames.swap(normalmapNames);
	}

	void ModelLoader::Create(Vertex_PTMB vertices[], UINT vertexCount, UINT indices[], UINT indexCount, UINT modelType)
	{
		int counter = 0;
		m_modelType = modelType;
		m_vertexSizeInBytes = ModelType::VertexSizeInBytes(m_modelType);
		m_vertices.resize((size_t)(vertexCount * getVertexSizeInFloats()));
		m_indices.resize(indexCount);
		for (UINT v = 0; v < vertexCount; v++)
		{
			if (ModelType::HasPositions(modelType))
			{
				m_vertices[counter++] = vertices[v].position.x;
				m_vertices[counter++] = vertices[v].position.y;
				m_vertices[counter++] = vertices[v].position.z;
			}
			if (ModelType::HasTexcoords(modelType))
			{
				m_vertices[counter++] = vertices[v].texcoord.x;
				m_vertices[counter++] = vertices[v].texcoord.y;
			}
			if (ModelType::HasNormals(modelType))
			{
				m_vertices[counter++] = vertices[v].normal.x;
				m_vertices[counter++] = vertices[v].normal.y;
				m_vertices[counter++] = vertices[v].normal.z;
			}
			if (ModelType::HasTangentsBinormals(modelType))
			{
				m_vertices[counter++] = vertices[v].tangent.x;
				m_vertices[counter++] = vertices[v].tangent.y;
				m_vertices[counter++] = vertices[v].tangent.z;
				m_vertices[counter++] = vertices[v].binormal.x;
				m_vertices[counter++] = vertices[v].binormal.y;
				m_vertices[counter++] = vertices[v].binormal.z;
			}
			if (ModelType::HasBones(modelType))
			{
				m_vertices[counter++] = vertices[v].boneWeights[0];
				m_vertices[counter++] = vertices[v].boneWeights[1];
				m_vertices[counter++] = vertices[v].boneWeights[2];
				m_vertices[counter++] = vertices[v].boneWeights[3];
				m_vertices[counter++] = vertices[v].boneIndex[0];
				m_vertices[counter++] = vertices[v].boneIndex[1];
				m_vertices[counter++] = vertices[v].boneIndex[2];
				m_vertices[counter++] = vertices[v].boneIndex[3];
			}
		}
		for (UINT i = 0; i < indexCount; i++)
			m_indices[i] = indices[i];
		m_groups.push_back({ 0, indexCount, 0 });
		m_textureNames.push_back(L"");
		m_normalmapNames.push_back(L"");
	}

	void ModelLoader::LoadAssimp(LPCWSTR filename, UINT modelType)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(ToStr(filename).c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_Triangulate |
			aiProcess_SortByPType);
		if (scene == NULL)
		{
			//auto error = importer.GetErrorString();
			throw std::exception(("Import failed: " + ToStr(filename)).c_str());
		}
		StoreData(scene, modelType);
	}

#pragma region Load assimp

	void ModelLoader::StoreData(const aiScene* scene, UINT modelType)
	{
		StoreMaterials(scene, modelType);
		StoreVertices(scene, modelType);
	}

	std::wstring FolderlessFilename(LPCSTR filename)
	{
		int lastSlashIndex = -1;
		for (int i = 0; filename[i]; i++)
			if (filename[i] == '\\' || filename[i] == '/')
				lastSlashIndex = i;
		std::wstring str;
		for (int i = lastSlashIndex + 1; filename[i]; i++)
			str += (WCHAR)filename[i];
		return str;
	}
	void ModelLoader::StoreMaterials(const aiScene* scene, UINT modelType)
	{
		m_textureNames.resize(scene->mNumMaterials);
		for (UINT i = 1; i < scene->mNumMaterials; i++)
		{
			if (ModelType::HasTexture(modelType) && scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString path;
				scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				m_textureNames[i] = FolderlessFilename(path.data);
			}
			else
				m_textureNames[i] = L"";
		}

		m_normalmapNames.resize(scene->mNumMaterials);
		for (UINT i = 1; i < scene->mNumMaterials; i++)
		{
			if (ModelType::HasNormalmap(modelType) && scene->mMaterials[i]->GetTextureCount(aiTextureType_NORMALS) > 0)
			{
				aiString path;
				scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &path);
				m_normalmapNames[i] = FolderlessFilename(path.data);
			}
			else
				m_normalmapNames[i] = L"";
		}
	}
	void ModelLoader::StoreVertices(const aiScene* scene, UINT modelType)
	{
		m_groups.resize(scene->mNumMeshes);
		UINT vertexCount = 0;
		UINT indexCount = 0;
		for (UINT m = 0; m < scene->mNumMeshes; m++)
		{
			aiMesh* mesh = scene->mMeshes[m];
			m_modelType |= ModelType::ToModelType(
				mesh->HasPositions(),
				mesh->HasTextureCoords(0),
				mesh->HasNormals(),
				mesh->HasTangentsAndBitangents(),
				mesh->HasBones(),
				scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE) > 0,
				scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_NORMALS) > 0);
			m_groups[m].startIndex = indexCount;
			m_groups[m].materialIndex = mesh->mMaterialIndex;
			vertexCount += scene->mMeshes[m]->mNumVertices;
			for (UINT ii = 0; ii < scene->mMeshes[m]->mNumFaces; ii++)
				indexCount += scene->mMeshes[m]->mFaces[ii].mNumIndices;
			m_groups[m].indexCount = indexCount - m_groups[m].startIndex;
		}
		m_modelType = ModelType::RemoveUnnecessary(m_modelType & modelType);
		m_vertexSizeInBytes = ModelType::VertexSizeInBytes(m_modelType);
		m_vertices.resize((size_t)(vertexCount * m_vertexSizeInBytes / sizeof(VertexElement)));
		m_indices.resize(indexCount);

		vertexCount = 0;
		UINT vertexCounter = 0;
		UINT indexCounter = 0;
		for (UINT m = 0; m < scene->mNumMeshes; m++)
		{
			aiMesh* mesh = scene->mMeshes[m];

			for (UINT v = 0; v < mesh->mNumVertices; v++)
			{
				if (ModelType::HasPositions(m_modelType))
				{
					m_vertices[vertexCounter++] = mesh->mVertices[v].x;
					m_vertices[vertexCounter++] = mesh->mVertices[v].y;
					m_vertices[vertexCounter++] = -mesh->mVertices[v].z;
				}
				if (ModelType::HasTexcoords(m_modelType))
				{
					m_vertices[vertexCounter++] = mesh->mTextureCoords[0][v].x;
					m_vertices[vertexCounter++] = 1.0f - mesh->mTextureCoords[0][v].y;
				}
				if (ModelType::HasNormals(m_modelType))
				{
					m_vertices[vertexCounter++] = mesh->mNormals[v].x;
					m_vertices[vertexCounter++] = mesh->mNormals[v].y;
					m_vertices[vertexCounter++] = -mesh->mNormals[v].z;
				}
				if (ModelType::HasTangentsBinormals(m_modelType))
				{
					m_vertices[vertexCounter++] = mesh->mTangents[v].x;
					m_vertices[vertexCounter++] = mesh->mTangents[v].y;
					m_vertices[vertexCounter++] = -mesh->mTangents[v].z;
					m_vertices[vertexCounter++] = -mesh->mBitangents[v].x;
					m_vertices[vertexCounter++] = -mesh->mBitangents[v].y;
					m_vertices[vertexCounter++] = mesh->mBitangents[v].z;
				}
				if (ModelType::HasBones(m_modelType))
				{
					m_vertices[vertexCounter++] = 0.0f;
					m_vertices[vertexCounter++] = 0.0f;
					m_vertices[vertexCounter++] = 0.0f;
					m_vertices[vertexCounter++] = 0.0f;
					m_vertices[vertexCounter++] = 0u;
					m_vertices[vertexCounter++] = 0u;
					m_vertices[vertexCounter++] = 0u;
					m_vertices[vertexCounter++] = 0u;
				}
			}

			for (UINT f = 0; f < mesh->mNumFaces; f++)
			{
				for (UINT i = 2; i < mesh->mFaces[f].mNumIndices; i++)
				{
					m_indices[indexCounter++] = mesh->mFaces[f].mIndices[0] + vertexCount;
					m_indices[indexCounter++] = mesh->mFaces[f].mIndices[i] + vertexCount;
					m_indices[indexCounter++] = mesh->mFaces[f].mIndices[i - 1] + vertexCount;
				}
			}
			vertexCount += mesh->mNumVertices;
		}
	}

#pragma endregion


#pragma region Load PMX

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

	void ModelLoader::LoadPMX(LPCWSTR filename, UINT modelType)
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

	void ModelLoader::PMXLoadVertexData(std::ifstream& file, int boneIndexSize, int extradata)
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

	void ModelLoader::PMXLoadIndexData(std::ifstream& file, int indexSize)
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

	void ModelLoader::PMXLoadTextureNames(std::ifstream& file, int textByteCount)
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

	void ModelLoader::PMXLoadMaterials(std::ifstream& file, int textByteCount, int texIndexSize)
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

	void ModelLoader::PMXLoadBones(std::ifstream& file, int textByteCount, int indexSize)
	{
		int boneCount;
		file.read((char*)& boneCount, 4);

	}

#pragma endregion

	void ModelLoader::LoadOMD(LPCWSTR filename, UINT modelType)
	{
		std::ifstream infile(filename, std::ios::in | std::ios::binary);
		if (!infile.good())
			throw std::exception(std::string("Failed to load file: " + ToStr(filename)).c_str());
		char filedata[4];
		infile.read(filedata, sizeof(filedata));
		infile.close();
		if ((filedata[1] != 'o' && filedata[1] != 'O') ||
			(filedata[2] != 'm' && filedata[2] != 'M') ||
			(filedata[3] != 'd' && filedata[3] != 'D'))
			throw std::exception(std::string("Corrupted file: " + ToStr(filename)).c_str());
		if (filedata[0] == 't' || filedata[0] == 'T')
			LoadOMDText(filename, modelType);
		else if (filedata[0] == 'b' || filedata[0] == 'B')
			LoadOMDBinary(filename, modelType);
		else
			throw std::exception(std::string("Corrupted file: " + ToStr(filename)).c_str());
	}

#pragma region Load binary

	void ModelLoader::LoadOMDBinary(LPCWSTR filename, UINT modelType)
	{
		std::ifstream infile(filename, std::ios::in | std::ios::binary);
		OMDHeader header;
		ReadHeaderBinary(infile, header, modelType);
		ReadVerticesBinary(infile, header);
		ReadIndicesBinary(infile, header);
		ReadGroupsBinary(infile, header);
		ReadMaterialsBinary(infile, header);
		ReadHitboxBinary(infile, header);
		ReadBonesBinary(infile, header);
		ReadAnimationsBinary(infile, header);
	}
	void ModelLoader::ReadHeaderBinary(std::ifstream& infile, OMDHeader& header, UINT modelType)
	{
		infile.read((char*)& header, sizeof(header));
		m_modelType = ModelType::RemoveUnnecessary(header.modelType & modelType);
		m_vertexSizeInBytes = ModelType::VertexSizeInBytes(m_modelType);
		m_boundingVolumeType = header.boundingVolumePrimitive;
	}
	void ModelLoader::ReadVerticesBinary(std::ifstream& infile, OMDHeader& header)
	{
		m_vertices.resize(header.vertexCount * m_vertexSizeInBytes / sizeof(VertexElement));
		if (ModelType::VertexLayout(m_modelType) == ModelType::VertexLayout(header.modelType))
			infile.read((char*)m_vertices.data(), header.vertexCount * m_vertexSizeInBytes);
		else
		{
			VertexElement tmp[8];
			UINT vertexCounter = 0;
			for (UINT i = 0; i < header.vertexCount; i++)
			{
				if (ModelType::HasPositions(header.modelType))
				{
					infile.read((char*)tmp, 3 * sizeof(VertexElement));
					if (ModelType::HasPositions(m_modelType))
					{
						m_vertices[vertexCounter++] = tmp[0];
						m_vertices[vertexCounter++] = tmp[1];
						m_vertices[vertexCounter++] = tmp[2];
					}
				}
				if (ModelType::HasTexcoords(header.modelType))
				{
					infile.read((char*)tmp, 2 * sizeof(VertexElement));
					if (ModelType::HasTexcoords(m_modelType))
					{
						m_vertices[vertexCounter++] = tmp[0];
						m_vertices[vertexCounter++] = tmp[1];
					}
				}
				if (ModelType::HasNormals(header.modelType))
				{
					infile.read((char*)tmp, 3 * sizeof(VertexElement));
					if (ModelType::HasNormals(m_modelType))
					{
						m_vertices[vertexCounter++] = tmp[0];
						m_vertices[vertexCounter++] = tmp[1];
						m_vertices[vertexCounter++] = tmp[2];
					}
				}
				if (ModelType::HasTangentsBinormals(header.modelType))
				{
					infile.read((char*)tmp, 6 * sizeof(VertexElement));
					if (ModelType::HasTangentsBinormals(m_modelType))
					{
						m_vertices[vertexCounter++] = tmp[0];
						m_vertices[vertexCounter++] = tmp[1];
						m_vertices[vertexCounter++] = tmp[2];
						m_vertices[vertexCounter++] = tmp[3];
						m_vertices[vertexCounter++] = tmp[4];
						m_vertices[vertexCounter++] = tmp[5];
					}
				}
				if (ModelType::HasBones(header.modelType))
				{
					infile.read((char*)tmp, 8 * sizeof(VertexElement));
					if (ModelType::HasBones(m_modelType))
					{
						m_vertices[vertexCounter++] = tmp[0];
						m_vertices[vertexCounter++] = tmp[1];
						m_vertices[vertexCounter++] = tmp[2];
						m_vertices[vertexCounter++] = tmp[3];
						m_vertices[vertexCounter++] = tmp[4];
						m_vertices[vertexCounter++] = tmp[5];
						m_vertices[vertexCounter++] = tmp[6];
						m_vertices[vertexCounter++] = tmp[7];
					}
				}
			}
		}
	}
	void ModelLoader::ReadIndicesBinary(std::ifstream& infile, OMDHeader& header)
	{
		m_indices.resize(header.indexCount);
		infile.read((char*)m_indices.data(), header.indexCount * sizeof(UINT));
	}
	void ModelLoader::ReadGroupsBinary(std::ifstream& infile, OMDHeader& header)
	{
		m_groups.resize(header.groupCount);
		infile.read((char*)m_groups.data(), header.groupCount * sizeof(VertexGroup));
	}
	void ModelLoader::ReadMaterialsBinary(std::ifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		m_textureNames.resize(header.materialCount);
		m_normalmapNames.resize(header.materialCount);
		for (UINT i = 0; i < header.materialCount; i++)
		{
			infile.read((char*)& ch, sizeof(WCHAR));
			while (ch)
			{
				if (ModelType::HasTexture(m_modelType))
					m_textureNames[i] += ch;
				infile.read((char*)& ch, sizeof(WCHAR));
			}
			infile.read((char*)& ch, sizeof(WCHAR));
			while (ch)
			{
				if (ModelType::HasNormalmap(m_modelType))
					m_normalmapNames[i] += ch;
				infile.read((char*)& ch, sizeof(WCHAR));
			}
		}
	}
	void ModelLoader::ReadHitboxBinary(std::ifstream& infile, OMDHeader& header)
	{
		switch (header.boundingVolumePrimitive)
		{
		case mth::BoundingVolume::CUBOID:
			infile.read((char*)& m_bvPosition, sizeof(m_bvPosition));
			infile.read((char*)& m_bvCuboidSize, sizeof(m_bvCuboidSize));
			break;
		case mth::BoundingVolume::SPHERE:
			infile.read((char*)& m_bvPosition, sizeof(m_bvPosition));
			infile.read((char*)& m_bvSphereRadius, sizeof(m_bvSphereRadius));
			break;
		default:
			header.boundingVolumePrimitive = mth::BoundingVolume::NO_TYPE;
			break;
		}
		if (header.hitboxTriangleCount)
		{
			m_hitbox.resize(header.hitboxTriangleCount);
			infile.read((char*)m_hitbox.data(), header.hitboxTriangleCount * sizeof(mth::Triangle));
		}
	}
	void ModelLoader::ReadBonesBinary(std::ifstream& infile, OMDHeader& header)
	{
	}
	void ModelLoader::ReadAnimationsBinary(std::ifstream& infile, OMDHeader& header)
	{
	}

#pragma endregion

#pragma region Load text

	void ModelLoader::LoadOMDText(LPCWSTR filename, UINT modelType)
	{
		std::wifstream infile(filename, std::ios::in);
		OMDHeader header;
		ReadHeaderText(infile, header, modelType);
		ReadVerticesText(infile, header);
		ReadIndicesText(infile, header);
		ReadGroupsText(infile, header);
		ReadMaterialsText(infile, header);
		ReadHitboxText(infile, header);
		ReadBonesText(infile, header);
		ReadAnimationsText(infile, header);
	}
	void ModelLoader::ReadHeaderText(std::wifstream& infile, OMDHeader& header, UINT modelType)
	{
		WCHAR ch;
		infile >> ch;
		header.fileFormat = (char)ch;
		infile >> ch;
		header.extension[0] = (char)ch;
		infile >> ch;
		header.extension[1] = (char)ch;
		infile >> ch;
		header.extension[2] = (char)ch;
		do { infile >> ch; } while (ch != ':');
		infile >> header.modelType;
		do { infile >> ch; } while (ch != ':');
		infile >> header.vertexCount;
		do { infile >> ch; } while (ch != ':');
		infile >> header.indexCount;
		do { infile >> ch; } while (ch != ':');
		infile >> header.groupCount;
		do { infile >> ch; } while (ch != ':');
		infile >> header.materialCount;
		do { infile >> ch; } while (ch != ':');
		infile >> header.boundingVolumePrimitive;
		do { infile >> ch; } while (ch != ':');
		infile >> header.hitboxTriangleCount;
		do { infile >> ch; } while (ch != ':');
		infile >> header.boneCount;
		do { infile >> ch; } while (ch != ':');
		infile >> header.animationCount;
		m_modelType = ModelType::RemoveUnnecessary(header.modelType & modelType);
		m_vertexSizeInBytes = ModelType::VertexSizeInBytes(m_modelType);
		m_boundingVolumeType = header.boundingVolumePrimitive;
	}
	void ModelLoader::ReadVerticesText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		m_vertices.resize(header.vertexCount * m_vertexSizeInBytes / sizeof(VertexElement));
		VertexElement tmp[8];
		UINT vertexCounter = 0;
		for (UINT i = 0; i < header.vertexCount; i++)
		{
			if (ModelType::HasPositions(header.modelType))
			{
				infile >> tmp[0].f;
				infile >> tmp[1].f;
				infile >> tmp[2].f;
				if (ModelType::HasPositions(m_modelType))
				{
					m_vertices[vertexCounter++] = tmp[0];
					m_vertices[vertexCounter++] = tmp[1];
					m_vertices[vertexCounter++] = tmp[2];
				}
			}
			if (ModelType::HasTexcoords(header.modelType))
			{
				infile >> tmp[0].f;
				infile >> tmp[1].f;
				if (ModelType::HasTexcoords(m_modelType))
				{
					m_vertices[vertexCounter++] = tmp[0];
					m_vertices[vertexCounter++] = tmp[1];
				}
			}
			if (ModelType::HasNormals(header.modelType))
			{
				infile >> tmp[0].f;
				infile >> tmp[1].f;
				infile >> tmp[2].f;
				if (ModelType::HasNormals(m_modelType))
				{
					m_vertices[vertexCounter++] = tmp[0];
					m_vertices[vertexCounter++] = tmp[1];
					m_vertices[vertexCounter++] = tmp[2];
				}
			}
			if (ModelType::HasTangentsBinormals(header.modelType))
			{
				infile >> tmp[0].f;
				infile >> tmp[1].f;
				infile >> tmp[2].f;
				infile >> tmp[3].f;
				infile >> tmp[4].f;
				infile >> tmp[5].f;
				if (ModelType::HasTangentsBinormals(m_modelType))
				{
					m_vertices[vertexCounter++] = tmp[0];
					m_vertices[vertexCounter++] = tmp[1];
					m_vertices[vertexCounter++] = tmp[2];
					m_vertices[vertexCounter++] = tmp[3];
					m_vertices[vertexCounter++] = tmp[4];
					m_vertices[vertexCounter++] = tmp[5];
				}
			}
			if (ModelType::HasBones(header.modelType))
			{
				infile >> tmp[0].f;
				infile >> tmp[1].f;
				infile >> tmp[2].f;
				infile >> tmp[3].f;
				infile >> tmp[4].u;
				infile >> tmp[5].u;
				infile >> tmp[6].u;
				infile >> tmp[7].u;
				if (ModelType::HasBones(m_modelType))
				{
					m_vertices[vertexCounter++] = tmp[0];
					m_vertices[vertexCounter++] = tmp[1];
					m_vertices[vertexCounter++] = tmp[2];
					m_vertices[vertexCounter++] = tmp[3];
					m_vertices[vertexCounter++] = tmp[4];
					m_vertices[vertexCounter++] = tmp[5];
					m_vertices[vertexCounter++] = tmp[6];
					m_vertices[vertexCounter++] = tmp[7];
				}
			}
		}
	}
	void ModelLoader::ReadIndicesText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		m_indices.resize(header.indexCount);
		for (UINT i = 0; i < header.indexCount; i++)
			infile >> m_indices[i];
	}
	void ModelLoader::ReadGroupsText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		m_groups.resize(header.groupCount);
		for (UINT i = 0; i < header.groupCount; i++)
		{
			do { infile >> ch; } while (ch != ':');
			infile >> m_groups[i].startIndex;
			do { infile >> ch; } while (ch != ':');
			infile >> m_groups[i].indexCount;
			do { infile >> ch; } while (ch != ':');
			infile >> m_groups[i].materialIndex;
		}

	}
	void ModelLoader::ReadMaterialsText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		m_textureNames.resize(header.materialCount);
		m_normalmapNames.resize(header.materialCount);
		for (UINT i = 0; i < header.materialCount; i++)
		{
			do { infile >> ch; } while (ch != ':');
			do { infile.read(&ch, 1); } while (ch == ' ');
			for (; ch != '\n'; infile.read(&ch, 1))
				m_textureNames[i] += ch;
			do { infile >> ch; } while (ch != ':');
			do { infile.read(&ch, 1); } while (ch == ' ');
			for (; ch != '\n'; infile.read(&ch, 1))
				m_normalmapNames[i] += ch;
		}
	}
	void ModelLoader::ReadHitboxText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
		switch (header.boundingVolumePrimitive)
		{
		case mth::BoundingVolume::CUBOID:
			infile >> m_bvPosition.x;
			infile >> m_bvPosition.y;
			infile >> m_bvPosition.z;
			infile >> m_bvCuboidSize.x;
			infile >> m_bvCuboidSize.y;
			infile >> m_bvCuboidSize.z;
			break;
		case mth::BoundingVolume::SPHERE:
			infile >> m_bvPosition.x;
			infile >> m_bvPosition.y;
			infile >> m_bvPosition.z;
			infile >> m_bvSphereRadius;
			break;
		default:
			header.boundingVolumePrimitive = mth::BoundingVolume::NO_TYPE;
			break;
		}

		do { infile >> ch; } while (ch != ':');
		if (header.hitboxTriangleCount)
		{
			m_hitbox.resize(header.hitboxTriangleCount);
			mth::float3 tri[3];
			mth::float3 plainNormal;
			float plainDistance;
			for (UINT i = 0; i < header.hitboxTriangleCount; i++)
			{
				infile >> tri[0].x;
				infile >> tri[0].y;
				infile >> tri[0].z;
				infile >> tri[1].x;
				infile >> tri[1].y;
				infile >> tri[1].z;
				infile >> tri[2].x;
				infile >> tri[2].y;
				infile >> tri[2].z;
				infile >> plainNormal.x;
				infile >> plainNormal.y;
				infile >> plainNormal.z;
				infile >> plainDistance;
				m_hitbox[i] = mth::Triangle(tri, plainNormal, plainDistance);
			}
		}
	}
	void ModelLoader::ReadBonesText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
	}
	void ModelLoader::ReadAnimationsText(std::wifstream& infile, OMDHeader& header)
	{
		WCHAR ch;
		do { infile >> ch; } while (ch != ':');
	}

#pragma endregion

#pragma region Export binary

	void ModelLoader::ExportOMDBinary(LPCWSTR filename, UINT modelType)
	{
		OMDHeader header;
		header.fileFormat = 'B';
		header.extension[0] = 'O';
		header.extension[1] = 'M';
		header.extension[2] = 'D';
		header.modelType = ModelType::RemoveUnnecessary(m_modelType & modelType);
		header.vertexCount = (UINT)m_vertices.size() / (m_vertexSizeInBytes / sizeof(VertexElement));
		header.indexCount = (UINT)m_indices.size();
		header.groupCount = (UINT)m_groups.size();
		header.materialCount = (UINT)m_textureNames.size();
		header.boundingVolumePrimitive = m_boundingVolumeType;
		header.hitboxTriangleCount = (UINT)m_hitbox.size();
		header.boneCount = 0;
		header.animationCount = 0;

		std::ofstream outfile(filename, std::ios::out | std::ios::binary);
		WriteHeaderBinary(outfile, header);
		WriteVerticesBinary(outfile, header);
		WriteIndicesBinary(outfile, header);
		WriteGroupsBinary(outfile, header);
		WriteMaterialsBinary(outfile, header);
		WriteHitboxBinary(outfile, header);
		WriteBonesBinary(outfile, header);
		WriteAnimationsBinary(outfile, header);
		outfile.close();
	}
	void ModelLoader::WriteHeaderBinary(std::ofstream& outfile, OMDHeader& header)
	{
		outfile.write((char*)& header, sizeof(header));
	}
	void ModelLoader::WriteVerticesBinary(std::ofstream& outfile, OMDHeader& header)
	{
		if (ModelType::VertexLayout(m_modelType) == ModelType::VertexLayout(header.modelType))
			outfile.write((char*)m_vertices.data(), header.vertexCount * m_vertexSizeInBytes);
		else
		{
			UINT vertexCounter = 0;
			for (UINT i = 0; i < header.vertexCount; i++)
			{
				if (ModelType::HasPositions(header.modelType))
					outfile.write((char*)& m_vertices[vertexCounter], 3 * sizeof(VertexElement));
				if (ModelType::HasPositions(m_modelType))
					vertexCounter += 3;
				if (ModelType::HasTexcoords(header.modelType))
					outfile.write((char*)& m_vertices[vertexCounter], 2 * sizeof(VertexElement));
				if (ModelType::HasTexcoords(m_modelType))
					vertexCounter += 2;
				if (ModelType::HasNormals(header.modelType))
					outfile.write((char*)& m_vertices[vertexCounter], 3 * sizeof(VertexElement));
				if (ModelType::HasNormals(m_modelType))
					vertexCounter += 3;
				if (ModelType::HasTangentsBinormals(header.modelType))
					outfile.write((char*)& m_vertices[vertexCounter], 6 * sizeof(VertexElement));
				if (ModelType::HasTangentsBinormals(m_modelType))
					vertexCounter += 6;
				if (ModelType::HasBones(header.modelType))
					outfile.write((char*)& m_vertices[vertexCounter], 8 * sizeof(VertexElement));
				if (ModelType::HasBones(m_modelType))
					vertexCounter += 8;
			}
		}
	}
	void ModelLoader::WriteIndicesBinary(std::ofstream& outfile, OMDHeader& header)
	{
		outfile.write((char*)m_indices.data(), sizeof(UINT) * m_indices.size());
	}
	void ModelLoader::WriteGroupsBinary(std::ofstream& outfile, OMDHeader& header)
	{
		outfile.write((char*)m_groups.data(), m_groups.size() * sizeof(VertexGroup));
	}
	void ModelLoader::WriteMaterialsBinary(std::ofstream& outfile, OMDHeader& header)
	{
		for (UINT i = 0; i < header.materialCount; i++)
		{
			WCHAR nul = 0;
			if (ModelType::HasTexture(header.modelType))
				outfile.write((char*)m_textureNames[i].c_str(), (m_textureNames[i].length() + 1) * sizeof(WCHAR));
			else
				outfile.write((char*)& nul, sizeof(WCHAR));
			if (ModelType::HasNormalmap(header.modelType))
				outfile.write((char*)m_normalmapNames[i].c_str(), (m_normalmapNames[i].length() + 1) * sizeof(WCHAR));
			else
				outfile.write((char*)& nul, sizeof(WCHAR));
		}
	}
	void ModelLoader::WriteHitboxBinary(std::ofstream& outfile, OMDHeader& header)
	{
		switch (header.boundingVolumePrimitive)
		{
		case mth::BoundingVolume::CUBOID:
			outfile.write((char*)& m_bvPosition, sizeof(m_bvPosition));
			outfile.write((char*)& m_bvCuboidSize, sizeof(m_bvCuboidSize));
			break;
		case mth::BoundingVolume::SPHERE:
			outfile.write((char*)& m_bvPosition, sizeof(m_bvPosition));
			outfile.write((char*)& m_bvSphereRadius, sizeof(m_bvSphereRadius));
			break;
		}
		if (header.hitboxTriangleCount)
			outfile.write((char*)m_hitbox.data(), header.hitboxTriangleCount * sizeof(mth::Triangle));
	}
	void ModelLoader::WriteBonesBinary(std::ofstream& outfile, OMDHeader& header)
	{
	}
	void ModelLoader::WriteAnimationsBinary(std::ofstream& outfile, OMDHeader& header)
	{
	}

#pragma endregion

#pragma region Export text

	void ModelLoader::ExportOMDText(LPCWSTR filename, UINT modelType)
	{
		OMDHeader header;
		header.fileFormat = 'T';
		header.extension[0] = 'O';
		header.extension[1] = 'M';
		header.extension[2] = 'D';
		header.modelType = ModelType::RemoveUnnecessary(m_modelType & modelType);
		header.vertexCount = (UINT)m_vertices.size() / (m_vertexSizeInBytes / sizeof(VertexElement));
		header.indexCount = (UINT)m_indices.size();
		header.groupCount = (UINT)m_groups.size();
		header.materialCount = (UINT)m_textureNames.size();
		header.boundingVolumePrimitive = m_boundingVolumeType;
		header.hitboxTriangleCount = (UINT)m_hitbox.size();
		header.boneCount = 0;
		header.animationCount = 0;

		std::wofstream outfile(filename, std::ios::out);
		WriteHeaderText(outfile, header);
		WriteVerticesText(outfile, header);
		WriteIndicesText(outfile, header);
		WriteGroupsText(outfile, header);
		WriteMaterialsText(outfile, header);
		WriteHitboxText(outfile, header);
		WriteBonesText(outfile, header);
		WriteAnimationsText(outfile, header);
		outfile.close();
	}
	void ModelLoader::WriteHeaderText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << header.fileFormat;
		outfile << header.extension[0];
		outfile << header.extension[1];
		outfile << header.extension[2];
		outfile << std::endl;
		outfile << L"Model type: " << header.modelType << std::endl;
		outfile << L"Vertex count: " << header.vertexCount << std::endl;
		outfile << L"Index count: " << header.indexCount << std::endl;
		outfile << L"Group count: " << header.groupCount << std::endl;
		outfile << L"Material count: " << header.materialCount << std::endl;
		outfile << L"Bounding volume primitive: " << header.boundingVolumePrimitive << std::endl;
		outfile << L"Hitbox triangle count: " << header.hitboxTriangleCount << std::endl;
		outfile << L"Bone count: " << header.boneCount << std::endl;
		outfile << L"Animation count: " << header.animationCount << std::endl;
	}
	void ModelLoader::WriteVerticesText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Vertices:" << std::endl;
		UINT vertexCounter = 0;
		for (UINT i = 0; i < header.vertexCount; i++)
		{
			if (ModelType::HasPositions(header.modelType))
			{
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
			}
			else if (ModelType::HasPositions(m_modelType))
				vertexCounter += 3;
			if (ModelType::HasTexcoords(header.modelType))
			{
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
			}
			else if (ModelType::HasTexcoords(m_modelType))
				vertexCounter += 2;
			if (ModelType::HasNormals(header.modelType))
			{
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
			}
			else if (ModelType::HasNormals(m_modelType))
				vertexCounter += 3;
			if (ModelType::HasTangentsBinormals(header.modelType))
			{
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
			}
			else if (ModelType::HasTangentsBinormals(m_modelType))
				vertexCounter += 6;
			if (ModelType::HasBones(header.modelType))
			{
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].f << ' ';
				outfile << m_vertices[vertexCounter++].u << ' ';
				outfile << m_vertices[vertexCounter++].u << ' ';
				outfile << m_vertices[vertexCounter++].u << ' ';
				outfile << m_vertices[vertexCounter++].u << ' ';
			}
			else if (ModelType::HasBones(m_modelType))
				vertexCounter += 8;
			outfile << std::endl;
		}
	}
	void ModelLoader::WriteIndicesText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Indices:" << std::endl;
		for (UINT i = 0; i < header.indexCount; i++)
			outfile << m_indices[i] << ' ';
		outfile << std::endl;
	}
	void ModelLoader::WriteGroupsText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Groups:" << std::endl;
		for (UINT i = 0; i < header.groupCount; i++)
		{
			outfile << L"New group" << std::endl;
			outfile << L"\tStart index: " << m_groups[i].startIndex << std::endl;
			outfile << L"\tIndex count: " << m_groups[i].indexCount << std::endl;
			outfile << L"\tMaterial index: " << m_groups[i].materialIndex << std::endl;
		}
	}
	void ModelLoader::WriteMaterialsText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Materials:" << std::endl;
		for (UINT i = 0; i < header.materialCount; i++)
		{
			outfile << L"New material" << std::endl;
			outfile << L"\tTexture name: ";
			if (ModelType::HasTexture(header.modelType))
				outfile << m_textureNames[i];
			outfile << std::endl;
			outfile << L"\tNormalmap name: ";
			if (ModelType::HasNormalmap(header.modelType))
				outfile << m_normalmapNames[i];
			outfile << std::endl;
		}
	}
	void ModelLoader::WriteHitboxText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Bounding volume:" << std::endl;
		switch (header.boundingVolumePrimitive)
		{
		case mth::BoundingVolume::CUBOID:
			outfile << m_bvPosition.x << ' ';
			outfile << m_bvPosition.y << ' ';
			outfile << m_bvPosition.z << ' ';
			outfile << m_bvCuboidSize.x << ' ';
			outfile << m_bvCuboidSize.y << ' ';
			outfile << m_bvCuboidSize.z << ' ';
			outfile << std::endl;
			break;
		case mth::BoundingVolume::SPHERE:
			outfile << m_bvPosition.x << ' ';
			outfile << m_bvPosition.y << ' ';
			outfile << m_bvPosition.z << ' ';
			outfile << m_bvSphereRadius << ' ';
			outfile << std::endl;
			break;
		default:
			header.boundingVolumePrimitive = mth::BoundingVolume::NO_TYPE;
			break;
		}

		outfile << L"Hitbox:" << std::endl;
		if (header.hitboxTriangleCount)
		{
			for (UINT i = 0; i < header.hitboxTriangleCount; i++)
			{
				outfile << m_hitbox[i].getVertex(0).x << ' ';
				outfile << m_hitbox[i].getVertex(0).y << ' ';
				outfile << m_hitbox[i].getVertex(0).z << ' ';
				outfile << m_hitbox[i].getVertex(1).x << ' ';
				outfile << m_hitbox[i].getVertex(1).y << ' ';
				outfile << m_hitbox[i].getVertex(1).z << ' ';
				outfile << m_hitbox[i].getVertex(2).x << ' ';
				outfile << m_hitbox[i].getVertex(2).y << ' ';
				outfile << m_hitbox[i].getVertex(2).z << ' ';
				outfile << m_hitbox[i].getPlainNormal().x << ' ';
				outfile << m_hitbox[i].getPlainNormal().y << ' ';
				outfile << m_hitbox[i].getPlainNormal().z << ' ';
				outfile << m_hitbox[i].getPlainDistance() << ' ';
				outfile << std::endl;
			}
		}
	}
	void ModelLoader::WriteBonesText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Bones:" << std::endl;
	}
	void ModelLoader::WriteAnimationsText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Animations:" << std::endl;
	}

#pragma endregion

	ModelLoader::ModelLoader() :
		m_vertexSizeInBytes(0),
		m_modelType(0),
		m_boundingVolumeType(0),
		m_bvSphereRadius(0.0f) {}
	ModelLoader::ModelLoader(LPCWSTR filename, UINT modelType) :
		m_vertexSizeInBytes(0),
		m_modelType(0),
		m_boundingVolumeType(0)
	{
		LoadModel(filename, modelType);
	}
	void ModelLoader::Clear()
	{
		m_folder.clear();
		m_filename.clear();
		m_vertexSizeInBytes = 0;
		m_modelType = 0;
		m_boundingVolumeType = 0;
		m_vertices.clear();
		m_indices.clear();
		m_textureNames.clear();
		m_normalmapNames.clear();
		m_groups.clear();
		m_bvPosition = mth::float3();
		m_bvCuboidSize = mth::float3();
		m_bvSphereRadius = 0.0f;
		m_hitbox.clear();
	}
	void ModelLoader::ExportOMD(LPCWSTR filename, UINT modelType, bool binary)
	{
		if (binary)
			ExportOMDBinary(filename, modelType);
		else
			ExportOMDText(filename, modelType);
	}
	void ModelLoader::LoadModel(LPCWSTR filename, UINT modelType)
	{
		std::wstring path;
		for (int i = 0; filename[i]; i++)
		{
			if (filename[i] != '\"')
				path += filename[i];
		}
		UINT lastSlashIndex = 0;
		UINT lastDotIndex = 0;
		UINT i;
		for (i = 0; path[i]; i++)
		{
			if (path[i] == '/' || path[i] == '\\')
				lastSlashIndex = i;
			if (path[i] == '.')
				lastDotIndex = i;
		}
		for (i = 0; i <= lastSlashIndex; i++)
			m_folder += path[i];
		while (i < lastDotIndex)
			m_filename += path[i++];

		if (path[i + 0] == '.' &&
			path[i + 1] == 'o' &&
			path[i + 2] == 'm' &&
			path[i + 3] == 'd' &&
			path[i + 4] == '\0')
			LoadOMD(path.c_str(), modelType);
		else if (path[i + 0] == '.' &&
			path[i + 1] == 'p' &&
			path[i + 2] == 'm' &&
			path[i + 3] == 'x' &&
			path[i + 4] == '\0')
			LoadPMX(path.c_str(), modelType);
		else
			LoadAssimp(path.c_str(), modelType);
		OrganizeMaterials();
	}

#pragma region Create primitives

	void ModelLoader::CreateCube(mth::float3 p, mth::float3 s, UINT modelType)
	{
		Vertex_PTMB vertices[] = {
			{{p.x + s.x, p.y, p.z + s.z}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x + s.x, p.y, p.z}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z}, {1.0f, 0.0f}, {0.0f, -1.0f,0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z + s.z}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x + s.x, p.y + s.y, p.z + s.z}, { 0.0f, 1.0f} ,{ 0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{p.x, p.y + s.y, p.z + s.z}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{p.x, p.y + s.y, p.z},{ 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y + s.y, p.z}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y, p.z + s.z}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y + s.y, p.z + s.z}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f},{0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y + s.y, p.z},{ 1.0f, 0.0f},{ 1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y, p.z}, { 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f, -1.0f}},
			{{p.x + s.x, p.y, p.z}, { 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f},{ 0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x + s.x, p.y + s.y, p.z}, { 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y + s.y, p.z}, { 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z}, { 0.0f, 0.0f},{ 0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z}, { 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f},{ 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{p.x, p.y + s.y, p.z}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f},{ 0.0f, 1.0f, 0.0f},{ 0.0f, 0.0f, 1.0f}},
			{{p.x, p.y + s.y, p.z + s.z}, { 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},{ 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{p.x, p.y, p.z + s.z}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},{ 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{p.x + s.x, p.y + s.y, p.z + s.z}, { 0.0f, 1.0f},{ 0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
			{{p.x + s.x, p.y, p.z + s.z}, { 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f},{ -1.0f, 0.0f, 0.0f}},
			{{p.x, p.y, p.z + s.z}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f},{ -1.0f, 0.0f, 0.0f}},
			{{p.x, p.y + s.y, p.z + s.z}, { 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}
		};
		UINT indices[] = {
			0, 2, 1, 0, 3, 2, 4, 6, 5, 4, 7, 6, 8, 10, 9, 8, 11, 10, 12, 14, 13, 12, 15, 14, 16, 18, 17, 16, 19, 18, 20, 22, 21, 20, 23, 22
		};
		Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]), modelType);
	}

	void ModelLoader::CreateFullScreenQuad()
	{
		CreateQuad({ -1.0f, -1.0f }, { 2.0f, 2.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ModelType::PT);
		Transform(mth::float4x4::Translation(0.0f, 0.0f, 1.0f) * mth::float4x4::RotationX(-mth::pi * 0.5f));
	}

	void ModelLoader::CreateScreenQuad(mth::float2 pos, mth::float2 size)
	{
		CreateQuad(pos, size, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ModelType::PT);
		Transform(mth::float4x4::Translation(0.0f, 0.0f, 1.0f) * mth::float4x4::RotationX(-mth::pi * 0.5f));
	}

	void ModelLoader::CreateQuad(mth::float2 pos, mth::float2 size, UINT modelType)
	{
		CreateQuad(pos, size, { 0.0f, 0.0f }, { 1.0f, 1.0f }, modelType);
	}

	void ModelLoader::CreateQuad(mth::float2 pos, mth::float2 size, mth::float2 tpos, mth::float2 tsize, UINT modelType)
	{
		Vertex_PTMB vertices[] = {
		{ {pos.x + size.x, 0.0f, pos.y}, { tpos.x + tsize.x, tpos.y + tsize.y }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {pos.x + size.x, 0.0f, pos.y + size.y}, { tpos.x + tsize.x, tpos.y }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {pos.x, 0.0f, pos.y + size.y}, { tpos.x, tpos.y }, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {pos.x, 0.0f, pos.y}, { tpos.x, tpos.y + tsize.y }, { 0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }
		};
		UINT indices[] = { 0, 2, 1, 0, 3, 2 };
		Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]), modelType);
	}

#pragma endregion

	void ModelLoader::MakeHitboxFromVertices()
	{
		mth::float3 maxpos, minpos;
		m_hitbox.clear();
		m_boundingVolumeType = mth::BoundingVolume::CUBOID;
		m_bvSphereRadius = 0.0f;
		minpos.x = maxpos.x = m_vertices[0].f;
		minpos.y = maxpos.y = m_vertices[1].f;
		minpos.z = maxpos.z = m_vertices[2].f;
		UINT vertexSize = getVertexSizeInFloats();
		m_hitbox.resize(m_indices.size() / 3);
		mth::float3 tri[3];
		for (UINT i = 0; i < (UINT)m_hitbox.size(); i++)
		{
			tri[0] = mth::float3(
				m_vertices[vertexSize * m_indices[i * 3 + 0] + 0].f,
				m_vertices[vertexSize * m_indices[i * 3 + 0] + 1].f,
				m_vertices[vertexSize * m_indices[i * 3 + 0] + 2].f);
			tri[1] = mth::float3(
				m_vertices[vertexSize * m_indices[i * 3 + 1] + 0].f,
				m_vertices[vertexSize * m_indices[i * 3 + 1] + 1].f,
				m_vertices[vertexSize * m_indices[i * 3 + 1] + 2].f);
			tri[2] = mth::float3(
				m_vertices[vertexSize * m_indices[i * 3 + 2] + 0].f,
				m_vertices[vertexSize * m_indices[i * 3 + 2] + 1].f,
				m_vertices[vertexSize * m_indices[i * 3 + 2] + 2].f);
			for (UINT v = 0; v < 3; v++)
			{
				if (minpos.x > tri[v].x) minpos.x = tri[v].x;
				if (maxpos.x < tri[v].x) maxpos.x = tri[v].x;
				if (minpos.y > tri[v].x) minpos.y = tri[v].x;
				if (maxpos.y < tri[v].x) maxpos.y = tri[v].x;
				if (minpos.z > tri[v].x) minpos.z = tri[v].x;
				if (maxpos.z < tri[v].x) maxpos.z = tri[v].x;
			}
			m_hitbox[i] = mth::Triangle(tri);
		}
		m_bvPosition = minpos;
		m_bvCuboidSize = maxpos - minpos;
	}

	void ModelLoader::MakeVerticesFromHitbox()
	{
		m_modelType = ModelType::P;
		m_vertexSizeInBytes = ModelType::VertexSizeInBytes(m_modelType);
		m_vertices.resize(m_hitbox.size() * 3 * 3);
		for (size_t i = 0; i < m_hitbox.size(); i++)
		{
			m_vertices[9 * i + 0] = m_hitbox[i].getVertex(0).x;
			m_vertices[9 * i + 1] = m_hitbox[i].getVertex(0).y;
			m_vertices[9 * i + 2] = m_hitbox[i].getVertex(0).z;
			m_vertices[9 * i + 3] = m_hitbox[i].getVertex(1).x;
			m_vertices[9 * i + 4] = m_hitbox[i].getVertex(1).y;
			m_vertices[9 * i + 5] = m_hitbox[i].getVertex(1).z;
			m_vertices[9 * i + 6] = m_hitbox[i].getVertex(2).x;
			m_vertices[9 * i + 7] = m_hitbox[i].getVertex(2).y;
			m_vertices[9 * i + 8] = m_hitbox[i].getVertex(2).z;
		}
		m_indices.resize(m_hitbox.size() * 3);
		for (int i = 0; i < (int)m_indices.size(); i++)
			m_indices[i] = i;
		m_groups.clear();
		m_groups.push_back({ 0, (UINT)m_indices.size() , 0 });
		m_textureNames.clear();
		m_textureNames.push_back(L"");
		m_normalmapNames.clear();
		m_normalmapNames.push_back(L"");
	}

	bool ModelLoader::HasHitbox()
	{
		return m_hitbox.size() > 0;
	}

	void ModelLoader::SwapHitboxes(ModelLoader& other)
	{
		std::swap(other.m_boundingVolumeType, m_boundingVolumeType);
		std::swap(other.m_bvPosition, m_bvPosition);
		std::swap(other.m_bvCuboidSize, m_bvCuboidSize);
		std::swap(other.m_bvSphereRadius, m_bvSphereRadius);
		other.m_hitbox.swap(m_hitbox);
	}

	void ModelLoader::FlipInsideOut()
	{
		for (size_t i = 0; i < m_indices.size(); i += 3)
		{
			UINT tmp = m_indices[i + 1];
			m_indices[i + 1] = m_indices[i + 2];
			m_indices[i + 2] = tmp;
		}
		UINT vertexSize = getVertexSizeInFloats();
		if (ModelType::HasNormals(m_modelType))
		{
			UINT offset = ModelType::NormalOffset(m_modelType);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				m_vertices[i * vertexSize + offset + 0].f *= -1.0f;
				m_vertices[i * vertexSize + offset + 1].f *= -1.0f;
				m_vertices[i * vertexSize + offset + 2].f *= -1.0f;
			}
		}
		if (ModelType::HasTangentsBinormals(m_modelType))
		{
			UINT offset1 = ModelType::TangentOffset(m_modelType);
			UINT offset2 = ModelType::BinormalOffset(m_modelType);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				m_vertices[i * vertexSize + offset1 + 0].f *= -1.0f;
				m_vertices[i * vertexSize + offset1 + 1].f *= -1.0f;
				m_vertices[i * vertexSize + offset1 + 2].f *= -1.0f;
				m_vertices[i * vertexSize + offset2 + 0].f *= -1.0f;
				m_vertices[i * vertexSize + offset2 + 1].f *= -1.0f;
				m_vertices[i * vertexSize + offset2 + 2].f *= -1.0f;
			}
		}
	}

	void ModelLoader::Transform(mth::float4x4 transform)
	{
		UINT vertexSize = getVertexSizeInFloats();
		mth::float4 v;
		if (ModelType::HasPositions(m_modelType))
		{
			UINT offset = ModelType::PositionOffset(m_modelType);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				v.x = m_vertices[i * vertexSize + offset + 0].f;
				v.y = m_vertices[i * vertexSize + offset + 1].f;
				v.z = m_vertices[i * vertexSize + offset + 2].f;
				v.w = 1;
				v = transform * v;
				m_vertices[i * vertexSize + offset + 0] = v.x;
				m_vertices[i * vertexSize + offset + 1] = v.y;
				m_vertices[i * vertexSize + offset + 2] = v.z;
			}
		}
		if (ModelType::HasNormals(m_modelType))
		{
			UINT offset = ModelType::NormalOffset(m_modelType);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				v.x = m_vertices[i * vertexSize + offset + 0].f;
				v.y = m_vertices[i * vertexSize + offset + 1].f;
				v.z = m_vertices[i * vertexSize + offset + 2].f;
				v.w = 1;
				v = transform * v;
				m_vertices[i * vertexSize + offset + 0] = v.x;
				m_vertices[i * vertexSize + offset + 1] = v.y;
				m_vertices[i * vertexSize + offset + 2] = v.z;
			}
		}
		if (ModelType::HasTangentsBinormals(m_modelType))
		{
			UINT offset1 = ModelType::TangentOffset(m_modelType);
			UINT offset2 = ModelType::BinormalOffset(m_modelType);
			for (UINT i = 0; i < getVertexCount(); i++)
			{
				v.x = m_vertices[i * vertexSize + offset1 + 0].f;
				v.y = m_vertices[i * vertexSize + offset1 + 1].f;
				v.z = m_vertices[i * vertexSize + offset1 + 2].f;
				v.w = 1;
				v = transform * v;
				m_vertices[i * vertexSize + offset1 + 0] = v.x;
				m_vertices[i * vertexSize + offset1 + 1] = v.y;
				m_vertices[i * vertexSize + offset1 + 2] = v.z;

				v.x = m_vertices[i * vertexSize + offset2 + 0].f;
				v.y = m_vertices[i * vertexSize + offset2 + 1].f;
				v.z = m_vertices[i * vertexSize + offset2 + 2].f;
				v.w = 1;
				v = transform * v;
				m_vertices[i * vertexSize + offset2 + 0] = v.x;
				m_vertices[i * vertexSize + offset2 + 1] = v.y;
				m_vertices[i * vertexSize + offset2 + 2] = v.z;
			}
		}
	}
}