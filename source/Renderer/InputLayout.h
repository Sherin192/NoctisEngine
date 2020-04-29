#ifndef INPUT_LAYOUT_H
#define INPUT_LAYOUT_H

#include "Core_pch.h"


namespace noctis::rdr 
{
//------------------------------------------------------------------------------------
//		Forward Declarations:
//------------------------------------------------------------------------------------
	class RenderDevice;
	class VertexShader;

//====================================================================================





//------------------------------------------------------------------------------------
//		Constants:
//------------------------------------------------------------------------------------

	constexpr const uint8_t kMaxLayoutElements = 5;

	static constexpr const int16_t autoOffset = -1;

//====================================================================================




//------------------------------------------------------------------------------------
//	VertexElement :	API agnostic representation of an vertex element and it's properties.
//------------------------------------------------------------------------------------
	struct VertexElement
	{
		enum Format
		{
			kFloat4,
			kFloat3,
			kFloat2,
			kFloat1,
			kHFloat4,		//HalfFloat
			kUInt4,
			kUInt3,
			kUInt2,
			kUInt1,
			kUNormShort4
			//Extend if needed
		};

		enum Type
		{
			kPosition,
			kNormal,
			kTangent,
			kBitangent,
			kTexCoord
		};

		Type type;
		int8_t semanticIndex;
		Format format;
		int16_t offset = autoOffset;
	};

//====================================================================================





//------------------------------------------------------------------------------------
//	VertexElementLayout :	Abstract base class that represents a vertex layout, API dependent 
//							derived classes implement the behaviour.
//							The class is only constructible with the Factory method Create.
//------------------------------------------------------------------------------------
	class VertexElementLayout
	{
	public:
		VertexElementLayout() = delete;

		struct Factory
		{
			static std::unique_ptr<VertexElementLayout> Create(std::initializer_list<VertexElement>&& elements);
		};

		//Public interface
		virtual void Bind(std::shared_ptr<RenderDevice>& renderDevice, VertexShader& shader) = 0;
		virtual void Bind(std::shared_ptr<RenderDevice>& renderDevice) = 0;

		const uint16_t	GetStride() const noexcept { return stride; }
		const uint32_t	GetOffset() const noexcept { return offset; }
		const auto		GetNumberElements() const noexcept { return m_elements.size(); }

	protected:
		VertexElementLayout(std::initializer_list<VertexElement>&& elements)
			: m_elements(elements), stride(), offset()
		{}
		std::vector<VertexElement>					m_elements;
		uint16_t stride;
		uint32_t offset;
	};

//====================================================================================





//------------------------------------------------------------------------------------
// FormatToSize() : Returns the size in bytes for a given format.
//------------------------------------------------------------------------------------
	constexpr uint16_t FormatToSize(VertexElement& elem)
	{
		switch (elem.format)
		{
		case VertexElement::Format::kFloat4:
			return sizeof(float) * 4;
		case VertexElement::Format::kFloat3:
			return sizeof(float) * 3;
		case VertexElement::Format::kFloat2:
			return sizeof(float) * 2;
		case VertexElement::Format::kFloat1:
			return sizeof(float);
		case VertexElement::Format::kHFloat4:
			return sizeof(float) * 2;
		case VertexElement::Format::kUInt4:
			return sizeof(uint32_t) * 4;
		case VertexElement::Format::kUInt3:
			return sizeof(uint32_t) * 3;
		case VertexElement::Format::kUInt2:
			return sizeof(uint32_t) * 2;
		case VertexElement::Format::kUInt1:
			return sizeof(uint32_t);
		case VertexElement::Format::kUNormShort4:
			return sizeof(uint16_t) * 4;
		default:
			//TODO: deal with this in some constructive way.
			return 0;
		}

	}

//====================================================================================
}	//noctis::rdr
#endif //INPUT_LAYOUT_H
