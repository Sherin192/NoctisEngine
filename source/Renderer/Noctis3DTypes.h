#ifndef _NOCTIS_3D_TYPES_H
#define _NOCTIS_3D_TYPES_H

//#include "Core_pch.h"

namespace noctis::rdr
{
//==================================================================================================
//	3DTypes :	This types use DirectX 11 as a model to define platfor agnostic interfaces to 
//				allow use of different API in the future.
//==================================================================================================


	class Texture;

	enum TextureType : unsigned int { DIFFUSE = 0, SPECULAR, NORMAL, HEIGHT, COUNT };


	enum Topology
	{
		kLineList,
		kTriangleList,
		kPatch3List
	};

	enum FillMode
	{
		kWireframe,
		kSolid
	};

	enum CullMode
	{
		kNone,
		kFront,
		kBack
	};

	struct ViewPort {
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
	};

	constexpr const uint8_t kMaxRenderTargets = 4;
	/*struct RenderTarget
	{
		Texture renderTarget;
	};*/
	/*enum FillMode
	{
		kWireframe,
		kSolid
	};


	enum CullMode
	{
		kNone,
		kFront,
		kBack
	};
*/

	//enum RasterizerType
	//{
	//	SOLID_CULL_BACK,
	//	SOLID_CULL_NONE,
	//	WIREFRAME_CULL_NONE,
	//	COUNT
	//};


	struct RasterizerDesc
	{
		FillMode fillMode;
		CullMode cullMode;

		bool cclockwise;
		bool depthClipEnable;
		bool scissorEnable;
		bool multisampleEnable;
		bool antialiasedLineEnable;
	};


	enum Blend
	{
		kZero,
		kOne,
		kSrcColor,
		kInvSrcColor,
		kSrcAlpha,
		kInvSrcAlpha,
		kDestAlpha,
		kInvDestAlpha,
		kDestColor,
		kInvDestColor,
		kSrcAlphaSat,
		kBlendFactor,
		kInvBlendFactor,
		kSrc1Color,
		kInvSrc1Color,
		kSrc1Alpha,
		kInvSrc1Alpha
	};

	enum BlendOp
	{
		kAdd,
		kSubtract,
		kRevSubtract,
		kMin,
		kMax
	};


	//enum StencilOp
	//{
	//	kKeep,
	//	kZero,
	//	kReplace,
	//	kIncrSat,
	//	kDecrSat,
	//	kInvert,
	//	kIncr,
	//	kDecr
	//};

	enum Comparison
	{
		kNever,
		kLess,
		kEqual,
		kLessEqual,
		kGreater,
		kNotEqual,
		kGreaterEqual,
		kAlways
	};


	//enum DepthWriteMask
	//{
	//	kZero,
	//	kAll
	//};


	//struct StencileDesc
	//{
	//	StencilOp failOp;
	//	StencilOp depthFailOp;
	//	StencilOp passOp;
	//	Comparison func;
//	};


	//struct DepthStencileDesc
	//{
	//	bool depthEnable;
	//	bool stencilEnable;
	//	
	//	DepthWriteMask depthWriteMask;
	//	Comparison depthFunc;
	//	uint8_t stencilReadMask;
	//	uint8_t stencilWriteMask;
	//	StencileDesc FrontFace;
	//	StencileDesc BackFace;
	//};


	//enum ResourceAccessType {kNone, KRead, kWrite};


	/*enum BufferType {
		kVertexBuffer,
		kIndexBuffer,
		kStaticConstantBuffer,
		kDynamicConstantBuffer
	};*/


	enum ConstantBufferType {kStatic, kDynamic};
}
#endif //_NOCTIS_3D_TYPES_H