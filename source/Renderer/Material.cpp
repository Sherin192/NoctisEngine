#include "Core_pch.h"
#include "Material.h"

namespace noctis::rdr
{

GPUMaterial Material::defaultMaterial = GPUMaterial{
										{ 1.0f, 1.0f, 1.0f, 1.0f },
										{ 1.0f, 1.0f, 1.0f, 1.0f },
										{ 1.0f, 1.0f, 1.0f, 256.0f },
										{ 1.0f, 1.0f, 1.0f, 1.0f },
										0, 0, 0, 0
};
}