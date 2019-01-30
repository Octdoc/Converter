#include "position.h"

namespace mth
{
	Position::Position()
	{
		Reset();
	}
	void Position::MoveForward(float d)
	{
		position.x += sinf(rotation.y)*d;
		position.z += cosf(rotation.y)*d;
	}
	void Position::MoveBackward(float d)
	{
		position.x -= sinf(rotation.y)*d;
		position.z -= cosf(rotation.y)*d;
	}
	void Position::MoveRight(float d)
	{
		position.x += cosf(rotation.y)*d;
		position.z -= sinf(rotation.y)*d;
	}
	void Position::MoveLeft(float d)
	{
		position.x -= cosf(rotation.y)*d;
		position.z += sinf(rotation.y)*d;
	}
	void Position::MoveUp(float d)
	{
		position.y += d;
	}
	void Position::MoveDown(float d)
	{
		position.y -= d;
	}
	void Position::Move(float3 delta)
	{
		position += delta;
	}
	void Position::MoveInLookDirection(float d)
	{
		MoveInLookDirection({ 0.0f, 0.0f, d });
	}
	void Position::MoveInLookDirection(float3 delta)
	{
		position += GetRotationMatrix()*delta;
	}
	void Position::LookDown(float r)
	{
		rotation.x += r;
	}
	void Position::LookUp(float r)
	{
		rotation.x -= r;
	}
	void Position::TurnRight(float r)
	{
		rotation.y += r;
	}
	void Position::TurnLeft(float r)
	{
		rotation.y -= r;
	}
	void Position::RollRight(float r)
	{
		rotation.z -= r;
	}
	void Position::RollLeft(float r)
	{
		rotation.z += r;
	}
	void Position::ScaleX(float s)
	{
		scale.x *= s;
	}
	void Position::ScaleY(float s)
	{
		scale.y *= s;
	}
	void Position::ScaleZ(float s)
	{
		scale.z *= s;
	}
	float4x4 Position::GetPositionMatrix()
	{
		return float4x4::Translation(position);
	}
	float4x4 Position::GetRotationMatrix()
	{
		return float4x4::Rotation(rotation);
	}
	float4x4 Position::GetScaleMatrix()
	{
		return float4x4::Scaling(scale);
	}
	float4x4 Position::GetWorldMatrix()
	{
		return float4x4::ScalingRotationTranslation(scale, rotation, position);
	}
	void Position::Reset()
	{
		position = { 0.0f, 0.0f, 0.0f };
		rotation = { 0.0f, 0.0f, 0.0f };
		scale = { 1.0f, 1.0f, 1.0f };
	}
	float Position::DistanceSquare(Position& other)
	{
		float x = position.x - other.position.x,
			y = position.y - other.position.y,
			z = position.z - other.position.z;
		return x * x + y * y + z * z;
	}
	float Position::Distance(Position& other)
	{
		return sqrtf(DistanceSquare(other));
	}
}