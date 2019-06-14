#include "omdexporter.h"

namespace gfx
{

#pragma region Export binary

	void OMDExporter::ExportOMDBinary(LPCWSTR filename, UINT modelType)
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
	void OMDExporter::WriteHeaderBinary(std::ofstream& outfile, OMDHeader& header)
	{
		outfile.write((char*)& header, sizeof(header));
	}
	void OMDExporter::WriteVerticesBinary(std::ofstream& outfile, OMDHeader& header)
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
	void OMDExporter::WriteIndicesBinary(std::ofstream& outfile, OMDHeader& header)
	{
		outfile.write((char*)m_indices.data(), sizeof(UINT) * m_indices.size());
	}
	void OMDExporter::WriteGroupsBinary(std::ofstream& outfile, OMDHeader& header)
	{
		outfile.write((char*)m_groups.data(), m_groups.size() * sizeof(VertexGroup));
	}
	void OMDExporter::WriteMaterialsBinary(std::ofstream& outfile, OMDHeader& header)
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
	void OMDExporter::WriteHitboxBinary(std::ofstream& outfile, OMDHeader& header)
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
	void OMDExporter::WriteBonesBinary(std::ofstream& outfile, OMDHeader& header)
	{
	}
	void OMDExporter::WriteAnimationsBinary(std::ofstream& outfile, OMDHeader& header)
	{
	}

#pragma endregion

#pragma region Export text

	void OMDExporter::ExportOMDText(LPCWSTR filename, UINT modelType)
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
	void OMDExporter::WriteHeaderText(std::wofstream& outfile, OMDHeader& header)
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
	void OMDExporter::WriteVerticesText(std::wofstream& outfile, OMDHeader& header)
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
	void OMDExporter::WriteIndicesText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Indices:" << std::endl;
		for (UINT i = 0; i < header.indexCount; i++)
			outfile << m_indices[i] << ' ';
		outfile << std::endl;
	}
	void OMDExporter::WriteGroupsText(std::wofstream& outfile, OMDHeader& header)
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
	void OMDExporter::WriteMaterialsText(std::wofstream& outfile, OMDHeader& header)
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
	void OMDExporter::WriteHitboxText(std::wofstream& outfile, OMDHeader& header)
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
	void OMDExporter::WriteBonesText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Bones:" << std::endl;
	}
	void OMDExporter::WriteAnimationsText(std::wofstream& outfile, OMDHeader& header)
	{
		outfile << std::endl << L"Animations:" << std::endl;
	}

#pragma endregion

}