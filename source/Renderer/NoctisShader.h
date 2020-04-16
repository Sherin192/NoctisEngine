#ifndef NOCTIS_SHADER_H
#define NOCTIS_SHADER_H

#if defined(_WIN32)
#include "Dx11Shader.h"
#endif
#include "InputLayout.h"

constexpr uint8_t MAX_LAYOUT_ELEMENTS = 5;

namespace noctis::rdr
{
	class RenderDevice;
	template <typename ShaderType>
	class Dx11Shader;


	using ShaderID = uint16_t;

	struct D3D11_BUFFER_DESC;

	enum CBUFFERTYPE
	{
		CB_PER_OBJECT,
		CB_PER_FRAME
	};

	//TODO: implement base class selection based on a constexpr value/function 
	//interface of sort : class Shader : publilc TMP_func::type -> return API base class implementation
	class VertexShader : public Dx11Shader<VertexShader>
	{
	public:
		using Base = Dx11Shader<VertexShader>;
		using type = VertexShader;
		VertexShader(std::shared_ptr<RenderDevice>& renderDevice, std::wstring&& source) : Base(renderDevice, std::forward<std::wstring&&>(source)) {}

		VertexElementLayout const* const GetInputLyout() const;
		void SetInputLyout(std::initializer_list<VertexElement>&& layout);
		void BindInputLayout(std::shared_ptr<RenderDevice>& renderDevice);

	private:
		std::unique_ptr<VertexElementLayout> m_layout;
	};


	class PixelShader : public Dx11Shader<PixelShader>
	{
	public:
	using Base = Dx11Shader<PixelShader>;
		using type = PixelShader;

		PixelShader(std::shared_ptr<RenderDevice>& renderDevice, std::wstring&& source) : Base(renderDevice, std::forward<std::wstring&&>(source)) {}
	};

	class ComputeShader : public Dx11Shader<ComputeShader>
	{
	public:
		using Base = Dx11Shader<ComputeShader>;
		using type = ComputeShader;

		ComputeShader(std::shared_ptr<RenderDevice>& renderDevice, std::wstring&& source) : Base(renderDevice, std::forward<std::wstring&&>(source)) {}
	};
}
#endif //NOCTIS_SHADER_H