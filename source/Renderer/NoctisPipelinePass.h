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
		PipelinePass(std::shared_ptr<RenderDevice>& renderDevice)
		{
		}


		void AddShaders(std::shared_ptr<VertexShader>& vShader, std::shared_ptr<noctis::rdr::PixelShader>& pShader)
		{
			m_pVertexShader = vShader;
			m_pPixelShader = pShader;
		}

		template <template<typename> typename CB, typename DataType>
		void BindConstantBuffers(std::shared_ptr<RenderDevice>& renderDevice, CB<DataType>* buffer)
		{
			buffer->Bind(renderDevice /*, slot*/);
		}

		template <template<typename> typename CB, typename DataType>
		void UpdateConstantBuffers(std::shared_ptr<RenderDevice>& renderDevice, CB<DataType>* buffer)
		{
			buffer->Update(renderDevice /*, slot*/);
		}

		void BindShaders(std::shared_ptr<RenderDevice>& renderDevice)
		{
			m_pVertexShader->Bind(renderDevice);
			m_pPixelShader->Bind(renderDevice);
		}

		template <typename T>
		void Render(std::shared_ptr<RenderDevice>& renderDevice, IRenderable<T>& model,  Camera& camera)
		{
			renderDevice->SetPrimitiveTopology(kTriangleList);

			m_pVertexShader->BindInputLayout(renderDevice);
			/*BindInputLayout(renderDevice);
			*/
			BindShaders(renderDevice);
			model.Render(renderDevice, camera);
		}

	private:
		//A basic pipeline pass needs at least the vertex and pixel shaders.
		std::shared_ptr<VertexShader>									m_pVertexShader;
		std::shared_ptr<PixelShader>									m_pPixelShader;

		//RenderState& m_renderState;
	};
}

#endif //NOCTIS_PIPELINE_PASS_H