#ifndef _MATERIAL_H
#define _MATERIAL_H
#include "NoctisRenderDevice.h"
#include "NoctisBuffers.h"
namespace noctis::rdr
{

	constexpr auto kDefaultMaterial = "_DefaultMaterial";
	constexpr auto kDefaultPBRMaterial = "_DefaultPBRMaterial";
//------------------------------------------------------------------------------------
//		Material: Base class for all materials
//------------------------------------------------------------------------------------
class Material
{
public:
	enum class MaterialType { Legacy, PBR };
	Material() = default;
	Material(const std::string& name, MaterialType type) noexcept : m_name(name), m_type(type) {}

	virtual void Update(std::shared_ptr<RenderDevice> renderDevice) {}
	virtual void Bind(std::shared_ptr<RenderDevice> renderDevice) {}
	virtual void AddTexture(std::shared_ptr<Texture> texture) {}
	std::shared_ptr<Texture> GetTexture(TextureUsage usage) { return m_textures[usage]; }
	const auto& GetName() const noexcept { return m_name; }
	const auto& GetType() const noexcept { return m_type; }
	protected:
	std::string						m_name;
	MaterialType					m_type;
	std::array<std::shared_ptr<Texture>, TextureUsage::COUNT> m_textures;

};
//====================================================================================




//------------------------------------------------------------------------------------
//		PhongMaterial: 
//------------------------------------------------------------------------------------
class PhongMaterial : public Material
{
	//------------------------------------------------------------------------------------
	//		GPUMaterial: This structure mirrors the GPU representation of a material.
	//------------------------------------------------------------------------------------
	struct GPUMaterial
	{
		math::Nvec4 diffuse;					// 16 bytes
		//-----------------------------------
		math::Nvec4 specular;					// 16 bytes
		//-----------------------------------
		math::Nvec4 reflect;					// 16 bytes
		//-----------------------------------
		int textureBitField;					// 4  bytes
		float pad[3];							// 12 bytes
		//-----------------------------------
	};	//										   80 bytes total

public:

	PhongMaterial() : Material(kDefaultMaterial, MaterialType::Legacy), m_material(defaultMaterial) {}
	PhongMaterial(const std::string& name) : Material(name, MaterialType::Legacy), m_material(defaultMaterial) {}
	PhongMaterial(std::shared_ptr<RenderDevice>& renderDevice,const  std::string& name, GPUMaterial& material) 
		: Material(name, MaterialType::Legacy), m_material(material)
	{
		m_cbuffer.Init(renderDevice, true, &m_material);
	}

	//void SetAmbient(math::Nvec4 color) { m_material.ambient = color; }
	void SetDiffuse(math::Nvec4 color) { m_material.diffuse = color; }
	void SetSpecular(math::Nvec4 color) { m_material.specular = color; }
	void AddTexture(std::shared_ptr<Texture> texture);
	void Update(std::shared_ptr<RenderDevice> renderDevice) { m_cbuffer.Update(renderDevice, m_material);}
	void Bind(std::shared_ptr<RenderDevice> renderDevice);
	
	GPUMaterial& GetData() noexcept { return m_material; }
	
	static GPUMaterial			defaultMaterial;

private:
	void SetTextureBitField(int texture) noexcept { m_material.textureBitField |= 1 << texture;}
	
	ConstantBuffer<GPUMaterial>	m_cbuffer;
	GPUMaterial					m_material;
};
//====================================================================================





//------------------------------------------------------------------------------------
//		PBRMaterial:
//------------------------------------------------------------------------------------
class PBRMaterial : public Material
{
	//------------------------------------------------------------------------------------
	//		PBRMaterial: This structure mirrors the GPU representation of a PBR material.
	//------------------------------------------------------------------------------------
	struct PBRMaterialData
	{
		math::Nvec4 albedo;							// 16 bytes
		//-----------------------------------
		float metallic;								// 4  bytes
		float roughness;							// 4  bytes
		float ao;									// 4  bytes
		int textureBitField;						// 4  bytes
	};												// 32 bytes total
public:
	PBRMaterial() : Material(kDefaultPBRMaterial, MaterialType::PBR), m_material(defaultMaterial) {}
	PBRMaterial(const std::string& name) : Material(name, MaterialType::PBR), m_material(defaultMaterial) {}
	PBRMaterial(std::shared_ptr<RenderDevice>& renderDevice, const std::string& name, PBRMaterialData& material) 
		: Material(name, MaterialType::PBR), m_material(material)
	{ 
		m_cbuffer.Init(renderDevice, true, &m_material); 
	};
	void AddTexture(std::shared_ptr<Texture> texture);
	void Update(std::shared_ptr<RenderDevice> renderDevice) { m_cbuffer.Update(renderDevice, m_material); }
	void Bind(std::shared_ptr<RenderDevice> renderDevice);
	auto& GetData() noexcept { return m_material; }
	static PBRMaterialData			defaultMaterial;

private:
	void SetTextureBitField(int texture) noexcept { m_material.textureBitField |= 1 << texture;; }
	ConstantBuffer<PBRMaterialData>	m_cbuffer;
	PBRMaterialData					m_material;
};
//====================================================================================





//------------------------------------------------------------------------------------
//		MaterialPool: All materials have to be added to the Pool and the name 
//					  of the material should be used to access it.
//------------------------------------------------------------------------------------
class MaterialPool
{
public:

	void AddMaterial(const std::string& name, std::shared_ptr<Material> material)
	{

		auto [iter, insert] = m_materials.try_emplace(name, material);
		if (insert)
			m_materials[name]->Update(m_pRenderDevice);
	}

	std::shared_ptr<Material> GetMaterial(const std::string name)
	{

		if (m_materials.count(name))
		{
			return m_materials[name];
		}
		return {};
	}
	
	void Init(std::shared_ptr<RenderDevice>& renderDevice)
	{
		m_pRenderDevice = renderDevice;

		AddMaterial(kDefaultMaterial, std::make_shared<PhongMaterial>());
		AddMaterial(kDefaultPBRMaterial, std::make_shared<PBRMaterial>());
	}

	static MaterialPool& Instance()
	{
		static MaterialPool instance = MaterialPool();
		return instance;
	}
private:
	std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
	std::shared_ptr<RenderDevice> m_pRenderDevice;
};
//====================================================================================





constexpr const char* TextureUsageToString(Material::MaterialType type, TextureUsage usage)
{
	auto legacy = type == Material::MaterialType::Legacy;

	switch (usage)
	{
	case DIFFUSE:
		return legacy ? "Diffuse" : "Albedo";
	case SPECULAR:
		return legacy ? "Specular" : "Metallic";
	case NORMAL:
		return "Normal Map";
	case HEIGHT:
		return legacy ? "Height Map" : "Roughness";
	case OPACITY:
		return "Opacity";
	case EMISSIVE:
		return "Emissive";
	case AMBIENT_OCCLUSION:
		return "Ambient Occlusion";
	default:
		return "";
	}
}

}	//noctis::rdr
#endif //_MATERIAL_H