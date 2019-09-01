#ifndef NOCTIS_CONSTANT_BUFFER_H
#define NOCTIS_CONSTANT_BUFFER_H
//#include "Core_pch.h"
#include "NoctisBuffer.h"
#include "ConstantBufferData.h"
namespace noctis::rdr {

	enum class BufferType{kVertex, kIndex, kConstant};

	//TODO: When the openGL renderer is implemented the base class will have to be selected based on the chosen renderer
	template <typename DataType>
	class ConstantBuffer : public Dx11Buffer<ConstantBuffer<DataType>, DataType>
	{
		using Base = Dx11Buffer<ConstantBuffer<DataType>, DataType>;

	public:
		ConstantBuffer() = default;
		ConstantBuffer(RenderDevice* renderDevice) : Base(renderDevice) {}
		ConstantBuffer(std::shared_ptr<RenderDevice>& renderDevice) : Base(renderDevice) {}

		template <typename DataType>
		ConstantBuffer(RenderDevice* renderDevice, bool dynamic, DataType* data) : Base(renderDevice, dynamic, data) {}

		template <typename DataType>
		ConstantBuffer(std::shared_ptr<RenderDevice>& renderDevice, bool dynamic, DataType* data) : Base(renderDevice, dynamic, data) {}

	private:
		ConstantBuffer(ConstantBuffer&) = delete;
		ConstantBuffer& operator=(ConstantBuffer&) = delete;
	};




	template <typename DataType>
	class VertexBuffer : public Dx11Buffer<VertexBuffer<DataType>, DataType>
	{
		using Base = Dx11Buffer<VertexBuffer<DataType>, DataType>;
	public:

		VertexBuffer() = default;

		VertexBuffer(std::shared_ptr<RenderDevice>& renderDevice) : Base(renderDevice), m_stride(), m_offset() {}

		VertexBuffer(std::shared_ptr<RenderDevice>& renderDevice, bool dynamic, DataType* data, uint32_t n_elem, uint32_t stride, uint32_t offset) : Base(renderDevice, dynamic, data, n_elem), m_stride(stride), m_offset(offset) {}
		void Bind(std::shared_ptr<RenderDevice>& renderDevice)
		{
			Base::Bind(renderDevice);
		}

		void Swap(VertexBuffer& other) noexcept
		{
			Base::Swap(other);
			m_stride = other.m_stride;
			m_offset = other.m_offset;
		}


		void SetStride(const uint32_t stride) { m_stride = stride; }
		void SetOffset(const uint32_t offset) { m_offset = offset; }
		const uint32_t& GetStride() const noexcept { return m_stride; }
		const uint32_t& GetOffset() const noexcept { return m_offset; }

	private:
		VertexBuffer(VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&) = delete;
		uint32_t m_stride;
		uint32_t m_offset;
	};

	template <typename DataType = uint32_t>
	class IndexBuffer : public Dx11Buffer<IndexBuffer<DataType>, DataType>
	{
		using Base = Dx11Buffer<IndexBuffer<DataType>, DataType>;
	public:

		IndexBuffer() = default;

		IndexBuffer(std::shared_ptr<RenderDevice>& renderDevice) : Base(renderDevice) {}

		IndexBuffer(std::shared_ptr<RenderDevice>& renderDevice, bool dynamic, DataType* data, uint32_t n_elem) : Base(renderDevice, dynamic, data, n_elem) {}

	private:
		IndexBuffer(IndexBuffer&) = delete;
		IndexBuffer& operator=(IndexBuffer&) = delete;
	};

}

#endif //NOCTIS_CONSTANT_BUFFER_H