#include "boundingvolume.h"

namespace mth
{
	bool Intersects_PointCuboid(mth::float3 p1, mth::float3 p2min, mth::float3 p2max)
	{
		return
			p2min.x < p1.x && p1.x < p2max.x &&
			p2min.y < p1.y && p1.y < p2max.y &&
			p2min.z < p1.z && p1.z < p2max.z;
	}
	bool Intersects_PointSphere(mth::float3 p1, mth::float3 p2, float r)
	{
		return (p1 - p2).LengthSquare() < r*r;
	}

	bool Intersects_CuboidCuboid(BV_Cuboid& h1, BV_Cuboid& h2)
	{
		return Intersects_PointCuboid(h1.position, h2.position - h1.size, h2.position + h2.size);
	}
	bool Intersects_CuboidSphere(BV_Cuboid& h1, BV_Sphere& h2)
	{
		mth::float3 point;
		point.x = fmaxf(h1.position.x, fminf(h2.position.x, h1.position.x + h1.size.x));
		point.y = fmaxf(h1.position.y, fminf(h2.position.y, h1.position.y + h1.size.y));
		point.z = fmaxf(h1.position.z, fminf(h2.position.z, h1.position.z + h1.size.z));
		return Intersects_PointSphere(point, h2.position, h2.radius);
	}
	bool Intersects_SphereSphere(BV_Sphere& h1, BV_Sphere& h2)
	{
		return Intersects_PointSphere(h1.position, h2.position, h1.radius + h2.radius);
	}

	bool BV_Cuboid::Intersects(BoundingVolume& other)
	{
		return other.Intersects(*this);
	}
	bool BV_Cuboid::Intersects(BV_Cuboid& other)
	{
		return Intersects_CuboidCuboid(*this, other);
	}
	bool BV_Cuboid::Intersects(BV_Sphere& other)
	{
		return Intersects_CuboidSphere(*this, other);
	}
	bool BV_Sphere::Intersects(BoundingVolume& other)
	{
		return other.Intersects(*this);
	}
	bool BV_Sphere::Intersects(BV_Cuboid& other)
	{
		return Intersects_CuboidSphere(other, *this);
	}
	bool BV_Sphere::Intersects(BV_Sphere& other)
	{
		return Intersects_SphereSphere(*this, other);
	}

	bool Intersects(BoundingVolume& bv1, BoundingVolume& bv2)
	{
		return bv1.Intersects(bv2);
	}
}