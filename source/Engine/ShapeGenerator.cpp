#include "Core_pch.h"
#include "ShapeGenerator.h"
#include "Renderer/NoctisRenderDevice.h"
#include "Renderer/Vertex.h"

namespace sg
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	using noctis::rdr::Vertex;

	void ShapeGenerator::CreateCube(float width, float height, float depth, Data& data)
	{
		//Reserve the space for the vertices and indices;
		data.vertices.reserve(24);
		data.indices.reserve(36);
		data.stride = sizeof(Vertex);
		data.offset = 0;

		float hWidth = 0.5f * width;
		float hHeight = 0.5f * height;
		float hDepth = 0.5f * depth;

		data.vertices.emplace_back(Vertex(Vector3(-hWidth, -hHeight, -hDepth), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f)));
		data.vertices.emplace_back(Vertex(Vector3(-hWidth, +hHeight, -hDepth), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)));
		data.vertices.emplace_back(Vertex(Vector3(+hWidth, +hHeight, -hDepth), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)));
		data.vertices.emplace_back(Vertex(Vector3(+hWidth, -hHeight, -hDepth), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f)));

		// Fill in the back face vertex data.
		data.vertices.emplace_back(Vertex(Vector3(-hWidth, -hHeight, +hDepth), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)));
		data.vertices.emplace_back(Vertex(Vector3(+hWidth, -hHeight, +hDepth), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)));
		data.vertices.emplace_back(Vertex(Vector3(+hWidth, +hHeight, +hDepth), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f)));
		data.vertices.emplace_back(Vertex(Vector3(-hWidth, +hHeight, +hDepth), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f)));

		// Fill in the top face vertex data.
		data.vertices.emplace_back(Vertex(Vector3(-hWidth, +hHeight, -hDepth), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)));
		data.vertices.emplace_back(Vertex(Vector3(-hWidth, +hHeight, +hDepth), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f)));
		data.vertices.emplace_back(Vertex(Vector3(+hWidth, +hHeight, +hDepth), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f)));
		data.vertices.emplace_back(Vertex(Vector3(+hWidth, +hHeight, -hDepth), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)));

		// Fill in the bottom face vertex data.
		data.vertices.emplace_back(Vertex(Vector3(-hWidth, -hHeight, -hDepth), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)));
		data.vertices.emplace_back(Vertex(Vector3(+hWidth, -hHeight, -hDepth), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)));
		data.vertices.emplace_back(Vertex(Vector3(+hWidth, -hHeight, +hDepth), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f)));
		data.vertices.emplace_back(Vertex(Vector3(-hWidth, -hHeight, +hDepth), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f)));

		// Fill in the left face vertex data.
		data.vertices.emplace_back(Vertex(Vector3(-hWidth, -hHeight, +hDepth), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)));
		data.vertices.emplace_back(Vertex(Vector3(-hWidth, +hHeight, +hDepth), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f)));
		data.vertices.emplace_back(Vertex(Vector3(-hWidth, +hHeight, -hDepth), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f)));
		data.vertices.emplace_back(Vertex(Vector3(-hWidth, -hHeight, -hDepth), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)));

		// Fill in the right face vertex data.
		data.vertices.emplace_back(Vertex(Vector3(+hWidth, -hHeight, -hDepth), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f)));
		data.vertices.emplace_back(Vertex(Vector3(+hWidth, +hHeight, -hDepth), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)));
		data.vertices.emplace_back(Vertex(Vector3(+hWidth, +hHeight, +hDepth), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)));
		data.vertices.emplace_back(Vertex(Vector3(+hWidth, -hHeight, +hDepth), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f)));
	
		data.indices = {	0, 1, 2, 0, 2, 3, 
							4, 5, 6, 4, 6, 7, 
							8, 9, 10, 8, 10, 11,
							12, 13, 14, 12, 14, 15,
							16, 17, 18, 16, 18, 19,
							20, 21, 22, 20, 22, 23
							};
	}

	void ShapeGenerator::CreateSphere(float radius, uint32_t stackCount, uint32_t sliceCount, Data& data)
	{
		data.vertices.push_back(Vertex(Vector3(0.0f, radius, 0.0f), Vector3(0.0f, +1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)));
		data.stride = sizeof(Vertex);
		data.offset = 0;

		float phiStep = DirectX::XM_PI / stackCount;
		float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

		for (uint32_t i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// Vertices of ring.
			for (uint32_t j = 0; j < sliceCount; ++j)
			{
				float theta = j * thetaStep;

				float x = radius * sinf(phi) * cosf(theta);
				float y = radius * cosf(phi);
				float z = radius * sinf(phi) * sinf(theta);

				Vector3 normal(x, y, z);
				normal.Normalize();
				Vector3 tangent;
				tangent.x = -radius * sinf(phi)*sinf(theta);
				tangent.y = 0.0f;
				tangent.z = radius * sinf(phi)*cosf(theta);
				tangent.Normalize();
				
				Vector2 texCoord(theta / M_2_PI, phi / M_PI);

				data.vertices.emplace_back(Vertex(Vector3(x, y, z), normal, tangent, texCoord));
			}
		}

		data.vertices.push_back(Vertex(Vector3(0.0f, -radius, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)));

		for (UINT i = 1; i < sliceCount; ++i)
		{
			data.indices.push_back(0);
			data.indices.push_back(i + 1);
			data.indices.push_back(i);
		}
		data.indices.push_back(0);
		data.indices.push_back(1);
		data.indices.push_back(sliceCount);

		UINT topIndex = 1;
		UINT ringVertexCount = sliceCount;

		for (UINT i = 0; i < stackCount - 2; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				if (j < sliceCount - 1)
				{
					data.indices.push_back(topIndex + i * ringVertexCount + j);
					data.indices.push_back(topIndex + i * ringVertexCount + j + 1);
					data.indices.push_back(topIndex + (i + 1)*ringVertexCount + j);

					data.indices.push_back(topIndex + (i + 1)*ringVertexCount + j);
					data.indices.push_back(topIndex + i * ringVertexCount + j + 1);
					data.indices.push_back(topIndex + (i + 1)*ringVertexCount + j + 1);
				}
				else
				{
					data.indices.push_back(topIndex + i * ringVertexCount + j);
					data.indices.push_back(topIndex + i * ringVertexCount);
					data.indices.push_back(topIndex + (i + 1) * ringVertexCount + j);

					data.indices.push_back(topIndex + (i + 1) * ringVertexCount + j);
					data.indices.push_back(topIndex + i * ringVertexCount);
					data.indices.push_back(topIndex + (i + 1)*ringVertexCount);
				}

			}
		}
		UINT bottomIndex = (UINT)data.vertices.size() - 1;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			data.indices.push_back(bottomIndex);
			data.indices.push_back(bottomIndex - ringVertexCount + i);
			data.indices.push_back(bottomIndex - ringVertexCount + i + 1);
		}

		data.indices.push_back(bottomIndex);
		data.indices.push_back(bottomIndex - 1);
		data.indices.push_back(bottomIndex - ringVertexCount);
	}
//
//	void ShapeGenerator::CreateCylinder(float topRadius, float bottomRadius, float height, UINT sliceCount, Data& data)
//	{
//		std::vector<Vertex> vertices;
//		std::vector<UINT> indices;
//
//		vertices.emplace_back(Vertex(0.0f, height / 2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));				//Top Vertex
//
//		float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;
//
//		// Vertices of top ring.
//		for (UINT j = 0; j < sliceCount; ++j)
//		{
//			float theta = j * thetaStep;
//
//			float x = topRadius * cosf(theta);
//			float y = height / 2;
//			float z = topRadius * sinf(theta);
//
//			vertices.emplace_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f));
//		}
//
//		for (UINT j = 0; j < sliceCount; ++j)
//		{
//			float theta = j * thetaStep;
//
//			float x = bottomRadius * cosf(theta);
//			float y = -height / 2;
//			float z = bottomRadius * sinf(theta);
//
//			vertices.emplace_back(Vertex(x, y, z, 1.0f, 0.0f, 0.0f, 1.0f));
//		}
//
//		vertices.emplace_back(Vertex(0.0f, -height / 2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));				//Bottom Vertex
//
//		for (UINT i = 1; i < sliceCount; ++i)
//		{
//			indices.push_back(0);
//			indices.push_back(i + 1);
//			indices.push_back(i);
//		}
//		indices.push_back(0);
//		indices.push_back(1);
//		indices.push_back(sliceCount);
//
//		UINT topIndex = 1;
//
//		for (UINT j = 0; j < sliceCount; ++j)
//		{
//			if (j < sliceCount - 1)
//			{
//				indices.push_back(topIndex + j);
//				indices.push_back(topIndex + j + 1);
//				indices.push_back(topIndex + sliceCount + j);
//
//				indices.push_back(topIndex + sliceCount + j);
//				indices.push_back(topIndex + j + 1);
//				indices.push_back(topIndex + sliceCount + j + 1);
//			}
//			else
//			{
//				indices.push_back(topIndex + j);
//				indices.push_back(topIndex);
//				indices.push_back(topIndex + sliceCount + j);
//
//				indices.push_back(topIndex + sliceCount + j);
//				indices.push_back(topIndex);
//				indices.push_back(topIndex + sliceCount);
//			}
//		}
//		UINT bottomIndex = (UINT)vertices.size() - 1;
//
//		for (UINT i = 0; i < sliceCount; ++i)
//		{
//			indices.push_back(bottomIndex);
//			indices.push_back(bottomIndex - sliceCount + i);
//			indices.push_back(bottomIndex - sliceCount + i + 1);
//		}
//
//		indices.push_back(bottomIndex);
//		indices.push_back(bottomIndex - 1);
//		indices.push_back(bottomIndex - sliceCount);
//
//		data.vertices = vertices;
//		data.indices = indices;
//	}
//
//	void ShapeGenerator::CreateCone(float radius, float height, UINT sliceCount, Data & data)
//	{
//		std::vector<Vertex> vertices;
//		std::vector<UINT> indices;
//
//		vertices.emplace_back(Vertex(0.0f, height / 2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));				//Top Vertex
//
//		float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;
//
//		// Vertices of bottom ring.
//		for (UINT j = 0; j < sliceCount; ++j)
//		{
//			float theta = j * thetaStep;
//
//			float x = radius * cosf(theta);
//			float y = -height / 2;
//			float z = radius * sinf(theta);
//
//			vertices.emplace_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f));
//		}
//
//		vertices.emplace_back(Vertex(0.0f, -height / 2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));				//Bottom Vertex
//
//		for (UINT i = 1; i < sliceCount; ++i)
//		{
//			indices.push_back(0);
//			indices.push_back(i + 1);
//			indices.push_back(i);
//		}
//		indices.push_back(0);
//		indices.push_back(1);
//		indices.push_back(sliceCount);
//
//		UINT bottomIndex = static_cast<UINT>(vertices.size()) - 1;
//
//		for (UINT i = 0; i < sliceCount; ++i)
//		{
//			indices.push_back(bottomIndex);
//			indices.push_back(bottomIndex - sliceCount + i);
//			indices.push_back(bottomIndex - sliceCount + i + 1);
//		}
//
//		indices.push_back(bottomIndex);
//		indices.push_back(bottomIndex - 1);
//		indices.push_back(bottomIndex - sliceCount);
//
//		data.vertices = vertices;
//		data.indices = indices;
//	}
//
	void ShapeGenerator::CreatePlane(float width, float depth, uint32_t wDivisions, uint32_t hDivisions, Data& data)
	{
		std::vector<Vertex> vertices;
		std::vector<UINT> indices;

		float halfWidth = 0.5f * width;
		float halfDepth = 0.5f * depth;

		float wStep = width / (wDivisions - 1);
		float hStep = depth / (hDivisions - 1);

		float uStep = 1.0f / (wDivisions - 1);
		float vStep = 1.0f / (hDivisions - 1);

		for (uint32_t i = 0; i < hDivisions; ++i)
		{
			for (uint32_t j = 0; j < wDivisions; ++j)
			{

				data.vertices.push_back(Vertex(Vector3(-halfWidth + j * wStep, 0.0f, halfDepth - i * hStep), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(j * uStep, i * vStep)));
				/*float y = GenerateHeight(-halfWidth + j * wStep, halfDepth - i * hStep);
				float r = 0.0f;
				float g = 0.0f;
				if (y > 15.0f)
					r = 1.0f;
				else if (y > 0.0f)
					r = y / 15.0f;
				else if (y > -15.0f)
					g = -y / 15.0f;
				else if (y < -15.0f)
					g = 1.0f;*/
				//vertices.emplace_back(Vertex(-halfWidth + j * wStep, y, halfDepth - i * hStep, r, g, 0.0f, 1.0f));
			}
		}

		//for (int i = 0; i < hDivisions; ++i)
		//{
		//	int j;
		//	j = i % 2 == 0 ? i * wDivisions  : i * wDivisions + 1;
		//	for (; j < wDivisions - 1; j++)
		//	{
		//		Vector3 edge1 = data.vertices[j].position - data.vertices[j + 1].position;
		//		Vector3 edge2 = data.vertices[j].position - data.vertices[(i + 1) * wDivisions + j].position;
		//		Vector2 deltaUV1 = data.vertices[j].texCoord - data.vertices[j + 1].texCoord;
		//		Vector2 deltaUV2 = data.vertices[j].texCoord - data.vertices[(i + 1) * wDivisions + j].texCoord;

		//		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		//		
		//		Vector3 bitangent;

		//		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		//		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		//		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		//		bitangent.Normalize();

		//		data.vertices[j].bitangent = bitangent;
		//		data.vertices[j + 1].bitangent = bitangent;
		//		data.vertices[(i + 1)* wDivisions + j].bitangent = bitangent;
		//	}
		//}

		for (uint32_t i = 0; i < hDivisions - 1; ++i)
		{
			for (uint32_t j = 0; j < wDivisions - 1; ++j)
			{
				indices.push_back(i * hDivisions + j);
				indices.push_back(i * hDivisions + j + 1);
				indices.push_back((i + 1) * hDivisions + j);

				indices.push_back((i + 1) * hDivisions + j);
				indices.push_back(i * hDivisions + j + 1);
				indices.push_back((i + 1)  * hDivisions + j + 1);
			}
		}

		//data.vertices = vertices;
		data.indices = indices;
	}
}