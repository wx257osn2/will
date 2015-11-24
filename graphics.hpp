#pragma once
#include"d3d.hpp"
#include"dxgi.hpp"
#include"d2d.hpp"
#include"wic.hpp"
#include"dwrite.hpp"
#include<memory>
#include<vector>
namespace will{
class hwnd_render_target:protected dxgi::swap_chain, public d2d::device::context{
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
	hwnd_render_target(HWND hwnd):dxgi::swap_chain(create_swap_chain(hwnd)), d2d::device::context(get_buffer()), hwnd(hwnd), status(0ul){}
	using d2d::device::context::get;
	using d2d::device::context::operator->;
	using d2d::device::context::operator bool;
	template<typename F, typename G>
	void draw(F&& f, G&& g){
		using namespace std::literals::chrono_literals;
		if((status & DXGI_STATUS_OCCLUDED)){
			DXGI_PRESENT_PARAMETERS param = {};
			status = (*static_cast<dxgi::swap_chain*>(this))->Present1(1, DXGI_PRESENT_TEST, &param);
			sleep(static_cast<std::chrono::milliseconds>(1s)/60/2);
			return;
		}
		d2d::device::context::draw(std::forward<F>(f));
		DXGI_PRESENT_PARAMETERS param = {};
		status = dxgi::swap_chain::get()->Present1(1, 0, &param);
		g();
	}
	template<typename F>
	void draw(F&& f){
		draw(std::forward<F>(f), []{});
	}
};
class gdi_compatible_render_target:public dxgi::surface, public d2d::device::context{
	static dxgi::surface create_surface(int w, int h){
		d3d::device dev;
		return dxgi::surface(dev.create_texture2d(will::d3d::texture2d::description{}.width(w).height(h).mip_levels(1).array_size(1).format(DXGI_FORMAT_B8G8R8A8_UNORM).sample_count(1).sample_quality(0).bind_flags(D3D11_BIND_RENDER_TARGET).misc_flags(D3D11_RESOURCE_MISC_GDI_COMPATIBLE)));
	}
public:
	gdi_compatible_render_target(int width, int height) : dxgi::surface(create_surface(width, height)), d2d::device::context(*static_cast<dxgi::surface*>(this)){}
	using d2d::device::context::get;
	using d2d::device::context::operator->;
	using d2d::device::context::operator bool;
	template<typename F, typename G>
	void draw(F&& f, G&& g){
		d2d::device::context::draw(std::forward<F>(f), std::forward<G>(g));
	}
	template<typename F>
	void draw(F&& f){
		draw(std::forward<F>(f), []{});
	}
	template<typename F>
	bitmap prerender(const D2D1_SIZE_F& desired_size, F&& f){
		auto rt = create_compatible_render_target(desired_size, D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE);
		auto rt_ = rt.get_render_target();
		rt_->BeginDraw();
		f(rt_);
		rt_->EndDraw();
		return rt.get_bitmap();
	}
	template<typename F>
	bitmap prerender(const D2D1_SIZE_U& desired_pixel_size, F&& f){
		auto rt = create_compatible_render_target(desired_pixel_size, D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE);
		auto rt_ = rt.get_render_target();
		rt_->BeginDraw();
		f(rt_);
		rt_->EndDraw();
		return rt.get_bitmap();
	}
	template<typename F>
	bitmap prerender(F&& f){
		auto rt = create_compatible_render_target(D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE);
		auto rt_ = rt.get_render_target();
		rt_->BeginDraw();
		f(rt_);
		rt_->EndDraw();
		return rt.get_bitmap();
	}
};
}