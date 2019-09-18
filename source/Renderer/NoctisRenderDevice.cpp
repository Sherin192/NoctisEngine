#include "NoctisRenderDevice.h"
#include "NoctisSampler.h"

namespace noctis::rdr
{

	void RenderDevice::Present()
	{
		GetImpl().Present();
	}





	void RenderDevice::SetRasterizerState(RasterizerType state)
	{
		GetImpl().SetRasterizerState(state);
	}






	void RenderDevice::ClearRenderTargetView()
	{
		GetImpl().ClearRenderTargetView();
	}






	void RenderDevice::ClearDepthStencilView(/*TODO pass flags and other arguments*/)
	{
		GetImpl().ClearDepthStencilView();
	}





	void RenderDevice::SetPrimitiveTopology(Topology topology)
	{
		GetImpl().SetPrimitiveTopology(topology);
	}



	void RenderDevice::SetSampler(uint32_t slot, uint32_t num, std::shared_ptr<Sampler>& sampler)
	{
		GetImpl().SetSampler(slot, num, sampler);
	}

}

