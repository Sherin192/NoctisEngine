#include "NoctisShader.h"

namespace noctis::rdr
{

		VertexElementLayout const* const VertexShader::GetInputLyout() const 
		{ 
			return m_layout.get(); 
		};
		
		
		
		
		void VertexShader::SetInputLyout(std::initializer_list<VertexElement>&& layout) 
		{ 
			m_layout = VertexElementLayout::Factory::Create(std::forward<std::initializer_list<VertexElement>>(layout)); 
		};
		
		
		
		
		void VertexShader::BindInputLayout(std::shared_ptr<RenderDevice>& renderDevice) 
		{ 
			m_layout->Bind(renderDevice, *this); 
		}
}
