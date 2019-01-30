#pragma once

#include "position.h"

namespace mth
{
	class Triangle
	{
		mth::float3 m_vertices[3];
		mth::float3 m_plainNormal;
		float m_plainDistance;

	public:
		Triangle();
		Triangle(float3 tri[]);
		Triangle(float3 tri1, float3 tri2, float3 tri3);
		Triangle(float3 tri[], float3 plainNormal, float plainDistance);

		bool isPointOver(float3 point);
		float DirectionalDistance(Position ray);
		float DirectionalDistance(float3 raypos, float3 raydir);

		inline float3* getVertices(unsigned index) { return m_vertices; }
		inline float3 getVertex(unsigned index) { return m_vertices[index]; }
		inline float3 getPlainNormal() { return m_plainNormal; }
		inline float getPlainDistance() { return m_plainDistance; }
	};

	bool isPointOverTriangle(float3 tri[], float3 point);
}