#pragma once

#include "shaderbase.h"
#include "constantbuffer.h"

namespace gfx
{
	class VertexShader
	{
		SMART_PTR(VertexShader)
		NO_COPY(VertexShader)

	private:
		AutoReleasePtr<ID3D11VertexShader> m_vertexShader;
		AutoReleasePtr<ID3D11InputLayout> m_inputLayout;
		UINT m_modelType;

	private:
		void CreateVertexShader(Graphics& graphics, ID3DBlob* shaderBuffer);
		void CreateInputLayout(Graphics& graphics, ID3DBlob* shaderBuffer);

	public:
		VertexShader(Graphics& graphics, LPCWSTR shaderFileName, UINT modelType);
		VertexShader(Graphics& graphics, UINT modelType);

		void SetShaderToRender(Graphics& graphics);
		static void SetCBuffer(Graphics& graphics, CBuffer& buffer, UINT index = 0);
	};
}