#pragma once

#include "linalg.h"
#include "helpers.h"

namespace mth
{

	class BV_Cuboid;
	class BV_Sphere;

	class BoundingVolume
	{
	public:
		enum Type :UINT
		{
			NO_TYPE = 0,
			CUBOID = 1,
			SPHERE
		};

		mth::float3 position;

	public:
		virtual bool Intersects(BoundingVolume& other) = 0;
		virtual bool Intersects(BV_Cuboid& other) = 0;
		virtual bool Intersects(BV_Sphere& other) = 0;
	};

	class BV_Cuboid :public BoundingVolume
	{
	public:
		mth::float3 size;

	public:
		virtual bool Intersects(BoundingVolume& other) override;
		virtual bool Intersects(BV_Cuboid& other) override;
		virtual bool Intersects(BV_Sphere& other) override;
	};

	class BV_Sphere :public BoundingVolume
	{
	public:
		float radius;

	public:
		virtual bool Intersects(BoundingVolume& other) override;
		virtual bool Intersects(BV_Cuboid& other) override;
		virtual bool Intersects(BV_Sphere& other) override;
	};

	bool Intersects(BoundingVolume& bv1, BoundingVolume& bv2);
}