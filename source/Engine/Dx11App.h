#ifndef DX11APP_H
#define DX11APP_H	
#include "App.h"

namespace noctis
{
	class Dx11App : public App
	{
	public:
		Dx11App(HINSTANCE);
		virtual ~Dx11App() = default;

		virtual void						Run() override;

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
		bool								InitDirect3D();

#if NOCTIS_USE_IMGUI
		virtual void						InitImGui() override;

	protected:
		virtual void						RenderToImGui() override;
		virtual void						RenderImGuiFrame() {};
#endif //NOCTIS_USE_IMGUI
};
}
#endif //DX11APP_H