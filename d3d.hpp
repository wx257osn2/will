//Copyright (C) 2014-2017, 2019 I
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
				template<typename T, std::enable_if_t<!std::is_pointer<std::decay_t<T>>::value, std::nullptr_t> = nullptr>
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
		template<typename I>
		struct resource : child<I>{
			using child::child;
			::D3D11_RESOURCE_DIMENSION get_type()const{::D3D11_RESOURCE_DIMENSION t;(*this)->GetType(&t);return t;}
			::UINT get_eviction_priority()const{return (*this)->GetEvictionPriority();}
			void set_eviction_priority(::UINT ep){return (*this)->SetEvictionPriority(ep);}
			__declspec(property(get=get_eviction_priority, put=set_eviction_priority)) ::UINT eviction_priority;
		};
		template<typename I>
		struct view : child<I> {
			using child::child;
			resource<ID3D11Resource> get_resource()const{ID3D11Resource* ptr;(*this)->GetResource(&ptr);return resource<ID3D11Resource>{std::move(ptr)};}
			template<typename D3D11ResourceWrapper>
			expected<D3D11ResourceWrapper, hresult_error> get_resource()const{
				auto qi = query_interface<detail::get_interface<D3D11ResourceWrapper>>(get_resource().get());
				if(qi)
					return D3D11ResourceWrapper{std::move(*qi)};
				return make_unexpected(qi.error());
			}
		};
		struct render_target_view : view<::ID3D11RenderTargetView>{
			using view::view;
			struct description : detail::property<::D3D11_RENDER_TARGET_VIEW_DESC>{
				description():property(){}
		#define PROPERTYDECL(name, type, membername) description& name(type t){prop.membername = t;return *this;}
				PROPERTYDECL(format, ::DXGI_FORMAT, Format)
				PROPERTYDECL(view_dimension, ::D3D11_RTV_DIMENSION, ViewDimension)
		#undef  PROPERTYDECL
		#define PROPERTYDECL(name, type, membername, viewdimension) description& name(type t){prop.ViewDimension = viewdimension;prop.membername = t;return *this;}
				PROPERTYDECL(buffer, const D3D11_BUFFER_RTV&, Buffer, ::D3D11_RTV_DIMENSION_BUFFER)
				PROPERTYDECL(texture1d, D3D11_TEX1D_RTV, Texture1D, ::D3D11_RTV_DIMENSION_TEXTURE1D)
				PROPERTYDECL(texture1d_array, const D3D11_TEX1D_ARRAY_RTV&, Texture1DArray, ::D3D11_RTV_DIMENSION_TEXTURE1DARRAY)
				PROPERTYDECL(texture2d, D3D11_TEX2D_RTV, Texture2D, ::D3D11_RTV_DIMENSION_TEXTURE2D)
				PROPERTYDECL(texture2d_array, const D3D11_TEX2D_ARRAY_RTV&, Texture2DArray, ::D3D11_RTV_DIMENSION_TEXTURE2DARRAY)
				PROPERTYDECL(texture2d_multisampled, D3D11_TEX2DMS_RTV, Texture2DMS, ::D3D11_RTV_DIMENSION_TEXTURE2DMS)
				PROPERTYDECL(texture2d_multisampled_array, const D3D11_TEX2DMS_ARRAY_RTV&, Texture2DMSArray, ::D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY)
				PROPERTYDECL(texture3d, const D3D11_TEX3D_RTV&, Texture3D, ::D3D11_RTV_DIMENSION_TEXTURE3D)
		#undef  PROPERTYDECL
			};
			::D3D11_RENDER_TARGET_VIEW_DESC get_desc()const{::D3D11_RENDER_TARGET_VIEW_DESC d;(*this)->GetDesc(&d);return d;}
		};
		struct buffer : resource<::ID3D11Buffer>{
			using resource::resource;
			struct description : detail::property<::D3D11_BUFFER_DESC>{
		#define PROPERTYDECL(name, type, membername) description& name(type t){prop.membername = t;return *this;}
				PROPERTYDECL(byte_width, UINT, ByteWidth)
				PROPERTYDECL(usage, D3D11_USAGE, Usage)
				PROPERTYDECL(bind_flags, D3D11_BIND_FLAG, BindFlags)
				PROPERTYDECL(cpu_access_flags, D3D11_CPU_ACCESS_FLAG, CPUAccessFlags)
				PROPERTYDECL(misc_flags, D3D11_RESOURCE_MISC_FLAG, MiscFlags)
				PROPERTYDECL(structure_byte_stride, UINT, StructureByteStride)
				description& bind_flags(std::underlying_type_t<D3D11_BIND_FLAG> t){return bind_flags(D3D11_BIND_FLAG(t));}
				description& cpu_access_flags(std::underlying_type_t<D3D11_CPU_ACCESS_FLAG> t){return cpu_access_flags(D3D11_CPU_ACCESS_FLAG(t));}
				description& misc_flags(std::underlying_type_t<D3D11_RESOURCE_MISC_FLAG> t){return misc_flags(D3D11_RESOURCE_MISC_FLAG(t));}
		#undef  PROPERTYDECL
			};
			::D3D11_BUFFER_DESC get_desc()const{::D3D11_BUFFER_DESC d;(*this)->GetDesc(&d);return d;}
		};
		struct depth_stencil_view : view<::ID3D11DepthStencilView>{
			using view::view;
			struct description : detail::property<::D3D11_DEPTH_STENCIL_VIEW_DESC>{
				description():property(){}
		#define PROPERTYDECL(name, type, membername) description& name(type t){prop.membername = t;return *this;}
				PROPERTYDECL(format, ::DXGI_FORMAT, Format)
				PROPERTYDECL(view_dimension, ::D3D11_DSV_DIMENSION, ViewDimension)
				PROPERTYDECL(flags, ::UINT, Flags)
		#undef  PROPERTYDECL
		#define PROPERTYDECL(name, type, membername, viewdimension) description& name(type t){prop.ViewDimension = viewdimension;prop.membername = t;return *this;}
				PROPERTYDECL(texture1d, D3D11_TEX1D_DSV, Texture1D, ::D3D11_DSV_DIMENSION_TEXTURE1D)
				PROPERTYDECL(texture1d_array, const D3D11_TEX1D_ARRAY_DSV&, Texture1DArray, ::D3D11_DSV_DIMENSION_TEXTURE1DARRAY)
				PROPERTYDECL(texture2d, D3D11_TEX2D_DSV, Texture2D, ::D3D11_DSV_DIMENSION_TEXTURE2D)
				PROPERTYDECL(texture2d_array, const D3D11_TEX2D_ARRAY_DSV&, Texture2DArray, ::D3D11_DSV_DIMENSION_TEXTURE2DARRAY)
				PROPERTYDECL(texture2d_multisampled, D3D11_TEX2DMS_DSV, Texture2DMS, ::D3D11_DSV_DIMENSION_TEXTURE2DMS)
				PROPERTYDECL(texture2d_multisampled_array, const D3D11_TEX2DMS_ARRAY_DSV&, Texture2DMSArray, ::D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY)
		#undef  PROPERTYDECL
			};
			::D3D11_DEPTH_STENCIL_VIEW_DESC get_desc()const{::D3D11_DEPTH_STENCIL_VIEW_DESC d;(*this)->GetDesc(&d);return d;}
		};
		struct input_layout : child<::ID3D11InputLayout>{using child::child;};
		struct vertex_shader : child<::ID3D11VertexShader>{using child::child;};
		struct pixel_shader : child<::ID3D11PixelShader>{using child::child;};
		struct compute_shader : child<::ID3D11ComputeShader>{using child::child;};
		//struct class_instance : child<::ID3D11ClassInstance>
		//struct class_linkage : child<::ID3D11ClassLinkage>
		struct sampler_state : child<::ID3D11SamplerState>{
			using child::child;
			class description : public detail::property<::D3D11_SAMPLER_DESC>{
				class address_property{
					description* prop;
				public:
					address_property(description* p):prop(p){}
					address_property(const address_property&) = delete;
					address_property(address_property&&) = delete;
					address_property& operator=(const address_property&) = delete;
					address_property& operator=(address_property&&) = delete;
					description& operator()(::D3D11_TEXTURE_ADDRESS_MODE u, ::D3D11_TEXTURE_ADDRESS_MODE v, ::D3D11_TEXTURE_ADDRESS_MODE w)const{prop->prop.AddressU = u; prop->prop.AddressV = v; prop->prop.AddressW; return *prop;}
					description& operator()(const std::array<::D3D11_TEXTURE_ADDRESS_MODE, 3>& uvw)const{return (*this)(uvw[0], uvw[1], uvw[2]);}
					description& operator()(::D3D11_TEXTURE_ADDRESS_MODE t)const{return (*this)(t, t, t);}
					description& u(::D3D11_TEXTURE_ADDRESS_MODE u)const{prop->prop.AddressU = u; return *prop;}
					description& v(::D3D11_TEXTURE_ADDRESS_MODE v)const{prop->prop.AddressV = v; return *prop;}
					description& w(::D3D11_TEXTURE_ADDRESS_MODE w)const{prop->prop.AddressW = w; return *prop;}
				};
				friend class address_property;
				class bc_property{
					description* prop;
				public:
					bc_property(description* p):prop(p){}
					bc_property(const bc_property&) = delete;
					bc_property(bc_property&&) = delete;
					bc_property& operator=(const bc_property&) = delete;
					bc_property& operator=(bc_property&&) = delete;
					description& operator()(::FLOAT r, ::FLOAT g, ::FLOAT b, ::FLOAT a)const{prop->prop.BorderColor[0] = r; prop->prop.BorderColor[1] = g; prop->prop.BorderColor[2] = b; prop->prop.BorderColor[3] = a; return *prop;}
					description& operator()(const std::array<::FLOAT, 4>& rgba)const{return (*this)(rgba[0], rgba[1], rgba[2], rgba[3]);}
					description& r(::FLOAT r)const{prop->prop.BorderColor[0] = r; return *prop;}
					description& g(::FLOAT g)const{prop->prop.BorderColor[1] = g; return *prop;}
					description& b(::FLOAT b)const{prop->prop.BorderColor[2] = b; return *prop;}
					description& a(::FLOAT a)const{prop->prop.BorderColor[3] = a; return *prop;}
				};
				friend class bc_property;
				class lod_property{
					description* prop;
				public:
					lod_property(description* p):prop(p){}
					lod_property(const lod_property&) = delete;
					lod_property(lod_property&&) = delete;
					lod_property& operator=(const lod_property&) = delete;
					lod_property& operator=(lod_property&&) = delete;
					description& operator()(::FLOAT min, ::FLOAT max)const{prop->prop.MinLOD = min; prop->prop.MaxLOD = max; return *prop;}
					description& min(::FLOAT min)const{prop->prop.MinLOD = min; return *prop;}
					description& max(::FLOAT max)const{prop->prop.MaxLOD = max; return *prop;}
				};
				friend class lod_property;
			public:
				description():property(){prop.Filter = ::D3D11_FILTER_MIN_MAG_MIP_LINEAR;prop.AddressU = prop.AddressV = prop.AddressW = ::D3D11_TEXTURE_ADDRESS_CLAMP;prop.MinLOD = std::numeric_limits<float>::lowest();prop.MaxLOD = std::numeric_limits<float>::max();prop.MipLODBias = 0.f;prop.MaxAnisotropy = 1;prop.ComparisonFunc = ::D3D11_COMPARISON_NEVER;prop.BorderColor[0] = prop.BorderColor[1] = prop.BorderColor[2] = prop.BorderColor[3] = 1.f;}
		#define PROPERTYDECL(name, type, membername) description& name(type t){prop.membername = t;return *this;}
				PROPERTYDECL(filter, ::D3D11_FILTER, Filter)
				const address_property address = this;
				PROPERTYDECL(mip_lod_bias, ::FLOAT, MipLODBias)
				PROPERTYDECL(max_anisotropy, ::UINT, MaxAnisotropy)
				PROPERTYDECL(comparison_func, ::D3D11_COMPARISON_FUNC, ComparisonFunc)
				const bc_property border_color = this;
				const lod_property lod = this;
		#undef  PROPERTYDECL
			};
			::D3D11_SAMPLER_DESC get_desc()const{::D3D11_SAMPLER_DESC d;(*this)->GetDesc(&d);return d;}
		};
		struct shader_resource_view : view<ID3D11ShaderResourceView>{
			using view::view;
			struct description : detail::property<::D3D11_SHADER_RESOURCE_VIEW_DESC>{
		#define PROPERTYDECL(name, type, membername) description& name(type t){prop.membername = t;return *this;}
				PROPERTYDECL(format, ::DXGI_FORMAT, Format)
				PROPERTYDECL(view_dimension, ::D3D11_SRV_DIMENSION, ViewDimension)
		#undef  PROPERTYDECL
		#define PROPERTYDECL(name, type, membername, viewdimension) description& name(type t){prop.ViewDimension = viewdimension;prop.membername = t;return *this;}
				PROPERTYDECL(buffer, const D3D11_BUFFER_SRV&, Buffer, ::D3D11_SRV_DIMENSION_BUFFER)
				PROPERTYDECL(texture1d, const D3D11_TEX1D_SRV&, Texture1D, ::D3D11_SRV_DIMENSION_TEXTURE1D)
				PROPERTYDECL(texture1d_array, const D3D11_TEX1D_ARRAY_SRV&, Texture1DArray, ::D3D11_SRV_DIMENSION_TEXTURE1DARRAY)
				PROPERTYDECL(texture2d, const D3D11_TEX2D_SRV&, Texture2D, ::D3D11_SRV_DIMENSION_TEXTURE2D)
				PROPERTYDECL(texture2d_array, const D3D11_TEX2D_ARRAY_SRV&, Texture2DArray, ::D3D11_SRV_DIMENSION_TEXTURE2DARRAY)
				PROPERTYDECL(texture2d_multisampled, D3D11_TEX2DMS_SRV, Texture2DMS, ::D3D11_SRV_DIMENSION_TEXTURE2DMS)
				PROPERTYDECL(texture2d_multisampled_array, const D3D11_TEX2DMS_ARRAY_SRV&, Texture2DMSArray, ::D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY)
				PROPERTYDECL(texture3d, const D3D11_TEX3D_SRV&, Texture3D, ::D3D11_SRV_DIMENSION_TEXTURE3D)
				PROPERTYDECL(texture_cube, const D3D11_TEXCUBE_SRV&, TextureCube, ::D3D11_SRV_DIMENSION_TEXTURECUBE)
				PROPERTYDECL(texture_cube_array, const D3D11_TEXCUBE_ARRAY_SRV&, TextureCubeArray, ::D3D11_SRV_DIMENSION_TEXTURECUBEARRAY)
				PROPERTYDECL(buffer_ex, const D3D11_BUFFEREX_SRV&, BufferEx, ::D3D11_SRV_DIMENSION_BUFFEREX)
		#undef  PROPERTYDECL
			};
			::D3D11_SHADER_RESOURCE_VIEW_DESC get_desc(){::D3D11_SHADER_RESOURCE_VIEW_DESC d;(*this)->GetDesc(&d);return d;}
		};
		struct unordered_access_view : view<::ID3D11UnorderedAccessView>{
			using view::view;
			struct description : detail::property<::D3D11_UNORDERED_ACCESS_VIEW_DESC>{
				description():property(){}
		#define PROPERTYDECL(name, type, membername) description& name(type t){prop.membername = t;return *this;}
				PROPERTYDECL(format, ::DXGI_FORMAT, Format)
				PROPERTYDECL(view_dimension, ::D3D11_UAV_DIMENSION, ViewDimension)
		#undef  PROPERTYDECL
		#define PROPERTYDECL(name, type, membername, viewdimension) description& name(type t){prop.ViewDimension = viewdimension;prop.membername = t;return *this;}
				PROPERTYDECL(buffer, const D3D11_BUFFER_UAV&, Buffer, ::D3D11_UAV_DIMENSION_BUFFER)
				PROPERTYDECL(texture1d, D3D11_TEX1D_UAV, Texture1D, ::D3D11_UAV_DIMENSION_TEXTURE1D)
				PROPERTYDECL(texture1d_array, const D3D11_TEX1D_ARRAY_UAV&, Texture1DArray, ::D3D11_UAV_DIMENSION_TEXTURE1DARRAY)
				PROPERTYDECL(texture2d, D3D11_TEX2D_UAV, Texture2D, ::D3D11_UAV_DIMENSION_TEXTURE2D)
				PROPERTYDECL(texture2d_array, const D3D11_TEX2D_ARRAY_UAV&, Texture2DArray, ::D3D11_UAV_DIMENSION_TEXTURE2DARRAY)
				PROPERTYDECL(texture3d, const D3D11_TEX3D_UAV&, Texture3D, ::D3D11_UAV_DIMENSION_TEXTURE3D)
		#undef  PROPERTYDECL
			};
			::D3D11_UNORDERED_ACCESS_VIEW_DESC get_desc(){::D3D11_UNORDERED_ACCESS_VIEW_DESC d;(*this)->GetDesc(&d);return d;}
		};
		struct command_list : child<::ID3D11CommandList>{
			using child::child;
			::UINT get_context_flags()const{return (*this)->GetContextFlags();}
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
				scoped_subresource(scoped_subresource&& t)noexcept:D3D11_MAPPED_SUBRESOURCE{std::move(t.get())}, c{t.c}, resource{std::move(t.resource)}, subresource_id{std::move(t.subresource_id)}{t.resource = nullptr;}
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
			void ia_set_input_layout(::ID3D11InputLayout* il){(*this)->IASetInputLayout(il);}
			template<typename D3D11InputLayout, std::enable_if_t<!std::is_same<std::decay_t<D3D11InputLayout>, ::ID3D11InputLayout>::value, std::nullptr_t> = nullptr>
			void ia_set_input_layout(D3D11InputLayout&& il){ia_set_input_layout(std::forward<D3D11InputLayout>(il).get());}
			void ia_set_vertex_buffers(::UINT start_slot, ::UINT num_buffers, ::ID3D11Buffer*const* vertex_buffers, const ::UINT* strides, const ::UINT* offsets){(*this)->IASetVertexBuffers(start_slot, num_buffers, vertex_buffers, strides, offsets);}
			void ia_set_vertex_buffer(::UINT start_slot, ::ID3D11Buffer* vertex_buffer, ::UINT strides, ::UINT offsets = 0){ia_set_vertex_buffers(start_slot, 1, &vertex_buffer, &strides, &offsets);}
			template<typename D3D11Buffer, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<D3D11Buffer>>, ID3D11Buffer>::value, std::nullptr_t> = nullptr>
			void ia_set_vertex_buffer(::UINT start_slot, D3D11Buffer&& vertex_buffers, ::UINT strides, ::UINT offsets = 0){ia_set_vertex_buffer(start_slot, std::forward<D3D11Buffer>(vertex_buffers).get(), strides, offsets);}
			void ia_set_primitive_topology(::D3D11_PRIMITIVE_TOPOLOGY t){(*this)->IASetPrimitiveTopology(t);}
			void vs_set_shader(::ID3D11VertexShader* vs){(*this)->VSSetShader(vs, nullptr, 0);}
			template<typename VertexShader, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<VertexShader>>, ID3D11VertexShader>::value, std::nullptr_t> = nullptr>
			void vs_set_shader(VertexShader&& vs){vs_set_shader(std::forward<VertexShader>(vs).get());}
			void rs_set_viewport(::UINT num_viewport, const ::D3D11_VIEWPORT* viewports){(*this)->RSSetViewports(num_viewport, viewports);}
			template<std::size_t N>
			void rs_set_viewport(const ::D3D11_VIEWPORT(&viewports)[N]){(*this)->RSSetViewports(N, viewports);}
			void rs_set_viewport(const std::vector<::D3D11_VIEWPORT> viewports){(*this)->RSSetViewports(static_cast<::UINT>(viewports.size()), viewports.data());}
			template<std::size_t N>
			void rs_set_viewport(const std::array<::D3D11_VIEWPORT, N> viewports){(*this)->RSSetViewports(static_cast<::UINT>(viewports.size()), viewports.data());}
			void ps_set_shader(::ID3D11PixelShader* ps){(*this)->PSSetShader(ps, nullptr, 0);}
			template<typename PixelShader, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<PixelShader>>, ID3D11PixelShader>::value, std::nullptr_t> = nullptr>
			void ps_set_shader(PixelShader&& ps){ps_set_shader(std::forward<PixelShader>(ps).get());}
			void ps_set_samplers(::UINT start_slot, ::UINT num_samplers, ID3D11SamplerState*const* samplers){(*this)->PSSetSamplers(start_slot, num_samplers, samplers);}
			void ps_set_sampler(::UINT start_slot, ::ID3D11SamplerState* sampler){ps_set_samplers(start_slot, 1, &sampler);}
			template<typename SamplerState, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<SamplerState>>, ::ID3D11SamplerState>::value, std::nullptr_t> = nullptr>
			void ps_set_sampler(::UINT start_slot, SamplerState&& ss){ps_set_sampler(start_slot, std::forward<SamplerState>(ss).get());}
			void ps_set_shader_resources(::UINT start_slot, ::UINT num_views, ID3D11ShaderResourceView*const* shader_resource_views){(*this)->PSSetShaderResources(start_slot, num_views, shader_resource_views);}
			void ps_set_shader_resource(::UINT start_slot, ::ID3D11ShaderResourceView* shader_resource_view){ps_set_shader_resources(start_slot, 1, &shader_resource_view);}
			template<typename ShaderResource, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<ShaderResource>>, ::ID3D11ShaderResourceView>::value, std::nullptr_t> = nullptr>
			void ps_set_shader_resource(::UINT start_slot, ShaderResource&& ss){ps_set_shader_resource(start_slot, std::forward<ShaderResource>(ss).get());}
			void om_set_render_targets(::UINT num_views, ::ID3D11RenderTargetView*const* render_target_views, ::ID3D11DepthStencilView* depth_stencil_view){(*this)->OMSetRenderTargets(num_views, render_target_views, depth_stencil_view);}
			template<typename DepthStencilView, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<DepthStencilView>>, ::ID3D11DepthStencilView>::value && !std::is_same<std::decay_t<DepthStencilView>, std::nullptr_t>::value, std::nullptr_t> = nullptr>
			void om_set_render_targets(::UINT num_views, ::ID3D11RenderTargetView*const* render_target_views, DepthStencilView&& depth_stencil_view){om_set_render_targets(num_views, render_target_views, std::forward<DepthStencilView>(depth_stencil_view).get());}
			template<std::size_t N, typename DepthStencilView>
			void om_set_render_targets(::ID3D11RenderTargetView*(&render_target_views)[N], DepthStencilView&& depth_stencil_view){om_set_render_targets(N, render_target_views, std::forward<DepthStencilView>(depth_stencil_view));}
			template<std::size_t N, typename DepthStencilView>
			void om_set_render_targets(std::array<::ID3D11RenderTargetView*, N>& render_target_views, DepthStencilView&& depth_stencil_view){om_set_render_targets(N, render_target_views.data(), std::forward<DepthStencilView>(depth_stencil_view));}
			template<typename DepthStencilView>
			void om_set_render_targets(std::vector<::ID3D11RenderTargetView*>& render_target_views, DepthStencilView&& depth_stencil_view){om_set_render_targets(render_target_views.size(), render_target_views.data(), std::forward<DepthStencilView>(depth_stencil_view));}
			template<typename DepthStencilView>
			void om_set_render_targets(::ID3D11RenderTargetView* render_target_view, DepthStencilView&& depth_stencil_view){om_set_render_targets(1, &render_target_view, std::forward<DepthStencilView>(depth_stencil_view));}
			template<typename RenderTargetView, typename DepthStencilView, std::enable_if_t<!std::is_same<std::decay_t<RenderTargetView>, std::nullptr_t>::value, std::nullptr_t> = nullptr>
			void om_set_render_targets(RenderTargetView&& render_target_view, DepthStencilView&& depth_stencil_view){om_set_render_targets(std::forward<RenderTargetView>(render_target_view).get(), std::forward<DepthStencilView>(depth_stencil_view));}
			void clear_render_target_view(::ID3D11RenderTargetView* render_target_view, const float (&color_rgba)[4]){(*this)->ClearRenderTargetView(render_target_view, color_rgba);}
			void clear_render_target_view(::ID3D11RenderTargetView* render_target_view, const std::array<float, 4>& color_rgba){(*this)->ClearRenderTargetView(render_target_view, color_rgba.data());}
			template<typename RenderTargetView, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<RenderTargetView>>, ::ID3D11RenderTargetView>::value, std::nullptr_t> = nullptr>
			void clear_render_target_view(RenderTargetView&& render_target_view, const float (&color_rgba)[4]){clear_render_target_view(std::forward<RenderTargetView>(render_target_view).get(), color_rgba);}
			template<typename RenderTargetView, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<RenderTargetView>>, ::ID3D11RenderTargetView>::value, std::nullptr_t> = nullptr>
			void clear_render_target_view(RenderTargetView&& render_target_view, const std::array<float, 4>& color_rgba){clear_render_target_view(std::forward<RenderTargetView>(render_target_view).get(), color_rgba);}
			void clear_depth_stencil_view(::ID3D11DepthStencilView* depth_stencil_view, ::FLOAT depth, ::UINT8 stencil){(*this)->ClearDepthStencilView(depth_stencil_view, ::D3D11_CLEAR_DEPTH | ::D3D11_CLEAR_STENCIL, depth, stencil);}
			template<typename DepthStencilView, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<DepthStencilView>>, ::ID3D11DepthStencilView>::value, std::nullptr_t> = nullptr>
			void clear_depth_stencil_view(DepthStencilView&& depth_stencil_view, ::FLOAT depth, ::UINT8 stencil){clear_depth_stencil_view(std::forward<DepthStencilView>(depth_stencil_view).get(), depth, stencil);}
			void clear_depth_stencil_view(::ID3D11DepthStencilView* depth_stencil_view, ::FLOAT depth){(*this)->ClearDepthStencilView(depth_stencil_view, ::D3D11_CLEAR_DEPTH, depth, 0);}
			template<typename DepthStencilView, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<DepthStencilView>>, ::ID3D11DepthStencilView>::value, std::nullptr_t> = nullptr>
			void clear_depth_stencil_view(DepthStencilView&& depth_stencil_view, ::FLOAT depth){clear_depth_stencil_view(std::forward<DepthStencilView>(depth_stencil_view).get(), depth);}
			void clear_depth_stencil_view(::ID3D11DepthStencilView* depth_stencil_view, ::UINT8 stencil){(*this)->ClearDepthStencilView(depth_stencil_view, ::D3D11_CLEAR_STENCIL, 1.f, stencil);}
			template<typename DepthStencilView, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<DepthStencilView>>, ::ID3D11DepthStencilView>::value, std::nullptr_t> = nullptr>
			void clear_depth_stencil_view(DepthStencilView&& depth_stencil_view, ::UINT8 stencil){clear_depth_stencil_view(std::forward<DepthStencilView>(depth_stencil_view).get(), stencil);}
			void draw(::UINT vertex_count, ::UINT start_vertex_location = 0u){(*this)->Draw(vertex_count, start_vertex_location);}
			void cs_set_shader(::ID3D11ComputeShader* cs){(*this)->CSSetShader(cs, nullptr, 0);}
			template<typename ComputeShader, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<ComputeShader>>, ID3D11ComputeShader>::value, std::nullptr_t> = nullptr>
			void cs_set_shader(ComputeShader&& cs){cs_set_shader(std::forward<ComputeShader>(cs).get());}
			void cs_set_samplers(::UINT start_slot, ::UINT num_samplers, ID3D11SamplerState*const* samplers){(*this)->CSSetSamplers(start_slot, num_samplers, samplers);}
			void cs_set_sampler(::UINT start_slot, ::ID3D11SamplerState* sampler){cs_set_samplers(start_slot, 1, &sampler);}
			template<typename SamplerState, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<SamplerState>>, ::ID3D11SamplerState>::value, std::nullptr_t> = nullptr>
			void cs_set_sampler(::UINT start_slot, SamplerState&& ss){cs_set_sampler(start_slot, std::forward<SamplerState>(ss).get());}
			void cs_set_shader_resources(::UINT start_slot, ::UINT num_views, ID3D11ShaderResourceView*const* shader_resource_views){(*this)->CSSetShaderResources(start_slot, num_views, shader_resource_views);}
			void cs_set_shader_resource(::UINT start_slot, ::ID3D11ShaderResourceView* shader_resource_view){cs_set_shader_resources(start_slot, 1, &shader_resource_view);}
			template<typename ShaderResource, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<ShaderResource>>, ::ID3D11ShaderResourceView>::value, std::nullptr_t> = nullptr>
			void cs_set_shader_resource(::UINT start_slot, ShaderResource&& ss){cs_set_shader_resource(start_slot, std::forward<ShaderResource>(ss).get());}
			void cs_set_unordered_access_views(::UINT start_slot, ::UINT num_views, ID3D11UnorderedAccessView*const* unordered_access_view_views, const ::UINT* initial_counts){(*this)->CSSetUnorderedAccessViews(start_slot, num_views, unordered_access_view_views, initial_counts);}
			void cs_set_unordered_access_view(::UINT start_slot, ::ID3D11UnorderedAccessView* unordered_access_view_view, ::UINT initial_count = -1){cs_set_unordered_access_views(start_slot, 1, &unordered_access_view_view, &initial_count);}
			template<typename UnorderedAccessView, std::enable_if_t<!std::is_same<std::remove_pointer_t<std::decay_t<UnorderedAccessView>>, ::ID3D11UnorderedAccessView>::value, std::nullptr_t> = nullptr>
			void cs_set_unordered_access_view(::UINT start_slot, UnorderedAccessView&& ss, ::UINT initial_count = -1){cs_set_unordered_access_view(start_slot, std::forward<UnorderedAccessView>(ss).get(), initial_count);}
			template<typename F>
			expected<command_list, hresult_error> finish_command_list(F&& f, bool restore_deferred_context_state = false){
				f(*this);
				return detail::convert_to_rich_interface<command_list>(com_create_resource<ID3D11CommandList>([&](ID3D11CommandList** ptr){return (*this)->FinishCommandList(restore_deferred_context_state ? TRUE : FALSE, ptr);}), _T(__FUNCTION__));
			}
			void execute_command_list(ID3D11CommandList* command_list, bool restore_context_state = true){(*this)->ExecuteCommandList(command_list, restore_context_state ? TRUE : FALSE);}
			template<typename CommandList>
			void execute_command_list(CommandList&& command_list, bool restore_context_state = true){execute_command_list(std::forward<CommandList>(command_list).get(), restore_context_state);}
			void dispatch(::UINT thread_group_count_x, ::UINT thread_group_count_y = 1u, ::UINT thread_group_count_z = 1u){(*this)->Dispatch(thread_group_count_x, thread_group_count_y, thread_group_count_z);}
			friend device;
			friend d3d;
		};
		class texture2d : public resource<ID3D11Texture2D>{
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
				description& bind_flags(std::underlying_type_t<D3D11_BIND_FLAG> t){return bind_flags(D3D11_BIND_FLAG(t));}
				description& cpu_access_flags(std::underlying_type_t<D3D11_CPU_ACCESS_FLAG> t){return cpu_access_flags(D3D11_CPU_ACCESS_FLAG(t));}
				description& misc_flags(std::underlying_type_t<D3D11_RESOURCE_MISC_FLAG> t){return misc_flags(D3D11_RESOURCE_MISC_FLAG(t));}
		#undef  PROPERTYDECL
			};
			D3D11_TEXTURE2D_DESC get_desc()const noexcept{D3D11_TEXTURE2D_DESC desc;(*this)->GetDesc(&desc);return desc;}
			texture2d(texture2d&&)noexcept = default;
			texture2d(IDXGISurface2* surf):resource{std::move(+query_interface<ID3D11Texture2D>(surf))}{}
			texture2d(IDXGIResource1* surf):resource{std::move(+query_interface<ID3D11Texture2D>(surf))}{}
			template<typename DXGISurface, std::enable_if_t<!std::is_base_of<IDXGISurface2, std::remove_pointer_t<std::decay_t<DXGISurface>>>::value && !std::is_base_of<IDXGIResource1, std::remove_pointer_t<std::decay_t<DXGISurface>>>::value, std::nullptr_t> = nullptr>
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
		):detail::resource<ID3D11Device1>(+create(flags)){}
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
		expected<render_target_view, hresult_error> create_render_target_view(ID3D11Resource* resource, const D3D11_RENDER_TARGET_VIEW_DESC& desc)const noexcept{
			return detail::convert_to_rich_interface<render_target_view>(com_create_resource<ID3D11RenderTargetView>([&](ID3D11RenderTargetView** ptr){return (*this)->CreateRenderTargetView(resource, &desc, ptr);}), _T(__FUNCTION__));
		}
		template<typename Resource, std::enable_if_t<!std::is_base_of<ID3D11Resource, std::remove_pointer_t<std::decay_t<Resource>>>::value, std::nullptr_t> = nullptr>
		expected<render_target_view, hresult_error> create_render_target_view(Resource&& r, const D3D11_RENDER_TARGET_VIEW_DESC& desc)const noexcept{return create_render_target_view(std::forward<Resource>(r).get(), desc);}
		expected<render_target_view, hresult_error> create_render_target_view(ID3D11Resource* resource)const noexcept{
			return detail::convert_to_rich_interface<render_target_view>(com_create_resource<ID3D11RenderTargetView>([&](ID3D11RenderTargetView** ptr){return (*this)->CreateRenderTargetView(resource, nullptr, ptr);}), _T(__FUNCTION__));
		}
		template<typename Resource, std::enable_if_t<!std::is_base_of<ID3D11Resource, std::remove_pointer_t<std::decay_t<Resource>>>::value, std::nullptr_t> = nullptr>
		expected<render_target_view, hresult_error> create_render_target_view(Resource&& r)const noexcept{return create_render_target_view(std::forward<Resource>(r).get());}
		expected<depth_stencil_view, hresult_error> create_depth_stencil_view(ID3D11Resource* res, const ::D3D11_DEPTH_STENCIL_VIEW_DESC& desc)const noexcept{
			return detail::convert_to_rich_interface<depth_stencil_view>(com_create_resource<ID3D11DepthStencilView>([&](ID3D11DepthStencilView** ptr){return (*this)->CreateDepthStencilView(res, &desc, ptr);}), _T(__FUNCTION__));
		}
		template<typename Resource, std::enable_if_t<!std::is_base_of<ID3D11Resource, std::remove_pointer_t<std::decay_t<Resource>>>::value, std::nullptr_t> = nullptr>
		expected<depth_stencil_view, hresult_error> create_depth_stencil_view(Resource&& r, const ::D3D11_DEPTH_STENCIL_VIEW_DESC& desc)const noexcept{return create_depth_stencil_view(std::forward<Resource>(r).get(), desc);}
		expected<buffer, hresult_error> create_buffer(const D3D11_BUFFER_DESC& buffer_desc, const D3D11_SUBRESOURCE_DATA& subresource_data)const noexcept{
			return detail::convert_to_rich_interface<buffer>(com_create_resource<ID3D11Buffer>([&](ID3D11Buffer** ptr){return (*this)->CreateBuffer(&buffer_desc, &subresource_data, ptr);}), _T(__FUNCTION__));
		}
		expected<input_layout, hresult_error> create_input_layout(const ::D3D11_INPUT_ELEMENT_DESC* element_descs, ::UINT num_elements, const void* shader_bytecode_with_input_signature, std::size_t bytecode_length)const noexcept{
			return detail::convert_to_rich_interface<input_layout>(com_create_resource<ID3D11InputLayout>([&](ID3D11InputLayout * *ptr){return (*this)->CreateInputLayout(element_descs, num_elements, shader_bytecode_with_input_signature, bytecode_length, ptr);}), _T(__FUNCTION__));
		}
		template<typename T, std::size_t N>
		expected<input_layout, hresult_error> create_input_layout(const ::D3D11_INPUT_ELEMENT_DESC* element_descs, ::UINT num_elements, const T(&shader_bytecode_with_input_signature)[N])const noexcept{
			return create_input_layout(element_descs, num_elements, shader_bytecode_with_input_signature, N);
		}
#if 0
		template<std::size_t M, typename T, std::size_t N>
		expected<input_layout, hresult_error> create_input_layout(const ::D3D11_INPUT_ELEMENT_DESC (&element_descs)[M], const T(&shader_bytecode_with_input_signature)[N])const noexcept{
			return create_input_layout(element_descs, static_cast<::UINT>(M), shader_bytecode_with_input_signature, N);
		}
#endif
		template<std::size_t M, typename T, std::size_t N>
		expected<input_layout, hresult_error> create_input_layout(const std::array<::D3D11_INPUT_ELEMENT_DESC, M>& element_descs, const T(&shader_bytecode_with_input_signature)[N])const noexcept{
			return create_input_layout(element_descs.data(), static_cast<::UINT>(element_descs.size()), shader_bytecode_with_input_signature, N);
		}
		template<typename T, std::size_t N>
		expected<input_layout, hresult_error> create_input_layout(const std::vector<::D3D11_INPUT_ELEMENT_DESC>& element_descs, const T(&shader_bytecode_with_input_signature)[N])const noexcept{
			return create_input_layout(element_descs.data(), static_cast<::UINT>(element_descs.size()), shader_bytecode_with_input_signature, N);
		}
		expected<vertex_shader, hresult_error> create_vertex_shader(const void* shader_bytecode, std::size_t bytecode_length)const noexcept{
			return detail::convert_to_rich_interface<vertex_shader>(com_create_resource<ID3D11VertexShader>([&](ID3D11VertexShader** ptr){return (*this)->CreateVertexShader(shader_bytecode, bytecode_length, nullptr, ptr);}), _T(__FUNCTION__));
		}
		template<typename T, std::size_t N>
		expected<vertex_shader, hresult_error> create_vertex_shader(const T(&shader_bytecode)[N])const noexcept{
			return create_vertex_shader(shader_bytecode, N);
		}
		expected<pixel_shader, hresult_error> create_pixel_shader(const void* shader_bytecode, std::size_t bytecode_length)const noexcept{
			return detail::convert_to_rich_interface<pixel_shader>(com_create_resource<ID3D11PixelShader>([&](ID3D11PixelShader** ptr){return (*this)->CreatePixelShader(shader_bytecode, bytecode_length, nullptr, ptr);}), _T(__FUNCTION__));
		}
		template<typename T, std::size_t N>
		expected<pixel_shader, hresult_error> create_pixel_shader(const T(&shader_bytecode)[N])const noexcept{
			return create_pixel_shader(shader_bytecode, N);
		}
		expected<compute_shader, hresult_error> create_compute_shader(const void* shader_bytecode, std::size_t bytecode_length)const noexcept{
			return detail::convert_to_rich_interface<compute_shader>(com_create_resource<ID3D11ComputeShader>([&](ID3D11ComputeShader** ptr){return (*this)->CreateComputeShader(shader_bytecode, bytecode_length, nullptr, ptr);}), _T(__FUNCTION__));
		}
		template<typename T, std::size_t N>
		expected<compute_shader, hresult_error> create_compute_shader(const T(&shader_bytecode)[N])const noexcept{
			return create_compute_shader(shader_bytecode, N);
		}
		expected<sampler_state, hresult_error> create_sampler_state(const ::D3D11_SAMPLER_DESC& desc = will::d3d::sampler_state::description{})const noexcept{
			return detail::convert_to_rich_interface<sampler_state>(com_create_resource<ID3D11SamplerState>([&](ID3D11SamplerState** ptr){return (*this)->CreateSamplerState(&desc, ptr);}), _T(__FUNCTION__));
		}
		expected<shader_resource_view, hresult_error> create_shader_resource_view(::ID3D11Resource* resource, const ::D3D11_SHADER_RESOURCE_VIEW_DESC& desc)const noexcept{
			return detail::convert_to_rich_interface<shader_resource_view>(com_create_resource<ID3D11ShaderResourceView>([&](ID3D11ShaderResourceView** ptr){return (*this)->CreateShaderResourceView(resource, &desc, ptr);}), _T(__FUNCTION__));
		}
		template<typename Resource, std::enable_if_t<!std::is_base_of<::ID3D11Resource, std::remove_pointer_t<std::decay_t<Resource>>>::value && !std::is_base_of<::IDXGIResource, std::remove_pointer_t<std::decay_t<Resource>>>::value, std::nullptr_t> = nullptr>
		expected<shader_resource_view, hresult_error> create_shader_resource_view(Resource&& resource, const ::D3D11_SHADER_RESOURCE_VIEW_DESC& desc)const noexcept{
			return create_shader_resource_view(std::forward<Resource>(resource).get(), desc);
		}
		expected<unordered_access_view, hresult_error> create_unordered_access_view(::ID3D11Resource* resource, const ::D3D11_UNORDERED_ACCESS_VIEW_DESC& desc)const noexcept{
			return detail::convert_to_rich_interface<unordered_access_view>(com_create_resource<ID3D11UnorderedAccessView>([&](ID3D11UnorderedAccessView** ptr){return (*this)->CreateUnorderedAccessView(resource, &desc, ptr);}), _T(__FUNCTION__));
		}
		template<typename Resource, std::enable_if_t<!std::is_base_of<::ID3D11Resource, std::remove_pointer_t<std::decay_t<Resource>>>::value && !std::is_base_of<::IDXGIResource, std::remove_pointer_t<std::decay_t<Resource>>>::value, std::nullptr_t> = nullptr>
		expected<unordered_access_view, hresult_error> create_unordered_access_view(Resource&& resource, const ::D3D11_UNORDERED_ACCESS_VIEW_DESC& desc)const noexcept{
			return create_unordered_access_view(std::forward<Resource>(resource).get(), desc);
		}
		static expected<device, hresult_error> create(std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT
#ifdef _DEBUG
			| D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS
#endif
		)noexcept{
#ifdef _DEBUG
			expected<device, hresult_error> dev =
#else
			return
#endif
			detail::convert_to_rich_interface<device>(com_create_resource<ID3D11Device>([&](ID3D11Device** ptr){return ::D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0u, D3D11_SDK_VERSION, ptr, nullptr, nullptr);}).bind([](ID3D11Device* ptr){auto ret = query_interface<ID3D11Device1>(ptr);if(ret)ptr->Release();return std::move(ret);}), _T(__FUNCTION__));
#ifdef _DEBUG
			if(dev){
				expected<com_ptr<ID3D11InfoQueue>, HRESULT> ptr = query_interface<ID3D11InfoQueue>(dev->get()).map([](ID3D11InfoQueue* ptr){return com_ptr<ID3D11InfoQueue>{std::move(ptr)};});
				if(ptr){
					(*ptr)->SetBreakOnSeverity(::D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
					(*ptr)->SetBreakOnSeverity(::D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
					(*ptr)->SetBreakOnSeverity(::D3D11_MESSAGE_SEVERITY_WARNING, TRUE);
				}
			}
			return dev;
#endif
		}
	};
	using subresource = device::subresource;
	using texture2d = device::texture2d;
	using render_target_view = device::render_target_view;
	using buffer = device::buffer;
	using depth_stencil_view = device::depth_stencil_view;
	using input_layout = device::input_layout;
	using vertex_shader = device::vertex_shader;
	using pixel_shader = device::pixel_shader;
	using compute_shader = device::compute_shader;
	using sampler_state = device::sampler_state;
	using shader_resource_view = device::shader_resource_view;
	using unordered_access_view = device::unordered_access_view;
	using command_list = device::command_list;
};
}