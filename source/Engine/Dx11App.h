#ifndef DX11APP_H
#define DX11APP_H	
#include "App.h"
#include "Core_pch.h"

#if NOCTIS_USE_IMGUI 
#include "imgui/imgui.h"
#endif //NOCTIS_USE_IMGUI 
namespace noctis
{
	class Dx11App : public App
	{
	public:
		Dx11App(HINSTANCE);
		virtual ~Dx11App() = default;

		virtual void						Run() final override;

		virtual bool						Init() override;
		virtual void						Update(float) = 0;
		virtual void						Render(float) = 0;

		virtual void						OnMouseMove(float x, float y) {}
		virtual void						OnKeyPressed() {};
		virtual void						OnKeyReleased() {};

		virtual std::weak_ptr<rdr::RenderDevice>	GetRenderDevice() override { return m_pRenderDevice; }

	private:
		void								ProcessMouseInput();
		void								ProcessKeyboardInput();

#if NOCTIS_USE_IMGUI
	protected:
		void						InitImGui() override;
		void						RenderToImGui() override;
		void						RenderImGuiFrame() override {};
#endif //NOCTIS_USE_IMGUI
};
}
#endif //DX11APP_H