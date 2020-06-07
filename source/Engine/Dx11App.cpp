#include "Core_pch.h"
#include "Dx11App.h"

#include "Window.h"
#include "Renderer/NoctisRenderDevice.h"

#if NOCTIS_USE_IMGUI 
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#endif //NOCTIS_USE_IMGUI

namespace noctis
{

	//-----------------------------------------------------------------------------
	// Dx11App(HINSTANCE hInstance) : Constructor.
	//-----------------------------------------------------------------------------
	Dx11App::Dx11App(HINSTANCE hInstance)
	{
		m_pClock = std::make_unique<Clock>();
		m_pWindow = std::make_unique<Window>(hInstance);
		Init();
	}




	//-----------------------------------------------------------------------------
	// Dx11App::Init() : Initializes the window and direct3D
	//-----------------------------------------------------------------------------
	bool Dx11App::Init()
	{
		m_pRenderDevice = std::make_shared<rdr::RenderDevice>();
		if (m_pRenderDevice->Init(m_pWindow->GetHandle(), m_pWindow->GetWidth(), m_pWindow->GetHeight()))
		{
//		if (InitDirect3D())
//		{
#if NOCTIS_USE_IMGUI
			InitImGui();
#endif //NOCTIS_USE_IMGUI
			return true;
		}
		return false;
	}





	//-----------------------------------------------------------------------------
	// Dx11App::Run() : Executes the main loop and processes the messages.
	//-----------------------------------------------------------------------------
	void Dx11App::Run()
	{
		while (true)
		{
			m_pWindow->Update();
			if (const auto opt = m_pWindow->ProcessMessage())
			{
				PostQuitMessage(opt.value());
				break;
			}
			ProcessMouseInput();
			ProcessKeyboardInput();

			const float dt = m_pClock->DeltaTime();
			Update(dt);
			Render(dt);
#if NOCTIS_USE_IMGUI
			RenderToImGui();
#endif //NOCTIC_USE_IMGUI
			m_pRenderDevice->Present();
			m_pClock->Tick();
		}
	}





	//-----------------------------------------------------------------------------
	// Dx11App::OnMouseMove() : Passes the mouse position to child class impl.
	//-----------------------------------------------------------------------------
	void Dx11App::ProcessMouseInput()
	{
		auto state = m_pWindow->GetMouseState();

		if (Window::IsMouseLocked())
			OnMouseMove((float)state.x, (float)state.y);
	}






	//-----------------------------------------------------------------------------
	// Dx11App::OnMouseMove() : Passes the keyboard state to child class impl.
	//-----------------------------------------------------------------------------
	void Dx11App::ProcessKeyboardInput()
	{
		OnKeyPressed();
		OnKeyReleased();
	}






#if NOCTIS_USE_IMGUI
	//-----------------------------------------------------------------------------
	// DxApp::InitImGui() : Initialisation of ImGui.
	//-----------------------------------------------------------------------------
	void Dx11App::InitImGui()
	{
		//Setup imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(m_pWindow->GetHandle());
		ImGui_ImplDX11_Init(m_pRenderDevice->GetDevice(), m_pRenderDevice->GetDeviceContext());
	}






	//-----------------------------------------------------------------------------
	// DxApp::RenderToImGui() : Takes care of the setup for the frame.
	//-----------------------------------------------------------------------------
	void Dx11App::RenderToImGui()
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
