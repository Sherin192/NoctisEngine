#ifndef _MATERIAL_H
#define _MATERIAL_H
#include "NoctisRenderDevice.h"

namespace noctis::rdr
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

//====================================================================================





//------------------------------------------------------------------------------------
//		Material: 
//------------------------------------------------------------------------------------
class Material
{
public:
	Material() noexcept : m_ID(m_nextID++), m_material(defaultMaterial) {};
	Material(GPUMaterial mat) noexcept
		: m_ID(m_nextID++), m_material(mat) {}
	void SetTextureBitField(int texture) noexcept { m_material.textureBitField |= 1 << texture; }
	GPUMaterial& GetGPUMaterial() noexcept { return m_material; }
	static GPUMaterial defaultMaterial;
private:
	inline static unsigned int		m_nextID = 0;
	unsigned int					m_ID;
	GPUMaterial						m_material;

};

//====================================================================================
}	//noctis::rdr
#endif //_MATERIAL_H