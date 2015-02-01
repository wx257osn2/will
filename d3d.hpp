#pragma once
#include"com.hpp"
#include<array>
#include<d3d11.h>
#pragma comment(lib, "d3d11.lib")
namespace will{
class d3d{
	//I have GTX780(released in 2013), and fucking NVIDIA didn't support D3D11.1 on that.
	/*struct _impl_D3Ds{
		ID3D11Device* dev;
		ID3D11DeviceContext* cont;
		D3D_FEATURE_LEVEL level;
	};
	template<std::size_t N>
	static _impl_D3Ds make_d3d_device(D3D11_CREATE_DEVICE_FLAG flags, std::array<D3D_FEATURE_LEVEL, N> levels){
		_impl_D3Ds outputs;
		auto ret = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, levels.data(), levels.size(), D3D11_SDK_VERSION, &outputs.dev, &outputs.level, &outputs.cont);
		return SUCCEEDED(ret) ? outputs : _impl_D3Ds{nullptr, nullptr, static_cast<D3D_FEATURE_LEVEL>(ret)};
	}
	com_ptr<ID3D11Device1> dev;
	com_ptr<ID3D11DeviceContext1> cont;
	explicit d3d(_impl_D3Ds ds):dev(com_ptr<ID3D11Device>(ds.dev).as<ID3D11Device1>()), cont(com_ptr<ID3D11DeviceContext>(ds.cont).as<ID3D11DeviceContext1>()){}*/
public:
	d3d() = delete;
	d3d(const d3d&) = delete;
	d3d(d3d&&) = delete;
	d3d& operator=(const d3d&) = delete;
	d3d& operator=(d3d&&) = delete;
	~d3d() = default;
	class texture2d{
		com_ptr<ID3D11Texture2D> tex;
	public:
		texture2d(ID3D11Texture2D* p):tex(p){}
		texture2d(const texture2d&) = delete;
		texture2d(texture2d&&) = default;
		texture2d& operator=(const texture2d&) = delete;
		texture2d& operator=(texture2d&&) = default;
		~texture2d() = default;
		ID3D11Texture2D* get()const{return tex.get();}
		ID3D11Texture2D* operator->()const{return tex.get();}
		explicit operator bool()const{return static_cast<bool>(tex);}
		class description{
			D3D11_TEXTURE2D_DESC desc;
		public:
			description():desc(){desc.MipLevels = 1; desc.ArraySize = 1; desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; desc.SampleDesc.Count = 1; desc.BindFlags = D3D11_BIND_RENDER_TARGET;}
			description(const description&) = default;
			description(description&&) = default;
			description& operator=(const description&) = default;
			description& operator=(description&&) = default;
			~description() = default;
	#define PROPERTYDECL(name, type, membername) description& name(type t){desc.membername = t;return *this;}
			PROPERTYDECL(width, UINT, Width)
			PROPERTYDECL(height, UINT, Height)
			PROPERTYDECL(mip_levels, UINT, MipLevels)
			PROPERTYDECL(array_size, UINT, ArraySize)
			PROPERTYDECL(format, DXGI_FORMAT, Format)
			PROPERTYDECL(sample_count, UINT, SampleDesc.Count)
			PROPERTYDECL(sample_quality, UINT, SampleDesc.Quality)
			PROPERTYDECL(usage, D3D11_USAGE, Usage)
			PROPERTYDECL(bind_flags, D3D11_BIND_FLAG, BindFlags)
			PROPERTYDECL(cpu_access_flags, D3D11_CPU_ACCESS_FLAG, CPUAccessFlags)
			PROPERTYDECL(misc_flags, D3D11_RESOURCE_MISC_FLAG, MiscFlags)
	#undef  PROPERTYDECL
			operator D3D11_TEXTURE2D_DESC&(){return desc;}
			D3D11_TEXTURE2D_DESC& get(){return desc;}
		};
	};
	class device{
		com_ptr<ID3D11Device> dev;
	public:
		explicit device(D3D11_CREATE_DEVICE_FLAG flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT):dev(com_create_resource<ID3D11Device>([&](ID3D11Device** ptr){return D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0u, D3D11_SDK_VERSION, ptr, nullptr, nullptr);})){}
		device(const device&) = delete;
		device(device&&) = default;
		device& operator=(const device&) = delete;
		device& operator=(device&&) = default;
		~device() = default;
		ID3D11Device* get()const{return dev.get();}
		ID3D11Device* operator->()const{return dev.get();}
		explicit operator bool()const{return static_cast<bool>(dev);}
		class context{
			com_ptr<ID3D11DeviceContext> cont;
			explicit context(ID3D11DeviceContext* cont):cont(cont){}
		public:
			context(const context&) = delete;
			context(context&&) = default;
			context& operator=(const context&) = delete;
			context& operator=(context&&) = default;
			~context() = default;
			ID3D11DeviceContext* get()const{return cont.get();}
			ID3D11DeviceContext* operator->()const{return cont.get();}
			explicit operator bool()const{return static_cast<bool>(cont);}
			friend class will::d3d::device;
		};
		context create_deferred_context()const{return context{com_create_resource<ID3D11DeviceContext>([&](ID3D11DeviceContext** ptr){return dev->CreateDeferredContext(0, ptr);})};}
		context get_immediate_context()const{return context{com_create_resource<ID3D11DeviceContext>([&](ID3D11DeviceContext** ptr){return dev->GetImmediateContext(ptr), S_OK;})};}
		texture2d create_texture2d(const D3D11_TEXTURE2D_DESC& description = texture2d::description())const{
			return texture2d{com_create_resource<ID3D11Texture2D>([&](ID3D11Texture2D** ptr){return dev->CreateTexture2D(&description, nullptr, ptr);})};
		}
	};
};
}