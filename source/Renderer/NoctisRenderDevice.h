#ifndef _NOCTIS_RENDER_DEVICE_H
#define _NOCTIS_RENDER_DEVICE_H

#if defined(_WIN32)
#include "Dx11RenderDevice.h"
#endif
namespace noctis::rdr
{
	template <typename DataType>
	class ConstantBuffer;
	class Sampler;

	//TODO: the base class will be selected based on a compile time expression 
	//A compile time meta function which will return the desired API implementation of RenderDevice
	//RenderDeviceAPI<RenderDevice>::type <- will return APIRenderDevice<RenderDevice> 	
	class RenderDevice final : public Dx11RenderDevice
	{
		using Base = Dx11RenderDevice;
		
		public:
		//============================================================================================
		// Public Interface:
		//============================================================================================
			void Present();

			//TODO: a render target Color must be added as the second parameter when a platform independent Vector/Color class is implemented. Currently it will default to the color set
			//in the underlying implementation.
			void Clear(uint8_t targets, float depthColor = 1.0f, float stencilColor = 0.0f);

			void SetRasterizerState(RasterizerType state);

			void SetPrimitiveTopology(Topology topology);

			void SetSampler(uint32_t slot, uint32_t num, std::shared_ptr<Sampler>& sampler);

			template <template<typename> typename BufferType, typename DataType, typename... Args>
			auto CreateBuffer(Args... args);
	};

	template <template<typename> typename BufferType, typename DataType, typename... Args>
	auto RenderDevice::CreateBuffer(Args... args)
	{
		return new BufferType<DataType>(this, std::forward<Args>(args)...);
	};
}
#endif //_NOCTIS_RENDER_DEVICE_H
