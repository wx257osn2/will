#pragma once
#include"com.hpp"
#include<d3d11.h>
#include<dxgi1_2.h>
#pragma comment(lib, "Dxgi.lib")
namespace will{
class dxgi{
	com_ptr<IDXGIFactory2> fac;
	explicit dxgi(IDXGIFactory2* factory):fac(factory){}
public:
	dxgi(const dxgi&) = delete;
	dxgi(dxgi&&) = default;
	dxgi& operator=(const dxgi&) = delete;
	dxgi& operator=(dxgi&&) = default;
	~dxgi() = default;
	IDXGIFactory2* get()const{return fac.get();}
	IDXGIFactory2* operator->()const{return fac.get();}
	explicit operator bool()const{return static_cast<bool>(fac);}
	class surface{
		com_ptr<IDXGISurface2> srf;
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
		explicit surface(IDXGISurface2* surface):srf(surface){}
		explicit surface(ID3D11Texture2D* ptr):srf(query_interface<IDXGISurface2>(ptr)){}
		template<typename Surface, typename std::enable_if<!std::is_same<typename std::decay<Surface>::type, surface>::value>::type* = nullptr>
		explicit surface(Surface&& sf):surface(std::forward<Surface>(sf).get()){}
		surface(const surface&) = delete;
		surface(surface&&) = default;
		surface& operator=(const surface&) = delete;
		surface& operator=(surface&&) = default;
		~surface() = default;
		IDXGISurface2* get()const{return srf.get();}
		IDXGISurface2* operator->()const{return srf.get();}
		explicit operator bool()const{return static_cast<bool>(srf);}
		device_context_handle get_dc(BOOL discard = FALSE)const{return device_context_handle{*this, discard};}
	};
	class adapter{
		com_ptr<IDXGIAdapter2> ada;
		explicit adapter(com_ptr<IDXGIAdapter2>&& adapter):ada(std::move(adapter)){}
	public:
		adapter(const adapter&) = delete;
		adapter(adapter&&) = default;
		adapter& operator=(const adapter&) = delete;
		adapter& operator=(adapter&&) = default;
		~adapter() = default;
		dxgi get_factory()const{return dxgi(com_create_resource<IDXGIFactory2>([&](IDXGIFactory2** ptr){return ada->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(ptr));}));}
		IDXGIAdapter2* get()const{return ada.get();}
		IDXGIAdapter2* operator->()const{return ada.get();}
		explicit operator bool()const{return static_cast<bool>(ada);}
		class device{
			com_ptr<IDXGIDevice2> dev;
		public:
			explicit device(ID3D11Device* ptr):dev(query_interface<IDXGIDevice2>(ptr)){}
			template<typename Device>
			explicit device(Device&& dev):device(std::forward<Device>(dev).get()){}
			device(const device&) = delete;
			device(device&&) = default;
			device& operator=(const device&) =  delete;
			device& operator=(device&&) = default;
			~device() = default;
			adapter get_adapter()const{return adapter(com_ptr<IDXGIAdapter>(com_create_resource<IDXGIAdapter>([&](IDXGIAdapter** ptr){return dev->GetAdapter(ptr);})).as<IDXGIAdapter2>());}
			dxgi get_factory()const{return get_adapter().get_factory();}
			IDXGIDevice2* get()const{return dev.get();}
			IDXGIDevice2* operator->()const{return dev.get();}
			explicit operator bool()const{return static_cast<bool>(dev);}
		};
	};
	using device = adapter::device;
	class swap_chain{
		com_ptr<IDXGISwapChain1> sc;
	public:
		class description{
			DXGI_SWAP_CHAIN_DESC1 desc;
		public:
			description():desc(){desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; desc.SampleDesc.Count = 1; desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; desc.BufferCount = 2;}
			description(const description&) = default;
			description(description&&) = default;
			description& operator=(const description&) = default;
			description& operator=(description&&) = default;
			~description() = default;
	#define PROPERTYDECL(name, type, membername) description& name(type t){desc.membername = t;return *this;}
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
			operator DXGI_SWAP_CHAIN_DESC1&(){return desc;}
			DXGI_SWAP_CHAIN_DESC1& get(){return desc;}
		};
		explicit swap_chain(IDXGISwapChain1* p):sc(p){}
		swap_chain(const swap_chain&) = delete;
		swap_chain(swap_chain&&) = default;
		swap_chain& operator=(const swap_chain&) = delete;
		swap_chain& operator=(swap_chain&&) = default;
		~swap_chain() = default;
		IDXGISwapChain1* get()const{return sc.get();}
		IDXGISwapChain1* operator->()const{return sc.get();}
		explicit operator bool()const{return static_cast<bool>(sc);}
		surface get_buffer(UINT buffer_index = 0)const{
			return surface(com_create_resource<IDXGISurface2>([&](IDXGISurface2** ptr){return sc->GetBuffer(buffer_index, __uuidof(IDXGISurface2), reinterpret_cast<void**>(ptr));}));
		}
	};
	swap_chain create_swap_chain(ID3D11Device* device, HWND hwnd, const DXGI_SWAP_CHAIN_DESC1& description = swap_chain::description{})const{
		return swap_chain(com_create_resource<IDXGISwapChain1>([&](IDXGISwapChain1** ptr){return fac->CreateSwapChainForHwnd(device, hwnd, &description, nullptr, nullptr, ptr);}));
	}
	template<typename Device>
	swap_chain create_swap_chain(Device&& device, HWND hwnd, const DXGI_SWAP_CHAIN_DESC1& description = swap_chain::description{})const{
		return create_swap_chain(std::forward<Device>(device).get(), hwnd, description);
	}
};
}