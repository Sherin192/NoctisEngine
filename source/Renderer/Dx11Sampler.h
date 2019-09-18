#ifndef DX11_SAMPLER_H
#define DX11_SAMPLER_H

#include <wrl.h>

namespace noctis::rdr
{
	class RenderDevice;

	template <typename Derived, typename... Bases>
	struct is_derived_from : std::bool_constant<(std::is_base_of_v<Bases, Derived>&& ...)> {};
	template <typename Derived, typename... Bases>
	constexpr bool is_derived_from_v = is_derived_from<Derived, Bases...>::value;

	template <typename... Args>
	constexpr auto NoctisToAPISampler(SamplerType<Args...> type)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
		//Currently fixed 
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if (std::is_base_of_v<FilterPoint, decltype(type)>)		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		else if (std::is_base_of_v<FilterLinear, decltype(type)>)		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		else if (std::is_base_of_v<FilterAnisotropic, decltype(type)>)	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		else if (is_derived_from_v<decltype(type), FilterMinPoint, FilterMagPoint, FilterMipLinear>)	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		else if (is_derived_from_v<decltype(type), FilterMinPoint, FilterMagLinear, FilterMipPoint>)	samplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
		else if (is_derived_from_v<decltype(type), FilterMinPoint, FilterMagLinear, FilterMipLinear>) samplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		else if (is_derived_from_v<decltype(type), FilterMinLinear, FilterMagPoint, FilterMipPoint>)	samplerDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
		else if (is_derived_from_v<decltype(type), FilterMinLinear, FilterMagPoint, FilterMipLinear>) samplerDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		else if (is_derived_from_v<decltype(type), FilterMinLinear, FilterMagLinear, FilterMipPoint>) samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		//else assert(0);//Unsupported Filter type


		if (std::is_base_of_v<AddressUWrap, decltype(type)>)		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		else if (std::is_base_of_v<AddressUClamp, decltype(type)>)		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		else if (std::is_base_of_v<AddressUMirror, decltype(type)>)		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		else if (std::is_base_of_v<AddressUBorder, decltype(type)>)		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		else if (std::is_base_of_v<AddressUMirrorOnce, decltype(type)>)	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;

		if (std::is_base_of_v<AddressVWrap, decltype(type)>)		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		else if (std::is_base_of_v<AddressVClamp, decltype(type)>)		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		else if (std::is_base_of_v<AddressVMirror, decltype(type)>)		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		else if (std::is_base_of_v<AddressVBorder, decltype(type)>)		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		else if (std::is_base_of_v<AddressVMirrorOnce, decltype(type)>)	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;

		if (std::is_base_of_v<AddressWWrap, decltype(type)>)		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		else if (std::is_base_of_v<AddressWClamp, decltype(type)>)		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		else if (std::is_base_of_v<AddressWMirror, decltype(type)>)		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		else if (std::is_base_of_v<AddressWBorder, decltype(type)>)		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		else if (std::is_base_of_v<AddressWMirrorOnce, decltype(type)>)	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;

		if (std::is_base_of_v<CompareNever, decltype(type)>)		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		else if (std::is_base_of_v<CompareLess, decltype(type)>)		samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
		else if (std::is_base_of_v<CompareEqual, decltype(type)>)		samplerDesc.ComparisonFunc = D3D11_COMPARISON_EQUAL;
		else if (std::is_base_of_v<CompareLessEqual, decltype(type)>)	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		else if (std::is_base_of_v<CompareGreater, decltype(type)>)		samplerDesc.ComparisonFunc = D3D11_COMPARISON_GREATER;
		else if (std::is_base_of_v<CompareNotEqual, decltype(type)>)	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NOT_EQUAL;
		else if (std::is_base_of_v<CompareGreaterEqual, decltype(type)>)samplerDesc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;
		else if (std::is_base_of_v<CompareAlways, decltype(type)>)		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

		static const float blackOpaque[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		static const float blackTransparent[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		static const float whiteOpaque[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		static const float whiteTransparent[4] = { 1.0f, 1.0f, 1.0f, 0.0f };

		if (std::is_base_of_v<BorderBlackOpaque, decltype(type)>)		std::copy(std::begin(blackOpaque), std::end(blackOpaque), std::begin(samplerDesc.BorderColor));
		else if (std::is_base_of_v<BorderBlackTransparent, decltype(type)>)	std::copy(std::begin(blackTransparent), std::end(blackTransparent), std::begin(samplerDesc.BorderColor));
		else if (std::is_base_of_v<BorderWhiteOpaque, decltype(type)>)		std::copy(std::begin(whiteOpaque), std::end(whiteOpaque), std::begin(samplerDesc.BorderColor));
		else if (std::is_base_of_v<BorderWhiteTransparent, decltype(type)>)	std::copy(std::begin(whiteTransparent), std::end(whiteTransparent), std::begin(samplerDesc.BorderColor));
		else	std::copy(std::begin(blackOpaque), std::end(blackOpaque), std::begin(samplerDesc.BorderColor));

		return samplerDesc;
	}

	template <typename Derived>
	class Dx11Sampler
	{
	public:
		Dx11Sampler(std::shared_ptr<RenderDevice>& renderDevice, D3D11_SAMPLER_DESC desc) : m_desc(desc)
		{
			auto hResult = renderDevice->GetDevice()->CreateSamplerState(&desc, m_pSampler.GetAddressOf());
			if (FAILED(hResult)) printf("Failed to create a sampler state\n"); //TODO: Replace when logging is implemented.
		}

		auto& GetDx11Sampler() { return m_pSampler; }
		//Just a test
		//auto GetSamplerDesc() { return Dx11ToNoctisSampler; }
	private:
		D3D11_SAMPLER_DESC m_desc;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>				m_pSampler;
	};

}

#endif //DX11_SAMPLER_H