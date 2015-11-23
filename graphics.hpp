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
class auto_restore_resources{
	template<typename T>
	class reverse_{
		T t;
	public:
		using iterator = typename std::decay_t<T>::reverse_iterator;
		reverse_(T&& t):t(std::forward<T>(t)){}
		iterator begin(){return t.rbegin();}
		iterator end(){return t.rend();}
	};
	template<typename T>
	static reverse_<T&&> reverse(T&& t){return {std::forward<T>(t)};}
	struct device_dependent_base{
		device_dependent_base()noexcept{}
		virtual ~device_dependent_base()noexcept{}
		virtual void construct() = 0;
		virtual void destruct()  = 0;
		virtual void* get() = 0;
	};
	using device_dependent_resources = std::vector<std::shared_ptr<device_dependent_base>>;
	device_dependent_resources ddrs;
	template<typename I>
	class ddr_base{
		auto_restore_resources& parent;
		device_dependent_resources::size_type index;
	public:
		ddr_base(auto_restore_resources& parent, device_dependent_resources::size_type i) : parent(parent), index(i+1){}
		~ddr_base(){if(index != 0)if(parent.ddrs.size()-1u == index)parent.ddrs.pop_back();else parent.ddrs[index].reset();}
		ddr_base(const ddr_base&) = delete;
		ddr_base(ddr_base&& o):parent(o.parent), index(o.index){o.index = 0;}
		ddr_base& operator=(const ddr_base&) = delete;
		ddr_base& operator=(ddr_base&&) = delete;
		I* get()const{return static_cast<I*>(parent.ddrs[index]->get());}
		I* operator->(){return get();}
		friend auto_restore_resources;
	};
public:
	struct factory_create_tag{};
	template<typename T>
	using factory_type = ddr_base<T>;
	template<typename T>
	using resource_type = ddr_base<T>;
private:
	static void discard_ddr(device_dependent_resources& ddrs){
		for(auto&& x : reverse(ddrs))
			if(x)
				x->destruct();
	}
	static void create_ddr(device_dependent_resources& ddrs){
		for(auto&& x : ddrs)
			if(x)
				x->construct();
	}
	template<typename T, typename F>
	class device_dependent_resource:public device_dependent_base{
		std::aligned_storage_t<sizeof(T), alignof(T)> data;
		F init_function;
		T& get_context(){return *reinterpret_cast<T*>(&data);}
	public:
		device_dependent_resource(F&& f):init_function(std::forward<F>(f)){construct();}
		~device_dependent_resource(){destruct();}
		void construct()override{
			new(&get_context()) T{init_function()};
		}
		void destruct()override{
			get_context().~T();
		}
		void* get()override{
			return static_cast<void*>(get_context().get());
		}
	};
	template<typename T, typename F>
	class device_dependent_resource<com_ptr<T>, F>:public device_dependent_base{
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
			return static_cast<void*>(data.get());
		}
	};
	template<typename T, typename F>
	class device_dependent_resource<T*, F> : public device_dependent_resource<com_ptr<T>, F>{public:using device_dependent_resource<com_ptr<T>, F>::device_dependent_resource;};
public:
	template<typename T, typename F>
	T create_factory(F&& f){ddrs.emplace_back(std::make_shared<device_dependent_resource<basic_d2d<auto_restore_resources>::device::context, F>>(std::forward<F>(f)));return T(*this, 0u);}
	template<typename T, typename F>
	T create_resource(F&& f){ddrs.emplace_back(std::make_shared<device_dependent_resource<std::decay_t<decltype(f())>, F>>(std::forward<F>(f)));return T(*this, ddrs.size()-1u);}
	template<typename F, typename G>
	void restore_device(F&& f, G&& g){
		discard_ddr(ddrs);
		f();
		create_ddr(ddrs);
		g();
	}
	template<typename U, typename F, typename G, typename H, typename I>
	static HRESULT draw(U&& u, F&& draw_impl, G&& inter_restore, H&& after_restore, I&& after_draw){
		auto ret = u.draw(std::forward<F>(draw_impl));
		if(ret == D2DERR_RECREATE_TARGET)
			u.restore_device(std::forward<G>(inter_restore), std::forward<H>(after_restore));
		after_draw();
		return ret;
	}
};
}
class hwnd_render_target:protected dxgi::swap_chain, public basic_d2d<detail::auto_restore_resources>::device::context{
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
	hwnd_render_target(HWND hwnd):dxgi::swap_chain(create_swap_chain(hwnd)), context(create_factory<ddr_base<ID2D1DeviceContext>>([this]{return get_buffer();})), hwnd(hwnd), status(0ul){}
	using context::get;
	using context::operator->;
	template<typename F, typename G, typename H>
	HRESULT draw(F&& f, G&& g, H&& h){
		using namespace std::literals::chrono_literals;
		if((status & DXGI_STATUS_OCCLUDED)){
			DXGI_PRESENT_PARAMETERS param = {};
			status = dxgi::swap_chain::get()->Present1(1, DXGI_PRESENT_TEST, &param);
			will::sleep(static_cast<std::chrono::milliseconds>(1s)/60/2);
			return status;
		}
		auto ret = detail::auto_restore_resources::draw(*this, std::forward<F>(f), [&]{
			dxgi::swap_chain::~swap_chain();
			new (static_cast<dxgi::swap_chain*>(this)) dxgi::swap_chain(std::move(create_swap_chain(hwnd)));
		}, std::forward<G>(g), std::forward<H>(h));
		DXGI_PRESENT_PARAMETERS param = {};
		status = dxgi::swap_chain::get()->Present1(1, 0, &param);
		return ret;
	}
	template<typename F, typename H>
	HRESULT draw(F&& f, H&& h){
		return draw(std::forward<F>(f), []{}, std::forward<H>(h));
	}
	template<typename F>
	HRESULT draw(F&& f){
		return draw(std::forward<F>(f), []{});
	}
};
class gdi_compatible_render_target:public dxgi::surface, public basic_d2d<detail::auto_restore_resources>::device::context{
	static dxgi::surface create_surface(int w, int h){
		d3d::device dev;
		return dxgi::surface(dev.create_texture2d(will::d3d::texture2d::description{}.width(w).height(h).mip_levels(1).array_size(1).format(DXGI_FORMAT_B8G8R8A8_UNORM).sample_count(1).sample_quality(0).bind_flags(D3D11_BIND_RENDER_TARGET).misc_flags(D3D11_RESOURCE_MISC_GDI_COMPATIBLE)));
	}
public:
	gdi_compatible_render_target(int width, int height) : dxgi::surface(create_surface(width, height)), context(create_factory<ddr_base<ID2D1DeviceContext>>([this]()->dxgi::surface&{return *static_cast<dxgi::surface*>(this);})){}
	using context::get;
	using context::operator->;
	template<typename F, typename G, typename H>
	void draw(F&& f, G&& g, H&& h){
		draw(std::forward<F>(f), []{}, std::forward<G>(g), std::forward<H>(h));
	}
	template<typename F, typename H>
	void draw(F&& f, H&& h){
		draw(std::forward<F>(f), []{}, std::forward<H>(h));
	}
	template<typename F>
	void draw(F&& f){
		draw(std::forward<F>(f), []{});
	}
private:
	static ID2D1BitmapRenderTarget* create_compatible_render_target_impl(gdi_compatible_render_target& c, const D2D1_SIZE_F& desired_size, const D2D1_SIZE_U& desired_pixel_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE){
		return com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return c->CreateCompatibleRenderTarget(desired_size, desired_pixel_size, desired_format, option, ptr);});
	}
	static ID2D1BitmapRenderTarget* create_compatible_render_target_impl(gdi_compatible_render_target& c, const D2D1_SIZE_F& desired_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE){
		return com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return c->CreateCompatibleRenderTarget(&desired_size, nullptr, &desired_format, option, ptr);});
	}
	static ID2D1BitmapRenderTarget* create_compatible_render_target_impl(gdi_compatible_render_target& c, const D2D1_SIZE_U& desired_pixel_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE){
		return com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return c->CreateCompatibleRenderTarget(nullptr, &desired_pixel_size, &desired_format, option, ptr);});
	}
	static ID2D1BitmapRenderTarget* create_compatible_render_target_impl(gdi_compatible_render_target& c, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE){
		return com_create_resource<ID2D1BitmapRenderTarget>([&](ID2D1BitmapRenderTarget** ptr){return c->CreateCompatibleRenderTarget(nullptr, nullptr, &desired_format, option, ptr);});
	}
public:
	bitmap_render_target create_compatible_render_target(const D2D1_SIZE_F& desired_size, const D2D1_SIZE_U& desired_pixel_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE){
		return create_resource<bitmap_render_target>([&, desired_size, desired_pixel_size, desired_format, option](){return create_compatible_render_target_impl(*this, desired_size, desired_pixel_size, desired_format, option);});
	}
	bitmap_render_target create_compatible_render_target(const D2D1_SIZE_F& desired_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE){
		return create_resource<bitmap_render_target>([&, desired_size, desired_format, option](){return create_compatible_render_target_impl(*this, desired_size, desired_format, option);});
	}
	bitmap_render_target create_compatible_render_target(const D2D1_SIZE_U& desired_pixel_size, const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE){
		return create_resource<bitmap_render_target>([&, desired_pixel_size, desired_format, option](){return create_compatible_render_target_impl(*this, desired_pixel_size, desired_format, option);});
	}
	bitmap_render_target create_compatible_render_target(const D2D1_PIXEL_FORMAT& desired_format = D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS option = D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE){
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
};
}