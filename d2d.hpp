#pragma once
#include"com.hpp"
#include<dxgi1_2.h>
#include<wincodec.h>
#include<d2d1_1.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxguid.lib")
#include<functional>
#include<utility>
namespace will{
class d2d{
	com_ptr<ID2D1Factory1> factory;
	d2d(com_ptr<ID2D1Factory1>&& fac):factory(std::move(fac)){}
	template<typename T>
	class resource{
		com_ptr<T> res;
	public:
		explicit resource(T* resource):res(resource){}
		resource(const resource&) = delete;
		resource(resource&&) = default;
		resource& operator=(const resource&) = delete;
		resource& operator=(resource&&) = default;
		T* get()const{return res.get();}
		T* operator->()const{return res.get();}
		explicit operator bool()const{return static_cast<bool>(res);}
	};
public:
	explicit d2d(D2D1_FACTORY_TYPE t = D2D1_FACTORY_TYPE_MULTI_THREADED):factory(com_create_resource<ID2D1Factory1>(std::bind(static_cast<HRESULT(*)(D2D1_FACTORY_TYPE,ID2D1Factory1**)>(D2D1CreateFactory), t, std::placeholders::_1))){}
	d2d(const d2d&) = delete;
	d2d(d2d&&) = default;
	d2d& operator=(const d2d&) = delete;
	d2d& operator=(d2d&&) = default;
	~d2d() = default;
	ID2D1Factory1* get()const{return factory.get();}
	ID2D1Factory1* operator->()const{return factory.get();}
	using solid_color_brush = resource<ID2D1SolidColorBrush>;
	using gradient_stop_collection = resource<ID2D1GradientStopCollection1>;
	using linear_gradient_brush = resource<ID2D1LinearGradientBrush>;
	using radial_gradient_brush = resource<ID2D1RadialGradientBrush>;
	using bitmap_brush = resource<ID2D1BitmapBrush1>;
	class bitmap{
		com_ptr<ID2D1Bitmap1> bm;
	public:
		explicit bitmap(ID2D1Bitmap1* bitmap):bm(bitmap){}
		bitmap(const bitmap&) = delete;
		bitmap(bitmap&&) = default;
		bitmap& operator=(const bitmap&) = delete;
		bitmap& operator=(bitmap&&) = default;
		ID2D1Bitmap1* get()const{return bm.get();}
		ID2D1Bitmap1* operator->()const{return bm.get();}
		explicit operator bool()const{return static_cast<bool>(bm);}
		class property{
			D2D1_BITMAP_PROPERTIES1 prop;
		public:
			property():prop(){}
			property(const property&) = default;
			property(property&&) = default;
			property& operator=(const property&) = default;
			property& operator=(property&&) = default;
			~property() = default;
	#define PROPERTYDECL(name, type, membername) property& name(type t){prop.membername = t;return *this;}
			PROPERTYDECL(format, DXGI_FORMAT, pixelFormat.format)
			PROPERTYDECL(alpha_mode, D2D1_ALPHA_MODE, pixelFormat.alphaMode)
			PROPERTYDECL(dpi_x, FLOAT, dpiX)
			PROPERTYDECL(dpi_y, FLOAT, dpiY)
			property& dpi(FLOAT x, FLOAT y){prop.dpiX = x; prop.dpiY = y; return *this;}
			PROPERTYDECL(option, D2D1_BITMAP_OPTIONS, bitmapOptions)
			PROPERTYDECL(color_context, ID2D1ColorContext*, colorContext)
	#undef  PROPERTYDECL
			operator D2D1_BITMAP_PROPERTIES1&(){return prop;}
			D2D1_BITMAP_PROPERTIES1& get(){return prop;}
		};
	};
	class effect{
		com_ptr<ID2D1Effect> eff;
		class impl;
	public:
		explicit effect(ID2D1Effect* effect):eff(effect){}
		effect(const effect&) = delete;
		effect(effect&&) = default;
		effect& operator=(const effect&) = delete;
		effect& operator=(effect&&) = default;
		ID2D1Effect* get()const{return eff.get();}
		ID2D1Effect* operator->()const{return eff.get();}
		explicit operator bool()const{return static_cast<bool>(eff);}
		effect& operator|=(ID2D1Bitmap1* input){eff->SetInput(0, input); return *this;}
		effect& operator|=(ID2D1Effect* input){eff->SetInputEffect(0, input); return *this;}
		template<typename Input>
		effect& operator|=(Input&& input){return *this |= std::forward<Input>(input).get();}
	private:
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
	};
	class device{
		com_ptr<ID2D1Device> dev;
	public:
		explicit device(ID2D1Device* dev):dev(dev){}
		device(const device&) = delete;
		device(device&&) = default;
		device& operator=(const device&) = delete;
		device& operator=(device&&) = default;
		~device() = default;
		ID2D1Device* get()const{return dev.get();}
		ID2D1Device* operator->()const{return dev.get();}
		explicit operator bool()const{return static_cast<bool>(dev);}
		UINT64 max_texture_memory()const{return dev->GetMaximumTextureMemory();}
		void max_texture_memory(UINT64 bytes)const{dev->SetMaximumTextureMemory(bytes);}
		d2d get_factory()const{return d2d(com_ptr<ID2D1Factory>(com_create_resource<ID2D1Factory>([&](ID2D1Factory** ptr){return dev->GetFactory(ptr), S_OK;})).as<ID2D1Factory1>());}
		class context{
			com_ptr<ID2D1DeviceContext> cont;
			explicit context(ID2D1DeviceContext* ptr):cont(ptr){}
		public:
			explicit context(IDXGISurface2* surface):cont(com_create_resource<ID2D1DeviceContext>([&](ID2D1DeviceContext** ptr){return D2D1CreateDeviceContext(surface, nullptr, ptr);})){}
			context(IDXGISurface2* surface, const D2D1_CREATION_PROPERTIES& prop):cont(com_create_resource<ID2D1DeviceContext>([&](ID2D1DeviceContext** ptr){return D2D1CreateDeviceContext(surface, prop, ptr);})){}
			template<typename Surface>
			explicit context(Surface&& surf):context(std::forward<Surface>(surf).get()){}
			template<typename Surface>
			explicit context(Surface&& surf, const D2D1_CREATION_PROPERTIES& prop):context(std::forward<Surface>(surf).get(), prop){}
			context(const context&) = delete;
			context(context&& o) = default;
			context& operator=(const context&) = delete;
			context& operator=(context&&) = default;
			~context() = default;
			class property{
				D2D1_CREATION_PROPERTIES prop;
			public:
				property(D2D1_THREADING_MODE th):prop(){prop.threadingMode = th;}
				property(const property&) = default;
				property(property&&) = default;
				property& operator=(const property&) = default;
				property& operator=(property&&) = default;
				~property() = default;
		#define PROPERTYDECL(name, type, membername) property& name(type t){prop.membername = t;return *this;}
				PROPERTYDECL(threading_mode, D2D1_THREADING_MODE, threadingMode)
				PROPERTYDECL(debug_level, D2D1_DEBUG_LEVEL, debugLevel)
				PROPERTYDECL(option, D2D1_DEVICE_CONTEXT_OPTIONS, options)
		#undef  PROPERTYDECL
				operator D2D1_CREATION_PROPERTIES&(){return prop;}
				D2D1_CREATION_PROPERTIES& get(){return prop;}
			};
			ID2D1DeviceContext* get()const{return cont.get();}
			ID2D1DeviceContext* operator->()const{return cont.get();}
			explicit operator bool()const{return static_cast<bool>(cont);}
			device get_device()const{return device{com_create_resource<ID2D1Device>([&](ID2D1Device** ptr){return cont->GetDevice(ptr), S_OK;})};}
			solid_color_brush make_solid_color_brush(const D2D1_COLOR_F& col, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return solid_color_brush(com_create_resource<ID2D1SolidColorBrush>([&](ID2D1SolidColorBrush** x){return cont->CreateSolidColorBrush(col, prop, x);}));
			}
			template<typename D2D1_GRADIENT_STOP_ARRAY>
			gradient_stop_collection make_gradient_stop_collection(const D2D1_GRADIENT_STOP_ARRAY& gradient_stops, D2D1_COLOR_SPACE preinterpolation_space = D2D1_COLOR_SPACE_SRGB, D2D1_COLOR_SPACE postinterpolation_space = D2D1_COLOR_SPACE_SCRGB, D2D1_BUFFER_PRECISION buffer_precision = D2D1_BUFFER_PRECISION_8BPC_UNORM_SRGB, D2D1_EXTEND_MODE extend_mode = D2D1_EXTEND_MODE_CLAMP, D2D1_COLOR_INTERPOLATION_MODE color_interpolation_mode = D2D1_COLOR_INTERPOLATION_MODE_STRAIGHT){
				return gradient_stop_collection(com_create_resource<ID2D1GradientStopCollection1>([&](ID2D1GradientStopCollection1** x){return cont->CreateGradientStopCollection(gradient_stops.data(), gradient_stops.size(), preinterpolation_space, postinterpolation_space, buffer_precision, extend_mode, color_interpolation_mode, x);}));
			}
			linear_gradient_brush make_linear_gradient_brush(const gradient_stop_collection& stops, const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES& pos, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return linear_gradient_brush(com_create_resource<ID2D1LinearGradientBrush>([&](ID2D1LinearGradientBrush** x){return cont->CreateLinearGradientBrush(pos, prop, stops.get(), x);}));
			}
			radial_gradient_brush make_radial_gradient_brush(const gradient_stop_collection& stops, const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES& pos, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return radial_gradient_brush(com_create_resource<ID2D1RadialGradientBrush>([&](ID2D1RadialGradientBrush** x){return cont->CreateRadialGradientBrush(pos, prop, stops.get(), x);}));
			}
			bitmap_brush make_bitmap_brush(const bitmap& bm, const D2D1_BITMAP_BRUSH_PROPERTIES1& extm = D2D1::BitmapBrushProperties1(), const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
				return bitmap_brush(com_create_resource<ID2D1BitmapBrush1>([&](ID2D1BitmapBrush1** x){return cont->CreateBitmapBrush(bm.get(), extm, prop, x);}));
			}
			effect create_effect(const CLSID& clsid)const{
				return effect(com_create_resource<ID2D1Effect>([&](ID2D1Effect** ptr){return cont->CreateEffect(clsid, ptr);}));
			}
			bitmap create_bitmap(IWICFormatConverter* conv)const{
				return bitmap(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return cont->CreateBitmapFromWicBitmap(conv, nullptr, x);}));
			}
			bitmap create_bitmap(IWICFormatConverter* conv, const D2D1_BITMAP_PROPERTIES1& prop)const{
				return bitmap(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return cont->CreateBitmapFromWicBitmap(conv, prop, x);}));
			}
			bitmap create_bitmap(IDXGISurface2* surf)const{
				return bitmap(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return cont->CreateBitmapFromDxgiSurface(surf, nullptr, x);}));
			}
			bitmap create_bitmap(IDXGISurface2* surf, const D2D1_BITMAP_PROPERTIES1& prop)const{
				return bitmap(com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return cont->CreateBitmapFromDxgiSurface(surf, prop, x);}));
			}
			template<typename Source>
			bitmap create_bitmap(Source&& src)const{
				return create_bitmap(std::forward<Source>(src).get());
			}
			template<typename Source>
			bitmap create_bitmap(Source&& src, const D2D1_BITMAP_PROPERTIES1& prop)const{
				return create_bitmap(std::forward<Source>(src).get(), prop);
			}
			template<typename F, typename G>
			HRESULT draw(F&& f, G&& g){
				cont->BeginDraw();
				f();
				auto ret = cont->EndDraw();
				g();
				return ret;	
			}
			template<typename F>
			HRESULT draw(F&& f){
				return draw(std::forward<F>(f), []{});
			}
			friend class will::d2d::device;
		};
		context create_context(D2D1_DEVICE_CONTEXT_OPTIONS option = D2D1_DEVICE_CONTEXT_OPTIONS_NONE)const{return context{com_create_resource<ID2D1DeviceContext>([&](ID2D1DeviceContext** ptr){return dev->CreateDeviceContext(option, ptr);})};}
	};
	device create_device(IDXGIDevice2* dxgi_device)const{return device{com_create_resource<ID2D1Device>([&](ID2D1Device** ptr){return factory->CreateDevice(dxgi_device, ptr);})};}
	template<typename DXGIDevice>
	device create_device(DXGIDevice&& dev)const{return create_device(std::forward<DXGIDevice>(dev).get());}
};
}