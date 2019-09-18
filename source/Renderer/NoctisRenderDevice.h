#ifndef _NOCTIS_RENDER_DEVICE_H
#define _NOCTIS_RENDER_DEVICE_H

#include "NoctisDx11RenderDevice.h"

namespace noctis::rdr
{

	class Sampler;

	template <typename DataType>
	class ConstantBuffer;

	template<typename Derived>
	class GLRenderDevice
	{
	public:

		//============================================================================================
		// Public Interface:
		//============================================================================================
		void Present() {};

		void SetRasterizerState(RasterizerType state) {};

		void ClearRenderTargetView() {};

		void ClearDepthStencilView(/*TODO pass flags and other arguments*/) {};

		/*template <template<typename> typename BufferType, typename DataType, typename... Args>
		auto CreateBuffer(Args... args) 
		{
			return new BufferType<DataType>(std::forward<Args>(args)...);
		}*/

		template <typename... Args>
		bool												Init(Args... args /*HWND*/)
		{
			return false;
		}
	};

	//TODO: the base class will be selected based on a compile time expression 
	//A compile time meta function which will return the desired API implementation of RenderDevice
	//RenderDeviceAPI<RenderDevice>::type <- will return APIRenderDevice<RenderDevice> 	
	class RenderDevice : public Dx11RenderDevice<RenderDevice>
	{
		using Base = Dx11RenderDevice<RenderDevice>;
		
		auto& GetImpl() { return static_cast<Base&>(*this); }

		public:

			auto& AsBase() { return GetImpl(); }
		//============================================================================================
		// Public Interface:
		//============================================================================================
			void Present();

			void SetRasterizerState(RasterizerType state);

			void ClearRenderTargetView();

			void ClearDepthStencilView(/*TODO pass flags and other arguments*/);

			void SetPrimitiveTopology(Topology topology);

			void SetSampler(uint32_t slot, uint32_t num, std::shared_ptr<Sampler>& sampler);

			template <template<typename> typename BufferType, typename DataType, typename... Args>
			auto CreateBuffer(Args... args);
			
		template <typename... Args>
		bool												Init(Args... args /*HWND*/)
		{
			return GetImpl().Init(std::forward<Args>(args)...);
		}
	private:



	};





	template <template<typename> typename BufferType, typename DataType, typename... Args>
	auto RenderDevice::CreateBuffer(Args... args)
	{
		return GetImpl().CreateBuffer<BufferType, DataType>(this, std::forward<Args>(args)...);
	};
}


#endif //_NOCTIS_RENDER_DEVICE_H
