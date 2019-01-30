#include "camcontroller.h"

namespace mth
{
	void CamController::SetTargetPosition()
	{
		m_target.position = mth::float3x3::Rotation(m_target.rotation) * mth::float3(0.0f, 0.0f, -m_distance) - m_center;
	}
	CamController::CamController(Position& target) :
		m_target(target), m_sensitivity(0.008f), m_distance(10.0f), m_center()
	{
		m_prevMousePosition.x = 0;
		m_prevMousePosition.y = 0;
	}
	bool CamController::MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		POINT mouseDelta;
		switch (msg)
		{
		case WM_MOUSEMOVE:
			mouseDelta.x = LOWORD(lparam) - m_prevMousePosition.x;
			mouseDelta.y = HIWORD(lparam) - m_prevMousePosition.y;
			m_prevMousePosition.x = LOWORD(lparam);
			m_prevMousePosition.y = HIWORD(lparam);
			if (wparam & MK_RBUTTON)
			{
				m_center += mth::float3x3::Rotation(m_target.rotation) *
					mth::float3(mouseDelta.x*m_sensitivity*m_distance*0.1f,
						-mouseDelta.y * m_sensitivity*m_distance*0.1f, 0.0f);
				SetTargetPosition();
				return true;
			}
			else if (wparam & MK_LBUTTON)
			{
				m_target.LookUp(-mouseDelta.y * m_sensitivity);
				m_target.TurnRight(mouseDelta.x*m_sensitivity);
				SetTargetPosition();
				return true;
			}
			return false;
		case WM_MOUSEWHEEL:
			if (0 > GET_WHEEL_DELTA_WPARAM(wparam))
				m_distance *= 1.1f;
			else
				m_distance /= 1.1f;
			SetTargetPosition();
			return true;
		}
		return false;
	}
}
