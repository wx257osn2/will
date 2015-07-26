#pragma once
#include"com.hpp"
#include"wic.hpp"
#include<d2d1.h>
#pragma comment(lib, "d2d1.lib")
#include<memory>
#include<vector>
namespace will{
class d2d{
	com_ptr<ID2D1Factory> factory;
	template<typename T>
	class reverse_{
		T& t;
	public:
		using iterator = typename T::reverse_iterator;
		reverse_(T& t):t(t){}
		iterator begin(){return t.rbegin();}
		iterator end(){return t.rend();}
	};
	template<typename T>
	struct ref_wrap{
		T* t;
		ref_wrap():t(nullptr){}
		ref_wrap(T& t):t(&t){}
		ref_wrap& operator=(T& o){t = &o;return *this;}
		operator T&()const{return *t;}
		T& get()const{return *t;}
	};
	template<typename T>
	static reverse_<T> reverse(T& t){return reverse_<T>(t);}
	struct device_dependent_base{
		bool available;
		device_dependent_base()noexcept{}
		virtual ~device_dependent_base()noexcept{}
		virtual void construct() = 0;
		virtual void destruct()  = 0;
		virtual void* get() = 0;
		operator bool(){return available;}
	};
	std::vector<std::vector<std::shared_ptr<device_dependent_base>>> ddrs;
	template<typename I>
	class ddr_base{
		ref_wrap<d2d> factory;
		std::vector<std::vector<std::shared_ptr<device_dependent_base>>>::size_type parent_index;
		std::vector<std::shared_ptr<device_dependent_base>>::size_type index;
		bool life;
		template<typename T, typename Dummy = void>
		class device_context;
		template<typename Dummy>
		class device_context<ID2D1GdiInteropRenderTarget, Dummy>{HDC dc;ddr_base& t;public:device_context(D2D1_DC_INITIALIZE_MODE mode, ddr_base& t):t(t){t->GetDC(mode, &dc);}~device_context(){RECT r = {};t->ReleaseDC(&r);}operator HDC(){return dc;}};
	public:
		ddr_base():life(false){}
		ddr_base(d2d& w, std::vector<std::vector<std::shared_ptr<device_dependent_base>>>::size_type pi, std::vector<std::shared_ptr<device_dependent_base>>::size_type i)
			: factory(w), parent_index(pi), index(i), life(true){}
		~ddr_base(){if(life)factory.get().ddrs[parent_index][index].reset();}
		ddr_base(const ddr_base&) = delete;
		ddr_base(ddr_base&& o):factory(o.factory), parent_index(o.parent_index), index(o.index), life(o.life){}
		ddr_base& operator=(const ddr_base&) = delete;
		ddr_base& operator=(ddr_base&& o){factory = o.factory;parent_index = o.parent_index;index = o.index;life = o.life;o.life = false;return *this;}
		I* get()const{return factory.t ? reinterpret_cast<I*>(factory.get().ddrs[parent_index][index].get()->get()) : nullptr;}
		I* operator->(){return get();}
		device_context<I> get_dc(D2D1_DC_INITIALIZE_MODE mode){return {mode, *this};}
		explicit operator bool(){return life && get() != nullptr;}
	};
public:
	using solid_color_brush = ddr_base<ID2D1SolidColorBrush>;
	using gradient_stop_collection = ddr_base<ID2D1GradientStopCollection>;
	using linear_gradient_brush = ddr_base<ID2D1LinearGradientBrush>;
	using radial_gradient_brush = ddr_base<ID2D1RadialGradientBrush>;
	using bitmap = ddr_base<ID2D1Bitmap>;
	using bitmap_brush = ddr_base<ID2D1BitmapBrush>;
	using gdi_interop_render_target = ddr_base<ID2D1GdiInteropRenderTarget>;
	template<typename T, typename F>
	class device_dependent_resource:public device_dependent_base{
		com_ptr<T> data;
		F init_function;
	public:
		device_dependent_resource(F&& f):init_function(std::forward<F>(f)){construct();}
		~device_dependent_resource(){destruct();}
		void construct()override{
			data = init_function();
		}
		void destruct()override{
			data.reset();
		}
		void* get()override{
			return data.get();
		}
	};
private:
	template<typename T, typename F>
	static std::shared_ptr<device_dependent_resource<T, F>> make_ddr(F&& f){return std::make_shared<device_dependent_resource<T, F>>(std::forward<F>(f));}
	template<typename F>
	void add_ddr(F&& f){ddrs.emplace_back(std::vector<std::shared_ptr<device_dependent_base>>{make_ddr<typename std::remove_pointer<decltype(f())>::type>(std::forward<F>(f))});}
	static void discard_ddr(std::vector<std::shared_ptr<device_dependent_base>>& ddrs){
		for(auto&& x : reverse(ddrs))
			x->destruct();
	}
	static void create_ddr(std::vector<std::shared_ptr<device_dependent_base>>& ddrs){
		for(auto&& x : ddrs)
			x->construct();
	}
	template<typename F>
	static void restore_device_lost(std::vector<std::shared_ptr<device_dependent_base>>& ddrs, F&& f){
		discard_ddr(ddrs);
		create_ddr(ddrs);
		f();
	}
	template<typename T>
	class render_target_base{
		ref_wrap<d2d> factory;
		std::vector<std::vector<std::shared_ptr<device_dependent_base>>>::size_type index;
		bool life;
		template<typename U, typename F>
		U make_ddr(F&& f){factory.get().ddrs[index].emplace_back(d2d::make_ddr<typename std::remove_pointer<decltype(f())>::type>(std::forward<F>(f)));return U(factory, index, factory.get().ddrs[index].size()-1u);}
	public:
		render_target_base():life(false){}
		render_target_base(d2d& w, std::vector<std::vector<std::shared_ptr<device_dependent_base>>>::size_type i):factory(w), index(i), life(true){}
		~render_target_base(){if(life){factory.get().ddrs[index].clear();factory.get().ddrs[index].shrink_to_fit();}}
		render_target_base(const render_target_base&) = delete;
		render_target_base(render_target_base&& o):factory(o.factory), index(o.index), life(o.life){o.life = false;}
		render_target_base& operator=(const render_target_base&) = delete;
		render_target_base& operator=(render_target_base&& o){factory = o.factory;index = o.index;life = o.life;o.life = false;return *this;}
		T* get(){return factory.t ? reinterpret_cast<T*>(factory.get().ddrs[index][0].get()->get()) : nullptr;}
		T* operator->(){return get();}
		explicit operator bool(){return life && get() != nullptr;}
		solid_color_brush create_solid_color_brush(const D2D1_COLOR_F& col, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
			return this->make_ddr<solid_color_brush>([col,prop,this]{return com_create_resource<ID2D1SolidColorBrush>([&](ID2D1SolidColorBrush** x){return get()->CreateSolidColorBrush(col, prop, x);});});
		}
		gradient_stop_collection create_gradient_stop_collection(std::vector<D2D1_GRADIENT_STOP>&& gradient_stops, D2D1_GAMMA gamma = D2D1_GAMMA_1_0, D2D1_EXTEND_MODE extm = D2D1_EXTEND_MODE_CLAMP){
			return this->make_ddr<gradient_stop_collection>([gradient_stops, gamma, extm, this]{return com_create_resource<ID2D1GradientStopCollection>([&](ID2D1SolidColorBrush** x){return get()->CreateGradientStopCollection(gradient_stops.data(), gradient_stops.size(), gamma, extm, x);});});
		}
		linear_gradient_brush create_linear_gradient_brush(const gradient_stop_collection& stops, const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES& pos, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
			return this->make_ddr<linear_gradient_brush>([&stops, pos, prop, this]{return com_create_resource<ID2D1LinearGradientBrush>([&](ID2D1LinearGradientBrush** x){return get()->CreateLinearGradientBrush(pos, prop, stops.get(), x);});});
		}
		radial_gradient_brush create_radial_gradient_brush(const gradient_stop_collection& stops, const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES& pos, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
			return this->make_ddr<radial_gradient_brush>([&stops, pos, prop, this]{return com_create_resource<ID2D1RadialGradientBrush>([&](ID2D1RadialGradientBrush** x){return get()->CreateRadialGradientBrush(pos, prop, stops.get(), x);});});
		}
		bitmap create_bitmap(IWICFormatConverter* conv){
			return this->make_ddr<bitmap>([conv, this]()->ID2D1Bitmap*{return conv != nullptr ? com_create_resource<ID2D1Bitmap>([&](ID2D1Bitmap** x){return get()->CreateBitmapFromWicBitmap(conv, nullptr, x);}) : nullptr;});
		}
		template<typename WICConverter>
		bitmap create_bitmap(WICConverter&& conv){
			return make_bitmap(conv.get());
		}
		bitmap_brush create_bitmap_brush(const bitmap& bm, const D2D1_BITMAP_BRUSH_PROPERTIES& extm, const D2D1_BRUSH_PROPERTIES& prop){
			return this->make_ddr<bitmap_brush>([&bm, extm, prop, this]{return com_create_resource<ID2D1BitmapBrush>([&](ID2D1BitmapBrush** x){return get()->CreateBitmapBrush(bm.get(), extm, prop, x);});});
		}
		gdi_interop_render_target create_gdi_interop_render_target(){return this->make_ddr<gdi_interop_render_target>([this]{return com_create_resource<ID2D1GdiInteropRenderTarget>([&](ID2D1GdiInteropRenderTarget** x){return get()->QueryInterface(x);});});}
		template<typename F, typename G>
		void draw(F&& f, G&& g){
			(*this)->BeginDraw();
			f();
			if((*this)->EndDraw() == D2DERR_RECREATE_TARGET)
				restore_device_lost(factory.get().ddrs[index], std::forward<G>(g));
		}
		template<typename F>
		void draw(F&& f){
			draw(std::forward<F>(f), []{});
		}
	};
public:
	using hwnd_render_target = render_target_base<ID2D1HwndRenderTarget>;
	using render_target = render_target_base<ID2D1RenderTarget>;
	d2d(D2D1_FACTORY_TYPE t = D2D1_FACTORY_TYPE_MULTI_THREADED):factory(com_create_resource<ID2D1Factory>(std::bind(static_cast<HRESULT(*)(D2D1_FACTORY_TYPE,ID2D1Factory**)>(D2D1CreateFactory), t, std::placeholders::_1))){}
	d2d(const d2d&) = default;
	d2d(d2d&&) = default;
	~d2d() = default;
	hwnd_render_target create_hwnd_render_target(HWND hwnd, const D2D1_RENDER_TARGET_PROPERTIES& prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1_PRESENT_OPTIONS op = D2D1_PRESENT_OPTIONS_IMMEDIATELY){add_ddr([hwnd, prop, op, this]{return com_create_resource<ID2D1HwndRenderTarget>([&](ID2D1HwndRenderTarget** x){const auto rc = [](HWND hwnd){RECT rc; GetClientRect(hwnd, &rc); return rc;}(hwnd);return factory->CreateHwndRenderTarget(prop, D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top), op), x);});});return hwnd_render_target(*this, ddrs.size() - 1u);}
	template<typename Window>
	hwnd_render_target create_hwnd_render_target(Window&& w,  const D2D1_RENDER_TARGET_PROPERTIES& prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1_PRESENT_OPTIONS op = D2D1_PRESENT_OPTIONS_IMMEDIATELY){return create_hwnd_render_target(w.get_hwnd(), prop, op);}
	render_target create_wic_bitmap_render_target(IWICBitmap* bitmap, const D2D1_RENDER_TARGET_PROPERTIES& prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), 0.0f, 0.0f, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE)){add_ddr([bitmap, prop, this]{return com_create_resource<ID2D1RenderTarget>([&](ID2D1RenderTarget** x){return factory->CreateWicBitmapRenderTarget(bitmap, prop, x);});});return render_target(*this, ddrs.size() - 1u);}
	template<typename Bitmap>
	render_target create_wic_bitmap_render_target(Bitmap&& bm, const D2D1_RENDER_TARGET_PROPERTIES& prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), 0.0f, 0.0f, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE)){return create_wic_bitmap_render_target(bm.get(), prop);}
	render_target create_dxgi_surface_render_target(IDXGISurface* surface, const D2D1_RENDER_TARGET_PROPERTIES& prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), 0.0f, 0.0f, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE)){add_ddr([surface, prop, this]{return com_create_resource<ID2D1RenderTarget>([&](ID2D1RenderTarget** x){return factory->CreateDxgiSurfaceRenderTarget(surface, prop, x);});});return render_target(*this, ddrs.size() - 1u);}
	template<typename Surface>
	render_target create_dxgi_surface_render_target(Surface&& sf, const D2D1_RENDER_TARGET_PROPERTIES& prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), 0.0f, 0.0f, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE)){return create_dxgi_surface_render_target(sf.get(), prop);}
	ID2D1Factory* get(){return factory.get();}
	ID2D1Factory* operator->(){return get();}
};
}