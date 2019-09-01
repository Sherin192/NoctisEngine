#ifndef PIPELINE_PASS_H
#define PIPELINE_PASS_H
#include "Core_pch.h"
#include "NoctisConstantBuffer.h"

namespace noctis {

	class PipelinePass
	{
	public:
		PipelinePass(std::weak_ptr<RenderDevice> rDevice, RenderState& rState)
		{
		
		}
		void AddConstantBuffer(ConstantBuffer&);
		template<typename First, typename... Rest>
		void AddShaders(First& shader, Rest&&... args)
		{
		
		}


	private:
		RenderState&						m_renderState;
		std::vector<ConstantBuffer>			m_constantBuffers;


	};
}

#endif PIPELINE_PASS_H
