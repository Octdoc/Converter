#pragma once

#include "position.h"
#include"helpers.h"

namespace mth
{
	class CamController
	{
		Position& m_target;
		POINT m_prevMousePosition;
		float m_sensitivity;
		float m_distance;
		mth::float3 m_center;

	public:
		CamController(Position& target);
		void SetTargetPosition();
		bool MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam);
	};
}