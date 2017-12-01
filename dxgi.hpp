//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"com.hpp"
#include"_resource_property.hpp"
#include<d3d11_1.h>
#include<dxgi1_2.h>
#pragma comment(lib, "Dxgi.lib")
namespace will{
class dxgi : public detail::resource<IDXGIFactory2>{
	using resource::resource;
	template<typename I>
	struct dxgi_resource : detail::resource<I>{
		using resource::resource;
	protected:
		template<typename T>
		expected<T, hresult_error> get_parent(const TCHAR* func_name)const{return detail::convert_to_rich_interface<T>(com_create_resource<detail::get_interface<T>>([&](detail::get_interface<T>** ptr){return (*this)->GetParent(__uuidof(detail::get_interface<T>), reinterpret_cast<void**>(ptr));}), func_name);}
	};
public:
	class adapter : public dxgi_resource<IDXGIAdapter2>{
		using dxgi_resource::dxgi_resource;
	public:
		expected<dxgi, hresult_error> get_factory(){return get_parent<dxgi>(_T(__FUNCTION__));}
		class device : public detail::resource<IDXGIDevice2>{
			using resource::resource;
			template<typename I>
			struct dxgi_device_resource : detail::resource<I>{
				using resource::resource;
			protected:
				template<typename T = device>
				expected<T, hresult_error> get_device_impl(const TCHAR* func_name)const{return detail::convert_to_rich_interface<T>(com_create_resource<detail::get_interface<T>>([&](detail::get_interface<T>** ptr){return (*this)->GetDevice(__uuidof(detail::get_interface<T>), reinterpret_cast<void**>(ptr));}), func_name);}
			};
		public:
			class surface : public dxgi_device_resource<IDXGISurface2>{
				class device_context_handle{
					const surface* srf;
					HDC dc;
					device_context_handle(const surface& s, HDC d)noexcept:srf{&s}, dc{d}{}
					expected<void, hresult_error> release(RECT* r)noexcept{
						if(dc == nullptr)
							return {};
						auto hr = (*srf)->ReleaseDC(r);
						if(SUCCEEDED(hr)){
							dc = nullptr;
							return {};
						}
						return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
					}
				public:
					device_context_handle(const surface& s, BOOL discard = FALSE):srf(&s), dc(+s.get_dc(discard)){}
					device_context_handle(const device_context_handle&) = delete;
					device_context_handle(device_context_handle&& other)noexcept:srf(other.srf), dc(other.dc){other.dc = nullptr;}
					device_context_handle& operator=(const device_context_handle&) = delete;
					device_context_handle& operator=(device_context_handle&& rhs)noexcept{srf = rhs.srf; dc = rhs.dc;rhs.dc = nullptr;}
					expected<void, hresult_error> release(RECT& r)noexcept{return release(&r);}
					expected<void, hresult_error> release()noexcept{return release(nullptr);}
					~device_context_handle()noexcept{release();}
					operator HDC()const noexcept{return dc;}
					friend surface;
				};
			public:
				struct description : detail::property<::DXGI_SURFACE_DESC>{
					description():property{}{prop.SampleDesc.Count = 1u;}
			#define PROPERTYDECL(name, type, membername) description& name(type t){prop.membername = t;return *this;}
					PROPERTYDECL(width,          UINT,             Width)
					PROPERTYDECL(height,         UINT,             Height)
					PROPERTYDECL(format,         DXGI_FORMAT,      Format)
					PROPERTYDECL(sample,         DXGI_SAMPLE_DESC, SampleDesc)
					PROPERTYDECL(sample_count,   UINT,             SampleDesc.Count)
					PROPERTYDECL(sample_quality, UINT,             SampleDesc.Quality)
			#undef  PROPERTYDECL
				};
				class scoped_readonly_mapped_rect : public ::DXGI_MAPPED_RECT{
					const surface& sf;
					scoped_readonly_mapped_rect(::DXGI_MAPPED_RECT&& mr, const surface& surf)noexcept: ::DXGI_MAPPED_RECT{std::move(mr)}, sf{surf}{}
				public:
					scoped_readonly_mapped_rect(const surface& surf): ::DXGI_MAPPED_RECT(+surf.map_read()), sf{surf}{}
					scoped_readonly_mapped_rect(scoped_readonly_mapped_rect&& t): ::DXGI_MAPPED_RECT{std::move(static_cast<::DXGI_MAPPED_RECT&>(t))}, sf{t.sf}{t.pBits = nullptr;}
					~scoped_readonly_mapped_rect()noexcept{if(pBits)sf.unmap();}
					friend surface;
				};
				class scoped_mapped_rect : public ::DXGI_MAPPED_RECT{
					surface& sf;
					scoped_mapped_rect(::DXGI_MAPPED_RECT&& s, surface& surf)noexcept: ::DXGI_MAPPED_RECT{std::move(s)}, sf{surf}{}
				public:
					scoped_mapped_rect(surface& surf, UINT option): ::DXGI_MAPPED_RECT(+surf.map(option)), sf{surf}{}
					scoped_mapped_rect(scoped_mapped_rect&& t): ::DXGI_MAPPED_RECT{std::move(static_cast<::DXGI_MAPPED_RECT&>(t))}, sf{t.sf}{t.pBits = nullptr;}
					~scoped_mapped_rect()noexcept{if(pBits)sf.unmap();}
					friend surface;
				};
				using dxgi_device_resource::dxgi_device_resource;
				expected<device, hresult_error> get_device()const{return get_device_impl(_T(__FUNCTION__));}
				static expected<surface, hresult_error> create(ID3D11Texture2D*&& ptr){return detail::convert_to_rich_interface<surface>(query_interface<IDXGISurface2>(ptr), _T(__FUNCTION__));}
				template<typename Surface, typename std::enable_if<!std::is_same<typename std::decay<Surface>::type, surface>::value>::type* = nullptr>
				static expected<surface, hresult_error> create(Surface&& srf){return create(std::forward<Surface>(srf).get());}
				explicit surface(ID3D11Texture2D*&& ptr):surface(+create(std::move(ptr))){}
				template<typename Surface, typename std::enable_if<!std::is_same<typename std::decay<Surface>::type, surface>::value>::type* = nullptr>
				explicit surface(Surface&& sf):surface(+create(std::forward<Surface>(sf).get())){}
				expected<device_context_handle, hresult_error> get_dc(BOOL discard = FALSE)const{
					return will::com_create_resource<std::remove_pointer_t<HDC>>([&](HDC* t){return (*this)->GetDC(discard, t);})
					         .map([&](HDC dc){return device_context_handle{*this, dc};})
					         .emap([](HRESULT hr){return make_unexpected<hresult_error>(_T("will::dxgi::device::surface::get_dc"), hr);});
				}
				expected<::DXGI_SURFACE_DESC, hresult_error> get_desc()const{::DXGI_SURFACE_DESC desc;const auto hr = (*this)->GetDesc(&desc);if(FAILED(hr))return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);return desc;}
				expected<::DXGI_MAPPED_RECT, hresult_error> map(UINT option){::DXGI_MAPPED_RECT rect;const auto hr = (*this)->Map(&rect, option);if(FAILED(hr))return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);return rect;}
				expected<::DXGI_MAPPED_RECT, hresult_error> map_read()const{::DXGI_MAPPED_RECT rect;const auto hr = (*this)->Map(&rect, DXGI_MAP_READ);if(FAILED(hr))return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);return rect;}
				expected<void, hresult_error> unmap()const{const auto hr = (*this)->Unmap();if(FAILED(hr))return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);return {};}
				expected<scoped_readonly_mapped_rect, hresult_error> scoped_map()const{return map_read().map([&](::DXGI_MAPPED_RECT&& m){return scoped_readonly_mapped_rect{std::move(m), *this};});}
				expected<scoped_mapped_rect, hresult_error> scoped_map(UINT option){return map(option).map([&](::DXGI_MAPPED_RECT&& m){return scoped_mapped_rect{std::move(m), *this};});}
			};
			static expected<device, hresult_error> create(ID3D11Device1* ptr){return detail::convert_to_rich_interface<device>(query_interface<IDXGIDevice2>(ptr), _T(__FUNCTION__));}
			template<typename Device>
			static expected<device, hresult_error> create(Device&& dev){return create(std::forward<Device>(dev).get());}
			explicit device(ID3D11Device1* ptr):device(+create(ptr)){}
			template<typename Device>
			explicit device(Device&& dev):device(+create(std::forward<Device>(dev))){}
			expected<adapter, hresult_error> get_adapter()const{
				return com_create_resource<IDXGIAdapter>([&](IDXGIAdapter** ptr){return (*this)->GetAdapter(ptr);})
				         .map([](IDXGIAdapter* t){return adapter{*com_ptr<IDXGIAdapter>{std::move(t)}.as<IDXGIAdapter2>()};})
				         .emap([](HRESULT e){return make_unexpected<hresult_error>(_T("will::dxgi::device::get_adapter"), e);});
			}
			expected<dxgi, hresult_error> get_factory()const{return get_adapter().bind([](adapter&& a){return a.get_factory();});}
			int get_gpu_thread_priority()const{int t;(*this)->GetGPUThreadPriority(&t);return t;}
			expected<void, hresult_error> set_gpu_thread_priority(int priority){const auto hr = (*this)->SetGPUThreadPriority(priority);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
			__declspec(property(get=get_gpu_thread_priority, put=set_gpu_thread_priority)) int gpu_thread_priority;
			expected<UINT, hresult_error> get_maximum_frame_latency(){UINT latency;const auto hr = (*this)->GetMaximumFrameLatency(&latency);if(SUCCEEDED(hr))return latency;return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
			expected<void, hresult_error> set_maximum_frame_latency(UINT latency = 3){const auto hr = (*this)->SetMaximumFrameLatency(latency);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		private:
			expected<surface, hresult_error> _create_surface_impl(const DXGI_SURFACE_DESC* desc, UINT num_surfaces, DXGI_USAGE usage, const DXGI_SHARED_RESOURCE* shared_resource)const{
				return com_create_resource<IDXGISurface>([&](IDXGISurface** ptr){return (*this)->CreateSurface(desc, num_surfaces, usage, shared_resource, ptr);})
				         .map([](IDXGISurface* suf){return surface{*com_ptr<IDXGISurface>{std::move(suf)}.as<IDXGISurface2>()};})
				         .emap([](HRESULT hr){return make_unexpected<hresult_error>(_T("will::dxgi::device::create_surface"), hr);});
			}
		public:
			expected<surface, hresult_error> create_surface(const DXGI_SURFACE_DESC& desc, UINT num_surfaces, DXGI_USAGE usage, const DXGI_SHARED_RESOURCE& shared_resource)const{return _create_surface_impl(&desc, num_surfaces, usage, &shared_resource);}
			expected<surface, hresult_error> create_surface(const DXGI_SURFACE_DESC& desc, UINT num_surfaces, DXGI_USAGE usage)const{return _create_surface_impl(&desc, num_surfaces, usage, nullptr);}
		};
	};
	using device = adapter::device;
	using surface = device::surface;
	struct output : dxgi_resource<IDXGIOutput1>{
		using dxgi_resource::dxgi_resource;
	};
	struct mode_description : detail::property<::DXGI_MODE_DESC>{
		mode_description():property(){}
#define PROPERTYDECL(name, type, membername) mode_description& name(type t){prop.membername = t;return *this;}
		PROPERTYDECL(width, UINT, Width)
		PROPERTYDECL(height, UINT, Height)
		PROPERTYDECL(refresh_rate, ::DXGI_RATIONAL, RefreshRate)
		PROPERTYDECL(format, DXGI_FORMAT, Format)
		PROPERTYDECL(scanline_order, DXGI_MODE_SCANLINE_ORDER, ScanlineOrdering)
		PROPERTYDECL(scaling, DXGI_MODE_SCALING, Scaling)
#undef  PROPERTYDECL
	};
	struct swap_chain : dxgi_resource<IDXGISwapChain1>{
		struct description : detail::property<DXGI_SWAP_CHAIN_DESC1>{
			description():property(){prop.Format = DXGI_FORMAT_B8G8R8A8_UNORM; prop.SampleDesc.Count = 1; prop.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; prop.BufferCount = 2;}
	#define PROPERTYDECL(name, type, membername) description& name(type t){prop.membername = t;return *this;}
			PROPERTYDECL(width, UINT, Width)
			PROPERTYDECL(height, UINT, Height)
			PROPERTYDECL(format, DXGI_FORMAT, Format)
			PROPERTYDECL(stereo, bool, Stereo)
			PROPERTYDECL(sample_count, UINT, SampleDesc.Count)
			PROPERTYDECL(sample_quality, UINT, SampleDesc.Quality)
			PROPERTYDECL(buffer_usage, DXGI_USAGE, BufferUsage)
			PROPERTYDECL(buffer_count, UINT, BufferCount)
			PROPERTYDECL(scaling, DXGI_SCALING, Scaling)
			PROPERTYDECL(swap_effect, DXGI_SWAP_EFFECT, SwapEffect)
			PROPERTYDECL(alpha_mode, DXGI_ALPHA_MODE, AlphaMode)
			PROPERTYDECL(flags, DXGI_SWAP_CHAIN_FLAG, Flags)
	#undef  PROPERTYDECL
		};
		using dxgi_resource::dxgi_resource;
		expected<dxgi, hresult_error> get_factory(){return get_parent<dxgi>(_T(__FUNCTION__));}
		expected<surface, hresult_error> get_buffer(UINT buffer_index = 0)const{
			return detail::convert_to_rich_interface<surface>(com_create_resource<IDXGISurface2>([&](IDXGISurface2** ptr){return (*this)->GetBuffer(buffer_index, __uuidof(IDXGISurface2), reinterpret_cast<void**>(ptr));}), _T(__FUNCTION__));
		}
		expected<output, hresult_error> get_containing_output()const{
			return detail::convert_to_rich_interface<output>(com_create_resource<IDXGIOutput>([&](IDXGIOutput** ptr){return (*this)->GetContainingOutput(ptr);}).map([](IDXGIOutput* ptr){auto ret =* query_interface<IDXGIOutput1>(ptr);ptr->Release();return ret;}), _T(__FUNCTION__));
		}
		expected<::DXGI_FRAME_STATISTICS, hresult_error> get_frame_statistics()const{
			::DXGI_FRAME_STATISTICS fs;
			auto hr = (*this)->GetFrameStatistics(&fs);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return fs;
		}
		expected<::UINT, hresult_error> get_last_present_count()const{
			::UINT lpc;
			auto hr = (*this)->GetLastPresentCount(&lpc);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return lpc;
		}
		expected<void, hresult_error> resize_buffers(::UINT buffer_count = 0, ::UINT width = 0, ::UINT height = 0, ::DXGI_FORMAT new_format = DXGI_FORMAT_UNKNOWN, std::underlying_type_t<::DXGI_SWAP_CHAIN_FLAG> flags = 0){
			auto hr = (*this)->ResizeBuffers(buffer_count, width, height, new_format, flags);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return {};
		}
		expected<void, hresult_error> resize_target(const ::DXGI_MODE_DESC& new_target_params){
			auto hr = (*this)->ResizeTarget(&new_target_params);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return {};
		}
		expected<::DXGI_RGBA, hresult_error> get_background_color()const{
			::DXGI_RGBA col;
			auto hr = (*this)->GetBackgroundColor(&col);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return col;
		}
		expected<void, hresult_error> set_background_color(const ::DXGI_RGBA& col){
			auto hr = (*this)->SetBackgroundColor(&col);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return {};
		}
		expected<::DXGI_SWAP_CHAIN_DESC1, hresult_error> get_desc()const{
			::DXGI_SWAP_CHAIN_DESC1 desc;
			auto hr = (*this)->GetDesc1(&desc);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return desc;
		}
		expected<::DXGI_MODE_ROTATION, hresult_error> get_rotation()const{
			::DXGI_MODE_ROTATION mode;
			auto hr = (*this)->GetRotation(&mode);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return mode;
		}
		expected<void, hresult_error> set_rotation(::DXGI_MODE_ROTATION mode){
			auto hr = (*this)->SetRotation(mode);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return {};
		}
	};
	expected<swap_chain, hresult_error> create_swap_chain(ID3D11Device* device, HWND hwnd, const DXGI_SWAP_CHAIN_DESC1& description = swap_chain::description{}.get())const{
		return detail::convert_to_rich_interface<swap_chain>(com_create_resource<IDXGISwapChain1>([&](IDXGISwapChain1** ptr){return (*this)->CreateSwapChainForHwnd(device, hwnd, &description, nullptr, nullptr, ptr);}), _T(__FUNCTION__));
	}
	template<typename Device, std::enable_if_t<!std::is_base_of<ID3D11Device, std::remove_pointer_t<std::decay_t<Device>>>::value>* = nullptr>
	expected<swap_chain, hresult_error> create_swap_chain(Device&& device, HWND hwnd, const DXGI_SWAP_CHAIN_DESC1& description = swap_chain::description{})const{
		return create_swap_chain(std::forward<Device>(device).get(), hwnd, description);
	}
	template<typename HWnd, std::enable_if_t<!std::is_same<std::decay_t<HWnd>, HWND>::value>* = nullptr>
	expected<swap_chain, hresult_error> create_swap_chain(ID3D11Device* device, HWnd&& hwnd, const DXGI_SWAP_CHAIN_DESC1& description = swap_chain::description{})const{
		return create_swap_chain(device, std::forward<Device>(hwnd).get(), description);
	}
	template<typename Device, typename HWnd, std::enable_if_t<!std::is_base_of<ID3D11Device, std::remove_pointer_t<std::decay_t<Device>>>::value && !std::is_same<std::decay_t<HWnd>, HWND>::value>* = nullptr>
	expected<swap_chain, hresult_error> create_swap_chain(Device&& device, HWnd&& hwnd, const DXGI_SWAP_CHAIN_DESC1& description = swap_chain::description{})const{
		return create_swap_chain(std::forward<Device>(device).get(), std::forward<Device>(hwnd).get(), description);
	}
	static expected<dxgi, hresult_error> create_factory(){
		return detail::convert_to_rich_interface<dxgi>(com_create_resource<IDXGIFactory2>([](IDXGIFactory2** ptr){return CreateDXGIFactory1(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(ptr));}), _T(__FUNCTION__));
	}
};

}