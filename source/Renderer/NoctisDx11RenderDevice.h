#ifndef _NOCTIS_DX11RENDER_DEVICE_H
#define _NOCTIS_DX11RENDER_DEVICE_H
#include <memory>
#include "Window.h"
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <array>
#include <DirectXColors.h>
#include "DirectXTK/Inc/SimpleMath.h"
//#include "DX11Utils.h"

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

namespace noctis::rdr
{
	//TODO: Rename this to Dx11RenderDevice
	template <typename Derived>
	class Dx11RenderDevice
	{
	public:

		//This functions are only available in Dx11 context 
		Microsoft::WRL::ComPtr<ID3D11Device>				GetDevice() const { return m_pDevice.Get(); }
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>			GetDeviceContext() const { return m_pImmediateContext.Get(); }
		Microsoft::WRL::ComPtr<IDXGISwapChain>				GetSwapChain()	const { return m_pSwapChain.Get(); }

		void Present();

		void SetRasterizerState(RasterizerType type);

		void ClearRenderTargetView();

		void ClearDepthStencilView(/*todo pass flags and other arguments*/);

		template <template<typename> typename BufferType, typename DataType, typename... Args>
		auto	CreateBuffer(Args&& ... args);

		bool Init(HWND hwnd, unsigned width, unsigned height);

		void InitDepthStencilBuffer(unsigned width, unsigned height);

		void CreateRasterizationState();

#if NOCTIS_USE_IMGUI
		//-----------------------------------------------------------------------------
		// DxApp::InitImGui() : Initialisation of ImGui.
		//-----------------------------------------------------------------------------
		void InitImGui();

		void RenderToImGui();
#endif //NOCTIS_USE_IMGUI
		//void												CreateRasterizationState();
		//void												InitDepthStencilBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Device>				m_pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_pImmediateContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain>				m_pSwapChain;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_pBackBufferTex;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_pDepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_pDepthStencilView;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_pBackBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_pRTView;

#if _DEBUG
		Microsoft::WRL::ComPtr<ID3D11Debug>					m_pDebugDevice;
#endif

		//Not sure if all these are needed.
		D3D11_TEXTURE2D_DESC								m_depthStenciBufferDesc;
		D3D11_VIEWPORT										m_viewPort;
		D3D_DRIVER_TYPE										m_driverType;
		D3D_FEATURE_LEVEL									m_featureLevel;
		DXGI_SWAP_CHAIN_DESC								m_swapDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC						m_depthStencilViewDesc;

		std::array<Microsoft::WRL::ComPtr<ID3D11RasterizerState>, RasterizerType::COUNT>			m_rasterizerStates;

		bool												m_bEnable4xMsaa;
		unsigned int										m_windowWidth;
		unsigned int										m_windowHeight;
	};
	




	template <typename Derived>
	void Dx11RenderDevice<Derived>::Present()
	{
		m_pSwapChain->Present(0, 0);
	}





	template <typename Derived>
	void Dx11RenderDevice<Derived>::SetRasterizerState(RasterizerType type)
	{
		m_pImmediateContext->RSSetState(m_rasterizerStates[type].Get());
	}





	template <typename Derived>
	void Dx11RenderDevice<Derived>::ClearRenderTargetView()
	{
		m_pImmediateContext->ClearRenderTargetView(m_pRTView.Get(), DirectX::Colors::AliceBlue);
	}





	template <typename Derived>
	void Dx11RenderDevice<Derived>::ClearDepthStencilView(/*todo pass flags and other arguments*/)
	{
		m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}





	template <typename Derived>
	template <template<typename> typename BufferType, typename DataType, typename... Args>
	auto Dx11RenderDevice<Derived>::CreateBuffer(Args&& ... args)
	{
		return new BufferType<DataType>(std::forward<Args>(args)...);
	}





	template <typename Derived>
	bool Dx11RenderDevice<Derived>::Init(HWND hwnd, unsigned width, unsigned height)
	{
		UINT createDeviceFlags = 0;

#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3
		};

		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		//CREATE DEVICE

		HRESULT hResult;
		for (UINT i = 0; i < numDriverTypes; ++i)
		{
			hResult = D3D11CreateDevice(NULL, driverTypes[i], NULL, createDeviceFlags, featureLevels,
				numFeatureLevels, D3D11_SDK_VERSION, &m_pDevice, &m_featureLevel, &m_pImmediateContext);

			if (SUCCEEDED(hResult))
			{
				m_driverType = driverTypes[i];
				break;
			}
		}

		// Check 4X MSAA quality support for our back buffer format.
		UINT msaaQuality;

		m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaaQuality);
		assert(msaaQuality > 0);

		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapDesc.BufferCount = 1;											//Two buffers
		swapDesc.BufferDesc.Width = width;
		swapDesc.BufferDesc.Height = height;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow = hwnd;//Window::GetHandle();
		swapDesc.Windowed = true;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapDesc.SampleDesc.Count = m_bEnable4xMsaa ? 4 : 1;
		swapDesc.SampleDesc.Quality = m_bEnable4xMsaa ? msaaQuality - 1 : 0;
		swapDesc.SampleDesc.Count = 4;											//Multisampling
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		//alt-enter fullscreen

		// To correctly create the swap chain, we must use the IDXGIFactory that was
		// used to create the device.  If we tried to use a different IDXGIFactory instance
		// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
		// This function is being called with a device from a different IDXGIFactory."

		m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(m_pDebugDevice.GetAddressOf()));

		IDXGIDevice* dxgiDevice = 0;
		m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)& dxgiDevice);

		IDXGIAdapter* dxgiAdapter = 0;
		dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)& dxgiAdapter);

		IDXGIFactory* dxgiFactory = 0;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)& dxgiFactory);

		dxgiFactory->CreateSwapChain(m_pDevice.Get(), &swapDesc, &m_pSwapChain);

		dxgiDevice->Release();
		dxgiAdapter->Release();
		dxgiFactory->Release();
		//CREATE RENDER TARGET VIEW

		m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_pBackBufferTex.GetAddressOf()));

		m_pDevice->CreateRenderTargetView(m_pBackBufferTex.Get(), nullptr, &m_pRTView);

		//DEPTH/STENCIL BUFFER INITIALIZATION
		InitDepthStencilBuffer(width, height);

		m_pDevice->CreateTexture2D(&m_depthStenciBufferDesc, 0, &m_pDepthStencilBuffer);
		m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), 0, &m_pDepthStencilView);

		assert(m_pDepthStencilBuffer.Get() != nullptr);
		assert(m_pDepthStencilView.Get() != nullptr);
		//BIND RENDER TARGET VIEW 
		m_pImmediateContext->OMSetRenderTargets(1, m_pRTView.GetAddressOf(), m_pDepthStencilView.Get());

		//VIEWPORT INITIALIZATION
		m_viewPort.Width = static_cast<float>(width);
		m_viewPort.Height = static_cast<float>(height);
		m_viewPort.TopLeftX = 0;
		m_viewPort.TopLeftY = 0;
		m_viewPort.MinDepth = 0.0f;
		m_viewPort.MaxDepth = 1.0f;

		//BIND VIEWPORT
		m_pImmediateContext->RSSetViewports(1, &m_viewPort);

		//Create all rasterizer states that we support.
		CreateRasterizationState();
		return true;
	}





	template <typename Derived>
	void Dx11RenderDevice<Derived>::InitDepthStencilBuffer(unsigned width, unsigned height)
	{
		m_depthStenciBufferDesc.Width = width;
		m_depthStenciBufferDesc.Height = height;
		m_depthStenciBufferDesc.MipLevels = 1;
		m_depthStenciBufferDesc.ArraySize = 1;
		m_depthStenciBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		m_depthStenciBufferDesc.SampleDesc.Count = 4;
		m_depthStenciBufferDesc.SampleDesc.Quality = 0;
		m_depthStenciBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_depthStenciBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		m_depthStenciBufferDesc.CPUAccessFlags = 0;
		m_depthStenciBufferDesc.MiscFlags = 0;

		/*m_depthStencilDesc.DepthEnable = true;
		m_depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		m_depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;*/
	}





	template <typename Derived>
	void Dx11RenderDevice<Derived>::CreateRasterizationState()
	{
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthClipEnable = true;

		HR(m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStates[RasterizerType::SOLID_CULL_BACK]));

		rasterizerDesc.CullMode = D3D11_CULL_NONE;

		HR(m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStates[RasterizerType::SOLID_CULL_NONE]));

		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

		HR(m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStates[RasterizerType::WIREFRAME_CULL_NONE]));

	}






#if NOCTIS_USE_IMGUI
	template <typename Derived>
	void Dx11RenderDevice<Derived>::InitImGui()
	{
		//Setup imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(m_pWindow->GetHandle());
		ImGui_ImplDX11_Init(m_pRenderDevice->GetDevice().Get(), m_pRenderDevice->GetDeviceContext().Get());
	}






	template <typename Derived>
	void Dx11RenderDevice<Derived>::RenderToImGui()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		RenderImGuiFrame();
		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
#endif //NOCTIS_USE_IMGUI
}



#endif _NOCTIS_DX11RENDER_DEVICE_H