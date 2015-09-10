#pragma once
#include"com.hpp"
#include"_resouce_property.hpp"
#include<array>
#include<d3d11.h>
#pragma comment(lib, "d3d11.lib")
namespace will{
struct d3d{
	d3d() = delete;
	d3d(const d3d&) = delete;
	d3d(d3d&&) = delete;
	d3d& operator=(const d3d&) = delete;
	d3d& operator=(d3d&&) = delete;
	~d3d() = delete;
	struct texture2d : detail::resource<ID3D11Texture2D>{
		using resource::resource;
		struct description : detail::property<D3D11_TEXTURE2D_DESC>{
			description():property(){prop.MipLevels = 1; prop.ArraySize = 1; prop.Format = DXGI_FORMAT_B8G8R8A8_UNORM; prop.SampleDesc.Count = 1; prop.BindFlags = D3D11_BIND_RENDER_TARGET;}
	#define PROPERTYDECL(name, type, membername) description& name(type t){prop.membername = t;return *this;}
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
		};
	};
	struct device : detail::resource<ID3D11Device>{
		explicit device(D3D11_CREATE_DEVICE_FLAG flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT):resource(com_create_resource<ID3D11Device>([&](ID3D11Device** ptr){return D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0u, D3D11_SDK_VERSION, ptr, nullptr, nullptr);})){}
		class context : public detail::resource<ID3D11DeviceContext>{
			using resource::resource;
		public:
			friend struct will::d3d::device;
		};
		context create_deferred_context()const{return context{com_create_resource<ID3D11DeviceContext>([&](ID3D11DeviceContext** ptr){return (*this)->CreateDeferredContext(0, ptr);})};}
		context get_immediate_context()const{return context{com_create_resource<ID3D11DeviceContext>([&](ID3D11DeviceContext** ptr){return (*this)->GetImmediateContext(ptr), S_OK;})};}
		texture2d create_texture2d(const D3D11_TEXTURE2D_DESC& description = texture2d::description())const{
			return texture2d{com_create_resource<ID3D11Texture2D>([&](ID3D11Texture2D** ptr){return (*this)->CreateTexture2D(&description, nullptr, ptr);})};
		}
	};
};
}