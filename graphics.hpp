//Copyright (C) 2014-2018 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"d3d.hpp"
#include"dxgi.hpp"
#include"d2d.hpp"
#include"wic.hpp"
#include"dwrite.hpp"
#include"amp.hpp"
#include"amp_graphics.hpp"
#include<memory>
#include<vector>

namespace will{

namespace interop{

namespace detail{

template<typename, typename>struct memory_data2d;
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, will::d2d::bitmap::memory_data>{
	static will::d2d::bitmap::memory_data&& conv(will::d2d::bitmap::memory_data&& d){return std::move(d);}
	static const will::d2d::bitmap::memory_data& conv(const will::d2d::bitmap::memory_data& d){return d;}
};
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, will::d2d::bitmap::scoped_mapped_rect>{
	static will::d2d::bitmap::memory_data conv(will::d2d::bitmap::scoped_mapped_rect&& d){return {d.pitch, d.bits};}
	static const will::d2d::bitmap::memory_data& conv(const will::d2d::bitmap::scoped_mapped_rect& d){return d;}
};
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, will::d2d::bitmap::scoped_readonly_mapped_rect>{
	static will::d2d::bitmap::memory_data conv(will::d2d::bitmap::scoped_readonly_mapped_rect&& d){return {d.pitch, d.bits};}
	static const will::d2d::bitmap::memory_data& conv(const will::d2d::bitmap::scoped_readonly_mapped_rect& d){return d;}
};
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, will::d3d::device::subresource>{
	static will::d2d::bitmap::memory_data conv(const will::d3d::device::subresource& d){return {d.RowPitch, d.pData};}
};
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, will::d3d::device::context::scoped_subresource>{
	static will::d2d::bitmap::memory_data conv(const will::d3d::device::context::scoped_subresource& d){return {d.RowPitch, d.pData};}
};
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, ::D3D11_MAPPED_SUBRESOURCE>{
	static will::d2d::bitmap::memory_data conv(const ::D3D11_MAPPED_SUBRESOURCE& d){return {d.RowPitch, d.pData};}
};
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, ::D3D11_SUBRESOURCE_DATA>{
	static will::d2d::bitmap::memory_data conv(const ::D3D11_SUBRESOURCE_DATA& d){return {d.SysMemPitch, d.pSysMem};}
};

template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, will::d2d::bitmap::memory_data>{
	static ::D3D11_SUBRESOURCE_DATA conv(const will::d2d::bitmap::memory_data& d){return {d.data, d.pitch};}
};
template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, will::d2d::bitmap::scoped_mapped_rect>{
	static ::D3D11_SUBRESOURCE_DATA conv(const will::d2d::bitmap::scoped_mapped_rect& d){return {d.bits, d.pitch};}
};
template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, will::d2d::bitmap::scoped_readonly_mapped_rect>{
	static ::D3D11_SUBRESOURCE_DATA conv(const will::d2d::bitmap::scoped_readonly_mapped_rect& d){return {d.bits, d.pitch};}
};
template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, will::d3d::device::subresource>{
	static ::D3D11_SUBRESOURCE_DATA conv(will::d3d::device::subresource&& d){return {d.pData, d.RowPitch};}
	static const ::D3D11_SUBRESOURCE_DATA& conv(const will::d3d::device::subresource& d){return reinterpret_cast<const ::D3D11_SUBRESOURCE_DATA&>(d);}
};
template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, will::d3d::device::context::scoped_subresource>{
	static ::D3D11_SUBRESOURCE_DATA conv(will::d3d::device::context::scoped_subresource&& d){return {d.pData, d.RowPitch};}
	static const ::D3D11_SUBRESOURCE_DATA& conv(const will::d3d::device::context::scoped_subresource& d){return reinterpret_cast<const ::D3D11_SUBRESOURCE_DATA&>(d);}
};
template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, ::D3D11_MAPPED_SUBRESOURCE>{
	static ::D3D11_SUBRESOURCE_DATA conv(::D3D11_MAPPED_SUBRESOURCE&& d){return {d.pData, d.RowPitch};}
	static const ::D3D11_SUBRESOURCE_DATA& conv(const ::D3D11_MAPPED_SUBRESOURCE& d){return reinterpret_cast<const ::D3D11_SUBRESOURCE_DATA&>(d);}
};
template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, ::D3D11_SUBRESOURCE_DATA>{
	static ::D3D11_SUBRESOURCE_DATA&& conv(::D3D11_SUBRESOURCE_DATA&& d){return std::move(d);}
	static const ::D3D11_SUBRESOURCE_DATA& conv(const ::D3D11_SUBRESOURCE_DATA& d){return d;}
};

}

template<typename To, typename From>
inline decltype(auto) memory_data2d(From&& from){return detail::memory_data2d<To, std::decay_t<From>>::conv(std::forward<From>(from));}
template<typename From>
inline decltype(auto) memory_data2d(From&& from){return detail::memory_data2d<will::d2d::bitmap::memory_data, std::decay_t<From>>::conv(std::forward<From>(from));}

namespace detail{

template<typename, typename>struct reinterpret_conversion_impl;
template<>
struct reinterpret_conversion_impl<will::d2d::bitmap, will::d2d::bitmap>{
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::d2d::bitmap& bmp, const will::d2d::bitmap::property& prop, const will::d2d::device::context& devcont){
		if(bmp.get_pixel_format().format == prop.get().pixelFormat.format)
			return bmp.clone(devcont);
		if((bmp.get_bitmap_options() & D2D1_BITMAP_OPTIONS_CPU_READ) != 0){
			return bmp.scoped_map().bind([&](will::d2d::bitmap::scoped_readonly_mapped_rect&& rect){
				return devcont.create_bitmap(bmp.get_pixel_size(), rect);
			});
		}
		auto p = prop.get();
		p.bitmapOptions |= ::D2D1_BITMAP_OPTIONS_CPU_READ | ::D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
		return devcont.create_bitmap(bmp.get_pixel_size(), p).bind([&](will::d2d::bitmap&& t){
			return t.copy_from(bmp).bind([&](){
				return conv(t, prop, devcont);});});
	}
};
template<>
struct reinterpret_conversion_impl<will::d2d::bitmap, will::d3d::texture2d>{
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::d3d::texture2d& tex, const will::d2d::bitmap::property& prop, const will::d3d::device& dev, const will::d2d::device::context& devcont){
		auto desc = tex.get_desc();
		if(desc.Format == prop.get().pixelFormat.format)
			return devcont.create_bitmap({desc.Width, desc.Height}, prop).map([&](will::d2d::bitmap&& bmp){
				bmp.get_surface().map([&](will::dxgi::surface&& surf){
					dev.get_immediate_context().copy(will::d3d::texture2d{std::move(surf)}, tex);
				});
				return std::move(bmp);
			});
		if(desc.Usage == D3D11_USAGE_STAGING && (desc.CPUAccessFlags & D3D11_CPU_ACCESS_READ) != 0)
			return dev.get_immediate_context()
			          .create_scoped_subresource(tex, 0, D3D11_MAP_READ)
			          .bind([&](will::d3d::device::context::scoped_subresource&& sr){
				return devcont.create_bitmap({desc.Width, desc.Height}, will::interop::memory_data2d(std::move(sr)), prop);
			});
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.BindFlags = 0;
		return dev.create_texture2d(desc).bind([&](const will::d3d::texture2d& t){
			dev.get_immediate_context().copy(t, tex);
			return conv(t, prop, dev, devcont);
		});
	}
	template<typename RenderTarget>
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::d3d::texture2d& tex, const will::d2d::bitmap::property& prop, RenderTarget&& rt){return conv(tex, prop, rt.get_d3d_device(), rt);}
};
template<>
struct reinterpret_conversion_impl<will::d3d::texture2d, will::d3d::texture2d>{
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d3d::texture2d& tex, will::d3d::texture2d::description desc, const will::d3d::device& dev){
		auto d = tex.get_desc();
		if(desc.get().Width == 0)
			desc.width(d.Width);
		if(desc.get().Height == 0)
			desc.height(d.Height);
		if(desc.get().Format == tex.get_desc().Format)
			return dev.create_texture2d(desc).map([&](will::d3d::texture2d&& t){
				dev.get_immediate_context().copy(t, tex);
				return std::move(t);
			});
		if(tex.get_desc().Usage == D3D11_USAGE_STAGING && (tex.get_desc().CPUAccessFlags & D3D11_CPU_ACCESS_READ) != 0)
			return dev.get_immediate_context()
			          .create_scoped_subresource(tex, 0, D3D11_MAP_READ)
			          .bind([&](will::d3d::device::context::scoped_subresource&& sr){
				return dev.create_texture2d(desc, will::interop::memory_data2d<::D3D11_SUBRESOURCE_DATA>(std::move(sr)));
			});
		d.Usage = D3D11_USAGE_STAGING;
		d.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		d.BindFlags = 0;
		return dev.create_texture2d(d).bind([&](will::d3d::texture2d&& t){
			dev.get_immediate_context().copy(t, tex);
			return conv(t, desc, dev);
		});
	}
	template<typename RenderTarget>
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d3d::texture2d& tex, will::d3d::texture2d::description desc, RenderTarget&& rt){return conv(tex, desc, rt.get_d3d_device());}
};
template<>
struct reinterpret_conversion_impl<will::d3d::texture2d, will::d2d::bitmap>{
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d2d::bitmap& bmp, will::d3d::texture2d::description desc, const will::d3d::device& dev, const will::d2d::device::context& devcont){
		{
			const auto p = bmp.get_pixel_size();
			if(desc.get().Width == 0)
				desc.width(p.width);
			if(desc.get().Height == 0)
				desc.height(p.height);
		}
		if(desc.get().Format == bmp.get_pixel_format().format)
			return dev.create_texture2d(desc).map([&](will::d3d::texture2d&& tex){
				bmp.get_surface().map([&](will::dxgi::surface&& surf){
					dev.get_immediate_context().copy(tex, will::d3d::texture2d{std::move(surf)});
				});
				return std::move(tex);
			});
		if((bmp.get_bitmap_options() & D2D1_BITMAP_OPTIONS_CPU_READ) != 0)
			return bmp.scoped_map()
				.bind([&](will::d2d::bitmap::scoped_readonly_mapped_rect&& sr){
				return dev.create_texture2d(desc, will::interop::memory_data2d<::D3D11_SUBRESOURCE_DATA>(sr));
			});
		const auto dpi = bmp.get_dpi();
		return devcont.create_bitmap(bmp.get_pixel_size(), D2D1::BitmapProperties1(::D2D1_BITMAP_OPTIONS_CPU_READ | ::D2D1_BITMAP_OPTIONS_CANNOT_DRAW, bmp.get_pixel_format(), dpi.x, dpi.y, bmp.get_color_context())).bind([&](will::d2d::bitmap&& t){
			return t.copy_from(bmp).bind([&](){
				return conv(t, desc, dev, devcont);});});
	}
	template<typename RenderTarget>
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d2d::bitmap& bmp, const will::d3d::texture2d::description& desc, RenderTarget&& rt){return conv(bmp, desc, rt.get_d3d_device(), rt);}
};
template<typename ValueType, int Rank>
struct reinterpret_conversion_impl<will::d2d::bitmap, concurrency::graphics::texture_view<ValueType, Rank>>{
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::amp::graphics::texture_view<ValueType, Rank>& texv, const will::d2d::bitmap::property& prop, const will::d3d::device& dev, const will::d2d::device::context& devcont){
		return reinterpret_conversion_impl<will::d2d::bitmap, will::d3d::texture2d>::conv(will::amp::graphics::direct3d::get_texture(texv), prop, dev, devcont);
	}
	template<typename RenderTarget>
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::amp::graphics::texture_view<ValueType, Rank>& texv, const will::d2d::bitmap::property& prop, RenderTarget&& rt){return conv(texv, prop, rt.get_d3d_device(), rt);}
};
template<typename ValueType, int Rank>
struct reinterpret_conversion_impl<concurrency::graphics::texture<ValueType, Rank>, will::d2d::bitmap>{
	static will::expected<will::amp::graphics::texture<ValueType, Rank>, hresult_error> conv(const will::d2d::bitmap& bmp, const will::d3d::device& dev, const will::d2d::device::context& devcont, const will::amp::accelerator_view& accv){
		constexpr auto fn = _T(__FUNCTION__);
		return reinterpret_conversion_impl<will::d3d::texture2d, will::d2d::bitmap>::conv(bmp, will::d3d::texture2d::description{}.format(DXGI_FORMAT_R8G8B8A8_UNORM).bind_flags(D3D11_BIND_UNORDERED_ACCESS|D3D11_BIND_SHADER_RESOURCE), dev, devcont).bind([&](will::d3d::texture2d&& tex){
			return will::amp::graphics::direct3d::make_texture<will::amp::graphics::unorm_4, 2>(accv, tex)
			.emap([&](std::exception_ptr&& ptr){try{std::rethrow_exception(ptr);}catch(concurrency::runtime_exception& e){return make_unexpected<hresult_error>(fn, e.get_error_code());}});});
	}
	template<typename RenderTarget>
	static will::expected<will::amp::graphics::texture<ValueType, Rank>, hresult_error> conv(const will::d2d::bitmap& bmp, RenderTarget&& rt){return conv(bmp, rt.get_d3d_device(), rt, rt.get_accelerator_view());}
};

}

template<typename To, typename From, typename... Args>
inline expected<To, hresult_error> reinterpret_convert(From&& from, Args&&... args){return detail::reinterpret_conversion_impl<To, std::decay_t<From>>::conv(std::forward<From>(from), std::forward<Args>(args)...);}

}

class hwnd_render_target:protected d3d::device, protected dxgi::swap_chain, public d2d::device::context, public dwrite{
	static expected<dxgi::swap_chain, hresult_error> create_swap_chain(HWND hwnd, d3d::device& dev, const DXGI_SWAP_CHAIN_DESC1& desc = will::dxgi::swap_chain::description{}.format(DXGI_FORMAT_B8G8R8A8_UNORM)){
		return dxgi::device::create(dev).bind([&](dxgi::device&& dev2){
			return dev2.get_factory().bind([&](dxgi&& dxgi_fac){
				return dxgi_fac.create_swap_chain(dev, hwnd, desc).bind([&](dxgi::swap_chain&& sc){
					return dev2.set_maximum_frame_latency(1).map([&]()->dxgi::swap_chain{
						return std::move(sc);
					});
				});
			});
		});
	}
	HWND hwnd;
	HRESULT status;
	amp::accelerator_view av;
	hwnd_render_target(d3d::device&& dev, dxgi::swap_chain&& sc, d2d::device::context&& devcon, dwrite&& dw, HWND hw, amp::accelerator_view&& accv) : device{std::move(dev)}, swap_chain{std::move(sc)}, d2d::device::context{std::move(devcon)}, dwrite{std::move(dw)}, hwnd(std::move(hw)), status(0ul), av{std::move(accv)}{}
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const DXGI_SWAP_CHAIN_DESC1& desc, const D2D1_CREATION_PROPERTIES* prop, DWRITE_FACTORY_TYPE type, will::amp::queuing_mode qm){
		return d3d::device::create(flag).bind([&](d3d::device&& d3d_dev){
			return create_swap_chain(hwnd, d3d_dev, desc).bind([&](dxgi::swap_chain&& sc){
				return sc.get_buffer().bind([&](dxgi::surface&& surf){
					return d2d::device::context::create(surf, prop).bind([&](d2d::device::context&& devcon){
						return dwrite::create_factory(type).bind([&](dwrite&& dw){
							return amp::direct3d::create_accelerator_view(d3d_dev, qm)
								.emap([](std::exception_ptr&& ptr){try{std::rethrow_exception(ptr);}catch(concurrency::runtime_exception& e){return make_unexpected<hresult_error>(_T("will::hwnd_render_target::create"), e.get_error_code());}}).map([&](amp::accelerator_view&& av){
								return hwnd_render_target{std::move(d3d_dev), std::move(sc), std::move(devcon), std::move(dw), hwnd, std::move(av)};
							});
						});
					});
				});
			});
		});
	}
public:
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd, const D2D1_CREATION_PROPERTIES& prop){return create(hwnd, D3D11_CREATE_DEVICE_BGRA_SUPPORT, will::dxgi::swap_chain::description{}.format(DXGI_FORMAT_B8G8R8A8_UNORM), &prop, DWRITE_FACTORY_TYPE_SHARED, will::amp::queuing_mode_automatic);}
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd,
#ifdef _DEBUG
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_WARNING, D2D1_DEVICE_CONTEXT_OPTIONS_NONE}
#else
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS}
#endif
	){return create(hwnd, flag, will::dxgi::swap_chain::description{}.format(DXGI_FORMAT_B8G8R8A8_UNORM), &prop, DWRITE_FACTORY_TYPE_SHARED, will::amp::queuing_mode_automatic);}
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const DXGI_SWAP_CHAIN_DESC1& desc,
#ifdef _DEBUG
		const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_WARNING, D2D1_DEVICE_CONTEXT_OPTIONS_NONE}
#else
		const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS}
#endif
	){return create(hwnd, flag, desc, &prop, DWRITE_FACTORY_TYPE_SHARED, will::amp::queuing_mode_automatic);}
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const DXGI_SWAP_CHAIN_DESC1& desc, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type){return create(hwnd, flag, desc, &prop, type, will::amp::queuing_mode_automatic);}
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const DXGI_SWAP_CHAIN_DESC1& desc, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type, amp::queuing_mode qm){return create(hwnd, flag, desc, &prop, type, qm);}
	template<typename Window, typename... Args, std::enable_if_t<!std::is_same<std::decay_t<Window>, HWND>::value>* = nullptr>
	static expected<hwnd_render_target, hresult_error> create(Window&& w, Args&&... args){return create(w.get(), std::forward<Args>(args)...);}
	hwnd_render_target(HWND hwnd, const D2D1_CREATION_PROPERTIES& prop):hwnd_render_target{+create(hwnd, prop)}{}
	explicit hwnd_render_target(HWND hwnd,
#ifdef _DEBUG
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_WARNING, D2D1_DEVICE_CONTEXT_OPTIONS_NONE}
#else
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS}
#endif
	):hwnd_render_target{+create(hwnd, flag, prop)}{}
	explicit hwnd_render_target(HWND hwnd, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const DXGI_SWAP_CHAIN_DESC1& desc,
#ifdef _DEBUG
		const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_WARNING, D2D1_DEVICE_CONTEXT_OPTIONS_NONE}
#else
		const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS}
#endif
	):hwnd_render_target{+create(hwnd, flag, desc, prop)}{}
	hwnd_render_target(HWND hwnd, D3D11_CREATE_DEVICE_FLAG flag, const DXGI_SWAP_CHAIN_DESC1& desc, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type):hwnd_render_target{+create(hwnd, flag, desc, prop, type)}{}
	hwnd_render_target(HWND hwnd, D3D11_CREATE_DEVICE_FLAG flag, const DXGI_SWAP_CHAIN_DESC1& desc, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type, amp::queuing_mode qm):hwnd_render_target{+create(hwnd, flag, desc, prop, type, qm)}{}
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
	amp::accelerator_view& get_accelerator_view(){return av;}
	const amp::accelerator_view& get_accelerator_view()const{return av;}
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
	template<typename To, typename From, typename... Args>
	expected<To, hresult_error> reinterpret_convert(From&& from, Args&&... args)const{return interop::reinterpret_convert<To>(std::forward<From>(from), std::forward<Args>(args)..., *this);}
};
class gdi_compatible_render_target:protected d3d::device, public dxgi::surface, public d2d::device::context, public dwrite{
	static expected<dxgi::surface, hresult_error> create_surface(unsigned int w, unsigned int h, d3d::device&& dev){
		return dev.create_texture2d(will::d3d::texture2d::description{}.width(w).height(h).mip_levels(1).array_size(1).format(DXGI_FORMAT_B8G8R8A8_UNORM).sample_count(1).sample_quality(0).bind_flags(D3D11_BIND_RENDER_TARGET).misc_flags(D3D11_RESOURCE_MISC_GDI_COMPATIBLE)).bind([](d3d::texture2d&& tex){return dxgi::surface::create(std::move(tex));});
	}
	gdi_compatible_render_target(d3d::device&& dev, dxgi::surface&& surf, d2d::device::context&& devcont, dwrite&& dw, amp::accelerator_view&& accv):device{std::move(dev)}, surface{std::move(surf)}, d2d::device::context{std::move(devcont)}, dwrite{std::move(dw)}, av{std::move(accv)}{}
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<unsigned int>& wh, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const D2D1_CREATION_PROPERTIES* prop, DWRITE_FACTORY_TYPE type, amp::queuing_mode qm){
		return d3d::device::create(flag).bind([&](d3d::device&& d3d_dev){
			return create_surface(wh.w, wh.h, std::move(d3d_dev)).bind([&](dxgi::surface&& surf){
				return d2d::device::context::create(surf, prop).bind([&](d2d::device::context&& devcon){
					return dwrite::create_factory(type).bind([&](dwrite&& dw){
						return amp::direct3d::create_accelerator_view(d3d_dev, qm)
							.emap([](std::exception_ptr&& ptr){try{std::rethrow_exception(ptr);}catch(concurrency::runtime_exception& e){return make_unexpected<hresult_error>(_T("will::gdi_compatible_render_target::create"), e.get_error_code());}}).map([&](amp::accelerator_view& av){
							return gdi_compatible_render_target{std::move(d3d_dev), std::move(surf), std::move(devcon), std::move(dw), std::move(av)};
						});
					});
				});
			});
		});
	}
	amp::accelerator_view av;
public:
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<unsigned int>& wh, const D2D1_CREATION_PROPERTIES& prop){return create(wh, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &prop, DWRITE_FACTORY_TYPE_SHARED, amp::queuing_mode_automatic);}
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<unsigned int>& wh,
#ifdef _DEBUG
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_WARNING, D2D1_DEVICE_CONTEXT_OPTIONS_NONE}
#else
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS}
#endif
	){return create(wh, flag, &prop, DWRITE_FACTORY_TYPE_SHARED, amp::queuing_mode_automatic);}
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<unsigned int>& wh, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type){return create(wh, flag, &prop, type, amp::queuing_mode_automatic);}
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<unsigned int>& wh, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type, amp::queuing_mode qm){return create(wh, flag, &prop, type, qm);}
	template<typename T, typename... Args, std::enable_if_t<!std::is_same<T, unsigned int>::value, std::nullptr_t> = nullptr>
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<T>& wh, Args&&... args){return create(wh.cast<unsigned int>(), std::forward<Args>(args)...);}
	gdi_compatible_render_target(const will::two_dim::wh<unsigned int>& wh, const D2D1_CREATION_PROPERTIES& prop) : gdi_compatible_render_target{+create(wh, D3D11_CREATE_DEVICE_BGRA_SUPPORT, prop)}{}
	explicit gdi_compatible_render_target(const will::two_dim::wh<unsigned int>& wh,
#ifdef _DEBUG
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_WARNING, D2D1_DEVICE_CONTEXT_OPTIONS_NONE}
#else
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS}
#endif
	) : gdi_compatible_render_target{+create(wh, flag, prop)}{}
	gdi_compatible_render_target(const will::two_dim::wh<unsigned int>& wh, D3D11_CREATE_DEVICE_FLAG flag, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type) : gdi_compatible_render_target{+create(wh, flag, prop, type)}{}
	gdi_compatible_render_target(const will::two_dim::wh<unsigned int>& wh, D3D11_CREATE_DEVICE_FLAG flag, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type, amp::queuing_mode qm) : gdi_compatible_render_target{+create(wh, flag, prop, type, qm)}{}
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
	amp::accelerator_view& get_accelerator_view(){return av;}
	const amp::accelerator_view& get_accelerator_view()const{return av;}
	template<typename To, typename From, typename... Args>
	expected<To, hresult_error> reinterpret_convert(From&& from, Args&&... args)const{return interop::reinterpret_convert<To>(std::forward<From>(from), std::forward<Args>(args)..., *this);}
};
}