#pragma once
#include"com.hpp"
#include"_resouce_property.hpp"
#include<d3d11.h>
#include<dxgi1_2.h>
#pragma comment(lib, "Dxgi.lib")
namespace will{
class dxgi : public detail::resource<IDXGIFactory2>{
	using resource::resource;
public:
	class surface : public detail::resource<IDXGISurface2>{
		class device_context_handle{
			HDC dc;
			const surface& srf;
		public:
			device_context_handle(const surface& s, BOOL discard):srf(s){srf->GetDC(discard, &dc);}
			device_context_handle(const device_context_handle&) = delete;
			device_context_handle(device_context_handle&&) = default;
			device_context_handle& operator=(const device_context_handle&) = delete;
			device_context_handle& operator=(device_context_handle&&) = default;
			~device_context_handle(){RECT r = {}; srf->ReleaseDC(&r);}
			operator HDC()const{return dc;}
		};
	public:
		using resource::resource;
		explicit surface(ID3D11Texture2D* ptr):resource(query_interface<IDXGISurface2>(ptr)){}
		template<typename Surface, typename std::enable_if<!std::is_same<typename std::decay<Surface>::type, surface>::value>::type* = nullptr>
		explicit surface(Surface&& sf):surface(std::forward<Surface>(sf).get()){}
		device_context_handle get_dc(BOOL discard = FALSE)const{return device_context_handle{*this, discard};}
	};
	class adapter : public detail::resource<IDXGIAdapter2>{
		using resource::resource;
	public:
		dxgi get_factory()const{return dxgi(com_create_resource<IDXGIFactory2>([&](IDXGIFactory2** ptr){return (*this)->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(ptr));}));}
		struct device : detail::resource<IDXGIDevice2>{
			explicit device(ID3D11Device* ptr):resource(query_interface<IDXGIDevice2>(ptr)){}
			template<typename Device>
			explicit device(Device&& dev):device(std::forward<Device>(dev).get()){}
			adapter get_adapter()const{return adapter(com_ptr<IDXGIAdapter>(com_create_resource<IDXGIAdapter>([&](IDXGIAdapter** ptr){return (*this)->GetAdapter(ptr);})).as<IDXGIAdapter2>());}
			dxgi get_factory()const{return get_adapter().get_factory();}
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
		surface get_buffer(UINT buffer_index = 0)const{
			return surface(com_create_resource<IDXGISurface2>([&](IDXGISurface2** ptr){return (*this)->GetBuffer(buffer_index, __uuidof(IDXGISurface2), reinterpret_cast<void**>(ptr));}));
		}
	};
	swap_chain create_swap_chain(ID3D11Device* device, HWND hwnd, const DXGI_SWAP_CHAIN_DESC1& description = swap_chain::description{})const{
		return swap_chain(com_create_resource<IDXGISwapChain1>([&](IDXGISwapChain1** ptr){return (*this)->CreateSwapChainForHwnd(device, hwnd, &description, nullptr, nullptr, ptr);}));
	}
	template<typename Device>
	swap_chain create_swap_chain(Device&& device, HWND hwnd, const DXGI_SWAP_CHAIN_DESC1& description = swap_chain::description{})const{
		return create_swap_chain(std::forward<Device>(device).get(), hwnd, description);
	}
};
}