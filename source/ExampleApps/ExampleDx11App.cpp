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
		static_assert(sizeof(CBFrameData) % 16 == 0);

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
		m_pSpherePBR->SetMaterial(kDefaultPBRMaterial, m_pSpherePBR->GetRootNode()->m_name);

		m_pSpherePBR->SetTransform(spherePBRTransform);

		m_pModels.push_back(m_pSpherePBR);
		m_pModels2.insert({ m_pSpherePBR->GetRootNode()->m_name, m_pSpherePBR });

		m_pSpherePhong = AssetImporter::Instance().LoadModel("..\\resources\\Models\\SphereFBX\\sphere.fbx");
		Transform spherePhongTransform({ 50.0f, 50.0f, 20.0f }, { 0.0f, 0.0f, 0.0f }, { 4.0f, 4.0f, 4.0f });
		m_pSpherePhong->SetMaterial(kDefaultMaterial, "Sphere");

		m_pSpherePhong->SetTransform(spherePhongTransform);

		m_pModels.push_back(m_pSpherePhong);
		//-------------------------------------------------------------------------------------------------------------------------------
		
		m_pSkull = AssetImporter::Instance().LoadModel("..\\resources\\Models\\suzanne.obj");

		Transform skullTransform({ 0.0f, 50.0f, 10.0f }, { 1.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f });
		m_pSkull->SetTransform(skullTransform);
		
		m_pModels.push_back(m_pSkull);

		//--------------------------------------------------------------------------------------------------------------------------------------
		//Create a model
		m_pCrate = std::make_unique<Model>(m_pRenderDevice, sg::Shape::CUBE);

		m_pCrate->SetMaterial(crateMaterialName, sg::kShapeNameCube);

		Transform crateTransform({ 30.0f, 30.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
		m_pCrate->SetTransform(crateTransform);

		m_pMaterialPreview = AssetImporter::Instance().LoadModel("..\\resources\\Models\\materialpreview-subdiv.obj");
	
		Transform sphereTransform({ 50.0f, 50.0f, -20.0f }, { 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f });

		m_pMaterialPreview->SetTransform(sphereTransform);


		m_pMaterialPreview->SetMaterial(rustyMaterialName, "Cylinder.001_Cylinder.002");

		m_pModels.push_back(m_pMaterialPreview);

		//----------------------------------------------------------------------------------------------------------

		m_pGLTFModel = AssetImporter::Instance().LoadModel("..\\resources\\Models\\door\\scene.gltf");
		Transform GLTFModelTransform({ 0.0f, 00.0f, 0.0f }, { kPI * 3.0f/2.0f, 0.0f, 0.0f}, { 10.0f, 10.0f, 10.0f });
		m_pGLTFModel->SetTransform(GLTFModelTransform);

		m_pModels.push_back(m_pGLTFModel);



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

		m_pLights.push_back(std::make_shared<NoctisLight>(NoctisLight(m_pRenderDevice, NoctisLight::Type::kDirectional, { 0.5f, 0.5f, 0.5f, 1.0f }, math::Nvec3::Zero(), { -0.2f, -1.0f, -0.3f })));
		m_pLights.push_back(std::make_shared<NoctisLight>(NoctisLight(m_pRenderDevice, NoctisLight::Type::kPoint, { 1.0f, 0.6f, 0.0f, 1.0f }, { 0.0f, 50.0f, 30.0f }, math::Nvec3::Zero(), { 1.0f, 0.0022f, 0.00019f } )));

		return true;
	}





	void ExampleDx11App::Update(float dt)
	{
		cbFrameData.eyePos = m_camera->GetPosition();


		cbFrameData.dirLight.ambient = m_pLights[0]->GetColor();
		cbFrameData.dirLight.diffuse = m_pLights[0]->GetColor();
		cbFrameData.dirLight.specular = m_pLights[0]->GetColor();
		cbFrameData.dirLight.direction = m_pLights[0]->GetTransform().rotation;


		for (int i = 1; i < m_pLights.size(); ++i)
		{
			cbFrameData.pointLights[i].attenuation = m_pLights[i]->GetAttenuation();
			cbFrameData.pointLights[i].diffuse = m_pLights[i]->GetColor();
			cbFrameData.pointLights[i].enabled = m_pLights[i]->m_enabled;
			cbFrameData.pointLights[i].specular = m_pLights[i]->GetColor();
			cbFrameData.pointLights[i].position = m_pLights[i]->GetTransform().position;
		}
		m_pConstantPerFrame->Update(m_pRenderDevice, cbFrameData);
		m_pConstantPerFrame->Bind(m_pRenderDevice, 1);
		//-----------------------------------------------------

	}





	void ExampleDx11App::Render(float dt)
	{
		m_pRenderDevice->Clear(kRender | kDepth);

		
		//m_pPipelinePass->Render(m_pRenderDevice, *m_pCrate, *m_camera);
		//m_pPipelinePass->Render(m_pRenderDevice, *m_pSkull, *m_camera);
		//m_pPipelinePass->Render(m_pRenderDevice, *m_pSponza, *m_camera);
		m_pPipelinePass->Render(m_pRenderDevice, *m_pSpherePhong, *m_camera);
		m_pPBRPipelinePass->Render(m_pRenderDevice, *m_pSpherePBR, *m_camera);
		m_pPBRPipelinePass->Render(m_pRenderDevice, *m_pMaterialPreview, *m_camera);
		m_pPBRPipelinePass->Render(m_pRenderDevice, *m_pGLTFModel, *m_camera);
		m_pSkyboxPipelinePass->Render(m_pRenderDevice, *m_pSkybox, *m_camera);



#if _DEBUG
		for(auto& light : m_pLights)
			m_pFlatPipelinePass->Render(m_pRenderDevice, *light, *m_camera);
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
		ImGui::Begin("Menu");

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

		//A recursive lambda to render the Node based representation of a mesh.
		auto renderTree = [](const auto renderTree, Node* node, uint8_t& index, uint8_t& selectedIndex, Node** outSelected) -> void
		{
			uint8_t currentIndex = ++index;
			//Make sure that only nodes with at least one mesh are added to the hierarchy. 
			if (node->m_meshes.size() && ImGui::TreeNodeEx(node->m_name.c_str(), selectedIndex != -1 && currentIndex == selectedIndex ? ImGuiTreeNodeFlags_Selected : 0))
			{
				selectedIndex = ImGui::IsItemClicked() ? currentIndex : selectedIndex;
				for (const auto& children : node->m_pNodes)
				{
					renderTree(renderTree, children.get(), currentIndex, selectedIndex, outSelected);
				}
				ImGui::TreePop();
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
			if (node->m_meshes.size() && ImGui::TreeNodeEx(node->m_name.c_str(), selectedIndex != -1 && currentIndex == selectedIndex ? ImGuiTreeNodeFlags_Selected : 0))
			{
				selectedIndex = ImGui::IsItemClicked() ? currentIndex : selectedIndex;
				ImGui::TreePop();
			}
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
			ImGui::SliderFloat3("Position", &GLTFModelTransform.position.x, -1000.0f, 1000.0f);
			ImGui::SliderFloat3("Rotation", &GLTFModelTransform.rotation.x, 0.0f, 7.0f);
			ImGui::SliderFloat3("Scale", &GLTFModelTransform.scale.x, 0.2f, 10.0f);
			ImGui::Checkbox("Enable", (bool*)&selectedLight->m_enabled);

			int misc_flags = ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions;
			ImGui::ColorEdit3("Color", (float*)&selectedLight->GetColor(), misc_flags);
			ImGui::SameLine(); HelpMarker("Click on the colored square to open a color picker.\nCTRL+click on individual component to input value.\n");
			ImGui::SliderFloat3("Attenuation", &selectedLight->GetAttenuation().x, 0.0f, 1.0f);
		}
		else if (GLTFModel)
		{
			ImGui::Separator();
			auto& GLTFModelTransform = GLTFModel->m_transform;
			ImGui::Text("%s", GLTFModel->m_name.c_str());
			ImGui::SliderFloat3("Position", &GLTFModelTransform.position.x, -1000.0f, 1000.0f);
			ImGui::SliderFloat3("Rotation", &GLTFModelTransform.rotation.x, 0.0f, 7.0f);
			ImGui::SliderFloat3("Scale", &GLTFModelTransform.scale.x, 0.2f, 10.0f);
			//ImGui::DragFloat3("Scale", &GLTFModelTransform.scale.x, 0.1f);
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
}