#ifndef APP_H
#define APP_H
#include "Window.h"
#include "Clock.h"

namespace noctis::rdr
{
class RenderDevice;
}
namespace noctis
{
	class App
	{
	public:
		virtual								~App() = default;

		virtual void						Run() = 0;

		virtual bool						Init() = 0;
		virtual void						Update(float) = 0;
		virtual void						Render(float) = 0;

		virtual void						OnMouseMove(float x, float y) = 0;
		virtual void						OnKeyPressed() = 0;
		virtual void						OnKeyReleased() = 0;

		float								GetDeltaTime() { return m_pClock->DeltaTime(); };
		virtual std::weak_ptr<noctis::rdr::RenderDevice>	GetRenderDevice() = 0;

	protected:
#if NOCTIS_USE_IMGUI
		virtual void						InitImGui() = 0;
		virtual void						RenderToImGui() = 0;
		virtual void						RenderImGuiFrame() = 0;
#endif //NOCTIS_USE_IMGUI

		std::shared_ptr<noctis::rdr::RenderDevice>		m_pRenderDevice;
		std::unique_ptr<Window>				m_pWindow;
		std::unique_ptr<Clock>				m_pClock;
	};
}
#endif //APP_H