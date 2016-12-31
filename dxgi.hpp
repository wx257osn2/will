#pragma once
#include"com.hpp"
#include"_resource_property.hpp"
#include<d3d11.h>
#include<dxgi1_2.h>
#pragma comment(lib, "Dxgi.lib")
namespace will{
class dxgi : public detail::resource<IDXGIFactory2>{
	using resource::resource;
public:
	class surface : public detail::resource<IDXGISurface2>{
		class device_context_handle{
			const surface* srf;
			HDC dc;
			device_context_handle(const surface& s, HDC d)noexcept:srf{&s}, dc{d}{}
			expected<void, hresult_error> release(RECT* r)noexcept{
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
			~device_context_handle()noexcept{if(dc)release();}
			operator HDC()const noexcept{return dc;}
			friend surface;
		};
	public:
		using resource::resource;
		static expected<surface, hresult_error> create(ID3D11Texture2D* ptr){return detail::convert_to_rich_interface<surface>(query_interface<IDXGISurface2>(ptr), _T(__FUNCTION__));}
		template<typename Surface, typename std::enable_if<!std::is_same<typename std::decay<Surface>::type, surface>::value>::type* = nullptr>
		static expected<surface, hresult_error> create(Surface&& srf){return create(std::forward<Surface>(srf).get());}
		explicit surface(ID3D11Texture2D* ptr):surface(+create(ptr)){}
		template<typename Surface, typename std::enable_if<!std::is_same<typename std::decay<Surface>::type, surface>::value>::type* = nullptr>
		explicit surface(Surface&& sf):surface(+create(std::forward<Surface>(sf).get())){}
		expected<device_context_handle, hresult_error> get_dc(BOOL discard = FALSE)const{
			return will::com_create_resource<std::remove_pointer_t<HDC>>([&](HDC* t){return (*this)->GetDC(discard, t);})
			         .map([&](HDC dc){return device_context_handle{*this, dc};})
			         .emap([](HRESULT hr){return make_unexpected<hresult_error>(_T("will::dxgi::surface::get_dc"), hr);});
		}
	};
	class adapter : public detail::resource<IDXGIAdapter2>{
		using resource::resource;
	public:
		expected<dxgi, hresult_error> get_factory()const{
			return detail::convert_to_rich_interface<dxgi>(com_create_resource<IDXGIFactory2>([&](IDXGIFactory2** ptr){return (*this)->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(ptr));}), _T(__FUNCTION__));
		}
		struct device : detail::resource<IDXGIDevice2>{
			using resource::resource;
			static expected<device, hresult_error> create(ID3D11Device* ptr){return detail::convert_to_rich_interface<device>(query_interface<IDXGIDevice2>(ptr), _T(__FUNCTION__));}
			template<typename Device>
			static expected<device, hresult_error> create(Device&& dev){return create(std::forward<Device>(dev).get());}
			explicit device(ID3D11Device* ptr):device(+create(ptr)){}
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
		};
	};
	using device = adapter::device;
	struct swap_chain : detail::resource<IDXGISwapChain1>{
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
		using resource::resource;
		expected<surface, hresult_error> get_buffer(UINT buffer_index = 0)const{
			return detail::convert_to_rich_interface<surface>(com_create_resource<IDXGISurface2>([&](IDXGISurface2** ptr){return (*this)->GetBuffer(buffer_index, __uuidof(IDXGISurface2), reinterpret_cast<void**>(ptr));}), _T(__FUNCTION__));
		}
	};
	expected<swap_chain, hresult_error> create_swap_chain(ID3D11Device* device, HWND hwnd, const DXGI_SWAP_CHAIN_DESC1& description = swap_chain::description{}.get())const{
		return detail::convert_to_rich_interface<swap_chain>(com_create_resource<IDXGISwapChain1>([&](IDXGISwapChain1** ptr){return (*this)->CreateSwapChainForHwnd(device, hwnd, &description, nullptr, nullptr, ptr);}), _T(__FUNCTION__));
	}
	template<typename Device, std::enable_if_t<!std::is_same<std::decay_t<Device>, ID3D11Device*>::value>* = nullptr>
	expected<swap_chain, hresult_error> create_swap_chain(Device&& device, HWND hwnd, const DXGI_SWAP_CHAIN_DESC1& description = swap_chain::description{})const{
		return create_swap_chain(std::forward<Device>(device).get(), hwnd, description);
	}
	template<typename HWnd, std::enable_if_t<!std::is_same<std::decay_t<HWnd>, HWND>::value>* = nullptr>
	expected<swap_chain, hresult_error> create_swap_chain(ID3D11Device* device, HWnd&& hwnd, const DXGI_SWAP_CHAIN_DESC1& description = swap_chain::description{})const{
		return create_swap_chain(device, std::forward<Device>(hwnd).get(), description);
	}
	template<typename Device, typename HWnd, std::enable_if_t<!std::is_same<std::decay_t<Device>, ID3D11Device*>::value && !std::is_same<std::decay_t<HWnd>, HWND>::value>* = nullptr>
	expected<swap_chain, hresult_error> create_swap_chain(Device&& device, HWnd&& hwnd, const DXGI_SWAP_CHAIN_DESC1& description = swap_chain::description{})const{
		return create_swap_chain(std::forward<Device>(device).get(), std::forward<Device>(hwnd).get(), description);
	}
	static expected<dxgi, hresult_error> create_factory(){
		return detail::convert_to_rich_interface<dxgi>(com_create_resource<IDXGIFactory2>([](IDXGIFactory2** ptr){return CreateDXGIFactory1(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(ptr));}), _T(__FUNCTION__));
	}
};
}