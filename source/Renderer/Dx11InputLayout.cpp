#include "Core_pch.h"
#include "NoctisRenderDevice.h"
#include "InputLayout.h"
#include "NoctisShader.h"

namespace noctis::rdr
{
	static constexpr const char* kSemanticPosition = "POSITION";
	static constexpr const char* kSemanticNormal = "NORMAL";
	static constexpr const char* kSemanticTangent = "TANGENT";
	static constexpr const char* kSemanticBitangent = "BITANGENT";
	static constexpr const char* kSemanticTexCoord = "TEXCOORD";





	static constexpr const char* GetSemanticName(VertexElement& elem)
	{
		switch (elem.type)
		{
		case VertexElement::kPosition:
			return kSemanticPosition;
			break;
		case VertexElement::kNormal:
			return kSemanticNormal;
			break;
		case VertexElement::kTangent:
			return kSemanticTangent;
			break;
		case VertexElement::kBitangent:
			return kSemanticBitangent;
			break;
		case VertexElement::kTexCoord:
			return kSemanticTexCoord;
			break;
		default:
			//TODO: Handle the default case to be safe.
			return nullptr;
		}
	}






	static constexpr DXGI_FORMAT GetDx11Format(VertexElement& elem)
	{
		switch (elem.format)
		{
		case VertexElement::Format::kFloat4:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case VertexElement::Format::kFloat3:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case VertexElement::Format::kFloat2:
			return DXGI_FORMAT_R32G32_FLOAT;
		case VertexElement::Format::kFloat1:
			return DXGI_FORMAT_R32_FLOAT;
		case VertexElement::Format::kHFloat4:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case VertexElement::Format::kUInt4:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		case VertexElement::Format::kUInt3:
			return DXGI_FORMAT_R32G32B32_UINT;
		case VertexElement::Format::kUInt2:
			return DXGI_FORMAT_R32G32_UINT;
		case VertexElement::Format::kUInt1:
			return DXGI_FORMAT_R32_UINT;
		case VertexElement::Format::kUNormShort4:
			return DXGI_FORMAT_R16G16B16A16_UNORM;
		default:
			//TODO: Handle the default case to be safe.
			return DXGI_FORMAT_FORCE_UINT;

		}
	}





	//==================================================================================================
	//	Dx11VertexElementLayout :	Concrete implementation of VertexElementLayout for Dx11.
	//==================================================================================================
	class Dx11VertexElementLayout : public VertexElementLayout
	{
	public:
		void Bind(std::shared_ptr<RenderDevice>& renderDevice, VertexShader& shader);
		void Bind(std::shared_ptr<RenderDevice>& renderDevice);

		Dx11VertexElementLayout(std::initializer_list<VertexElement>&& elements)
			: VertexElementLayout(std::forward<std::initializer_list<VertexElement>>(elements))
			, layout()
			, nElements()
		{}


	private:
		void Create(std::shared_ptr<RenderDevice>& renderDevice, VertexShader& shader);


		D3D11_INPUT_ELEMENT_DESC						layout[kMaxLayoutElements];
		size_t											nElements;

		Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_pVertexLayout;
	};





	//-----------------------------------------------------------------------------
	// VertexElementLayout::Factory::Create :
	//-----------------------------------------------------------------------------
	std::unique_ptr<VertexElementLayout> VertexElementLayout::Factory::Create(std::initializer_list<VertexElement>&& elements)
	{
		return std::make_unique<Dx11VertexElementLayout>(std::forward<std::initializer_list<VertexElement>>(elements));
	}





	//-----------------------------------------------------------------------------
	// Dx11VertexElementLayout::Create : Creates ID3D11InputLayout.
	//-----------------------------------------------------------------------------
	void Dx11VertexElementLayout::Create(std::shared_ptr<RenderDevice>& renderDevice, VertexShader& shader)
	{
		for (int i = 0; i < m_elements.size(); ++i)
		{
			layout[i].SemanticName = GetSemanticName(m_elements[i]);
			layout[i].SemanticIndex = m_elements[i].semanticIndex;
			layout[i].Format = GetDx11Format(m_elements[i]);
			layout[i].InputSlot = 0;
			layout[i].AlignedByteOffset = i == 0 ? 0 : m_elements[i].offset == autoOffset ? D3D11_APPEND_ALIGNED_ELEMENT : m_elements[i].offset;
			layout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			layout[i].InstanceDataStepRate = 0;

			stride += FormatToSize(m_elements[i]);
		}
		nElements = m_elements.size();
		if (renderDevice)
		{
			HRESULT hResult;
			auto* byteCode = shader.GetByteCode();
			hResult = renderDevice->GetDevice()->CreateInputLayout(layout, nElements, byteCode->GetBufferPointer(), byteCode->GetBufferSize(), &m_pVertexLayout);
			if (FAILED(hResult))
				std::cerr << "Failed to create input layout" << std::endl; //TODO: Change to a proper logging system.
		}
	}





	//-----------------------------------------------------------------------------
	// Dx11VertexElementLayout::Bind :
	//-----------------------------------------------------------------------------
	void Dx11VertexElementLayout::Bind(std::shared_ptr<RenderDevice>& renderDevice, VertexShader& shader)
	{

		if (!m_pVertexLayout)
			Create(renderDevice, shader);

		renderDevice->GetDeviceContext()->IASetInputLayout(m_pVertexLayout.Get());
	}




	//-----------------------------------------------------------------------------
	// Dx11VertexElementLayout::Bind :
	//-----------------------------------------------------------------------------
	void Dx11VertexElementLayout::Bind(std::shared_ptr<RenderDevice>& renderDevice)
	{
		renderDevice->GetDeviceContext()->IASetInputLayout(m_pVertexLayout.Get());
	}
}