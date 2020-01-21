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

	enum TextureUsage : unsigned int { DIFFUSE = 0, ALBEDO = 0, SPECULAR = 1, METALLIC = 1, NORMAL, HEIGHT = 3, ROUGHNESS = 3, OPACITY, EMISSIVE, AMBIENT_OCCLUSION, COUNT, CUBE_MAP, UNSPECIFIED };


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

	struct AddressWrap			{};
	struct AddressClamp			{};
	struct AddressMirror		{};
	struct AddressBorder		{};
	struct AddressMirrorOnce	{};

	struct CompareNever			{};
	struct CompareLess			{};
	struct CompareEqual			{};
	struct CompareLessEqual		{};
	struct CompareGreater		{};
	struct CompareNotEqual		{};
	struct CompareGreaterEqual	{};
	struct CompareAlways		{};

	//Supported border colors
	struct BorderBlackOpaque {};
	struct BorderBlackTransparent {};
	struct BorderWhiteOpaque{};
	struct BorderWhiteTransparent {};

	template <typename... Args>
	struct SamplerType : public Args... {};
	
	using SamplerPointWrapNever			= SamplerType<FilterPoint, AddressWrap, CompareNever>;
	using SamplerPointWrapLess			= SamplerType<FilterPoint, AddressWrap, CompareLess>;
	using SamplerPointWrapEqual			= SamplerType<FilterPoint, AddressWrap, CompareEqual>;
	using SamplerPointWrapLessEqual		= SamplerType<FilterPoint, AddressWrap, CompareLessEqual>;
	using SamplerPointWrapGreater		= SamplerType<FilterPoint, AddressWrap, CompareGreater>;
	using SamplerPointWrapNotEqual		= SamplerType<FilterPoint, AddressWrap, CompareNotEqual>;
	using SamplerPointWrapGreaterEqual	= SamplerType<FilterPoint, AddressWrap, CompareGreaterEqual>;
	using SamplerPointWrapAlways		= SamplerType<FilterPoint, AddressWrap, CompareAlways>;

	using SamplerPointClampNever		= SamplerType<FilterPoint, AddressClamp, CompareNever>;
	using SamplerPointClampLess			= SamplerType<FilterPoint, AddressClamp, CompareLess>;
	using SamplerPointClampEqual		= SamplerType<FilterPoint, AddressClamp, CompareEqual>;
	using SamplerPointClampLessEqual	= SamplerType<FilterPoint, AddressClamp, CompareLessEqual>;
	using SamplerPointClampGreater		= SamplerType<FilterPoint, AddressClamp, CompareGreater>;
	using SamplerPointClampNotEqual		= SamplerType<FilterPoint, AddressClamp, CompareNotEqual>;
	using SamplerPointClampGreaterEqual	= SamplerType<FilterPoint, AddressClamp, CompareGreaterEqual>;
	using SamplerPointClampAlways		= SamplerType<FilterPoint, AddressClamp, CompareAlways>;


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


	/*enum BufferType {
		kVertexBuffer,
		kIndexBuffer,
		kStaticConstantBuffer,
		kDynamicConstantBuffer
	};*/

	enum ClearTargetType : uint8_t {
		kDepth = BIT(0),
		kStencil = BIT(1),
		kRender = BIT(2)
	};
	enum ConstantBufferType {kStatic, kDynamic};
}
#endif //_NOCTIS_3D_TYPES_H