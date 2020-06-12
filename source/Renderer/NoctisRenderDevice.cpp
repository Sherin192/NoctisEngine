#include "NoctisRenderDevice.h"
#include "NoctisSampler.h"
#include "NoctisTexture.h"
namespace noctis::rdr
{

	void RenderDevice::Present()
	{
		Base::Present();
	}



//------------------------------------------------------------------------------------
//		Clear: Clears the buffers specified by the targets variable.
//------------------------------------------------------------------------------------
	void RenderDevice::Clear(uint8_t targets, float depthColor /*= 1.0f*/, float stencilColor /*= 0.0f*/)
	{
		if (targets & kRender)
			Base::ClearRenderTarget();
		Base::ClearDepthStencil(targets, depthColor, stencilColor);
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		SetRasterizerState: Sets the rasterizer state.
//------------------------------------------------------------------------------------
	void RenderDevice::SetRasterizerState(RasterizerType state)
	{
		Base::SetRasterizerState(state);
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		SetPrimitiveTopology: Sets the primitive topology.
//------------------------------------------------------------------------------------
	void RenderDevice::SetPrimitiveTopology(Topology topology)
	{
		Base::SetPrimitiveTopology(topology);
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		SetSampler: Sets the sampler at slot.
//------------------------------------------------------------------------------------
	void RenderDevice::SetSampler(uint32_t slot, uint32_t num, std::shared_ptr<Sampler>& sampler)
	{
		//TODO::The num parameter cannot be used with the Sampler in the current state.
		//This should be either removed or redesigned to take advantage of sampler arrays.
		Base::SetSampler(slot, num, sampler);
	}

//====================================================================================





//------------------------------------------------------------------------------------
//		SetRenderTarget: Sets the render target, not passing a rtv means resetting it
//						 to the back buffer.
//------------------------------------------------------------------------------------
	void RenderDevice::SetRenderTarget(Texture* rtv /* = nullptr*/)
	{
		Base::SetRenderTarget(static_cast<Texture::Base*>(rtv));
	}
//====================================================================================
} //noctis::rdr

