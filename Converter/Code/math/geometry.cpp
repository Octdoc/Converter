#include "geometry.h"

namespace mth
{
	Triangle::Triangle() :m_plainDistance(0) {}
	Triangle::Triangle(float3 tri[]) :
		Triangle(tri[0], tri[1], tri[2]) {}
	Triangle::Triangle(float3 tri1, float3 tri2, float3 tri3)
	{
		m_vertices[0] = tri1;
		m_vertices[1] = tri2;
		m_vertices[2] = tri3;
		m_plainNormal = (m_vertices[1] - m_vertices[2]).Cross(m_vertices[1] - m_vertices[0]).Normalized();
		m_plainDistance = m_plainNormal.Dot(m_vertices[0]);
	}

	Triangle::Triangle(float3 tri[], float3 plainNormal, float plainDistance)
	{
		m_vertices[0] = tri[0];
		m_vertices[1] = tri[1];
		m_vertices[2] = tri[2];
		m_plainNormal = plainNormal;
		m_plainDistance = plainDistance;
	}

	bool Triangle::isPointOver(float3 point)
	{
		return isPointOverTriangle(m_vertices, point);
	}

	float Triangle::DirectionalDistance(Position ray)
	{
		return DirectionalDistance(ray.position, float3x3::Rotation(ray.rotation)*float3(0.0f, 0.0f, 1.0f));
	}

	float Triangle::DirectionalDistance(float3 raypos, float3 raydir)
	{
		float distfromtriplain = m_plainNormal.Dot(raypos) - m_plainDistance;
		float disttowardplain = -m_plainNormal.Dot(raydir);
		if (disttowardplain < 0.0f)
			return NAN;
		float dist = distfromtriplain / disttowardplain;
		float3 point = dist * raydir + raypos;
		return isPointOverTriangle(m_vertices, point) ? dist : NAN;
	}

	bool isPointOverTriangle(float3 tri[], float3 point)
	{
		float3 vecs[3] = { tri[1] - tri[0],
			tri[2] - tri[0],
			point - tri[0] };
		float dot00 = vecs[0].Dot(vecs[0]);
		float dot01 = vecs[0].Dot(vecs[1]);
		float dot02 = vecs[0].Dot(vecs[2]);
		float dot11 = vecs[1].Dot(vecs[1]);
		float dot12 = vecs[1].Dot(vecs[2]);
		float denom = dot00 * dot11 - dot01 * dot01;
		float u = (dot11 * dot02 - dot01 * dot12) / denom;
		float v = (dot00 * dot12 - dot01 * dot02) / denom;
		return u >= 0.0f && v >= 0.0f && u + v <= 1.0f;
	}
}