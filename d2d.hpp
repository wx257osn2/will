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
				com_ptr<ID2D1DeviceContext> c;
			public:
				explicit renderer(com_ptr<ID2D1DeviceContext>&& r):c(std::move(r)){}
				renderer(const renderer&) = delete;
				renderer(renderer&&) = default;
				renderer& operator=(const renderer&) = delete;
				renderer& operator=(renderer&&) = default;
				~renderer() = default;
				ID2D1DeviceContext* get()const{return c;}
				const com_ptr<ID2D1DeviceContext>& operator->()const{return c;}
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
			expected<renderer, hresult_error> get_renderer()const{return query_interface<ID2D1DeviceContext>(get()).emap([](HRESULT hr){return make_unexpected<hresult_error>(_T("will::d2d::device::bitmap_render_target::get_renderer"), hr);}).map([](com_ptr<ID2D1DeviceContext>&& ptr){return renderer{std::move(ptr)};});}
			template<typename F>
			expected<void, hresult_error> draw(F&& f){
				return get_renderer().bind([&](renderer&& x)->expected<void, hresult_error>{
					(*this)->BeginDraw();
					f(x);
					std::pair<D2D1_TAG, D2D1_TAG> tag;
					const auto hr = (*this)->EndDraw(&tag.first, &tag.second);
					if(SUCCEEDED(hr))
						return {};
					set_tags(tag);
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
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
			renderer get_renderer(){(*this)->AddRef();return renderer{get()};}
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