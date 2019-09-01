#ifndef VERTEX_H
#define VERTEX_H

using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;

namespace noctis::rdr
{
//---------------------------------------------------------------------------------
//Simple Vertex Structure.
//---------------------------------------------------------------------------------
struct Vertex
{
	Vertex() = default;

	Vertex(Vector3 _pos, Vector3 _normal, Vector3 _tangent, Vector2 _texCoord)
		: position(_pos), normal(_normal), tangent(_tangent), bitangent(Vector3::Zero), texCoord(_texCoord)
	{}
	Vertex(Vector3 _pos, Vector3 _normal, Vector3 _tangent, Vector3 _bitangent, Vector2 _texCoord)
		: position(_pos), normal(_normal), tangent(_tangent), bitangent(_bitangent), texCoord(_texCoord)
	{}

	Vector3 position;
	Vector3 normal;
	Vector3 tangent;
	Vector3 bitangent;
	Vector2 texCoord;
};
}
#endif //VERTEX_H
