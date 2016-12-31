#pragma once
#include"d3d.hpp"
#include"dxgi.hpp"
#include"d2d.hpp"
#include"wic.hpp"
#include"dwrite.hpp"
#include<memory>
#include<vector>
namespace will{
class hwnd_render_target:protected d3d::device, protected dxgi::swap_chain, public d2d::device::context, public dwrite{
	static expected<dxgi::swap_chain, hresult_error> create_swap_chain(HWND hwnd, d3d::device&& dev){
		return dxgi::device::create(dev).bind([&](dxgi::device&& dev2){
			return dev2.get_factory().bind([&](dxgi&& dxgi_fac){
				return dxgi_fac.create_swap_chain(dev, hwnd, will::dxgi::swap_chain::description{}).bind([&](dxgi::swap_chain&& sc){
					return dev2.set_maximum_frame_latency(1).map([&]()->dxgi::swap_chain{
						return std::move(sc);
					});
				});
			});
		});
	}
	HWND hwnd;
	HRESULT status;
	hwnd_render_target(d3d::device&& dev, dxgi::swap_chain&& sc, d2d::device::context&& devcon, dwrite&& dw, HWND hw) : device{std::move(dev)}, swap_chain{std::move(sc)}, d2d::device::context{std::move(devcon)}, dwrite{std::move(dw)}, hwnd(std::move(hw)), status(0ul){}
public:
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd, D3D11_CREATE_DEVICE_FLAG flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT, const D2D1_CREATION_PROPERTIES* prop = nullptr, DWRITE_FACTORY_TYPE type = DWRITE_FACTORY_TYPE_SHARED){
		return d3d::create_device(flag).bind([&](d3d::device&& d3d_dev){
			return create_swap_chain(hwnd, std::move(d3d_dev)).bind([&](dxgi::swap_chain&& sc){
				return sc.get_buffer().bind([&](dxgi::surface&& surf){
					return d2d::device::context::create(surf, prop).bind([&](d2d::device::context&& devcon){
						return dwrite::create_factory(type).map([&](dwrite&& dw){
							return hwnd_render_target{std::move(d3d_dev), std::move(sc), std::move(devcon), std::move(dw), hwnd};
						});
					});
				});
			});
		});
	}
	template<typename Window, typename... Args, std::enable_if_t<!std::is_same<std::decay_t<Window>, HWND>::value>* = nullptr>
	static expected<hwnd_render_target, hresult_error> create(Window&& w, Args&&... args){return create(w.get(), std::forward<Args>(args)...);}
	explicit hwnd_render_target(HWND hwnd):hwnd_render_target{+create(hwnd)}{}
	hwnd_render_target(HWND hwnd, const D2D1_CREATION_PROPERTIES& prop):hwnd_render_target{+create(hwnd, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &prop)}{}
	hwnd_render_target(HWND hwnd, D3D11_CREATE_DEVICE_FLAG flag, const D2D1_CREATION_PROPERTIES& prop):hwnd_render_target{+create(hwnd, flag, &prop)}{}
	hwnd_render_target(HWND hwnd, D3D11_CREATE_DEVICE_FLAG flag, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type):hwnd_render_target{+create(hwnd, flag, &prop, type)}{}
	template<typename Window, typename... Args, std::enable_if_t<!std::is_same<std::decay_t<Window>, HWND>::value>* = nullptr>
	explicit hwnd_render_target(Window&& w, Args&&... args):hwnd_render_target(w.get(), std::forward<Args>(args)...){}
	using d2d::device::context::get;
	using d2d::device::context::operator->;
	using d2d::device::context::operator bool;
	template<typename F>
	expected<void, hresult_error> draw(F&& f){
		using namespace std::literals::chrono_literals;
		DXGI_PRESENT_PARAMETERS param = {};
		if((status & DXGI_STATUS_OCCLUDED)){
			status = (*static_cast<dxgi::swap_chain*>(this))->Present1(1, DXGI_PRESENT_TEST, &param);
			sleep(1000ms/60/2);
			return make_unexpected<hresult_error>(_T(__FUNCTION__), status);
		}
		return d2d::device::context::draw(std::forward<F>(f)).bind([&]()->expected<void, hresult_error>{
			status = dxgi::swap_chain::get()->Present1(1, 0, &param);
			if(SUCCEEDED(status))
				return {};
			return make_unexpected<hresult_error>(_T("will::hwnd_render_target::draw"), status);
		});
	}
	d3d::device& get_d3d_device(){return *this;}
	const d3d::device& get_d3d_device()const{return *this;}
	dwrite& get_dwrite_factory(){return *this;}
	const dwrite& get_dwrite_factory()const{return *this;}
	expected<hwnd_render_target, hresult_error> recreate_render_target(){
		static_cast<d2d::device::context&>(*this).~context();
		static_cast<dxgi::swap_chain&>(*this).~swap_chain();
		return create_swap_chain(hwnd, std::move(*this)).bind([&](dxgi::swap_chain&& sc){
			return get_buffer().map([&](dxgi::surface&& sf){
				new (static_cast<dxgi::swap_chain*>(this)) dxgi::swap_chain(std::move(sc));
				new (static_cast<d2d::device::context*>(this)) d2d::device::context(std::move(sf));
				return std::move(*this);
			});
		});
	}
};
class gdi_compatible_render_target:protected d3d::device, public dxgi::surface, public d2d::device::context, public dwrite{
	static expected<dxgi::surface, hresult_error> create_surface(unsigned int w, unsigned int h, d3d::device&& dev){
		return dev.create_texture2d(will::d3d::texture2d::description{}.width(w).height(h).mip_levels(1).array_size(1).format(DXGI_FORMAT_B8G8R8A8_UNORM).sample_count(1).sample_quality(0).bind_flags(D3D11_BIND_RENDER_TARGET).misc_flags(D3D11_RESOURCE_MISC_GDI_COMPATIBLE)).bind([](d3d::texture2d&& tex){return dxgi::surface::create(std::move(tex));});
	}
	gdi_compatible_render_target(d3d::device&& dev, dxgi::surface&& surf, d2d::device::context&& devcont, dwrite&& dw):device{std::move(dev)}, surface{std::move(surf)}, d2d::device::context{std::move(devcont)}, dwrite{std::move(dw)}{}
public:
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<unsigned int>& wh, D3D11_CREATE_DEVICE_FLAG flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT, const D2D1_CREATION_PROPERTIES* prop = nullptr, DWRITE_FACTORY_TYPE type = DWRITE_FACTORY_TYPE_SHARED){
		return d3d::create_device(flag).bind([&](d3d::device&& d3d_dev){
			return create_surface(wh.w, wh.h, std::move(d3d_dev)).bind([&](dxgi::surface&& surf){
				return d2d::device::context::create(surf, prop).bind([&](d2d::device::context&& devcon){
					return dwrite::create_factory(type).map([&](dwrite&& dw){
						return gdi_compatible_render_target{std::move(d3d_dev), std::move(surf), std::move(devcon), std::move(dw)};
					});
				});
			});
		});
	}
	explicit gdi_compatible_render_target(const will::two_dim::wh<unsigned int>& wh) : gdi_compatible_render_target{+create(wh)}{}
	explicit gdi_compatible_render_target(const will::two_dim::wh<int>& wh) : gdi_compatible_render_target(will::two_dim::wh<unsigned int>{static_cast<unsigned int>(wh.w), static_cast<unsigned int>(wh.h)}){}
	gdi_compatible_render_target(const will::two_dim::wh<unsigned int>& wh, const D2D1_CREATION_PROPERTIES& prop) : gdi_compatible_render_target{+create(wh, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &prop)}{}
	gdi_compatible_render_target(const will::two_dim::wh<int>& wh, const D2D1_CREATION_PROPERTIES& prop) : gdi_compatible_render_target(will::two_dim::wh<unsigned int>{static_cast<unsigned int>(wh.w), static_cast<unsigned int>(wh.h)}, prop){}
	gdi_compatible_render_target(const will::two_dim::wh<unsigned int>& wh, D3D11_CREATE_DEVICE_FLAG flag, const D2D1_CREATION_PROPERTIES& prop) : gdi_compatible_render_target{+create(wh, flag, &prop)}{}
	gdi_compatible_render_target(const will::two_dim::wh<int>& wh, D3D11_CREATE_DEVICE_FLAG flag, const D2D1_CREATION_PROPERTIES& prop) : gdi_compatible_render_target(will::two_dim::wh<unsigned int>{static_cast<unsigned int>(wh.w), static_cast<unsigned int>(wh.h)}, flag, prop){}
	gdi_compatible_render_target(const will::two_dim::wh<unsigned int>& wh, D3D11_CREATE_DEVICE_FLAG flag, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type) : gdi_compatible_render_target{+create(wh, flag, &prop, type)}{}
	gdi_compatible_render_target(const will::two_dim::wh<int>& wh, D3D11_CREATE_DEVICE_FLAG flag, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type) : gdi_compatible_render_target(will::two_dim::wh<unsigned int>{static_cast<unsigned int>(wh.w), static_cast<unsigned int>(wh.h)}, flag, prop, type){}
	using d2d::device::context::get;
	using d2d::device::context::operator->;
	using d2d::device::context::operator bool;
	using d2d::device::context::draw;
	template<typename F>
	expected<d2d::bitmap, hresult_error> prerender(const D2D1_SIZE_F& desired_size, F&& f){
		return create_compatible_render_target(desired_size, D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE).bind([&](d2d::device::bitmap_render_target&& rt){
			return rt.draw(std::forward<F>(f)).bind([&]{
				return rt.get_bitmap();
			});
		});
	}
	template<typename F>
	expected<d2d::bitmap, hresult_error> prerender(const D2D1_SIZE_U& desired_pixel_size, F&& f){
		return create_compatible_render_target(desired_pixel_size, D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE).bind([&](d2d::device::bitmap_render_target&& rt){
			return rt.draw(std::forward<F>(f)).bind([&]{
				return rt.get_bitmap();
			});
		});
	}
	template<typename F>
	expected<d2d::bitmap, hresult_error> prerender(F&& f){
		return create_compatible_render_target(D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE).bind([&](d2d::device::bitmap_render_target&& rt){
			return rt.draw(std::forward<F>(f)).bind([&]{
				return rt.get_bitmap();
			});
		});
	}
	d3d::device& get_d3d_device(){return *this;}
	const d3d::device& get_d3d_device()const{return *this;}
	dwrite& get_dwrite_factory(){return *this;}
	const dwrite& get_dwrite_factory()const{return *this;}
};
}