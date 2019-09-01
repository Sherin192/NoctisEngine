#include "NoctisRenderDevice.h"

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
}

