#ifndef _NOCTIS_3D_TYPES_H
#define _NOCTIS_3D_TYPES_H

#define BIT(x) 1 << x

namespace noctis::rdr
{
	//==================================================================================================
	//	3DTypes :	This types use DirectX 11 as a model to define platfor agnostic interfaces to 
	//				allow use of different API in the future.
	//==================================================================================================



	class Texture;
	//Do not reorder those enums, and add only to the end just before UNSPECIFIED as they are used as slot values for shader resources.
	enum TextureUsage : unsigned int { DIFFUSE = 0, ALBEDO = 0, SPECULAR = 1, METALLIC = 1, NORMAL = 2, HEIGHT = 3, ROUGHNESS = 3, OPACITY = 4, EMISSIVE = 5, AMBIENT_OCCLUSION = 6, COUNT, CUBE_MAP, RTV, HDR, UNSPECIFIED };

	enum class AccessType { Read, Write, ReadWrite };

	enum ComparisonFunc
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


#define MakeFilterMinMagMip(type)	\
	struct FilterMin##type {};		\
	struct FilterMag##type {};		\
	struct FilterMip##type {}		

#define MakeAddressUVW(type)		\
	struct AddressU##type {};		\
	struct AddressV##type{};		\
	struct AddressW##type{}

	MakeFilterMinMagMip(Point);
	MakeFilterMinMagMip(Linear);
	MakeFilterMinMagMip(Anisotropic);
	struct FilterPoint : public FilterMinPoint, FilterMagPoint, FilterMipPoint {};
	struct FilterLinear : public FilterMinLinear, FilterMagLinear, FilterMipLinear {};
	struct FilterAnisotropic : public FilterMinAnisotropic, FilterMagAnisotropic, FilterMipAnisotropic {};

	MakeAddressUVW(Wrap);
	MakeAddressUVW(Clamp);
	MakeAddressUVW(Mirror);
	MakeAddressUVW(Border);
	MakeAddressUVW(MirrorOnce);

	struct AddressWrap : public AddressUWrap, AddressVWrap, AddressWWrap {};
	struct AddressClamp : public AddressUClamp, AddressVClamp, AddressWClamp {};
	struct AddressMirror : public AddressUMirror, AddressVMirror, AddressWMirror {};
	struct AddressBorder : public AddressUBorder, AddressVBorder, AddressWBorder {};
	struct AddressMirrorOnce : public AddressUMirrorOnce, AddressVMirrorOnce, AddressWMirrorOnce {};

	struct CompareNever {};
	struct CompareLess {};
	struct CompareEqual {};
	struct CompareLessEqual {};
	struct CompareGreater {};
	struct CompareNotEqual {};
	struct CompareGreaterEqual {};
	struct CompareAlways {};

	//Supported border colors
	struct BorderBlackOpaque {};
	struct BorderBlackTransparent {};
	struct BorderWhiteOpaque {};
	struct BorderWhiteTransparent {};

	template <typename... Args>
	struct SamplerType : public Args...
	{};

	//------------------------------------------------------------------------------------
	//		Point Filter Types
	//------------------------------------------------------------------------------------

	using SamplerPointWrapNever = SamplerType<FilterPoint, AddressWrap, CompareNever>;
	using SamplerPointWrapLess = SamplerType<FilterPoint, AddressWrap, CompareLess>;
	using SamplerPointWrapEqual = SamplerType<FilterPoint, AddressWrap, CompareEqual>;
	using SamplerPointWrapLessEqual = SamplerType<FilterPoint, AddressWrap, CompareLessEqual>;
	using SamplerPointWrapGreater = SamplerType<FilterPoint, AddressWrap, CompareGreater>;
	using SamplerPointWrapNotEqual = SamplerType<FilterPoint, AddressWrap, CompareNotEqual>;
	using SamplerPointWrapGreaterEqual = SamplerType<FilterPoint, AddressWrap, CompareGreaterEqual>;
	using SamplerPointWrapAlways = SamplerType<FilterPoint, AddressWrap, CompareAlways>;

	using SamplerPointClampNever = SamplerType<FilterPoint, AddressClamp, CompareNever>;
	using SamplerPointClampLess = SamplerType<FilterPoint, AddressClamp, CompareLess>;
	using SamplerPointClampEqual = SamplerType<FilterPoint, AddressClamp, CompareEqual>;
	using SamplerPointClampLessEqual = SamplerType<FilterPoint, AddressClamp, CompareLessEqual>;
	using SamplerPointClampGreater = SamplerType<FilterPoint, AddressClamp, CompareGreater>;
	using SamplerPointClampNotEqual = SamplerType<FilterPoint, AddressClamp, CompareNotEqual>;
	using SamplerPointClampGreaterEqual = SamplerType<FilterPoint, AddressClamp, CompareGreaterEqual>;
	using SamplerPointClampAlways = SamplerType<FilterPoint, AddressClamp, CompareAlways>;

	using SamplerPointMirrorNever = SamplerType<FilterPoint, AddressMirror, CompareNever>;
	using SamplerPointMirrorLess = SamplerType<FilterPoint, AddressMirror, CompareLess>;
	using SamplerPointMirrorEqual = SamplerType<FilterPoint, AddressMirror, CompareEqual>;
	using SamplerPointMirrorLessEqual = SamplerType<FilterPoint, AddressMirror, CompareLessEqual>;
	using SamplerPointMirrorGreater = SamplerType<FilterPoint, AddressMirror, CompareGreater>;
	using SamplerPointMirrorNotEqual = SamplerType<FilterPoint, AddressMirror, CompareNotEqual>;
	using SamplerPointMirrorGreaterEqual = SamplerType<FilterPoint, AddressMirror, CompareGreaterEqual>;
	using SamplerPointMirrorAlways = SamplerType<FilterPoint, AddressMirror, CompareAlways>;

	using SamplerPointBorderNever = SamplerType<FilterPoint, AddressBorder, CompareNever>;
	using SamplerPointBorderLess = SamplerType<FilterPoint, AddressBorder, CompareLess>;
	using SamplerPointBorderEqual = SamplerType<FilterPoint, AddressBorder, CompareEqual>;
	using SamplerPointBorderLessEqual = SamplerType<FilterPoint, AddressBorder, CompareLessEqual>;
	using SamplerPointBorderGreater = SamplerType<FilterPoint, AddressBorder, CompareGreater>;
	using SamplerPointBorderNotEqual = SamplerType<FilterPoint, AddressBorder, CompareNotEqual>;
	using SamplerPointBorderGreaterEqual = SamplerType<FilterPoint, AddressBorder, CompareGreaterEqual>;
	using SamplerPointBorderAlways = SamplerType<FilterPoint, AddressBorder, CompareAlways>;

	//====================================================================================

	//------------------------------------------------------------------------------------
	//		Linear Filter Types
	//------------------------------------------------------------------------------------

	using SamplerLinearWrapNever = SamplerType<FilterLinear, AddressWrap, CompareNever>;
	using SamplerLinearWrapLess = SamplerType<FilterLinear, AddressWrap, CompareLess>;
	using SamplerLinearWrapEqual = SamplerType<FilterLinear, AddressWrap, CompareEqual>;
	using SamplerLinearWrapLessEqual = SamplerType<FilterLinear, AddressWrap, CompareLessEqual>;
	using SamplerLinearWrapGreater = SamplerType<FilterLinear, AddressWrap, CompareGreater>;
	using SamplerLinearWrapNotEqual = SamplerType<FilterLinear, AddressWrap, CompareNotEqual>;
	using SamplerLinearWrapGreaterEqual = SamplerType<FilterLinear, AddressWrap, CompareGreaterEqual>;
	using SamplerLinearWrapAlways = SamplerType<FilterLinear, AddressWrap, CompareAlways>;

	using SamplerLinearClampNever = SamplerType<FilterLinear, AddressClamp, CompareNever>;
	using SamplerLinearClampLess = SamplerType<FilterLinear, AddressClamp, CompareLess>;
	using SamplerLinearClampEqual = SamplerType<FilterLinear, AddressClamp, CompareEqual>;
	using SamplerLinearClampLessEqual = SamplerType<FilterLinear, AddressClamp, CompareLessEqual>;
	using SamplerLinearClampGreater = SamplerType<FilterLinear, AddressClamp, CompareGreater>;
	using SamplerLinearClampNotEqual = SamplerType<FilterLinear, AddressClamp, CompareNotEqual>;
	using SamplerLinearClampGreaterEqual = SamplerType<FilterLinear, AddressClamp, CompareGreaterEqual>;
	using SamplerLinearClampAlways = SamplerType<FilterLinear, AddressClamp, CompareAlways>;

	using SamplerLinearMirrorNever = SamplerType<FilterLinear, AddressMirror, CompareNever>;
	using SamplerLinearMirrorLess = SamplerType<FilterLinear, AddressMirror, CompareLess>;
	using SamplerLinearMirrorEqual = SamplerType<FilterLinear, AddressMirror, CompareEqual>;
	using SamplerLinearMirrorLessEqual = SamplerType<FilterLinear, AddressMirror, CompareLessEqual>;
	using SamplerLinearMirrorGreater = SamplerType<FilterLinear, AddressMirror, CompareGreater>;
	using SamplerLinearMirrorNotEqual = SamplerType<FilterLinear, AddressMirror, CompareNotEqual>;
	using SamplerLinearMirrorGreaterEqual = SamplerType<FilterLinear, AddressMirror, CompareGreaterEqual>;
	using SamplerLinearMirrorAlways = SamplerType<FilterLinear, AddressMirror, CompareAlways>;

	using SamplerLinearBorderNever = SamplerType<FilterLinear, AddressBorder, CompareNever>;
	using SamplerLinearBorderLess = SamplerType<FilterLinear, AddressBorder, CompareLess>;
	using SamplerLinearBorderEqual = SamplerType<FilterLinear, AddressBorder, CompareEqual>;
	using SamplerLinearBorderLessEqual = SamplerType<FilterLinear, AddressBorder, CompareLessEqual>;
	using SamplerLinearBorderGreater = SamplerType<FilterLinear, AddressBorder, CompareGreater>;
	using SamplerLinearBorderNotEqual = SamplerType<FilterLinear, AddressBorder, CompareNotEqual>;
	using SamplerLinearBorderGreaterEqual = SamplerType<FilterLinear, AddressBorder, CompareGreaterEqual>;
	using SamplerLinearBorderAlways = SamplerType<FilterLinear, AddressBorder, CompareAlways>;

	//====================================================================================

//------------------------------------------------------------------------------------
//		Anisotropic Filter Types
//------------------------------------------------------------------------------------

	using SamplerAnisotropicWrapNever = SamplerType<FilterAnisotropic, AddressWrap, CompareNever>;
	using SamplerAnisotropicWrapLess = SamplerType<FilterAnisotropic, AddressWrap, CompareLess>;
	using SamplerAnisotropicWrapEqual = SamplerType<FilterAnisotropic, AddressWrap, CompareEqual>;
	using SamplerAnisotropicWrapLessEqual = SamplerType<FilterAnisotropic, AddressWrap, CompareLessEqual>;
	using SamplerAnisotropicWrapGreater = SamplerType<FilterAnisotropic, AddressWrap, CompareGreater>;
	using SamplerAnisotropicWrapNotEqual = SamplerType<FilterAnisotropic, AddressWrap, CompareNotEqual>;
	using SamplerAnisotropicWrapGreaterEqual = SamplerType<FilterAnisotropic, AddressWrap, CompareGreaterEqual>;
	using SamplerAnisotropicWrapAlways = SamplerType<FilterAnisotropic, AddressWrap, CompareAlways>;

	using SamplerAnisotropicClampNever = SamplerType<FilterAnisotropic, AddressClamp, CompareNever>;
	using SamplerAnisotropicClampLess = SamplerType<FilterAnisotropic, AddressClamp, CompareLess>;
	using SamplerAnisotropicClampEqual = SamplerType<FilterAnisotropic, AddressClamp, CompareEqual>;
	using SamplerAnisotropicClampLessEqual = SamplerType<FilterAnisotropic, AddressClamp, CompareLessEqual>;
	using SamplerAnisotropicClampGreater = SamplerType<FilterAnisotropic, AddressClamp, CompareGreater>;
	using SamplerAnisotropicClampNotEqual = SamplerType<FilterAnisotropic, AddressClamp, CompareNotEqual>;
	using SamplerAnisotropicClampGreaterEqual = SamplerType<FilterAnisotropic, AddressClamp, CompareGreaterEqual>;
	using SamplerAnisotropicClampAlways = SamplerType<FilterAnisotropic, AddressClamp, CompareAlways>;

	using SamplerAnisotropicMirrorNever = SamplerType<FilterAnisotropic, AddressMirror, CompareNever>;
	using SamplerAnisotropicMirrorLess = SamplerType<FilterAnisotropic, AddressMirror, CompareLess>;
	using SamplerAnisotropicMirrorEqual = SamplerType<FilterAnisotropic, AddressMirror, CompareEqual>;
	using SamplerAnisotropicMirrorLessEqual = SamplerType<FilterAnisotropic, AddressMirror, CompareLessEqual>;
	using SamplerAnisotropicMirrorGreater = SamplerType<FilterAnisotropic, AddressMirror, CompareGreater>;
	using SamplerAnisotropicMirrorNotEqual = SamplerType<FilterAnisotropic, AddressMirror, CompareNotEqual>;
	using SamplerAnisotropicMirrorGreaterEqual = SamplerType<FilterAnisotropic, AddressMirror, CompareGreaterEqual>;
	using SamplerAnisotropicMirrorAlways = SamplerType<FilterAnisotropic, AddressMirror, CompareAlways>;

	using SamplerAnisotropicBorderNever = SamplerType<FilterAnisotropic, AddressBorder, CompareNever>;
	using SamplerAnisotropicBorderLess = SamplerType<FilterAnisotropic, AddressBorder, CompareLess>;
	using SamplerAnisotropicBorderEqual = SamplerType<FilterAnisotropic, AddressBorder, CompareEqual>;
	using SamplerAnisotropicBorderLessEqual = SamplerType<FilterAnisotropic, AddressBorder, CompareLessEqual>;
	using SamplerAnisotropicBorderGreater = SamplerType<FilterAnisotropic, AddressBorder, CompareGreater>;
	using SamplerAnisotropicBorderNotEqual = SamplerType<FilterAnisotropic, AddressBorder, CompareNotEqual>;
	using SamplerAnisotropicBorderGreaterEqual = SamplerType<FilterAnisotropic, AddressBorder, CompareGreaterEqual>;
	using SamplerAnisotropicBorderAlways = SamplerType<FilterAnisotropic, AddressBorder, CompareAlways>;

	//====================================================================================
	enum FilterMode
	{
		kPoint,
		kLinear,
		kAnisotropic
	};

	enum AddressMode
	{
		kWrap,
		kMMirror,
		kClamp,
		kBorder,
		kMirrorOnce
	};

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

	enum Format
	{
		// RGBA
		kRGBA32F,
		kRGBA16F,
		kRGBA8UN,
		kRGBA8UN_SRGB,
		kD32F,
		kD24UNS8U
	};


	template <typename... Args>
	struct DepthType : public Args... {};

	struct DepthDisable {};
	struct DepthWriteAll {};
	struct DepthWriteZero {};

	using DWriteAll = DepthType<DepthWriteAll>;

	template <typename... Args>
	struct StencilType : public Args... {};

	template <typename DepthType, typename StencilType>
	struct DepthStencilType
	{
		using depthType = DepthType;
		using stencilType = StencilType;
	};
	
	struct StencilDisable {};

#define MakeStencilOpFail_DepthFail_Pass(type)		\
	struct StencilFailOp##type {};		\
	struct StencilDepthFailOp##type{};		\
	struct StencilPassOp##type{}

	MakeStencilOpFail_DepthFail_Pass(Keep);
	MakeStencilOpFail_DepthFail_Pass(Zero);
	MakeStencilOpFail_DepthFail_Pass(Replace);
	MakeStencilOpFail_DepthFail_Pass(IncrSat);
	MakeStencilOpFail_DepthFail_Pass(DecSat);
	MakeStencilOpFail_DepthFail_Pass(Invert);
	MakeStencilOpFail_DepthFail_Pass(Incr);
	MakeStencilOpFail_DepthFail_Pass(Decr);

	using StencilOpWrite = StencilType<StencilFailOpKeep, StencilDepthFailOpKeep, StencilPassOpReplace, CompareAlways>;
	using StencilOpMask = StencilType<StencilFailOpKeep, StencilDepthFailOpKeep, StencilPassOpKeep, CompareNotEqual>;

	//using test = DepthStencilType<DepthType<DepthWriteAll>, StencilType<StencilOpWrite>>

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

	enum ClearTargetType : uint8_t {
		kDepth = BIT(0),
		kStencil = BIT(1),
		kRender = BIT(2)
	};
	enum ConstantBufferType {kStatic, kDynamic};
}
#endif //_NOCTIS_3D_TYPES_H