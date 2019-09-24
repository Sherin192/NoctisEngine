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
	class Model;

	enum CBufferType { kModel, kFrame, kCount};


	class PipelinePass
	{
	public:
		PipelinePass(std::shared_ptr<RenderDevice>& renderDevice)
		{
		}


		void AddShaders(std::weak_ptr<VertexShader> vShader, std::weak_ptr<noctis::rdr::PixelShader> pShader)
		{
			m_pVertexShader = vShader.lock();
			m_pPixelShader = pShader.lock();
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

		void Render(std::shared_ptr<RenderDevice>& renderDevice, Model& model,  Camera& camera)
		{
			renderDevice->SetRasterizerState(RasterizerType::SOLID_CULL_BACK);

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