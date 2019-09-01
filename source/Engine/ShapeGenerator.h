#pragma once

namespace noctis::rdr
{
	struct Vertex;
}
//---------------------------------------------------------------------------------
//Return type from all methods of Shape Generator.
//---------------------------------------------------------------------------------
struct Data
{
	std::vector<noctis::rdr::Vertex>			vertices;
	std::vector<unsigned int>	indices;
	//std::vector<Texture>		textures;
	unsigned int				stride;
	unsigned int				offset;
};
namespace sg
{ 

	enum class Shape
	{
		SPHERE,
		CUBE,
		CYLINDER,
		CONE,
		PLANE,
		TRIANGLE,
		SKULL
	};
	constexpr const char* kShapeNameCube		= "cube";
	constexpr const char* kShapeNameSphere		= "sphere";
	constexpr const char* kShapeNamePlane		= "plane";
	constexpr const char* kShapeNameSkull		= "skull";

	
	
	
	using namespace DirectX;



class ShapeGenerator
{
public:
	enum class Shape {CUBE, SPHERE, CYLINDER};
	static ShapeGenerator& Instance() 
	{
		static ShapeGenerator instance;
		return instance;
	}

	void CreateCube(float width, float height, float depth, Data& data);
	void CreateSphere(float, UINT, UINT, Data&);
	//void CreateCylinder(float topRadius, float bottomRadius, float height, UINT sliceCount, Data& data);
	//void CreateCone(float radius, float height, UINT sliceCount, Data& data);
	void CreatePlane(float width, float depth, UINT wDivisions , UINT hDivisions, Data& data);

	void CreateSkull(Data& data);

private:
	float GenerateHeight(float x, float z) const { return (0.3f * (z*cos(0.15f * x) + x * sin(0.2f * z))); }
};

}