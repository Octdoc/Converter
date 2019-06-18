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
		gfx::Entity::U m_hitbox;
		gfx::VertexShader::P m_vs[6];
		gfx::PixelShader::P m_ps[6];
		gfx::CBuffer::U m_matrixBuffer;
		gfx::CBuffer::U m_lightBuffer;
		gfx::CBuffer::U m_colorBuffer;
		gfx::SamplerState::U m_sampler;

		gfx::Texture::P m_defaultTexture;
		gfx::Texture::P m_defaultNormalmap;

		gfx::Camera m_cam;
		mth::CamController m_camController;
		bool m_showHitbox;

	private:
		int ModelTypeToIndex(UINT modelType);

	public:
		Scene(gfx::Graphics& graphics);

		void SetEntityDefaultCube(gfx::ModelLoader& ml);
		void SetEntity(gfx::ModelLoader& ml);
		void ClearEntity();
		void SetHitbox(gfx::ModelLoader& ml);
		void ClearHitbox();
		inline void ShowHitbox(bool show) { m_showHitbox = show; }

		bool HandleCamera(UINT msg, WPARAM wparam, LPARAM lparam);
		void Render();
	};
}