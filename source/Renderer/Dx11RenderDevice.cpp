#include "Dx11RenderDevice.h"
#include "Dx11Texture.h"

namespace noctis::rdr
{
	void Dx11RenderDevice::Present()
	{
		m_pSwapChain->Present(1, 0);
		m_pImmediateContext->OMSetRenderTargets(1, m_pRTView.GetAddressOf(), m_pDepthStencilView.Get());
	}





	void Dx11RenderDevice::SetRasterizerState(RasterizerType type)
	{
		m_pImmediateContext->RSSetState(m_rasterizerStates[type].Get());
	}





	void Dx11RenderDevice::ClearRenderTarget()
	{
		m_pImmediateContext->ClearRenderTargetView(m_pRTView.Get(), DirectX::Colors::AliceBlue);
	}





	void Dx11RenderDevice::ClearDepthStencil(uint8_t targets, float depthColor, float stencilColor)
	{
		static_assert(D3D11_CLEAR_DEPTH == ClearTargetType::kDepth && D3D11_CLEAR_STENCIL == ClearTargetType::kStencil);
		static constexpr uint8_t flags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;
		
		m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), targets & flags, depthColor, stencilColor);
	}





	void Dx11RenderDevice::SetPrimitiveTopology(Topology topology)
	{
		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}



	void Dx11RenderDevice::SetSampler(uint32_t slot, uint32_t num, std::shared_ptr<Sampler>& sampler)
	{
		m_pImmediateContext->PSSetSamplers(slot, num, sampler->GetDx11Sampler().GetAddressOf());
	}





	auto Dx11RenderDevice::InitDepthStencilDescs(unsigned width, unsigned height)
	{
		D3D11_DEPTH_STENCIL_DESC		depthStencilDesc{};
		D3D11_TEXTURE2D_DESC			depthStenciBufferDesc{};
		D3D11_DEPTH_STENCIL_VIEW_DESC	depthStencilViewDesc{};

		depthStenciBufferDesc.Width = width;
		depthStenciBufferDesc.Height = height;
		depthStenciBufferDesc.MipLevels = 1u;
		depthStenciBufferDesc.ArraySize = 1u;
		depthStenciBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStenciBufferDesc.SampleDesc.Count = 1u;
		depthStenciBufferDesc.SampleDesc.Quality = 0u;
		depthStenciBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStenciBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStenciBufferDesc.CPUAccessFlags = 0u;
		depthStenciBufferDesc.MiscFlags = 0u;

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0u;

		return std::make_tuple(depthStencilDesc, depthStenciBufferDesc, depthStencilViewDesc);
	}





	bool Dx11RenderDevice::Init(HWND hwnd, unsigned width, unsigned height)
	{
		using Microsoft::WRL::ComPtr;
		UINT createDeviceFlags = 0;
		m_windowHandle = hwnd;
		m_windowWidth = width;
		m_windowHeight = height;
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

		// To correctly create the swap chain, we must use the IDXGIFactory that was
		// used to create the device.  If we tried to use a different IDXGIFactory instance
		// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
		// This function is being called with a device from a different IDXGIFactory."

		m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(m_pDebugDevice.GetAddressOf()));

		ComPtr<IDXGIDevice1> dxgiDevice;
		m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.GetAddressOf());

		ComPtr<IDXGIAdapter> dxgiAdapter;
		dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)dxgiAdapter.GetAddressOf());

		ComPtr<IDXGIFactory5> dxgiFactory;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)dxgiFactory.GetAddressOf());

		BOOL allowTearing = 0;
		dxgiFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));

		DXGI_SWAP_CHAIN_DESC1 swapDesc;
		ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC1));
		swapDesc.Width = width;
		swapDesc.Height = height;
		swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.Stereo = FALSE;
		swapDesc.SampleDesc = { 1, 0 };											//With flip model swap chain, this member must be specified as {1, 0}
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 2;
		swapDesc.Scaling = DXGI_SCALING_STRETCH;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapDesc.Flags = allowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

		dxgiFactory->CreateSwapChainForHwnd(m_pDevice.Get(), hwnd, &swapDesc, nullptr, nullptr, &m_pSwapChain);

		//CREATE RENDER TARGET VIEW
		m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_pBackBufferTex.GetAddressOf()));

		m_pDevice->CreateRenderTargetView(m_pBackBufferTex.Get(), nullptr, &m_pBackBufferRTView);
		m_pRTView = m_pBackBufferRTView;

		//DEPTH/STENCIL BUFFER INITIALIZATION
		auto [dsDesc, dsbDesc, dsvDesc] = InitDepthStencilDescs(width, height);		//Fills all the _DESC structs for the depth stencil initialization.

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState;
		m_pDevice->CreateDepthStencilState(&dsDesc, &dsState);

		m_pImmediateContext->OMSetDepthStencilState(dsState.Get(), NULL);		//Second parameter is for stencile which is not used at the moment.

		m_pDevice->CreateTexture2D(&dsbDesc, 0, &m_pDepthStencilBuffer);
		m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), &dsvDesc, &m_pDepthStencilView);

		assert(m_pDepthStencilBuffer.Get() != nullptr);
		assert(m_pDepthStencilView.Get() != nullptr);
		//BIND RENDER TARGET VIEW 
		m_pImmediateContext->OMSetRenderTargets(1, m_pBackBufferRTView.GetAddressOf(), m_pDepthStencilView.Get());

		//VIEWPORT INITIALIZATION
		D3D11_VIEWPORT viewPort;
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		viewPort.Width = static_cast<float>(width);
		viewPort.Height = static_cast<float>(height);
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;

		m_pImmediateContext->RSSetViewports(1, &viewPort);

		//Create all rasterizer states that we support.
		CreateRasterizationState();
		return true;
	}






	void Dx11RenderDevice::CreateRasterizationState()
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




	void Dx11RenderDevice::SetRenderTarget(Dx11Texture* rtv)
	{
		if (rtv)
			m_pRTView = rtv->GetRTV();
		else m_pRTView = m_pBackBufferRTView;
		m_pImmediateContext->OMSetRenderTargets(1, rtv? m_pRTView.GetAddressOf() : m_pBackBufferRTView.GetAddressOf(), m_pDepthStencilView.Get());
	}
	void Dx11RenderDevice::ResetRenderTarget()
	{
		m_pImmediateContext->OMSetRenderTargets(1, m_pBackBufferRTView.GetAddressOf(), m_pDepthStencilView.Get());
	}

} //noctis::rdr