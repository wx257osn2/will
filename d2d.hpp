#pragma once
#include"com.hpp"
#include"_2dim.hpp"
#include"_resouce_property.hpp"
#include<dxgi1_2.h>
#include<wincodec.h>
#include<d2d1_1.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxguid.lib")
#include<functional>
#include<utility>

namespace will{

namespace detail{

struct resource_unholder{
	constexpr resource_unholder() = default;
	template<typename T>
	using factory_type = detail::resource<T>;
	template<typename T>
	using resource_type = detail::resource<T>;
	template<typename T, typename F>
	T create_factory(F&& f){return T{f()};}
	template<typename T, typename F>
	T create_resource(F&& f){return T{f()};}
	struct factory_create_tag{};
};

}

template<typename ResourceHolder>
class basic_d2d : public detail::resource<ID2D1Factory1>{
	template<typename T>
	using Resource = typename ResourceHolder::template resource_type<T>;
	using resource = detail::resource<ID2D1Factory1>;
	using resource::resource;
public:
	explicit basic_d2d(D2D1_FACTORY_TYPE t = D2D1_FACTORY_TYPE_MULTI_THREADED):resource(com_create_resource<ID2D1Factory1>(std::bind(static_cast<HRESULT(*)(D2D1_FACTORY_TYPE,ID2D1Factory1**)>(D2D1CreateFactory), t, std::placeholders::_1))){}
	class stroke_style : public detail::resource<ID2D1StrokeStyle1>{
		using resource = detail::resource<ID2D1StrokeStyle1>;
	public:
		using resource::resource;
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
	};
	class device : public Resource<ID2D1Device>{
		using resource = Resource<ID2D1Device>;
	public:
		using resource::resource;
		UINT64 max_texture_memory()const{return (*this)->GetMaximumTextureMemory();}
		void max_texture_memory(UINT64 bytes)const{(*this)->SetMaximumTextureMemory(bytes);}
		basic_d2d get_factory()const{return basic_d2d{com_ptr<ID2D1Factory>{com_create_resource<ID2D1Factory>([&](ID2D1Factory** ptr){return (*this)->GetFactory(ptr), S_OK;})}.as<ID2D1Factory1>()};}
		struct context : ResourceHolder, detail::resource<ID2D1DeviceContext>{
			class solid_color_brush : public Resource<ID2D1SolidColorBrush>{
				class _color{
					solid_color_brush& brush;
				public:
					_color(solid_color_brush& b):brush(b){}
					_color(const _color&) = delete;
					_color(_color&&){}
					_color& operator=(const _color&) = delete;
					_color& operator=(_color&&){}
					~_color() = default;
					operator D2D1::ColorF()const{return brush->GetColor();}
					_color& operator=(const D2D1::ColorF& c){b->SetColor(c);}
				};
				using resource = Resource<ID2D1SolidColorBrush>;
			public:
				using resource::resource;
				_color color = *this;
			};
			class gradient_stop_collection : public Resource<ID2D1GradientStopCollection1>{
				using resource = Resource<ID2D1GradientStopCollection1>;
			public:
				using resource::resource;
				class property{
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
			#undef  PROPERTYDECL
					friend context;
				};
			};
			using linear_gradient_brush = Resource<ID2D1LinearGradientBrush>;
			using radial_gradient_brush = Resource<ID2D1RadialGradientBrush>;
			using bitmap_brush = Resource<ID2D1BitmapBrush1>;
			class bitmap : public Resource<ID2D1Bitmap1>{
				using resource = Resource<ID2D1Bitmap1>;
			public:
				using resource::resource;
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
					using detail::property<D2D1_BITMAP_PROPERTIES1>::property;
			#define PROPERTYDECL(name, type, membername) property& name(type t){prop.membername = t;return *this;}
					PROPERTYDECL(format, DXGI_FORMAT, pixelFormat.format)
					PROPERTYDECL(alpha_mode, D2D1_ALPHA_MODE, pixelFormat.alphaMode)
					const dpi_property dpi = this;
					PROPERTYDECL(option, D2D1_BITMAP_OPTIONS, bitmapOptions)
					PROPERTYDECL(color_context, ID2D1ColorContext*, colorContext)
			#undef  PROPERTYDECL
				};
			};
			class effect : public Resource<ID2D1Effect>{
				class impl{
					UINT32 index;
					effect& eff;
				public:
					impl(UINT32 i, effect& eff):index(i), eff(eff){}
					effect& operator|=(ID2D1Bitmap1* input){eff->SetInput(index, input); return eff;}
					effect& operator|=(ID2D1Effect* input){eff->SetInputEffect(index, input); return eff;}
					template<typename Input>
					effect& operator|=(Input&& input){return *this |= std::forward<Input>(input).get();}
				};
				using resource = Resource<ID2D1Effect>;
			public:
				using resource::resource;
				effect& operator|=(ID2D1Bitmap1* input){(*this)->SetInput(0, input); return *this;}
				effect& operator|=(ID2D1Effect* input){(*this)->SetInputEffect(0, input); return *this;}
				template<typename Input>
				effect& operator|=(Input&& input){return *this |= std::forward<Input>(input).get();}
				impl operator[](UINT32 i){return impl{i, *this};}
			};
			class bitmap_render_target : public Resource<ID2D1BitmapRenderTarget>{
				using resource = Resource<ID2D1BitmapRenderTarget>;
			public:
				using resource::resource;
				detail::resource<ID2D1Bitmap1> get_bitmap()const{
					return std::move(detail::resource<ID2D1Bitmap1>{com_ptr<ID2D1Bitmap>{com_create_resource<ID2D1Bitmap>([&](ID2D1Bitmap** ptr){return (*this)->GetBitmap(ptr);})}.as<ID2D1Bitmap1>()});
				}
				template<typename F, typename G>
				HRESULT draw(F&& f, G&& g){
					rt->BeginDraw();
					f();
					const auto ret = rt->EndDraw();
					g();
					return ret;
				}
				template<typename F>
				HRESULT draw(F&& f){
					return draw(std::forward<F>(f), []{});
				}
			};
		protected:
			using resource::resource;
		public:
			explicit context(IDXGISurface2* surface):ResourceHolder(), resource(com_create_resource<ID2D1DeviceContext>([&](ID2D1DeviceContext** ptr){return D2D1CreateDeviceContext(surface, nullptr, ptr);})){}
			context(IDXGISurface2* surface, const D2D1_CREATION_PROPERTIES& prop):ResourceHolder(), resource(com_create_resource<ID2D1DeviceContext>([&](ID2D1DeviceContext** ptr){return D2D1CreateDeviceContext(surface, prop, ptr);})){}
			template<typename Surface>
			explicit context(Surface&& surf):context(std::forward<Surface>(surf).get()){}
			template<typename Surface>
			explicit context(Surface&& surf, const D2D1_CREATION_PROPERTIES& prop):context(std::forward<Surface>(surf).get(), prop){}
			struct property : detail::property<D2D1_CREATION_PROPERTIES>{
				property(D2D1_THREADING_MODE th):detail::property<D2D1_CREATION_PROPERTIES>(){prop.threadingMode = th;}
		#define PROPERTYDECL(name, type, membername) property& name(type t){prop.membername = t;return *this;}
				PROPERTYDECL(threading_mode, D2D1_THREADING_MODE, threadingMode)
				PROPERTYDECL(debug_level, D2D1_DEBUG_LEVEL, debugLevel)
				PROPERTYDECL(option, D2D1_DEVICE_CONTEXT_OPTIONS, options)
		#undef  PROPERTYDECL
			};
			device get_device()const{return create_resource<device>([&](){return com_create_resource<ID2D1Device>([&](ID2D1Device** ptr){return (*this)->GetDevice(ptr), S_OK;});});}
			solid_color_brush create_solid_color_brush(const D2D1_COLOR_F& col, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return create_resource<solid_color_brush>([&, col, prop]{return com_create_resource<ID2D1SolidColorBrush>([&](ID2D1SolidColorBrush** x){return (*this)->CreateSolidColorBrush(col, prop, x);});});
			}
			template<typename D2D1_GRADIENT_STOP_ARRAY>
			gradient_stop_collection create_gradient_stop_collection(const D2D1_GRADIENT_STOP_ARRAY& gradient_stops, D2D1_COLOR_SPACE preinterpolation_space = D2D1_COLOR_SPACE_SRGB, D2D1_COLOR_SPACE postinterpolation_space = D2D1_COLOR_SPACE_SCRGB, D2D1_BUFFER_PRECISION buffer_precision = D2D1_BUFFER_PRECISION_8BPC_UNORM_SRGB, D2D1_EXTEND_MODE extend_mode = D2D1_EXTEND_MODE_CLAMP, D2D1_COLOR_INTERPOLATION_MODE color_interpolation_mode = D2D1_COLOR_INTERPOLATION_MODE_STRAIGHT){
				return create_resource<gradient_stop_collection>([&, gradient_stops, preinterpolation_space, postinterpolation_space, buffer_precision, extend_mode, color_interpolation_mode](){return com_create_resource<ID2D1GradientStopCollection1>([&](ID2D1GradientStopCollection1** x){return (*this)->CreateGradientStopCollection(gradient_stops.data(), gradient_stops.size(), preinterpolation_space, postinterpolation_space, buffer_precision, extend_mode, color_interpolation_mode, x);});});
			}
			template<typename D2D1_GRADIENT_STOP_ARRAY>
			gradient_stop_collection create_gradient_stop_collection(const D2D1_GRADIENT_STOP_ARRAY& gradient_stops, const typename gradient_stop_collection::property& prop){
				return std::move(create_gradient_stop_collection(gradient_stops, prop.preerps, prop.posterps, prop.bufpre, prop.extm, prop.colerpm));
			}
			linear_gradient_brush create_linear_gradient_brush(const gradient_stop_collection& stops, const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES& pos, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return create_resource<linear_gradient_brush>([&, pos, prop](){return com_create_resource<ID2D1LinearGradientBrush>([&](ID2D1LinearGradientBrush** x){return (*this)->CreateLinearGradientBrush(pos, prop, stops.get(), x);});});
			}
			radial_gradient_brush create_radial_gradient_brush(const gradient_stop_collection& stops, const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES& pos, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return create_resource<radial_gradient_brush>([&, pos, prop](){return com_create_resource<ID2D1RadialGradientBrush>([&](ID2D1RadialGradientBrush** x){return (*this)->CreateRadialGradientBrush(pos, prop, stops.get(), x);});});
			}
			bitmap_brush create_bitmap_brush(const bitmap& bm, const D2D1_BITMAP_BRUSH_PROPERTIES1& extm = D2D1::BitmapBrushProperties1(), const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return create_resource<bitmap_brush>([&, extm, prop](){return com_create_resource<ID2D1BitmapBrush1>([&](ID2D1BitmapBrush1** x){return (*this)->CreateBitmapBrush(bm.get(), extm, prop, x);});});
			}
			effect create_effect(const CLSID& clsid){
				return create_resource<effect>([&, clsid](){return com_create_resource<ID2D1Effect>([&](ID2D1Effect** ptr){return (*this)->CreateEffect(clsid, ptr);});});
			}
			template<typename RandomAccessableContainer>
			stroke_style create_stroke_style(const D2D1_STROKE_STYLE_PROPERTIES1& prop, RandomAccessableContainer&& dashes)const{
				return get_device().get_factory().create_stroke_style(prop, std::forward<RandomAccessableContainer>(dashes));
			}
			stroke_style create_stroke_style(const D2D1_STROKE_STYLE_PROPERTIES1& prop)const{
				return get_device().get_factory().create_stroke_style(prop);
			}
		private:
			static ID2D1Bitmap1* create_bitmap_impl(context& c, IWICFormatConverter* conv){
				return com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return c->CreateBitmapFromWicBitmap(conv, nullptr, x);});
			}
			static ID2D1Bitmap1* create_bitmap_impl(context& c, IWICFormatConverter* conv, const D2D1_BITMAP_PROPERTIES1& prop){
				return com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return c->CreateBitmapFromWicBitmap(conv, prop, x);});
			}
			static ID2D1Bitmap1* create_bitmap_impl(context& c, IDXGISurface2* surf){
				return com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return c->CreateBitmapFromDxgiSurface(surf, nullptr, x);});
			}
			static ID2D1Bitmap1* create_bitmap_impl(context& c, IDXGISurface2* surf, const D2D1_BITMAP_PROPERTIES1& prop){
				return com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return c->CreateBitmapFromDxgiSurface(surf, prop, x);});
			}
		public:
			template<typename Source>
			bitmap create_bitmap(Source&& src){
				return create_resource<bitmap>([&, src](){return create_bitmap_impl(*this, std::forward<Source>(src).get());});
			}
			template<typename Source>
			bitmap create_bitmap(Source&& src, const D2D1_BITMAP_PROPERTIES1& prop){
				return create_resource<bitmap>([&, src, prop](){return create_bitmap_impl(*this, std::forward<Source>(src).get(), prop);});
			}
		private:
			static ID2D1BitmapRenderTarget* create_compatible_render_target_impl(context& c, const D2D1_SIZE_F& desired_size, const D2D1_SIZE_U& desired_pixel_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE){
				return com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return c->CreateCompatibleRenderTarget(desired_size, desired_pixel_size, desired_format, option, ptr);});
			}
			static ID2D1BitmapRenderTarget* create_compatible_render_target_impl(context& c, const D2D1_SIZE_F& desired_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE){
				return com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return c->CreateCompatibleRenderTarget(&desired_size, nullptr, &desired_format, option, ptr);});
			}
			static ID2D1BitmapRenderTarget* create_compatible_render_target_impl(context& c, const D2D1_SIZE_U& desired_pixel_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE){
				return com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return c->CreateCompatibleRenderTarget(nullptr, &desired_pixel_size, &desired_format, option, ptr);});
			}
			static ID2D1BitmapRenderTarget* create_compatible_render_target_impl(context& c, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE){
				return com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return c->CreateCompatibleRenderTarget(nullptr, nullptr, &desired_format, option, ptr);});
			}
		public:
			bitmap_render_target create_compatible_render_target(const D2D1_SIZE_F& desired_size, const D2D1_SIZE_U& desired_pixel_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE){
				return create_resource<bitmap_render_target>([&, desired_size, desired_pixel_size, desired_format, option](){return create_compatible_render_target_impl(*this, desired_size, desired_pixel_size, desired_format, option);});
			}
			bitmap_render_target create_compatible_render_target(const D2D1_SIZE_F& desired_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE){
				return create_resource<bitmap_render_target>([&, desired_size, desired_format, option](){return create_compatible_render_target_impl(*this, desired_size, desired_format, option);});
			}
			bitmap_render_target create_compatible_render_target(const D2D1_SIZE_U& desired_pixel_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE){
				return create_resource<bitmap_render_target>([&, desired_pixel_size, desired_format, option](){return create_compatible_render_target_impl(*this, desired_pixel_size, desired_format, option);});
			}
			bitmap_render_target create_compatible_render_target(const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE){
				return create_resource<bitmap_render_target>([&, desired_format, option](){return create_compatible_render_target_impl(*this, desired_format, option);});
			}
			template<typename Size, typename F>
			bitmap prerender(const Size& desired_size, F&& f){
				return create_resource<bitmap>([&, desired_size, f](){
					com_ptr<ID2D1BitmapRenderTarget> rt{create_compatible_render_target_impl(*this, desired_size)};
					auto rt_ = rt.as<ID2D1DeviceContext>();
					rt_->BeginDraw();
					f(rt_.get());
					rt_->EndDraw();
					return std::move(rt.get_bitmap());
				});
			}
			template<typename F>
			bitmap prerender(F&& f){
				return create_resource<bitmap>([&, desired_size, f](){
					com_ptr<ID2D1BitmapRenderTarget> rt{create_compatible_render_target_impl(*this)};
					auto rt_ = rt.as<ID2D1DeviceContext>();
					rt_->BeginDraw();
					f(rt_.get());
					rt_->EndDraw();
					return std::move(rt.get_bitmap());
				});
			}
			template<typename F, typename G>
			HRESULT draw(F&& f, G&& g){
				(*this)->BeginDraw();
				f();
				auto ret = (*this)->EndDraw();
				g();
				return ret;
			}
			template<typename F>
			HRESULT draw(F&& f){
				return draw(std::forward<F>(f), []{});
			}

			friend class will::basic_d2d<ResourceHolder>::device;
		};
		context create_context(D2D1_DEVICE_CONTEXT_OPTIONS option = D2D1_DEVICE_CONTEXT_OPTIONS_NONE)const{return context{com_create_resource<ID2D1DeviceContext>([&](ID2D1DeviceContext** ptr){return (*this)->CreateDeviceContext(option, ptr);})};}
		using solid_color_brush = typename context::solid_color_brush;
		using gradient_stop_collection = typename context::gradient_stop_collection;
		using linear_gradient_brush = typename context::linear_gradient_brush;
		using radial_gradient_brush = typename context::radial_gradient_brush;
		using bitmap_brush = typename context::bitmap_brush;
		using bitmap = typename context::bitmap;
		using effect = typename context::effect;
		using bitmap_render_target = typename context::bitmap_render_target;
	};
	using solid_color_brush = typename device::solid_color_brush;
	using gradient_stop_collection = typename device::gradient_stop_collection;
	using linear_gradient_brush = typename device::linear_gradient_brush;
	using radial_gradient_brush = typename device::radial_gradient_brush;
	using bitmap_brush = typename device::bitmap_brush;
	using bitmap = typename device::bitmap;
	using effect = typename device::effect;
	using bitmap_render_target = typename device::bitmap_render_target;
	template<typename RandomAccessableContainer>
	stroke_style create_stroke_style(const D2D1_STROKE_STYLE_PROPERTIES1& prop, RandomAccessableContainer&& dashes)const{
		return stroke_style(com_create_resource<ID2D1StrokeStyle1>([&](ID2D1StrokeStyle1** ptr){return (*this)->CreateStrokeStyle(prop, dashes.data(), dashes.size(), ptr);}))
	}
	stroke_style create_stroke_style(const D2D1_STROKE_STYLE_PROPERTIES1& prop)const{
		return stroke_style(com_create_resource<ID2D1StrokeStyle1>([&](ID2D1StrokeStyle1** ptr){return (*this)->CreateStrokeStyle(prop, nullptr, 0, ptr);}));
	}
	device create_device(IDXGIDevice2* dxgi_device)const{return device{com_create_resource<ID2D1Device>([&](ID2D1Device** ptr){return (*this)->CreateDevice(dxgi_device, ptr);})};}
	template<typename DXGIDevice>
	device create_device(DXGIDevice&& dev)const{return create_device(std::forward<DXGIDevice>(dev).get());}
};

using d2d = basic_d2d<detail::resource_unholder>;

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
}
}
}