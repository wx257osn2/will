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
			description():property(){prop.MipLevels = 1; prop.ArraySize = 1; prop.Format = DXGI_FORMAT_B8G8R8A8_UNORM; prop.SampleDesc.Count = 1;}
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
			description& bind_flags(int t){return bind_flags(D3D11_BIND_FLAG(t));}
			description& cpu_access_flags(int t){return cpu_access_flags(D3D11_CPU_ACCESS_FLAG(t));}
			description& misc_flags(int t){return misc_flags(D3D11_RESOURCE_MISC_FLAG(t));}
	#undef  PROPERTYDECL
		};
	};
	struct device : detail::resource<ID3D11Device>{
		struct subresource : D3D11_MAPPED_SUBRESOURCE{
			struct data : detail::property<D3D11_SUBRESOURCE_DATA>{
				using detail::property<D3D11_SUBRESOURCE_DATA>::property;
		#define PROPERTYDECL(name, type, membername) data& name(type t){prop.membername = t;return *this;}
				PROPERTYDECL(memory, const void*, pSysMem)
				template<typename T>
				data& memory(const T& t){prop.pSysMem = t.data();return *this;}
				PROPERTYDECL(pitch, UINT, SysMemPitch)
				PROPERTYDECL(slice_pitch, UINT, SysMemSlicePitch)
		#undef  PROPERTYDECL
			};
			explicit subresource(const D3D11_MAPPED_SUBRESOURCE& m):D3D11_MAPPED_SUBRESOURCE(m){}
			operator D3D11_MAPPED_SUBRESOURCE&(){return *this;}
			operator const D3D11_MAPPED_SUBRESOURCE&()const{return *this;}
			D3D11_MAPPED_SUBRESOURCE& get(){return *this;}
			const D3D11_MAPPED_SUBRESOURCE& get()const{return *this;}
		};
		class context : public detail::resource<ID3D11DeviceContext>{
			using resource::resource;
			subresource create_subresource(ID3D11Resource* resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0){
				D3D11_MAPPED_SUBRESOURCE m;
				(*this)->Map(resource, subresource_id, map_type, map_flags, &m);
				return subresource{m};
			}
			void release_subresource(ID3D11Resource* resource, UINT subresource_id){
				(*this)->Unmap(resource, subresource_id);
			}
		public:
			class scoped_subresource : public subresource{
				context& c;
				ID3D11Resource* resource;
				UINT subresource_id;
			public:
				scoped_subresource(context& context, ID3D11Resource* resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0):subresource(context.create_subresource(resource, subresource_id, map_type, map_flags)), c(context), resource(resource), subresource_id(subresource_id){}
				~scoped_subresource(){c.release_subresource(resource, subresource_id);}
			};
			friend struct will::d3d::device;
			scoped_subresource create_scoped_subresource(ID3D11Resource* resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0){
				return scoped_subresource{*this, resource, subresource_id, map_type, map_flags};
			}
			template<typename Resource, std::enable_if_t<!std::is_pointer<Resource>::value>* = nullptr>
			scoped_subresource create_scoped_subresource(Resource&& resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0){
				return scoped_subresource{*this, std::forward<Resource>(resource).get(), subresource_id, map_type, map_flags};
			}
			template<typename F>
			void map(F&& f, ID3D11Resource* resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0){
				std::forward<F>(f)(create_scoped_subresource(resource, subresource_id, map_type, map_flags));
			}
			template<typename F, typename T>
			void map(F&& f, T* resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0){
				will::com_ptr<ID3D11Resource> res = query_interface<ID3D11Resource>(resource);
				map(std::forward<F>(f), res.get(), subresource_id, map_type, map_flags);
			}
			template<typename F, typename Resource>
			void map(F&& f, Resource&& resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0){
				map(std::forward<F>(f), std::forward<Resource>(resource).get(), subresource_id, map_type, map_flags);
			}
		};
		explicit device(D3D11_CREATE_DEVICE_FLAG flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT):resource(com_create_resource<ID3D11Device>([&](ID3D11Device** ptr){return D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0u, D3D11_SDK_VERSION, ptr, nullptr, nullptr);})){}
		explicit device(int flags):device(static_cast<D3D11_CREATE_DEVICE_FLAG>(flags)){}
		context create_deferred_context()const{return context{com_create_resource<ID3D11DeviceContext>([&](ID3D11DeviceContext** ptr){return (*this)->CreateDeferredContext(0, ptr);})};}
		context get_immediate_context()const{return context{com_create_resource<ID3D11DeviceContext>([&](ID3D11DeviceContext** ptr){return (*this)->GetImmediateContext(ptr), S_OK;})};}
		texture2d create_texture2d(const D3D11_TEXTURE2D_DESC& description = texture2d::description())const{
			return texture2d{com_create_resource<ID3D11Texture2D>([&](ID3D11Texture2D** ptr){return (*this)->CreateTexture2D(&description, nullptr, ptr);})};
		}
		texture2d create_texture2d(const D3D11_TEXTURE2D_DESC& description, const D3D11_SUBRESOURCE_DATA& subresource)const{
			return texture2d{com_create_resource<ID3D11Texture2D>([&](ID3D11Texture2D** ptr){return (*this)->CreateTexture2D(&description, &subresource, ptr);})};
		}
	};
};
}