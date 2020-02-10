#include "ExampleDx11App.h"
#include "Renderer/NoctisRenderDevice.h"
#include "Engine/AssetImporter.h"
#include "Engine/CameraFPS.h"
#include "Renderer/Model.h"
#include "Renderer/NoctisCubeMap.h"
#include "NoctisPipelinePass.h"
#include "Renderer/NoctisSampler.h"
#include "Renderer/Material.h"


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
		AssetImporter::Instance().Init(m_pRenderDevice);
		MaterialPool::Instance().Init(m_pRenderDevice);
		
		//_____________________________________MATERILA_SETUP_____________________________________________
		std::string crateMaterialName = "Crate";
		auto crateMaterial = std::make_shared<PhongMaterial>(crateMaterialName);
		//Load textures for the model, this is done only for generated models.
		std::shared_ptr<Texture> crateDiffuse = AssetImporter::Instance().LoadTexture("..\\resources\\Models\\Crate\\container2.png", TextureUsage::DIFFUSE);
		std::shared_ptr<Texture> crateSpecular = AssetImporter::Instance().LoadTexture("..\\resources\\Models\\Crate\\container2_specular.png", TextureUsage::SPECULAR);
		std::shared_ptr<Texture> crateEmissive = AssetImporter::Instance().LoadTexture("..\\resources\\Models\\Crate\\matrix.jpg", TextureUsage::EMISSIVE);
		std::shared_ptr<Texture> crateNormal = AssetImporter::Instance().LoadTexture("..\\resources\\Models\\Crate\\crate_normal.png", TextureUsage::NORMAL);
		crateMaterial->AddTexture(crateDiffuse);
		crateMaterial->AddTexture(crateSpecular);
		crateMaterial->AddTexture(crateEmissive);
		crateMaterial->AddTexture(crateNormal);


		std::string rustyMaterialName = "Rusty";
		auto rustyMaterial = std::make_shared<PBRMaterial>(rustyMaterialName);
		std::shared_ptr<Texture> rustyAlbedo = AssetImporter::Instance().LoadTexture("..\\resources\\Models\\PBR\\rustediron2_basecolor.png", TextureUsage::ALBEDO);
		std::shared_ptr<Texture> rustyMetallic = AssetImporter::Instance().LoadTexture("..\\resources\\Models\\PBR\\rustediron2_metallic.png", TextureUsage::METALLIC);
		std::shared_ptr<Texture> rustyRoughness = AssetImporter::Instance().LoadTexture("..\\resources\\Models\\PBR\\rustediron2_roughness.png", TextureUsage::ROUGHNESS);
		std::shared_ptr<Texture> rustyNormal = AssetImporter::Instance().LoadTexture("..\\resources\\Models\\PBR\\rustediron2_normal.png", TextureUsage::NORMAL);

		rustyMaterial->AddTexture(rustyAlbedo);
		rustyMaterial->AddTexture(rustyMetallic);
		rustyMaterial->AddTexture(rustyRoughness);
		rustyMaterial->AddTexture(rustyNormal);

		MaterialPool::Instance().AddMaterial(crateMaterialName, crateMaterial);
		MaterialPool::Instance().AddMaterial(rustyMaterialName, rustyMaterial);
		//________________________________________________________________________________________________

		//_______________________________________SHADER_SETUP_____________________________________________
		m_pPipelinePass.reset(new PipelinePass(m_pRenderDevice));
		m_pFlatPipelinePass.reset(new PipelinePass(m_pRenderDevice));
		m_pPBRPipelinePass.reset(new PipelinePass(m_pRenderDevice));
		m_pSkyboxPipelinePass.reset(new PipelinePass(m_pRenderDevice));
		

		m_pVShader = std::make_shared<VertexShader>(m_pRenderDevice, L"../resources/Shaders/PhongLightingVertexShader.hlsl");
		m_pPShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/PhongLightingPixelShader.hlsl");
		m_pFlatVShader = std::make_shared<VertexShader>(m_pRenderDevice, L"../resources/Shaders/FlatVertexShader.hlsl");
		m_pFlatPShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/FlatPixelShader.hlsl");
		m_pPBRPShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/PBRPixelShader.hlsl");
		m_pSkyboxVShader = std::make_shared<VertexShader>(m_pRenderDevice, L"../resources/Shaders/SkyboxVertexShader.hlsl");
		m_pSkyboxPShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/SkyboxPixelShader.hlsl");

		m_pSkyboxVShader->SetInputLyout({
			{VertexElement::kPosition, 0, VertexElement::kFloat3 }
			});

		m_pFlatVShader->SetInputLyout({
			{VertexElement::kPosition, 0, VertexElement::kFloat3 }
			});

		m_pVShader->SetInputLyout({
			{VertexElement::kPosition, 0, VertexElement::kFloat3 },
			{VertexElement::kNormal, 0, VertexElement::kFloat3},
			{VertexElement::kTangent, 0, VertexElement::kFloat3},
			{VertexElement::kBitangent, 0, VertexElement::kFloat3},
			{VertexElement::kTexCoord, 0, VertexElement::kFloat2}
			});

		m_pPipelinePass->AddShaders(m_pVShader, m_pPShader);
		m_pFlatPipelinePass->AddShaders(m_pFlatVShader, m_pFlatPShader);
		m_pPBRPipelinePass->AddShaders(m_pVShader, m_pPBRPShader);
		m_pSkyboxPipelinePass->AddShaders(m_pSkyboxVShader, m_pSkyboxPShader);

		m_camera = std::make_unique<Camera>(math::Nvec3(15.0f, 50.0f, 0.0f), 0.2f, 0.0f, 0.4 * 3.14f, m_pWindow->GetWidth(), m_pWindow->GetHeight(), 1.0f, 10000.0f);

		//-------------------------------------------------------------------------------------------------------------------------------
		m_pSpherePBR = AssetImporter::Instance().LoadModel("..\\resources\\Models\\SphereFBX\\sphere.fbx");
		Transform spherePBRTransform({ 50.0f, 50.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 4.0f, 4.0f, 4.0f });
		//m_pSpherePBR->SetMaterial(rustyMaterialName, "Sphere");
		m_pSpherePBR->SetMaterial(kDefaultPBRMaterial, "Sphere");

		m_pSpherePBR->SetTransform(spherePBRTransform);

		m_pSpherePhong = AssetImporter::Instance().LoadModel("..\\resources\\Models\\SphereFBX\\sphere.fbx");
		Transform spherePhongTransform({ 50.0f, 50.0f, 20.0f }, { 0.0f, 0.0f, 0.0f }, { 4.0f, 4.0f, 4.0f });
		m_pSpherePhong->SetMaterial(kDefaultMaterial, "Sphere");

		m_pSpherePhong->SetTransform(spherePhongTransform);

		//-------------------------------------------------------------------------------------------------------------------------------
		
		m_pSkull = AssetImporter::Instance().LoadModel("..\\resources\\Models\\suzanne.obj");

		Transform skullTransform({ 0.0f, 50.0f, 10.0f }, { 1.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f });
		m_pSkull->SetTransform(skullTransform);
		//--------------------------------------------------------------------------------------------------------------------------------------
		//Create a model
		m_pCrate = std::make_unique<Model>(m_pRenderDevice, sg::Shape::CUBE);

		m_pCrate->SetMaterial(crateMaterialName, sg::kShapeNameCube);

		Transform crateTransform({ 30.0f, 30.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
		m_pCrate->SetTransform(crateTransform);

		//m_pSphere = std::make_unique<Model>(m_pRenderDevice, sg::Shape::SPHERE);
		m_pMaterialPreview = AssetImporter::Instance().LoadModel("..\\resources\\Models\\materialpreview-subdiv.obj");
	
		Transform sphereTransform({ 50.0f, 50.0f, -20.0f }, { 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f });

		m_pMaterialPreview->SetTransform(sphereTransform);


		m_pMaterialPreview->SetMaterial(rustyMaterialName, "Cylinder.001_Cylinder.002");
		//----------------------------------------------------------------------------------------------------------

		m_pGLTFModel = AssetImporter::Instance().LoadModel("..\\resources\\Models\\door\\scene.gltf");
		Transform GLTFModelTransform({ 0.0f, 00.0f, 0.0f }, { kPI * 3.0f/2.0f, 0.0f, 0.0f}, { 10.0f, 10.0f, 10.0f });
		m_pGLTFModel->SetTransform(GLTFModelTransform);




		//___________________________________SPONZA_____________________________________________________________

		//TODO:Give the model the default transform.
		/*m_pSponza = AssetImporter::Instance().LoadModel("..\\resources\\Models\\Sponza\\sponza.obj");
		Transform sponzaTransform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
		m_pSponza->SetTransform(sponzaTransform);*/
		//______________________________________________________________________________________________________
		
		//_______________________________________SKYBOX_________________________________________________________

		std::array<std::string, 6> cubeMapPaths{"..\\resources\\Models\\skybox\\right.jpg",
												"..\\resources\\Models\\skybox\\left.jpg",
												"..\\resources\\Models\\skybox\\top.jpg",
												"..\\resources\\Models\\skybox\\bottom.jpg",
												"..\\resources\\Models\\skybox\\front.jpg",
												"..\\resources\\Models\\skybox\\back.jpg" };

		m_pSkybox = AssetImporter::Instance().LoadCubeMap(cubeMapPaths);
		
		//______________________________________________________________________________________________________

		DirectionalLight dirLight;
		dirLight.ambient = { 0.03f, 0.024f, 0.014f, 1.0f };
		dirLight.diffuse = { 0.7f, 0.42f, 0.26f, 1.0f };
		dirLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
		dirLight.direction = { -0.2f, -1.0f, -0.3f };

		cbFrameData.dirLight = dirLight;

		PointLight pointLight;
		pointLight.diffuse = { 1.0f, 0.6f, 0.0f, 1.0f };
		pointLight.specular = { 1.0f, 0.6f, 0.0f, 0.0f };
		pointLight.attenuation = { 1.0f, 0.0022f, 0.00019f };
		pointLight.position = { 0.0f, 50.0f, 30.0f };
		pointLight.enabled = 1;

		cbFrameData.pointLights[0] = pointLight;

		pointLight.position = { 0.0f, 0.0f, -30.0f };

		cbFrameData.pointLights[1] = pointLight;

		pointLight.position = { -20.0f, 40.0f, 0.0f };

		cbFrameData.pointLights[2] = pointLight;

		for (int i = 3; i < 8; ++i)
		{
			pointLight.enabled = 0;

			cbFrameData.pointLights[i] = pointLight;
		}
		static_assert(sizeof(CBFrameData) % 16 == 0);
		m_pConstantPerFrame.reset(m_pRenderDevice->CreateBuffer<rdr::ConstantBuffer, CBFrameData>(true, &cbFrameData));

		m_pPipelinePass->BindConstantBuffers(m_pRenderDevice, m_pConstantPerFrame.get());
		m_pFlatPipelinePass->BindConstantBuffers(m_pRenderDevice, m_pConstantPerFrame.get());
		//m_pSkyboxPipelinePass->BindConstantBuffers(m_pRenderDevice, m_pConstantPerFrame.get());


		return true;
	}





	void ExampleDx11App::Update(float dt)
	{

	}





	void ExampleDx11App::Render(float dt)
	{
		m_pRenderDevice->Clear(kRender | kDepth);

		// Update Frame CB - 2 ways
		cbFrameData.eyePos = m_camera->GetPosition();
		m_pConstantPerFrame->Update(m_pRenderDevice, cbFrameData);
		m_pConstantPerFrame->Bind(m_pRenderDevice, 1);
		//-----------------------------------------------------

		//m_pPipelinePass->Render(m_pRenderDevice, *m_pCrate, *m_camera);
		//m_pPipelinePass->Render(m_pRenderDevice, *m_pSkull, *m_camera);
		//m_pPipelinePass->Render(m_pRenderDevice, *m_pSponza, *m_camera);
		//m_pPipelinePass->Render(m_pRenderDevice, *m_pSpherePhong, *m_camera);
		//m_pPBRPipelinePass->Render(m_pRenderDevice, *m_pSpherePBR, *m_camera);
		m_pPBRPipelinePass->Render(m_pRenderDevice, *m_pMaterialPreview, *m_camera);
		m_pPBRPipelinePass->Render(m_pRenderDevice, *m_pGLTFModel, *m_camera);
		m_pSkyboxPipelinePass->Render(m_pRenderDevice, *m_pSkybox, *m_camera);



#if _DEBUG
		for (int i = 0; i < 8; ++i)
		{
			if (cbFrameData.pointLights[i].enabled)
				cbFrameData.pointLights[i].Render(m_pRenderDevice, m_pFlatPipelinePass.get(), *m_camera);
		}
#endif //_DEBUG
#if NOCTIS_USE_IMGUI
		RenderToImGui();
#endif //NOCTIC_USE_IMGUI
		//_______________________________________________________________________________________________
		m_pRenderDevice->Present();

	}





#if NOCTIS_USE_IMGUI
	//-----------------------------------------------------------------------------
	// ExampleDx11App::RenderToImGui() : Place here all that has to be rendered by ImGui.
	//-----------------------------------------------------------------------------
	void ExampleDx11App::RenderImGuiFrame()
	{
		static int selected_light = 0;
		ImGui::Begin("Lights");
		ImGui::Text("Directional Light");
		ImGui::SliderFloat3("Direction", &cbFrameData.dirLight.direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat3("Global ambient", &cbFrameData.dirLight.ambient.x, 0.0f, 10.0f);
		ImGui::SliderFloat3("Directional Diffuse", &cbFrameData.dirLight.diffuse.x, 0.0f, 10.0f);
		ImGui::SliderFloat3("SpecularDir", &cbFrameData.dirLight.specular.x, 0.0f, 10.0f);
		ImGui::Text("Point Lights");
		ImGui::SliderFloat3("Position", &cbFrameData.pointLights[selected_light].position.x, -500.0f, 500.0f);
		ImGui::SliderFloat3("Diffuse", &cbFrameData.pointLights[selected_light].diffuse.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("Specular", &cbFrameData.pointLights[selected_light].specular.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("Attenuation", &cbFrameData.pointLights[selected_light].attenuation.x, 0.0f, 1.0f);
		ImGui::Checkbox("Enable", (bool*)&cbFrameData.pointLights[selected_light].enabled);
		//TODO: Need a better way of exposing these walues to the UI.
		auto mat = dynamic_cast<PBRMaterial*>(&*m_pSpherePBR->GetMaterial("Sphere"));
		ImGui::SliderFloat("Metalness", &mat->GetData().metallic, 0.0f, 1.0f);
		ImGui::SliderFloat("Roughness", &mat->GetData().roughness, 0.05f, 1.0f);

		auto& GLTFModelTransform = m_pGLTFModel->GetRootNode()->m_transform;

		ImGui::Text("GLTF Model data");
		ImGui::SliderFloat3("PositionModel", &GLTFModelTransform.position.x, -1000.0f, 1000.0f);
		ImGui::SliderFloat3("RotationModel", &GLTFModelTransform.rotation.x, 0.0f, 7.0f);
		ImGui::SliderFloat3("ScaleModel", &GLTFModelTransform.scale.x, -10.0f, 100.0f);

		mat->Update(m_pRenderDevice);

		ImGui::End();

	}
#endif //NOCTIS_USE_IMGUI






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
		const float camVelocity = 50.0f;
		//const XMVECTOR camPos = m_camera->GetPosition();
		if (kb.Up || kb.W)
			m_camera->Move({ 0.0f, 0.0f, camVelocity * dt });

		if (kb.Down || kb.S)
			m_camera->Move({ 0.0f, 0.0f, -camVelocity * dt });

		if (kb.Left || kb.A)
			m_camera->Move({ camVelocity * dt, 0.0f, 0.0f });

		if (kb.Right || kb.D)
			m_camera->Move({ -camVelocity * dt, 0.0f, 0.0f });
		if (kb.Q)
			m_camera->Rotate({ -1.0f, 0.0f });
		if (kb.E)
			m_camera->Rotate({ 1.0f, 0.0f });
		if (kb.R)
			m_camera->Rotate({ 0.0f, -1.0f });
		if (kb.T)
			m_camera->Rotate({ 0.0f, 1.0f });


	}
}