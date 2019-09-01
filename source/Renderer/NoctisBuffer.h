#ifndef _NOCTIS_BUFFER_H
#define _NOCTIS_BUFFER_H

#include "Noctis3DTypes.h"
#include "NoctisRenderDevice.h"
namespace noctis::rdr
{
//------------------------------------------------------------------------------------
//		Forward Declarations:
//------------------------------------------------------------------------------------
	template <typename DataType>
	class ConstantBuffer;
	template <typename DataType>
	class VertexBuffer;
	template <typename DataType>
	class IndexBuffer;

//====================================================================================








//------------------------------------------------------------------------------------
//		Dx11 Base Class for Buffers:
//------------------------------------------------------------------------------------

	template <typename BufferType, typename DataType>
	class Dx11Buffer
	{
	public:
		using value_type = DataType;

		Dx11Buffer() : m_pBuffer() {};

		Dx11Buffer(RenderDevice* renderDevice);
		Dx11Buffer(std::shared_ptr<RenderDevice>& renderDevice);

		Dx11Buffer(RenderDevice* renderDevice, bool dynamic, DataType* data, uint32_t n_elem = 1);
		Dx11Buffer(std::shared_ptr<RenderDevice>& renderDevice, bool dynamic, DataType* data, uint32_t n_elem = 1);
		
		void Init(RenderDevice* renderDevice, bool dynamic, DataType* data, size_t n_elem = 1);
		void Init(std::shared_ptr<RenderDevice>& renderDevice, bool dynamic, DataType* data, size_t n_elem = 1);

		void Bind(std::shared_ptr<RenderDevice>& renderDevice, uint32_t slot = 0);

		void Update(std::shared_ptr<RenderDevice>& renderDevice, DataType& data);

		void Swap(Dx11Buffer& other) noexcept;

	private:

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
	};

//====================================================================================




	//====================================================================================
	// Constructors and initialization functions.
	//====================================================================================
	template <typename BufferType, typename DataType>
	Dx11Buffer<BufferType, DataType>::Dx11Buffer(RenderDevice* renderDevice)
		: m_pBuffer()
	{
		Init(renderDevice, true, nullptr);
	}

	template <typename BufferType, typename DataType>
	Dx11Buffer<BufferType, DataType>::Dx11Buffer(std::shared_ptr<RenderDevice>& renderDevice)
		: Dx11Buffer(renderDevice.get())
	{}






	template <typename BufferType, typename DataType>
	Dx11Buffer<BufferType, DataType>::Dx11Buffer(RenderDevice* renderDevice, bool dynamic, DataType* data, uint32_t n_elem /*= 1*/)
	{
		Init(renderDevice, dynamic, data, n_elem);
	}


	template <typename BufferType, typename DataType>
	Dx11Buffer<BufferType, DataType>::Dx11Buffer(std::shared_ptr<RenderDevice>& renderDevice, bool dynamic, DataType* data, uint32_t n_elem /*= 1*/)
		:Dx11Buffer(renderDevice.get(), dynamic, data, n_elem)
	{}





	template <typename BufferType, typename DataType>
	void Dx11Buffer<BufferType, DataType>::Init(RenderDevice* renderDevice, bool dynamic, DataType* data, size_t n_elem /*= 1*/)
	{
		BufferType* buffer = static_cast<BufferType*>(this);

		HRESULT hResult;
		::D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(::D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(DataType) * n_elem;
		desc.MiscFlags = 0;

		using BaseType = std::decay_t<decltype(*buffer)>;
		if constexpr (std::is_same_v<ConstantBuffer<DataType>, BaseType>)
		{
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		}
		else if constexpr (std::is_same_v<VertexBuffer<DataType>, BaseType>)
		{
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		}
		else if constexpr (std::is_same_v<IndexBuffer<DataType>, BaseType>)
		{
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		}
		if (dynamic)
		{
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.CPUAccessFlags = 0;
		}

		D3D11_SUBRESOURCE_DATA init_data = {};
		if (data)
			init_data.pSysMem = data;

		HRESULT hr = renderDevice->GetDevice()->CreateBuffer(&desc, data ? &init_data : NULL, m_pBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			printf("Log\n");
		}
	}

	template <typename BufferType, typename DataType>
	void Dx11Buffer<BufferType, DataType>::Init(std::shared_ptr<RenderDevice>& renderDevice, bool dynamic, DataType* data, size_t n_elem /*= 1*/)
	{
		Init(renderDevice.get(), dynamic, data, n_elem);
	}

	//====================================================================================
	// Public Interface.
	//====================================================================================

	template <typename BufferType, typename DataType>
	void Dx11Buffer<BufferType, DataType>::Bind(std::shared_ptr<RenderDevice>& renderDevice, uint32_t slot /*= 0*/)
	{
		assert(m_pBuffer != nullptr);

		BufferType * buffer = static_cast<BufferType*>(this);

		using BaseType = std::decay_t<decltype(*buffer)>;
		if constexpr (std::is_same_v<ConstantBuffer<DataType>, BaseType>)
		{
			renderDevice->GetDeviceContext()->VSSetConstantBuffers(slot, 1, m_pBuffer.GetAddressOf());
			renderDevice->GetDeviceContext()->PSSetConstantBuffers(slot, 1, m_pBuffer.GetAddressOf());
		}
		else if constexpr (std::is_same_v<VertexBuffer<DataType>, BaseType>)
		{
			renderDevice->GetDeviceContext()->IASetVertexBuffers(0, 1, m_pBuffer.GetAddressOf(), &buffer->GetStride(), &buffer->GetOffset());
		}
		else if constexpr (std::is_same_v<IndexBuffer<DataType>, BaseType>)
		{
			renderDevice->GetDeviceContext()->IASetIndexBuffer(m_pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		}

	};






	template <typename BufferType, typename DataType>
	void Dx11Buffer<BufferType, DataType>::Update(std::shared_ptr<RenderDevice>& renderDevice, DataType& data)
	{
		if (!m_pBuffer)
			Init(renderDevice, true, &data);
		
		assert(m_pBuffer != nullptr);

		static_assert(sizeof(DataType) % 16 == 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		renderDevice->GetDeviceContext()->Map(m_pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		memcpy(mappedResource.pData, &data, sizeof(DataType));

		renderDevice->GetDeviceContext()->Unmap(m_pBuffer.Get(), 0);
	}







	template <typename BufferType, typename DataType>
	void Dx11Buffer<BufferType, DataType>::Swap(Dx11Buffer& other) noexcept
	{
		m_pBuffer.Swap(other.m_pBuffer);
	}

}	//noctis::



#endif //_NOCTIS_BUFFER_H
