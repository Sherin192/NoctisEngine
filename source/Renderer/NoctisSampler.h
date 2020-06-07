#ifndef NOCTIS_SAMPLER_H
#define NOCTIS_SAMPLER_H
#if defined (WIN32)
#include "Dx11Sampler.h"
#endif

namespace noctis::rdr
{
	class Sampler : public Dx11Sampler<Sampler>
	{
		using Base = Dx11Sampler<Sampler>;
	public:
		template <typename... Args>
		Sampler(std::shared_ptr<RenderDevice>& renderDevice, SamplerType<Args...> type) 
			: Base(std::forward<std::shared_ptr<RenderDevice>&>(renderDevice), NoctisToAPISampler(type)) {}
	};

	struct SamplerPool
	{
		enum class SamplerTypeID : uint8_t {
			PointWrap,
			PointClamp,
			PointMirror,
			LinearWrap,
			LinearClamp,
			LinearMirror,
			AnisotropicWrap,
			AnisotropicClamp,
			AnisotropicMirror,
			Count
		};

		SamplerPool(std::shared_ptr<RenderDevice> renderDevice)
		{
			m_samplers = {	std::make_shared<Sampler>(renderDevice, SamplerPointWrapNever{}),
							std::make_shared<Sampler>(renderDevice, SamplerPointClampNever{}),
							std::make_shared<Sampler>(renderDevice, SamplerPointMirrorNever{}),
							std::make_shared<Sampler>(renderDevice, SamplerLinearWrapNever{}),
							std::make_shared<Sampler>(renderDevice, SamplerLinearClampNever{}),
							std::make_shared<Sampler>(renderDevice, SamplerLinearMirrorNever{}),
							std::make_shared<Sampler>(renderDevice, SamplerAnisotropicWrapNever{}),
							std::make_shared<Sampler>(renderDevice, SamplerAnisotropicClampNever{}),
							std::make_shared<Sampler>(renderDevice, SamplerAnisotropicMirrorNever{})
			};
		}
	
		auto GetSampler(SamplerTypeID id) const noexcept { return m_samplers[static_cast<uint8_t>(id)]; }
	private:
		std::array<std::shared_ptr<Sampler>, static_cast<uint8_t>(SamplerTypeID::Count)> m_samplers;
	};


	class DepthStencilState: public Dx11DeptStencilState<DepthStencilState>
	{
		using Base = Dx11DeptStencilState;
	public:
		template <typename DepthType, typename StencilType>
		DepthStencilState(std::shared_ptr<RenderDevice>& renderDevice, DepthStencilType<DepthType, StencilType> type, uint8_t stencilRef = 0xFF)
			: Base(std::forward<std::shared_ptr<RenderDevice>&>(renderDevice), NoctisToAPIDepthStencil(type), stencilRef) {}
	};
}
#endif //NOCTIS_SAMPLER_H
