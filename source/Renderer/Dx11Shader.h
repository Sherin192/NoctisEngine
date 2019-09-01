#include "Core_pch.h"
#include "NoctisRenderDevice.h"
namespace noctis::rdr
{
	class VertexShader;
	class PixelShader;

	namespace traits {
		//Base case
		template <typename ShaderType> struct Dx11ShaderObject {};

		template <>	struct Dx11ShaderObject<VertexShader> { using type = ID3D11VertexShader; };

		template <> struct Dx11ShaderObject<PixelShader> { using type = ID3D11PixelShader; };

		template <typename ShaderType>
		using Dx11ShaderObject_t = typename Dx11ShaderObject<ShaderType>::type;
	}

	template <typename ShaderType>
	class Dx11Shader
	{
	public:
		using ShaderObject = traits::Dx11ShaderObject_t<ShaderType>;

		Dx11Shader(std::shared_ptr<RenderDevice>& renderDevice, std::wstring& source)
			:m_file(source)
		{
			Init(renderDevice);
		}
		Dx11Shader(std::shared_ptr<RenderDevice>& renderDevice, std::wstring&& source)
			:m_file(std::move(source))
		{
			Init(renderDevice);
		}

		void Bind(std::shared_ptr<RenderDevice>& renderDevice)
		{
			ShaderType* shaderType = static_cast<ShaderType*>(this);

			HRESULT hResult;
			if constexpr (std::is_same_v<VertexShader, std::decay_t<decltype(*shaderType)>>)
			{
				renderDevice->GetDeviceContext()->VSSetShader(m_pShaderObject.Get(), 0, 0);
			}
			else if constexpr (std::is_same_v<PixelShader, std::decay_t<decltype(*shaderType)>>)
			{
				renderDevice->GetDeviceContext()->PSSetShader(m_pShaderObject.Get(), 0, 0);
			}
		}

		[[nodiscard]] auto* GetByteCode() const noexcept { return m_pByteCode.Get(); }
	private:
		void Init(std::shared_ptr<RenderDevice>& renderDevice)
		{
			ShaderType* shaderType = static_cast<ShaderType*>(this);
			HRESULT hResult;

			ID3D10Blob* errorMessages = nullptr;
			if constexpr (std::is_same_v<VertexShader, std::decay_t<decltype(*shaderType)>>)
			{
				hResult = D3DCompileFromFile(static_cast<LPCWSTR>(m_file.c_str()), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", 0, 0, &m_pByteCode, &errorMessages);
				hResult = renderDevice->GetDevice()->CreateVertexShader(m_pByteCode->GetBufferPointer(), m_pByteCode->GetBufferSize(), NULL, &m_pShaderObject);

			}
			else if constexpr (std::is_same_v<PixelShader, std::decay_t<decltype(*shaderType)>>)
			{
				hResult = D3DCompileFromFile(static_cast<LPCWSTR>(m_file.c_str()), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", 0, 0, &m_pByteCode, &errorMessages);
				hResult = renderDevice->GetDevice()->CreatePixelShader(m_pByteCode->GetBufferPointer(), m_pByteCode->GetBufferSize(), NULL, &m_pShaderObject);

			}

			if (FAILED(hResult))
			{
				//TODO: LOG fix when proper logging is implemented.
				std::cerr << "Failed to compile Vertex Shader" << std::endl;
				char* errorMsg = (char*)errorMessages->GetBufferPointer();
				std::cerr << errorMsg;
			}


		};
	private:
		const std::filesystem::path								m_file;

		Microsoft::WRL::ComPtr<ID3DBlob>						m_pByteCode;
		Microsoft::WRL::ComPtr<ShaderObject>					m_pShaderObject;
	};
}