#include "ExampleDx11App.h"
#include "Renderer/NoctisRenderDevice.h"
#include "Engine/AssetImporter.h"
#include "Engine/CameraFPS.h"
#include "Engine/Model.h"
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
		//Dx11App::Init();
	}





	//-----------------------------------------------------------------------------
	// ExampleDx11App::Init() : Runs only once. Initialize the data here.
	//-----------------------------------------------------------------------------
	bool ExampleDx11App::Init()
	{
		static_assert(sizeof(CBFrameData) % 16 == 0);
		AssetImporter::Instance().Init(m_pRenderDevice);
		MaterialPool::Instance().Init(m_pRenderDevice);
		
		//_____________________________________MATERILA_SETUP_____________________________________________
		std::string crateMaterialName = "Crate";
		auto crateMaterial = std::make_shared<PhongMaterial>(crateMaterialName);
		//Load textures for the model, this is done only for generated models.
		std::shared_ptr<Texture> crateDiffuse = AssetImporter::Instance().LoadTexture("..\\resources\\Models\\Crate\\container2.png", TextureUsage::DIFFUSE);
		std::shared_ptr<Texture> crateSpecular = AssetImporter::Instance().LoadTexture("..\\resources\\Models\\Crate\\container2_specular.png", TextureUsage::SPECULAR);
		std::shared_ptr<Texture> crateNormal = AssetImporter::Instance().LoadTexture("..\\resources\\Models\\Crate\\crate_normal.png", TextureUsage::NORMAL);
		crateMaterial->AddTexture(crateDiffuse);
		crateMaterial->AddTexture(crateSpecular);
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

		//Add the materials to the pool
		MaterialPool::Instance().AddMaterial(crateMaterialName, crateMaterial);
		MaterialPool::Instance().AddMaterial(rustyMaterialName, rustyMaterial);
		//________________________________________________________________________________________________
		
		//Create the various render targets
		auto& [width, height] = m_pRenderDevice->GetWindowWidthHeight();
		m_hdrRTV = std::make_unique<Texture>(m_pRenderDevice, nullptr, "", width, height, 4, TextureUsage::RTV, Format::kRGBA16F);
		m_filterRTV = std::make_unique<Texture>(m_pRenderDevice, nullptr, "", width, height, 4, TextureUsage::RTV, Format::kRGBA16F);
		m_grayscaleRTV = std::make_unique<Texture>(m_pRenderDevice, nullptr, "", width, height, 4, TextureUsage::RTV, Format::kRGBA16F);

		//Create a camera
		m_camera = std::make_unique<Camera>(math::Nvec3(15.0f, 50.0f, 0.0f), 0.2f, 0.0f, 0.4 * 3.14f, m_pWindow->GetWidth(), m_pWindow->GetHeight(), 1.0f, 10000.0f);

		//Load and set up models
		m_pCrate = std::make_shared<Model>(m_pRenderDevice, sg::Shape::CUBE, "Crate");
		Transform crateTransform({ 10.0f, 70.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
		m_pCrate->SetTransform(crateTransform);
		m_pCrate->SetMaterial(crateMaterialName, "cube");

		m_pModels.push_back(m_pCrate);

		m_pSphereGenerated = std::make_shared<Model>(m_pRenderDevice, sg::Shape::SPHERE, "SphereGenerated");
		Transform sphereGeneratedTransform({ 10.0f, 50.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 4.0f, 4.0f, 4.0f });
		m_pSphereGenerated->SetTransform(sphereGeneratedTransform);
		m_pModels.push_back(m_pSphereGenerated);

		m_pSpherePBR = AssetImporter::Instance().LoadModel("..\\resources\\Models\\SphereFBX\\sphere.fbx");
		Transform spherePBRTransform({ 50.0f, 50.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 4.0f, 4.0f, 4.0f });
		m_pSpherePBR->SetMaterial(kDefaultPBRMaterial, m_pSpherePBR->GetRootNode()->m_name);

		m_pSpherePBR->SetTransform(spherePBRTransform);
		m_pSpherePBR->SetName("PBRShpere");
		m_pModels.push_back(m_pSpherePBR);

		m_pSpherePhong = AssetImporter::Instance().LoadModel("..\\resources\\Models\\SphereFBX\\sphere.fbx");
		Transform spherePhongTransform({ 50.0f, 50.0f, 20.0f }, { 0.0f, 0.0f, 0.0f }, { 4.0f, 4.0f, 4.0f });
		m_pSpherePhong->SetMaterial(kDefaultMaterial, "Sphere");
		m_pSpherePhong->SetName("PhongShpere");
		m_pSpherePhong->SetTransform(spherePhongTransform);

		m_pModels.push_back(m_pSpherePhong);

		m_pGLTFModel = AssetImporter::Instance().LoadModel("..\\resources\\Models\\fire\\scene.gltf");
		Transform GLTFModelTransform({ 0.0f, 00.0f, 0.0f }, { kPI * 3.0f/2.0f, 0.0f, 0.0f}, { 10.0f, 10.0f, 10.0f });
		m_pGLTFModel->SetTransform(GLTFModelTransform);

		m_pModels.push_back(m_pGLTFModel);

		//TODO:Give the model the default transform.
		/*m_pSponza = AssetImporter::Instance().LoadModel("..\\resources\\Models\\Sponza\\sponza.obj");
		Transform sponzaTransform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
		m_pSponza->SetTransform(sponzaTransform);
		m_pModels.push_back(m_pSponza);*/
		
		//Load a skybox
		std::array<std::string, 6> cubeMapPaths{"..\\resources\\Models\\skybox\\right.jpg",
												"..\\resources\\Models\\skybox\\left.jpg",
												"..\\resources\\Models\\skybox\\top.jpg",
												"..\\resources\\Models\\skybox\\bottom.jpg",
												"..\\resources\\Models\\skybox\\front.jpg",
												"..\\resources\\Models\\skybox\\back.jpg" };

		m_pSkybox = AssetImporter::Instance().LoadCubeMap(cubeMapPaths);

		//Add some  lights to the scene
		m_pLights.push_back(std::make_shared<NoctisLight>(NoctisLight(m_pRenderDevice, NoctisLight::Type::kDirectional, { 0.1f, 0.1f, 0.1f, 1.0f }, math::Nvec3::Zero(), { -0.2f, -1.0f, -0.3f })));
		m_pLights.push_back(std::make_shared<NoctisLight>(NoctisLight(m_pRenderDevice, NoctisLight::Type::kPoint, { 2.0f, 2.0f, 2.0f, 1.0f }, { 0.0f, 50.0f, 30.0f }, math::Nvec3::Zero(), { 1.0f, 0.0022f, 0.00019f })));


		//Create shaders
		m_pVShader = std::make_shared<VertexShader>(m_pRenderDevice, L"../resources/Shaders/PhongLightingVertexShader.hlsl");
		m_pPShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/PhongLightingPixelShader.hlsl");
		m_pFlatVShader = std::make_shared<VertexShader>(m_pRenderDevice, L"../resources/Shaders/FlatVertexShader.hlsl");
		m_pFlatPShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/FlatPixelShader.hlsl");
		m_pPBRPShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/PBRPixelShader.hlsl");
		m_pSkyboxVShader = std::make_shared<VertexShader>(m_pRenderDevice, L"../resources/Shaders/SkyboxVertexShader.hlsl");
		m_pSkyboxPShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/SkyboxPixelShader.hlsl");
		m_pHdrVShader = std::make_shared<VertexShader>(m_pRenderDevice, L"../resources/Shaders/HdrVertexShader.hlsl");
		m_pHdrPShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/HdrPixelShader.hlsl");
		m_pGoochPShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/GoochPixelShader.hlsl");
		m_pFullScreenFilterPShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/FullScreenNegativePixelShader.hlsl");
		m_pFullScreenGrayscalePShader = std::make_shared<PixelShader>(m_pRenderDevice, L"../resources/Shaders/FullScreenGrayscalePixelShader.hlsl");
		
		//Set input layouts for the different shaders
		m_pHdrVShader->SetInputLyout({});

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

		//Create pipeline passes 
		m_pForwardPass.reset(new ForwardPipelinePass<Model>(m_pRenderDevice, m_hdrRTV, m_camera));
		m_pForwardPass->AddIRenderables(m_pSpherePhong, m_pSphereGenerated, m_pCrate);
		m_pForwardPass->AddShaders(m_pVShader, m_pPShader);

		m_pForwardPBRPass.reset(new ForwardPipelinePass<Model>(m_pRenderDevice, m_hdrRTV, m_camera));
		m_pForwardPBRPass->AddIRenderables(m_pSpherePBR, m_pGLTFModel);
		m_pForwardPBRPass->AddShaders(m_pVShader, m_pPBRPShader);
		
		m_pForwardGoochPass.reset(new ForwardPipelinePass<Model>(m_pRenderDevice, m_hdrRTV, m_camera));
		m_pForwardGoochPass->AddIRenderables(m_pSphereGenerated);
		m_pForwardGoochPass->AddShaders(m_pVShader, m_pGoochPShader);

		m_pToneMapHdrPass.reset(new PostProcessPipelinePass<Texture>(m_pRenderDevice, {}, m_camera));
		m_pToneMapHdrPass->AddResources(m_hdrRTV);
		m_pToneMapHdrPass->AddShaders(m_pHdrVShader, m_pHdrPShader);

		m_pSkyboxPipelinePass.reset(new ForwardPipelinePass<CubeMap>(m_pRenderDevice, m_hdrRTV, m_camera));
		m_pSkyboxPipelinePass->AddIRenderables(m_pSkybox);
		m_pSkyboxPipelinePass->AddShaders(m_pSkyboxVShader, m_pSkyboxPShader);

		m_pFlatPipelinePass.reset(new ForwardPipelinePass<NoctisLight>(m_pRenderDevice, m_hdrRTV, m_camera));
		m_pFlatPipelinePass->AddIRenderables(m_pLights[0], m_pLights[1]);
		m_pFlatPipelinePass->AddShaders(m_pFlatVShader, m_pFlatPShader);

		m_pFullScreenFilterPass.reset(new PostProcessPipelinePass<Texture>(m_pRenderDevice, {}, m_camera));
		m_pFullScreenFilterPass->AddResources(m_filterRTV);
		m_pFullScreenFilterPass->AddShaders(m_pHdrVShader, m_pFullScreenFilterPShader);


		m_pFullScreenGrayscalePass.reset(new PostProcessPipelinePass<Texture>(m_pRenderDevice, {}, m_camera));
		m_pFullScreenGrayscalePass->AddResources(m_grayscaleRTV);
		m_pFullScreenGrayscalePass->AddShaders(m_pHdrVShader, m_pFullScreenGrayscalePShader);

		//------------------------------- Breaks abstraction layers ----------------------------------------------------

		rdr::DepthStencilState test(m_pRenderDevice, DepthStencilType<DepthType<DepthWriteAll>, StencilType<StencilOpWrite>>{});
		


		//______________________________________________________________________________________________________
		m_pConstantPerFrame.reset(m_pRenderDevice->CreateBuffer<rdr::ConstantBuffer, CBFrameData>(true, &cbFrameData));
		cbFrameData.ambient = 0.03f;
		
		return true;
	}





	void ExampleDx11App::Update(float dt)
	{
		//Update the camera position
		cbFrameData.eyePos = m_camera->GetPosition();

		if (m_enableGammaCorrection)
			cbFrameData.gammaCorrection = gamma;
		else
			cbFrameData.gammaCorrection = 1.0f;

		if (m_enabledNormalMapping)
			cbFrameData.enabledNormalMapping = true;
		else
			cbFrameData.enabledNormalMapping = false;

		//Update Lights
		cbFrameData.dirLight.diffuse = m_pLights[0]->GetColor();
		cbFrameData.dirLight.direction = m_pLights[0]->GetTransform().rotation;
		cbFrameData.dirLight.enabled = m_pLights[0]->m_enabled;

		std::for_each(std::begin(cbFrameData.pointLights), std::end(cbFrameData.pointLights), [](auto& light) { light.enabled = false; });
		for (int i = 1; i < m_pLights.size(); ++i)
		{
			cbFrameData.pointLights[i -1].attenuation = m_pLights[i]->GetAttenuation();
			cbFrameData.pointLights[i -1].diffuse = m_pLights[i]->GetColor();
			cbFrameData.pointLights[i -1].enabled = m_pLights[i]->m_enabled;
			cbFrameData.pointLights[i -1].position = m_pLights[i]->GetTransform().position;
		}
		m_pConstantPerFrame->Update(m_pRenderDevice, cbFrameData);
		m_pConstantPerFrame->Bind(m_pRenderDevice, 1);
	}





	void ExampleDx11App::Render(float dt)
	{
		//TODO: This clears the default rtv but not the hdr render target view which is bound in PipelinePass and 
		m_pRenderDevice->Clear(kRender | kDepth | kStencil);
	
		m_pForwardPBRPass->Render(m_pRenderDevice);
		m_pSkyboxPipelinePass->Render(m_pRenderDevice);
#if _DEBUG
		m_pFlatPipelinePass->Render(m_pRenderDevice);
#endif //_DEBUG

		m_pForwardPass->Render(m_pRenderDevice);
		m_pForwardGoochPass->Render(m_pRenderDevice);

		//Reads the hdrRTV and draws to filterRTV, grayscaleRTV or to the screen
		if (m_enableNegativeFilter)
			m_pToneMapHdrPass->Render(m_pRenderDevice, m_filterRTV);
		else if (m_enableGrayscale)
			m_pToneMapHdrPass->Render(m_pRenderDevice, m_grayscaleRTV);
		else
			m_pToneMapHdrPass->Render(m_pRenderDevice);

		//Reads the filterRTV and draws to the grayscaleRTV or to the screen
		if (m_enableNegativeFilter && m_enableGrayscale)
			m_pFullScreenFilterPass->Render(m_pRenderDevice, m_grayscaleRTV);
		else if (m_enableNegativeFilter)
			m_pFullScreenFilterPass->Render(m_pRenderDevice);

		if (m_enableGrayscale)
			m_pFullScreenGrayscalePass->Render(m_pRenderDevice);

#if NOCTIS_USE_IMGUI
		RenderToImGui();
#endif //NOCTIC_USE_IMGUI
	}





#if NOCTIS_USE_IMGUI
	//-----------------------------------------------------------------------------
	// ExampleDx11App::RenderToImGui() : Place here all that has to be rendered by ImGui.
	//-----------------------------------------------------------------------------
	void ExampleDx11App::RenderImGuiFrame()
	{
		//Lambda to create a Help marker
		auto HelpMarker = [](const char* desc)
		{
			ImGui::TextDisabled("(?)");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(desc);
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		};

		ImGui::Begin("Menu");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		if (ImGui::CollapsingHeader("Help"))
		{
			ImGui::Text("USER GUIDE:");
			ImGui::BulletText("WASD or Arrows to move around the scene.");
			ImGui::BulletText("Mouse to look around the scene.");
			ImGui::BulletText("Block the mouse with CTRL + ALT + Z.");
		}

		if (ImGui::CollapsingHeader("Configuration"))
		{
			ImGui::DragFloat("Gamma", &gamma, 0.1f, 1.0f, 10.0f);
			ImGui::Checkbox("Gamma Correction", &m_enableGammaCorrection);
			ImGui::SameLine();
			HelpMarker("If the texture is coloured it contains both Metallic and Roughness.");
			ImGui::Checkbox("Negative Filter", &m_enableNegativeFilter);
			ImGui::Checkbox("Grayscale Filter", &m_enableGrayscale);
			ImGui::Checkbox("Blur Filter", &m_enableBlurFilter);
			ImGui::Checkbox("Normal Mapping", &m_enabledNormalMapping);
			if (ImGui::Button("Add Light"))
			{
				AddLight();
			}
			ImGui::DragFloat("Ambient", &cbFrameData.ambient, 0.01f, 0.0f, 1.0f);
		}

		//A recursive lambda to render the Node based representation of a mesh.
		auto renderTree = [](const auto renderTree, Node* node, uint8_t& index, uint8_t& selectedIndex, Node** outSelected) -> void
		{
			uint8_t currentIndex = ++index;
			//Don't add nodes with only one child
			if (node->m_pNodes.size() > 0)
			{
				bool node_open = ImGui::TreeNodeEx(node->m_name.c_str(), selectedIndex != -1 && currentIndex == selectedIndex ? ImGuiTreeNodeFlags_Selected : 0);
				selectedIndex = ImGui::IsItemClicked() ? currentIndex : selectedIndex;
				
				if (node_open)
				{
					for (const auto& children : node->m_pNodes)
					{
						renderTree(renderTree, children.get(), index, selectedIndex, outSelected);
					}
					ImGui::TreePop();
				}
			}
			else if (node->m_meshes.size())
			{
				ImGui::TreeNodeEx(node->m_name.c_str(), (selectedIndex != -1 && currentIndex == selectedIndex ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
				selectedIndex = ImGui::IsItemClicked() ? currentIndex : selectedIndex;
			}
			//Otherwise do not add the current node, however try to render it's children.
			else
			{
				for (const auto& children : node->m_pNodes)
				{
					renderTree(renderTree, children.get(), index, selectedIndex, outSelected);
				}
			}
			if (currentIndex == selectedIndex)
				*outSelected = node;
		};

		//Unfortunatelly due to some flaws of the current design, Lights need a separate function as it is currently impossible to retrieve the light object
		//from the Node which would be returned by the renderTree function. This will need to be restructured into a single interface. For the time being 
		//lights are not allowed to have hierarchical structures.
		auto renderLight = [this](NoctisLight* light, uint8_t& index, uint8_t& selectedIndex, NoctisLight** outSelected) -> void
		{
			uint8_t currentIndex = ++index;
			auto node = light->GetModel(m_pRenderDevice).GetRootNode();
			ImGui::TreeNodeEx(node->m_name.c_str(), (selectedIndex != -1 && currentIndex == selectedIndex ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
			selectedIndex = ImGui::IsItemClicked() ? currentIndex : selectedIndex;
			if (currentIndex == selectedIndex)
				*outSelected = light;
		};

		//Lambda which renders the passed texture as a small 16x16 square and displays a tooltip of size 256x256 showing the image on hover.
		auto& renderTextureImg = [](auto& pTex)
		{
			float iconSize = 16;
			//Arbitrary color to represent an empty image.
			static ImColor color = { 150, 150, 150, 100 };
			if (!pTex)
			{
				ImGui::ColorButton("EmptyImageLabel", *(ImVec4*)&color, ImGuiColorEditFlags_NoTooltip, ImVec2(iconSize, iconSize));
				ImGui::SameLine();
				return;
			}
			//TODO:: SRVs are DirectX construct hence this should be done on a lower level of abstraction.
			//		 For OpenGL and other rendering APIs ImTextureID must be assigned a different ID type.
			ImTextureID texID = pTex->GetSRV().Get();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::Image(texID, ImVec2(iconSize, iconSize));
			ImGui::SameLine();
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				float tooltipSize = 256.0f;
				ImGui::Image(texID, ImVec2(tooltipSize, tooltipSize));
				ImGui::EndTooltip();
			}
		};

		//Lambda which renders the material passed to it.
		auto& renderMaterial = [&HelpMarker, &renderTextureImg, this](auto& material)
		{
			//To keep track of metallic-roughness textures.
			bool hasMetallic = true;
			auto materialType = material->GetType();
			if (materialType == Material::MaterialType::Legacy)
			{
				PhongMaterial* ptr = reinterpret_cast<PhongMaterial*>(material.get());
				ImGui::Checkbox("Use Blinn-Phong", (bool*)&ptr->GetData().blinn);
			}
			for (unsigned int i = 0; i < TextureUsage::COUNT; ++i)
			{
				auto& pTex = material->GetTexture(static_cast<TextureUsage>(i));
				renderTextureImg(pTex);
				bool renderText = true;
				if (materialType == Material::MaterialType::PBR)
				{
					if (!pTex && i == TextureUsage::ALBEDO)
					{
						renderText = false;
						int misc_flags = ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions;
						PBRMaterial* ptr = reinterpret_cast<PBRMaterial*>(material.get());
						ImGui::ColorEdit3(TextureUsageToString(materialType, static_cast<TextureUsage>(i)), (float*)&ptr->GetData().albedo, misc_flags);
					}
					if (!pTex && i == TextureUsage::METALLIC)
					{
						renderText = false;
						hasMetallic = false;
						
						ImGui::SameLine();
						PBRMaterial* ptr = reinterpret_cast<PBRMaterial*>(material.get());
						ImGui::DragFloat(TextureUsageToString(materialType, static_cast<TextureUsage>(i)), &ptr->GetData().metallic, 0.01f, 0.0f, 1.0f);
					}
					else if (!pTex && !hasMetallic && i == TextureUsage::ROUGHNESS)
					{
						renderText = false;
						ImGui::SameLine();
						PBRMaterial* ptr = reinterpret_cast<PBRMaterial*>(material.get());
						ImGui::DragFloat(TextureUsageToString(materialType, static_cast<TextureUsage>(i)), &ptr->GetData().roughness, 0.01f, 0.05f, 1.0f);
					}
				}
				else if (materialType == Material::MaterialType::Legacy)
				{
					if (!pTex && i == TextureUsage::DIFFUSE)
					{
						renderText = false;
						int misc_flags = ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions;
						PhongMaterial* ptr = reinterpret_cast<PhongMaterial*>(material.get());
						ImGui::ColorEdit3(TextureUsageToString(materialType, static_cast<TextureUsage>(i)), (float*)&ptr->GetData().diffuse, misc_flags);
					}
					else if (!pTex && i == TextureUsage::SPECULAR)
					{
						renderText = false;
						int misc_flags = ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions;
						PhongMaterial* ptr = reinterpret_cast<PhongMaterial*>(material.get());
						ImGui::ColorEdit3(TextureUsageToString(materialType, static_cast<TextureUsage>(i)), (float*)&ptr->GetData().specular, misc_flags);
						ImGui::DragFloat("Specular Power", (float*)&ptr->GetData().specular.w, 1.0f, 1.0f, 1024.0f);

					}
				}
				if (renderText)
				{
					ImGui::Text("%s", TextureUsageToString(materialType, static_cast<TextureUsage>(i)));
					if (i == TextureUsage::METALLIC)
					{
						ImGui::SameLine();
						HelpMarker("If the texture is coloured it contains both Metallic and Roughness.");
					}
				}

				material->Update(m_pRenderDevice);	
			}
		};


		Node* GLTFModel = nullptr;
		NoctisLight* selectedLight = nullptr;

		if (ImGui::CollapsingHeader("Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
		{
			uint8_t index = 0;
			static uint8_t selectedIndex = 0;
			for (auto& model : m_pModels)
			{
				auto root = model->GetRootNode();
				renderTree(renderTree, root, index, selectedIndex, &GLTFModel);
			}

			for (auto& light : m_pLights)
			{
				renderLight(light.get(), index, selectedIndex, &selectedLight);
			}
		}
		if (selectedLight)
		{
			ImGui::Separator();
			auto& GLTFModelTransform = selectedLight->GetTransform();
			ImGui::Text("%s", "Light");
			ImGui::DragFloat3("Position", &GLTFModelTransform.position.x, 0.1f);
			ImGui::DragFloat3("Rotation", &GLTFModelTransform.rotation.x, 0.01f);
			ImGui::DragFloat3("Scale", &GLTFModelTransform.scale.x, 0.01f);
			ImGui::Checkbox("Enable", (bool*)&selectedLight->m_enabled);

			int misc_flags = ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions;
			ImGui::ColorEdit3("Color", (float*)&selectedLight->GetColor(), misc_flags);
			ImGui::SameLine(); HelpMarker("Click on the colored square to open a color picker.\nCTRL+click on individual component to input value.\n");
			ImGui::DragFloat3("Attenuation", &selectedLight->GetAttenuation().x, 0.01f, 0.01f, 1.0f);
		}
		else if (GLTFModel)
		{
			ImGui::Separator();
			auto& GLTFModelTransform = GLTFModel->m_transform;
			ImGui::Text("%s", GLTFModel->m_name.c_str());
			ImGui::DragFloat3("Position", &GLTFModelTransform.position.x, 0.1f);
			ImGui::DragFloat3("Rotation", &GLTFModelTransform.rotation.x, 0.1f);
			ImGui::DragFloat3("Scale", &GLTFModelTransform.scale.x, 0.1f);
			if (GLTFModel->m_meshes.size())
			{
				ImGui::Separator();
				ImGuiIO& io = ImGui::GetIO();
				auto material = GLTFModel->m_pOwner->GetMaterial(GLTFModel->m_pOwner->GetMeshes()[GLTFModel->m_meshes[0]]->GetName());
				ImGui::Text("%s", material->GetName().c_str());
				
				renderMaterial(material);
			}
		}
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


	void ExampleDx11App::AddLight()
	{
		m_pLights.push_back(std::make_shared<NoctisLight>(NoctisLight(m_pRenderDevice, NoctisLight::Type::kPoint, { 2.0f, 2.0f, 2.0f, 1.0f }, { 0.0f, 50.0f, 0.0f }, math::Nvec3::Zero(), { 1.0f, 0.0022f, 0.00019f })));
		m_pFlatPipelinePass->AddIRenderables(m_pLights.back());
	}
}