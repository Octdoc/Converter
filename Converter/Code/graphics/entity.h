#pragma once

#include "model.h"
#include "material.h"
#include "math/position.h"

namespace gfx
{
	class Entity :public mth::Position
	{
		SMART_PTR(Entity)

	private:
		Model::P m_model;
		mth::float4 m_color;
		std::vector<Material::P> m_materials;

	public:
		Entity(Model::P model);
		Entity(Model::P model, Material::P materials[]);
		Entity(Model::P model, mth::float4 color);
		Entity(Model::P model, Material::P materials[], mth::float4 color);

		void Render(Graphics& graphics);

		inline mth::float4 getColor() const { return m_color; }
		inline void setColor(mth::float4 color) { m_color = color; }
		inline Model::P getModel() { return m_model; }
	};
}