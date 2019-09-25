#include "NoctisRenderDevice.h"
#include "NoctisSampler.h"

namespace noctis::rdr
{

	void RenderDevice::Present()
	{
		Base::Present();
	}




	void RenderDevice::Clear(uint8_t targets, float depthColor /*= 1.0f*/, float stencilColor /*= 0.0f*/)
	{
		if (targets & kRender)
			Base::ClearRenderTarget();
		Base::ClearDepthStencil(targets, depthColor, stencilColor);
	}



	void RenderDevice::SetRasterizerState(RasterizerType state)
	{
		Base::SetRasterizerState(state);
	}




	void RenderDevice::SetPrimitiveTopology(Topology topology)
	{
		Base::SetPrimitiveTopology(topology);
	}



	void RenderDevice::SetSampler(uint32_t slot, uint32_t num, std::shared_ptr<Sampler>& sampler)
	{
		Base::SetSampler(slot, num, sampler);
	}

}

