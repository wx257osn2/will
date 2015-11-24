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
class d2d : public detail::resource<ID2D1Factory1>{
	using resource::resource;
public:
	explicit d2d(D2D1_FACTORY_TYPE t = D2D1_FACTORY_TYPE_MULTI_THREADED):resource(com_create_resource<ID2D1Factory1>(std::bind(static_cast<HRESULT(*)(D2D1_FACTORY_TYPE,ID2D1Factory1**)>(D2D1CreateFactory), t, std::placeholders::_1))){}
	struct stroke_style : detail::resource<ID2D1StrokeStyle1>{
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
	struct device : detail::resource<ID2D1Device>{
		using resource::resource;
		UINT64 max_texture_memory()const{return (*this)->GetMaximumTextureMemory();}
		void max_texture_memory(UINT64 bytes)const{(*this)->SetMaximumTextureMemory(bytes);}
		d2d get_factory()const{return d2d{com_ptr<ID2D1Factory>{com_create_resource<ID2D1Factory>([&](ID2D1Factory** ptr){return (*this)->GetFactory(ptr), S_OK;})}.as<ID2D1Factory1>()};}
		class context : public detail::resource<ID2D1DeviceContext>{
			using resource::resource;
		public:
			class solid_color_brush : public detail::resource<ID2D1SolidColorBrush>{
				class _color{
					solid_color_brush& brush;
				public:
					_color(solid_color_brush& b):brush(b){}
					_color(const _color&) = delete;
					_color(_color&&) = default;
					_color& operator=(const _color&) = delete;
					_color& operator=(_color&&) = default;
					~_color() = default;
					operator D2D1_COLOR_F()const{return brush->GetColor();}
					_color& operator=(const D2D1::ColorF& c){brush->SetColor(c);}
				};
			public:
				using resource::resource;
				_color color = *this;
			};
			struct gradient_stop_collection : detail::resource<ID2D1GradientStopCollection1>{
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
			using linear_gradient_brush = detail::resource<ID2D1LinearGradientBrush>;
			using radial_gradient_brush = detail::resource<ID2D1RadialGradientBrush>;
			using bitmap_brush = detail::resource<ID2D1BitmapBrush1>;
			struct bitmap : detail::resource<ID2D1Bitmap1>{
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
			class effect : public detail::resource<ID2D1Effect>{
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
			public:
				using resource::resource;
				effect& operator|=(ID2D1Bitmap1* input){(*this)->SetInput(0, input); return *this;}
				effect& operator|=(ID2D1Effect* input){(*this)->SetInputEffect(0, input); return *this;}
				template<typename Input>
				effect& operator|=(Input&& input){return *this |= std::forward<Input>(input).get();}
				impl operator[](UINT32 i){return impl{i, *this};}
				template<typename Prop, typename Value>
				effect&  operator()(Prop&& p, Value&& v)& {(*this)->SetValue(std::forward<Prop>(p), std::forward<Value>(v)); return           *this ;}
				template<typename Prop, typename Value>
				effect&& operator()(Prop&& p, Value&& v)&&{(*this)->SetValue(std::forward<Prop>(p), std::forward<Value>(v)); return std::move(*this);}
			};
			struct bitmap_render_target : detail::resource<ID2D1BitmapRenderTarget>{
				using resource::resource;
				bitmap get_bitmap()const{
					return bitmap{com_ptr<ID2D1Bitmap>{com_create_resource<ID2D1Bitmap>([&](ID2D1Bitmap** ptr){return (*this)->GetBitmap(ptr);})}.as<ID2D1Bitmap1>()};
				}
				context get_render_target()const{
					return context{res.as<ID2D1DeviceContext>()};
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
			explicit context(IDXGISurface2* surface):resource(com_create_resource<ID2D1DeviceContext>([&](ID2D1DeviceContext** ptr){return D2D1CreateDeviceContext(surface, nullptr, ptr);})){}
			context(IDXGISurface2* surface, const D2D1_CREATION_PROPERTIES& prop):resource(com_create_resource<ID2D1DeviceContext>([&](ID2D1DeviceContext** ptr){return D2D1CreateDeviceContext(surface, prop, ptr);})){}
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
			device get_device()const{return device{com_create_resource<ID2D1Device>([&](ID2D1Device** ptr){return (*this)->GetDevice(ptr), S_OK;})};}
			solid_color_brush create_solid_color_brush(const D2D1_COLOR_F& col, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return solid_color_brush(com_create_resource<ID2D1SolidColorBrush>([&](ID2D1SolidColorBrush** x){return (*this)->CreateSolidColorBrush(col, prop, x);}));
			}
			template<typename D2D1_GRADIENT_STOP_ARRAY>
			gradient_stop_collection create_gradient_stop_collection(const D2D1_GRADIENT_STOP_ARRAY& gradient_stops, D2D1_COLOR_SPACE preinterpolation_space = D2D1_COLOR_SPACE_SRGB, D2D1_COLOR_SPACE postinterpolation_space = D2D1_COLOR_SPACE_SCRGB, D2D1_BUFFER_PRECISION buffer_precision = D2D1_BUFFER_PRECISION_8BPC_UNORM_SRGB, D2D1_EXTEND_MODE extend_mode = D2D1_EXTEND_MODE_CLAMP, D2D1_COLOR_INTERPOLATION_MODE color_interpolation_mode = D2D1_COLOR_INTERPOLATION_MODE_STRAIGHT){
				return gradient_stop_collection(com_create_resource<ID2D1GradientStopCollection1>([&](ID2D1GradientStopCollection1** x){return (*this)->CreateGradientStopCollection(gradient_stops.data(), gradient_stops.size(), preinterpolation_space, postinterpolation_space, buffer_precision, extend_mode, color_interpolation_mode, x);}));
			}
			template<typename D2D1_GRADIENT_STOP_ARRAY>
			gradient_stop_collection create_gradient_stop_collection(const D2D1_GRADIENT_STOP_ARRAY& gradient_stops, const typename gradient_stop_collection::property& prop){
				return std::move(create_gradient_stop_collection(gradient_stops, prop.preerps, prop.posterps, prop.bufpre, prop.extm, prop.colerpm));
			}
			linear_gradient_brush create_linear_gradient_brush(const gradient_stop_collection& stops, const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES& pos, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return linear_gradient_brush(com_create_resource<ID2D1LinearGradientBrush>([&](ID2D1LinearGradientBrush** x){return (*this)->CreateLinearGradientBrush(pos, prop, stops.get(), x);}));
			}
			radial_gradient_brush create_radial_gradient_brush(const gradient_stop_collection& stops, const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES& pos, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return radial_gradient_brush(com_create_resource<ID2D1RadialGradientBrush>([&](ID2D1RadialGradientBrush** x){return (*this)->CreateRadialGradientBrush(pos, prop, stops.get(), x);}));
			}
			bitmap_brush create_bitmap_brush(const bitmap& bm, const D2D1_BITMAP_BRUSH_PROPERTIES1& extm = D2D1::BitmapBrushProperties1(), const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return bitmap_brush(com_create_resource<ID2D1BitmapBrush1>([&](ID2D1BitmapBrush1** x){return (*this)->CreateBitmapBrush(bm.get(), extm, prop, x);}));
			}
			effect create_effect(const CLSID& clsid)const{
				return effect(com_create_resource<ID2D1Effect>([&](ID2D1Effect** ptr){return (*this)->CreateEffect(clsid, ptr);}));
			}
			template<typename RandomAccessableContainer>
			stroke_style create_stroke_style(const D2D1_STROKE_STYLE_PROPERTIES1& prop, RandomAccessableContainer&& dashes)const{
				return get_device().get_factory().create_stroke_style(prop, std::forward<RandomAccessableContainer>(dashes));
			}
			stroke_style create_stroke_style(const D2D1_STROKE_STYLE_PROPERTIES1& prop)const{
				return get_device().get_factory().create_stroke_style(prop);
			}
			bitmap create_bitmap(IWICFormatConverter* conv)const{
				return bitmap(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return (*this)->CreateBitmapFromWicBitmap(conv, nullptr, x);}));
			}
			bitmap create_bitmap(IWICFormatConverter* conv, const D2D1_BITMAP_PROPERTIES1& prop)const{
				return bitmap(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return (*this)->CreateBitmapFromWicBitmap(conv, prop, x);}));
			}
			bitmap create_bitmap(IDXGISurface2* surf)const{
				return bitmap(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return (*this)->CreateBitmapFromDxgiSurface(surf, nullptr, x);}));
			}
			bitmap create_bitmap(IDXGISurface2* surf, const D2D1_BITMAP_PROPERTIES1& prop)const{
				return bitmap(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return (*this)->CreateBitmapFromDxgiSurface(surf, prop, x);}));
			}
			template<typename Source>
			bitmap create_bitmap(Source&& src)const{
				return create_bitmap(std::forward<Source>(src).get());
			}
			template<typename Source>
			bitmap create_bitmap(Source&& src, const D2D1_BITMAP_PROPERTIES1& prop)const{
				return create_bitmap(std::forward<Source>(src).get(), prop);
			}
			bitmap_render_target create_compatible_render_target(const D2D1_SIZE_F& desired_size, const D2D1_SIZE_U& desired_pixel_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE)const{
				return bitmap_render_target{com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return (*this)->CreateCompatibleRenderTarget(desired_size, desired_pixel_size, desired_format, option, ptr);})};
			}
			bitmap_render_target create_compatible_render_target(const D2D1_SIZE_F& desired_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE)const{
				return bitmap_render_target{com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return (*this)->CreateCompatibleRenderTarget(&desired_size, nullptr, &desired_format, option, ptr);})};
			}
			bitmap_render_target create_compatible_render_target(const D2D1_SIZE_U& desired_pixel_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE)const{
				return bitmap_render_target{com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return (*this)->CreateCompatibleRenderTarget(nullptr, &desired_pixel_size, &desired_format, option, ptr);})};
			}
			bitmap_render_target create_compatible_render_target(const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE)const{
				return bitmap_render_target{com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return (*this)->CreateCompatibleRenderTarget(nullptr, nullptr, &desired_format, option, ptr);})};
			}
			template<typename F>
			bitmap prerender(const D2D1_SIZE_F& desired_size, F&& f){
				auto rt = create_compatible_render_target(desired_size);
				auto rt_ = rt.get_render_target();
				rt_->BeginDraw();
				f(rt_);
				rt_->EndDraw();
				return rt.get_bitmap();
			}
			template<typename F>
			bitmap prerender(const D2D1_SIZE_U& desired_pixel_size, F&& f){
				auto rt = create_compatible_render_target(desired_pixel_size);
				auto rt_ = rt.get_render_target();
				rt_->BeginDraw();
				f(rt_);
				rt_->EndDraw();
				return rt.get_bitmap();
			}
			template<typename F>
			bitmap prerender(F&& f){
				auto rt = create_compatible_render_target();
				auto rt_ = rt.get_render_target();
				rt_->BeginDraw();
				f(rt_);
				rt_->EndDraw();
				return rt.get_bitmap();
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

			friend struct will::d2d::device;
		};
		context create_context(D2D1_DEVICE_CONTEXT_OPTIONS option = D2D1_DEVICE_CONTEXT_OPTIONS_NONE)const{return context{com_create_resource<ID2D1DeviceContext>([&](ID2D1DeviceContext** ptr){return (*this)->CreateDeviceContext(option, ptr);})};}
		using solid_color_brush =  context::solid_color_brush;
		using gradient_stop_collection =  context::gradient_stop_collection;
		using linear_gradient_brush =  context::linear_gradient_brush;
		using radial_gradient_brush =  context::radial_gradient_brush;
		using bitmap_brush =  context::bitmap_brush;
		using bitmap =  context::bitmap;
		using effect =  context::effect;
		using bitmap_render_target =  context::bitmap_render_target;
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