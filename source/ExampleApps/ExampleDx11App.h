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
	template <typename T> class ForwardPipelinePass;
	template <typename T> class PostProcessPipelinePass;
	class VertexShader;
	class PixelShader;
	class ComputeShader;
	template <typename DataType>
	class ConstantBuffer;
	class Model;
	class CubeMap;
}


class TestDx11App : public Dx11App
{
public:
	TestDx11App(HINSTANCE hInstance) :Dx11App(hInstance) {}

	bool								Init() override { return true; };
	void								Update(float) override {};
	void								Render(float) override {};
	void								OnMouseMove(float x, float y) override {};
	void								OnKeyPressed() override {};
};
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
		std::vector<std::shared_ptr<rdr::Model>> m_pModels;
		std::vector <std::shared_ptr<rdr::NoctisLight>> m_pLights;
		std::shared_ptr<rdr::Model>				m_pSponza;
		std::shared_ptr<rdr::Model>				m_pSpherePBR;
		std::shared_ptr<rdr::Model>				m_pSpherePhong;
		std::shared_ptr<rdr::Model>				m_pCrate;
		std::shared_ptr<rdr::Model>				m_pSphereGenerated;
		std::shared_ptr<rdr::Model>				m_pGLTFModel;

		std::shared_ptr<rdr::CubeMap>			m_pSkybox;
		std::shared_ptr<Camera>					m_camera;
		float									camVelocity = 50.0f;
		float									camRotationSpeed = 1.0f;
		noctis::rdr::CBFrameData				cbFrameData;

		
		std::unique_ptr<noctis::rdr::ConstantBuffer<rdr::CBFrameData>>		m_pConstantPerFrame;
		std::unique_ptr<rdr::ForwardPipelinePass<rdr::Model>>				m_pForwardPass;
		std::unique_ptr<rdr::ForwardPipelinePass<rdr::Model>>				m_pForwardPBRPass;
		std::unique_ptr<rdr::ForwardPipelinePass<rdr::Model>>				m_pForwardGoochPass;
		std::unique_ptr<rdr::PostProcessPipelinePass<rdr::Texture>>			m_pToneMapHdrPass;
		std::unique_ptr<rdr::ForwardPipelinePass<rdr::CubeMap>>				m_pSkyboxPipelinePass;
		std::unique_ptr< rdr::ForwardPipelinePass<rdr::NoctisLight>>		m_pFlatPipelinePass;
		std::unique_ptr<rdr::PostProcessPipelinePass<rdr::Texture>>			m_pFullScreenFilterPass;
		std::unique_ptr<rdr::PostProcessPipelinePass<rdr::Texture>>			m_pFullScreenGrayscalePass;



		std::shared_ptr<rdr::VertexShader>		m_pVShader;
		std::shared_ptr<rdr::PixelShader>		m_pPShader;
		std::shared_ptr<rdr::VertexShader>		m_pFlatVShader;
		std::shared_ptr<rdr::PixelShader>		m_pFlatPShader;
		std::shared_ptr<rdr::PixelShader>		m_pPBRPShader;
		std::shared_ptr<rdr::VertexShader>		m_pSkyboxVShader;
		std::shared_ptr<rdr::PixelShader>		m_pSkyboxPShader;
		std::shared_ptr<rdr::VertexShader>		m_pHdrVShader;
		std::shared_ptr<rdr::PixelShader>		m_pHdrPShader;
		std::shared_ptr<rdr::PixelShader>		m_pGoochPShader;
		std::shared_ptr<rdr::PixelShader>		m_pFullScreenFilterPShader;
		std::shared_ptr<rdr::PixelShader>		m_pFullScreenGrayscalePShader;


		std::shared_ptr<rdr::Texture>	m_hdrRTV;
		std::shared_ptr<rdr::Texture>	m_filterRTV;
		std::shared_ptr<rdr::Texture>	m_grayscaleRTV;

		bool m_enableGammaCorrection = true;
		bool m_enableNegativeFilter = false;
		bool m_enableGrayscale = false;
		bool m_enableBlurFilter = false;
		bool m_enabledNormalMapping = true;
		bool m_showLights = true;
		float gamma = 2.2f;
		void AddLight();
	};
}
#endif //DX11CUBE_APP_H	
