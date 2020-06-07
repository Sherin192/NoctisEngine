#ifndef NOCTIS_PIPELINE_PASS_H
#define NOCTIS_PIPELINE_PASS_H

#include "NoctisBuffers.h"
#include "NoctisShader.h"
namespace noctis
{
	class Camera;
}

namespace noctis::rdr{

	
	class VertexShader;
	class PixelShader;
	template<typename T>
	struct IRenderable;

	enum CBufferType { kModel, kFrame, kCount};

	class PipelinePass
	{
	public:
		PipelinePass(std::shared_ptr<RenderDevice>& renderDevice, std::shared_ptr<Texture>& renderTarget)
			: m_pRenderTarget(renderTarget)
		{}

	protected:
		std::shared_ptr<Texture> m_pRenderTarget;
	};


	template <typename T>
	class ForwardPipelinePass : public PipelinePass
	{
	public:
		ForwardPipelinePass(std::shared_ptr<RenderDevice>& renderDevice, std::shared_ptr<Texture>& renderTarget, std::shared_ptr<Camera>& camera)
			: PipelinePass(renderDevice, renderTarget), m_pCamera(camera)
		{}

		void AddShaders(std::shared_ptr<VertexShader>& vShader, std::shared_ptr<noctis::rdr::PixelShader>& pShader)
		{
			m_pVertexShader = vShader;
			m_pPixelShader = pShader;
		}
		

		void BindShaders(std::shared_ptr<RenderDevice>& renderDevice)
		{
			m_pVertexShader->Bind(renderDevice);
			m_pPixelShader->Bind(renderDevice);
		}



		void BindSamplers(std::shared_ptr<RenderDevice>& renderDevice)
		{
			static SamplerPool sampler_pool(renderDevice);

			m_pVertexShader->BindSampler(renderDevice, static_cast<uint8_t>(SamplerPool::SamplerTypeID::LinearWrap), sampler_pool.GetSampler(SamplerPool::SamplerTypeID::LinearWrap));
			m_pPixelShader->BindSampler(renderDevice, static_cast<uint8_t>(SamplerPool::SamplerTypeID::LinearWrap), sampler_pool.GetSampler(SamplerPool::SamplerTypeID::LinearWrap));
		}

		template<typename... Args, typename = typename std::enable_if<(std::is_same<Args, T>::value && ...)>::type>
		void AddIRenderables(std::shared_ptr<Args>... args)
		{
			(m_pRenderables.push_back(args), ...);
		}

		void Render(std::shared_ptr<RenderDevice>& renderDevice)
		{
			renderDevice->SetRenderTarget(m_pRenderTarget.get());

			renderDevice->SetPrimitiveTopology(kTriangleList);

			m_pVertexShader->BindInputLayout(renderDevice);
			BindSamplers(renderDevice);
			BindShaders(renderDevice);
			
			for (auto& renderable : m_pRenderables)
				renderable->Render(renderDevice, *m_pCamera);
			//model.Render(renderDevice, camera);
		}
	private:
		std::shared_ptr<Camera>											m_pCamera;
		std::shared_ptr<VertexShader>									m_pVertexShader;
		std::shared_ptr<PixelShader>									m_pPixelShader;
		std::vector<std::shared_ptr<IRenderable<T>>>					m_pRenderables;
	};

	template <typename T>
	class PostProcessPipelinePass : public PipelinePass
	{
	public:
		PostProcessPipelinePass(std::shared_ptr<RenderDevice> renderDevice, std::shared_ptr<Texture> renderTarget, std::shared_ptr<Camera> camera)
			: PipelinePass(renderDevice, renderTarget), m_pCamera(camera)
		{}

		void AddShaders(std::shared_ptr<VertexShader> vShader, std::shared_ptr<noctis::rdr::PixelShader> pShader)
		{
			m_pVertexShader = vShader;
			m_pPixelShader = pShader;
		}


		void BindShaders(std::shared_ptr<RenderDevice> renderDevice)
		{
			m_pVertexShader->Bind(renderDevice);
			m_pPixelShader->Bind(renderDevice);
		}

		template<typename... Args, typename = typename std::enable_if<(std::is_same<Args, T>::value && ...)>::type>
		void AddResources(std::shared_ptr<Args>... args)
		{
			(m_pResources.push_back(args), ...);
		}

		void Render(std::shared_ptr<RenderDevice> renderDevice, std::shared_ptr<Texture> rdv = nullptr)
		{
			//TODO:: (Requires research) This is a temporary rendering setup for a post process. Currently a vertex shader generating a triangle is assumes.
			//It might be necessary to change this in the future to make this more robust. Another thing is that Textures are assumed to be the only
			//resource needed for now.
			if (rdv)
			{
				renderDevice->SetRenderTarget(rdv.get());
				renderDevice->Clear(kRender | kDepth);
			}
			else
			{
				renderDevice->SetRenderTarget();
				renderDevice->Clear(kRender | kDepth);
			}

			renderDevice->SetPrimitiveTopology(kTriangleList);
			assert(m_pVertexShader->GetInputLyout()->GetNumberElements() == 0);
			m_pVertexShader->BindInputLayout(renderDevice);

			BindShaders(renderDevice);

			for (auto& resource : m_pResources)
				resource->Bind(renderDevice, AccessType::Read);
			renderDevice->GetDeviceContext()->Draw(3, 0);

			for (auto& resource : m_pResources)
				resource->Unbind(renderDevice, AccessType::Read);
		}
	private:
		std::shared_ptr<Camera>											m_pCamera;
		std::shared_ptr<VertexShader>									m_pVertexShader;
		std::shared_ptr<PixelShader>									m_pPixelShader;
		std::vector<std::shared_ptr<rdr::Texture>>						m_pResources;
	};
}

#endif //NOCTIS_PIPELINE_PASS_H