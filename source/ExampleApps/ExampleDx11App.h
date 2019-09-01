#ifndef DX11CUBE_APP_H
#define DX11CUBE_APP_H	
#include "Core_pch.h"
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
#if NOCTIC_USE_IMGUI
		void								RenderImGuiFrame() override;
#endif //NOCTIC_USE_IMGUI
	private:

		std::unique_ptr<rdr::Model>				m_pSkull;
		std::unique_ptr<rdr::Model>				m_pCrate;
		std::unique_ptr<rdr::Model>				m_pPlane;

		std::shared_ptr<Camera>							m_camera;

		noctis::rdr::CBFrameData						cbFrameData;

		//Experimental
		std::shared_ptr<rdr::Model>				m_nanoCrisis;
		
		noctis::rdr::ConstantBuffer<rdr::CBFrameData>*		m_constantPerFrame;
		noctis::rdr::ConstantBuffer<rdr::CBModelData>*		m_constantPerObject;
		noctis::rdr::PipelinePass*							m_pipelinePass;
		std::shared_ptr<rdr::VertexShader>		m_pVShader;
		std::shared_ptr<rdr::PixelShader>		m_pPShader;
	};
}
#endif //DX11CUBE_APP_H	
