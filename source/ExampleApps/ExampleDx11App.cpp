#include "ExampleDx11App.h"
#include "Renderer/NoctisRenderDevice.h"
#include "Engine/AssetImporter.h"
#include "Engine/CameraFPS.h"
#include "Renderer/Model.h"
#include "NoctisPipelinePass.h"


namespace noctis
{
	using namespace rdr;

	ExampleDx11App::ExampleDx11App(HINSTANCE hInstance)
		:Dx11App(hInstance)
	{
		Dx11App::Init();
	}





	//-----------------------------------------------------------------------------
	// ExampleDx11App::Init() : Runs only once. Initialize the data here.
	//-----------------------------------------------------------------------------
	bool ExampleDx11App::Init()
	{
		m_pipelinePass = new PipelinePass(m_pRenderDevice);
		
		AssetImporter::Instance(m_pRenderDevice);

		m_pVShader = std::make_shared<VertexShader>(m_pRenderDevice, L"../resources/Shaders/PhongLightingVertexShader.hlsl");
		m_pPShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/PhongLightingPixelShader.hlsl");
		
		m_pVShader->SetInputLyout({
			{VertexElement::kPosition, 0, VertexElement::kFloat3 },
			{VertexElement::kNormal, 0, VertexElement::kFloat3},
			{VertexElement::kTangent, 0, VertexElement::kFloat3},
			{VertexElement::kBitangent, 0, VertexElement::kFloat3},
			{VertexElement::kTexCoord, 0, VertexElement::kFloat2}
			});

		m_pipelinePass->AddShaders(m_pVShader, m_pPShader);

		m_camera = std::make_unique<Camera>(Vector3(15.0f, 5.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
		m_camera->InitProjMatrix(0.4 * 3.14f, m_pWindow->GetWidth(), m_pWindow->GetHeight(), 1.0f, 2000.0f);

		//-------------------------------------------------------------------------------------------------------------------------------
		m_pSkull = std::make_unique<Model>(m_pRenderDevice, sg::Shape::SKULL);

		const DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		const DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(rotaxis, 1.0f);
		const DirectX::XMMATRIX Translation = DirectX::XMMatrixTranslation(0.0f, 20.0f, 0.0f);
		const DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
		Transform transform;
		transform.Set(Rotation, Translation, Scale);
		m_pSkull->SetTransform(transform);
		//--------------------------------------------------------------------------------------------------------------------------------------
		//TODO:Refactor the shape generator so this is done automaticaly when we pass it the textures paths
		//Create a model
		m_pCrate = std::make_unique<Model>(m_pRenderDevice, sg::Shape::CUBE);

		//Load textures for the model, this is done only for generated models.
		std::shared_ptr<Texture> crateDiffuse = AssetImporter::Instance(m_pRenderDevice).LoadTexture("..\\resources\\Models\\Crate\\container2.png", TextureType::DIFFUSE);
		std::shared_ptr<Texture> crateSpecular = AssetImporter::Instance(m_pRenderDevice).LoadTexture("..\\resources\\Models\\Crate\\container2_specular.png", TextureType::SPECULAR);
		std::shared_ptr<Texture> crateNormal = AssetImporter::Instance(m_pRenderDevice).LoadTexture("..\\resources\\Models\\Crate\\crate_normal.png", TextureType::NORMAL);

		//Set textures previously loaded, this is also done only for generated models.
		m_pCrate->SetTexture(crateDiffuse, TextureType::DIFFUSE, sg::kShapeNameCube);
		m_pCrate->SetTexture(crateSpecular, TextureType::SPECULAR, sg::kShapeNameCube);
		m_pCrate->SetTexture(crateNormal, TextureType::NORMAL, sg::kShapeNameCube);

		//Create the transform for the model
		//TODO:Create the model with a default transform of  position (0,0,0), rotation (0,0,0), scale (1,1,1).
		const DirectX::XMMATRIX crateRotation = DirectX::XMMatrixRotationAxis(rotaxis, 0.0f);
		const DirectX::XMMATRIX cratePosition = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		const DirectX::XMMATRIX crateScale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
		transform.Set(crateRotation, cratePosition, crateScale);

		m_pCrate->SetTransform(transform);
		//_______________________________NANO_EXPERIMENT________________________________________________________

		//TODO:Give the model the default transform.
		m_pSponza = AssetImporter::Instance(m_pRenderDevice).LoadModel("..\\resources\\Models\\Sponza\\sponza.obj");

		const DirectX::XMMATRIX sponzaRotation = DirectX::XMMatrixRotationAxis(rotaxis, 0.0f);
		const DirectX::XMMATRIX sponzaPosition = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		const DirectX::XMMATRIX sponzaScale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);

		m_pSponza->GetTransform().Set(sponzaRotation, sponzaPosition, sponzaScale);
		//______________________________________________________________________________________________________
		DirectionalLight dirLight;
		dirLight.ambient = { 0.03f, 0.024f, 0.014f, 1.0f };
		dirLight.diffuse = { 0.7f, 0.42f, 0.26f, 1.0f };
		dirLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
		dirLight.direction = { -0.2f, -1.0f, -0.3f };

		cbFrameData.dirLight = dirLight;

		PointLight pointLight;
		pointLight.ambient = { 0.1f, 0.06f, 0.0f, 1.0f };
		pointLight.diffuse = { 1.0f, 0.6f, 0.0f, 1.0f };
		pointLight.specular = { 1.0f, 0.6f, 0.0f, 0.0f };
		pointLight.attenuation = { 1.0f, 0.0022f, 0.00019f };
		pointLight.position = { 0.0f, 0.0f, 30.0f };
		pointLight.range = 150.0f;
		pointLight.enabled = 1;

		cbFrameData.pointLights[0] = pointLight;

		for (int i = 1; i < 8; ++i)
		{
			pointLight.enabled = 0;

			cbFrameData.pointLights[i] = pointLight;
		}
		static_assert(sizeof(CBFrameData) % 16 == 0);
		m_constantPerFrame = m_pRenderDevice->CreateBuffer<rdr::ConstantBuffer, CBFrameData>(true, &cbFrameData);

		m_pipelinePass->BindConstantBuffers(m_pRenderDevice, m_constantPerFrame);

		return true;
	}





	void ExampleDx11App::Update(float dt)
	{

	}





	void ExampleDx11App::Render(float dt)
	{
		m_pRenderDevice->ClearRenderTargetView();
		m_pRenderDevice->ClearDepthStencilView();

		// Update Frame CB - 2 ways
		cbFrameData.eyePos = m_camera->GetPosition();
		m_constantPerFrame->Update(m_pRenderDevice, cbFrameData);
		m_constantPerFrame->Bind(m_pRenderDevice, 1);
		//-----------------------------------------------------

		//TODO:CHeck why Frame cb is at index 1, consider using index 0?
		m_pipelinePass->Render(m_pRenderDevice, *m_pCrate, *m_camera);
		m_pipelinePass->Render(m_pRenderDevice, *m_pSkull, *m_camera);
		m_pipelinePass->Render(m_pRenderDevice, *m_pSponza, *m_camera);
#if _DEBUG && 0
		for (int i = 0; i < 8; ++i)
		{
			if (CBFrameData.pointLights[i].enabled)
				CBFrameData.pointLights[i].Render(m_pRenderDevice, *m_pLightShader, *m_camera);
		}
#endif //_DEBUG
#if NOCTIC_USE_IMGUI
		RenderToImGui();
#endif //NOCTIC_USE_IMGUI
		//_______________________________________________________________________________________________
		m_pRenderDevice->GetSwapChain()->Present(0, 0);

	}





#if NOCTIC_USE_IMGUI
	//-----------------------------------------------------------------------------
	// ExampleDx11App::RenderToImGui() : Place here all that has to be rendered by ImGui.
	//-----------------------------------------------------------------------------
	void ExampleDx11App::RenderImGuiFrame()
	{
		ImGui::Begin("Debug");
		/*
			ImGui::SliderFloat3("PL1.ambient", &pointLight.position, 0.0f, 1.0f);
			ImGui::SliderFloat3("PL1.diffuse", &CBFrameData.dirLight.diffuse.x, 0.0f, 1.0f);
			ImGui::SliderFloat3("PL1.specular", &CBFrameData.dirLight.specular.x, 0.0f, 1.0f);*/
		Material& crateMat = m_pCrate->GetMaterial(sg::kShapeNameCube);



		ImGui::SliderFloat("PL1.pow", &crateMat.GetGPUMaterial().specular.w, 0.0f, 1000.0f);
		//ImGui::SliderFloat3("PL1diffuse", &CBFrameData.pointLights[0].diffuse.x, 0.0f, 10.0f);

		ImGui::SliderFloat3("PL1ambient", &CBFrameData.pointLights[0].position.x, 0.0f, 100.0f);
		//ImGui::SliderFloat3("PL1specular", &CBFrameData.pointLights[0].specular.x, 0.0f, 10.0f);



		ImGui::End();

	}
#endif //NOCTIC_USE_IMGUI






	//-----------------------------------------------------------------------------
	// ExampleDx11App::OnMouseMove() : Deal here with mouse moved event.
	//-----------------------------------------------------------------------------
	void ExampleDx11App::OnMouseMove(float x, float y)
	{
		const auto mouse = m_pWindow->GetMouseState();
		const auto prevMouse = m_pWindow->GetPrevMouseState();
		if (prevMouse.x != x && prevMouse.y != y)
		{
			m_camera->Rotate({ (float)x, (float)y });
		}
	}





	//-----------------------------------------------------------------------------
	// ExampleDx11App::OnKeyPressed() : Deal here with key pressed event.
	//-----------------------------------------------------------------------------
	void ExampleDx11App::OnKeyPressed()
	{
		using DirectX::XMVECTOR;
		const auto kb = m_pWindow->GetKeyboardState();
		const float dt = GetDeltaTime();

		if (kb.Escape)
			PostQuitMessage(0);
		const float camVelocity = 25.0f;
		const XMVECTOR camPos = m_camera->GetPosition();
		if (kb.Up || kb.W)
			m_camera->Move({ 0.0f, 0.0f, camVelocity * dt });

		if (kb.Down || kb.S)
			m_camera->Move({ 0.0f, 0.0f, -camVelocity * dt });

		if (kb.Left || kb.A)
			m_camera->Move({ -camVelocity * dt, 0.0f, 0.0f });

		if (kb.Right || kb.D)
			m_camera->Move({ camVelocity * dt, 0.0f, 0.0f });

	}
}