#pragma once

#include <cmath>
#include <memory>

namespace mth
{
	const float pi = 3.1415926536f;

	class float2;
	class float3;
	class float4;

	class quaternion;

	class float2x2;
	class float3x3;
	class float4x4;
	typedef float4x4 matrix;

	class float2
	{
	public:
		float x;
		float y;

	public:
		float2();
		float2(const float f);
		float2(const float * const v);
		float2(const float x, const float y);
		float2(const float2& v);
		float2(const float2* v);
		bool isZeroVector() const;
		float Dot(const float2 v) const;
		float LengthSquare() const;
		float Length() const;
		void Abs();
		void Normalize();
		float2 Normalized() const;
		float2 operator+(const float2 v) const;
		float2 operator-(const float2 v) const;
		float2 operator*(const float2 v) const;
		float2 operator/(const float2 v) const;
		float2 operator+(const float f) const;
		float2 operator-(const float f) const;
		float2 operator*(const float f) const;
		float2 operator/(const float f) const;
		float2 operator-() const;
		float2& operator+=(const float2 v);
		float2& operator-=(const float2 v);
		float2& operator*=(const float2 v);
		float2& operator/=(const float2 v);
		float2& operator=(const float2 v);
		float2& operator+=(const float f);
		float2& operator-=(const float f);
		float2& operator*=(const float f);
		float2& operator/=(const float f);
		float2& operator=(const float f);
		float2 operator*(const float2x2& m) const;
		bool operator==(float2 v);
		bool isNear(float2 v, float eps);
		float operator()(const int i) const;
		float& operator()(const int i);
		operator float3() const;
		operator float4() const;
		operator float*() const;
	};

	class float3
	{
	public:
		float x;
		float y;
		float z;

	public:
		float3();
		float3(const float f);
		float3(const float * const v);
		float3(const float x, const float y, const float z);
		float3(const float3& v);
		float3(const float3* v);
		bool isZeroVector() const;
		float Dot(const float3 v) const;
		float3 Cross(const float3 v) const;
		float LengthSquare() const;
		float Length() const;
		void Abs();
		void Normalize();
		float3 Normalized() const;
		float3 operator+(const float3 v) const;
		float3 operator-(const float3 v) const;
		float3 operator*(const float3 v) const;
		float3 operator/(const float3 v) const;
		float3 operator+(const float f) const;
		float3 operator-(const float f) const;
		float3 operator*(const float f) const;
		float3 operator/(const float f) const;
		float3 operator-() const;
		float3& operator+=(const float3 v);
		float3& operator-=(const float3 v);
		float3& operator*=(const float3 v);
		float3& operator/=(const float3 v);
		float3& operator=(const float3 v);
		float3& operator+=(const float f);
		float3& operator-=(const float f);
		float3& operator*=(const float f);
		float3& operator/=(const float f);
		float3& operator=(const float f);
		float3 operator*(const float3x3& m) const;
		bool operator==(float3 v);
		bool isNear(float3 v, float eps);
		float operator()(const int i) const;
		float& operator()(const int i);
		operator float2() const;
		operator float4() const;
		operator float*() const;
	};

	class float4
	{
	public:
		float x;
		float y;
		float z;
		float w;

	public:
		float4();
		float4(const float f);
		float4(const float * const v);
		float4(const float x, const float y, const float z, const float w);
		float4(const float4& v);
		float4(const float4* v);
		bool isZeroVector() const;
		float Dot(const float4 v) const;
		float LengthSquare() const;
		float Length() const;
		void Abs();
		void Normalize();
		float4 Normalized() const;
		float4 operator+(const float4 v) const;
		float4 operator-(const float4 v) const;
		float4 operator*(const float4 v) const;
		float4 operator/(const float4 v) const;
		float4 operator+(const float f) const;
		float4 operator-(const float f) const;
		float4 operator*(const float f) const;
		float4 operator/(const float f) const;
		float4 operator-() const;
		float4& operator+=(const float4 v);
		float4& operator-=(const float4 v);
		float4& operator*=(const float4 v);
		float4& operator/=(const float4 v);
		float4& operator=(const float4 v);
		float4& operator+=(const float f);
		float4& operator-=(const float f);
		float4& operator*=(const float f);
		float4& operator/=(const float f);
		float4& operator=(const float f);
		float4 operator*(const float4x4& m) const;
		bool operator==(float4 v);
		bool isNear(float4 v, float eps);
		float operator()(const int i) const;
		float& operator()(const int i);
		operator float2() const;
		operator float3() const;
		operator float*() const;
	};

	class quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

	public:
		quaternion();
		quaternion(const float f);
		quaternion(const float * const q);
		quaternion(const float x, const float y, const float z, const float w);
		quaternion(const quaternion& q);
		quaternion(const quaternion* q);
		bool isZeroVector() const;
		quaternion Conjugate() const;
		quaternion Inverse();
		float LengthSquare() const;
		float Length() const;
		quaternion operator+(const quaternion q) const;
		quaternion operator-(const quaternion q) const;
		quaternion operator*(const quaternion q) const;
		quaternion operator+(const float f) const;
		quaternion operator-(const float f) const;
		quaternion operator*(const float f) const;
		quaternion operator/(const float f) const;
		quaternion operator-() const;
		quaternion& operator+=(const quaternion q);
		quaternion& operator-=(const quaternion q);
		quaternion& operator*=(const quaternion q);
		quaternion& operator=(const quaternion q);
		quaternion& operator+=(const float f);
		quaternion& operator-=(const float f);
		quaternion& operator*=(const float f);
		quaternion& operator/=(const float f);
		quaternion& operator=(const float f);
		bool operator==(quaternion q);
		bool operator==(float f);
		bool isNear(quaternion q, float eps);
		float operator()(const int i) const;
		float& operator()(const int i);
		operator float*() const;
	};

	class float2x2
	{
		float mat[2][2];

	public:
		float2x2();
		float2x2(const float f);
		float2x2(const float * const m);
		float2x2(const float _00, const float _01, const float _10, const float _11);
		float2x2(const float2x2& m);
		float2x2(const float2x2* m);
		static float2x2 Identity();
		static float2x2 Rotation(const float a);
		static float2x2 Scaling(const float x, const float y);
		static float2x2 Scaling(const float2 s);
		static float2x2 RotationScaling(const float a, const float x, const float y);
		static float2x2 RotationScaling(const float a, const float2 s);

		float operator()(const int row, const int column) const;
		float& operator()(const int row, const int column);
		float2 operator*(const float2 v) const;
		operator float3x3() const;
		operator float4x4() const;
		float Determinant() const;
		void Invert();
		float2x2 Inverse() const;
		void Transpose();
		float2x2 Trasposed() const;
		float2x2 operator*(const float2x2& m) const;
		float2x2& operator*=(const float2x2& m);
		float2x2 operator+(const float2x2& m) const;
		float2x2& operator+=(const float2x2& m);
		float2x2 operator-(const float2x2& m) const;
		float2x2& operator-=(const float2x2& m);
		float2x2& operator=(const float2x2& m);
		float2x2 operator+(const float f) const;
		float2x2& operator+=(const float f);
		float2x2 operator-(const float f) const;
		float2x2& operator-=(const float f);
		float2x2 operator*(const float f) const;
		float2x2& operator*=(const float f);
		float2x2 operator/(const float f) const;
		float2x2& operator/=(const float f);
		float2x2& operator=(const float f);
		float2x2 operator-() const;
		bool operator==(float2x2 m);
		bool isNear(float2x2 m, float eps);
	};

	class float3x3
	{
		float mat[3][3];

	public:
		float3x3();
		float3x3(const float f);
		float3x3(const float * const m);
		float3x3(const float _00, const float _01, const float _02,
			const float _10, const float _11, const float _12,
			const float _20, const float _21, const float _22);
		float3x3(const float3x3& m);
		float3x3(const float3x3* m);
		static float3x3 Identity();
		static float3x3 Scaling2D(const float x, const float y);
		static float3x3 Scaling2D(const float2 s);
		static float3x3 Translation2D(const float x, const float y);
		static float3x3 Translation2D(const float2 t);
		static float3x3 Rotation2D(const float a);
		static float3x3 Scaling(const float x, const float y, const float z);
		static float3x3 Scaling(const float3 s);
		static float3x3 RotationX(const float a);
		static float3x3 RotationY(const float a);
		static float3x3 RotationZ(const float a);
		static float3x3 Rotation(const float pitch, const float yaw, const float roll);
		static float3x3 Rotation(const float3 r);
		static float3x3 RotationAxis(const float3 axis, const float a);
		static float3x3 RotationNormal(const float3 n, const float a);

		float operator()(const int row, const int column) const;
		float& operator()(const int row, const int column);
		float3 operator*(const float3 v) const;
		operator float2x2() const;
		operator float4x4() const;
		float Determinant() const;
		void Invert();
		float3x3 Inverse() const;
		void Transpose();
		float3x3 Trasposed() const;
		float3x3 operator*(const float3x3& m) const;
		float3x3& operator*=(const float3x3& m);
		float3x3 operator+(const float3x3& m) const;
		float3x3& operator+=(const float3x3& m);
		float3x3 operator-(const float3x3& m) const;
		float3x3& operator-=(const float3x3& m);
		float3x3& operator=(const float3x3& m);
		float3x3 operator+(const float f) const;
		float3x3& operator+=(const float f);
		float3x3 operator-(const float f) const;
		float3x3& operator-=(const float f);
		float3x3 operator*(const float f) const;
		float3x3& operator*=(const float f);
		float3x3 operator/(const float f) const;
		float3x3& operator/=(const float f);
		float3x3& operator=(const float f);
		float3x3 operator-() const;
		bool operator==(float3x3 m);
		bool isNear(float3x3 m, float eps);
	};

	class float4x4
	{
		float mat[4][4];

	public:
		float4x4();
		float4x4(const float f);
		float4x4(const float * const m);
		float4x4(const float _00, const float _01, const float _02, const float _03,
			const float _10, const float _11, const float _12, const float _13,
			const float _20, const float _21, const float _22, const float _23,
			const float _30, const float _31, const float _32, const float _33);
		float4x4(const float4x4& m);
		float4x4(const float4x4* m);
		static float4x4 Identity();
		static float4x4 Scaling(const float x, const float y, const float z);
		static float4x4 Scaling(const float3 s);
		static float4x4 Translation(const float x, const float y, const float z);
		static float4x4 Translation(const float3 t);
		static float4x4 RotationX(const float a);
		static float4x4 RotationY(const float a);
		static float4x4 RotationZ(const float a);
		static float4x4 Rotation(const float pitch, const float yaw, const float roll);
		static float4x4 Rotation(const float3 r);
		static float4x4 RotationAxis(const float3 axis, const float a);
		static float4x4 RotationNormal(const float3 n, const float a);
		static float4x4 ScalingRotationTranslation(const float sx, const float sy, const float sz,
			const float pitch, const float yaw, const float roll,
			const float tx, const float ty, const float tz);
		static float4x4 ScalingRotationTranslation(const float3 s, const float3 r, const float3 t);
		static float4x4 PerspectiveFOV(const float fov, const float screenAspect, const float screenNear, const float screenDepth);
		static float4x4 Orthographic(const float viewWidth, const float viewHeight, const float screenNear, const float screenDepth);
		static float4x4 LookTo(const float3 eye, const float3 direction, const float3 up);
		static float4x4 LookAt(const float3 eye, const float3 focus, const float3 up);

		float operator()(const int row, const int column) const;
		float& operator()(const int row, const int column);
		float4 operator*(const float4 v) const;
		operator float2x2() const;
		operator float3x3() const;
		float Determinant() const;
		void Invert();
		float4x4 Inverse() const;
		void Transpose();
		float4x4 Transposed() const;
		float4x4 operator*(const float4x4& m)const;
		float4x4& operator*=(const float4x4& m);
		float4x4 operator+(const float4x4& m) const;
		float4x4& operator+=(const float4x4& m);
		float4x4 operator-(const float4x4& m) const;
		float4x4& operator-=(const float4x4& m);
		float4x4& operator=(const float4x4& m);
		float4x4 operator+(const float f) const;
		float4x4& operator+=(const float f);
		float4x4 operator-(const float f) const;
		float4x4& operator-=(const float f);
		float4x4 operator*(const float f) const;
		float4x4& operator*=(const float f);
		float4x4 operator/(const float f) const;
		float4x4& operator/=(const float f);
		float4x4& operator=(const float f);
		float4x4 operator-() const;
		bool operator==(float4x4 m);
		bool isNear(float4x4 m, float eps);
	};


	float2 operator+(const float f, const float2 v);
	float3 operator+(const float f, const float3 v);
	float4 operator+(const float f, const float4 v);
	quaternion operator+(const float f, const quaternion v);
	float2x2 operator+(const float f, const float2x2 m);
	float3x3 operator+(const float f, const float3x3 m);
	float4x4 operator+(const float f, const float4x4 m);

	float2 operator-(const float f, const float2 v);
	float3 operator-(const float f, const float3 v);
	float4 operator-(const float f, const float4 v);
	quaternion operator-(const float f, const quaternion q);
	float2x2 operator-(const float f, const float2x2 m);
	float3x3 operator-(const float f, const float3x3 m);
	float4x4 operator-(const float f, const float4x4 m);

	float2 operator*(const float f, const float2 v);
	float3 operator*(const float f, const float3 v);
	float4 operator*(const float f, const float4 v);
	quaternion operator*(const float f, const quaternion q);
	float2x2 operator*(const float f, const float2x2 m);
	float3x3 operator*(const float f, const float3x3 m);
	float4x4 operator*(const float f, const float4x4 m);

}