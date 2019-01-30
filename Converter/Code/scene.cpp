#include "scene.h"

namespace cvt
{
	Scene::Scene(gfx::Graphics& graphics) :
		m_graphics(graphics),
		m_camController(m_cam)
	{
		m_vs[0] = gfx::VertexShader::U(new gfx::VertexShader(m_graphics, L"Shaders/vsP.cso", gfx::ShaderType::ToVertexLayout(gfx::ShaderType::P)));
		m_ps[0] = gfx::PixelShader::U(new gfx::PixelShader(m_graphics, L"Shaders/psP.cso"));
		m_vs[1] = gfx::VertexShader::U(new gfx::VertexShader(m_graphics, L"Shaders/vsPT.cso", gfx::ShaderType::ToVertexLayout(gfx::ShaderType::PT)));
		m_ps[1] = gfx::PixelShader::U(new gfx::PixelShader(m_graphics, L"Shaders/psPT.cso"));
		m_vs[2] = gfx::VertexShader::U(new gfx::VertexShader(m_graphics, L"Shaders/vsPN.cso", gfx::ShaderType::ToVertexLayout(gfx::ShaderType::PN)));
		m_ps[2] = gfx::PixelShader::U(new gfx::PixelShader(m_graphics, L"Shaders/psPN.cso"));
		m_vs[3] = gfx::VertexShader::U(new gfx::VertexShader(m_graphics, L"Shaders/vsPTN.cso", gfx::ShaderType::ToVertexLayout(gfx::ShaderType::PTN)));
		m_ps[3] = gfx::PixelShader::U(new gfx::PixelShader(m_graphics, L"Shaders/psPTN.cso"));
		m_vs[4] = gfx::VertexShader::U(new gfx::VertexShader(m_graphics, L"Shaders/vsPM.cso", gfx::ShaderType::ToVertexLayout(gfx::ShaderType::PM)));
		m_ps[4] = gfx::PixelShader::U(new gfx::PixelShader(m_graphics, L"Shaders/psPM.cso"));
		m_vs[5] = gfx::VertexShader::U(new gfx::VertexShader(m_graphics, L"Shaders/vsPTM.cso", gfx::ShaderType::ToVertexLayout(gfx::ShaderType::PTM)));
		m_ps[5] = gfx::PixelShader::U(new gfx::PixelShader(m_graphics, L"Shaders/psPTM.cso"));
		m_matrixBuffer = gfx::CBuffer::U(new gfx::CBuffer(m_graphics, sizeof(mth::matrix) * 2));
		m_lightBuffer = gfx::CBuffer::U(new gfx::CBuffer(m_graphics, sizeof(float) * 8));
		m_colorBuffer = gfx::CBuffer::U(new gfx::CBuffer(m_graphics, sizeof(float) * 4));
		m_sampler = gfx::SamplerState::U(new gfx::SamplerState(m_graphics));

		m_cam.SetScreenAspect(1000.0f / 720.0f);
		m_camController.SetTargetPosition();

		gfx::VertexShader::SetCBuffer(m_graphics, *m_matrixBuffer);
		gfx::PixelShader::SetCBuffer(m_graphics, *m_lightBuffer, 0);
		gfx::PixelShader::SetCBuffer(m_graphics, *m_colorBuffer, 1);
		m_sampler->SetSamplerState(m_graphics);
	}

	void Scene::SetEntity(gfx::ModelLoader& ml)
	{
		gfx::Model::P model = std::make_shared<gfx::Model>(m_graphics, ml);
		std::vector<gfx::Material::P> allMaterials, usedMaterials;
		gfx::VertexShader::P vs = m_vs[ml.getShaderType() - 1];
		gfx::PixelShader::P ps = m_ps[ml.getShaderType() - 1];
		vs->SetShaderToRender(m_graphics);
		ps->SetShaderToRender(m_graphics);
		allMaterials.resize(ml.getMaterialCount());
		for (UINT i = 0; i < ml.getMaterialCount(); i++)
			allMaterials[i] = std::make_shared<gfx::Material>(vs, ps,
				ml.getTexture(i) == L"" ? nullptr : std::make_shared<gfx::Texture>(m_graphics, (ml.getFolderName() + ml.getTexture(i)).c_str()),
				ml.getNormalmap(i) == L"" ? nullptr : std::make_shared<gfx::Texture>(m_graphics, (ml.getFolderName() + ml.getNormalmap(i)).c_str()));

		usedMaterials.resize(ml.getVertexGroupCount());
		for (UINT i = 0; i < ml.getVertexGroupCount(); i++)
			usedMaterials[i] = allMaterials[ml.getVertexGroup(i).materialIndex];

		m_entity = gfx::Entity::U(new gfx::Entity(model, usedMaterials.data()));
	}

	void Scene::ClearEntity()
	{
		m_entity.reset();
	}

	bool Scene::HandleCamera(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return m_camController.MessageHandler(msg, wparam, lparam);
	}

	void Scene::Render()
	{
		m_graphics.ClearScreen(0.1f, 0.1f, 0.2f);
		if (m_entity)
		{
			m_cam.Update();

			mth::matrix matrices[2];
			matrices[0] = m_entity->GetWorldMatrix();
			matrices[1] = m_cam.GetCameraMatrix();
			m_matrixBuffer->WriteBuffer(m_graphics, matrices);

			float lightBuffer[] = {
				1.0f, 1.0f, 1.0f, 1.0f,
				m_cam.position.x, m_cam.position.y, m_cam.position.z,
				0.5f
			};
			m_lightBuffer->WriteBuffer(m_graphics, lightBuffer);
			mth::float4 colorBuffer = m_entity->getColor();
			m_colorBuffer->WriteBuffer(m_graphics, &colorBuffer);

			m_entity->Render(m_graphics);
		}
		m_graphics.Present();
	}
}