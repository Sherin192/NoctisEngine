#ifndef _MATERIAL_H
#define _MATERIAL_H
#include "NoctisRenderDevice.h"
#include "NoctisBuffers.h"
namespace noctis::rdr
{

	constexpr auto kDefaultMaterial = "_DefaultMaterial";
	constexpr auto kDefaultPBRMaterial = "_DefaultPBRMaterial";
//------------------------------------------------------------------------------------
//		Material: 
//------------------------------------------------------------------------------------
class Material
{
public:
	Material() = default;
	Material(const std::string& name) noexcept : m_name(name) {}

	virtual void Update(std::shared_ptr<RenderDevice> renderDevice) {}
	virtual void Bind(std::shared_ptr<RenderDevice> renderDevice) {}
	virtual void AddTexture(std::shared_ptr<Texture> texture) {}
	const auto& GetName() { return m_name; }
	protected:
	std::string						m_name;
};
//====================================================================================





class PhongMaterial : public Material
{
	//------------------------------------------------------------------------------------
	//		GPUMaterial: This structure mirrors the GPU representation of a material.
	//------------------------------------------------------------------------------------
	struct GPUMaterial
	{
		//-----------------------------------
		math::Nvec4 ambient;					// 16 bytes
		//-----------------------------------
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

	PhongMaterial() : Material(kDefaultMaterial), m_material(defaultMaterial) {}
	PhongMaterial(const std::string& name) : Material(name), m_material(defaultMaterial) {}
	PhongMaterial(std::shared_ptr<RenderDevice>& renderDevice,const  std::string& name, GPUMaterial& material) 
		: Material(name), m_material(material)
	{
		m_cbuffer.Init(renderDevice, true, &m_material);
	}

	void SetAmbient(math::Nvec4 color) { m_material.ambient = color; }
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
	
	std::array<std::shared_ptr<Texture>, TextureUsage::COUNT> m_textures;
	std::shared_ptr<Texture>	m_diffuse;
	std::shared_ptr<Texture>	m_specular;
	std::shared_ptr<Texture>	m_normal;
	std::shared_ptr<Texture>	m_height;
};

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
	PBRMaterial() : Material(kDefaultPBRMaterial), m_material(defaultMaterial) {}
	PBRMaterial(std::shared_ptr<RenderDevice>& renderDevice, const std::string& name, PBRMaterialData& material) 
		: Material(name), m_material(material) 
	{ 
		m_cbuffer.Init(renderDevice, true, &m_material); 
	};

	PBRMaterial& operator= (PBRMaterial& other)
	{
		m_material = other.m_material;
		m_albedo = other.m_albedo;
		m_metallic = other.m_metallic;
		m_roughtness = other.m_roughtness;
		return *this;
	}

	void SetAlbedo(math::Nvec4 color) noexcept { m_material.albedo = color; }
	void SetMetallic(float value) noexcept { m_material.metallic = value; }
	void SetRoughtness(float value) noexcept { m_material.roughness = value; }
	void SetAlbedoTexture(std::shared_ptr<Texture> albedo) noexcept { m_albedo = albedo; SetTextureBitField(TextureUsage::DIFFUSE); }
	void SetMetallicTexture(std::shared_ptr<Texture> metallic) noexcept { m_metallic = metallic; SetTextureBitField(TextureUsage::NORMAL); }
	void SetRoughtnessTexture(std::shared_ptr<Texture> roughtness) noexcept { m_roughtness = roughtness; SetTextureBitField(TextureUsage::SPECULAR); }
	void Update(std::shared_ptr<RenderDevice> renderDevice) {
		m_cbuffer.Update(renderDevice, m_material);
	}
	void Bind(std::shared_ptr<RenderDevice> renderDevice) { m_cbuffer.Bind(renderDevice, 2); }
	auto& GetData() noexcept { return m_material; }
	static PBRMaterialData			defaultMaterial;

private:
	void SetTextureBitField(int texture) noexcept { m_material.textureBitField |= 1 << texture;; }
	ConstantBuffer<PBRMaterialData>	m_cbuffer;
	PBRMaterialData					m_material;
	std::shared_ptr<Texture>		m_albedo;
	std::shared_ptr<Texture>		m_metallic;
	std::shared_ptr<Texture>		m_roughtness;
};


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
}	//noctis::rdr
#endif //_MATERIAL_H