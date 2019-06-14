#include "scene.h"
#include <map>

extern std::wstring g_ExeFolder;

namespace cvt
{
	int Scene::ModelTypeToIndex(UINT modelType)
	{
		switch (modelType)
		{
		case gfx::ModelType::P:
			return 0;
		case gfx::ModelType::PT:
			return 1;
		case gfx::ModelType::PN:
			return 2;
		case gfx::ModelType::PTN:
			return 3;
		case gfx::ModelType::PM:
			return 4;
		case gfx::ModelType::PTM:
			return 5;
		}
		return -1;
	}
	Scene::Scene(gfx::Graphics& graphics) :
		m_graphics(graphics),
		m_camController(m_cam),
		m_showHitbox(true)
	{
		m_vs[0] = gfx::VertexShader::U(new gfx::VertexShader(m_graphics, ResolveFilename(L"Shaders/vsP.cso").c_str(), gfx::ModelType::P));
		m_ps[0] = gfx::PixelShader::U(new gfx::PixelShader(m_graphics, ResolveFilename(L"Shaders/psP.cso").c_str()));
		m_vs[1] = gfx::VertexShader::U(new gfx::VertexShader(m_graphics, ResolveFilename(L"Shaders/vsPT.cso").c_str(), gfx::ModelType::PT));
		m_ps[1] = gfx::PixelShader::U(new gfx::PixelShader(m_graphics, ResolveFilename(L"Shaders/psPT.cso").c_str()));
		m_vs[2] = gfx::VertexShader::U(new gfx::VertexShader(m_graphics, ResolveFilename(L"Shaders/vsPN.cso").c_str(), gfx::ModelType::PN));
		m_ps[2] = gfx::PixelShader::U(new gfx::PixelShader(m_graphics, ResolveFilename(L"Shaders/psPN.cso").c_str()));
		m_vs[3] = gfx::VertexShader::U(new gfx::VertexShader(m_graphics, ResolveFilename(L"Shaders/vsPTN.cso").c_str(), gfx::ModelType::PTN));
		m_ps[3] = gfx::PixelShader::U(new gfx::PixelShader(m_graphics, ResolveFilename(L"Shaders/psPTN.cso").c_str()));
		m_vs[4] = gfx::VertexShader::U(new gfx::VertexShader(m_graphics, ResolveFilename(L"Shaders/vsPM.cso").c_str(), gfx::ModelType::PM));
		m_ps[4] = gfx::PixelShader::U(new gfx::PixelShader(m_graphics, ResolveFilename(L"Shaders/psPM.cso").c_str()));
		m_vs[5] = gfx::VertexShader::U(new gfx::VertexShader(m_graphics, ResolveFilename(L"Shaders/vsPTM.cso").c_str(), gfx::ModelType::PTM));
		m_ps[5] = gfx::PixelShader::U(new gfx::PixelShader(m_graphics, ResolveFilename(L"Shaders/psPTM.cso").c_str()));
		m_matrixBuffer = gfx::CBuffer::U(new gfx::CBuffer(m_graphics, sizeof(mth::matrix) * 2));
		m_lightBuffer = gfx::CBuffer::U(new gfx::CBuffer(m_graphics, sizeof(float) * 8));
		m_colorBuffer = gfx::CBuffer::U(new gfx::CBuffer(m_graphics, sizeof(float) * 4));
		m_sampler = gfx::SamplerState::U(new gfx::SamplerState(m_graphics));
		m_defaultTexture = std::make_shared<gfx::Texture>(m_graphics, ResolveFilename(L"Media/missing.png").c_str());
		m_defaultNormalmap = std::make_shared<gfx::Texture>(m_graphics, ResolveFilename(L"Media/normal.png").c_str());

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
		int shaderIndex = ModelTypeToIndex(ml.getModelType());
		gfx::VertexShader::P vs = m_vs[shaderIndex];
		gfx::PixelShader::P ps = m_ps[shaderIndex];
		vs->SetShaderToRender(m_graphics);
		ps->SetShaderToRender(m_graphics);
		allMaterials.resize(ml.getMaterialCount());
		std::map<std::wstring, gfx::Texture::P> textures;
		for (UINT i = 0; i < ml.getMaterialCount(); i++)
		{
			gfx::Texture::P tex, norm;
			if (ml.getTexture(i)[0])
			{
				if (textures.find(ml.getTexture(i)) == textures.end())
				{
					try
					{
						tex = std::make_shared<gfx::Texture>(m_graphics, (ml.getFolderName() + ml.getTexture(i)).c_str());
					}
					catch (std::exception e)
					{
						tex = m_defaultTexture;
					}
					textures[ml.getTexture(i)] = tex;
				}
				else
				{
					tex = textures[ml.getTexture(i)];
				}
			}
			if (ml.getNormalmap(i)[0])
			{
				if (textures.find(ml.getNormalmap(i)) == textures.end())
				{
					try
					{
						norm = std::make_shared<gfx::Texture>(m_graphics, (ml.getFolderName() + ml.getNormalmap(i)).c_str());
					}
					catch (std::exception e)
					{
						norm = m_defaultNormalmap;
					}
					textures[ml.getNormalmap(i)] = norm;
				}
				norm = textures[ml.getNormalmap(i)];
			}
			allMaterials[i] = std::make_shared<gfx::Material>(vs, ps, tex, norm);
		}

		usedMaterials.resize(ml.getVertexGroupCount());
		for (UINT i = 0; i < ml.getVertexGroupCount(); i++)
			usedMaterials[i] = allMaterials[ml.getVertexGroup(i).materialIndex];

		m_entity = gfx::Entity::U(new gfx::Entity(model, usedMaterials.data()));
	}
	void Scene::ClearEntity()
	{
		m_entity.reset();
	}

	void Scene::SetHitbox(gfx::ModelLoader& ml)
	{
		gfx::Model::P model = std::make_shared<gfx::Model>(m_graphics, ml);
		std::vector<gfx::Material::P> allMaterials, usedMaterials;
		int shaderIndex = ModelTypeToIndex(ml.getModelType());
		gfx::VertexShader::P vs = m_vs[shaderIndex];
		gfx::PixelShader::P ps = m_ps[shaderIndex];
		vs->SetShaderToRender(m_graphics);
		ps->SetShaderToRender(m_graphics);
		allMaterials.resize(ml.getMaterialCount());
		for (UINT i = 0; i < ml.getMaterialCount(); i++)
			allMaterials[i] = std::make_shared<gfx::Material>(vs, ps,
				ml.getTexture(i)[0] ? std::make_shared<gfx::Texture>(m_graphics, (ml.getFolderName() + ml.getTexture(i)).c_str()) : nullptr,
				ml.getNormalmap(i)[0] ? std::make_shared<gfx::Texture>(m_graphics, (ml.getFolderName() + ml.getNormalmap(i)).c_str()) : nullptr);

		usedMaterials.resize(ml.getVertexGroupCount());
		for (UINT i = 0; i < ml.getVertexGroupCount(); i++)
			usedMaterials[i] = allMaterials[ml.getVertexGroup(i).materialIndex];

		m_hitbox = gfx::Entity::U(new gfx::Entity(model, usedMaterials.data()));
	}
	void Scene::ClearHitbox()
	{
		m_hitbox.reset();
	}

	bool Scene::HandleCamera(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return m_camController.MessageHandler(msg, wparam, lparam);
	}

	void Scene::Render()
	{
		m_graphics.ClearScreen(0.1f, 0.1f, 0.2f);
		m_cam.Update();
		mth::matrix matrices[2];
		matrices[1] = m_cam.GetCameraMatrix();
		float lightBuffer[] = {
			1.0f, 1.0f, 1.0f, 1.0f,
			m_cam.position.x, m_cam.position.y, m_cam.position.z,
			0.5f
		};
		m_lightBuffer->WriteBuffer(m_graphics, lightBuffer);

		if (m_entity)
		{
			matrices[0] = m_entity->GetWorldMatrix();
			m_matrixBuffer->WriteBuffer(m_graphics, matrices);
			mth::float4 colorBuffer = m_entity->getColor();
			m_colorBuffer->WriteBuffer(m_graphics, &colorBuffer);

			m_graphics.RasterizerSolid();
			m_entity->Render(m_graphics);
		}

		if (m_showHitbox && m_hitbox)
		{
			matrices[0] = m_hitbox->GetWorldMatrix();
			m_matrixBuffer->WriteBuffer(m_graphics, matrices);
			mth::float4 colorBuffer = m_hitbox->getColor();
			m_colorBuffer->WriteBuffer(m_graphics, &colorBuffer);

			m_graphics.RasterizerWireframe();
			m_hitbox->Render(m_graphics);
		}

		m_graphics.Present();
	}
}