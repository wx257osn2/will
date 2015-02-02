#pragma once
#include"d3d.hpp"
#include"dxgi.hpp"
#include"d2d.hpp"
#include"wic.hpp"
#include"dwrite.hpp"
#include<memory>
#include<vector>
namespace will{
namespace detail{
class auto_restore_render_target{
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
	static reverse_<T> reverse(T& t){return reverse_<T>(t);}
	struct device_dependent_base{
		bool available;
		device_dependent_base()noexcept{}
		virtual ~device_dependent_base()noexcept{}
		virtual void construct() = 0;
		virtual void destruct()  = 0;
		virtual void* get() = 0;
		explicit operator bool(){return available;}
	};
	std::vector<std::shared_ptr<device_dependent_base>> ddrs;
	template<typename I>
	class ddr_base{
		auto_restore_render_target& parent;
		std::vector<std::shared_ptr<device_dependent_base>>::size_type index;
		bool life = true;
	public:
		ddr_base(auto_restore_render_target& parent, std::vector<std::shared_ptr<device_dependent_base>>::size_type i) : parent(parent), index(i){}
		~ddr_base(){if(life)if(parent.ddrs.size() -1u == index)parent.ddrs.pop_back();else parent.ddrs[index].reset();}
		ddr_base(const ddr_base&) = delete;
		ddr_base(ddr_base&& o):parent(o.parent), index(o.index), life(o.life){o.life = false;}
		ddr_base& operator=(const ddr_base&) = delete;
		ddr_base& operator=(ddr_base&&) = delete;
		I* get()const{return static_cast<I*>(parent.ddrs[index]->get());}
		I* operator->(){return get();}
	};
	class effect{
		auto_restore_render_target& parent;
		std::vector<std::shared_ptr<device_dependent_base>>::size_type index;
		bool life = true;
		class impl;
	public:
		effect(auto_restore_render_target& parent, std::vector<std::shared_ptr<device_dependent_base>>::size_type i) : parent(parent), index(i){}
		~effect(){if(life)if(parent.ddrs.size() -1u == index)parent.ddrs.pop_back();else parent.ddrs[index].reset();}
		effect(const effect&) = delete;
		effect(effect&& o):parent(o.parent), index(o.index), life(o.life){o.life = false;}
		effect& operator=(const effect&) = delete;
		effect& operator=(effect&&) = delete;
		ID2D1Effect* get()const{return static_cast<ID2D1Effect*>(parent.ddrs[index]->get());}
		ID2D1Effect* operator->()const{return get();}
		effect& operator|=(ID2D1Bitmap1* input){get()->SetInput(0, input); return *this;}
		effect& operator|=(ID2D1Effect* input){get()->SetInputEffect(0, input); return *this;}
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
	static void discard_ddr(std::vector<std::shared_ptr<device_dependent_base>>& ddrs){
		for(auto&& x : reverse(ddrs))
			if(x)
				x->destruct();
	}
	static void create_ddr(std::vector<std::shared_ptr<device_dependent_base>>& ddrs){
		for(auto&& x : ddrs)
			if(x)
				x->construct();
	}
	template<typename F, typename G>
	void restore_device(F&& f, G&& g){
		discard_ddr(ddrs);
		f();
		create_ddr(ddrs);
		g();
	}
	template<typename U, typename F>
	U make_ddr(F&& f){ddrs.emplace_back(std::make_shared<device_dependent_resource<decltype(f()), F>>(std::forward<F>(f)));return U(*this, ddrs.size()-1u);}
	template<typename F>
	class device_context:public device_dependent_base{
		std::aligned_storage_t<sizeof(d2d::device::context), sizeof(d2d::device::context)> data;
		F init_function;
	public:
		device_context(F&& f):init_function(std::forward<F>(f)){construct();}
		~device_context(){destruct();}
		void construct()override{
			new (reinterpret_cast<d2d::device::context*>(&data)) d2d::device::context(std::move(init_function()));
		}
		void destruct()override{
			(*reinterpret_cast<d2d::device::context*>(&data)).~context();
		}
		void* get()override{
			return static_cast<void*>((*reinterpret_cast<d2d::device::context*>(&data)).get());
		}
	};
public:
	ID2D1DeviceContext* devcon(){return static_cast<ID2D1DeviceContext*>(ddrs[0]->get());}
	template<typename F>
	explicit auto_restore_render_target(F&& f){ddrs.emplace_back(std::make_shared<device_context<F>>(std::forward<F>(f)));}
	auto_restore_render_target(const auto_restore_render_target&) = delete;
	auto_restore_render_target(auto_restore_render_target&&) = default;
	auto_restore_render_target& operator=(const auto_restore_render_target&) = delete;
	auto_restore_render_target& operator=(auto_restore_render_target&&) = default;
	~auto_restore_render_target() = default;
	using solid_color_brush = ddr_base<ID2D1SolidColorBrush>;
	using gradient_stop_collection = ddr_base<ID2D1GradientStopCollection1>;
	using linear_gradient_brush = ddr_base<ID2D1LinearGradientBrush>;
	using radial_gradient_brush = ddr_base<ID2D1RadialGradientBrush>;
	using bitmap = ddr_base<ID2D1Bitmap1>;
	using bitmap_brush = ddr_base<ID2D1BitmapBrush1>;
	template<typename T, typename F>
	class device_dependent_resource:public device_dependent_base{
		com_ptr<std::remove_pointer_t<T>> data;
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
			return static_cast<void*>(data.get());
		}
	};
	solid_color_brush create_solid_color_brush(const D2D1_COLOR_F& col, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
		return this->make_ddr<solid_color_brush>([col,prop,this]{return com_create_resource<ID2D1SolidColorBrush>([&](ID2D1SolidColorBrush** x){return devcon()->CreateSolidColorBrush(col, prop, x);});});
	}
	template<typename D2D1_GRADIENT_STOP_ARRAY>
			gradient_stop_collection create_gradient_stop_collection(D2D1_GRADIENT_STOP_ARRAY&& gradient_stops, D2D1_COLOR_SPACE preinterpolation_space = D2D1_COLOR_SPACE_SRGB, D2D1_COLOR_SPACE postinterpolation_space = D2D1_COLOR_SPACE_SCRGB, D2D1_BUFFER_PRECISION buffer_precision = D2D1_BUFFER_PRECISION_8BPC_UNORM_SRGB, D2D1_EXTEND_MODE extend_mode = D2D1_EXTEND_MODE_CLAMP, D2D1_COLOR_INTERPOLATION_MODE color_interpolation_mode = D2D1_COLOR_INTERPOLATION_MODE_STRAIGHT){
		return this->make_ddr<gradient_stop_collection>([gradient_stops, preinterpolation_space, postinterpolation_space, buffer_precision, extend_mode, color_interpolation_mode, this]{return com_create_resource<ID2D1GradientStopCollection>([&](ID2D1SolidColorBrush** x){return devcon()->CreateGradientStopCollection(gradient_stops.data(), gradient_stops.size(), preinterpolation_space, postinterpolation_space, buffer_precision, extend_mode, color_interpolation_mode, x);});});
	}
	linear_gradient_brush create_linear_gradient_brush(const gradient_stop_collection& stops, const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES& pos, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
		return this->make_ddr<linear_gradient_brush>([&stops, pos, prop, this]{return com_create_resource<ID2D1LinearGradientBrush>([&](ID2D1LinearGradientBrush** x){return devcon()->CreateLinearGradientBrush(pos, prop, stops.get(), x);});});
	}
	radial_gradient_brush create_radial_gradient_brush(const gradient_stop_collection& stops, const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES& pos, const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
		return this->make_ddr<radial_gradient_brush>([&stops, pos, prop, this]{return com_create_resource<ID2D1RadialGradientBrush>([&](ID2D1RadialGradientBrush** x){return devcon()->CreateRadialGradientBrush(pos, prop, stops.get(), x);});});
	}
	effect create_effect(const CLSID& clsid){
		return this->make_ddr<effect>([clsid, this]{return com_create_resource<ID2D1Effect>([&](ID2D1Effect** ptr){return devcon()->CreateEffect(clsid, ptr);});});
	}
	bitmap create_bitmap(IWICFormatConverter* conv){
		return this->make_ddr<bitmap>([conv, this]{return com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return devcon()->CreateBitmapFromWicBitmap(conv, nullptr, x);});});
	}
	bitmap create_bitmap(IWICFormatConverter* conv, const D2D1_BITMAP_PROPERTIES1& prop){
		return this->make_ddr<bitmap>([conv, prop, this]{return com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return devcon()->CreateBitmapFromWicBitmap(conv, prop, x);});});
	}
	bitmap create_bitmap(IDXGISurface2* surf){
		return this->make_ddr<bitmap>([surf, this]{return com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return devcon()->CreateBitmapFromDxgiSurface(surf, nullptr, x);});});
	}
	bitmap create_bitmap(IDXGISurface2* surf, const D2D1_BITMAP_PROPERTIES1& prop){
		return this->make_ddr<bitmap>([surf, prop, this]{return com_create_resource<ID2D1Bitmap1>([&](ID2D1Bitmap1** x){return devcon()->CreateBitmapFromDxgiSurface(surf, prop, x);});});
	}
	template<typename Source>
	bitmap create_bitmap(Source&& src){
		return create_bitmap(std::forward<Source>(src).get());
	}
	template<typename Source>
	bitmap create_bitmap(Source&& src, const D2D1_BITMAP_PROPERTIES1& prop){
		return create_bitmap(std::forward<Source>(src).get(), prop);
	}
	bitmap_brush create_bitmap_brush(const bitmap& bm, const D2D1_BITMAP_BRUSH_PROPERTIES1& extm = D2D1::BitmapBrushProperties1(), const D2D1_BRUSH_PROPERTIES& prop = D2D1::BrushProperties()){
		return this->make_ddr<bitmap_brush>([&bm, extm, prop, this]{return com_create_resource<ID2D1BitmapBrush1>([&](ID2D1BitmapBrush1** x){return devcon()->CreateBitmapBrush(bm.get(), extm, prop, x);});});
	}
	template<typename F, typename G, typename H, typename I>
	void draw(F&& draw_impl, G&& inter_restore, H&& after_restore, I&& after_draw){
		devcon()->BeginDraw();
		draw_impl();
		if(devcon()->EndDraw() == D2DERR_RECREATE_TARGET)
			restore_device(std::forward<G>(inter_restore), std::forward<H>(after_restore));
		after_draw();
	}
};
}
class hwnd_render_target:protected dxgi::swap_chain, public detail::auto_restore_render_target{
	static dxgi::swap_chain create_swap_chain(HWND hwnd){
		d3d::device dev;
		dxgi::device dev2{dev};
		auto sc = dev2.get_factory().create_swap_chain(dev, hwnd, will::dxgi::swap_chain::description{});
		assert(sc);
		dev2->SetMaximumFrameLatency(1);
		return sc;
	}
	HWND hwnd;
	HRESULT status;
public:
	hwnd_render_target(HWND hwnd):swap_chain(create_swap_chain(hwnd)), auto_restore_render_target([this]{return will::d2d::device::context(get_buffer());}), hwnd(hwnd), status(0ul){}
	auto operator->()->decltype(std::declval<d2d::device::context>().get()){return devcon();}
	template<typename F, typename G, typename H>
	void draw(F&& f, G&& g, H&& h){
		if(!(status & DXGI_STATUS_OCCLUDED))
			static_cast<detail::auto_restore_render_target*>(this)->draw(std::forward<F>(f), [&]{
				static_cast<dxgi::swap_chain*>(this)->~swap_chain();
				new (static_cast<dxgi::swap_chain*>(this)) dxgi::swap_chain(std::move(create_swap_chain(hwnd)));
			}, std::forward<G>(g), std::forward<H>(h));
		DXGI_PRESENT_PARAMETERS param = {};
		status = (*static_cast<dxgi::swap_chain*>(this))->Present1(1, bool(status), &param);
	}
	template<typename F, typename H>
	void draw(F&& f, H&& h){
		draw(std::forward<F>(f), []{}, std::forward<H>(h));
	}
	template<typename F>
	void draw(F&& f){
		draw(std::forward<F>(f), []{});
	}
};
class hdc_interop_render_target:public dxgi::surface, public detail::auto_restore_render_target{
	static dxgi::surface create_surface(int w, int h){
		d3d::device dev;
		return dxgi::surface(dev.create_texture2d(will::d3d::texture2d::description{}.width(w).height(h).mip_levels(1).array_size(1).format(DXGI_FORMAT_B8G8R8A8_UNORM).sample_count(1).sample_quality(0).bind_flags(D3D11_BIND_RENDER_TARGET).misc_flags(D3D11_RESOURCE_MISC_GDI_COMPATIBLE)));
	}
public:
	hdc_interop_render_target(int width, int height) : surface(create_surface(width, height)), auto_restore_render_target([this]{return d2d::device::context(*static_cast<dxgi::surface*>(this));}){}
	auto operator->()->decltype(std::declval<d2d::device::context>().get()){return devcon();}
	template<typename F, typename G, typename H>
	void draw(F&& f, G&& g, H&& h){
		static_cast<detail::auto_restore_render_target*>(this)->draw(std::forward<F>(f), []{}, std::forward<G>(g), std::forward<H>(h));
	}
	template<typename F, typename H>
	void draw(F&& f, H&& h){
		draw(std::forward<F>(f), []{}, std::forward<H>(h));
	}
	template<typename F>
	void draw(F&& f){
		draw(std::forward<F>(f), []{});
	}
};
}