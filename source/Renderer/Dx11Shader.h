#include "Core_pch.h"
#include "NoctisRenderDevice.h"
namespace noctis::rdr
{
//------------------------------------------------------------------------------------
//		Forward Declarations:
//------------------------------------------------------------------------------------
	class VertexShader;
	class PixelShader;

//====================================================================================





//------------------------------------------------------------------------------------
//		Helper meta functions: Returns the type of the derived shader.
//------------------------------------------------------------------------------------
	namespace traits {
		//Base case
		template <typename ShaderType> struct Dx11ShaderObject {};

		template <>	struct Dx11ShaderObject<VertexShader> { using type = ID3D11VertexShader; };

		template <> struct Dx11ShaderObject<PixelShader> { using type = ID3D11PixelShader; };

		template <typename ShaderType>
		using Dx11ShaderObject_t = typename Dx11ShaderObject<ShaderType>::type;
	}	//traits

//====================================================================================





//------------------------------------------------------------------------------------
//		Dx11Shader: Dx11 base class for all types of shaders.
//------------------------------------------------------------------------------------
	template <typename ShaderType>
	class Dx11Shader
	{
	protected:
		//C.35: A base class destructor should be either public and virtual, or protected and nonvirtual
		~Dx11Shader() = default;
		
		using ShaderObject = traits::Dx11ShaderObject_t<ShaderType>;
	public:

		Dx11Shader(std::shared_ptr<RenderDevice>& renderDevice, std::wstring& source);

		Dx11Shader(std::shared_ptr<RenderDevice>& renderDevice, std::wstring&& source);

		void Bind(std::shared_ptr<RenderDevice>& renderDevice);

		[[nodiscard]] auto* GetByteCode() const noexcept;
	private:
		void Init(std::shared_ptr<RenderDevice>& renderDevice);
		
		const std::filesystem::path								m_file;

		Microsoft::WRL::ComPtr<ID3DBlob>						m_pByteCode;
		Microsoft::WRL::ComPtr<ShaderObject>					m_pShaderObject;
	};

//====================================================================================





//------------------------------------------------------------------------------------
//		Dx11Shader Constructor:
//------------------------------------------------------------------------------------
	template <typename ShaderType>
	Dx11Shader<ShaderType>::Dx11Shader(std::shared_ptr<RenderDevice>& renderDevice, std::wstring& source)
		:m_file(source)
	{
		Init(renderDevice);
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		Dx11Shader Constructor:
//------------------------------------------------------------------------------------
	template <typename ShaderType>
	Dx11Shader<ShaderType>::Dx11Shader(std::shared_ptr<RenderDevice>& renderDevice, std::wstring&& source)
		:m_file(std::move(source))
	{
		Init(renderDevice);
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		Bind: Binds the shader based on the type of the derived class.
//------------------------------------------------------------------------------------
	template <typename ShaderType>
	void Dx11Shader<ShaderType>::Bind(std::shared_ptr<RenderDevice>& renderDevice)
	{
		ShaderType* shaderType = static_cast<ShaderType*>(this);

		if constexpr (std::is_same_v<VertexShader, std::decay_t<decltype(*shaderType)>>)
		{
			renderDevice->GetDeviceContext()->VSSetShader(m_pShaderObject.Get(), 0, 0);
		}
		else if constexpr (std::is_same_v<PixelShader, std::decay_t<decltype(*shaderType)>>)
		{
			renderDevice->GetDeviceContext()->PSSetShader(m_pShaderObject.Get(), 0, 0);
		}
	}

//====================================================================================
	
	
	
	
	
//------------------------------------------------------------------------------------
//		GetByteCode: Returns the byte code for the shader.
//------------------------------------------------------------------------------------
	template <typename ShaderType>
	[[nodiscard]] auto* Dx11Shader<ShaderType>::GetByteCode() const noexcept 
	{ 
		return m_pByteCode.Get(); 
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		Init: Initializes the shader based on the type of derived class.
//------------------------------------------------------------------------------------
	template <typename ShaderType>
	void Dx11Shader<ShaderType>::Init(std::shared_ptr<RenderDevice>& renderDevice)
	{
		ShaderType* shaderType = static_cast<ShaderType*>(this);
		HRESULT hResult;

		ID3D10Blob* errorMessages = nullptr;
		UINT flags = 0;
#if defined(_DEBUG)
		flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif //_DEBUG

		if constexpr (std::is_same_v<VertexShader, std::decay_t<decltype(*shaderType)>>)
		{
			hResult = D3DCompileFromFile(static_cast<LPCWSTR>(m_file.c_str()), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", flags, 0, &m_pByteCode, &errorMessages);
			if (FAILED(hResult))
				Log(LogLevel::Error, "Failed to compile Vertex Shader.\n", (char*)errorMessages->GetBufferPointer());

			hResult = renderDevice->GetDevice()->CreateVertexShader(m_pByteCode->GetBufferPointer(), m_pByteCode->GetBufferSize(), NULL, &m_pShaderObject);

		}
		else if constexpr (std::is_same_v<PixelShader, std::decay_t<decltype(*shaderType)>>)
		{
			hResult = D3DCompileFromFile(static_cast<LPCWSTR>(m_file.c_str()), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", flags, 0, &m_pByteCode, &errorMessages);
			if (FAILED(hResult))
				Log(LogLevel::Error, "Failed to compile Pixel Shader.\n", (char*)errorMessages->GetBufferPointer());
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

//====================================================================================
}