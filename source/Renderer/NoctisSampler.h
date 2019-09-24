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
		Base& GetImpl() { return static_cast<Base&>(*this); }

		template <typename T>
		struct type_wrapper
		{
			using type = T;
		};
	public:
		template <typename... Args>
		Sampler(std::shared_ptr<RenderDevice>& renderDevice, SamplerType<Args...> type) 
			: Base(std::forward<std::shared_ptr<RenderDevice>&>(renderDevice), NoctisToAPISampler(type)) {}
	};


}
#endif //NOCTIS_SAMPLER_H
