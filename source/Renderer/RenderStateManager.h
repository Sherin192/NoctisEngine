#ifndef _RENDER_STATE_MANAGER_H
#define _RENDER_STATE_MANAGER_H

#include "Core_pch.h"
//
//namespace noctis
//{
//	
//
//
//
//	//==================================================================================================
//	//	RenderStateManager :	This class will hold all the different parts of a pipeline state.				
//	//==================================================================================================
//	class RenderState
//	{
//	public:
//
//		void SetTopology(Topology topology) { m_topology = topology; }
//		Topology GetTopology() const noexcept { return m_topology; }
//
//		void SetFillMode(FillMode mode) { m_fillMode = mode; }
//		FillMode GetFillMode() { return m_fillMode; }
//
//		void SetCullMode(CullMode mode) { m_cullMode = mode; }
//		CullMode GetCullMode() { return m_cullMode; }
//
//		//void SetInputLayout(VertexElementLayout& layout) { m_layout = layout; }
//		//VertexElementLayout GetInputLayout() { return m_layout; }
//
//		void SetViewPort(ViewPort view) { m_viewPort = view; }
//		ViewPort GetViewPort() { return m_viewPort; }
//
//		template <typename... Args>
//		void SetRenderTargets(Args&&... args) { m_renderTargets = { (args , ...) }; }

//		void SetSamplerState(...)
//	private:
//		Topology m_topology;
//		FillMode m_fillMode;
//		CullMode m_cullMode;
//		std::unique_ptr<VertexElementLayout> m_layout;
//		ViewPort m_viewPort;
//		ShaderID m_currShader;
//
//		std::array<RenderTarget, kMaxRenderTargets> m_renderTargets;
//	};
//}

#endif //_RENDER_STATE_MANAGER_H
