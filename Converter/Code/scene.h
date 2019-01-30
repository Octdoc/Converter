#pragma once

#include "graphics/entity.h"
#include "graphics/camera.h"
#include "math/camcontroller.h"
#include "graphics/samplerstate.h"

namespace cvt
{
	class Scene
	{
		gfx::Graphics& m_graphics;

		gfx::Entity::U m_entity;
		gfx::VertexShader::P m_vs[6];
		gfx::PixelShader::P m_ps[6];
		gfx::CBuffer::U m_matrixBuffer;
		gfx::CBuffer::U m_lightBuffer;
		gfx::CBuffer::U m_colorBuffer;
		gfx::SamplerState::U m_sampler;

		gfx::Camera m_cam;
		mth::CamController m_camController;

	public:
		Scene(gfx::Graphics& graphics);

		void SetEntity(gfx::ModelLoader& ml);
		void ClearEntity();

		bool HandleCamera(UINT msg, WPARAM wparam, LPARAM lparam);
		void Render();
	};
}