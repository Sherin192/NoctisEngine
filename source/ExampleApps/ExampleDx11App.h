#ifndef DX11CUBE_APP_H
#define DX11CUBE_APP_H	
#include "Dx11App.h"
#include "Renderer/ConstantBufferData.h"

namespace noctis
{
class Camera;

namespace rdr{
	struct CBModelData;
	struct CBFrameData;
	class PipelinePass;
	class VertexShader;
	class PixelShader;
	template <typename DataType>
	class ConstantBuffer;
	class Model;
}
	class ExampleDx11App : public Dx11App
	{
	public:
		ExampleDx11App(HINSTANCE hInstance);

		bool								Init() override;
		void								Update(float) override;
		void								Render(float) override;
		void								OnMouseMove(float x, float y) override;
		void								OnKeyPressed() override;

	protected:
#if NOCTIS_USE_IMGUI
		void								RenderImGuiFrame() override;
#endif //NOCTIS_USE_IMGUI
	private:

		std::shared_ptr<rdr::Model>				m_pSkull;
		std::unique_ptr<rdr::Model>				m_pCrate;
		std::shared_ptr<rdr::Model>				m_pSponza;

		std::shared_ptr<Camera>							m_camera;

		noctis::rdr::CBFrameData						cbFrameData;

		
		std::unique_ptr<noctis::rdr::ConstantBuffer<rdr::CBFrameData>>		m_pConstantPerFrame;
		std::unique_ptr<noctis::rdr::PipelinePass>							m_pPipelinePass;
		std::unique_ptr<noctis::rdr::PipelinePass>							m_pFlatPipelinePass;
		std::shared_ptr<rdr::VertexShader>		m_pVShader;
		std::shared_ptr<rdr::PixelShader>		m_pPShader;
		std::shared_ptr<rdr::VertexShader>		m_pFlatVShader;
		std::shared_ptr<rdr::PixelShader>		m_pFlatPShader;
	};
}
#endif //DX11CUBE_APP_H	
