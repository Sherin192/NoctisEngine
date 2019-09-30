#ifndef _NOCTIS_DX11RENDER_DEVICE_H
#define _NOCTIS_DX11RENDER_DEVICE_H
#include <memory>
#include "Window.h"
#include <wrl.h>
#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <array>
#include <DirectXColors.h>
#include "DirectXTK/Inc/SimpleMath.h"
#include "Noctis3DTypes.h"
#include "NoctisSampler.h"

#if _DEBUG
#define HR(x) do { if (FAILED(x))	\
{									\
	DebugBreak();					\
}} while(0)
#else
#define HR(x) x
#endif //_DEBUG

enum BUFFER
{
	DEFAULT_VERTEX_BUFFER,
	IMMUTABLE_VERTEX_BUFFER,
	DYNAMIC_VERTEX_BUFFER,
	DEFAULT_INDEX_BUFFER,
	IMMUTABLE_INDEX_BUFFER
};
enum RasterizerType
{
	SOLID_CULL_BACK,
	SOLID_CULL_NONE,
	WIREFRAME_CULL_NONE,
	COUNT
};


static auto NoctisToDx11Topology(noctis::rdr::Topology topology)
{
	using namespace noctis::rdr;
	switch (topology)
	{
	case kLineList:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case kTriangleList:
		return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case kPatch3List:
		return D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
	}
}

namespace noctis::rdr
{
	class Dx11RenderDevice
	{
	protected:
		//C.35: A base class destructor should be either public and virtual, or protected and nonvirtual
		~Dx11RenderDevice() = default;
	public:

		//This functions are only available in Dx11 context 
		auto GetDevice() const { return m_pDevice.Get(); }
		auto GetDeviceContext() const { return m_pImmediateContext.Get(); }
		auto GetSwapChain()	const { return m_pSwapChain.Get(); }

		void Present();

		void SetRasterizerState(RasterizerType);

		void SetPrimitiveTopology(Topology);

		void SetSampler(uint32_t slot, uint32_t num, std::shared_ptr<Sampler>& sampler);

		bool Init(HWND hwnd, unsigned width, unsigned height);


		void CreateRasterizationState();

	protected:
		void ClearRenderTarget();

		void ClearDepthStencil(uint8_t targets, float depthColor, float stencilColor);
	private:
		auto InitDepthStencilDescs(unsigned width, unsigned height);

//#if NOCTIS_USE_IMGUI
//		//-----------------------------------------------------------------------------
//		// DxApp::InitImGui() : Initialisation of ImGui.
//		//-----------------------------------------------------------------------------
//		void InitImGui();
//
//		void RenderToImGui();
//#endif //NOCTIS_USE_IMGUI
		//void												CreateRasterizationState();
		//void												InitDepthStencilBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Device>				m_pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_pImmediateContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1>				m_pSwapChain;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_pBackBufferTex;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_pDepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_pDepthStencilView;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_pBackBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_pRTView;

#if _DEBUG
		Microsoft::WRL::ComPtr<ID3D11Debug>					m_pDebugDevice;
#endif
		D3D_DRIVER_TYPE										m_driverType;
		D3D_FEATURE_LEVEL									m_featureLevel;

		std::array<Microsoft::WRL::ComPtr<ID3D11RasterizerState>, RasterizerType::COUNT>			m_rasterizerStates;

		bool												m_bEnable4xMsaa;
		unsigned int										m_windowWidth;
		unsigned int										m_windowHeight;
		HWND												m_windowHandle;
	};
}	//noctis::rdr
#endif _NOCTIS_DX11RENDER_DEVICE_H