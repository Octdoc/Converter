#include "entity.h"

namespace gfx
{
	Entity::Entity(Model::P model) :
		m_model(model), m_color(1.0f) {}
	Entity::Entity(Model::P model, Material::P materials[]) :
		m_model(model), m_color(1.0f)
	{
		m_materials.reserve(m_model->getGroupCount());
		for (UINT i = 0; i < m_model->getGroupCount(); i++)
			m_materials.push_back(materials[i]);
	}
	Entity::Entity(Model::P model, mth::float4 color) :
		m_model(model), m_color(color) {}
	Entity::Entity(Model::P model, Material::P materials[], mth::float4 color) :
		m_model(model), m_color(color)
	{
		m_materials.reserve(m_model->getGroupCount());
		for (UINT i = 0; i < m_model->getGroupCount(); i++)
			m_materials.push_back(materials[i]);
	}

	void Entity::Render(Graphics& graphics)
	{
		m_model->SetBuffersToRender(graphics);
		for (UINT i = 0; i < (UINT)m_materials.size(); i++)
		{
			m_materials[i]->SetMaterialToRender(graphics);
			m_model->DrawGroup(graphics, i);
		}
	}
}