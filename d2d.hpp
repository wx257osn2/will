#pragma once
#include"com.hpp"
#include"_2dim.hpp"
#include"_resource_property.hpp"
#include"dwrite.hpp"
#include<dxgi1_2.h>
#include<wincodec.h>
#include<d2d1_1.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxguid.lib")
#include<functional>
#include<utility>

namespace will{
class d2d : public detail::resource<ID2D1Factory1>{
	using resource::resource;
	template<typename I>
	struct d2d_resource : detail::resource<I>{
		using resource::resource;
		d2d get_factory()const{ID2D1Factory1* ptr;(*this)->GetFactory(reinterpret_cast<ID2D1Factory**>(&ptr));return d2d{std::move(ptr)};}
	};
public:
	struct creation_property : detail::property<D2D1_CREATION_PROPERTIES>{
		creation_property(D2D1_THREADING_MODE th):property(){prop.threadingMode = th;}
#define PROPERTYDECL(name, type, membername) creation_property& name(type t){prop.membername = t;return *this;}
		PROPERTYDECL(threading_mode, D2D1_THREADING_MODE, threadingMode)
		PROPERTYDECL(debug_level, D2D1_DEBUG_LEVEL, debugLevel)
		PROPERTYDECL(option, D2D1_DEVICE_CONTEXT_OPTIONS, options)
#undef  PROPERTYDECL
	};
	struct fill_t{}static constexpr fill = {};
	static UINT32 rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b){return static_cast<UINT32>(r)<<16 | static_cast<UINT32>(g)<<8 | b;}
	struct alpha_mode{
		alpha_mode() = delete;
		struct{
			constexpr operator D2D1_ALPHA_MODE()const noexcept{return D2D1_ALPHA_MODE_UNKNOWN;}
			constexpr operator DXGI_ALPHA_MODE()const noexcept{return DXGI_ALPHA_MODE_UNSPECIFIED;}
		}static constexpr unknown = {}, unspecified = {};
		struct{
			constexpr operator D2D1_ALPHA_MODE()const noexcept{return D2D1_ALPHA_MODE_PREMULTIPLIED;}
			constexpr operator D2D1_COLORMANAGEMENT_ALPHA_MODE()const noexcept{return D2D1_COLORMANAGEMENT_ALPHA_MODE_PREMULTIPLIED;}
			constexpr operator D2D1_COLORMATRIX_ALPHA_MODE()const noexcept{return D2D1_COLORMATRIX_ALPHA_MODE_PREMULTIPLIED;}
			constexpr operator D2D1_BITMAPSOURCE_ALPHA_MODE()const noexcept{return D2D1_BITMAPSOURCE_ALPHA_MODE_PREMULTIPLIED;}
			constexpr operator DXGI_ALPHA_MODE()const noexcept{return DXGI_ALPHA_MODE_PREMULTIPLIED;}
		}static constexpr premultiplied = {};
		struct{
			constexpr operator D2D1_ALPHA_MODE()const noexcept{return D2D1_ALPHA_MODE_STRAIGHT;}
			constexpr operator D2D1_COLORMANAGEMENT_ALPHA_MODE()const noexcept{return D2D1_COLORMANAGEMENT_ALPHA_MODE_STRAIGHT;}
			constexpr operator D2D1_COLORMATRIX_ALPHA_MODE()const noexcept{return D2D1_COLORMATRIX_ALPHA_MODE_STRAIGHT;}
			constexpr operator D2D1_BITMAPSOURCE_ALPHA_MODE()const noexcept{return D2D1_BITMAPSOURCE_ALPHA_MODE_STRAIGHT;}
			constexpr operator DXGI_ALPHA_MODE()const noexcept{return DXGI_ALPHA_MODE_STRAIGHT;}
		}static constexpr straight = {};
		struct{
			constexpr operator D2D1_ALPHA_MODE()const noexcept{return D2D1_ALPHA_MODE_IGNORE;}
			constexpr operator DXGI_ALPHA_MODE()const noexcept{return DXGI_ALPHA_MODE_IGNORE;}
		}static constexpr ignore = {};
	};
	struct interpolation_mode{
		interpolation_mode() = delete;
		struct{
			constexpr operator D2D1_INTERPOLATION_MODE()const noexcept{return D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_BITMAP_INTERPOLATION_MODE()const noexcept{return D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_DPICOMPENSATION_INTERPOLATION_MODE()const noexcept{return D2D1_DPICOMPENSATION_INTERPOLATION_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_BITMAPSOURCE_INTERPOLATION_MODE()const noexcept{return D2D1_BITMAPSOURCE_INTERPOLATION_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_3DTRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_3DTRANSFORM_INTERPOLATION_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_3DPERSPECTIVETRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_3DPERSPECTIVETRANSFORM_INTERPOLATION_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_SCALE_INTERPOLATION_MODE()const noexcept{return D2D1_SCALE_INTERPOLATION_MODE_NEAREST_NEIGHBOR;}
		}static constexpr nearest_neighbor = {};
		struct{
			constexpr operator D2D1_INTERPOLATION_MODE()const noexcept{return D2D1_INTERPOLATION_MODE_LINEAR;}
			constexpr operator D2D1_BITMAP_INTERPOLATION_MODE()const noexcept{return D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;}
			constexpr operator D2D1_DPICOMPENSATION_INTERPOLATION_MODE()const noexcept{return D2D1_DPICOMPENSATION_INTERPOLATION_MODE_LINEAR;}
			constexpr operator D2D1_BITMAPSOURCE_INTERPOLATION_MODE()const noexcept{return D2D1_BITMAPSOURCE_INTERPOLATION_MODE_LINEAR;}
			constexpr operator D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR;}
			constexpr operator D2D1_3DTRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_3DTRANSFORM_INTERPOLATION_MODE_LINEAR;}
			constexpr operator D2D1_3DPERSPECTIVETRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_3DPERSPECTIVETRANSFORM_INTERPOLATION_MODE_LINEAR;}
			constexpr operator D2D1_SCALE_INTERPOLATION_MODE()const noexcept{return D2D1_SCALE_INTERPOLATION_MODE_LINEAR;}
		}static constexpr linear = {};
		struct{
			constexpr operator D2D1_INTERPOLATION_MODE()const noexcept{return D2D1_INTERPOLATION_MODE_CUBIC;}
			constexpr operator D2D1_DPICOMPENSATION_INTERPOLATION_MODE()const noexcept{return D2D1_DPICOMPENSATION_INTERPOLATION_MODE_CUBIC;}
			constexpr operator D2D1_BITMAPSOURCE_INTERPOLATION_MODE()const noexcept{return D2D1_BITMAPSOURCE_INTERPOLATION_MODE_CUBIC;}
			constexpr operator D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_CUBIC;}
			constexpr operator D2D1_3DTRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_3DTRANSFORM_INTERPOLATION_MODE_CUBIC;}
			constexpr operator D2D1_3DPERSPECTIVETRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_3DPERSPECTIVETRANSFORM_INTERPOLATION_MODE_CUBIC;}
			constexpr operator D2D1_SCALE_INTERPOLATION_MODE()const noexcept{return D2D1_SCALE_INTERPOLATION_MODE_CUBIC;}
		}static constexpr cubic = {};
		struct{
			constexpr operator D2D1_INTERPOLATION_MODE()const noexcept{return D2D1_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR;}
			constexpr operator D2D1_DPICOMPENSATION_INTERPOLATION_MODE()const noexcept{return D2D1_DPICOMPENSATION_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR;}
			constexpr operator D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR;}
			constexpr operator D2D1_3DTRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_3DTRANSFORM_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR;}
			constexpr operator D2D1_3DPERSPECTIVETRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_3DPERSPECTIVETRANSFORM_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR;}
			constexpr operator D2D1_SCALE_INTERPOLATION_MODE()const noexcept{return D2D1_SCALE_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR;}
		}static constexpr multi_sample_linear = {};
		struct{
			constexpr operator D2D1_INTERPOLATION_MODE()const noexcept{return D2D1_INTERPOLATION_MODE_ANISOTROPIC;}
			constexpr operator D2D1_DPICOMPENSATION_INTERPOLATION_MODE()const noexcept{return D2D1_DPICOMPENSATION_INTERPOLATION_MODE_ANISOTROPIC;}
			constexpr operator D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_ANISOTROPIC;}
			constexpr operator D2D1_3DTRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_3DTRANSFORM_INTERPOLATION_MODE_ANISOTROPIC;}
			constexpr operator D2D1_3DPERSPECTIVETRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_3DPERSPECTIVETRANSFORM_INTERPOLATION_MODE_ANISOTROPIC;}
			constexpr operator D2D1_SCALE_INTERPOLATION_MODE()const noexcept{return D2D1_SCALE_INTERPOLATION_MODE_ANISOTROPIC;}
		}static constexpr anisotropic = {};
		struct{
			constexpr operator D2D1_INTERPOLATION_MODE()const noexcept{return D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;}
			constexpr operator D2D1_DPICOMPENSATION_INTERPOLATION_MODE()const noexcept{return D2D1_DPICOMPENSATION_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;}
			constexpr operator D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE()const noexcept{return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;}
			constexpr operator D2D1_SCALE_INTERPOLATION_MODE()const noexcept{return D2D1_SCALE_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;}
		}static constexpr high_quality_cubic = {};
		static constexpr D2D1_BITMAPSOURCE_INTERPOLATION_MODE fant = D2D1_BITMAPSOURCE_INTERPOLATION_MODE_FANT;
		static constexpr D2D1_BITMAPSOURCE_INTERPOLATION_MODE mipmap_linear = D2D1_BITMAPSOURCE_INTERPOLATION_MODE_MIPMAP_LINEAR;
	};
	struct scale_mode{
		scale_mode() = delete;
		struct{
			constexpr operator D2D1_CONVOLVEMATRIX_SCALE_MODE()const noexcept{return D2D1_CONVOLVEMATRIX_SCALE_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_DISTANTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_DISTANTDIFFUSE_SCALE_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_DISTANTSPECULAR_SCALE_MODE()const noexcept{return D2D1_DISTANTSPECULAR_SCALE_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_POINTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_POINTDIFFUSE_SCALE_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_POINTSPECULAR_SCALE_MODE()const noexcept{return D2D1_POINTSPECULAR_SCALE_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_SPOTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_SPOTDIFFUSE_SCALE_MODE_NEAREST_NEIGHBOR;}
			constexpr operator D2D1_SPOTSPECULAR_SCALE_MODE()const noexcept{return D2D1_SPOTSPECULAR_SCALE_MODE_NEAREST_NEIGHBOR;}
		}static constexpr nearest_neighbor = {};
		struct{
			constexpr operator D2D1_CONVOLVEMATRIX_SCALE_MODE()const noexcept{return D2D1_CONVOLVEMATRIX_SCALE_MODE_LINEAR;}
			constexpr operator D2D1_DISTANTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_DISTANTDIFFUSE_SCALE_MODE_LINEAR;}
			constexpr operator D2D1_DISTANTSPECULAR_SCALE_MODE()const noexcept{return D2D1_DISTANTSPECULAR_SCALE_MODE_LINEAR;}
			constexpr operator D2D1_POINTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_POINTDIFFUSE_SCALE_MODE_LINEAR;}
			constexpr operator D2D1_POINTSPECULAR_SCALE_MODE()const noexcept{return D2D1_POINTSPECULAR_SCALE_MODE_LINEAR;}
			constexpr operator D2D1_SPOTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_SPOTDIFFUSE_SCALE_MODE_LINEAR;}
			constexpr operator D2D1_SPOTSPECULAR_SCALE_MODE()const noexcept{return D2D1_SPOTSPECULAR_SCALE_MODE_LINEAR;}
		}static constexpr linear = {};
		struct{
			constexpr operator D2D1_CONVOLVEMATRIX_SCALE_MODE()const noexcept{return D2D1_CONVOLVEMATRIX_SCALE_MODE_CUBIC;}
			constexpr operator D2D1_DISTANTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_DISTANTDIFFUSE_SCALE_MODE_CUBIC;}
			constexpr operator D2D1_DISTANTSPECULAR_SCALE_MODE()const noexcept{return D2D1_DISTANTSPECULAR_SCALE_MODE_CUBIC;}
			constexpr operator D2D1_POINTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_POINTDIFFUSE_SCALE_MODE_CUBIC;}
			constexpr operator D2D1_POINTSPECULAR_SCALE_MODE()const noexcept{return D2D1_POINTSPECULAR_SCALE_MODE_CUBIC;}
			constexpr operator D2D1_SPOTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_SPOTDIFFUSE_SCALE_MODE_CUBIC;}
			constexpr operator D2D1_SPOTSPECULAR_SCALE_MODE()const noexcept{return D2D1_SPOTSPECULAR_SCALE_MODE_CUBIC;}
		}static constexpr cubic = {};
		struct{
			constexpr operator D2D1_CONVOLVEMATRIX_SCALE_MODE()const noexcept{return D2D1_CONVOLVEMATRIX_SCALE_MODE_MULTI_SAMPLE_LINEAR;}
			constexpr operator D2D1_DISTANTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_DISTANTDIFFUSE_SCALE_MODE_MULTI_SAMPLE_LINEAR;}
			constexpr operator D2D1_DISTANTSPECULAR_SCALE_MODE()const noexcept{return D2D1_DISTANTSPECULAR_SCALE_MODE_MULTI_SAMPLE_LINEAR;}
			constexpr operator D2D1_POINTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_POINTDIFFUSE_SCALE_MODE_MULTI_SAMPLE_LINEAR;}
			constexpr operator D2D1_POINTSPECULAR_SCALE_MODE()const noexcept{return D2D1_POINTSPECULAR_SCALE_MODE_MULTI_SAMPLE_LINEAR;}
			constexpr operator D2D1_SPOTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_SPOTDIFFUSE_SCALE_MODE_MULTI_SAMPLE_LINEAR;}
			constexpr operator D2D1_SPOTSPECULAR_SCALE_MODE()const noexcept{return D2D1_SPOTSPECULAR_SCALE_MODE_MULTI_SAMPLE_LINEAR;}
		}static constexpr multi_sample_linear = {};
		struct{
			constexpr operator D2D1_CONVOLVEMATRIX_SCALE_MODE()const noexcept{return D2D1_CONVOLVEMATRIX_SCALE_MODE_ANISOTROPIC;}
			constexpr operator D2D1_DISTANTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_DISTANTDIFFUSE_SCALE_MODE_ANISOTROPIC;}
			constexpr operator D2D1_DISTANTSPECULAR_SCALE_MODE()const noexcept{return D2D1_DISTANTSPECULAR_SCALE_MODE_ANISOTROPIC;}
			constexpr operator D2D1_POINTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_POINTDIFFUSE_SCALE_MODE_ANISOTROPIC;}
			constexpr operator D2D1_POINTSPECULAR_SCALE_MODE()const noexcept{return D2D1_POINTSPECULAR_SCALE_MODE_ANISOTROPIC;}
			constexpr operator D2D1_SPOTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_SPOTDIFFUSE_SCALE_MODE_ANISOTROPIC;}
			constexpr operator D2D1_SPOTSPECULAR_SCALE_MODE()const noexcept{return D2D1_SPOTSPECULAR_SCALE_MODE_ANISOTROPIC;}
		}static constexpr anisotropic = {};
		struct{
			constexpr operator D2D1_CONVOLVEMATRIX_SCALE_MODE()const noexcept{return D2D1_CONVOLVEMATRIX_SCALE_MODE_HIGH_QUALITY_CUBIC;}
			constexpr operator D2D1_DISTANTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_DISTANTDIFFUSE_SCALE_MODE_HIGH_QUALITY_CUBIC;}
			constexpr operator D2D1_DISTANTSPECULAR_SCALE_MODE()const noexcept{return D2D1_DISTANTSPECULAR_SCALE_MODE_HIGH_QUALITY_CUBIC;}
			constexpr operator D2D1_POINTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_POINTDIFFUSE_SCALE_MODE_HIGH_QUALITY_CUBIC;}
			constexpr operator D2D1_POINTSPECULAR_SCALE_MODE()const noexcept{return D2D1_POINTSPECULAR_SCALE_MODE_HIGH_QUALITY_CUBIC;}
			constexpr operator D2D1_SPOTDIFFUSE_SCALE_MODE()const noexcept{return D2D1_SPOTDIFFUSE_SCALE_MODE_HIGH_QUALITY_CUBIC;}
			constexpr operator D2D1_SPOTSPECULAR_SCALE_MODE()const noexcept{return D2D1_SPOTSPECULAR_SCALE_MODE_HIGH_QUALITY_CUBIC;}
		}static constexpr high_quality_cubic = {};
	};
	struct optimization{
		optimization() = delete;
		struct{
			constexpr operator D2D1_DIRECTIONALBLUR_OPTIMIZATION()const noexcept{return D2D1_DIRECTIONALBLUR_OPTIMIZATION_SPEED;}
			constexpr operator D2D1_GAUSSIANBLUR_OPTIMIZATION()const noexcept{return D2D1_GAUSSIANBLUR_OPTIMIZATION_SPEED;}
			constexpr operator D2D1_SHADOW_OPTIMIZATION()const noexcept{return D2D1_SHADOW_OPTIMIZATION_SPEED;}
		}static constexpr speed = {};
		struct{
			constexpr operator D2D1_DIRECTIONALBLUR_OPTIMIZATION()const noexcept{return D2D1_DIRECTIONALBLUR_OPTIMIZATION_BALANCED;}
			constexpr operator D2D1_GAUSSIANBLUR_OPTIMIZATION()const noexcept{return D2D1_GAUSSIANBLUR_OPTIMIZATION_BALANCED;}
			constexpr operator D2D1_SHADOW_OPTIMIZATION()const noexcept{return D2D1_SHADOW_OPTIMIZATION_BALANCED;}
		}static constexpr balanced = {};
		struct{
			constexpr operator D2D1_DIRECTIONALBLUR_OPTIMIZATION()const noexcept{return D2D1_DIRECTIONALBLUR_OPTIMIZATION_QUALITY;}
			constexpr operator D2D1_GAUSSIANBLUR_OPTIMIZATION()const noexcept{return D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY;}
			constexpr operator D2D1_SHADOW_OPTIMIZATION()const noexcept{return D2D1_SHADOW_OPTIMIZATION_QUALITY;}
		}static constexpr quality = {};
	};
	explicit d2d(D2D1_FACTORY_TYPE t = D2D1_FACTORY_TYPE_MULTI_THREADED):d2d{+create_factory(t)}{}
	struct stroke_style : d2d_resource<ID2D1StrokeStyle1>{
		using d2d_resource::d2d_resource;
		struct property : detail::property<D2D1_STROKE_STYLE_PROPERTIES1>{
			using detail::property<D2D1_STROKE_STYLE_PROPERTIES1>::property;
	#define PROPERTYDECL(name, type, membername) property& name(type t){prop.membername = t;return *this;}
			PROPERTYDECL(start_cap, D2D1_CAP_STYLE, startCap)
			PROPERTYDECL(end_cap, D2D1_CAP_STYLE, endCap)
			PROPERTYDECL(dash_cap, D2D1_CAP_STYLE, dashCap)
			property& cap(D2D1_CAP_STYLE s){prop.startCap = prop.endCap = prop.dashCap = s; return *this;}
			PROPERTYDECL(line_join, D2D1_LINE_JOIN, lineJoin)
			PROPERTYDECL(miter_limit, FLOAT, miterLimit)
			PROPERTYDECL(dash_style, D2D1_DASH_STYLE, dashStyle)
			PROPERTYDECL(dash_offset, FLOAT, dashOffset)
			PROPERTYDECL(transform_type, D2D1_STROKE_TRANSFORM_TYPE, transformType)
	#undef  PROPERTYDECL
		};
		D2D1_CAP_STYLE dash_cap()const{return (*this)->GetDashCap();}
		UINT32 dashes_count()const{return (*this)->GetDashesCount();}
		std::vector<FLOAT> dashes()const{const auto n = dashes_count();std::vector<FLOAT> data(n);(*this)->GetDashes(data.data(), n);return std::move(data);}
		FLOAT dash_offset()const{return (*this)->GetDashOffset();}
		D2D1_DASH_STYLE dash_style()const{return (*this)->GetDashStyle();}
		D2D1_CAP_STYLE end_cap()const{return (*this)->GetEndCap();}
		D2D1_LINE_JOIN line_join()const{return (*this)->GetLineJoin();}
		FLOAT miter_limit()const{return (*this)->GetMiterLimit();}
		D2D1_CAP_STYLE start_cap()const{return (*this)->GetStartCap();}
		D2D1_STROKE_TRANSFORM_TYPE transform_type()const{return (*this)->GetStrokeTransformType();}
	};
	struct drawing_state_block : d2d_resource<ID2D1DrawingStateBlock>{
		using d2d_resource::d2d_resource;
		struct description : detail::property<D2D1_DRAWING_STATE_DESCRIPTION>{
			using property::property;
	#define PROPERTYDECL(name, type, membername) description& name(type t){prop.membername = t;return *this;}
			PROPERTYDECL(antialias_mode, D2D1_ANTIALIAS_MODE, antialiasMode)
			PROPERTYDECL(text_antialias_mode, D2D1_TEXT_ANTIALIAS_MODE, textAntialiasMode)
			PROPERTYDECL(tag1, D2D1_TAG, tag1)
			PROPERTYDECL(tag2, D2D1_TAG, tag2)
			description& tags(const std::pair<D2D1_TAG, D2D1_TAG>& t){prop.tag1 = t.first;prop.tag2 = t.second;return *this;}
			PROPERTYDECL(transform, D2D1_MATRIX_3X2_F, transform)
	#undef  PROPERTYDECL
		};
		D2D1_DRAWING_STATE_DESCRIPTION get_description(){D2D1_DRAWING_STATE_DESCRIPTION d;(*this)->GetDescription(&d);return d;}
		void set_description(const D2D1_DRAWING_STATE_DESCRIPTION& d){(*this)->SetDescription(d);}
		__declspec(property(get=get_description, put=set_description)) D2D1_DRAWING_STATE_DESCRIPTION drawing_state_description;
		dwrite::rendering_params get_text_rendering_params()const{IDWriteRenderingParams1* ptr;(*this)->GetTextRenderingParams(reinterpret_cast<IDWriteRenderingParams**>(&ptr));return dwrite::rendering_params{std::move(ptr)};}
		void set_text_rendering_params(IDWriteRenderingParams1* params = nullptr){(*this)->SetTextRenderingParams(params);}
		template<typename DWriteRenderingParams>
		void set_text_rendering_params(DWriteRenderingParams&& params){set_text_rendering_params(std::forward<DWriteRenderingParams>(params).get());}
		__declspec(property(get=get_text_rendering_params, put=set_text_rendering_parms)) dwrite::rendering_params text_rendering_params;
	};
	class device : public d2d_resource<ID2D1Device>{
		template<typename I>
		struct brush : d2d_resource<I>{
			using d2d_resource::d2d_resource;
			FLOAT get_opacity()const{return (*this)->GetOpacity();}
			void set_opcaity(FLOAT opac){(*this)->SetOpacity(opac);}
			__declspec(property(get=get_opacity, put=set_opcaity)) FLOAT opacity;
			D2D1_MATRIX_3X2_F get_transform()const{D2D1_MATRIX_3X2_F mat;(*this)->GetTransform(&mat);return *this;}
			void set_transform(const D2D1_MATRIX_3X2_F& mat){(*this)->SetTransform(mat);}
			__declspec(property(get=get_transform, put=set_transform)) D2D1_MATRIX_3X2_F transform;
		};
		template<typename I>
		struct render_target;
	public:
		class context;
		struct solid_color_brush : brush<ID2D1SolidColorBrush>{
			using brush::brush;
			D2D1_COLOR_F get_color()const{return (*this)->GetColor();}
			solid_color_brush& set_color(const D2D1_COLOR_F& c){(*this)->SetColor(c);return *this;}
			solid_color_brush& set_color(const D2D1::ColorF::Enum& c){(*this)->SetColor(D2D1::ColorF(c));return *this;}
			solid_color_brush& set_color(UINT32 c){(*this)->SetColor(D2D1::ColorF(c));return *this;}
			__declspec(property(get=get_color, put=set_color)) D2D1_COLOR_F color;
		};
		struct gradient_stop_collection : detail::resource<ID2D1GradientStopCollection1>{
			using resource::resource;
			class property{
				std::vector<D2D1_GRADIENT_STOP> grads;
				D2D1_COLOR_SPACE preerps = D2D1_COLOR_SPACE_SRGB;
				D2D1_COLOR_SPACE posterps = D2D1_COLOR_SPACE_SCRGB;
				D2D1_BUFFER_PRECISION bufpre = D2D1_BUFFER_PRECISION_8BPC_UNORM_SRGB;
				D2D1_EXTEND_MODE extm = D2D1_EXTEND_MODE_CLAMP;
				D2D1_COLOR_INTERPOLATION_MODE colerpm = D2D1_COLOR_INTERPOLATION_MODE_STRAIGHT;
			public:
				explicit property() = default;
		#define PROPERTYDECL(name, type, membername) property& name(type t){membername = t;return *this;}
				PROPERTYDECL(preinterpolation_space, D2D1_COLOR_SPACE, preerps)
				PROPERTYDECL(postinterpolation_space, D2D1_COLOR_SPACE, posterps)
				PROPERTYDECL(buffer_precision, D2D1_BUFFER_PRECISION, bufpre)
				PROPERTYDECL(extend_mode, D2D1_EXTEND_MODE, extm)
				PROPERTYDECL(color_interpolation_mode, D2D1_COLOR_INTERPOLATION_MODE, colerpm)
				PROPERTYDECL(gradient_stops, const std::vector<D2D1_GRADIENT_STOP>&, grads)
				property& gradient_stops(std::vector<D2D1_GRADIENT_STOP>&& t){grads = std::move(t);return *this;}
				property& gradient_stops(std::initializer_list<D2D1_GRADIENT_STOP> il){grads = il;return *this;}
		#undef  PROPERTYDECL
				friend context;
			};
			D2D1_GAMMA color_interpolation_gamma()const{return (*this)->GetColorInterpolationGamma();}
			D2D1_EXTEND_MODE extend_mode()const{return (*this)->GetExtendMode();}
			UINT32 gradient_stop_count()const{return (*this)->GetGradientStopCount();}
			std::vector<D2D1_GRADIENT_STOP> gradient_stops()const{const auto n = gradient_stop_count();std::vector<D2D1_GRADIENT_STOP> data(n);(*this)->GetGradientStops1(data.data(), n);return std::move(data);}
			D2D1_BUFFER_PRECISION buffer_precision()const{return (*this)->GetBufferPrecision();}
			D2D1_COLOR_INTERPOLATION_MODE color_interpolation_mode()const{return (*this)->GetColorInterpolationMode();}
			D2D1_COLOR_SPACE post_interpolation_mode()const{return (*this)->GetPostInterpolationSpace();}
			D2D1_COLOR_SPACE pre_interpolation_mode()const{return (*this)->GetPreInterpolationSpace();}
		};
		struct linear_gradient_brush : brush<ID2D1LinearGradientBrush>{
			using brush::brush;
			D2D1_POINT_2F get_start_point()const{return (*this)->GetStartPoint();}
			void set_start_point(const D2D1_POINT_2F& sp){(*this)->SetStartPoint(sp);}
			template<typename T>
			void set_start_point(T&& t){(*this)->SetStartPoint(two_dim::attribute<D2D1_POINT_2F>(std::forward<T>(t)));}
			__declspec(property(get=get_start_point, put=set_start_point)) D2D1_POINT_2F start_point;
			D2D1_POINT_2F get_end_point()const{return (*this)->GetEndPoint();}
			void set_end_point(const D2D1_POINT_2F& ep){(*this)->SetEndPoint(ep);}
			template<typename T>
			void set_end_point(T&& t){(*this)->SetEndPoint(two_dim::attribute<D2D1_POINT_2F>(std::forward<T>(t)));}
			__declspec(property(get=get_end_point, put=set_end_point)) D2D1_POINT_2F end_point;
			gradient_stop_collection get_gradient_stop_collection()const{ID2D1GradientStopCollection1* ptr;(*this)->GetGradientStopCollection(reinterpret_cast<ID2D1GradientStopCollection**>(&ptr));return gradient_stop_collection{std::move(ptr)};}
		};
		class radial_gradient_brush : public brush<ID2D1RadialGradientBrush>{
			class radius_impl{
				ID2D1RadialGradientBrush* self;
				radius_impl(ID2D1RadialGradientBrush* p):self{p}{}
				friend radial_gradient_brush;
			public:
				operator will::two_dim::xy<FLOAT>()const{return {get_x(), get_y()};}
				operator D2D1_POINT_2F()const{return {get_x(), get_y()};}
				template<typename T>
				T attribute()const{return will::two_dim::attribute<T>(static_cast<will::two_dim::xy<FLOAT>>(*this));}
				FLOAT get_x()const{return self->GetRadiusX();}
				void set_x(FLOAT t){self->SetRadiusX(t);}
				__declspec(property(get=get_x, put=set_x)) FLOAT x;
				FLOAT get_y()const{return self->GetRadiusY();}
				void set_y(FLOAT t){self->SetRadiusY(t);}
				__declspec(property(get=get_y, put=set_y)) FLOAT y;
			};
		public:
			using brush::brush;
			D2D1_POINT_2F get_center()const{return (*this)->GetCenter();}
			void set_center(const D2D1_POINT_2F& c){(*this)->SetCenter(c);}
			template<typename T>
			void set_center(T&& t){(*this)->SetCenter(two_dim::attribute<D2D1_POINT_2F>(std::forward<T>(t)));}
			__declspec(property(get=get_center, put=set_center)) D2D1_POINT_2F center;
			radius_impl get_radius()const{return radius_impl{get()};}
			void set_radius(const two_dim::xy<FLOAT>& r){(*this)->SetRadiusX(r.x);(*this)->SetRadiusY(r.y);}
			__declspec(property(get=get_radius, put=set_radius)) radius_impl radius;
			D2D1_POINT_2F get_gradient_origin_offset()const{return (*this)->GetGradientOriginOffset();}
			void set_gradient_origin_offset(const D2D1_POINT_2F& goo){(*this)->SetGradientOriginOffset(goo);}
			template<typename T>
			void set_gradient_origin_offset(T&& t){(*this)->SetGradientOriginOffset(two_dim::attribute<D2D1_POINT_2F>(std::forward<T>(t)));}
			__declspec(property(get=get_gradient_origin_offset, put=set_gradient_origin_offset)) D2D1_POINT_2F gradient_origin_offset;
			gradient_stop_collection get_gradient_stop_collection()const{ID2D1GradientStopCollection1* ptr;(*this)->GetGradientStopCollection(reinterpret_cast<ID2D1GradientStopCollection**>(&ptr));return gradient_stop_collection{std::move(ptr)};}
		};
		class bitmap : public d2d_resource<ID2D1Bitmap1>{
			class copy_impl{
			protected:
				const context& devcont;
				const bitmap& bm;
			public:
				copy_impl(const context& devcont, const bitmap& bm):devcont(devcont), bm(bm){}
				friend bitmap;
			};
		public:
			using d2d_resource::d2d_resource;
			class property : public detail::property<D2D1_BITMAP_PROPERTIES1>{
				class dpi_property{
					property* prop;
				public:
					dpi_property(property* p):prop(p){}
					dpi_property(const dpi_property&) = delete;
					dpi_property(dpi_property&&) = delete;
					dpi_property& operator=(const dpi_property&) = delete;
					dpi_property& operator=(dpi_property&&) = delete;
					property& operator()(FLOAT x, FLOAT y)const{prop->prop.dpiX = x; prop->prop.dpiY = y; return *prop;}
					property& operator()(const two_dim::xy<FLOAT>& xy)const{return (*this)(xy.x, xy.y);}
					property& x(FLOAT x)const{prop->prop.dpiX = x; return *prop;}
					property& y(FLOAT y)const{prop->prop.dpiY = y; return *prop;}
				};
				friend class dpi_property;
			public:
				explicit property(){prop.pixelFormat = {DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED};}
		#define PROPERTYDECL(name, type, membername) property& name(type t){prop.membername = t;return *this;}
				PROPERTYDECL(format, DXGI_FORMAT, pixelFormat.format)
				PROPERTYDECL(alpha_mode, D2D1_ALPHA_MODE, pixelFormat.alphaMode)
				const dpi_property dpi = this;
				PROPERTYDECL(option, D2D1_BITMAP_OPTIONS, bitmapOptions)
				PROPERTYDECL(color_context, ID2D1ColorContext*, colorContext)
		#undef  PROPERTYDECL
			};
			struct memory_data{
				const void* data;
				UINT32 pitch;
			};
			struct region{
				const bitmap& bm;
				D2D1_RECT_U rect;
			};
			class lazy_manipulator{
				D2D1::Matrix4x4F mat;
				const bitmap& bm;
			public:
				friend bitmap;
				friend class renderer;
				lazy_manipulator(const bitmap& b):bm{b}{}
				lazy_manipulator& perspective_transform(const D2D1::Matrix4x4F& m = {}){mat = mat * m;return *this;}
				lazy_manipulator& scale_x(float x = 1.f){return perspective_transform(D2D1::Matrix4x4F::Scale(x, 1.f, 1.f));}
				lazy_manipulator& scale_y(float y = 1.f){return perspective_transform(D2D1::Matrix4x4F::Scale(1.f, y, 1.f));}
				lazy_manipulator& scale_z(float z = 1.f){return perspective_transform(D2D1::Matrix4x4F::Scale(1.f, 1.f, z));}
				lazy_manipulator& scale(float t = 1.f){return perspective_transform(D2D1::Matrix4x4F::Scale(t, t, t));}
				lazy_manipulator& scale(float x, float y, float z = 1.f){return perspective_transform(D2D1::Matrix4x4F::Scale(x, y, z));}
				lazy_manipulator& skew_x(float x = 1.f){return perspective_transform(D2D1::Matrix4x4F::SkewX(x));}
				lazy_manipulator& skew_y(float y = 1.f){return perspective_transform(D2D1::Matrix4x4F::SkewY(y));}
				lazy_manipulator& translate_x(float x = 0.f){return perspective_transform(D2D1::Matrix4x4F::Translation(x, 0.f, 0.f));}
				lazy_manipulator& translate_y(float y = 0.f){return perspective_transform(D2D1::Matrix4x4F::Translation(0.f, y, 0.f));}
				lazy_manipulator& translate_z(float z = 0.f){return perspective_transform(D2D1::Matrix4x4F::Translation(0.f, 0.f, z));}
				lazy_manipulator& translate(float x = 0.f, float y = 0.f, float z = 0.f){return perspective_transform(D2D1::Matrix4x4F::Translation(x, y, z));}
				lazy_manipulator& rotate_x(float x = 0.f){return perspective_transform(D2D1::Matrix4x4F::RotationX(x));}
				lazy_manipulator& rotate_y(float y = 0.f){return perspective_transform(D2D1::Matrix4x4F::RotationY(y));}
				lazy_manipulator& rotate_z(float z = 0.f){return perspective_transform(D2D1::Matrix4x4F::RotationZ(z));}
				lazy_manipulator& perspective_project(float d){return perspective_transform(D2D1::Matrix4x4F::PerspectiveProjection(d));}
				lazy_manipulator& rotate_arbitary_axis(float x, float y, float z, float degrees){return perspective_transform(D2D1::Matrix4x4F::RotationArbitraryAxis(x, y, z, degrees));}
				expected<bitmap, hresult_error> freeze(const context& c)const{return c.prerender([&](auto&& r){r.bitmap(bm, 1.f, D2D1_INTERPOLATION_MODE_LINEAR, mat);});}
			};
			bitmap(bitmap&&) = default;
			bitmap(const copy_impl& c):bitmap{[](const copy_impl& c){const auto dpi = c.bm.get_dpi();return std::move(+c.devcont.create_bitmap(c.bm.get_pixel_size(), D2D1::BitmapProperties1(c.bm.get_bitmap_options(), c.bm.get_pixel_format(), dpi.x, dpi.y, c.bm.get_color_context())) = c);}(c)}{}
			copy_impl copy(const context& c)const{return {c, *this};}
			lazy_manipulator perspective_transform(const D2D1::Matrix4x4F& m = {}){return lazy_manipulator{*this}.perspective_transform(m);}
			lazy_manipulator scale_x(float x = 1.f)const{return lazy_manipulator{*this}.scale_x(x);}
			lazy_manipulator scale_y(float y = 1.f)const{return lazy_manipulator{*this}.scale_y(y);}
			lazy_manipulator scale_z(float z = 1.f)const{return lazy_manipulator{*this}.scale_z(z);}
			lazy_manipulator scale(float x = 1.f, float y = 1.f, float z = 1.f)const{return lazy_manipulator{*this}.scale(x, y, z);}
			lazy_manipulator skew_x(float x = 1.f)const{return lazy_manipulator{*this}.skew_x(x);}
			lazy_manipulator skew_y(float y = 1.f)const{return lazy_manipulator{*this}.skew_y(y);}
			lazy_manipulator translate_x(float x = 0.f)const{return lazy_manipulator{*this}.translate_x(x);}
			lazy_manipulator translate_y(float y = 0.f)const{return lazy_manipulator{*this}.translate_y(y);}
			lazy_manipulator translate_z(float z = 0.f)const{return lazy_manipulator{*this}.translate_z(z);}
			lazy_manipulator translate(float x = 0.f, float y = 0.f, float z = 0.f)const{return lazy_manipulator{*this}.translate(x, y, z);}
			lazy_manipulator rotate_x(float x = 0.f)const{return lazy_manipulator{*this}.rotate_x(x);}
			lazy_manipulator rotate_y(float y = 0.f)const{return lazy_manipulator{*this}.rotate_y(y);}
			lazy_manipulator rotate_z(float z = 0.f)const{return lazy_manipulator{*this}.rotate_z(z);}
			lazy_manipulator perspective_project(float d){return lazy_manipulator{*this}.perspective_project(d);}
			lazy_manipulator rotate_arbitary_axis(float x, float y, float z, float degrees){return lazy_manipulator{*this}.rotate_arbitary_axis(x, y, z, degrees);}
			region get_region(const D2D1_RECT_U& r)const{return region{*this, r};}
			will::two_dim::xy<FLOAT> get_dpi()const{will::two_dim::xy<FLOAT> dpi;(*this)->GetDpi(&dpi.x, &dpi.y);return dpi;}
			D2D1_SIZE_U get_pixel_size()const{return (*this)->GetPixelSize();}
			D2D1_SIZE_F get_dip_size()const{return (*this)->GetSize();}
			D2D1_PIXEL_FORMAT get_pixel_format()const{return (*this)->GetPixelFormat();}
			D2D1_BITMAP_OPTIONS get_bitmap_options()const{return (*this)->GetOptions();}
			ID2D1ColorContext* get_color_context()const{ID2D1ColorContext* ptr;(*this)->GetColorContext(&ptr);return ptr;}
			bitmap& operator=(bitmap&&) = default;
			template<typename T, std::enable_if_t<!std::is_base_of<ID2D1RenderTarget, std::remove_pointer_t<std::decay_t<T>>>::value>* = nullptr>
			bitmap& operator=(const T& t){return *this = t.get();}
			bitmap& operator=(ID2D1Bitmap1* bm){(*this)->CopyFromBitmap(nullptr, bm, nullptr);return *this;}
			bitmap& operator=(ID2D1RenderTarget* rt){(*this)->CopyFromRenderTarget(nullptr, rt, nullptr);return *this;}
			bitmap& operator=(const copy_impl& c){return *this = c.bm.get();}
			bitmap& operator=(const region& r){(*this)->CopyFromBitmap(nullptr, r.bm.get(), &r.rect);return *this;}
			template<typename I>
			bitmap& operator=(const typename render_target<I>::region& r){(*this)->CopyFromRenderTarget(nullptr, r.rt, &r.rect);return *this;}
			bitmap& operator=(const memory_data& data){(*this)->CopyFromMemory(nullptr, data.data, data.pitch);return *this;}
		};
		class bitmap_brush : public brush<ID2D1BitmapBrush1>{
			class extend_mode_impl{
				ID2D1BitmapBrush1* self;
				extend_mode_impl(ID2D1BitmapBrush1* p):self{p}{}
				friend bitmap_brush;
			public:
				operator will::two_dim::xy<D2D1_EXTEND_MODE>()const{return {get_x(), get_y()};}
				D2D1_EXTEND_MODE get_x()const{return self->GetExtendModeX();}
				void set_x(D2D1_EXTEND_MODE t){self->SetExtendModeX(t);}
				__declspec(property(get=get_x, put=set_x)) D2D1_EXTEND_MODE x;
				D2D1_EXTEND_MODE get_y()const{return self->GetExtendModeY();}
				void set_y(D2D1_EXTEND_MODE t){self->SetExtendModeY(t);}
				__declspec(property(get=get_y, put=set_y)) D2D1_EXTEND_MODE y;
			};
		public:
			using brush::brush;
			bitmap get_bitmap()const{using bmp = class bitmap;ID2D1Bitmap1* ptr;(*this)->GetBitmap(reinterpret_cast<ID2D1Bitmap**>(&ptr));return bmp{std::move(ptr)};}
			void set_bitmap(ID2D1Bitmap1* bmp){(*this)->SetBitmap(bmp);}
			template<typename Bitmap>
			void set_bitmap(Bitmap&& b){set_bitmap(std::forward<Bitmap>(b).get());}
			__declspec(property(get=get_bitmap, put=set_bitmap)) bitmap bitmap;
			extend_mode_impl get_extend_mode()const{return extend_mode_impl{get()};}
			void set_extend_mode(const two_dim::xy<D2D1_EXTEND_MODE>& r){(*this)->SetExtendModeX(r.x);(*this)->SetExtendModeY(r.y);}
			__declspec(property(get=get_extend_mode, put=set_extend_mode)) extend_mode_impl radius;
			D2D1_INTERPOLATION_MODE get_interpolation_mode()const{return (*this)->GetInterpolationMode1();}
			void set_interpolation_mode(D2D1_INTERPOLATION_MODE im){(*this)->SetInterpolationMode1(im);}
			__declspec(property(get=get_interpolation_mode, put=set_interpolation_mode)) D2D1_INTERPOLATION_MODE interpolation_mode;
		};
		class effect : public d2d_resource<ID2D1Effect>{
			class io_impl{
				UINT32 index;
				effect& eff;
				ID2D1Image* get()const{ID2D1Image* ptr;eff->GetInput(index, &ptr);return ptr;}
			public:
				io_impl(UINT32 i, effect& eff):index(i), eff(eff){}
				io_impl& operator|=(ID2D1Image* input){eff->SetInput(index, input); return *this;}
				io_impl& operator|=(ID2D1Effect* input){eff->SetInputEffect(index, input); return *this;}
				template<typename Input>
				io_impl& operator|=(Input&& input){return *this |= std::forward<Input>(input).get();}
			};
			template<typename Prop>
			class sv_l_impl{
				Prop p;
				effect& eff;
			public:
				sv_l_impl(Prop p, effect& eff):p(p), eff(eff){}
				template<typename Value>
				effect& operator=(Value&& v){return +eff.set(p, std::forward<Value>(v));}
			};
			template<typename Prop>
			class sv_r_impl{
				Prop p;
				effect& eff;
			public:
				sv_r_impl(Prop p, effect& eff):p(p), eff(eff){}
				template<typename Value>
				effect&& operator=(Value&& v){return std::move(+eff.set(p, std::forward<Value>(v)));}
			};
		public:
			using d2d_resource::d2d_resource;
			effect& operator|=(ID2D1Bitmap1* input){(*this)->SetInput(0, input); return *this;}
			effect& operator|=(ID2D1Effect* input){(*this)->SetInputEffect(0, input); return *this;}
			template<typename Input>
			effect& operator|=(Input&& input){return *this |= std::forward<Input>(input).get();}
			io_impl operator[](UINT32 i){return io_impl{i, *this};}
			template<typename Prop>
			sv_l_impl<Prop> operator()(Prop p)& {return sv_l_impl<Prop>(p, *this);}
			template<typename Prop>
			sv_r_impl<Prop> operator()(Prop p)&&{return sv_r_impl<Prop>(p, *this);}
			template<typename Prop, typename Value>
			expected<effect&, hresult_error> set(Prop p, Value&& v)& {const auto hr = (*this)->SetValue(p, std::forward<Value>(v));if(SUCCEEDED(hr))return           *this ;return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
			template<typename Prop, typename Value>
			expected<effect , hresult_error> set(Prop p, Value&& v)&&{const auto hr = (*this)->SetValue(p, std::forward<Value>(v));if(SUCCEEDED(hr))return std::move(*this);return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		};
		class builtin_effects{
		template<typename T, const CLSID& ClassId>
		class impl : effect{protected:using self = T;using effect::set;using effect::operator|=;using effect::operator[];public:using effect::effect;using effect::get_factory;using effect::get;using effect::operator->;using effect::operator bool;using effect::operator=;using effect::swap;static const CLSID& clsid(){return ClassId;}};
		public:
		builtin_effects() = delete;
#define PROPERTYDECL(name, prop, type, default_value, precond, set_expression) \
			expected<self&, hresult_error> set_##name(type v = default_value)& {precond;const auto result = this->set(prop, set_expression);if(result)return           *this ;return make_unexpected(std::move(result.error()));}\
			expected<self , hresult_error> set_##name(type v = default_value)&&{precond;const auto result = this->set(prop, set_expression);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}\
			__declspec(property(put=set_##name)) std::decay_t<type> name;
		struct color{
		color() = delete;
			//(Win10)CLSID_D2D1LookupTable3D
			struct color_management : impl<color_management, CLSID_D2D1ColorManagement>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(source_context,      D2D1_COLORMANAGEMENT_PROP_SOURCE_COLOR_CONTEXT,         ID2D1ColorContext*,                    nullptr,                                          , v)
				template<typename ColorContext>
				expected<self&, hresult_error> set_source_context(ColorContext&& v)& {return set_source_context(std::forward<ColorContext>(v).get());}
				template<typename ColorContext>
				expected<self , hresult_error> set_source_context(ColorContext&& v)&&{return set_source_context(std::forward<ColorContext>(v).get());}
				PROPERTYDECL(source_intent,       D2D1_COLORMANAGEMENT_PROP_SOURCE_RENDERING_INTENT,      D2D1_COLORMANAGEMENT_RENDERING_INTENT, D2D1_COLORMANAGEMENT_RENDERING_INTENT_PERCEPTUAL, , v)
				PROPERTYDECL(destination_context, D2D1_COLORMANAGEMENT_PROP_DESTINATION_COLOR_CONTEXT,    ID2D1ColorContext*,                    nullptr,                                          , v)
				template<typename ColorContext>
				expected<self&, hresult_error> set_destination_context(ColorContext&& v)& {return set_destination_context(std::forward<ColorContext>(v).get());}
				template<typename ColorContext>
				expected<self , hresult_error> set_destination_context(ColorContext&& v)&&{return set_destination_context(std::forward<ColorContext>(v).get());}
				PROPERTYDECL(destination_intent,  D2D1_COLORMANAGEMENT_PROP_DESTINATION_RENDERING_INTENT, D2D1_COLORMANAGEMENT_RENDERING_INTENT, D2D1_COLORMANAGEMENT_RENDERING_INTENT_PERCEPTUAL, , v)
				PROPERTYDECL(alpha_mode,          D2D1_COLORMANAGEMENT_PROP_ALPHA_MODE,                   D2D1_COLORMANAGEMENT_ALPHA_MODE,       D2D1_COLORMANAGEMENT_ALPHA_MODE_PREMULTIPLIED,    , v)
				PROPERTYDECL(quality,             D2D1_COLORMANAGEMENT_PROP_QUALITY,                      D2D1_COLORMANAGEMENT_QUALITY,          D2D1_COLORMANAGEMENT_QUALITY_NORMAL,              , v)
			};
		private:
			struct _color_matrix_effect : impl<_color_matrix_effect, CLSID_D2D1ColorMatrix>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(color_matrix, D2D1_COLORMATRIX_PROP_COLOR_MATRIX, const D2D1_MATRIX_5X4_F&,    D2D1::Matrix5x4F(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0), , v               )
				PROPERTYDECL(alpha_mode,   D2D1_COLORMATRIX_PROP_ALPHA_MODE,   D2D1_COLORMATRIX_ALPHA_MODE, D2D1_COLORMATRIX_ALPHA_MODE_PREMULTIPLIED,                 , v               )
				PROPERTYDECL(clamp_output, D2D1_COLORMATRIX_PROP_CLAMP_OUTPUT, bool,                        false,                                                     , v ? TRUE : FALSE)
			};
		public:
			using color_matrix = _color_matrix_effect;
			struct discrete_transfer : impl<discrete_transfer, CLSID_D2D1DiscreteTransfer>{
				using impl::impl;
				using impl::operator|=;
				expected<self&, hresult_error> set_red  (const std::vector<float>& v = {0.f, 1.f})& {if(v.empty()){const auto result = this->set(D2D1_DISCRETETRANSFER_PROP_RED_DISABLE,   TRUE);if(result)return           *this ;return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_DISCRETETRANSFER_PROP_RED_TABLE,   reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return           *this ;return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				expected<self , hresult_error> set_red  (const std::vector<float>& v = {0.f, 1.f})&&{if(v.empty()){const auto result = this->set(D2D1_DISCRETETRANSFER_PROP_RED_DISABLE,   TRUE);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_DISCRETETRANSFER_PROP_RED_TABLE,   reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return std::move(*this);return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				__declspec(property(put=set_red))   std::vector<float> red;
				expected<self&, hresult_error> set_green(const std::vector<float>& v = {0.f, 1.f})& {if(v.empty()){const auto result = this->set(D2D1_DISCRETETRANSFER_PROP_GREEN_DISABLE, TRUE);if(result)return           *this ;return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_DISCRETETRANSFER_PROP_GREEN_TABLE, reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return           *this ;return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				expected<self , hresult_error> set_green(const std::vector<float>& v = {0.f, 1.f})&&{if(v.empty()){const auto result = this->set(D2D1_DISCRETETRANSFER_PROP_GREEN_DISABLE, TRUE);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_DISCRETETRANSFER_PROP_GREEN_TABLE, reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return std::move(*this);return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				__declspec(property(put=set_green)) std::vector<float> green;
				expected<self&, hresult_error> set_blue (const std::vector<float>& v = {0.f, 1.f})& {if(v.empty()){const auto result = this->set(D2D1_DISCRETETRANSFER_PROP_BLUE_DISABLE,  TRUE);if(result)return           *this ;return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_DISCRETETRANSFER_PROP_BLUE_TABLE,  reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return           *this ;return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				expected<self , hresult_error> set_blue (const std::vector<float>& v = {0.f, 1.f})&&{if(v.empty()){const auto result = this->set(D2D1_DISCRETETRANSFER_PROP_BLUE_DISABLE,  TRUE);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_DISCRETETRANSFER_PROP_BLUE_TABLE,  reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return std::move(*this);return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				__declspec(property(put=set_blue))  std::vector<float> blue;
				PROPERTYDECL(clamp_output, D2D1_DISCRETETRANSFER_PROP_CLAMP_OUTPUT, bool, false, , v ? TRUE : FALSE)
			};
			struct dpi_compensation : impl<dpi_compensation, CLSID_D2D1DpiCompensation>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(interpolation_mode, D2D1_DPICOMPENSATION_PROP_INTERPOLATION_MODE, D2D1_DPICOMPENSATION_INTERPOLATION_MODE, D2D1_DPICOMPENSATION_INTERPOLATION_MODE_LINEAR, , v)
				PROPERTYDECL(border_mode,        D2D1_DPICOMPENSATION_PROP_BORDER_MODE,        D2D1_BORDER_MODE,                        D2D1_BORDER_MODE_SOFT,                          , v)
				PROPERTYDECL(input_dpi,          D2D1_DPICOMPENSATION_PROP_INPUT_DPI,          float,                                   96.f,                                           , v)
			};
			struct gamma_transfer : impl<gamma_transfer, CLSID_D2D1GammaTransfer>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(  red_amplitude, D2D1_GAMMATRANSFER_PROP_RED_AMPLITUDE,   float, 1.f,   , v               )
				PROPERTYDECL(  red_exponent,  D2D1_GAMMATRANSFER_PROP_RED_EXPONENT,    float, 1.f,   , v               )
				PROPERTYDECL(  red_offset,    D2D1_GAMMATRANSFER_PROP_RED_OFFSET,      float, 0.f,   , v               )
				PROPERTYDECL(  red_enable,    D2D1_GAMMATRANSFER_PROP_RED_DISABLE,     bool,  true,  , v ? FALSE : TRUE)
				PROPERTYDECL(green_amplitude, D2D1_GAMMATRANSFER_PROP_GREEN_AMPLITUDE, float, 1.f,   , v               )
				PROPERTYDECL(green_exponent,  D2D1_GAMMATRANSFER_PROP_GREEN_EXPONENT,  float, 1.f,   , v               )
				PROPERTYDECL(green_offset,    D2D1_GAMMATRANSFER_PROP_GREEN_OFFSET,    float, 0.f,   , v               )
				PROPERTYDECL(green_enable,    D2D1_GAMMATRANSFER_PROP_GREEN_DISABLE,   bool,  true,  , v ? FALSE : TRUE)
				PROPERTYDECL( blue_amplitude, D2D1_GAMMATRANSFER_PROP_BLUE_AMPLITUDE,  float, 1.f,   , v               )
				PROPERTYDECL( blue_exponent,  D2D1_GAMMATRANSFER_PROP_BLUE_EXPONENT,   float, 1.f,   , v               )
				PROPERTYDECL( blue_offset,    D2D1_GAMMATRANSFER_PROP_BLUE_OFFSET,     float, 0.f,   , v               )
				PROPERTYDECL( blue_enable,    D2D1_GAMMATRANSFER_PROP_BLUE_DISABLE,    bool,  true,  , v ? FALSE : TRUE)
				PROPERTYDECL(alpha_amplitude, D2D1_GAMMATRANSFER_PROP_ALPHA_AMPLITUDE, float, 1.f,   , v               )
				PROPERTYDECL(alpha_exponent,  D2D1_GAMMATRANSFER_PROP_ALPHA_EXPONENT,  float, 1.f,   , v               )
				PROPERTYDECL(alpha_offset,    D2D1_GAMMATRANSFER_PROP_ALPHA_OFFSET,    float, 0.f,   , v               )
				PROPERTYDECL(alpha_enable,    D2D1_GAMMATRANSFER_PROP_ALPHA_DISABLE,   bool,  true,  , v ? FALSE : TRUE)
				PROPERTYDECL(clamp_output,    D2D1_GAMMATRANSFER_PROP_CLAMP_OUTPUT,    bool,  false, , v ? TRUE : FALSE)
			};
			//(Win10)CLSID_D2D1HueToRgb
			struct hue_rotation : impl<hue_rotation, CLSID_D2D1HueRotation>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(angle, D2D1_HUEROTATION_PROP_ANGLE, float, 0.f, , v)
			};
			struct linear_transfer : impl<linear_transfer, CLSID_D2D1LinearTransfer>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(  red_y_intercept, D2D1_LINEARTRANSFER_PROP_RED_Y_INTERCEPT,   float, 0.f,   , v               )
				PROPERTYDECL(  red_slope,       D2D1_LINEARTRANSFER_PROP_RED_SLOPE,         float, 1.f,   , v               )
				PROPERTYDECL(  red_enable,      D2D1_LINEARTRANSFER_PROP_RED_DISABLE,       bool,  true,  , v ? FALSE : TRUE)
				PROPERTYDECL(green_y_intercept, D2D1_LINEARTRANSFER_PROP_GREEN_Y_INTERCEPT, float, 0.f,   , v               )
				PROPERTYDECL(green_slope,       D2D1_LINEARTRANSFER_PROP_GREEN_SLOPE,       float, 1.f,   , v               )
				PROPERTYDECL(green_enable,      D2D1_LINEARTRANSFER_PROP_GREEN_DISABLE,     bool,  true,  , v ? FALSE : TRUE)
				PROPERTYDECL( blue_y_intercept, D2D1_LINEARTRANSFER_PROP_BLUE_Y_INTERCEPT,  float, 0.f,   , v               )
				PROPERTYDECL( blue_slope,       D2D1_LINEARTRANSFER_PROP_BLUE_SLOPE,        float, 1.f,   , v               )
				PROPERTYDECL( blue_enable,      D2D1_LINEARTRANSFER_PROP_BLUE_DISABLE,      bool,  true,  , v ? FALSE : TRUE)
				PROPERTYDECL(alpha_y_intercept, D2D1_LINEARTRANSFER_PROP_ALPHA_Y_INTERCEPT, float, 0.f,   , v               )
				PROPERTYDECL(alpha_slope,       D2D1_LINEARTRANSFER_PROP_ALPHA_SLOPE,       float, 0.f,   , v               )
				PROPERTYDECL(alpha_enable,      D2D1_LINEARTRANSFER_PROP_ALPHA_DISABLE,     bool,  true,  , v ? FALSE : TRUE)
				PROPERTYDECL(clamp_output,      D2D1_LINEARTRANSFER_PROP_CLAMP_OUTPUT,      bool,  false, , v ? TRUE : FALSE)
			};
			struct opacity_metadata : impl<opacity_metadata, CLSID_D2D1OpacityMetadata>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(output_rect, D2D1_OPACITYMETADATA_PROP_INPUT_OPAQUE_RECT, const D2D1_VECTOR_4F&, D2D1::Vector4F(-FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX), , v)
			};
			struct premultiply : impl<premultiply, CLSID_D2D1Premultiply>{
				using impl::impl;
				using impl::operator|=;
			};
			//(Win10)CLSID_D2D1RgbToHue
		private:
			struct _saturation_effect : impl<_saturation_effect, CLSID_D2D1Saturation>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(saturation, D2D1_SATURATION_PROP_SATURATION, float, .5f, if(v < 0.f || 1.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
			};
		public:
			using saturation = _saturation_effect;
			struct table_transfer : impl<table_transfer, CLSID_D2D1TableTransfer>{
				using impl::impl;
				using impl::operator|=;
				expected<self&, hresult_error> set_red  (const std::vector<float>& v = {0.f, 1.f})& {if(v.empty()){const auto result = this->set(D2D1_TABLETRANSFER_PROP_RED_DISABLE,   TRUE);if(result)return           *this ;return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_TABLETRANSFER_PROP_RED_TABLE,   reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return           *this ;return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				expected<self , hresult_error> set_red  (const std::vector<float>& v = {0.f, 1.f})&&{if(v.empty()){const auto result = this->set(D2D1_TABLETRANSFER_PROP_RED_DISABLE,   TRUE);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_TABLETRANSFER_PROP_RED_TABLE,   reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return std::move(*this);return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				__declspec(property(put=set_red))   std::vector<float> red;
				expected<self&, hresult_error> set_green(const std::vector<float>& v = {0.f, 1.f})& {if(v.empty()){const auto result = this->set(D2D1_TABLETRANSFER_PROP_GREEN_DISABLE, TRUE);if(result)return           *this ;return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_TABLETRANSFER_PROP_GREEN_TABLE, reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return           *this ;return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				expected<self , hresult_error> set_green(const std::vector<float>& v = {0.f, 1.f})&&{if(v.empty()){const auto result = this->set(D2D1_TABLETRANSFER_PROP_GREEN_DISABLE, TRUE);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_TABLETRANSFER_PROP_GREEN_TABLE, reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return std::move(*this);return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				__declspec(property(put=set_green)) std::vector<float> green;
				expected<self&, hresult_error> set_blue (const std::vector<float>& v = {0.f, 1.f})& {if(v.empty()){const auto result = this->set(D2D1_TABLETRANSFER_PROP_BLUE_DISABLE,  TRUE);if(result)return           *this ;return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_TABLETRANSFER_PROP_BLUE_TABLE,  reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return           *this ;return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				expected<self , hresult_error> set_blue (const std::vector<float>& v = {0.f, 1.f})&&{if(v.empty()){const auto result = this->set(D2D1_TABLETRANSFER_PROP_BLUE_DISABLE,  TRUE);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_TABLETRANSFER_PROP_BLUE_TABLE,  reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return std::move(*this);return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				__declspec(property(put=set_blue))  std::vector<float> blue;
				expected<self&, hresult_error> set_alpha(const std::vector<float>& v = {0.f, 1.f})& {if(v.empty()){const auto result = this->set(D2D1_TABLETRANSFER_PROP_ALPHA_DISABLE, TRUE);if(result)return           *this ;return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_TABLETRANSFER_PROP_ALPHA_TABLE, reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return           *this ;return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				expected<self , hresult_error> set_alpha(const std::vector<float>& v = {0.f, 1.f})&&{if(v.empty()){const auto result = this->set(D2D1_TABLETRANSFER_PROP_ALPHA_DISABLE, TRUE);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}for(auto&& x : v)if(x < 0.f || 1.f < x)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);const auto result = (*this)->SetValue(D2D1_TABLETRANSFER_PROP_ALPHA_TABLE, reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return std::move(*this);return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				__declspec(property(put=set_alpha)) std::vector<float> alpha;
				PROPERTYDECL(clamp_output, D2D1_TABLETRANSFER_PROP_CLAMP_OUTPUT, bool, false, , v ? TRUE : FALSE)
			};
			//(Win10)CLSID_D2D1Tint
			struct unpremultiply : impl<unpremultiply, CLSID_D2D1UnPremultiply>{
				using impl::impl;
				using impl::operator|=;
			};
			//(Win10)CLSID_D2D1YCbCr
		};
		using color_management = color::color_management;
		using color_matrix = color::color_matrix;
		using discrete_transfer = color::discrete_transfer;
		using dpi_compensation = color::dpi_compensation;
		using gamma_transfer = color::gamma_transfer;
		using hue_rotation = color::hue_rotation;
		using linear_transfer = color::linear_transfer;
		using opacity_metadata = color::opacity_metadata;
		using premultiply = color::premultiply;
		using saturation = color::saturation;
		using table_transfer = color::table_transfer;
		using unpremultiply = color::unpremultiply;
		struct composition{
		composition() = delete;
			//(Win10)CLSID_D2D1AlphaMask
			struct arithmetic_composite : impl<arithmetic_composite, CLSID_D2D1ArithmeticComposite>{
				using impl::impl;
				auto operator[](_In_range_(0, 1) UINT32 i){if(i > 1U)throw std::invalid_argument(__FUNCTION__);return impl::operator[](i);}
				PROPERTYDECL(coefficients, D2D1_ARITHMETICCOMPOSITE_PROP_COEFFICIENTS, const D2D1_VECTOR_4F&, D2D1::Vector4F(1.f, 0.f, 0.f, 0.f), , v               )
				PROPERTYDECL(clamp_output, D2D1_ARITHMETICCOMPOSITE_PROP_CLAMP_OUTPUT, bool,                  false,                              , v ? TRUE : FALSE)
			};
			struct blend : impl<blend, CLSID_D2D1Blend>{
				using impl::impl;
				auto operator[](_In_range_(0, 1) UINT32 i){if(i > 1U)throw std::invalid_argument(__FUNCTION__);return impl::operator[](i);}
				PROPERTYDECL(mode, D2D1_BLEND_PROP_MODE, D2D1_BLEND_MODE, D2D1_BLEND_MODE_MULTIPLY, , v)
			};
			struct composite : impl<composite, CLSID_D2D1Composite>{
				using impl::impl;
				using impl::operator[];
				PROPERTYDECL(mode, D2D1_COMPOSITE_PROP_MODE, D2D1_COMPOSITE_MODE, D2D1_COMPOSITE_MODE_SOURCE_OVER, , v)
			};
			//(Win10)CLSID_D2D1CrossFade
		};
		using arithmetic_composite = composition::arithmetic_composite;
		using blend = composition::blend;
		using composite = composition::composite;
		struct filter{
		filter() = delete;
			class convolve_matrix : public impl<convolve_matrix, CLSID_D2D1ConvolveMatrix>{
				class _kernel_size{
					convolve_matrix& cm;
				public:
					explicit _kernel_size(convolve_matrix& cm_):cm{cm_}{}
					expected<convolve_matrix&, hresult_error> set_x(UINT32 v = 3u)& {const auto result = cm.set(D2D1_CONVOLVEMATRIX_PROP_KERNEL_SIZE_X, v);if(result)return           cm ;return make_unexpected(std::move(result.error()));}
					expected<convolve_matrix , hresult_error> set_x(UINT32 v = 3u)&&{const auto result = cm.set(D2D1_CONVOLVEMATRIX_PROP_KERNEL_SIZE_X, v);if(result)return std::move(cm);return make_unexpected(std::move(result.error()));}
					__declspec(property(put=set_x)) UINT32 x;
					expected<convolve_matrix&, hresult_error> set_y(UINT32 v = 3u)& {const auto result = cm.set(D2D1_CONVOLVEMATRIX_PROP_KERNEL_SIZE_Y, v);if(result)return           cm ;return make_unexpected(std::move(result.error()));}
					expected<convolve_matrix , hresult_error> set_y(UINT32 v = 3u)&&{const auto result = cm.set(D2D1_CONVOLVEMATRIX_PROP_KERNEL_SIZE_Y, v);if(result)return std::move(cm);return make_unexpected(std::move(result.error()));}
					__declspec(property(put=set_y)) UINT32 y;
				};
				friend _kernel_size;
			public:
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(kernel_unit_length, D2D1_CONVOLVEMATRIX_PROP_KERNEL_UNIT_LENGTH, float,                          1.f,                                   , v               )
				PROPERTYDECL(scale_mode,         D2D1_CONVOLVEMATRIX_PROP_SCALE_MODE,         D2D1_CONVOLVEMATRIX_SCALE_MODE, D2D1_CONVOLVEMATRIX_SCALE_MODE_LINEAR, , v               )
				expected<self&, hresult_error> set_kernel_size_x(UINT32 v = 3u)& {const auto result = this->set(D2D1_CONVOLVEMATRIX_PROP_KERNEL_SIZE_X, v);if(result)return           *this ;return make_unexpected(std::move(result.error()));}
				expected<self , hresult_error> set_kernel_size_x(UINT32 v = 3u)&&{const auto result = this->set(D2D1_CONVOLVEMATRIX_PROP_KERNEL_SIZE_X, v);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}
				expected<self&, hresult_error> set_kernel_size_y(UINT32 v = 3u)& {const auto result = this->set(D2D1_CONVOLVEMATRIX_PROP_KERNEL_SIZE_Y, v);if(result)return           *this ;return make_unexpected(std::move(result.error()));}
				expected<self , hresult_error> set_kernel_size_y(UINT32 v = 3u)&&{const auto result = this->set(D2D1_CONVOLVEMATRIX_PROP_KERNEL_SIZE_Y, v);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}
				_kernel_size get_kernel_size()noexcept{return _kernel_size{*this};}
				expected<self&, hresult_error> set_kernel_size(const two_dim::wh<UINT32>& v = {3, 3})& {return                  set_kernel_size_x(v.w).bind([&v](self&  s){return           s .set_kernel_size_y(v.h);});}
				expected<self , hresult_error> set_kernel_size(const two_dim::wh<UINT32>& v = {3, 3})&&{return std::move(*this).set_kernel_size_x(v.w).bind([&v](self&& s){return std::move(s).set_kernel_size_y(v.h);});}
				__declspec(property(get=get_kernel_size, put=set_kernel_size)) _kernel_size kernel_size;
				expected<self&, hresult_error> set_kernel_matrix(const std::vector<float>& v = {0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f})& {const auto result = (*this)->SetValue(D2D1_CONVOLVEMATRIX_PROP_KERNEL_MATRIX, reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return           *this ;return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				expected<self , hresult_error> set_kernel_matrix(const std::vector<float>& v = {0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f})&&{const auto result = (*this)->SetValue(D2D1_CONVOLVEMATRIX_PROP_KERNEL_MATRIX, reinterpret_cast<const BYTE*>(v.data()), static_cast<UINT32>(v.size() * sizeof(float)));if(SUCCEEDED(result))return std::move(*this);return make_unexpected<hresult_error>(_T(__FUNCTION__), result);}
				__declspec(property(put=set_kernel_matrix)) std::vector<float> kernel_matrix;
				PROPERTYDECL(divisor,            D2D1_CONVOLVEMATRIX_PROP_DIVISOR,            float,                          1.f,                                   , v               )
				PROPERTYDECL(bias,               D2D1_CONVOLVEMATRIX_PROP_BIAS,               float,                          0.f,                                   , v               )
				PROPERTYDECL(kernel_offset,      D2D1_CONVOLVEMATRIX_PROP_KERNEL_OFFSET,      const D2D1_VECTOR_2F&,          D2D1::Vector2F(0.f, 0.f),              , v               )
				PROPERTYDECL(preserve_alpha,     D2D1_CONVOLVEMATRIX_PROP_PRESERVE_ALPHA,     bool,                           false,                                 , v ? TRUE : FALSE)
				PROPERTYDECL(border_mode,        D2D1_CONVOLVEMATRIX_PROP_BORDER_MODE,        D2D1_BORDER_MODE,               D2D1_BORDER_MODE_SOFT,                 , v               )
				PROPERTYDECL(clamp_output,       D2D1_CONVOLVEMATRIX_PROP_CLAMP_OUTPUT,       bool,                           false,                                 , v ? TRUE : FALSE)
			};
			struct directional_blur : impl<directional_blur, CLSID_D2D1DirectionalBlur>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(standard_deviation, D2D1_DIRECTIONALBLUR_PROP_STANDARD_DEVIATION, float,                             3.f,                                        , v)
				PROPERTYDECL(angle,              D2D1_DIRECTIONALBLUR_PROP_ANGLE,              float,                             0.f,                                        , v)
				PROPERTYDECL(optimization,       D2D1_DIRECTIONALBLUR_PROP_OPTIMIZATION,       D2D1_DIRECTIONALBLUR_OPTIMIZATION, D2D1_DIRECTIONALBLUR_OPTIMIZATION_BALANCED, , v)
				PROPERTYDECL(border_mode,        D2D1_DIRECTIONALBLUR_PROP_BORDER_MODE,        D2D1_BORDER_MODE,                  D2D1_BORDER_MODE_SOFT,                      , v)
			};
			//(Win10)CLSID_D2D1EdgeDetection
			struct gaussian_blur : impl<gaussian_blur, CLSID_D2D1GaussianBlur>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(standard_deviation, D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, FLOAT,                          3.f,                                     , v)
				PROPERTYDECL(optimization,       D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION,       D2D1_GAUSSIANBLUR_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_BALANCED, , v)
				PROPERTYDECL(border_mode,        D2D1_GAUSSIANBLUR_PROP_BORDER_MODE,        D2D1_BORDER_MODE,               D2D1_BORDER_MODE_SOFT,                   , v)
			};
			struct morphology : impl<morphology, CLSID_D2D1Morphology>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(mode,   D2D1_MORPHOLOGY_PROP_MODE,   D2D1_MORPHOLOGY_MODE, D2D1_MORPHOLOGY_MODE_ERODE, ,                                                                                          v)
				PROPERTYDECL(width,  D2D1_MORPHOLOGY_PROP_WIDTH,  UINT,                 1,                          if(v < 1 || 100 < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(height, D2D1_MORPHOLOGY_PROP_HEIGHT, UINT,                 1,                          if(v < 1 || 100 < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				expected<self&, hresult_error> set_kernel_size(const two_dim::wh<UINT>& v = {1, 1})& {return                  set_width(v.w).bind([&v](self&  s){return           s .set_height(v.h);});}
				expected<self , hresult_error> set_kernel_size(const two_dim::wh<UINT>& v = {1, 1})&&{return std::move(*this).set_width(v.w).bind([&v](self&& s){return std::move(s).set_height(v.h);});}
				__declspec(property(put=set_kernel_size)) two_dim::wh<UINT> kernel_size;
			};
		};
		using convolve_matrix = filter::convolve_matrix;
		using directional_blur = filter::directional_blur;
		using gaussian_blur = filter::gaussian_blur;
		using morphology = filter::morphology;
		struct lighting_and_stylizing{
		lighting_and_stylizing() = delete;
			struct displacement_map : impl<displacement_map, CLSID_D2D1DisplacementMap>{
				using impl::impl;
				auto operator[](_In_range_(0, 1) UINT32 i){if(i > 1U)throw std::invalid_argument(__FUNCTION__);return impl::operator[](i);}
				PROPERTYDECL(scale,             D2D1_DISPLACEMENTMAP_PROP_SCALE,            float,                 0.f,                     , v)
				PROPERTYDECL(x_channnel_select, D2D1_DISPLACEMENTMAP_PROP_X_CHANNEL_SELECT, D2D1_CHANNEL_SELECTOR, D2D1_CHANNEL_SELECTOR_A, , v)
				PROPERTYDECL(y_channnel_select, D2D1_DISPLACEMENTMAP_PROP_Y_CHANNEL_SELECT, D2D1_CHANNEL_SELECTOR, D2D1_CHANNEL_SELECTOR_A, , v)
			};
			struct distant_diffuse : impl<distant_diffuse, CLSID_D2D1DistantDiffuse>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(azimuth,            D2D1_DISTANTDIFFUSE_PROP_AZIMUTH,            float,                          0.f,                                   if(v < 0.f || 360.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),   v)
				PROPERTYDECL(elevation,          D2D1_DISTANTDIFFUSE_PROP_ELEVATION,          float,                          0.f,                                   if(v < 0.f || 360.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),   v)
				PROPERTYDECL(diffuse_constant,   D2D1_DISTANTDIFFUSE_PROP_DIFFUSE_CONSTANT,   float,                          1.f,                                   if(v < 0.f || 10000.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(surface_scale,      D2D1_DISTANTDIFFUSE_PROP_SURFACE_SCALE,      float,                          1.f,                                   if(v < 0.f || 10000.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(color,              D2D1_DISTANTDIFFUSE_PROP_COLOR,              const D2D1_VECTOR_3F&,          D2D1::Vector3F(1.f, 1.f, 1.f),         ,                                                                                                v)
				expected<self&, hresult_error> set_color(const D2D1_COLOR_F& v = {1.f, 1.f, 1.f, 1.f})& {return                  set_color(D2D1::Vector3F(v.r * v.a, v.g * v.a, v.b * v.a));}
				expected<self , hresult_error> set_color(const D2D1_COLOR_F& v = {1.f, 1.f, 1.f, 1.f})&&{return std::move(*this).set_color(D2D1::Vector3F(v.r * v.a, v.g * v.a, v.b * v.a));}
				expected<self&, hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::White)& {return                  set_color(D2D1::ColorF(v));}
				expected<self , hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::White)&&{return std::move(*this).set_color(D2D1::ColorF(v));}
				PROPERTYDECL(kernel_unit_length, D2D1_DISTANTDIFFUSE_PROP_KERNEL_UNIT_LENGTH, const D2D1_VECTOR_2F&,          D2D1::Vector2F(1.f, 1.f),              ,                                                                                                v)
				PROPERTYDECL(scale_mode,         D2D1_DISTANTDIFFUSE_PROP_SCALE_MODE,         D2D1_DISTANTDIFFUSE_SCALE_MODE, D2D1_DISTANTDIFFUSE_SCALE_MODE_LINEAR, ,                                                                                                v)
			};
			struct distant_specular : impl<distant_specular, CLSID_D2D1DistantSpecular>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(azimuth,            D2D1_DISTANTSPECULAR_PROP_AZIMUTH,            float,                           0.f,                                    if(v < 0.f || 360.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),   v)
				PROPERTYDECL(elevation,          D2D1_DISTANTSPECULAR_PROP_ELEVATION,          float,                           0.f,                                    if(v < 0.f || 360.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),   v)
				PROPERTYDECL(specular_exponent,  D2D1_DISTANTSPECULAR_PROP_SPECULAR_EXPONENT,  float,                           1.f,                                    if(v < 1.f || 128.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),   v)
				PROPERTYDECL(specular_constant,  D2D1_DISTANTSPECULAR_PROP_SPECULAR_CONSTANT,  float,                           1.f,                                    if(v < 0.f || 10000.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(surface_scale,      D2D1_DISTANTSPECULAR_PROP_SURFACE_SCALE,      float,                           1.f,                                    if(v < 0.f || 10000.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(color,              D2D1_DISTANTSPECULAR_PROP_COLOR,              const D2D1_VECTOR_3F&,           D2D1::Vector3F(1.f, 1.f, 1.f),          ,                                                                                                v)
				expected<self&, hresult_error> set_color(const D2D1_COLOR_F& v = {1.f, 1.f, 1.f, 1.f})& {return                  set_color(D2D1::Vector3F(v.r * v.a, v.g * v.a, v.b * v.a));}
				expected<self , hresult_error> set_color(const D2D1_COLOR_F& v = {1.f, 1.f, 1.f, 1.f})&&{return std::move(*this).set_color(D2D1::Vector3F(v.r * v.a, v.g * v.a, v.b * v.a));}
				expected<self&, hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::White)& {return                  set_color(D2D1::ColorF(v));}
				expected<self , hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::White)&&{return std::move(*this).set_color(D2D1::ColorF(v));}
				PROPERTYDECL(kernel_unit_length, D2D1_DISTANTSPECULAR_PROP_KERNEL_UNIT_LENGTH, const D2D1_VECTOR_2F&,           D2D1::Vector2F(1.f, 1.f),               ,                                                                                                v)
				PROPERTYDECL(scale_mode,         D2D1_DISTANTSPECULAR_PROP_SCALE_MODE,         D2D1_DISTANTSPECULAR_SCALE_MODE, D2D1_DISTANTSPECULAR_SCALE_MODE_LINEAR, ,                                                                                                v)
			};
			//(Win10)CLSID_D2D1Emboss
			struct point_diffuse : impl<point_diffuse, CLSID_D2D1PointDiffuse>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(light_position,     D2D1_POINTDIFFUSE_PROP_LIGHT_POSITION,     const D2D1_VECTOR_3F&,        D2D1::Vector3F(0.f, 0.f, 0.f),       ,                                                                                                v)
				PROPERTYDECL(diffuse_constant,   D2D1_POINTDIFFUSE_PROP_DIFFUSE_CONSTANT,   float,                        1.f,                                 if(v < 0.f || 10000.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(surface_scale,      D2D1_POINTDIFFUSE_PROP_SURFACE_SCALE,      float,                        1.f,                                 if(v < 0.f || 10000.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(color,              D2D1_POINTDIFFUSE_PROP_COLOR,              const D2D1_VECTOR_3F&,        D2D1::Vector3F(1.f, 1.f, 1.f),       ,                                                                                                v)
				expected<self&, hresult_error> set_color(const D2D1_COLOR_F& v = {1.f, 1.f, 1.f, 1.f})& {return                  set_color(D2D1::Vector3F(v.r * v.a, v.g * v.a, v.b * v.a));}
				expected<self , hresult_error> set_color(const D2D1_COLOR_F& v = {1.f, 1.f, 1.f, 1.f})&&{return std::move(*this).set_color(D2D1::Vector3F(v.r * v.a, v.g * v.a, v.b * v.a));}
				expected<self&, hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::White)& {return                  set_color(D2D1::ColorF(v));}
				expected<self , hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::White)&&{return std::move(*this).set_color(D2D1::ColorF(v));}
				PROPERTYDECL(kernel_unit_length, D2D1_POINTDIFFUSE_PROP_KERNEL_UNIT_LENGTH, const D2D1_VECTOR_2F&,        D2D1::Vector2F(1.f, 1.f),            ,                                                                                                v)
				PROPERTYDECL(scale_mode,         D2D1_POINTDIFFUSE_PROP_SCALE_MODE,         D2D1_POINTDIFFUSE_SCALE_MODE, D2D1_POINTDIFFUSE_SCALE_MODE_LINEAR, ,                                                                                                v)
			};
			struct point_specular : impl<point_specular, CLSID_D2D1PointSpecular>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(light_position,     D2D1_POINTSPECULAR_PROP_LIGHT_POSITION,     const D2D1_VECTOR_3F&,         D2D1::Vector3F(0.f, 0.f, 0.f),        ,                                                                                                v)
				PROPERTYDECL(specular_exponent,  D2D1_POINTSPECULAR_PROP_SPECULAR_EXPONENT,  float,                         1.f,                                  if(v < 1.f || 128.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),   v)
				PROPERTYDECL(specular_constant,  D2D1_POINTSPECULAR_PROP_SPECULAR_CONSTANT,  float,                         1.f,                                  if(v < 0.f || 10000.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(surface_scale,      D2D1_POINTSPECULAR_PROP_SURFACE_SCALE,      float,                         1.f,                                  if(v < 0.f || 10000.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(color,              D2D1_POINTSPECULAR_PROP_COLOR,              const D2D1_VECTOR_3F&,         D2D1::Vector3F(1.f, 1.f, 1.f),        ,                                                                                                v)
				expected<self&, hresult_error> set_color(const D2D1_COLOR_F& v = {1.f, 1.f, 1.f, 1.f})& {return                  set_color(D2D1::Vector3F(v.r * v.a, v.g * v.a, v.b * v.a));}
				expected<self , hresult_error> set_color(const D2D1_COLOR_F& v = {1.f, 1.f, 1.f, 1.f})&&{return std::move(*this).set_color(D2D1::Vector3F(v.r * v.a, v.g * v.a, v.b * v.a));}
				expected<self&, hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::White)& {return                  set_color(D2D1::ColorF(v));}
				expected<self , hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::White)&&{return std::move(*this).set_color(D2D1::ColorF(v));}
				PROPERTYDECL(kernel_unit_length, D2D1_POINTSPECULAR_PROP_KERNEL_UNIT_LENGTH, const D2D1_VECTOR_2F&,         D2D1::Vector2F(1.f, 1.f),             ,                                                                                                v)
				PROPERTYDECL(scale_mode,         D2D1_POINTSPECULAR_PROP_SCALE_MODE,         D2D1_POINTSPECULAR_SCALE_MODE, D2D1_POINTSPECULAR_SCALE_MODE_LINEAR, ,                                                                                                v)
			};
			//(Win10)CLSID_D2D1Posterize
			struct shadow : impl<shadow, CLSID_D2D1Shadow>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(blur_standard_deviation, D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, float,                    3.f,                                , v)
				PROPERTYDECL(color,                   D2D1_SHADOW_PROP_COLOR                  , const D2D1_VECTOR_4F&,    D2D1::Vector4F(0.f, 0.f, 0.f, 1.f), , v)
				expected<self&, hresult_error> set_color(const D2D1_COLOR_F& v = {0.f, 0.f, 0.f, 1.f})& {return                  set_color(D2D1::Vector4F(v.r, v.g, v.b, v.a));}
				expected<self , hresult_error> set_color(const D2D1_COLOR_F& v = {0.f, 0.f, 0.f, 1.f})&&{return std::move(*this).set_color(D2D1::Vector4F(v.r, v.g, v.b, v.a));}
				expected<self&, hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::Black)& {return                  set_color(D2D1::ColorF(v));}
				expected<self , hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::Black)&&{return std::move(*this).set_color(D2D1::ColorF(v));}
				PROPERTYDECL(optimization,            D2D1_SHADOW_PROP_OPTIMIZATION           , D2D1_SHADOW_OPTIMIZATION, D2D1_SHADOW_OPTIMIZATION_BALANCED,  , v)
			};
			struct spot_diffuse : impl<spot_diffuse, CLSID_D2D1SpotDiffuse>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(light_position,      D2D1_SPOTDIFFUSE_PROP_LIGHT_POSITION,      const D2D1_VECTOR_3F&,       D2D1::Vector3F(0.f, 0.f, 0.f),      ,                                                                                                v)
				PROPERTYDECL(points_at,           D2D1_SPOTDIFFUSE_PROP_POINTS_AT,           const D2D1_VECTOR_3F&,       D2D1::Vector3F(0.f, 0.f, 0.f),      ,                                                                                                v)
				PROPERTYDECL(focus,               D2D1_SPOTDIFFUSE_PROP_FOCUS,               float,                       1.f,                                if(v < 0.f || 200.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),   v)
				PROPERTYDECL(limiting_cone_angle, D2D1_SPOTDIFFUSE_PROP_LIMITING_CONE_ANGLE, float,                       90.f,                               if(v < 0.f || 90.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),    v)
				PROPERTYDECL(diffuse_constant,    D2D1_SPOTDIFFUSE_PROP_DIFFUSE_CONSTANT,    float,                       1.f,                                if(v < 0.f || 10000.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(surface_scale,       D2D1_SPOTDIFFUSE_PROP_SURFACE_SCALE,       float,                       1.f,                                if(v < 0.f || 10000.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(color,               D2D1_SPOTDIFFUSE_PROP_COLOR,               const D2D1_VECTOR_3F&,       D2D1::Vector3F(1.f, 1.f, 1.f),      ,                                                                                                v)
				expected<self&, hresult_error> set_color(const D2D1_COLOR_F& v = {1.f, 1.f, 1.f, 1.f})& {return                  set_color(D2D1::Vector3F(v.r * v.a, v.g * v.a, v.b * v.a));}
				expected<self , hresult_error> set_color(const D2D1_COLOR_F& v = {1.f, 1.f, 1.f, 1.f})&&{return std::move(*this).set_color(D2D1::Vector3F(v.r * v.a, v.g * v.a, v.b * v.a));}
				expected<self&, hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::White)& {return                  set_color(D2D1::ColorF(v));}
				expected<self , hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::White)&&{return std::move(*this).set_color(D2D1::ColorF(v));}
				PROPERTYDECL(kernel_unit_length,  D2D1_SPOTDIFFUSE_PROP_KERNEL_UNIT_LENGTH,  const D2D1_VECTOR_2F&,       D2D1::Vector2F(1.f, 1.f),           ,                                                                                                v)
				PROPERTYDECL(scale_mode,          D2D1_SPOTDIFFUSE_PROP_SCALE_MODE,          D2D1_SPOTDIFFUSE_SCALE_MODE, D2D1_SPOTDIFFUSE_SCALE_MODE_LINEAR, ,                                                                                                v)
			};
			struct spot_specular : impl<spot_specular, CLSID_D2D1SpotSpecular>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(light_position,      D2D1_SPOTSPECULAR_PROP_LIGHT_POSITION,      const D2D1_VECTOR_3F&,        D2D1::Vector3F(0.f, 0.f, 0.f),       ,                                                                                                v)
				PROPERTYDECL(points_at,           D2D1_SPOTSPECULAR_PROP_POINTS_AT,           const D2D1_VECTOR_3F&,        D2D1::Vector3F(0.f, 0.f, 0.f),       ,                                                                                                v)
				PROPERTYDECL(focus,               D2D1_SPOTSPECULAR_PROP_FOCUS,               float,                        1.f,                                 if(v < 0.f || 200.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),   v)
				PROPERTYDECL(limiting_cone_angle, D2D1_SPOTSPECULAR_PROP_LIMITING_CONE_ANGLE, float,                        90.f,                                if(v < 0.f || 90.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),    v)
				PROPERTYDECL(specular_exponent,   D2D1_SPOTSPECULAR_PROP_SPECULAR_EXPONENT,   float,                        1.f,                                 if(v < 1.f || 128.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),   v)
				PROPERTYDECL(specular_constant,   D2D1_SPOTSPECULAR_PROP_SPECULAR_CONSTANT,   float,                        1.f,                                 if(v < 0.f || 10000.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(surface_scale,       D2D1_SPOTSPECULAR_PROP_SURFACE_SCALE,       float,                        1.f,                                 if(v < 0.f || 10000.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(color,               D2D1_SPOTSPECULAR_PROP_COLOR,               const D2D1_VECTOR_3F&,        D2D1::Vector3F(1.f, 1.f, 1.f),       ,                                                                                                v)
				expected<self&, hresult_error> set_color(const D2D1_COLOR_F& v = {1.f, 1.f, 1.f, 1.f})& {return                  set_color(D2D1::Vector3F(v.r * v.a, v.g * v.a, v.b * v.a));}
				expected<self , hresult_error> set_color(const D2D1_COLOR_F& v = {1.f, 1.f, 1.f, 1.f})&&{return std::move(*this).set_color(D2D1::Vector3F(v.r * v.a, v.g * v.a, v.b * v.a));}
				expected<self&, hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::White)& {return                  set_color(D2D1::ColorF(v));}
				expected<self , hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::White)&&{return std::move(*this).set_color(D2D1::ColorF(v));}
				PROPERTYDECL(kernel_unit_length,  D2D1_SPOTSPECULAR_PROP_KERNEL_UNIT_LENGTH,  const D2D1_VECTOR_2F&,        D2D1::Vector2F(1.f, 1.f),            ,                                                                                                v)
				PROPERTYDECL(scale_mode,          D2D1_SPOTSPECULAR_PROP_SCALE_MODE,          D2D1_SPOTSPECULAR_SCALE_MODE, D2D1_SPOTSPECULAR_SCALE_MODE_LINEAR, ,                                                                                                v)
			};
		};
		using displacement_map = lighting_and_stylizing::displacement_map;
		using distant_diffuse = lighting_and_stylizing::distant_diffuse;
		using distant_specular = lighting_and_stylizing::distant_specular;
		using point_diffuse = lighting_and_stylizing::point_diffuse;
		using point_specular = lighting_and_stylizing::point_specular;
		using shadow = lighting_and_stylizing::shadow;
		using spot_diffuse = lighting_and_stylizing::spot_diffuse;
		using spot_specular = lighting_and_stylizing::spot_specular;
		struct photo{
		photo() = delete;
			struct brightness : impl<brightness, CLSID_D2D1Brightness>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(white_point, D2D1_BRIGHTNESS_PROP_WHITE_POINT, const D2D1_VECTOR_2F&, D2D1::Vector2F(1.f, 1.f), if(v.x < 0.f || 1.f < v.x || v.y < 0.f || 1.f < v.y)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(black_point, D2D1_BRIGHTNESS_PROP_BLACK_POINT, const D2D1_VECTOR_2F&, D2D1::Vector2F(0.f, 0.f), if(v.x < 0.f || 1.f < v.x || v.y < 0.f || 1.f < v.y)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
			};
			//(Win10)CLSID_D2D1Contrast
			//(Win10)CLSID_D2D1Exposure
			//(Win10)CLSID_D2D1Glayscale
			//(Win10)CLSID_D2D1HilightsShadows
			struct histogram : impl<histogram, CLSID_D2D1Histogram>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(num_bins,         D2D1_HISTOGRAM_PROP_NUM_BINS,         UINT32,                256,                     , v)
				PROPERTYDECL(channel_select,   D2D1_HISTOGRAM_PROP_CHANNEL_SELECT,   D2D1_CHANNEL_SELECTOR, D2D1_CHANNEL_SELECTOR_R, , v)
				expected<std::vector<float>, hresult_error> output()const{
					UINT32 c;
					{
						const auto hr = (*this)->GetValue(D2D1_HISTOGRAM_PROP_NUM_BINS, &c);
						if(FAILED(hr))
							return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
					}
					std::vector<float> vec;
					vec.resize(c);
					const auto hr = (*this)->GetValue(D2D1_HISTOGRAM_PROP_HISTOGRAM_OUTPUT, reinterpret_cast<BYTE*>(vec.data()), c * sizeof(float));
					if(FAILED(hr))
						return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
					return vec;
				}
				std::vector<float> operator()()const{return *output();}
			};
			//(Win10)CLSID_D2D1Invert
			//(Win10)CLSID_D2D1Sepia
			//(Win10)CLSID_D2D1Sharpen
			//(Win10)CLSID_D2D1Straighten
			//(Win10)CLSID_D2D1TemperatureTint
			//(Win10)CLSID_D2D1Vignette
		};
		using brightness = photo::brightness;
		using histogram = photo::histogram;
		struct source{
		source() = delete;
			struct bitmap_source : impl<bitmap_source, CLSID_D2D1BitmapSource>{
				using impl::impl;
				PROPERTYDECL(wic_bitmap_source,     D2D1_BITMAPSOURCE_PROP_WIC_BITMAP_SOURCE,     IWICBitmapSource*,                        nullptr,                                     ,                                                                                                v               )
				template<typename WICBitmapSource>
				expected<self&, hresult_error> set_wic_bitmap_source(WICBitmapSource&& v)& {return set_wic_bitmap_source(std::forward<WICBitmapSource>(v).get());}
				template<typename WICBitmapSource>
				expected<self , hresult_error> set_wic_bitmap_source(WICBitmapSource&& v)&&{return set_Wic_bitmap_source(std::forward<WICBitmapSource>(v).get());}
				PROPERTYDECL(scale,                 D2D1_BITMAPSOURCE_PROP_SCALE,                 const D2D1_VECTOR_2F&,                    D2D1::Vector2F(1.f, 1.f),                    if(v.x < 0.f || v.y < 0.f)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v               )
				PROPERTYDECL(interpolation_mode,    D2D1_BITMAPSOURCE_PROP_INTERPOLATION_MODE,    D2D1_BITMAPSOURCE_INTERPOLATION_MODE,     D2D1_BITMAPSOURCE_INTERPOLATION_MODE_LINEAR, ,                                                                                                v               )
				PROPERTYDECL(enable_dpi_correction, D2D1_BITMAPSOURCE_PROP_ENABLE_DPI_CORRECTION, bool,                                     false,                                       ,                                                                                                v ? TRUE : FALSE)
				PROPERTYDECL(alpha_mode,            D2D1_BITMAPSOURCE_PROP_ALPHA_MODE,            D2D1_BITMAPSOURCE_ALPHA_MODE,             D2D1_BITMAPSOURCE_ALPHA_MODE_PREMULTIPLIED,  ,                                                                                                v               )
				PROPERTYDECL(orientation,           D2D1_BITMAPSOURCE_PROP_ORIENTATION,           D2D1_BITMAPSOURCE_ORIENTATION,            D2D1_BITMAPSOURCE_ORIENTATION_DEFAULT,       ,                                                                                                v               )
			};
			struct flood : impl<flood, CLSID_D2D1Flood>{
				using impl::impl;
				PROPERTYDECL(color, D2D1_FLOOD_PROP_COLOR, const D2D1_VECTOR_4F&, D2D1::Vector4F(0.f, 0.f, 0.f, 1.f), , v)
				expected<self&, hresult_error> set_color(const D2D1_COLOR_F& v = {0.f, 0.f, 0.f, 1.f})& {return                  set_color(D2D1::Vector4F(v.r, v.g, v.b, v.a));}
				expected<self , hresult_error> set_color(const D2D1_COLOR_F& v = {0.f, 0.f, 0.f, 1.f})&&{return std::move(*this).set_color(D2D1::Vector4F(v.r, v.g, v.b, v.a));}
				expected<self&, hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::Black)& {return                  set_color(D2D1::ColorF(v));}
				expected<self , hresult_error> set_color(const D2D1::ColorF::Enum& v = D2D1::ColorF::Black)&&{return std::move(*this).set_color(D2D1::ColorF(v));}
			};
			struct turbulence : impl<turbulence, CLSID_D2D1Turbulence>{
				using impl::impl;
				PROPERTYDECL(offset,         D2D1_TURBULENCE_PROP_OFFSET,         const D2D1_VECTOR_2F&, D2D1::Vector2F(0.f, 0.f),          ,                                                                                                  v               )
				PROPERTYDECL(base_frequency, D2D1_TURBULENCE_PROP_BASE_FREQUENCY, const D2D1_VECTOR_2F&, D2D1::Vector2F(.01f, .01f),        if(v.x <= 0.f || v.y <= 0.f)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v               )
				PROPERTYDECL(num_octaves,    D2D1_TURBULENCE_PROP_NUM_OCTAVES,    UINT32,                1u,                                if(v == 0u)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),                  v               )
				PROPERTYDECL(seed,           D2D1_TURBULENCE_PROP_SEED,           UINT32,                0u,                                ,                                                                                                  v               )
				PROPERTYDECL(noise,          D2D1_TURBULENCE_PROP_NOISE,          D2D1_TURBULENCE_NOISE, D2D1_TURBULENCE_NOISE_FRACTAL_SUM, ,                                                                                                  v               )
				PROPERTYDECL(stitchable,     D2D1_TURBULENCE_PROP_STITCHABLE,     bool,                  false,                             ,                                                                                                  v ? TRUE : FALSE)
			};
		};
		using bitmap_source = source::bitmap_source;
		using flood = source::flood;
		using turbulence = source::turbulence;
		struct transform{
		transform() = delete;
			struct _2d_affine_transform : impl<_2d_affine_transform, CLSID_D2D12DAffineTransform>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(interpolation_mode, D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE, D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR, ,                                                                                             v)
				PROPERTYDECL(border_mode,        D2D1_2DAFFINETRANSFORM_PROP_BORDER_MODE,        D2D1_BORDER_MODE,                          D2D1_BORDER_MODE_SOFT,                            ,                                                                                             v)
				PROPERTYDECL(transform_matrix,   D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX,   D2D1_MATRIX_3X2_F,                         D2D1::Matrix3x2F::Identity(),                     ,                                                                                             v)
				PROPERTYDECL(sharpness,          D2D1_2DAFFINETRANSFORM_PROP_SHARPNESS,          float,                                     1.f,                                              if(v < 0.f || 1.f < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
			};
			struct _3d_transform : impl<_3d_transform, CLSID_D2D13DTransform>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(interpolation_mode, D2D1_3DTRANSFORM_PROP_INTERPOLATION_MODE, D2D1_3DTRANSFORM_INTERPOLATION_MODE, D2D1_3DTRANSFORM_INTERPOLATION_MODE_LINEAR,                                                       , v)
				PROPERTYDECL(border_mode,        D2D1_3DTRANSFORM_PROP_BORDER_MODE,        D2D1_BORDER_MODE,                    D2D1_BORDER_MODE_SOFT,                                                                            , v)
				PROPERTYDECL(transform_matrix,   D2D1_3DTRANSFORM_PROP_TRANSFORM_MATRIX,   const D2D1_MATRIX_4X4_F&,            D2D1::Matrix4x4F(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f), , v)
			};
			struct _3d_perspective_transform : impl<_3d_perspective_transform, CLSID_D2D13DPerspectiveTransform>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(interpolation_mode, D2D1_3DPERSPECTIVETRANSFORM_PROP_INTERPOLATION_MODE, D2D1_3DPERSPECTIVETRANSFORM_INTERPOLATION_MODE, D2D1_3DPERSPECTIVETRANSFORM_INTERPOLATION_MODE_LINEAR, ,                                                                                  v)
				PROPERTYDECL(border_mode,        D2D1_3DPERSPECTIVETRANSFORM_PROP_BORDER_MODE,        D2D1_BORDER_MODE,                               D2D1_BORDER_MODE_SOFT,                                 ,                                                                                  v)
				PROPERTYDECL(depth,              D2D1_3DPERSPECTIVETRANSFORM_PROP_DEPTH,              float,                                          1000.f,                                                if(v <= 0.f)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(perspective_origin, D2D1_3DPERSPECTIVETRANSFORM_PROP_PERSPECTIVE_ORIGIN, const D2D1_VECTOR_2F&,                          D2D1::Vector2F(0.f, 0.f),                              ,                                                                                  v)
				expected<self&, hresult_error> set_perspective_origin(const D2D1_VECTOR_3F& v = {0.f, 0.f, 1000.f})& {return                  set_perspective_origin(D2D1::Vector2F(v.x, v.y)).bind([&v](self&  x){return           x .set_depth(v.z);});}
				expected<self , hresult_error> set_perspective_origin(const D2D1_VECTOR_3F& v = {0.f, 0.f, 1000.f})&&{return std::move(*this).set_perspective_origin(D2D1::Vector2F(v.x, v.y)).bind([&v](self&& x){return std::move(x).set_depth(v.z);});}
				PROPERTYDECL(local_offset,       D2D1_3DPERSPECTIVETRANSFORM_PROP_LOCAL_OFFSET,       const D2D1_VECTOR_3F&,                          D2D1::Vector3F(0.f, 0.f, 0.f),                         ,                                                                                  v)
				PROPERTYDECL(global_offset,      D2D1_3DPERSPECTIVETRANSFORM_PROP_GLOBAL_OFFSET,      const D2D1_VECTOR_3F&,                          D2D1::Vector3F(0.f, 0.f, 0.f),                         ,                                                                                  v)
				PROPERTYDECL(rotation_origin,    D2D1_3DPERSPECTIVETRANSFORM_PROP_ROTATION_ORIGIN,    const D2D1_VECTOR_3F&,                          D2D1::Vector3F(0.f, 0.f, 0.f),                         ,                                                                                  v)
				PROPERTYDECL(rotation,           D2D1_3DPERSPECTIVETRANSFORM_PROP_ROTATION,           const D2D1_VECTOR_3F&,                          D2D1::Vector3F(0.f, 0.f, 0.f),                         ,                                                                                  v)
			};
			class border : public impl<border, CLSID_D2D1Border>{
				//template<typename T>
				class _edge_mode{
					border& b;
				public:
					explicit _edge_mode(border& b_):b{b_}{}
					expected<border&, hresult_error> set_x(D2D1_BORDER_EDGE_MODE v = D2D1_BORDER_EDGE_MODE_CLAMP)& {const auto result = b.set(D2D1_BORDER_PROP_EDGE_MODE_X, v);if(result)return           b ;return make_unexpected(std::move(result.error()));}
					expected<border , hresult_error> set_x(D2D1_BORDER_EDGE_MODE v = D2D1_BORDER_EDGE_MODE_CLAMP)&&{const auto result = b.set(D2D1_BORDER_PROP_EDGE_MODE_X, v);if(result)return std::move(b);return make_unexpected(std::move(result.error()));}
					__declspec(property(put=set_x)) D2D1_BORDER_EDGE_MODE x;
					expected<border&, hresult_error> set_y(D2D1_BORDER_EDGE_MODE v = D2D1_BORDER_EDGE_MODE_CLAMP)& {const auto result = b.set(D2D1_BORDER_PROP_EDGE_MODE_Y, v);if(result)return           b ;return make_unexpected(std::move(result.error()));}
					expected<border , hresult_error> set_y(D2D1_BORDER_EDGE_MODE v = D2D1_BORDER_EDGE_MODE_CLAMP)&&{const auto result = b.set(D2D1_BORDER_PROP_EDGE_MODE_Y, v);if(result)return std::move(b);return make_unexpected(std::move(result.error()));}
					__declspec(property(put=set_y)) D2D1_BORDER_EDGE_MODE y;
				};
				friend _edge_mode/*<D2D1_BORDER_EDGE_MODE>*/;
			public:
				using impl::impl;
				using impl::operator|=;
				expected<self&, hresult_error> set_edge_mode_x(D2D1_BORDER_EDGE_MODE v = D2D1_BORDER_EDGE_MODE_CLAMP)& {const auto result = this->set(D2D1_BORDER_PROP_EDGE_MODE_X, v);if(result)return           *this ;return make_unexpected(std::move(result.error()));}
				expected<self , hresult_error> set_edge_mode_x(D2D1_BORDER_EDGE_MODE v = D2D1_BORDER_EDGE_MODE_CLAMP)&&{const auto result = this->set(D2D1_BORDER_PROP_EDGE_MODE_X, v);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}
				expected<self&, hresult_error> set_edge_mode_y(D2D1_BORDER_EDGE_MODE v = D2D1_BORDER_EDGE_MODE_CLAMP)& {const auto result = this->set(D2D1_BORDER_PROP_EDGE_MODE_Y, v);if(result)return           *this ;return make_unexpected(std::move(result.error()));}
				expected<self , hresult_error> set_edge_mode_y(D2D1_BORDER_EDGE_MODE v = D2D1_BORDER_EDGE_MODE_CLAMP)&&{const auto result = this->set(D2D1_BORDER_PROP_EDGE_MODE_Y, v);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}
				_edge_mode/*<D2D1_BORDER_EDGE_MODE>*/ get_edge_mode()noexcept{return _edge_mode/*<D2D1_BORDER_EDGE_MODE>*/{*this};}
				expected<self&, hresult_error> set_edge_mode(D2D1_BORDER_EDGE_MODE v = D2D1_BORDER_EDGE_MODE_CLAMP)& {return                  set_edge_mode_x(v).bind([&v](self&  x){return           x .set_edge_mode_y(v);});}
				expected<self , hresult_error> set_edge_mode(D2D1_BORDER_EDGE_MODE v = D2D1_BORDER_EDGE_MODE_CLAMP)&&{return std::move(*this).set_edge_mode_x(v).bind([&v](self&& x){return std::move(x).set_edge_mode_y(v);});}
				__declspec(property(get=get_edge_mode, put=set_edge_mode)) _edge_mode/*<D2D1_BORDER_EDGE_MODE>*/ edge_mode;
			};
			struct crop : impl<crop, CLSID_D2D1Crop>{
				using impl::impl;
				using impl::operator|=;
				expected<self&, hresult_error> set_rect(const D2D1_VECTOR_4F& v = {-FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX})& {const auto result = this->set(D2D1_CROP_PROP_RECT, v);if(result)return           *this ;return make_unexpected(std::move(result.error()));}
				expected<self , hresult_error> set_rect(const D2D1_VECTOR_4F& v = {-FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX})&&{const auto result = this->set(D2D1_CROP_PROP_RECT, v);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}
				expected<self&, hresult_error> set_rect(const two_dim::xywh<float>& v = {{-FLT_MAX, -FLT_MAX}, {FLT_MAX, FLT_MAX}})& {return                  set_rect(D2D1::Vector4F(v.xy.x, v.xy.y, v.wh.w, v.wh.h));}
				expected<self , hresult_error> set_rect(const two_dim::xywh<float>& v = {{-FLT_MAX, -FLT_MAX}, {FLT_MAX, FLT_MAX}})&&{return std::move(*this).set_rect(D2D1::Vector4F(v.xy.x, v.xy.y, v.wh.w, v.wh.h));}
				__declspec(property(put=set_rect)) two_dim::xywh<float> rect;
				PROPERTYDECL(border_mode, D2D1_CROP_PROP_BORDER_MODE, D2D1_BORDER_MODE, D2D1_BORDER_MODE_SOFT, , v)
			};
		private:
			struct _scale_effect : impl<_scale_effect, CLSID_D2D1Scale>{
				using impl::impl;
				using impl::operator|=;
				PROPERTYDECL(scale,              D2D1_SCALE_PROP_SCALE,              const D2D1_VECTOR_2F&,         D2D1::Vector2F(1.f, 1.f),             if(v.x < 0 || v.y < 0)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG), v)
				PROPERTYDECL(center_point,       D2D1_SCALE_PROP_CENTER_POINT,       const D2D1_VECTOR_2F&,         D2D1::Vector2F(0.f, 0.f),             ,                                                                                            v)
				PROPERTYDECL(border_mode,        D2D1_SCALE_PROP_BORDER_MODE,        D2D1_BORDER_MODE,              D2D1_BORDER_MODE_SOFT,                ,                                                                                            v)
				PROPERTYDECL(sharpness,          D2D1_SCALE_PROP_SHARPNESS,          float,                         0.f,                                  if(v < 0 || 1 < v)return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG),     v)
				PROPERTYDECL(interpolation_mode, D2D1_SCALE_PROP_INTERPOLATION_MODE, D2D1_SCALE_INTERPOLATION_MODE, D2D1_SCALE_INTERPOLATION_MODE_LINEAR, ,                                                                                            v)
			};
		public:
			using scale = _scale_effect;
			struct tile : impl<tile, CLSID_D2D1Tile>{
				using impl::impl;
				using impl::operator|=;
				expected<self&, hresult_error> set_rect(const D2D1_VECTOR_4F& v = {0.f, 0.f, 100.f, 100.f})& {const auto result = this->set(D2D1_TILE_PROP_RECT, v);if(result)return           *this ;return make_unexpected(std::move(result.error()));}
				expected<self , hresult_error> set_rect(const D2D1_VECTOR_4F& v = {0.f, 0.f, 100.f, 100.f})&&{const auto result = this->set(D2D1_TILE_PROP_RECT, v);if(result)return std::move(*this);return make_unexpected(std::move(result.error()));}
				expected<self&, hresult_error> set_rect(const two_dim::xyxy<float>& v = {{0.f, 0.f}, {100.f, 100.f}})& {return                  set_rect(D2D1::Vector4F(v._1.x, v._1.y, v._2.x, v._2.y));}
				expected<self , hresult_error> set_rect(const two_dim::xyxy<float>& v = {{0.f, 0.f}, {100.f, 100.f}})&&{return std::move(*this).set_rect(D2D1::Vector4F(v._1.x, v._1.y, v._2.x, v._2.y));}
				__declspec(property(put=set_rect)) two_dim::xyxy<float> rect;
			};
		};
		using _2d_affine_transform = transform::_2d_affine_transform;
		using _3d_transform = transform::_3d_transform;
		using _3d_perspective_transform = transform::_3d_perspective_transform;
		using border = transform::border;
		using crop = transform::crop;
		using scale = transform::scale;
		using tile = transform::tile;
		struct transparency{
		transparency() = delete;
			//(Win10)CLSID_D2D1ChromaKey
			struct luminance_to_alpha : impl<luminance_to_alpha, CLSID_D2D1LuminanceToAlpha>{
				using impl::impl;
				using impl::operator|=;
			};
			//(Win10)CLSID_D2D1Opacity
		};
		using luminance_to_alpha = transparency::luminance_to_alpha;
#undef  PROPERTYDECL
		};
		struct layer : d2d_resource<ID2D1Layer>{
			using d2d_resource::d2d_resource;
			struct parameter : detail::property<D2D1_LAYER_PARAMETERS1>{
				explicit parameter(){prop.contentBounds = D2D1::InfiniteRect();prop.geometricMask = nullptr;prop.maskAntialiasMode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;prop.maskTransform = D2D1::IdentityMatrix();prop.opacity = 1.f;prop.opacityBrush = nullptr;prop.layerOptions = D2D1_LAYER_OPTIONS1_NONE;}
		#define PROPERTYDECL(name, type, membername) parameter& name(type t){prop.membername = t;return *this;}
				PROPERTYDECL(content_bounds, const D2D1_RECT_F&, contentBounds)
				PROPERTYDECL(geometric_mask, ID2D1Geometry*, geometricMask)
				PROPERTYDECL(mask_antialias_mode, D2D1_ANTIALIAS_MODE, maskAntialiasMode)
				PROPERTYDECL(mask_transform, D2D1_MATRIX_3X2_F, maskTransform)
				PROPERTYDECL(opacity, FLOAT, opacity)
				PROPERTYDECL(opacity_brush, ID2D1Brush*, opacityBrush)
				PROPERTYDECL(layer_options, D2D1_LAYER_OPTIONS1, layerOptions)
		#undef  PROPERTYDECL
			};
			D2D1_SIZE_F get_dip_size()const{return (*this)->GetSize();D2D1::LayerParameters1();}
		};
		struct render_target_property : detail::property<D2D1_RENDER_TARGET_PROPERTIES>{
			class dpi_property{
				render_target_property* prop;
			public:
				dpi_property(render_target_property* p):prop(p){}
				dpi_property(const dpi_property&) = delete;
				dpi_property(dpi_property&&) = delete;
				dpi_property& operator=(const dpi_property&) = delete;
				dpi_property& operator=(dpi_property&&) = delete;
				render_target_property& operator()(FLOAT x, FLOAT y)const{prop->prop.dpiX = x; prop->prop.dpiY = y; return *prop;}
				render_target_property& operator()(const two_dim::xy<FLOAT>& xy)const{return (*this)(xy.x, xy.y);}
				render_target_property& x(FLOAT x)const{prop->prop.dpiX = x; return *prop;}
				render_target_property& y(FLOAT y)const{prop->prop.dpiY = y; return *prop;}
			};
			friend class dpi_property;
		public:
			explicit render_target_property(){prop.pixelFormat = {DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED};}
	#define PROPERTYDECL(name, type, membername) render_target_property& name(type t){prop.membername = t;return *this;}
			PROPERTYDECL(type, D2D1_RENDER_TARGET_TYPE, type)
			PROPERTYDECL(format, DXGI_FORMAT, pixelFormat.format)
			PROPERTYDECL(alpha_mode, D2D1_ALPHA_MODE, pixelFormat.alphaMode)
			const dpi_property dpi = this;
			PROPERTYDECL(usage, D2D1_RENDER_TARGET_USAGE, usage)
			PROPERTYDECL(min_level, D2D1_FEATURE_LEVEL, minLevel)
	#undef  PROPERTYDECL
		};
	private:
		template<typename I>
		struct render_target : d2d_resource<I>{
			using d2d_resource::d2d_resource;
			using property = render_target_property;
			struct region{
				ID2D1RenderTarget* rt;
				D2D1_RECT_U rect;
			};
			region get_region(const D2D1_RECT_U& r)const{return {get(), r};}
			class renderer{
				ID2D1DeviceContext* c;
			public:
				explicit renderer(ID2D1DeviceContext* r):c(std::move(r)){}
				renderer(const renderer&) = delete;
				renderer(renderer&&) = default;
				renderer& operator=(const renderer&) = delete;
				renderer& operator=(renderer&&) = default;
				~renderer() = default;
				ID2D1DeviceContext* get()const{return c;}
				ID2D1DeviceContext* operator->()const{return get();}
				template<typename F>
				renderer& layer(const D2D1_LAYER_PARAMETERS1& params, ID2D1Layer* layer, F&& f){(*this)->PushLayer(&params, layer);f(*this);(*this)->PopLayer();return *this;}
				template<typename Layer, typename F>
				renderer& layer(const D2D1_LAYER_PARAMETERS1& params, Layer&& l, F&& f){return layer(params, std::forward<Layer>(l).get(), std::forward<F>(f));}
				template<typename F>
				renderer& axis_aligned_clip(const D2D1_RECT_F& clip_rect, D2D1_ANTIALIAS_MODE aa_mode, F&& f){(*this)->PushAxisAlignedClip(clip_rect);f(*this);(*this)->PopAxisAlignedClip();return *this;}
				template<typename F>
				renderer& axis_aligned_clip(const D2D1_RECT_F& clip_rect, F&& f){return axis_aligned_clip(clip_rect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE, std::forward<F>(f));}
				renderer& clear(const D2D_COLOR_F& col = D2D1::ColorF{D2D1::ColorF::Black, 0.F}){(*this)->Clear(col); return *this;}
				renderer& image(ID2D1Image* image, D2D1_INTERPOLATION_MODE interpolation_mode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE composite_mode = D2D1_COMPOSITE_MODE_SOURCE_OVER){(*this)->DrawImage(image, interpolation_mode, composite_mode);return *this;}
				renderer& image(ID2D1Effect* effect, D2D1_INTERPOLATION_MODE interpolation_mode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE composite_mode = D2D1_COMPOSITE_MODE_SOURCE_OVER){(*this)->DrawImage(effect, interpolation_mode, composite_mode);return *this;}
				template<typename Image, std::enable_if_t<!std::is_base_of<ID2D1Image, std::remove_pointer_t<std::decay_t<Image>>>::value>* = nullptr>
				renderer& image(Image&& im, D2D1_INTERPOLATION_MODE interpolation_mode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE composite_mode = D2D1_COMPOSITE_MODE_SOURCE_OVER){return image(std::forward<Image>(im).get(), interpolation_mode, composite_mode);}
				renderer& image(ID2D1Image* image, const D2D1_POINT_2F& target_offset, D2D1_INTERPOLATION_MODE interpolation_mode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE composite_mode = D2D1_COMPOSITE_MODE_SOURCE_OVER){(*this)->DrawImage(image, &target_offset, nullptr, interpolation_mode, composite_mode);return *this;}
				renderer& image(ID2D1Effect* effect, const D2D1_POINT_2F& target_offset, D2D1_INTERPOLATION_MODE interpolation_mode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE composite_mode = D2D1_COMPOSITE_MODE_SOURCE_OVER){(*this)->DrawImage(effect, &target_offset, nullptr, interpolation_mode, composite_mode);return *this;}
				template<typename Image, std::enable_if_t<!std::is_base_of<ID2D1Image, std::remove_pointer_t<std::decay_t<Image>>>::value>* = nullptr>
				renderer& image(Image&& im, const D2D1_POINT_2F& target_offset, D2D1_INTERPOLATION_MODE interpolation_mode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE composite_mode = D2D1_COMPOSITE_MODE_SOURCE_OVER){return image(std::forward<Image>(im).get(), target_offset, interpolation_mode, composite_mode);}
				renderer& image(ID2D1Image* image, const D2D1_RECT_F& image_rect, D2D1_INTERPOLATION_MODE interpolation_mode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE composite_mode = D2D1_COMPOSITE_MODE_SOURCE_OVER){(*this)->DrawImage(image, nullptr, &image_rect, interpolation_mode, composite_mode);return *this;}
				renderer& image(ID2D1Effect* effect, const D2D1_RECT_F& image_rect, D2D1_INTERPOLATION_MODE interpolation_mode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE composite_mode = D2D1_COMPOSITE_MODE_SOURCE_OVER){(*this)->DrawImage(effect, nullptr, &image_rect, interpolation_mode, composite_mode);return *this;}
				template<typename Image, std::enable_if_t<!std::is_base_of<ID2D1Image, std::remove_pointer_t<std::decay_t<Image>>>::value>* = nullptr>
				renderer& image(Image&& im, const D2D1_RECT_F& image_rect, D2D1_INTERPOLATION_MODE interpolation_mode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE composite_mode = D2D1_COMPOSITE_MODE_SOURCE_OVER){return image(std::forward<Image>(im).get(), image_rect, interpolation_mode, composite_mode);}
				renderer& image(ID2D1Image* image, const D2D1_POINT_2F& target_offset, const D2D1_RECT_F& image_rect, D2D1_INTERPOLATION_MODE interpolation_mode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE composite_mode = D2D1_COMPOSITE_MODE_SOURCE_OVER){(*this)->DrawImage(image, &target_offset, &image_rect, interpolation_mode, composite_mode);return *this;}
				renderer& image(ID2D1Effect* effect, const D2D1_POINT_2F& target_offset, const D2D1_RECT_F& image_rect, D2D1_INTERPOLATION_MODE interpolation_mode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE composite_mode = D2D1_COMPOSITE_MODE_SOURCE_OVER){(*this)->DrawImage(effect, &target_offset, &image_rect, interpolation_mode, composite_mode);return *this;}
				template<typename Image, std::enable_if_t<!std::is_base_of<ID2D1Image, std::remove_pointer_t<std::decay_t<Image>>>::value>* = nullptr>
				renderer& image(Image&& im, const D2D1_POINT_2F& target_offset, const D2D1_RECT_F image_rect, D2D1_INTERPOLATION_MODE interpolation_mode = D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE composite_mode = D2D1_COMPOSITE_MODE_SOURCE_OVER){return image(std::forward<Image>(im).get(), target_offset, image_rect, interpolation_mode, composite_mode);}
				renderer& text(const std::wstring& str, IDWriteTextFormat* format, const D2D1_RECT_F& rect, ID2D1Brush* br, D2D1_DRAW_TEXT_OPTIONS opt = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE measuring_mode = DWRITE_MEASURING_MODE_NATURAL){(*this)->DrawText(str.data(), static_cast<UINT32>(str.size()), format, rect, br, opt, measuring_mode);return *this;}
				template<std::size_t N>renderer& text(const WCHAR (&str)[N], IDWriteTextFormat* format, const D2D1_RECT_F& rect, ID2D1Brush* br, D2D1_DRAW_TEXT_OPTIONS opt = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE measuring_mode = DWRITE_MEASURING_MODE_NATURAL){(*this)->DrawText(str, static_cast<UINT32>(N), format, rect, br, opt, measuring_mode); return *this;}
				renderer& text(const WCHAR* str, IDWriteTextFormat* format, const D2D1_RECT_F& rect, ID2D1Brush* br, D2D1_DRAW_TEXT_OPTIONS opt = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE measuring_mode = DWRITE_MEASURING_MODE_NATURAL){(*this)->DrawText(str, static_cast<UINT32>(::wcslen(str)), format, rect, br, opt, measuring_mode); return *this;}
				template<typename String, typename Format, std::enable_if_t<!std::is_same<std::decay_t<Format>, IDWriteTextFormat*>::value>* = nullptr>
				renderer& text(String&& str, Format&& format, const D2D1_RECT_F& rect, ID2D1Brush* br, D2D1_DRAW_TEXT_OPTIONS opt = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE measuring_mode = DWRITE_MEASURING_MODE_NATURAL){return text(str, format.get(), rect, br, opt, measuring_mode);}
				template<typename String, typename Brush, std::enable_if_t<!std::is_base_of<ID2D1Brush, std::remove_pointer_t<std::decay_t<Brush>>>::value>* = nullptr>
				renderer& text(String&& str, IDWriteTextFormat* format, const D2D1_RECT_F& rect, Brush&& br, D2D1_DRAW_TEXT_OPTIONS opt = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE measuring_mode = DWRITE_MEASURING_MODE_NATURAL){return text(str, format, rect, br.get(), opt, measuring_mode);}
				template<typename String, typename Format, typename Brush, std::enable_if_t<!std::is_same<std::decay_t<Format>, IDWriteTextFormat*>::value && !std::is_base_of<ID2D1Brush, std::remove_pointer_t<std::decay_t<Brush>>>::value>* = nullptr>
				renderer& text(String&& str, Format&& format, const D2D1_RECT_F& rect, Brush&& br, D2D1_DRAW_TEXT_OPTIONS opt = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE measuring_mode = DWRITE_MEASURING_MODE_NATURAL){return text(str, format.get(), rect, br.get(), opt, measuring_mode);}
				template<typename String, typename Format, typename TwoDim, typename Brush, std::enable_if_t<!std::is_same<std::decay_t<Format>, IDWriteTextFormat*>::value && !std::is_base_of<ID2D1Brush, std::remove_pointer_t<std::decay_t<Brush>>>::value && !std::is_same<std::decay_t<TwoDim>, D2D1_RECT_F>::value>* = nullptr>
				renderer& text(String&& str, Format&& format, TwoDim&& rect, Brush&& br, D2D1_DRAW_TEXT_OPTIONS opt = D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE measuring_mode = DWRITE_MEASURING_MODE_NATURAL){return text(str, format.get(), rect.attribute<D2D1_RECT_F>(), br.get(), opt, measuring_mode);}
				renderer& rect(const D2D1_RECT_F& r, ID2D1Brush* brush, FLOAT stroke_width = 1.F, ID2D1StrokeStyle1* stroke_style = nullptr){(*this)->DrawRectangle(r, brush, stroke_width, stroke_style);return *this;}
				template<typename Brush, std::enable_if_t<!std::is_base_of<ID2D1Brush, std::remove_pointer_t<std::decay_t<Brush>>>::value>* = nullptr>
				renderer& rect(const D2D1_RECT_F& r, Brush&& brush, FLOAT stroke_width = 1.F, ID2D1StrokeStyle1* stroke_style = nullptr){return rect(r, std::forward<Brush>(brush).get(), stroke_width, stroke_style);}
				template<typename StrokeStyle, std::enable_if_t<!std::is_same<std::decay_t<StrokeStyle>, ID2D1StrokeStyle1*>::value>* = nullptr>
				renderer& rect(const D2D1_RECT_F& r, ID2D1Brush* brush, FLOAT stroke_width, StrokeStyle&& stroke_style){return rect(r, brush, stroke_width, std::forward<StrokeStyle>(stroke_style).get());}
				template<typename Brush, typename StrokeStyle, std::enable_if_t<!std::is_base_of<ID2D1Brush, std::remove_pointer_t<std::decay_t<Brush>>>::value && !std::is_same<std::decay_t<StrokeStyle>, ID2D1StrokeStyle1*>::value>* = nullptr>
				renderer& rect(const D2D1_RECT_F& r, Brush&& brush, FLOAT stroke_width, StrokeStyle&& stroke_style){return rect(r, std::forward<Brush>(brush).get(), stroke_width, std::forward<StrokeStyle>(stroke_style).get());}
				renderer& rect(const D2D1_RECT_F& r, ID2D1Brush* brush, ID2D1StrokeStyle1* stroke_style){return rect(r, brush, 1.F, stroke_style);}
				template<typename Brush, std::enable_if_t<!std::is_base_of<ID2D1Brush, std::remove_pointer_t<std::decay_t<Brush>>>::value>* = nullptr>
				renderer& rect(const D2D1_RECT_F& r, Brush&& brush, ID2D1StrokeStyle1* stroke_style){return rect(r, std::forward<Brush>(brush).get(), stroke_style);}
				template<typename StrokeStyle, std::enable_if_t<!std::is_same<std::decay_t<StrokeStyle>, ID2D1StrokeStyle1*>::value>* = nullptr>
				renderer& rect(const D2D1_RECT_F& r, ID2D1Brush* brush, StrokeStyle&& stroke_style){return rect(r, brush, std::forward<StrokeStyle>(stroke_style).get());}
				template<typename Brush, typename StrokeStyle, std::enable_if_t<!std::is_base_of<ID2D1Brush, std::remove_pointer_t<std::decay_t<Brush>>>::value && !std::is_same<std::decay_t<StrokeStyle>, ID2D1StrokeStyle1*>::value>* = nullptr>
				renderer& rect(const D2D1_RECT_F& r, Brush&& brush, StrokeStyle&& stroke_style){return rect(r, std::forward<Brush>(brush).get(), std::forward<StrokeStyle>(stroke_style).get());}
				renderer& rect(const D2D1_RECT_F& r, ID2D1Brush* brush, fill_t){(*this)->FillRectangle(r, brush);return *this;}
				template<typename Brush, std::enable_if_t<!std::is_base_of<ID2D1Brush, std::remove_pointer_t<std::decay_t<Brush>>>::value>* = nullptr>
				renderer& rect(const D2D1_RECT_F& r, Brush&& brush, fill_t){return rect(r, std::forward<Brush>(brush).get(), fill);}
				renderer& bitmap(ID2D1Bitmap1* bm, FLOAT opacity = 1.F, D2D1_INTERPOLATION_MODE interop = D2D1_INTERPOLATION_MODE_LINEAR){(*this)->DrawBitmap(bm, nullptr, opacity, interop, nullptr, nullptr);return *this;}
				template<typename Bitmap, std::enable_if_t<!std::is_same<std::decay_t<Bitmap>, ID2D1Bitmap1*>::value && !std::is_same<std::decay_t<Bitmap>, bitmap::lazy_manipulator>::value>* = nullptr>
				renderer& bitmap(Bitmap&& bm, FLOAT opacity = 1.F, D2D1_INTERPOLATION_MODE interop = D2D1_INTERPOLATION_MODE_LINEAR){return bitmap(std::forward<Bitmap>(bm).get(), opacity, interop);}
				renderer& bitmap(ID2D1Bitmap1* bm, const D2D1_RECT_F& dest_rect, FLOAT opacity = 1.F, D2D1_INTERPOLATION_MODE interop = D2D1_INTERPOLATION_MODE_LINEAR){(*this)->DrawBitmap(bm, dest_rect, opacity, interop, nullptr, nullptr);return *this;}
				template<typename Bitmap, std::enable_if_t<!std::is_same<std::decay_t<Bitmap>, ID2D1Bitmap1*>::value && !std::is_same<std::decay_t<Bitmap>, bitmap::lazy_manipulator>::value>* = nullptr>
				renderer& bitmap(Bitmap&& bm, const D2D1_RECT_F& dest_rect, FLOAT opacity = 1.F, D2D1_INTERPOLATION_MODE interop = D2D1_INTERPOLATION_MODE_LINEAR){return bitmap(std::forward<Bitmap>(bm).get(), dest_rect, opacity, interop);}
				renderer& bitmap(ID2D1Bitmap1* bm, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_RECT_F& src_rect){(*this)->DrawBitmap(bm, nullptr, opacity, interop, &src_rect, nullptr);return *this;}
				template<typename Bitmap, std::enable_if_t<!std::is_same<std::decay_t<Bitmap>, ID2D1Bitmap1*>::value && !std::is_same<std::decay_t<Bitmap>, bitmap::lazy_manipulator>::value>* = nullptr>
				renderer& bitmap(Bitmap&& bm, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_RECT_F& src_rect){return bitmap(std::fowrard<Bitmap>(bm), opacity, interop, src_rect);}
				renderer& bitmap(ID2D1Bitmap1* bm, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_MATRIX_4X4_F& perspective_transform){(*this)->DrawBitmap(bm, nullptr, opacity, interop, nullptr, &perspective_transform);return *this;}
				template<typename Bitmap, std::enable_if_t<!std::is_same<std::decay_t<Bitmap>, ID2D1Bitmap1*>::value && !std::is_same<std::decay_t<Bitmap>, bitmap::lazy_manipulator>::value>* = nullptr>
				renderer& bitmap(Bitmap&& bm, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_MATRIX_4X4_F& perspective_transform){return bitmap(std::forward<Bitmap>(bm).get(), opacity, interop, perspective_transform);}
				renderer& bitmap(ID2D1Bitmap1* bm, const D2D1_RECT_F& dest_rect, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_RECT_F& src_rect){(*this)->DrawBitmap(bm, &dest_rect, opacity, interop, &src_rect, nullptr);return *this;}
				template<typename Bitmap, std::enable_if_t<!std::is_same<std::decay_t<Bitmap>, ID2D1Bitmap1*>::value && !std::is_same<std::decay_t<Bitmap>, bitmap::lazy_manipulator>::value>* = nullptr>
				renderer& bitmap(Bitmap&& bm, const D2D1_RECT_F& dest_rect, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_RECT_F& src_rect){return bitmap(std::forward<Bitmap>(bm).get(), dest_rect, opacity, interop, src_rect);}
				renderer& bitmap(ID2D1Bitmap1* bm, const D2D1_RECT_F& dest_rect, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_MATRIX_4X4_F& perspective_transform){(*this)->DrawBitmap(bm, &dest_rect, opacity, interop, nullptr, &perspective_transform);return *this;}
				template<typename Bitmap, std::enable_if_t<!std::is_same<std::decay_t<Bitmap>, ID2D1Bitmap1*>::value && !std::is_same<std::decay_t<Bitmap>, bitmap::lazy_manipulator>::value>* = nullptr>
				renderer& bitmap(Bitmap&& bm, const D2D1_RECT_F& dest_rect, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_MATRIX_4X4_F& perspective_transform){return bitmap(std::forward<Bitmap>(bm).get(), dest_rect, opacity, interop, perspective_transform);}
				renderer& bitmap(ID2D1Bitmap1* bm, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_RECT_F& src_rect, const D2D1_MATRIX_4X4_F& perspective_transform){(*this)->DrawBitmap(bm, nullptr, opacity, interop, &src_rect, &perspective_transform);return *this;}
				template<typename Bitmap, std::enable_if_t<!std::is_same<std::decay_t<Bitmap>, ID2D1Bitmap1*>::value && !std::is_same<std::decay_t<Bitmap>, bitmap::lazy_manipulator>::value>* = nullptr>
				renderer& bitmap(Bitmap&& bm, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_RECT_F& src_rect, const D2D1_MATRIX_4X4_F& perspective_transform){return bitmap(std::forward<Bitmap>(bm).get(), opacity, interop, src_rect, perspective_transform);}
				renderer& bitmap(ID2D1Bitmap1* bm, const D2D1_RECT_F& dest_rect, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_RECT_F& src_rect, const D2D1_MATRIX_4X4_F& perspective_transform){(*this)->DrawBitmap(bm, dest_rect, opacity, interop, src_rect, perspective_transform);return *this;}
				template<typename Bitmap, std::enable_if_t<!std::is_same<std::decay_t<Bitmap>, ID2D1Bitmap1*>::value && !std::is_same<std::decay_t<Bitmap>, bitmap::lazy_manipulator>::value>* = nullptr>
				renderer& bitmap(Bitmap&& bm, const D2D1_RECT_F& dest_rect, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_RECT_F& src_rect, const D2D1_MATRIX_4X4_F& perspective_transform){return bitmap(std::forward<Bitmap>(bm).get(), dest_rect, opacity, interop, src_rect, perspective_transform);}
				renderer& bitmap(const bitmap::lazy_manipulator& bm, FLOAT opacity = 1.F, D2D1_INTERPOLATION_MODE interop = D2D1_INTERPOLATION_MODE_LINEAR){return bitmap(bm.bm.get(), opacity, interop, bm.mat);}
				renderer& bitmap(const bitmap::lazy_manipulator& bm, const D2D1_RECT_F& dest_rect, FLOAT opacity = 1.F, D2D1_INTERPOLATION_MODE interop = D2D1_INTERPOLATION_MODE_LINEAR){return bitmap(bm.bm.get(), dest_rect, opacity, interop, bm.mat);}
				renderer& bitmap(const bitmap::lazy_manipulator& bm, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_RECT_F& src_rect){return bitmap(bm.bm.get(), opacity, interop, src_rect, bm.mat);}
				renderer& bitmap(const bitmap::lazy_manipulator& bm, const D2D1_RECT_F& dest_rect, FLOAT opacity, D2D1_INTERPOLATION_MODE interop, const D2D1_RECT_F& src_rect){return bitmap(bm.bm.get(), dest_rect, opacity, interop, src_rect, bm.mat);}
			};
			std::pair<D2D1_TAG, D2D1_TAG> get_tags()const{std::pair<D2D1_TAG, D2D1_TAG> tags = {};(*this)->GetTags(&tags.first, &tags.second);return tags;}
			void set_tags(const std::pair<D2D1_TAG, D2D1_TAG>& tags){(*this)->SetTags(tags.first, tags.second);}
			__declspec(property(get=get_tags, put=set_tags)) std::pair<D2D1_TAG, D2D1_TAG> tags;
			expected<layer, hresult_error> create_layer()const{return detail::convert_to_rich_interface<layer>(com_create_resource<ID2D1Layer>([&](ID2D1Layer** ptr){return (*this)->CreateLayer(ptr);}), _T(__FUNCTION__));}
			expected<layer, hresult_error> create_layer(const D2D1_SIZE_F& s)const{return detail::convert_to_rich_interface<layer>(com_create_resource<ID2D1Layer>([&](ID2D1Layer** ptr){return (*this)->CreateLayer(s, ptr);}), _T(__FUNCTION__));}
			expected<solid_color_brush, hresult_error> create_solid_color_brush(const D2D1_COLOR_F& col, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties())const{
				return detail::convert_to_rich_interface<solid_color_brush>(com_create_resource<ID2D1SolidColorBrush>([&](ID2D1SolidColorBrush** x){return (*this)->CreateSolidColorBrush(col, prop, x);}), _T(__FUNCTION__));
			}
			expected<linear_gradient_brush, hresult_error> create_linear_gradient_brush(const gradient_stop_collection& stops, const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES& pos, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties())const{
				return detail::convert_to_rich_interface<linear_gradient_brush>(com_create_resource<ID2D1LinearGradientBrush>([&](ID2D1LinearGradientBrush** x){return (*this)->CreateLinearGradientBrush(pos, prop, stops.get(), x);}), _T(__FUNCTION__));
			}
			expected<radial_gradient_brush, hresult_error> create_radial_gradient_brush(const gradient_stop_collection& stops, const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES& pos, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties())const{
				return detail::convert_to_rich_interface<radial_gradient_brush>(com_create_resource<ID2D1RadialGradientBrush>([&](ID2D1RadialGradientBrush** x){return (*this)->CreateRadialGradientBrush(pos, prop, stops.get(), x);}), _T(__FUNCTION__));
			}
			D2D1_ANTIALIAS_MODE get_antialias_mode()const{return (*this)->GetAntialiasMode();}
			void set_antialias_mode(D2D1_ANTIALIAS_MODE t){(*this)->SetAntialiasMode(t);}
			__declspec(property(get=get_antialias_mode, put=set_antialias_mode)) D2D1_ANTIALIAS_MODE antialias_mode;
			D2D1_TEXT_ANTIALIAS_MODE get_text_antialias_mode()const{return (*this)->GetTextAntialiasMode();}
			void set_text_antialias_mode(D2D1_TEXT_ANTIALIAS_MODE t){(*this)->SetTextAntialiasMode(t);}
			__declspec(property(get=get_text_antialias_mode, put=set_text_antialias_mode)) D2D1_TEXT_ANTIALIAS_MODE text_antialias_mode;
			will::two_dim::xy<FLOAT> get_dpi()const{will::two_dim::xy<FLOAT> dpi;(*this)->GetDpi(&dpi.x, &dpi.y);return dpi;}
			void set_dpi(FLOAT x, FLOAT y)const{(*this)->SetDpi(x, y);}
			void set_dpi(const will::two_dim::xy<FLOAT>& dpi){set_dpi(dpi);}
			__declspec(property(get=get_dpi, put=set_dpi)) will::two_dim::xy<FLOAT> dpi;
			UINT32 get_maximum_bitmap_size()const{return (*this)->GetMaximumBitmapSize();}
			D2D1_SIZE_U get_pixel_size()const{return (*this)->GetPixelSize();}
			D2D1_SIZE_F get_dip_size()const{return (*this)->GetSize();}
			D2D1_PIXEL_FORMAT get_pixel_format()const{return (*this)->GetPixelFormat();}
			dwrite::rendering_params get_text_rendering_params()const{IDWriteRenderingParams1* ptr;(*this)->GetTextRenderingParams(reinterpret_cast<IDWriteRenderingParams**>(&ptr));return dwrite::rendering_params{std::move(ptr)};}
			void set_text_rendering_params(IDWriteRenderingParams1* params){(*this)->SetTextRenderingParams(params);}
			template<typename DWriteRenderingParams>
			void set_text_rendering_params(DWriteRenderingParams&& params){set_text_rendering_params(std::forward<DWriteRenderingParams>(params).get());}
			__declspec(property(get=get_text_rendering_params, put=set_text_rendering_parms)) dwrite::rendering_params text_rendering_params;
			D2D1_MATRIX_3X2_F get_transform()const{D2D1_MATRIX_3X2_F mat;(*this)->GetTransform(&mat);return *this;}
			void set_transform(const D2D1_MATRIX_3X2_F& mat){(*this)->SetTransform(mat);}
			__declspec(property(get=get_transform, put=set_transform)) D2D1_MATRIX_3X2_F transform;
			bool is_supported(const D2D1_RENDER_TARGET_PROPERTIES& prop = property{})const{return (*this)->IsSupported(&prop) == TRUE;}
			render_target& save_drawing_state(drawing_state_block& dsb)const{(*this)->SaveDrawingState(dsb.get());return *this;}
			expected<drawing_state_block, hresult_error> save_drawing_state()const{return get_factory().create_drawing_state_block().map([&](drawing_state_block&& b){save_drawing_state(b);return std::move(b);});}
			void restore_drawing_state(ID2D1DrawingStateBlock* dsb){(*this)->RestoreDrawingState(dsb);}
			template<typename DrawingStateBlock>
			void restore_drawing_state(DrawingStateBlock&& dsb){restore_drawing_state(std::forward<DrawingStateBlock>(dsb).get());}
		};
	public:
		struct bitmap_render_target : render_target<ID2D1BitmapRenderTarget>{
			using render_target::render_target;
			expected<bitmap, hresult_error> get_bitmap()const{return detail::convert_to_rich_interface<bitmap>(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** ptr){return (*this)->GetBitmap(reinterpret_cast<ID2D1Bitmap**>(ptr));}), _T(__FUNCTION__));}
			template<typename F>
			expected<void, hresult_error> draw(F&& f){
				return query_interface<ID2D1DeviceContext>(get()).emap([](HRESULT hr){return make_unexpected<hresult_error>(_T("will::d2d::device::bitmap_render_target::get_renderer"), hr);})
				                                                 .bind([&](ID2D1DeviceContext* x)->expected<void, hresult_error>{
					com_ptr<ID2D1DeviceContext> devcon = std::move(x);
					renderer r{devcon.get()};
					(*this)->BeginDraw();
					f(r);
					std::pair<D2D1_TAG, D2D1_TAG> tag;
					const auto hr = (*this)->EndDraw(&tag.first, &tag.second);
					if(SUCCEEDED(hr))
						return {};
					set_tags(tag);
					return make_unexpected<hresult_error>(_T("will::d2d::device::bitmap_render_target::draw"), hr);
				});
			}
		};
		explicit device(IDXGIDevice* dev):device{+create(dev)}{}
		device(IDXGIDevice* dev, const D2D1_CREATION_PROPERTIES& prop):device{+create(dev, prop)}{}
		template<typename Device>
		explicit device(Device&& dev):device{std::forward<Device>(dev)}{}
		template<typename Device>
		explicit device(Device&& dev, const D2D1_CREATION_PROPERTIES& prop):device{std::forward<Device>(dev), prop}{}
		static expected<device, hresult_error> create(IDXGIDevice* dev){return detail::convert_to_rich_interface<device>(com_create_resource<ID2D1Device>([&](ID2D1Device** ptr){return ::D2D1CreateDevice(dev, nullptr, ptr);}), _T(__FUNCTION__));}
		static expected<device, hresult_error> create(IDXGIDevice* dev, const D2D1_CREATION_PROPERTIES& prop){return detail::convert_to_rich_interface<device>(com_create_resource<ID2D1Device>([&](ID2D1Device** ptr){return ::D2D1CreateDevice(dev, &prop, ptr);}), _T(__FUNCTION__));}
		template<typename Device>
		static expected<device, hresult_error> create(Device&& dev){return create(std::forward<Device>(dev));}
		template<typename Device>
		static expected<device, hresult_error> create(Device&& dev, const D2D1_CREATION_PROPERTIES& prop){return create(std::forward<Device>(dev), prop);}
		UINT64 get_max_texture_memory()const{return (*this)->GetMaximumTextureMemory();}
		void set_max_texture_memory(UINT64 bytes){(*this)->SetMaximumTextureMemory(bytes);}
		__declspec(property(get=_get_max_texture_memory, put=_set_max_texture_memory)) UINT64 max_texture_memory;
		void clear_resource(const std::chrono::milliseconds& since_use)const{(*this)->ClearResources(static_cast<UINT>(since_use.count()));}
		class context : public render_target<ID2D1DeviceContext>{
			using render_target::render_target;
			renderer get_renderer(){return renderer{get()};}
		public:
			static expected<context, hresult_error> create(IDXGISurface2* surface, const D2D1_CREATION_PROPERTIES* prop = nullptr){return detail::convert_to_rich_interface<context>(com_create_resource<ID2D1DeviceContext>([&](ID2D1DeviceContext** ptr){return D2D1CreateDeviceContext(surface, prop, ptr);}), _T(__FUNCTION__));}
			static expected<context, hresult_error> create(IDXGISurface2* surface, const D2D1_CREATION_PROPERTIES& prop){return create(surface, &prop);}
			template<typename Surface>
			static expected<context, hresult_error> create(Surface&& surface, const D2D1_CREATION_PROPERTIES* prop = nullptr){return create(std::forward<Surface>(surface).get(), prop);}
			template<typename Surface>
			static expected<context, hresult_error> create(Surface&& surface, const D2D1_CREATION_PROPERTIES& prop){return create(std::forward<Surface>(surface).get(), &prop);}
			explicit context(IDXGISurface2* surface):context{+create(surface)}{}
			context(IDXGISurface2* surface, const D2D1_CREATION_PROPERTIES& prop):context{+create(surface, prop)}{}
			template<typename Surface>
			explicit context(Surface&& surf):context(+create(std::forward<Surface>(surf))){}
			template<typename Surface>
			explicit context(Surface&& surf, const D2D1_CREATION_PROPERTIES& prop):context(+create(std::forward<Surface>(surf).get(), prop)){}
			device get_device()const{ID2D1Device* ptr;(*this)->GetDevice(&ptr);return device{std::move(ptr)};}
			template<typename D2D1_GRADIENT_STOP_ARRAY>
			expected<gradient_stop_collection, hresult_error> create_gradient_stop_collection(const D2D1_GRADIENT_STOP_ARRAY& gradient_stops, D2D1_COLOR_SPACE preinterpolation_space = D2D1_COLOR_SPACE_SRGB, D2D1_COLOR_SPACE postinterpolation_space = D2D1_COLOR_SPACE_SCRGB, D2D1_BUFFER_PRECISION buffer_precision = D2D1_BUFFER_PRECISION_8BPC_UNORM_SRGB, D2D1_EXTEND_MODE extend_mode = D2D1_EXTEND_MODE_CLAMP, D2D1_COLOR_INTERPOLATION_MODE color_interpolation_mode = D2D1_COLOR_INTERPOLATION_MODE_STRAIGHT){
				return detail::convert_to_rich_interface<gradient_stop_collection>(com_create_resource<ID2D1GradientStopCollection1>([&](ID2D1GradientStopCollection1** x){return (*this)->CreateGradientStopCollection(gradient_stops.data(), static_cast<UINT32>(gradient_stops.size()), preinterpolation_space, postinterpolation_space, buffer_precision, extend_mode, color_interpolation_mode, x);}), _T(__FUNCTION__));
			}
			expected<gradient_stop_collection, hresult_error> create_gradient_stop_collection(const gradient_stop_collection::property& prop){
				return create_gradient_stop_collection(prop.grads, prop.preerps, prop.posterps, prop.bufpre, prop.extm, prop.colerpm);
			}
			expected<bitmap_brush, hresult_error> create_bitmap_brush(const bitmap& bm, const D2D1_BITMAP_BRUSH_PROPERTIES1& extm = D2D1::BitmapBrushProperties1(), const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return detail::convert_to_rich_interface<bitmap_brush>(com_create_resource<ID2D1BitmapBrush1>([&](ID2D1BitmapBrush1** x){return (*this)->CreateBitmapBrush(bm.get(), extm, prop, x);}), _T(__FUNCTION__));
			}
			expected<effect, hresult_error> create_effect(const CLSID& clsid)const{
				return detail::convert_to_rich_interface<effect>(com_create_resource<ID2D1Effect>([&](ID2D1Effect** ptr){return (*this)->CreateEffect(clsid, ptr);}), _T(__FUNCTION__));
			}
			template<typename T>
			expected<T, hresult_error> create_builtin_effect()const{
				return com_create_resource<ID2D1Effect>([&](ID2D1Effect** ptr){return (*this)->CreateEffect(T::clsid(), ptr);})
					.emap([](HRESULT hr){return make_unexpected<hresult_error>(_T("will::d2d::device::context::create_builtin_effect"), hr);})
					.map([](ID2D1Effect* eff){return T{std::move(eff)};});
			}
			expected<builtin_effects::color::color_management, hresult_error> create_color_management_effect()const{
				return create_builtin_effect<builtin_effects::color::color_management>();
			}
			expected<builtin_effects::color::color_matrix, hresult_error> create_color_matrix_effect()const{
				return create_builtin_effect<builtin_effects::color::color_matrix>();
			}
			expected<builtin_effects::color::discrete_transfer, hresult_error> create_discrete_transfer_effect()const{
				return create_builtin_effect<builtin_effects::color::discrete_transfer>();
			}
			expected<builtin_effects::color::dpi_compensation, hresult_error> create_dpi_compensation_effect()const{
				return create_builtin_effect<builtin_effects::color::dpi_compensation>();
			}
			expected<builtin_effects::color::gamma_transfer, hresult_error> create_gamma_transfer_effect()const{
				return create_builtin_effect<builtin_effects::color::gamma_transfer>();
			}
			expected<builtin_effects::color::hue_rotation, hresult_error> create_hue_rotation_effect()const{
				return create_builtin_effect<builtin_effects::color::hue_rotation>();
			}
			expected<builtin_effects::color::linear_transfer, hresult_error> create_linear_transfer_effect()const{
				return create_builtin_effect<builtin_effects::color::linear_transfer>();
			}
			expected<builtin_effects::color::opacity_metadata, hresult_error> create_opacity_metadata_effect()const{
				return create_builtin_effect<builtin_effects::color::opacity_metadata>();
			}
			expected<builtin_effects::color::premultiply, hresult_error> create_premultiply_effect()const{
				return create_builtin_effect<builtin_effects::color::premultiply>();
			}
			expected<builtin_effects::color::saturation, hresult_error> create_saturation_effect()const{
				return create_builtin_effect<builtin_effects::color::saturation>();
			}
			expected<builtin_effects::color::table_transfer, hresult_error> create_table_transfer_effect()const{
				return create_builtin_effect<builtin_effects::color::table_transfer>();
			}
			expected<builtin_effects::color::unpremultiply, hresult_error> create_unpremultiply_effect()const{
				return create_builtin_effect<builtin_effects::color::unpremultiply>();
			}
			expected<builtin_effects::composition::arithmetic_composite, hresult_error> create_arithmetic_composite_effect()const{
				return create_builtin_effect<builtin_effects::composition::arithmetic_composite>();
			}
			expected<builtin_effects::composition::blend, hresult_error> create_blend_effect()const{
				return create_builtin_effect<builtin_effects::composition::blend>();
			}
			expected<builtin_effects::composition::composite, hresult_error> create_composite_effect()const{
				return create_builtin_effect<builtin_effects::composition::composite>();
			}
			expected<builtin_effects::filter::convolve_matrix, hresult_error> create_convolve_matrix_effect()const{
				return create_builtin_effect<builtin_effects::filter::convolve_matrix>();
			}
			expected<builtin_effects::filter::directional_blur, hresult_error> create_directional_blur_effect()const{
				return create_builtin_effect<builtin_effects::filter::directional_blur>();
			}
			expected<builtin_effects::filter::gaussian_blur, hresult_error> create_gaussian_blur_effect()const{
				return create_builtin_effect<builtin_effects::filter::gaussian_blur>();
			}
			expected<builtin_effects::filter::morphology, hresult_error> create_morphology_effect()const{
				return create_builtin_effect<builtin_effects::filter::morphology>();
			}
			expected<builtin_effects::lighting_and_stylizing::displacement_map, hresult_error> create_displacement_map_effect()const{
				return create_builtin_effect<builtin_effects::lighting_and_stylizing::displacement_map>();
			}
			expected<builtin_effects::lighting_and_stylizing::distant_diffuse, hresult_error> create_distant_diffuse_effect()const{
				return create_builtin_effect<builtin_effects::lighting_and_stylizing::distant_diffuse>();
			}
			expected<builtin_effects::lighting_and_stylizing::point_diffuse, hresult_error> create_point_diffuse_effect()const{
				return create_builtin_effect<builtin_effects::lighting_and_stylizing::point_diffuse>();
			}
			expected<builtin_effects::lighting_and_stylizing::point_specular, hresult_error> create_point_specular_effect()const{
				return create_builtin_effect<builtin_effects::lighting_and_stylizing::point_specular>();
			}
			expected<builtin_effects::lighting_and_stylizing::shadow, hresult_error> create_shadow_effect()const{
				return create_builtin_effect<builtin_effects::lighting_and_stylizing::shadow>();
			}
			expected<builtin_effects::lighting_and_stylizing::spot_diffuse, hresult_error> create_spot_diffuse_effect()const{
				return create_builtin_effect<builtin_effects::lighting_and_stylizing::spot_diffuse>();
			}
			expected<builtin_effects::lighting_and_stylizing::spot_specular, hresult_error> create_spot_specular_effect()const{
				return create_builtin_effect<builtin_effects::lighting_and_stylizing::spot_specular>();
			}
			expected<builtin_effects::photo::brightness, hresult_error> create_brightness_effect()const{
				return create_builtin_effect<builtin_effects::photo::brightness>();
			}
			expected<builtin_effects::photo::histogram, hresult_error> create_histogram_effect()const{
				return create_builtin_effect<builtin_effects::photo::histogram>();
			}
			expected<builtin_effects::source::bitmap_source, hresult_error> create_bitmap_source_effect()const{
				return create_builtin_effect<builtin_effects::source::bitmap_source>();
			}
			expected<builtin_effects::source::flood, hresult_error> create_flood_effect()const{
				return create_builtin_effect<builtin_effects::source::flood>();
			}
			expected<builtin_effects::source::turbulence, hresult_error> create_turbulence_effect()const{
				return create_builtin_effect<builtin_effects::source::turbulence>();
			}
			expected<builtin_effects::transform::_2d_affine_transform, hresult_error> create_2d_affine_transform_effect()const{
				return create_builtin_effect<builtin_effects::transform::_2d_affine_transform>();
			}
			expected<builtin_effects::transform::_3d_transform, hresult_error> create_3d_transform_effect()const{
				return create_builtin_effect<builtin_effects::transform::_3d_transform>();
			}
			expected<builtin_effects::transform::_3d_perspective_transform, hresult_error> create_3d_perspective_transform_effect()const{
				return create_builtin_effect<builtin_effects::transform::_3d_perspective_transform>();
			}
			expected<builtin_effects::transform::border, hresult_error> create_border_effect()const{
				return create_builtin_effect<builtin_effects::transform::border>();
			}
			expected<builtin_effects::transform::crop, hresult_error> create_crop_effect()const{
				return create_builtin_effect<builtin_effects::transform::crop>();
			}
			expected<builtin_effects::transform::scale, hresult_error> create_scale_effect()const{
				return create_builtin_effect<builtin_effects::transform::scale>();
			}
			expected<builtin_effects::transform::tile, hresult_error> create_tile_effect()const{
				return create_builtin_effect<builtin_effects::transform::tile>();
			}
			expected<builtin_effects::transparency::luminance_to_alpha, hresult_error> create_luminance_to_alpha_effect()const{
				return create_builtin_effect<builtin_effects::transparency::luminance_to_alpha>();
			}
			template<typename RandomAccessableContainer>
			expected<stroke_style, hresult_error> create_stroke_style(const D2D1_STROKE_STYLE_PROPERTIES1& prop, RandomAccessableContainer&& dashes)const{
				return get_factory().create_stroke_style(prop, std::forward<RandomAccessableContainer>(dashes));
			}
			expected<stroke_style, hresult_error> create_stroke_style(const D2D1_STROKE_STYLE_PROPERTIES1& prop)const{
				return get_factory().create_stroke_style(prop);
			}
			expected<bitmap, hresult_error> create_bitmap(const D2D1_SIZE_U& size, const D2D1_BITMAP_PROPERTIES1& prop = D2D1::BitmapProperties1())const{
				return detail::convert_to_rich_interface<bitmap>(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return (*this)->CreateBitmap(size, nullptr, 0u, prop, x);}), _T(__FUNCTION__));
			}
			expected<bitmap, hresult_error> create_bitmap(const D2D1_SIZE_U& size, const bitmap::memory_data& data, const D2D1_BITMAP_PROPERTIES1& prop = D2D1::BitmapProperties1())const{
				return detail::convert_to_rich_interface<bitmap>(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return (*this)->CreateBitmap(size, data.data, data.pitch, prop, x);}), _T(__FUNCTION__));
			}
			expected<bitmap, hresult_error> create_bitmap(IWICBitmapSource* conv)const{
				return detail::convert_to_rich_interface<bitmap>(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return (*this)->CreateBitmapFromWicBitmap(conv, nullptr, x);}), _T(__FUNCTION__));
			}
			expected<bitmap, hresult_error> create_bitmap(IWICBitmapSource* conv, const D2D1_BITMAP_PROPERTIES1& prop)const{
				return detail::convert_to_rich_interface<bitmap>(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return (*this)->CreateBitmapFromWicBitmap(conv, prop, x);}), _T(__FUNCTION__));
			}
			expected<bitmap, hresult_error> create_bitmap(IDXGISurface2* surf)const{
				return detail::convert_to_rich_interface<bitmap>(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return (*this)->CreateBitmapFromDxgiSurface(surf, nullptr, x);}), _T(__FUNCTION__));
			}
			expected<bitmap, hresult_error> create_bitmap(IDXGISurface2* surf, const D2D1_BITMAP_PROPERTIES1& prop)const{
				return detail::convert_to_rich_interface<bitmap>(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return (*this)->CreateBitmapFromDxgiSurface(surf, prop, x);}), _T(__FUNCTION__));
			}
			template<typename Source, std::enable_if_t<!std::is_base_of<IWICBitmapSource, std::remove_pointer_t<std::decay_t<Source>>>::value>* = nullptr>
			expected<bitmap, hresult_error> create_bitmap(Source&& src)const{
				return create_bitmap(std::forward<Source>(src).get());
			}
			template<typename Source, std::enable_if_t<!std::is_same_v<std::decay_t<Source>, D2D1_SIZE_U>>* = nullptr>
			expected<bitmap, hresult_error> create_bitmap(Source&& src, const D2D1_BITMAP_PROPERTIES1& prop)const{
				return create_bitmap(std::forward<Source>(src).get(), prop);
			}
			expected<bitmap_render_target, hresult_error> create_compatible_render_target(const D2D1_SIZE_F& desired_size, const D2D1_SIZE_U& desired_pixel_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE)const{
				return detail::convert_to_rich_interface<bitmap_render_target>(com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return (*this)->CreateCompatibleRenderTarget(desired_size, desired_pixel_size, desired_format, option, ptr);}), _T(__FUNCTION__));
			}
			expected<bitmap_render_target, hresult_error> create_compatible_render_target(const D2D1_SIZE_F& desired_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE)const{
				return detail::convert_to_rich_interface<bitmap_render_target>(com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return (*this)->CreateCompatibleRenderTarget(&desired_size, nullptr, &desired_format, option, ptr);}), _T(__FUNCTION__));
			}
			expected<bitmap_render_target, hresult_error> create_compatible_render_target(const D2D1_SIZE_U& desired_pixel_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE)const{
				return detail::convert_to_rich_interface<bitmap_render_target>(com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return (*this)->CreateCompatibleRenderTarget(nullptr, &desired_pixel_size, &desired_format, option, ptr);}), _T(__FUNCTION__));
			}
			expected<bitmap_render_target, hresult_error> create_compatible_render_target(const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE)const{
				return detail::convert_to_rich_interface<bitmap_render_target>(com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return (*this)->CreateCompatibleRenderTarget(nullptr, nullptr, &desired_format, option, ptr);}), _T(__FUNCTION__));
			}
			template<typename F>
			expected<bitmap, hresult_error> prerender(const D2D1_SIZE_F& desired_size, F&& f)const{
				return create_compatible_render_target(desired_size).bind([&](bitmap_render_target&& rt){
					return rt.draw(std::forward<F>(f)).bind([&]{
						return rt.get_bitmap();
					});
				});
			}
			template<typename F>
			expected<bitmap, hresult_error> prerender(const D2D1_SIZE_U& desired_pixel_size, F&& f)const{
				return create_compatible_render_target(desired_pixel_size).bind([&](bitmap_render_target&& rt){
					return rt.draw(std::forward<F>(f)).bind([&]{
						return rt.get_bitmap();
					});
				});
			}
			template<typename F>
			expected<bitmap, hresult_error> prerender(F&& f)const{
				return create_compatible_render_target().bind([&](bitmap_render_target&& rt){
					return rt.draw(std::forward<F>(f)).bind([&]{
						return rt.get_bitmap();
					});
				});
			}
			template<typename F>
			expected<void, hresult_error> draw(F&& f){
				(*this)->BeginDraw();
				f(get_renderer());
				std::pair<D2D1_TAG, D2D1_TAG> tag;
				const auto hr = (*this)->EndDraw(&tag.first, &tag.second);
				if(SUCCEEDED(hr))
					return {};
				set_tags(tag);
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			}
			friend will::d2d::device;
		};
		expected<context, hresult_error> create_context(D2D1_DEVICE_CONTEXT_OPTIONS option = D2D1_DEVICE_CONTEXT_OPTIONS_NONE)const{return detail::convert_to_rich_interface<context>(com_create_resource<ID2D1DeviceContext>([&](ID2D1DeviceContext** ptr){return (*this)->CreateDeviceContext(option, ptr);}), _T(__FUNCTION__));}
	};
	using solid_color_brush =  device::solid_color_brush;
	using gradient_stop_collection =  device::gradient_stop_collection;
	using linear_gradient_brush =  device::linear_gradient_brush;
	using radial_gradient_brush =  device::radial_gradient_brush;
	using bitmap_brush =  device::bitmap_brush;
	using bitmap =  device::bitmap;
	using effect =  device::effect;
	using builtin_effects = device::builtin_effects;
	using bitmap_render_target =  device::bitmap_render_target;
	template<typename RandomAccessableContainer>
	expected<stroke_style, hresult_error> create_stroke_style(const D2D1_STROKE_STYLE_PROPERTIES1& prop, RandomAccessableContainer&& dashes)const{
		return detail::convert_to_rich_interface<stroke_style>(com_create_resource<ID2D1StrokeStyle1>([&](ID2D1StrokeStyle1** ptr){return (*this)->CreateStrokeStyle(prop, dashes.data(), static_cast<UINT32>(dashes.size()), ptr);}), _T(__FUNCTION__));
	}
	expected<stroke_style, hresult_error> create_stroke_style(const D2D1_STROKE_STYLE_PROPERTIES1& prop, std::initializer_list<FLOAT> dashes)const{
		return detail::convert_to_rich_interface<stroke_style>(com_create_resource<ID2D1StrokeStyle1>([&](ID2D1StrokeStyle1** ptr){return (*this)->CreateStrokeStyle(prop, dashes.begin(), static_cast<UINT32>(dashes.size()), ptr);}), _T(__FUNCTION__));
	}
	expected<stroke_style, hresult_error> create_stroke_style(const D2D1_STROKE_STYLE_PROPERTIES1& prop)const{
		return detail::convert_to_rich_interface<stroke_style>(com_create_resource<ID2D1StrokeStyle1>([&](ID2D1StrokeStyle1** ptr){return (*this)->CreateStrokeStyle(prop, nullptr, 0, ptr);}), _T(__FUNCTION__));
	}
	expected<drawing_state_block, hresult_error> create_drawing_state_block(const D2D1_DRAWING_STATE_DESCRIPTION& d, IDWriteRenderingParams1* p)const{return detail::convert_to_rich_interface<drawing_state_block>(com_create_resource<ID2D1DrawingStateBlock>([&](ID2D1DrawingStateBlock** ptr){return (*this)->CreateDrawingStateBlock(&d, p, ptr);}), _T(__FUNCTION__));}
	template<typename DWriteRenderingParams>
	expected<drawing_state_block, hresult_error> create_drawing_state_block(const D2D1_DRAWING_STATE_DESCRIPTION& d, DWriteRenderingParams&& p)const{return create_drawing_state_block(d, std::forward<DWriteRenderingParams>(p).get());}
	expected<drawing_state_block, hresult_error> create_drawing_state_block(IDWriteRenderingParams1* p)const{return detail::convert_to_rich_interface<drawing_state_block>(com_create_resource<ID2D1DrawingStateBlock>([&](ID2D1DrawingStateBlock** ptr){return (*this)->CreateDrawingStateBlock(nullptr, p, ptr);}), _T(__FUNCTION__));}
	template<typename DWriteRenderingParams>
	expected<drawing_state_block, hresult_error> create_drawing_state_block(DWriteRenderingParams&& p)const{return create_drawing_state_block(std::forward<DWriteRenderingParams>(p).get());}
	expected<drawing_state_block, hresult_error> create_drawing_state_block(const D2D1_DRAWING_STATE_DESCRIPTION& d)const{return detail::convert_to_rich_interface<drawing_state_block>(com_create_resource<ID2D1DrawingStateBlock>([&](ID2D1DrawingStateBlock** ptr){return (*this)->CreateDrawingStateBlock(d, ptr);}), _T(__FUNCTION__));}
	expected<drawing_state_block, hresult_error> create_drawing_state_block()const{return detail::convert_to_rich_interface<drawing_state_block>(com_create_resource<ID2D1DrawingStateBlock>([&](ID2D1DrawingStateBlock** ptr){return (*this)->CreateDrawingStateBlock(ptr);}), _T(__FUNCTION__));}
	expected<device, hresult_error> create_device(IDXGIDevice2* dxgi_device)const{return detail::convert_to_rich_interface<device>(com_create_resource<ID2D1Device>([&](ID2D1Device** ptr){return (*this)->CreateDevice(dxgi_device, ptr);}), _T(__FUNCTION__));}
	template<typename DXGIDevice>
	expected<device, hresult_error> create_device(DXGIDevice&& dev)const{return create_device(std::forward<DXGIDevice>(dev).get());}
	static expected<d2d, hresult_error> create_factory(D2D1_FACTORY_TYPE t = D2D1_FACTORY_TYPE_MULTI_THREADED){
		return detail::convert_to_rich_interface<d2d>(com_create_resource<ID2D1Factory1>([&](ID2D1Factory1** ptr){return D2D1CreateFactory(t, ptr);}), _T(__FUNCTION__));
	}
};
namespace two_dim{
namespace detail{
template<>struct attribute<::D2D1_RECT_F, xyxy<FLOAT>>{static ::D2D1_RECT_F impl(const xyxy<FLOAT>& p){return {p._1.x, p._1.y, p._2.x, p._2.y};}};
template<typename T>struct attribute<std::enable_if_t<!std::is_same<T, FLOAT>::value, ::D2D1_RECT_F>, xyxy<T>>{static ::D2D1_RECT_F impl(const xyxy<T>& p){return p.cast<FLOAT>().attribute<::D2D1_RECT_F>();}};
template<typename T>struct attribute<::D2D1_RECT_F, xywh<T>>{static ::D2D1_RECT_F impl(const xywh<T>& p){return static_cast<xyxy<T>>(p).cast<FLOAT>().attribute<::D2D1_RECT_F>();}};
template<>struct attribute<::D2D1_RECT_L, xyxy<LONG>>{static ::D2D1_RECT_L impl(const xyxy<LONG>& p){return {p._1.x, p._1.y, p._2.x, p._2.y};}};
template<typename T>struct attribute<std::enable_if_t<!std::is_same<T, LONG>::value, ::D2D1_RECT_L>, xyxy<T>>{static ::D2D1_RECT_L impl(const xyxy<T>& p){return p.cast<LONG>().attribute<::D2D1_RECT_L>();}};
template<typename T>struct attribute<::D2D1_RECT_L, xywh<T>> { static ::D2D1_RECT_L impl(const xywh<T>& p) { return static_cast<xyxy<T>>(p).cast<LONG>().attribute<::D2D1_RECT_L>(); } };
template<>struct attribute<::D2D1_RECT_U, xyxy<UINT32>>{static ::D2D1_RECT_U impl(const xyxy<UINT32>& p){return {p._1.x, p._1.y, p._2.x, p._2.y};}};
template<typename T>struct attribute<std::enable_if_t<!std::is_same<T, UINT32>::value, ::D2D1_RECT_U>, xyxy<T>>{static ::D2D1_RECT_U impl(const xyxy<T>& p){return p.cast<UINT32>().attribute<::D2D1_RECT_U>();}};
template<typename T>struct attribute<::D2D1_RECT_U, xywh<T>>{static ::D2D1_RECT_U impl(const xywh<T>& p){return static_cast<xyxy<T>>(p).cast<UINT32>().attribute<::D2D1_RECT_U>();}};
template<>struct attribute<::D2D1_POINT_2F, xy<FLOAT>>{static ::D2D1_POINT_2F impl(const xy<FLOAT>& p){return {p.x, p.y};}};
template<typename T>struct attribute<std::enable_if_t<!std::is_same<T, FLOAT>::value, ::D2D1_POINT_2F>, xy<T>>{static ::D2D1_POINT_2F impl(const xy<T>& p){return p.cast<FLOAT>().attribute<::D2D1_POINT_2F>();}};
template<>struct attribute<::D2D1_POINT_2L, xy<LONG>>{static ::D2D1_POINT_2L impl(const xy<LONG>& p){return {p.x, p.y};}};
template<typename T>struct attribute<std::enable_if_t<!std::is_same<T, LONG>::value, ::D2D1_POINT_2L>, xy<T>>{static ::D2D1_POINT_2L impl(const xy<T>& p){return p.cast<LONG>().attribute<::D2D1_POINT_2L>();}};
template<>struct attribute<::D2D1_POINT_2U, xy<UINT32>>{static ::D2D1_POINT_2U impl(const xy<UINT32>& p){return {p.x, p.y};}};
template<typename T>struct attribute<std::enable_if_t<!std::is_same<T, UINT32>::value, ::D2D1_POINT_2U>, xy<T>>{static ::D2D1_POINT_2U impl(const xy<T>& p){return p.cast<UINT32>().attribute<::D2D1_POINT_2U>();}};
template<>struct attribute<::D2D1_SIZE_F, wh<FLOAT>>{static ::D2D1_SIZE_F impl(const wh<FLOAT>& p){return {p.w, p.h};}};
template<typename T>struct attribute<std::enable_if_t<!std::is_same<T, FLOAT>::value, ::D2D1_SIZE_F>, wh<T>>{static ::D2D1_SIZE_F impl(const wh<T>& p){return p.cast<FLOAT>().attribute<::D2D1_SIZE_F>();}};
template<>struct attribute<::D2D1_SIZE_U, wh<UINT32>>{static ::D2D1_SIZE_U impl(const wh<UINT32>& p){return {p.w, p.h};}};
template<typename T>struct attribute<std::enable_if_t<!std::is_same<T, UINT32>::value, ::D2D1_SIZE_U>, wh<T>>{static ::D2D1_SIZE_U impl(const wh<T>& p){return p.cast<UINT32>().attribute<::D2D1_SIZE_U>();}};

template<>struct attribute<xyxy<FLOAT>, ::D2D1_RECT_F>{static xyxy<FLOAT> impl(const ::D2D1_RECT_F& p){return {{p.left, p.top}, {p.right, p.bottom}};}};
template<typename T>struct attribute<xyxy<T>, std::enable_if_t<!std::is_same<T, FLOAT>::value, ::D2D1_RECT_F>>{static xyxy<T> impl(const ::D2D1_RECT_F& p){return two_dim::attribute<xyxy<FLOAT>>(p).cast<T>();}};
template<typename T>struct attribute<xywh<T>, ::D2D1_RECT_F>{static xywh<T> impl(const ::D2D1_RECT_F& p){return static_cast<xywh<T>>(two_dim::attribute<xyxy<FLOAT>>(p).cast<T>());}};
//template<>struct attribute<xyxy<LONG>, ::D2D1_RECT_L>{static xyxy<LONG> impl(const ::D2D1_RECT_L& p){return {{p.left, p.top}, {p.right, p.bottom}};}};
template<typename T>struct attribute<xyxy<T>, std::enable_if_t<!std::is_same<T, LONG>::value, ::D2D1_RECT_L>>{static xyxy<T> impl(const ::D2D1_RECT_L& p){return two_dim::attribute<xyxy<LONG>>(p).cast<T>();}};
template<typename T>struct attribute<xywh<T>, ::D2D1_RECT_L>{static xywh<T> impl(const ::D2D1_RECT_L& p){return static_cast<xywh<T>>(two_dim::attribute<xyxy<LONG>>(p).cast<T>());}};
template<>struct attribute<xyxy<UINT32>, ::D2D1_RECT_U>{static xyxy<UINT32> impl(const ::D2D1_RECT_U& p){return {{p.left, p.top}, {p.right, p.bottom}};}};
template<typename T>struct attribute<xyxy<T>, std::enable_if_t<!std::is_same<T, UINT32>::value, ::D2D1_RECT_U>>{static xyxy<T> impl(const ::D2D1_RECT_U& p){return two_dim::attribute<xyxy<UINT32>>(p).cast<T>();}};
template<typename T>struct attribute<xywh<T>, ::D2D1_RECT_U>{static xywh<T> impl(const ::D2D1_RECT_U& p){return static_cast<xywh<T>>(two_dim::attribute<xyxy<UINT32>>(p).cast<T>());}};
template<>struct attribute<xy<FLOAT>, ::D2D1_POINT_2F>{static xy<FLOAT> impl(const ::D2D1_POINT_2F& p){return {p.x, p.y};}};
template<typename T>struct attribute<xy<T>, std::enable_if_t<!std::is_same<T, FLOAT>::value, ::D2D1_POINT_2F>>{static xy<T> impl(const ::D2D1_POINT_2F& p){return two_dim::attribute<xy<FLOAT>>(p).cast<T>();}};
template<>struct attribute<xy<LONG>, ::D2D1_POINT_2L>{static xy<LONG> impl(const ::D2D1_POINT_2L& p){return {p.x, p.y};}};
template<typename T>struct attribute<xy<T>, std::enable_if_t<!std::is_same<T, LONG>::value, ::D2D1_POINT_2L>>{static xy<LONG> impl(const ::D2D1_POINT_2L& p){return two_dim::attribute<xy<LONG>>(p).cast<T>();}};
template<>struct attribute<xy<UINT32>, ::D2D1_POINT_2U>{static xy<UINT32> impl(const ::D2D1_POINT_2U& p){return {p.x, p.y};}};
template<typename T>struct attribute<xy<T>, std::enable_if_t<!std::is_same<T, UINT32>::value, ::D2D1_POINT_2U>>{static xy<T> impl(const ::D2D1_POINT_2U& p){return two_dim::attribute<xy<UINT32>>(p).cast<T>();}};
template<>struct attribute<wh<FLOAT>, ::D2D1_SIZE_F>{static wh<FLOAT> impl(const ::D2D1_SIZE_F& p){return {p.width, p.height};}};
template<typename T>struct attribute<wh<T>, std::enable_if_t<!std::is_same<T, FLOAT>::value, ::D2D1_SIZE_F>>{static wh<T> impl(const ::D2D1_SIZE_F& p){return two_dim::attribute<wh<FLOAT>>(p).cast<T>();}};
template<>struct attribute<wh<UINT32>, ::D2D1_SIZE_U>{static wh<UINT32> impl(const ::D2D1_SIZE_U& p){return {p.width, p.height};}};
template<typename T>struct attribute<wh<T>, std::enable_if_t<!std::is_same<T, UINT32>::value, ::D2D1_SIZE_U>>{static wh<T> impl(const ::D2D1_SIZE_U& p){return two_dim::attribute<wh<UINT32>>(p).cast<T>();}};
}
}
}