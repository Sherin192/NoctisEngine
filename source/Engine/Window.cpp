#include "Window.h"

namespace noctis
{
#if NOCTIC_USE_IMGUI
	extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif //NOCTIC_USE_IMGUI

	LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	Window::Window(HINSTANCE hInstance)
		: m_hInstance(hInstance),
		m_title("Dx11App"),
		m_wndStyle(WS_OVERLAPPEDWINDOW)
	{
		m_width = 800;
		m_height = 600;

		if (!Init())
		{
			//LOG::Message and terminate the app as there is nothing we can do.
			PostQuitMessage(0);
		}
		m_pKeyboard = std::make_unique<DirectX::Keyboard>();
		m_pMouse = std::make_unique<DirectX::Mouse>();

		m_pMouse->SetWindow(m_handle);

		m_pMouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	}




	//-----------------------------------------------------------------------------
	// Window::Init() : Initializes the window.
	//-----------------------------------------------------------------------------
	bool Window::Init()
	{
		WNDCLASSEX wndclass;
		ZeroMemory(&wndclass, sizeof(WNDCLASSEX));

		wndclass.cbClsExtra = 0;
		wndclass.cbSize = sizeof(WNDCLASSEX);
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = MsgProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = m_hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = "DXAPPWNDCLASS";
		wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wndclass))
		{
			OutputDebugString("\n\nFailed to initialize the window class! \n\n");
			return false;
		}

		RECT rect = { 0, 0, m_width, m_height };
		AdjustWindowRect(&rect, m_wndStyle, FALSE);
		UINT width = rect.right - rect.left;
		UINT height = rect.bottom - rect.top;

		UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
		UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
		m_handle = CreateWindow("DXAPPWNDCLASS", m_title.c_str(), m_wndStyle, x, y, width, height, NULL, NULL, m_hInstance, NULL);

		if (!m_handle)
		{
			OutputDebugString("\n\n\nFailed to initialize the window ! \n\n\n");
			return false;
		}

		ShowWindow(m_handle, SW_SHOW);
		return true;
	}




	//-----------------------------------------------------------------------------
	// Window::Update() :  
	//-----------------------------------------------------------------------------
	void Window::Update()
	{
		auto kb = GetKeyboardState();

		m_mouseButtonStateTracker.Update(GetMouseState());
		m_keyboardKeyStateTracker.Update(kb);

		IsMouseLocked() ? m_pMouse->SetMode(DirectX::Mouse::MODE_RELATIVE) : m_pMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);

		//LShift+LAlt+Z will toggle mouseLocked state for now. If this combination of keys is pressed the keyboard state is reset.
		if (kb.LeftShift && kb.LeftAlt && kb.Z)
		{
			Window::LockMouse(!Window::IsMouseLocked());
			m_pKeyboard->Reset();
		}
	}






	//-----------------------------------------------------------------------------
	// Window::ProcessMessage() : Clears the message pump. 
	//-----------------------------------------------------------------------------
	std::optional<int> Window::ProcessMessage()
	{
		MSG msg = {};

		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
				return (int)msg.wParam;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return {};
	}



	//-----------------------------------------------------------------------------
	// CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) : 
	// Processes windows messages. 
	//-----------------------------------------------------------------------------
	LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
#if NOCTIC_USE_IMGUI
		if (!Window::IsMouseLocked() && ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			return true;
#endif //NOCTIC_USE_IMGUI
		switch (msg)
		{
		case WM_DESTROY:
		case WM_QUIT:
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		case WM_KILLFOCUS:
			break;
		case WM_SETFOCUS:
			break;
		case WM_ACTIVATEAPP:
			DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
			break;
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		return LRESULT();
	}
}