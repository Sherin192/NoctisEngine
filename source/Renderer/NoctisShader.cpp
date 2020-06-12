#include "NoctisShader.h"

namespace noctis::rdr
{
		

//------------------------------------------------------------------------------------
//		VertexShader: Returns the VertexElementLayout of this shader.
//------------------------------------------------------------------------------------
	VertexElementLayout const* const VertexShader::GetInputLyout() const 
	{ 
		return m_layout.get(); 
	};
	
//====================================================================================





//------------------------------------------------------------------------------------
//		SetInputLyout: Creates a VertexElementLayout from the listof VertexElements.
//------------------------------------------------------------------------------------
	void VertexShader::SetInputLyout(std::initializer_list<VertexElement>&& layout) 
	{ 
		m_layout = VertexElementLayout::Factory::Create(std::forward<std::initializer_list<VertexElement>>(layout)); 
	};
	
//====================================================================================





//------------------------------------------------------------------------------------
//		BindInputLayout: Binds the imput layout associated with this shader.
//------------------------------------------------------------------------------------
	void VertexShader::BindInputLayout(std::shared_ptr<RenderDevice>& renderDevice) 
	{ 
		m_layout->Bind(renderDevice, *this); 
	}

//====================================================================================

}
