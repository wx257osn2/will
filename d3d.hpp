//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"com.hpp"
#include"_resource_property.hpp"
#include"dxgi.hpp"
#include<array>
#include<d3d11_1.h>
#include<type_traits>
#pragma comment(lib, "d3d11.lib")
namespace will{
struct d3d{
	d3d() = delete;
	d3d(const d3d&) = delete;
	d3d(d3d&&) = delete;
	d3d& operator=(const d3d&) = delete;
	d3d& operator=(d3d&&) = delete;
	~d3d() = delete;
	class device : public detail::resource<ID3D11Device1>{
		using resource::resource;
	public:
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
		template<typename I>
		struct child : detail::resource<I>{
			using resource::resource;
			device get_device()const noexcept{ID3D11Device* ptr;(*this)->GetDevice(&ptr);auto ret =* query_interface<ID3D11Device1>(ptr);ptr->Release();return device{std::move(ret)};}
		};
		class context : public child<ID3D11DeviceContext1>{
			using child::child;
			expected<subresource, hresult_error> create_subresource(ID3D11Resource* resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0)const{
				D3D11_MAPPED_SUBRESOURCE m;
				auto hr = (*this)->Map(resource, subresource_id, map_type, map_flags, &m);
				if(SUCCEEDED(hr))
					return subresource{m};
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			}
			void release_subresource(ID3D11Resource* resource, UINT subresource_id)const{
				(*this)->Unmap(resource, subresource_id);
			}
		public:
			class scoped_subresource : public D3D11_MAPPED_SUBRESOURCE{
				const context& c;
				ID3D11Resource* resource;
				UINT subresource_id;
				scoped_subresource(D3D11_MAPPED_SUBRESOURCE&& s, const context& cont, ID3D11Resource* res, UINT sub_id)noexcept:D3D11_MAPPED_SUBRESOURCE{std::move(s)}, c{cont}, resource{res}, subresource_id{sub_id}{}
			public:
				scoped_subresource(const context& context, ID3D11Resource* resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0):D3D11_MAPPED_SUBRESOURCE(+context.create_subresource(resource, subresource_id, map_type, map_flags)), c(context), resource(resource), subresource_id(subresource_id){}
				scoped_subresource(scoped_subresource&& t):D3D11_MAPPED_SUBRESOURCE{std::move(t.get())}, c{t.c}, resource{std::move(t.resource)}, subresource_id{std::move(t.subresource_id)}{t.resource = nullptr;}
				~scoped_subresource()noexcept{if(resource)c.release_subresource(resource, subresource_id);}
				operator D3D11_MAPPED_SUBRESOURCE&(){return *this;}
				operator const D3D11_MAPPED_SUBRESOURCE&()const{return *this;}
				D3D11_MAPPED_SUBRESOURCE& get(){return *this;}
				const D3D11_MAPPED_SUBRESOURCE& get()const{return *this;}
				friend context;
			};
			expected<scoped_subresource, hresult_error> create_scoped_subresource(ID3D11Resource* resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0)const{
				return create_subresource(resource, subresource_id, map_type, map_flags).map([&](subresource&& s){return scoped_subresource{std::move(s), *this, resource, subresource_id};});
			}
			template<typename Resource, std::enable_if_t<!std::is_pointer<Resource>::value>* = nullptr>
			expected<scoped_subresource, hresult_error> create_scoped_subresource(Resource&& resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0)const{
				return create_scoped_subresource(std::forward<Resource>(resource).get(), subresource_id, map_type, map_flags);
			}
			template<typename F>
			expected<void, hresult_error> map(F&& f, ID3D11Resource* resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0)const{
				return create_scoped_subresource(resource, subresource_id, map_type, map_flags).map([&](scoped_subresource&& s){std::forward<F>(f)(std::move(s));});
			}
			template<typename F, typename T>
			expected<void, hresult_error> map(F&& f, T* resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0)const{
				return query_interface<ID3D11Resource>(resource).bind([&](ID3D11Resource* r){map(std::forward<F>(f), will::com_ptr<ID3D11Resource>{std::move(r)}.get(), subresource_id, map_type, map_flags);});
			}
			template<typename F, typename Resource, std::enable_if_t<!std::is_pointer<std::decay_t<Resource>>::value>* = nullptr>
			expected<void, hresult_error> map(F&& f, Resource&& resource, UINT subresource_id, D3D11_MAP map_type, UINT map_flags = 0)const{
				return map(std::forward<F>(f), std::forward<Resource>(resource).get(), subresource_id, map_type, map_flags);
			}
			void copy(ID3D11Resource* to, ID3D11Resource* from)const{
				(*this)->CopyResource(to, from);
			}
			template<typename Resource, std::enable_if_t<!std::is_base_of<ID3D11Resource, std::remove_pointer_t<std::decay_t<Resource>>>::value>* = nullptr>
			void copy(Resource&& to, ID3D11Resource* from)const{return copy(std::forward<Resource>(to).get(), from);}
			template<typename Resource, std::enable_if_t<!std::is_base_of<ID3D11Resource, std::remove_pointer_t<std::decay_t<Resource>>>::value>* = nullptr>
			void copy(ID3D11Resource* to, Resource&& from)const{return copy(to, std::forward<Resource>(from).get());}
			template<typename ResourceT, typename ResourceF, std::enable_if_t<!std::is_base_of<ID3D11Resource, std::remove_pointer_t<std::decay_t<ResourceT>>>::value && !std::is_base_of<ID3D11Resource, std::remove_pointer_t<std::decay_t<ResourceF>>>::value>* = nullptr>
			void copy(ResourceT&& to, ResourceF&& from)const{return copy(std::forward<ResourceT>(to).get(), std::forward<ResourceF>(from).get());}
			void flush(){(*this)->Flush();}
			friend device;
			friend d3d;
		};
		class texture2d : public child<ID3D11Texture2D>{
			class copy_impl{
				const context& devcont;
				const texture2d& tex;
			public:
				copy_impl(const context& devcont, const texture2d& tex)noexcept:devcont(devcont), tex(tex){}
				friend texture2d;
			};
			class copy_with_immediate_impl{
				const texture2d& tex;
			public:
				copy_with_immediate_impl(const texture2d& tex)noexcept:tex(tex){}
				friend texture2d;
			};
		public:
			using child::child;
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
				description& bind_flags(std::underlying_type_t<D3D11_BIND_FLAG> t){return bind_flags(D3D11_BIND_FLAG(t));}
				description& cpu_access_flags(std::underlying_type_t<D3D11_CPU_ACCESS_FLAG> t){return cpu_access_flags(D3D11_CPU_ACCESS_FLAG(t));}
				description& misc_flags(std::underlying_type_t<D3D11_RESOURCE_MISC_FLAG> t){return misc_flags(D3D11_RESOURCE_MISC_FLAG(t));}
		#undef  PROPERTYDECL
			};
			D3D11_TEXTURE2D_DESC get_desc()const noexcept{D3D11_TEXTURE2D_DESC desc;(*this)->GetDesc(&desc);return desc;}
			texture2d(texture2d&&)noexcept = default;
			texture2d(IDXGISurface2* surf):child{std::move(+query_interface<ID3D11Texture2D>(surf))}{}
			template<typename DXGISurface, std::enable_if_t<!std::is_base_of<IDXGISurface2, std::remove_pointer_t<std::decay_t<DXGISurface>>>::value, std::nullptr_t> = nullptr>
			texture2d(DXGISurface&& surf):texture2d{std::forward<DXGISurface>(surf).get()}{}
			texture2d(const copy_impl& c):texture2d{std::move(+c.tex.get_device().create_texture2d(c.tex.get_desc()) = c)}{}
			texture2d(const copy_with_immediate_impl& c):texture2d{std::move(+c.tex.get_device().create_texture2d(c.tex.get_desc()) = c)}{}
			texture2d& operator=(texture2d&&)noexcept = default;
			texture2d& operator=(const copy_impl& c){c.devcont.copy((*this), c.tex);return *this;}
			texture2d& operator=(const copy_with_immediate_impl& c){auto cont = c.tex.get_device().get_immediate_context();return *this = copy_impl{cont, c.tex};}
			copy_impl copy(const context& devcont)const noexcept{return copy_impl{devcont, *this};}
			copy_with_immediate_impl copy()const noexcept{return copy_with_immediate_impl{*this};}
		};
		explicit device(std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT
#ifdef _DEBUG
			| D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS
#endif
		):resource(+create(flags)){}
		expected<context, hresult_error> create_deferred_context()const noexcept{
			return detail::convert_to_rich_interface<context>(com_create_resource<ID3D11DeviceContext1>([&](ID3D11DeviceContext1** ptr){return (*this)->CreateDeferredContext1(0, ptr);}), _T(__FUNCTION__));
		}
		context get_immediate_context()const noexcept{ID3D11DeviceContext1* ptr;(*this)->GetImmediateContext1(&ptr);return context{std::move(ptr)};}
		expected<texture2d, hresult_error> create_texture2d(const D3D11_TEXTURE2D_DESC& description = texture2d::description().get())const noexcept{
			return detail::convert_to_rich_interface<texture2d>(com_create_resource<ID3D11Texture2D>([&](ID3D11Texture2D** ptr){return (*this)->CreateTexture2D(&description, nullptr, ptr);}), _T(__FUNCTION__));
		}
		expected<texture2d, hresult_error> create_texture2d(const D3D11_TEXTURE2D_DESC& description, const D3D11_SUBRESOURCE_DATA& subresource)const noexcept{
			return detail::convert_to_rich_interface<texture2d>(com_create_resource<ID3D11Texture2D>([&](ID3D11Texture2D** ptr){return (*this)->CreateTexture2D(&description, &subresource, ptr);}), _T(__FUNCTION__));
		}
		static expected<device, hresult_error> create(std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT
#ifdef _DEBUG
			| D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS
#endif
		)noexcept{
			return detail::convert_to_rich_interface<device>(com_create_resource<ID3D11Device>([&](ID3D11Device** ptr){return ::D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0u, D3D11_SDK_VERSION, ptr, nullptr, nullptr);}).bind([](ID3D11Device* ptr){auto ret = query_interface<ID3D11Device1>(ptr);if(ret)ptr->Release();return std::move(ret);}), _T(__FUNCTION__));
		}
	};
	using texture2d = device::texture2d;
};
}