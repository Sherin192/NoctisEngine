#ifndef _WINDOW_H
#define _WINDOW_H
#include "Core_pch.h"
#include <windows.h>
#include "DirectXTK/Inc/Keyboard.h"
#include "DirectXTK/Inc/Mouse.h"

#include <optional>
#include <string>

namespace noctis
{
class Window
{
public:
	Window(HINSTANCE);

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	UINT						GetWidth() const noexcept { return m_width; }
	UINT						GetHeight() const noexcept { return m_height; }
	HWND						GetHandle() const noexcept { return m_handle; }
	DirectX::Keyboard::State	GetKeyboardState() const  noexcept { return m_pKeyboard->GetState(); }
	DirectX::Keyboard::State	GetPrevKeyboardState() const  noexcept { return m_keyboardKeyStateTracker.GetLastState(); }
	DirectX::Mouse::State		GetMouseState() const noexcept { return m_pMouse->GetState(); }
	DirectX::Mouse::State		GetPrevMouseState() const noexcept { return m_mouseButtonStateTracker.GetLastState(); }


	void						Update();
	std::optional<int>			ProcessMessage();
	static bool					IsMouseLocked() { return m_bMouseLocked; }
	static void					LockMouse(bool lock) { m_bMouseLocked = lock;}
private:
	bool						Init();
	
	HINSTANCE					m_hInstance;
	DWORD						m_wndStyle;
	HWND						m_handle;
	
	std::string					m_title;
	UINT						m_width;
	UINT						m_height;
	inline static bool			m_bMouseLocked {false};


	std::unique_ptr<DirectX::Keyboard>			m_pKeyboard;
	std::unique_ptr<DirectX::Mouse>				m_pMouse;

	DirectX::Mouse::ButtonStateTracker			m_mouseButtonStateTracker;
	DirectX::Keyboard::KeyboardStateTracker		m_keyboardKeyStateTracker;
public:
	friend LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
}

#endif //_WINDOW_H
