#include "linalg.h"

namespace mth
{

#pragma region float2

	float2::float2()
	{
		memset(this, 0, sizeof(*this));
	}
	float2::float2(const float f) : x(f), y(f) {}
	float2::float2(const float * const v)
	{
		memcpy(this, v, sizeof(float2));
	}
	float2::float2(const float x, const float y) : x(x), y(y) {}
	float2::float2(const float2& v)
	{
		memcpy(this, &v, sizeof(float2));
	}
	float2::float2(const float2* v)
	{
		memcpy(this, v, sizeof(float2));
	}
	bool float2::isZeroVector() const
	{
		return x == 0.0f && y == 0.0f;
	}
	float float2::Dot(const float2 v) const
	{
		return x * v.x + y * v.y;
	}
	float float2::LengthSquare() const
	{
		return x * x + y * y;
	}
	float float2::Length() const
	{
		return sqrtf(LengthSquare());
	}
	void float2::Abs()
	{
		x = fabsf(x);
		y = fabsf(y);
	}
	void float2::Normalize()
	{
		*this /= Length();
	}
	float2 float2::Normalized() const
	{
		return *this / this->Length();
	}
	float2 float2::operator+(const float2 v) const
	{
		return float2(x + v.x, y + v.y);
	}
	float2 float2::operator-(const float2 v) const
	{
		return float2(x - v.x, y - v.y);
	}
	float2 float2::operator*(const float2 v) const
	{
		return float2(x * v.x, y * v.y);
	}
	float2 float2::operator/(const float2 v) const
	{
		return float2(x / v.x, y / v.y);
	}
	float2 float2::operator+(const float f) const
	{
		return float2(x + f, y + f);
	}
	float2 float2::operator-(const float f) const
	{
		return float2(x - f, y - f);
	}
	float2 float2::operator*(const float f) const
	{
		return float2(x * f, y * f);
	}
	float2 float2::operator/(const float f) const
	{
		return float2(x / f, y / f);
	}
	float2 float2::operator-() const
	{
		return float2(-x, -y);
	}
	float2& float2::operator+=(const float2 v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	float2& float2::operator-=(const float2 v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	float2& float2::operator*=(const float2 v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}
	float2& float2::operator/=(const float2 v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}
	float2& float2::operator=(const float2 v)
	{
		memcpy(this, &v, sizeof(float2));
		return *this;
	}
	float2& float2::operator+=(const float f)
	{
		x += f;
		y += f;
		return *this;
	}
	float2& float2::operator-=(const float f)
	{
		x -= f;
		y -= f;
		return *this;
	}
	float2& float2::operator*=(const float f)
	{
		x *= f;
		y *= f;
		return *this;
	}
	float2& float2::operator/=(const float f)
	{
		x /= f;
		y /= f;
		return *this;
	}
	float2& float2::operator=(const float f)
	{
		x = f;
		y = f;
		return *this;
	}
	float2 float2::operator*(const float2x2& m) const
	{
		return float2(x * m(0, 0) + y * m(1, 0), x * m(0, 1) + y * m(1, 1));
	}
	bool float2::operator==(float2 v)
	{
		return memcmp(this, &v, sizeof(float2)) == 0;
	}
	bool float2::isNear(float2 v, float eps)
	{
		return fabsf(x - v.x) < eps && fabsf(y - v.y) < eps;
	}
	float float2::operator()(const int i) const
	{
		return ((float*)((void*)this))[i];
	}
	float& float2::operator()(const int i)
	{
		return ((float*)((void*)this))[i];
	}
	float2::operator float3() const
	{
		return float3(x, y, 1.0f);
	}
	float2::operator float4() const
	{
		return float4(x, y, 1.0f, 1.0f);
	}
	float2::operator float*() const
	{
		return (float*)((void*)this);
	}

#pragma endregion

#pragma region float3

	float3::float3()
	{
		memset(this, 0, sizeof(float3));
	}
	float3::float3(const float f) : x(f), y(f), z(f) {}
	float3::float3(const float * const v)
	{
		memcpy(this, v, sizeof(float3));
	}
	float3::float3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	float3::float3(const float3& v)
	{
		memcpy(this, &v, sizeof(float3));
	}
	float3::float3(const float3* v)
	{
		memcpy(this, v, sizeof(float3));
	}
	bool float3::isZeroVector() const
	{
		return x == 0.0f && y == 0.0f &&z == 0.0f;
	}
	float float3::Dot(const float3 v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	float3 float3::Cross(const float3 v) const
	{
		return float3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	float float3::LengthSquare() const
	{
		return x * x + y * y + z * z;
	}
	float float3::Length() const
	{
		return sqrtf(LengthSquare());
	}
	void float3::Abs()
	{
		x = fabsf(x);
		y = fabsf(y);
		z = fabsf(z);
	}
	void float3::Normalize()
	{
		*this /= Length();
	}
	float3 float3::Normalized() const
	{
		return *this / this->Length();
	}
	float3 float3::operator+(const float3 v) const
	{
		return float3(x + v.x, y + v.y, z + v.z);
	}
	float3 float3::operator-(const float3 v) const
	{
		return float3(x - v.x, y - v.y, z - v.z);
	}
	float3 float3::operator*(const float3 v) const
	{
		return float3(x * v.x, y * v.y, z * v.z);
	}
	float3 float3::operator/(const float3 v) const
	{
		return float3(x / v.x, y / v.y, z / v.z);
	}
	float3 float3::operator+(const float f) const
	{
		return float3(x + f, y + f, z + f);
	}
	float3 float3::operator-(const float f) const
	{
		return float3(x - f, y - f, z - f);
	}
	float3 float3::operator*(const float f) const
	{
		return float3(x * f, y * f, z * f);
	}
	float3 float3::operator/(const float f) const
	{
		return float3(x / f, y / f, z / f);
	}
	float3 float3::operator-() const
	{
		return float3(-x, -y, -z);
	}
	float3& float3::operator+=(const float3 v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	float3& float3::operator-=(const float3 v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	float3& float3::operator*=(const float3 v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	float3& float3::operator/=(const float3 v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	float3& float3::operator=(const float3 v)
	{
		memcpy(this, &v, sizeof(float3));
		return *this;
	}
	float3& float3::operator+=(const float f)
	{
		x += f;
		y += f;
		z += f;
		return *this;
	}
	float3& float3::operator-=(const float f)
	{
		x -= f;
		y -= f;
		z -= f;
		return *this;
	}
	float3& float3::operator*=(const float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	float3& float3::operator/=(const float f)
	{
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}
	float3 & float3::operator=(const float f)
	{
		x = f;
		y = f;
		z = f;
		return *this;
	}
	float3 float3::operator*(const float3x3& m) const
	{
		return float3(
			x * m(0, 0) + y * m(1, 0) + z * m(2, 0),
			x * m(0, 1) + y * m(1, 1) + z * m(2, 1),
			x * m(0, 2) + y * m(1, 2) + z * m(2, 2));
	}
	bool float3::operator==(float3 v)
	{
		return memcmp(this, &v, sizeof(float3)) == 0;
	}
	bool float3::isNear(float3 v, float eps)
	{
		return fabsf(x - v.x) < eps && fabsf(y - v.y) < eps && fabsf(z - v.z) < eps;
	}
	float float3::operator()(const int i) const
	{
		return ((float*)((void*)this))[i];
	}
	float& float3::operator()(const int i)
	{
		return ((float*)((void*)this))[i];
	}
	float3::operator float2() const
	{
		return float2(x, y);
	}
	float3::operator float4() const
	{
		return float4(x, y, z, 1.0f);
	}
	float3::operator float*() const
	{
		return (float*)((void*)this);
	}

#pragma endregion

#pragma region float4

	float4::float4()
	{
		memset(this, 0, sizeof(float4));
	}
	float4::float4(const float f) : x(f), y(f), z(f), w(f) {}
	float4::float4(const float * const v)
	{
		memcpy(this, v, sizeof(float4));
	}
	float4::float4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
	float4::float4(const float4& v)
	{
		memcpy(this, &v, sizeof(float4));
	}
	float4::float4(const float4* v)
	{
		memcpy(this, v, sizeof(float4));
	}
	bool float4::isZeroVector() const
	{
		return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
	}
	float float4::Dot(const float4 v) const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}
	float float4::LengthSquare() const
	{
		return x * x + y * y + z * z + w * w;
	}
	float float4::Length() const
	{
		return sqrtf(LengthSquare());
	}
	void float4::Abs()
	{
		x = fabsf(x);
		y = fabsf(y);
		z = fabsf(z);
		w = fabsf(w);
	}
	void float4::Normalize()
	{
		*this /= Length();
	}
	float4 float4::Normalized() const
	{
		return *this / Length();
	}
	float4 float4::operator+(const float4 v) const
	{
		return float4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	float4 float4::operator-(const float4 v) const
	{
		return float4(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	float4 float4::operator*(const float4 v) const
	{
		return float4(x * v.x, y * v.y, z * v.z, w * v.w);
	}
	float4 float4::operator/(const float4 v) const
	{
		return float4(x / v.x, y / v.y, z / v.z, w / v.w);
	}
	float4 float4::operator+(const float f) const
	{
		return float4(x + f, y + f, z + f, w + f);
	}
	float4 float4::operator-(const float f) const
	{
		return float4(x - f, y - f, z - f, w - f);
	}
	float4 float4::operator*(const float f) const
	{
		return float4(x * f, y * f, z * f, w * f);
	}
	float4 float4::operator/(const float f) const
	{
		return float4(x / f, y / f, z / f, w / f);
	}
	float4 float4::operator-() const
	{
		return float4(-x, -y, -z, -w);
	}
	float4& float4::operator+=(const float4 v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	float4& float4::operator-=(const float4 v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	float4& float4::operator*=(const float4 v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}
	float4& float4::operator/=(const float4 v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}
	float4& float4::operator=(const float4 v)
	{
		memcpy(this, &v, sizeof(float4));
		return *this;
	}
	float4& float4::operator+=(const float f)
	{
		x += f;
		y += f;
		z += f;
		w += f;
		return *this;
	}
	float4& float4::operator-=(const float f)
	{
		x -= f;
		y -= f;
		z -= f;
		w -= f;
		return *this;
	}
	float4& float4::operator*=(const float f)
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}
	float4& float4::operator/=(const float f)
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}
	float4& float4::operator=(const float f)
	{
		x = f;
		y = f;
		z = f;
		w = f;
		return *this;
	}
	float4 float4::operator*(const float4x4& m) const
	{
		return float4(
			x * m(0, 0) + y * m(1, 0) + z * m(2, 0) + w * m(3, 0),
			x * m(0, 1) + y * m(1, 1) + z * m(2, 1) + w * m(3, 1),
			x * m(0, 2) + y * m(1, 2) + z * m(2, 2) + w * m(3, 2),
			x * m(0, 3) + y * m(1, 3) + z * m(2, 3) + w * m(3, 3));
	}
	bool float4::operator==(float4 v)
	{
		return memcmp(this, &v, sizeof(float4)) == 0;
	}
	bool float4::isNear(float4 v, float eps)
	{
		return fabsf(x - v.x) < eps && fabsf(y - v.y) < eps && fabsf(z - v.z) < eps && fabsf(w - v.w) < eps;
	}
	float float4::operator()(const int i) const
	{
		return ((float*)((void*)this))[i];
	}
	float& float4::operator()(const int i)
	{
		return ((float*)((void*)this))[i];
	}
	float4::operator float2() const
	{
		return float2(x, y);
	}
	float4::operator float3() const
	{
		return float3(x, y, z);
	}
	float4::operator float*() const
	{
		return (float*)((void*)this);
	}

#pragma endregion

#pragma region quaternion

	quaternion::quaternion()
	{
		memset(this, 0, sizeof(quaternion));
	}
	quaternion::quaternion(const float f) : x(0.0f), y(0.0f), z(0.0f), w(f) {}
	quaternion::quaternion(const float * const q)
	{
		memcpy(this, q, sizeof(quaternion));
	}
	quaternion::quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
	quaternion::quaternion(const quaternion& q)
	{
		memcpy(this, &q, sizeof(quaternion));
	}
	quaternion::quaternion(const quaternion* q)
	{
		memcpy(this, q, sizeof(quaternion));
	}
	bool quaternion::isZeroVector() const
	{
		return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
	}
	quaternion quaternion::Conjugate() const
	{
		return quaternion(-x, -y, -z, w);
	}
	quaternion quaternion::Inverse()
	{
		return Conjugate() / LengthSquare();
	}
	float quaternion::LengthSquare() const
	{
		return x * x + y * y + z * z + w * w;
	}
	float quaternion::Length() const
	{
		return sqrtf(LengthSquare());
	}
	quaternion quaternion::operator+(const quaternion q) const
	{
		return quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
	}
	quaternion quaternion::operator-(const quaternion q) const
	{
		return quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
	}
	quaternion quaternion::operator*(const quaternion q) const
	{
		return quaternion(
			q.x*w - q.y*z + q.z*y + q.w*x,
			q.x*z + q.y*w - q.z*x + q.w*y,
			-q.x*y + q.y*x + q.z*w + q.w*z,
			-q.x*x - q.y*y - q.z*z + q.w*w);
	}
	quaternion quaternion::operator+(const float f) const
	{
		return quaternion(x, y, z, w + f);
	}
	quaternion quaternion::operator-(const float f) const
	{
		return quaternion(x, y, z, w - f);
	}
	quaternion quaternion::operator*(const float f) const
	{
		return quaternion(x * f, y * f, z * f, w * f);
	}
	quaternion quaternion::operator/(const float f) const
	{
		return quaternion(x / f, y / f, z / f, w / f);
	}
	quaternion quaternion::operator-() const
	{
		return quaternion(-x, -y, -z, -w);
	}
	quaternion& quaternion::operator+=(const quaternion q)
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;
		return *this;
	}
	quaternion& quaternion::operator-=(const quaternion q)
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;
		return *this;
	}
	quaternion& quaternion::operator*=(const quaternion q)
	{
		*this = *this*q;
		return *this;
	}
	quaternion& quaternion::operator=(const quaternion q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
		return *this;
	}
	quaternion& quaternion::operator+=(const float f)
	{
		w += f;
		return *this;
	}
	quaternion& quaternion::operator-=(const float f)
	{
		w -= f;
		return *this;
	}
	quaternion& quaternion::operator*=(const float f)
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}
	quaternion& quaternion::operator/=(const float f)
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}
	quaternion& quaternion::operator=(const float f)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = f;
		return *this;
	}
	bool quaternion::operator==(quaternion q)
	{
		return memcmp(this, &q, sizeof(quaternion)) == 0;
	}
	bool quaternion::operator==(float f)
	{
		return x == 0.0f && y == 0.0f && z == 0.0f && w == f;
	}
	bool quaternion::isNear(quaternion q, float eps)
	{
		return fabsf(x - q.x) < eps && fabsf(y - q.y) < eps && fabsf(z - q.z) < eps && fabsf(w - q.w) < eps;
	}
	float quaternion::operator()(const int i) const
	{
		return ((float*)((void*)this))[i];
	}
	float& quaternion::operator()(const int i)
	{
		return ((float*)((void*)this))[i];
	}
	quaternion::operator float*() const
	{
		return (float*)((void*)this);
	}

#pragma endregion

#pragma region float2x2

	float2x2::float2x2()
	{
		memset(this, 0, sizeof(float2x2));
	}
	float2x2::float2x2(const float f)
	{
		mat[0][0] = f; mat[0][1] = f;
		mat[1][0] = f; mat[1][1] = f;
	}
	float2x2::float2x2(const float * const m)
	{
		memcpy(mat, m, sizeof(float2x2));
	}
	float2x2::float2x2(const float _00, const float _01, const float _10, const float _11)
	{
		mat[0][0] = _00; mat[0][1] = _01;
		mat[1][0] = _10; mat[1][1] = _11;
	}
	float2x2::float2x2(const float2x2& m)
	{
		memcpy(mat, &m, sizeof(float2x2));
	}
	float2x2::float2x2(const float2x2* m)
	{
		memcpy(mat, m, sizeof(float2x2));
	}
	float2x2 float2x2::Identity()
	{
		return float2x2(1.0f, 0.0f, 0.0f, 1.0f);
	}
	float2x2 float2x2::Rotation(const float a)
	{
		float ca = cosf(a), sa = sinf(a);
		return float2x2(ca, -sa, sa, ca);
	}
	float2x2 float2x2::Scaling(const float x, const float y)
	{
		return float2x2(x, 0.0f, 0.0f, y);
	}
	float2x2 float2x2::Scaling(const float2 s)
	{
		return float2x2(s.x, 0.0f, 0.0f, s.y);
	}
	float2x2 float2x2::RotationScaling(const float a, const float x, const float y)
	{
		float ca = cosf(a), sa = sinf(a);
		return float2x2(ca*x, -sa * x, sa*y, ca*y);
	}
	float2x2 float2x2::RotationScaling(const float a, const float2 s)
	{
		float ca = cosf(a), sa = sinf(a);
		return float2x2(ca*s.x, -sa * s.x, sa*s.y, ca*s.y);
	}
	float float2x2::operator()(const int row, const int column) const
	{
		return mat[row][column];
	}
	float& float2x2::operator()(const int row, const int column)
	{
		return mat[row][column];
	}
	float2 float2x2::operator*(const float2 v) const
	{
		return float2(mat[0][0] * v.x + mat[0][1] * v.y, mat[1][0] * v.x + mat[1][1] * v.y);
	}
	float2x2::operator float3x3() const
	{
		return float3x3(
			mat[0][0], mat[0][1], 0.0f,
			mat[1][0], mat[1][1], 0.0f,
			0.0f, 0.0f, 1.0f);
	}
	float2x2::operator float4x4() const
	{
		return float4x4(
			mat[0][0], mat[0][1], 0.0f, 0.0f,
			mat[1][0], mat[1][1], 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float float2x2::Determinant() const
	{
		return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
	}
	void float2x2::Invert()
	{
		*this = Inverse();
	}
	float2x2 float2x2::Inverse() const
	{
		float det = Determinant();
		return float2x2(
			mat[1][1] / det, mat[0][1] / -det,
			mat[1][0] / -det, mat[0][0] / det);
	}
	void float2x2::Transpose()
	{
		float tmp = mat[0][1]; mat[0][1] = mat[1][0]; mat[1][0] = tmp;
	}
	float2x2 float2x2::Trasposed() const
	{
		float2x2 m(this);
		m.Transpose();
		return m;
	}
	float2x2 float2x2::operator*(const float2x2& m) const
	{
		return float2x2(
			mat[0][0] * m(0, 0) + mat[0][1] * m(1, 0), mat[0][0] * m(0, 1) + mat[0][1] * m(1, 1),
			mat[1][0] * m(0, 0) + mat[1][1] * m(1, 0), mat[1][0] * m(0, 1) + mat[1][1] * m(1, 1));
	}
	float2x2& float2x2::operator*=(const float2x2& m)
	{
		return *this = *this*m;
	}
	float2x2 float2x2::operator+(const float2x2& m) const
	{
		return float2x2(
			mat[0][0] + m(0, 0), mat[0][1] + m(0, 1),
			mat[1][0] + m(1, 0), mat[1][1] + m(1, 1));
	}
	float2x2& float2x2::operator+=(const float2x2& m)
	{
		return *this = *this + m;
	}
	float2x2 float2x2::operator-(const float2x2& m) const
	{
		return float2x2(
			mat[0][0] - m(0, 0), mat[0][1] - m(0, 1),
			mat[1][0] - m(1, 0), mat[1][1] - m(1, 1));
	}
	float2x2& float2x2::operator-=(const float2x2& m)
	{
		return *this = *this - m;
	}
	float2x2& float2x2::operator=(const float2x2& m)
	{
		memcpy(this, &m, sizeof(float2x2));		return *this;
	}
	float2x2 float2x2::operator+(const float f) const
	{
		return float2x2(mat[0][0] + f, mat[0][1] + f, mat[1][0] + f, mat[1][1] + f);
	}
	float2x2& float2x2::operator+=(const float f)
	{
		return *this = *this + f;
	}
	float2x2 float2x2::operator-(const float f) const
	{
		return float2x2(mat[0][0] - f, mat[0][1] - f, mat[1][0] - f, mat[1][1] - f);
	}
	float2x2& float2x2::operator-=(const float f)
	{
		return *this = *this - f;
	}
	float2x2 float2x2::operator*(const float f) const
	{
		return float2x2(mat[0][0] * f, mat[0][1] * f, mat[1][0] * f, mat[1][1] * f);
	}
	float2x2& float2x2::operator*=(const float f)
	{
		return *this = *this*f;
	}
	float2x2 float2x2::operator/(const float f) const
	{
		return float2x2(mat[0][0] / f, mat[0][1] / f, mat[1][0] / f, mat[1][1] / f);
	}
	float2x2& float2x2::operator/=(const float f)
	{
		return *this = *this / f;
	}
	float2x2& float2x2::operator=(const float f)
	{
		return *this = float2x2(f);
	}
	float2x2 float2x2::operator-() const
	{
		return float2x2(-mat[0][0], -mat[0][1], -mat[1][0], -mat[1][1]);
	}

	bool float2x2::operator==(float2x2 m)
	{
		return memcmp(this, &m, sizeof(float2x2)) == 0;
	}

	bool float2x2::isNear(float2x2 m, float eps)
	{
		return fabsf(mat[0][0] - m.mat[0][0]) < eps &&
			fabsf(mat[0][1] - m.mat[0][1]) < eps &&
			fabsf(mat[1][0] - m.mat[1][0]) < eps &&
			fabsf(mat[1][1] - m.mat[1][1]) < eps;
	}

#pragma endregion

#pragma region float3x3

	float3x3::float3x3()
	{
		memset(this, 0, sizeof(float3x3));
	}
	float3x3::float3x3(const float f)
	{
		mat[0][0] = f; mat[0][1] = f; mat[0][2] = f;
		mat[1][0] = f; mat[1][1] = f; mat[1][2] = f;
		mat[2][0] = f; mat[2][1] = f; mat[2][2] = f;
	}
	float3x3::float3x3(const float * const m)
	{
		memcpy(this, m, sizeof(float3x3));
	}
	float3x3::float3x3(
		const float _00, const float _01, const float _02,
		const float _10, const float _11, const float _12,
		const float _20, const float _21, const float _22)
	{
		mat[0][0] = _00; mat[0][1] = _01; mat[0][2] = _02;
		mat[1][0] = _10; mat[1][1] = _11; mat[1][2] = _12;
		mat[2][0] = _20; mat[2][1] = _21; mat[2][2] = _22;
	}
	float3x3::float3x3(const float3x3& m)
	{
		memcpy(this, &m, sizeof(float3x3));
	}
	float3x3::float3x3(const float3x3* m)
	{
		memcpy(this, m, sizeof(float3x3));
	}
	float3x3 float3x3::Identity()
	{
		return float3x3(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f);
	}
	float3x3 float3x3::Scaling2D(const float x, const float y)
	{
		return float3x3(
			x, 0.0f, 0.0f,
			0.0f, y, 0.0f,
			0.0f, 0.0f, 1.0f);
	}
	float3x3 float3x3::Scaling2D(const float2 s)
	{
		return float3x3(
			s.x, 0.0f, 0.0f,
			0.0f, s.y, 0.0f,
			0.0f, 0.0f, 1.0f);
	}
	float3x3 float3x3::Translation2D(const float x, const float y)
	{
		return float3x3(
			1.0f, 0.0f, x,
			0.0f, 1.0f, y,
			0.0f, 0.0f, 1.0f);
	}
	float3x3 float3x3::Translation2D(const float2 t)
	{
		return float3x3(
			1.0f, 0.0f, t.x,
			0.0f, 1.0f, t.y,
			0.0f, 0.0f, 1.0f);
	}
	float3x3 float3x3::Rotation2D(const float a)
	{
		float ca = cosf(a), sa = sinf(a);
		return float3x3(
			ca, -sa, 0.0f,
			sa, ca, 0.0f,
			0.0f, 0.0f, 1.0f);
	}
	float3x3 float3x3::Scaling(const float x, const float y, const float z)
	{
		return float3x3(
			x, 0.0f, 0.0f,
			0.0f, y, 0.0f,
			0.0f, 0.0f, z);
	}
	float3x3 float3x3::Scaling(const float3 s)
	{
		return float3x3(
			s.x, 0.0f, 0.0f,
			0.0f, s.y, 0.0f,
			0.0f, 0.0f, s.z);
	}
	float3x3 float3x3::RotationX(const float a)
	{
		float ca = cosf(a), sa = sinf(a);
		return float3x3(
			1.0f, 0.0f, 0.0f,
			0.0f, ca, -sa,
			0.0f, sa, ca);
	}
	float3x3 float3x3::RotationY(const float a)
	{
		float ca = cosf(a), sa = sinf(a);
		return float3x3(
			ca, 0.0f, sa,
			0.0f, 1.0f, 0.0f,
			-sa, 0.0f, ca);
	}
	float3x3 float3x3::RotationZ(const float a)
	{
		float ca = cosf(a), sa = sinf(a);
		return float3x3(
			ca, -sa, 0.0f,
			sa, ca, 0.0f,
			0.0f, 0.0f, 1.0f);
	}
	float3x3 float3x3::Rotation(const float pitch, const float yaw, const float roll)
	{
		float cp = cosf(pitch), sp = sinf(pitch), cy = cosf(yaw), sy = sinf(yaw), cr = cosf(roll), sr = sinf(roll);
		return float3x3(
			sy*sp*sr + cy * cr, sy*sp*cr - cy * sr, sy*cp,
			cp*sr, cp*cr, -sp,
			cy*sp*sr - sy * cr, cy*sp*cr + sy * sr, cy*cp);
	}
	float3x3 float3x3::Rotation(const float3 r)
	{
		float cp = cosf(r.x), sp = sinf(r.x), cy = cosf(r.y), sy = sinf(r.y), cr = cosf(r.z), sr = sinf(r.z);
		return float3x3(
			sy*sp*sr + cy * cr, sy*sp*cr - cy * sr, sy*cp,
			cp*sr, cp*cr, -sp,
			cy*sp*sr - sy * cr, cy*sp*cr + sy * sr, cy*cp);
	}
	float3x3 float3x3::RotationAxis(const float3 axis, const float a)
	{
		return RotationNormal(axis.Normalized(), a);
	}
	float3x3 float3x3::RotationNormal(const float3 n, const float a)
	{
		float ca = cosf(a), sa = sinf(a);
		return float3x3(
			ca + n.x*n.x*(1 - ca), n.x*n.y*(1 - ca) - n.z*sa, n.x*n.z*(1 - ca) + n.y*sa,
			n.y*n.x*(1 - ca) + n.z*sa, ca + n.y*n.y*(1 - ca), n.y*n.z*(1 - ca) - n.x*sa,
			n.z*n.x*(1 - ca) - n.y*sa, n.z*n.y*(1 - ca) + n.x*sa, ca + n.z*n.z*(1 - ca));
	}
	float float3x3::operator()(const int row, const int column) const
	{
		return mat[row][column];
	}
	float& float3x3::operator()(const int row, const int column)
	{
		return mat[row][column];
	}
	float3 float3x3::operator*(const float3 v) const
	{
		return float3(
			mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z,
			mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z,
			mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z);
	}
	float3x3::operator float2x2() const
	{
		return float2x2(
			mat[0][0], mat[0][1],
			mat[1][0], mat[1][1]);
	}
	float3x3::operator float4x4() const
	{
		return float4x4(
			mat[0][0], mat[0][1], mat[0][2], 0.0f,
			mat[1][0], mat[1][1], mat[1][2], 0.0f,
			mat[2][0], mat[2][1], mat[2][2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float float3x3::Determinant() const
	{
		return
			mat[0][0] * mat[1][1] * mat[2][2] +
			mat[0][1] * mat[1][2] * mat[2][0] +
			mat[0][2] * mat[1][0] * mat[2][1] -
			mat[0][2] * mat[1][1] * mat[2][0] -
			mat[0][0] * mat[1][2] * mat[2][1] -
			mat[0][1] * mat[1][0] * mat[2][2];
	}
	void float3x3::Invert()
	{
		*this = Inverse();
	}
	float3x3 float3x3::Inverse() const
	{
		float det = Determinant();
		return float3x3(
			float2x2(mat[1][1], mat[1][2], mat[2][1], mat[2][2]).Determinant() / det,
			float2x2(mat[0][1], mat[0][2], mat[2][1], mat[2][2]).Determinant() / -det,
			float2x2(mat[0][1], mat[0][2], mat[1][1], mat[1][2]).Determinant() / det,

			float2x2(mat[1][0], mat[1][2], mat[2][0], mat[2][2]).Determinant() / -det,
			float2x2(mat[0][0], mat[0][2], mat[2][0], mat[2][2]).Determinant() / det,
			float2x2(mat[0][0], mat[0][2], mat[1][0], mat[1][2]).Determinant() / -det,

			float2x2(mat[1][0], mat[1][1], mat[2][0], mat[2][1]).Determinant() / det,
			float2x2(mat[0][0], mat[0][1], mat[2][0], mat[2][1]).Determinant() / -det,
			float2x2(mat[0][0], mat[0][1], mat[1][0], mat[1][1]).Determinant() / det);
	}
	void float3x3::Transpose()
	{
		float tmp;
		tmp = mat[0][1]; mat[0][1] = mat[1][0]; mat[1][0] = tmp;
		tmp = mat[0][2]; mat[0][2] = mat[2][0]; mat[2][0] = tmp;
		tmp = mat[1][2]; mat[1][2] = mat[2][1]; mat[2][1] = tmp;
	}
	float3x3 float3x3::Trasposed() const
	{
		float3x3 m(this);
		m.Transpose();
		return m;
	}
	float3x3 float3x3::operator*(const float3x3& m) const
	{
		return float3x3(
			mat[0][0] * m(0, 0) + mat[0][1] * m(1, 0) + mat[0][2] * m(2, 0),
			mat[0][0] * m(0, 1) + mat[0][1] * m(1, 1) + mat[0][2] * m(2, 1),
			mat[0][0] * m(0, 2) + mat[0][1] * m(1, 2) + mat[0][2] * m(2, 2),
			mat[1][0] * m(0, 0) + mat[1][1] * m(1, 0) + mat[1][2] * m(2, 0),
			mat[1][0] * m(0, 1) + mat[1][1] * m(1, 1) + mat[1][2] * m(2, 1),
			mat[1][0] * m(0, 2) + mat[1][1] * m(1, 2) + mat[1][2] * m(2, 2),
			mat[2][0] * m(0, 0) + mat[2][1] * m(1, 0) + mat[2][2] * m(2, 0),
			mat[2][0] * m(0, 1) + mat[2][1] * m(1, 1) + mat[2][2] * m(2, 1),
			mat[2][0] * m(0, 2) + mat[2][1] * m(1, 2) + mat[2][2] * m(2, 2));
	}
	float3x3& float3x3::operator*=(const float3x3& m)
	{
		return *this = *this*m;
	}
	float3x3 float3x3::operator+(const float3x3& m) const
	{
		return float3x3(
			mat[0][0] + m(0, 0), mat[0][1] + m(0, 1), mat[0][2] + m(0, 2),
			mat[1][0] + m(1, 0), mat[1][1] + m(1, 1), mat[1][2] + m(1, 2),
			mat[2][0] + m(2, 0), mat[2][1] + m(2, 1), mat[2][2] + m(2, 2));
	}
	float3x3& float3x3::operator+=(const float3x3& m)
	{
		return *this = *this + m;
	}
	float3x3 float3x3::operator-(const float3x3& m) const
	{
		return float3x3(
			mat[0][0] - m(0, 0), mat[0][1] - m(0, 1), mat[0][2] - m(0, 2),
			mat[1][0] - m(1, 0), mat[1][1] - m(1, 1), mat[1][2] - m(1, 2),
			mat[2][0] - m(2, 0), mat[2][1] - m(2, 1), mat[2][2] - m(2, 2));
	}
	float3x3& float3x3::operator-=(const float3x3& m)
	{
		return *this = *this - m;
	}
	float3x3& float3x3::operator=(const float3x3& m)
	{
		memcpy(this, &m, sizeof(float3x3));		return *this;
	}
	float3x3 float3x3::operator+(const float f) const
	{
		return float3x3(
			mat[0][0] + f, mat[0][1] + f, mat[0][2] + f,
			mat[1][0] + f, mat[1][1] + f, mat[1][2] + f,
			mat[2][0] + f, mat[2][1] + f, mat[2][2] + f);
	}
	float3x3& float3x3::operator+=(const float f)
	{
		return *this = *this + f;
	}
	float3x3 float3x3::operator-(const float f) const
	{
		return float3x3(
			mat[0][0] - f, mat[0][1] - f, mat[0][2] - f,
			mat[1][0] - f, mat[1][1] - f, mat[1][2] - f,
			mat[2][0] - f, mat[2][1] - f, mat[2][2] - f);
	}
	float3x3& float3x3::operator-=(const float f)
	{
		return *this = *this - f;
	}
	float3x3 float3x3::operator*(const float f) const
	{
		return float3x3(
			mat[0][0] * f, mat[0][1] * f, mat[0][2] * f,
			mat[1][0] * f, mat[1][1] * f, mat[1][2] * f,
			mat[2][0] * f, mat[2][1] * f, mat[2][2] * f);
	}
	float3x3& float3x3::operator*=(const float f)
	{
		return *this = *this*f;
	}
	float3x3 float3x3::operator/(const float f) const
	{
		return float3x3(
			mat[0][0] / f, mat[0][1] / f, mat[0][2] / f,
			mat[1][0] / f, mat[1][1] / f, mat[1][2] / f,
			mat[2][0] / f, mat[2][1] / f, mat[2][2] / f);
	}
	float3x3& float3x3::operator/=(const float f)
	{
		return *this = *this / f;
	}
	float3x3& float3x3::operator=(const float f)
	{
		return *this = float3x3(f);
	}
	float3x3 float3x3::operator-() const
	{
		return float3x3(
			-mat[0][0], -mat[0][1], -mat[0][2],
			-mat[1][0], -mat[1][1], -mat[1][2],
			-mat[2][0], -mat[2][1], -mat[2][2]);
	}

	bool float3x3::operator==(float3x3 m)
	{
		return memcmp(this, &m, sizeof(float3x3)) == 0;
	}

	bool float3x3::isNear(float3x3 m, float eps)
	{
		return fabsf(mat[0][0] - m.mat[0][0]) < eps &&
			fabsf(mat[0][1] - m.mat[0][1]) < eps &&
			fabsf(mat[0][2] - m.mat[0][2]) < eps &&
			fabsf(mat[1][0] - m.mat[1][0]) < eps &&
			fabsf(mat[1][1] - m.mat[1][1]) < eps &&
			fabsf(mat[1][2] - m.mat[1][2]) < eps &&
			fabsf(mat[2][0] - m.mat[2][0]) < eps &&
			fabsf(mat[2][1] - m.mat[2][1]) < eps &&
			fabsf(mat[2][2] - m.mat[2][2]) < eps;
	}

#pragma endregion

#pragma region float4x4

	float4x4::float4x4()
	{
		memset(this, 0, sizeof(float4x4));
	}
	float4x4::float4x4(const float f)
	{
		mat[0][0] = f; mat[0][1] = f; mat[0][2] = f; mat[0][3] = f;
		mat[1][0] = f; mat[1][1] = f; mat[1][2] = f; mat[1][3] = f;
		mat[2][0] = f; mat[2][1] = f; mat[2][2] = f; mat[2][3] = f;
		mat[3][0] = f; mat[3][1] = f; mat[3][2] = f; mat[3][3] = f;
	}
	float4x4::float4x4(const float * const m)
	{
		memcpy(this, m, sizeof(float4x4));
	}
	float4x4::float4x4(
		const float _00, const float _01, const float _02, const float _03,
		const float _10, const float _11, const float _12, const float _13,
		const float _20, const float _21, const float _22, const float _23,
		const float _30, const float _31, const float _32, const float _33)
	{
		mat[0][0] = _00; mat[0][1] = _01; mat[0][2] = _02; mat[0][3] = _03;
		mat[1][0] = _10; mat[1][1] = _11; mat[1][2] = _12; mat[1][3] = _13;
		mat[2][0] = _20; mat[2][1] = _21; mat[2][2] = _22; mat[2][3] = _23;
		mat[3][0] = _30; mat[3][1] = _31; mat[3][2] = _32; mat[3][3] = _33;
	}
	float4x4::float4x4(const float4x4& m)
	{
		memcpy(this, &m, sizeof(float4x4));
	}
	float4x4::float4x4(const float4x4* m)
	{
		memcpy(this, m, sizeof(float4x4));
	}
	float4x4 float4x4::Identity()
	{
		return float4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::Scaling(const float x, const float y, const float z)
	{
		return float4x4(
			x, 0.0f, 0.0f, 0.0f,
			0.0f, y, 0.0f, 0.0f,
			0.0f, 0.0f, z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::Scaling(const float3 s)
	{
		return float4x4(
			s.x, 0.0f, 0.0f, 0.0f,
			0.0f, s.y, 0.0f, 0.0f,
			0.0f, 0.0f, s.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::Translation(const float x, const float y, const float z)
	{
		return float4x4(
			1.0f, 0.0f, 0.0f, x,
			0.0f, 1.0f, 0.0f, y,
			0.0f, 0.0f, 1.0f, z,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::Translation(const float3 t)
	{
		return float4x4(
			1.0f, 0.0f, 0.0f, t.x,
			0.0f, 1.0f, 0.0f, t.y,
			0.0f, 0.0f, 1.0f, t.z,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::RotationX(const float a)
	{
		float ca = cosf(a), sa = sinf(a);
		return float4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, ca, -sa, 0.0f,
			0.0f, sa, ca, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::RotationY(const float a)
	{
		float ca = cosf(a), sa = sinf(a);
		return float4x4(
			ca, 0.0f, sa, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sa, 0.0f, ca, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::RotationZ(const float a)
	{
		float ca = cosf(a), sa = sinf(a);
		return float4x4(
			ca, -sa, 0.0f, 0.0f,
			sa, ca, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::Rotation(const float pitch, const float yaw, const float roll)
	{
		float cp = cosf(pitch), sp = sinf(pitch), cy = cosf(yaw), sy = sinf(yaw), cr = cosf(roll), sr = sinf(roll);
		return float4x4(
			sy*sp*sr + cy * cr, sy*sp*cr - cy * sr, sy*cp, 0.0f,
			cp*sr, cp*cr, -sp, 0.0f,
			cy*sp*sr - sy * cr, cy*sp*cr + sy * sr, cy*cp, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::Rotation(const float3 r)
	{
		float cp = cosf(r.x), sp = sinf(r.x), cy = cosf(r.y), sy = sinf(r.y), cr = cosf(r.z), sr = sinf(r.z);
		return float4x4(
			sy*sp*sr + cy * cr, sy*sp*cr - cy * sr, sy*cp, 0.0f,
			cp*sr, cp*cr, -sp, 0.0f,
			cy*sp*sr - sy * cr, cy*sp*cr + sy * sr, cy*cp, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::RotationAxis(const float3 axis, const float a)
	{
		return RotationNormal(axis.Normalized(), a);
	}
	float4x4 float4x4::RotationNormal(const float3 n, const float a)
	{
		float ca = cosf(a), sa = sinf(a);
		return float4x4(
			ca + n.x*n.x*(1 - ca), n.x*n.y*(1 - ca) - n.z*sa, n.x*n.z*(1 - ca) + n.y*sa, 0.0f,
			n.y*n.x*(1 - ca) + n.z*sa, ca + n.y*n.y*(1 - ca), n.y*n.z*(1 - ca) - n.x*sa, 0.0f,
			n.z*n.x*(1 - ca) - n.y*sa, n.z*n.y*(1 - ca) + n.x*sa, ca + n.z*n.z*(1 - ca), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::ScalingRotationTranslation(const float sx, const float sy, const float sz, const float pitch, const float yaw, const float roll, const float tx, const float ty, const float tz)
	{
		float cpitch = cosf(pitch), spitch = sinf(pitch), cyaw = cosf(yaw), syaw = sinf(yaw), croll = cosf(roll), sroll = sinf(roll);
		return float4x4(
			(syaw*spitch*sroll + cyaw * croll)*sx, (syaw*spitch*croll - cyaw * sroll)*sy, (syaw*cpitch)*sz, tx,
			(cpitch*sroll)*sx, (cpitch*croll)*sy, (-spitch)*sz, ty,
			(cyaw*spitch*sroll - syaw * croll)*sx, (cyaw*spitch*croll + syaw * sroll)*sy, (cyaw*cpitch)*sz, tz,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::ScalingRotationTranslation(const float3 s, const float3 r, const float3 t)
	{
		float cpitch = cosf(r.x), spitch = sinf(r.x), cyaw = cosf(r.y), syaw = sinf(r.y), croll = cosf(r.z), sroll = sinf(r.z);
		return float4x4(
			(syaw*spitch*sroll + cyaw * croll)*s.x, (syaw*spitch*croll - cyaw * sroll)*s.y, (syaw*cpitch)*s.z, t.x,
			(cpitch*sroll)*s.x, (cpitch*croll)*s.y, (-spitch)*s.z, t.y,
			(cyaw*spitch*sroll - syaw * croll)*s.x, (cyaw*spitch*croll + syaw * sroll)*s.y, (cyaw*cpitch)*s.z, t.z,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::PerspectiveFOV(const float fov, const float screenAspect, const float screenNear, const float screenDepth)
	{
		float yScale = 1.0f / tanf(fov / 2.0f);
		float xScale = yScale / screenAspect;
		return float4x4(
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, screenDepth / (screenDepth - screenNear), -screenDepth * screenNear / (screenDepth - screenNear),
			0.0f, 0.0f, 1.0f, 0.0f);
	}
	float4x4 float4x4::Orthographic(const float viewWidth, const float viewHeight, const float screenNear, const float screenDepth)
	{
		return float4x4(
			2.0f / viewWidth, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / viewHeight, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (screenDepth - screenNear), -screenNear / (screenDepth - screenNear),
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::LookTo(const float3 eye, const float3 direction, const float3 up)
	{
		float3 zaxis = direction.Normalized();
		float3 xaxis = up.Cross(zaxis).Normalized();
		float3 yaxis = zaxis.Cross(xaxis);
		return float4x4(
			xaxis.x, xaxis.y, xaxis.z, -xaxis.Dot(eye),
			yaxis.x, yaxis.y, yaxis.z, -yaxis.Dot(eye),
			zaxis.x, zaxis.y, zaxis.z, -zaxis.Dot(eye),
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	float4x4 float4x4::LookAt(const float3 eye, const float3 focus, const float3 up)
	{
		return LookTo(eye, focus - eye, up);
	}
	float float4x4::operator()(const int row, const int column) const
	{
		return mat[row][column];
	}
	float& float4x4::operator()(const int row, const int column)
	{
		return mat[row][column];
	}
	float4 float4x4::operator*(const float4 v) const
	{
		return float4(
			mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z + mat[0][3] * v.w,
			mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z + mat[1][3] * v.w,
			mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z + mat[2][3] * v.w,
			mat[3][0] * v.x + mat[3][1] * v.y + mat[3][2] * v.z + mat[3][3] * v.w);
	}
	float4x4::operator float2x2() const
	{
		return float2x2(
			mat[0][0], mat[0][1],
			mat[1][0], mat[1][1]);
	}
	float4x4::operator float3x3() const
	{
		return float3x3(
			mat[0][0], mat[0][1], mat[0][2],
			mat[1][0], mat[1][1], mat[1][2],
			mat[2][0], mat[2][1], mat[2][2]);
	}
	float float4x4::Determinant() const
	{
		return
			float3x3(mat[1][1], mat[1][2], mat[1][3], mat[2][1], mat[2][2], mat[2][3], mat[3][1], mat[3][2], mat[3][3]).Determinant() * mat[0][0] -
			float3x3(mat[0][1], mat[0][2], mat[0][3], mat[2][1], mat[2][2], mat[2][3], mat[3][1], mat[3][2], mat[3][3]).Determinant() * mat[1][0] +
			float3x3(mat[0][1], mat[0][2], mat[0][3], mat[1][1], mat[1][2], mat[1][3], mat[3][1], mat[3][2], mat[3][3]).Determinant() * mat[2][0] -
			float3x3(mat[0][1], mat[0][2], mat[0][3], mat[1][1], mat[1][2], mat[1][3], mat[2][1], mat[2][2], mat[2][3]).Determinant() * mat[3][0];
	}
	void float4x4::Invert()
	{
		*this = Inverse();
	}
	float4x4 float4x4::Inverse() const
	{
		float det = Determinant();
		return float4x4(
			float3x3(mat[1][1], mat[1][2], mat[1][3], mat[2][1], mat[2][2], mat[2][3], mat[3][1], mat[3][2], mat[3][3]).Determinant() / det,
			float3x3(mat[0][1], mat[0][2], mat[0][3], mat[2][1], mat[2][2], mat[2][3], mat[3][1], mat[3][2], mat[3][3]).Determinant() / -det,
			float3x3(mat[0][1], mat[0][2], mat[0][3], mat[1][1], mat[1][2], mat[1][3], mat[3][1], mat[3][2], mat[3][3]).Determinant() / det,
			float3x3(mat[0][1], mat[0][2], mat[0][3], mat[1][1], mat[1][2], mat[1][3], mat[2][1], mat[2][2], mat[2][3]).Determinant() / -det,

			float3x3(mat[1][0], mat[1][2], mat[1][3], mat[2][0], mat[2][2], mat[2][3], mat[3][0], mat[3][2], mat[3][3]).Determinant() / -det,
			float3x3(mat[0][0], mat[0][2], mat[0][3], mat[2][0], mat[2][2], mat[2][3], mat[3][0], mat[3][2], mat[3][3]).Determinant() / det,
			float3x3(mat[0][0], mat[0][2], mat[0][3], mat[1][0], mat[1][2], mat[1][3], mat[3][0], mat[3][2], mat[3][3]).Determinant() / -det,
			float3x3(mat[0][0], mat[0][2], mat[0][3], mat[1][0], mat[1][2], mat[1][3], mat[2][0], mat[2][2], mat[2][3]).Determinant() / det,

			float3x3(mat[1][0], mat[1][1], mat[1][3], mat[2][0], mat[2][1], mat[2][3], mat[3][0], mat[3][1], mat[3][3]).Determinant() / det,
			float3x3(mat[0][0], mat[0][1], mat[0][3], mat[2][0], mat[2][1], mat[2][3], mat[3][0], mat[3][1], mat[3][3]).Determinant() / -det,
			float3x3(mat[0][0], mat[0][1], mat[0][3], mat[1][0], mat[1][1], mat[1][3], mat[3][0], mat[3][1], mat[3][3]).Determinant() / det,
			float3x3(mat[0][0], mat[0][1], mat[0][3], mat[1][0], mat[1][1], mat[1][3], mat[2][0], mat[2][1], mat[2][3]).Determinant() / -det,

			float3x3(mat[1][0], mat[1][1], mat[1][2], mat[2][0], mat[2][1], mat[2][2], mat[3][0], mat[3][1], mat[3][2]).Determinant() / -det,
			float3x3(mat[0][0], mat[0][1], mat[0][2], mat[2][0], mat[2][1], mat[2][2], mat[3][0], mat[3][1], mat[3][2]).Determinant() / det,
			float3x3(mat[0][0], mat[0][1], mat[0][2], mat[1][0], mat[1][1], mat[1][2], mat[3][0], mat[3][1], mat[3][2]).Determinant() / -det,
			float3x3(mat[0][0], mat[0][1], mat[0][2], mat[1][0], mat[1][1], mat[1][2], mat[2][0], mat[2][1], mat[2][2]).Determinant() / det);
	}
	void float4x4::Transpose()
	{
		float tmp;
		tmp = mat[0][1]; mat[0][1] = mat[1][0]; mat[1][0] = tmp;
		tmp = mat[0][2]; mat[0][2] = mat[2][0]; mat[2][0] = tmp;
		tmp = mat[0][3]; mat[0][3] = mat[3][0]; mat[3][0] = tmp;
		tmp = mat[1][2]; mat[1][2] = mat[2][1]; mat[2][1] = tmp;
		tmp = mat[1][3]; mat[1][3] = mat[3][1]; mat[3][1] = tmp;
		tmp = mat[2][3]; mat[2][3] = mat[3][2]; mat[3][2] = tmp;
	}
	float4x4 float4x4::Transposed() const
	{
		float4x4 m(this);
		m.Transpose();
		return m;
	}
	float4x4 float4x4::operator*(const float4x4& m) const
	{
		return float4x4(
			mat[0][0] * m(0, 0) + mat[0][1] * m(1, 0) + mat[0][2] * m(2, 0) + mat[0][3] * m(3, 0),
			mat[0][0] * m(0, 1) + mat[0][1] * m(1, 1) + mat[0][2] * m(2, 1) + mat[0][3] * m(3, 1),
			mat[0][0] * m(0, 2) + mat[0][1] * m(1, 2) + mat[0][2] * m(2, 2) + mat[0][3] * m(3, 2),
			mat[0][0] * m(0, 3) + mat[0][1] * m(1, 3) + mat[0][2] * m(2, 3) + mat[0][3] * m(3, 3),
			mat[1][0] * m(0, 0) + mat[1][1] * m(1, 0) + mat[1][2] * m(2, 0) + mat[1][3] * m(3, 0),
			mat[1][0] * m(0, 1) + mat[1][1] * m(1, 1) + mat[1][2] * m(2, 1) + mat[1][3] * m(3, 1),
			mat[1][0] * m(0, 2) + mat[1][1] * m(1, 2) + mat[1][2] * m(2, 2) + mat[1][3] * m(3, 2),
			mat[1][0] * m(0, 3) + mat[1][1] * m(1, 3) + mat[1][2] * m(2, 3) + mat[1][3] * m(3, 3),
			mat[2][0] * m(0, 0) + mat[2][1] * m(1, 0) + mat[2][2] * m(2, 0) + mat[2][3] * m(3, 0),
			mat[2][0] * m(0, 1) + mat[2][1] * m(1, 1) + mat[2][2] * m(2, 1) + mat[2][3] * m(3, 1),
			mat[2][0] * m(0, 2) + mat[2][1] * m(1, 2) + mat[2][2] * m(2, 2) + mat[2][3] * m(3, 2),
			mat[2][0] * m(0, 3) + mat[2][1] * m(1, 3) + mat[2][2] * m(2, 3) + mat[2][3] * m(3, 3),
			mat[3][0] * m(0, 0) + mat[3][1] * m(1, 0) + mat[3][2] * m(2, 0) + mat[3][3] * m(3, 0),
			mat[3][0] * m(0, 1) + mat[3][1] * m(1, 1) + mat[3][2] * m(2, 1) + mat[3][3] * m(3, 1),
			mat[3][0] * m(0, 2) + mat[3][1] * m(1, 2) + mat[3][2] * m(2, 2) + mat[3][3] * m(3, 2),
			mat[3][0] * m(0, 3) + mat[3][1] * m(1, 3) + mat[3][2] * m(2, 3) + mat[3][3] * m(3, 3));
	}
	float4x4& float4x4::operator*=(const float4x4& m)
	{
		return *this = *this*m;
	}
	float4x4 float4x4::operator+(const float4x4& m) const
	{
		return float4x4(
			mat[0][0] + m(0, 0), mat[0][1] + m(0, 1), mat[0][2] + m(0, 2), mat[0][3] + m(0, 3),
			mat[1][0] + m(1, 0), mat[1][1] + m(1, 1), mat[1][2] + m(1, 2), mat[1][3] + m(1, 3),
			mat[2][0] + m(2, 0), mat[2][1] + m(2, 1), mat[2][2] + m(2, 2), mat[2][3] + m(2, 3),
			mat[3][0] + m(3, 0), mat[3][1] + m(3, 1), mat[3][2] + m(3, 2), mat[3][3] + m(3, 3));
	}
	float4x4& float4x4::operator+=(const float4x4& m)
	{
		return *this = *this + m;
	}
	float4x4 float4x4::operator-(const float4x4& m) const
	{
		return float4x4(
			mat[0][0] - m(0, 0), mat[0][1] - m(0, 1), mat[0][2] - m(0, 2), mat[0][3] - m(0, 3),
			mat[1][0] - m(1, 0), mat[1][1] - m(1, 1), mat[1][2] - m(1, 2), mat[1][3] - m(1, 3),
			mat[2][0] - m(2, 0), mat[2][1] - m(2, 1), mat[2][2] - m(2, 2), mat[2][3] - m(2, 3),
			mat[3][0] - m(3, 0), mat[3][1] - m(3, 1), mat[3][2] - m(3, 2), mat[3][3] - m(3, 3));
	}
	float4x4& float4x4::operator-=(const float4x4& m)
	{
		return *this = *this - m;
	}
	float4x4& float4x4::operator=(const float4x4& m)
	{
		memcpy(this, &m, sizeof(float4x4));		return *this;
	}
	float4x4 float4x4::operator+(const float f) const
	{
		return float4x4(
			mat[0][0] + f, mat[0][1] + f, mat[0][2] + f, mat[0][3] + f,
			mat[1][0] + f, mat[1][1] + f, mat[1][2] + f, mat[1][3] + f,
			mat[2][0] + f, mat[2][1] + f, mat[2][2] + f, mat[2][3] + f,
			mat[3][0] + f, mat[3][1] + f, mat[3][2] + f, mat[3][3] + f);
	}
	float4x4& float4x4::operator+=(const float f)
	{
		return*this = *this + f;
	}
	float4x4 float4x4::operator-(const float f) const
	{
		return float4x4(
			mat[0][0] - f, mat[0][1] - f, mat[0][2] - f, mat[0][3] - f,
			mat[1][0] - f, mat[1][1] - f, mat[1][2] - f, mat[1][3] - f,
			mat[2][0] - f, mat[2][1] - f, mat[2][2] - f, mat[2][3] - f,
			mat[3][0] - f, mat[3][1] - f, mat[3][2] - f, mat[3][3] - f);
	}
	float4x4& float4x4::operator-=(const float f)
	{
		return *this = *this - f;
	}
	float4x4 float4x4::operator*(const float f) const
	{
		return float4x4(
			mat[0][0] * f, mat[0][1] * f, mat[0][2] * f, mat[0][3] * f,
			mat[1][0] * f, mat[1][1] * f, mat[1][2] * f, mat[1][3] * f,
			mat[2][0] * f, mat[2][1] * f, mat[2][2] * f, mat[2][3] * f,
			mat[3][0] * f, mat[3][1] * f, mat[3][2] * f, mat[3][3] * f);
	}
	float4x4& float4x4::operator*=(const float f)
	{
		return *this = *this*f;
	}
	float4x4 float4x4::operator/(const float f) const
	{
		return float4x4(
			mat[0][0] / f, mat[0][1] / f, mat[0][2] / f, mat[0][3] / f,
			mat[1][0] / f, mat[1][1] / f, mat[1][2] / f, mat[1][3] / f,
			mat[2][0] / f, mat[2][1] / f, mat[2][2] / f, mat[2][3] / f,
			mat[3][0] / f, mat[3][1] / f, mat[3][2] / f, mat[3][3] / f);
	}
	float4x4& float4x4::operator/=(const float f)
	{
		return *this = *this / f;
	}
	float4x4& float4x4::operator=(const float f)
	{
		return *this = float4x4(f);
	}
	float4x4 float4x4::operator-() const
	{
		return float4x4(
			-mat[0][0], -mat[0][1], -mat[0][2], -mat[0][3],
			-mat[1][0], -mat[1][1], -mat[1][2], -mat[1][3],
			-mat[2][0], -mat[2][1], -mat[2][2], -mat[2][3],
			-mat[3][0], -mat[3][1], -mat[3][2], -mat[3][3]);
	}

	bool float4x4::operator==(float4x4 m)
	{
		return memcmp(this, &m, sizeof(float4x4)) == 0;
	}

	bool float4x4::isNear(float4x4 m, float eps)
	{
		return fabsf(mat[0][0] - m.mat[0][0]) < eps &&
			fabsf(mat[0][1] - m.mat[0][1]) < eps &&
			fabsf(mat[0][2] - m.mat[0][2]) < eps &&
			fabsf(mat[0][3] - m.mat[0][3]) < eps &&
			fabsf(mat[1][0] - m.mat[1][0]) < eps &&
			fabsf(mat[1][1] - m.mat[1][1]) < eps &&
			fabsf(mat[1][2] - m.mat[1][2]) < eps &&
			fabsf(mat[1][3] - m.mat[1][3]) < eps &&
			fabsf(mat[2][0] - m.mat[2][0]) < eps &&
			fabsf(mat[2][1] - m.mat[2][1]) < eps &&
			fabsf(mat[2][2] - m.mat[2][2]) < eps &&
			fabsf(mat[2][3] - m.mat[2][3]) < eps &&
			fabsf(mat[3][0] - m.mat[3][0]) < eps &&
			fabsf(mat[3][1] - m.mat[3][1]) < eps &&
			fabsf(mat[3][2] - m.mat[3][2]) < eps &&
			fabsf(mat[3][3] - m.mat[3][3]) < eps;
	}

#pragma endregion

#pragma region Functions

	float2 operator+(float f, float2 v)
	{
		return v + f;
	}
	float3 operator+(float f, float3 v)
	{
		return v + f;
	}
	float4 operator+(float f, float4 v)
	{
		return v + f;
	}
	quaternion operator+(const float f, const quaternion q)
	{
		return q + f;
	}
	float2x2 operator+(float f, float2x2 m)
	{
		return m + f;
	}
	float3x3 operator+(float f, float3x3 m)
	{
		return m + f;
	}
	float4x4 operator+(float f, float4x4 m)
	{
		return m + f;
	}

	float2 operator-(float f, float2 v)
	{
		return float2(f) - v;
	}
	float3 operator-(float f, float3 v)
	{
		return float3(f) - v;
	}
	float4 operator-(float f, float4 v)
	{
		return float4(f) - v;
	}
	quaternion operator-(const float f, const quaternion q)
	{
		return quaternion(f) - q;
	}
	float2x2 operator-(float f, float2x2 m)
	{
		return float2x2(f) - m;
	}
	float3x3 operator-(float f, float3x3 m)
	{
		return float3x3(f) - m;
	}
	float4x4 operator-(float f, float4x4 m)
	{
		return float4x4(f) - m;
	}

	float2 operator*(float f, float2 v)
	{
		return v * f;
	}
	float3 operator*(float f, float3 v)
	{
		return v * f;
	}
	float4 operator*(float f, float4 v)
	{
		return v * f;
	}
	quaternion operator*(const float f, const quaternion q)
	{
		return q * f;
	}
	float2x2 operator*(float f, float2x2 m)
	{
		return m * f;
	}
	float3x3 operator*(float f, float3x3 m)
	{
		return m * f;
	}
	float4x4 operator*(float f, float4x4 m)
	{
		return m * f;
	}

#pragma endregion


}