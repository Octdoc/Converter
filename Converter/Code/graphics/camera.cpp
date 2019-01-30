#include "camera.h"

namespace gfx
{
	Camera::Camera(float screenAspect, float fov, float screenNear, float screenDepth)
	{
		m_screenAspect = screenAspect;
		m_screenNear = screenNear;
		m_screenDepth = screenDepth;
		m_fieldOfView = fov;
		m_projectionMatrix = mth::float4x4::PerspectiveFOV(m_fieldOfView, m_screenAspect, m_screenNear, m_screenDepth);
	}
	void Camera::SetFOV(float fov)
	{
		m_fieldOfView = fov;
		m_projectionMatrix = mth::float4x4::PerspectiveFOV(m_fieldOfView, m_screenAspect, m_screenNear, m_screenDepth);
	}
	float Camera::getFOV()
	{
		return m_fieldOfView;
	}
	void Camera::SetScreenAspect(float screenAspect)
	{
		m_screenAspect = screenAspect;
		m_projectionMatrix = mth::float4x4::PerspectiveFOV(m_fieldOfView, m_screenAspect, m_screenNear, m_screenDepth);
	}
	void Camera::Update()
	{
		mth::float3 up(0.0f, 1.0f, 0.0f), lookAt(0.0f, 0.0f, 1.0f);
		mth::float4x4 rotationMatrix;

		rotationMatrix = mth::float4x4::Rotation(rotation);

		lookAt = rotationMatrix * lookAt;
		up = rotationMatrix * up;

		lookAt += position;

		m_viewMatrix = mth::float4x4::LookAt(position, lookAt, up);
	}
	mth::float4x4 Camera::getViewMatrix()
	{
		return m_viewMatrix;
	}
	mth::float4x4 Camera::getProjectionMatrix()
	{
		return m_projectionMatrix;
	}
	mth::float4x4 Camera::GetCameraMatrix()
	{
		return m_projectionMatrix * m_viewMatrix;
	}
}