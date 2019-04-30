//Copyright (C) 2014-2019 I
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
	struct dxgi_object : detail::resource<I>{
		using resource::resource;
	protected:
		template<typename T>
		expected<T, hresult_error> get_parent(const TCHAR* func_name)const{return detail::convert_to_rich_interface<T>(com_create_resource<detail::get_interface<T>>([&](detail::get_interface<T>** ptr){return (*this)->GetParent(__uuidof(detail::get_interface<T>), reinterpret_cast<void**>(ptr));}), func_name);}
	};
public:
	struct format{
		format() = delete;
		static constexpr bool is_typeless(::DXGI_FORMAT t)noexcept{
			switch(t){
			case ::DXGI_FORMAT_R32G32B32A32_TYPELESS:
			case ::DXGI_FORMAT_R32G32B32_TYPELESS:
			case ::DXGI_FORMAT_R16G16B16A16_TYPELESS:
			case ::DXGI_FORMAT_R32G32_TYPELESS:
			case ::DXGI_FORMAT_R32G8X24_TYPELESS:
			case ::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
			case ::DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			case ::DXGI_FORMAT_R10G10B10A2_TYPELESS:
			case ::DXGI_FORMAT_R8G8B8A8_TYPELESS:
			case ::DXGI_FORMAT_R16G16_TYPELESS:
			case ::DXGI_FORMAT_R32_TYPELESS:
			case ::DXGI_FORMAT_R24G8_TYPELESS:
			case ::DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
			case ::DXGI_FORMAT_X24_TYPELESS_G8_UINT:
			case ::DXGI_FORMAT_R8G8_TYPELESS:
			case ::DXGI_FORMAT_R16_TYPELESS:
			case ::DXGI_FORMAT_R8_TYPELESS:
			case ::DXGI_FORMAT_BC1_TYPELESS:
			case ::DXGI_FORMAT_BC2_TYPELESS:
			case ::DXGI_FORMAT_BC3_TYPELESS:
			case ::DXGI_FORMAT_BC4_TYPELESS:
			case ::DXGI_FORMAT_BC5_TYPELESS:
			case ::DXGI_FORMAT_B8G8R8A8_TYPELESS:
			case ::DXGI_FORMAT_B8G8R8X8_TYPELESS:
			case ::DXGI_FORMAT_BC6H_TYPELESS:
			case ::DXGI_FORMAT_BC7_TYPELESS:
				return true;
			default:
				return false;
			}
		}
		static constexpr ::DXGI_FORMAT to_unorm(::DXGI_FORMAT t)noexcept{
			switch(t){
			case ::DXGI_FORMAT_R16G16B16A16_TYPELESS:
			case ::DXGI_FORMAT_R16G16B16A16_FLOAT:
			case ::DXGI_FORMAT_R16G16B16A16_UNORM:
			case ::DXGI_FORMAT_R16G16B16A16_UINT:
			case ::DXGI_FORMAT_R16G16B16A16_SNORM:
			case ::DXGI_FORMAT_R16G16B16A16_SINT:
				return ::DXGI_FORMAT_R16G16B16A16_UNORM;
			case ::DXGI_FORMAT_R10G10B10A2_TYPELESS:
			case ::DXGI_FORMAT_R10G10B10A2_UNORM:
			case ::DXGI_FORMAT_R10G10B10A2_UINT:
				return ::DXGI_FORMAT_R10G10B10A2_UNORM;
			case ::DXGI_FORMAT_R8G8B8A8_TYPELESS:
			case ::DXGI_FORMAT_R8G8B8A8_UNORM:
			case ::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			case ::DXGI_FORMAT_R8G8B8A8_UINT:
			case ::DXGI_FORMAT_R8G8B8A8_SNORM:
			case ::DXGI_FORMAT_R8G8B8A8_SINT:
				return ::DXGI_FORMAT_R8G8B8A8_UNORM;
			case ::DXGI_FORMAT_R16G16_TYPELESS:
			case ::DXGI_FORMAT_R16G16_FLOAT:
			case ::DXGI_FORMAT_R16G16_UNORM:
			case ::DXGI_FORMAT_R16G16_UINT:
			case ::DXGI_FORMAT_R16G16_SNORM:
			case ::DXGI_FORMAT_R16G16_SINT:
				return ::DXGI_FORMAT_R16G16_UNORM;
			case ::DXGI_FORMAT_R8G8_TYPELESS:
			case ::DXGI_FORMAT_R8G8_UNORM:
			case ::DXGI_FORMAT_R8G8_UINT:
			case ::DXGI_FORMAT_R8G8_SNORM:
			case ::DXGI_FORMAT_R8G8_SINT:
				return ::DXGI_FORMAT_R8G8_UNORM;
			case ::DXGI_FORMAT_R16_TYPELESS:
			case ::DXGI_FORMAT_R16_FLOAT:
			case ::DXGI_FORMAT_D16_UNORM:
			case ::DXGI_FORMAT_R16_UNORM:
			case ::DXGI_FORMAT_R16_UINT:
			case ::DXGI_FORMAT_R16_SNORM:
			case ::DXGI_FORMAT_R16_SINT:
				return ::DXGI_FORMAT_R16_UNORM;
			case ::DXGI_FORMAT_R8_TYPELESS:
			case ::DXGI_FORMAT_R8_UNORM:
			case ::DXGI_FORMAT_R8_UINT:
			case ::DXGI_FORMAT_R8_SNORM:
			case ::DXGI_FORMAT_R8_SINT:
				return ::DXGI_FORMAT_R8_UNORM;
			case ::DXGI_FORMAT_BC1_TYPELESS:
			case ::DXGI_FORMAT_BC1_UNORM:
				return ::DXGI_FORMAT_BC1_UNORM;
			case ::DXGI_FORMAT_BC2_TYPELESS:
			case ::DXGI_FORMAT_BC2_UNORM:
				return ::DXGI_FORMAT_BC2_UNORM;
			case ::DXGI_FORMAT_BC3_TYPELESS:
			case ::DXGI_FORMAT_BC3_UNORM:
				return ::DXGI_FORMAT_BC3_UNORM;
			case ::DXGI_FORMAT_BC4_TYPELESS:
			case ::DXGI_FORMAT_BC4_UNORM:
			case ::DXGI_FORMAT_BC4_SNORM:
				return ::DXGI_FORMAT_BC4_UNORM;
			case ::DXGI_FORMAT_BC5_TYPELESS:
			case ::DXGI_FORMAT_BC5_UNORM:
			case ::DXGI_FORMAT_BC5_SNORM:
				return ::DXGI_FORMAT_BC5_UNORM;
			case ::DXGI_FORMAT_B8G8R8A8_TYPELESS:
				return ::DXGI_FORMAT_B8G8R8A8_UNORM;
			case ::DXGI_FORMAT_B8G8R8X8_TYPELESS:
				return ::DXGI_FORMAT_B8G8R8X8_UNORM;
			case ::DXGI_FORMAT_BC7_TYPELESS:
			case ::DXGI_FORMAT_BC7_UNORM:
				return ::DXGI_FORMAT_BC7_UNORM;
			default:
				return t;
			}
		}
		static constexpr bool same_family(::DXGI_FORMAT a, ::DXGI_FORMAT b)noexcept{
			switch(a){
			case ::DXGI_FORMAT_R32G32B32A32_TYPELESS:
			case ::DXGI_FORMAT_R32G32B32A32_FLOAT:
			case ::DXGI_FORMAT_R32G32B32A32_UINT:
			case ::DXGI_FORMAT_R32G32B32A32_SINT:
				return ::DXGI_FORMAT_R32G32B32A32_TYPELESS <= b && b <= ::DXGI_FORMAT_R32G32B32A32_SINT;
			case ::DXGI_FORMAT_R32G32B32_TYPELESS:
			case ::DXGI_FORMAT_R32G32B32_FLOAT:
			case ::DXGI_FORMAT_R32G32B32_UINT:
			case ::DXGI_FORMAT_R32G32B32_SINT:
				return ::DXGI_FORMAT_R32G32B32A32_TYPELESS <= b && b <= ::DXGI_FORMAT_R32G32B32A32_SINT;
			case ::DXGI_FORMAT_R16G16B16A16_TYPELESS:
			case ::DXGI_FORMAT_R16G16B16A16_FLOAT:
			case ::DXGI_FORMAT_R16G16B16A16_UNORM:
			case ::DXGI_FORMAT_R16G16B16A16_UINT:
			case ::DXGI_FORMAT_R16G16B16A16_SNORM:
			case ::DXGI_FORMAT_R16G16B16A16_SINT:
				return ::DXGI_FORMAT_R16G16B16A16_TYPELESS <= b && b <= ::DXGI_FORMAT_R16G16B16A16_SINT;
			case ::DXGI_FORMAT_R32G32_TYPELESS:
			case ::DXGI_FORMAT_R32G32_FLOAT:
			case ::DXGI_FORMAT_R32G32_UINT:
			case ::DXGI_FORMAT_R32G32_SINT:
				return ::DXGI_FORMAT_R32G32_TYPELESS <= b && b <= ::DXGI_FORMAT_R32G32_SINT;
			case ::DXGI_FORMAT_R32G8X24_TYPELESS:
			case ::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			case ::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
			case ::DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
				return ::DXGI_FORMAT_R32G8X24_TYPELESS <= b && b <= ::DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
			case ::DXGI_FORMAT_R10G10B10A2_TYPELESS:
			case ::DXGI_FORMAT_R10G10B10A2_UNORM:
			case ::DXGI_FORMAT_R10G10B10A2_UINT:
				return ::DXGI_FORMAT_R10G10B10A2_TYPELESS <= b && b <= ::DXGI_FORMAT_R10G10B10A2_UINT;
			case ::DXGI_FORMAT_R8G8B8A8_TYPELESS:
			case ::DXGI_FORMAT_R8G8B8A8_UNORM:
			case ::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			case ::DXGI_FORMAT_R8G8B8A8_UINT:
			case ::DXGI_FORMAT_R8G8B8A8_SNORM:
			case ::DXGI_FORMAT_R8G8B8A8_SINT:
				return ::DXGI_FORMAT_R8G8B8A8_TYPELESS <= b && b <= ::DXGI_FORMAT_R8G8B8A8_SINT;
			case ::DXGI_FORMAT_R16G16_TYPELESS:
			case ::DXGI_FORMAT_R16G16_FLOAT:
			case ::DXGI_FORMAT_R16G16_UNORM:
			case ::DXGI_FORMAT_R16G16_UINT:
			case ::DXGI_FORMAT_R16G16_SNORM:
			case ::DXGI_FORMAT_R16G16_SINT:
				return ::DXGI_FORMAT_R16G16_TYPELESS <= b && b <= ::DXGI_FORMAT_R16G16_SINT;
			case ::DXGI_FORMAT_R32_TYPELESS:
			case ::DXGI_FORMAT_D32_FLOAT:
			case ::DXGI_FORMAT_R32_FLOAT:
			case ::DXGI_FORMAT_R32_UINT:
			case ::DXGI_FORMAT_R32_SINT:
				return ::DXGI_FORMAT_R32_TYPELESS <= b && b <= ::DXGI_FORMAT_R32_SINT;
			case ::DXGI_FORMAT_R24G8_TYPELESS:
			case ::DXGI_FORMAT_D24_UNORM_S8_UINT:
			case ::DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
			case ::DXGI_FORMAT_X24_TYPELESS_G8_UINT:
				return ::DXGI_FORMAT_R24G8_TYPELESS <= b && b <= ::DXGI_FORMAT_X24_TYPELESS_G8_UINT;
			case ::DXGI_FORMAT_R8G8_TYPELESS:
			case ::DXGI_FORMAT_R8G8_UNORM:
			case ::DXGI_FORMAT_R8G8_UINT:
			case ::DXGI_FORMAT_R8G8_SNORM:
			case ::DXGI_FORMAT_R8G8_SINT:
				return ::DXGI_FORMAT_R8G8_TYPELESS <= b && b <= ::DXGI_FORMAT_R8G8_SINT;
			case ::DXGI_FORMAT_R16_TYPELESS:
			case ::DXGI_FORMAT_R16_FLOAT:
			case ::DXGI_FORMAT_D16_UNORM:
			case ::DXGI_FORMAT_R16_UNORM:
			case ::DXGI_FORMAT_R16_UINT:
			case ::DXGI_FORMAT_R16_SNORM:
			case ::DXGI_FORMAT_R16_SINT:
				return ::DXGI_FORMAT_R16_TYPELESS <= b && b <= ::DXGI_FORMAT_R16_SINT;
			case ::DXGI_FORMAT_R8_TYPELESS:
			case ::DXGI_FORMAT_R8_UNORM:
			case ::DXGI_FORMAT_R8_UINT:
			case ::DXGI_FORMAT_R8_SNORM:
			case ::DXGI_FORMAT_R8_SINT:
			case ::DXGI_FORMAT_A8_UNORM:
				return ::DXGI_FORMAT_R8_TYPELESS <= b && b <= ::DXGI_FORMAT_A8_UNORM;
			case ::DXGI_FORMAT_R8G8_B8G8_UNORM:
			case ::DXGI_FORMAT_G8R8_G8B8_UNORM:
				return b == ::DXGI_FORMAT_R8G8_B8G8_UNORM || b == ::DXGI_FORMAT_G8R8_G8B8_UNORM;
			case ::DXGI_FORMAT_BC1_TYPELESS:
			case ::DXGI_FORMAT_BC1_UNORM:
			case ::DXGI_FORMAT_BC1_UNORM_SRGB:
				return ::DXGI_FORMAT_BC1_TYPELESS <= b && b <= ::DXGI_FORMAT_BC1_UNORM_SRGB;
			case ::DXGI_FORMAT_BC2_TYPELESS:
			case ::DXGI_FORMAT_BC2_UNORM:
			case ::DXGI_FORMAT_BC2_UNORM_SRGB:
				return ::DXGI_FORMAT_BC2_TYPELESS <= b && b <= ::DXGI_FORMAT_BC2_UNORM_SRGB;
			case ::DXGI_FORMAT_BC3_TYPELESS:
			case ::DXGI_FORMAT_BC3_UNORM:
			case ::DXGI_FORMAT_BC3_UNORM_SRGB:
				return ::DXGI_FORMAT_BC3_TYPELESS <= b && b <= ::DXGI_FORMAT_BC3_UNORM_SRGB;
			case ::DXGI_FORMAT_BC4_TYPELESS:
			case ::DXGI_FORMAT_BC4_UNORM:
			case ::DXGI_FORMAT_BC4_SNORM:
				return ::DXGI_FORMAT_BC4_TYPELESS <= b && b <= ::DXGI_FORMAT_BC4_SNORM;
			case ::DXGI_FORMAT_BC5_TYPELESS:
			case ::DXGI_FORMAT_BC5_UNORM:
			case ::DXGI_FORMAT_BC5_SNORM:
				return ::DXGI_FORMAT_BC5_TYPELESS <= b && b <= ::DXGI_FORMAT_BC5_SNORM;
			case ::DXGI_FORMAT_B8G8R8A8_UNORM:
			case ::DXGI_FORMAT_B8G8R8X8_UNORM:
			case ::DXGI_FORMAT_B8G8R8A8_TYPELESS:
			case ::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			case ::DXGI_FORMAT_B8G8R8X8_TYPELESS:
			case ::DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
				return b == ::DXGI_FORMAT_B8G8R8A8_UNORM || b == ::DXGI_FORMAT_B8G8R8X8_UNORM || (::DXGI_FORMAT_B8G8R8A8_TYPELESS <= b && b <= ::DXGI_FORMAT_B8G8R8X8_UNORM_SRGB);
			case ::DXGI_FORMAT_BC6H_TYPELESS:
			case ::DXGI_FORMAT_BC6H_UF16:
			case ::DXGI_FORMAT_BC6H_SF16:
				return ::DXGI_FORMAT_BC6H_TYPELESS <= b && b <= ::DXGI_FORMAT_BC6H_SF16;
			case ::DXGI_FORMAT_BC7_TYPELESS:
			case ::DXGI_FORMAT_BC7_UNORM:
			case ::DXGI_FORMAT_BC7_UNORM_SRGB:
				return ::DXGI_FORMAT_BC7_TYPELESS <= b && b <= ::DXGI_FORMAT_BC7_UNORM_SRGB;
			default:
				return a == b;
			}
		}
	};
	class enum_adapters_range;
	struct output;
	class adapter : public dxgi_object<IDXGIAdapter2>{
		using dxgi_object::dxgi_object;
		friend class dxgi::enum_adapters_range;
	public:
		expected<dxgi, hresult_error> get_factory(){return get_parent<dxgi>(_T(__FUNCTION__));}
		class device : public detail::resource<IDXGIDevice2>{
			using resource::resource;
			template<typename I>
			struct dxgi_device_sub_object : dxgi_object<I>{
				using dxgi_object::dxgi_object;
			protected:
				template<typename T = device>
				expected<T, hresult_error> get_device_impl(const TCHAR* func_name)const{return detail::convert_to_rich_interface<T>(com_create_resource<detail::get_interface<T>>([&](detail::get_interface<T>** ptr){return (*this)->GetDevice(__uuidof(detail::get_interface<T>), reinterpret_cast<void**>(ptr));}), func_name);}
			};
		public:
			class surface : public dxgi_device_sub_object<IDXGISurface2>{
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
					~device_context_handle()noexcept{auto _ [[maybe_unused]] = release();}
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
					scoped_readonly_mapped_rect(scoped_readonly_mapped_rect&& t)noexcept: ::DXGI_MAPPED_RECT{std::move(static_cast<::DXGI_MAPPED_RECT&>(t))}, sf{t.sf}{t.pBits = nullptr;}
					~scoped_readonly_mapped_rect()noexcept{if(pBits)auto _ [[maybe_unused]] = sf.unmap();}
					friend surface;
				};
				class scoped_mapped_rect : public ::DXGI_MAPPED_RECT{
					surface& sf;
					scoped_mapped_rect(::DXGI_MAPPED_RECT&& s, surface& surf)noexcept: ::DXGI_MAPPED_RECT{std::move(s)}, sf{surf}{}
				public:
					scoped_mapped_rect(surface& surf, UINT option): ::DXGI_MAPPED_RECT(+surf.map(option)), sf{surf}{}
					scoped_mapped_rect(scoped_mapped_rect&& t)noexcept: ::DXGI_MAPPED_RECT{std::move(static_cast<::DXGI_MAPPED_RECT&>(t))}, sf{t.sf}{t.pBits = nullptr;}
					~scoped_mapped_rect()noexcept{if(pBits)auto _ [[maybe_unused]] = sf.unmap();}
					friend surface;
				};
				using dxgi_device_sub_object::dxgi_device_sub_object;
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
			struct resource : dxgi_device_sub_object<IDXGIResource1>{
				using dxgi_device_sub_object::dxgi_device_sub_object;
				expected<device, hresult_error> get_device()const{return get_device_impl(_T(__FUNCTION__));}
				expected<::UINT, hresult_error> get_eviction_priority()const{
					::UINT t;
					::HRESULT hr = (*this)->GetEvictionPriority(&t);
					if(FAILED(hr))
						return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
					return t;
				}
				expected<::DXGI_USAGE, hresult_error> get_usage()const{
					::DXGI_USAGE t;
					::HRESULT hr = (*this)->GetUsage(&t);
					if(FAILED(hr))
						return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
					return t;
				}
				expected<void, hresult_error> set_eviction_priority(::UINT eviction_priority)const{
					::HRESULT hr = (*this)->SetEvictionPriority(eviction_priority);
					if(FAILED(hr))
						return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
					return {};
				}
				//expected<::HANDLE, hresult_error> create_shared_handle(const SECURITY_ATTRIBUTES&, DWORD, LPCWSTR);
				//expected<surface, hresult_error> create_subresource_surface(UINT index);
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
		class enum_outputs_range{
			adapter& a;
			explicit enum_outputs_range(adapter& ada):a{ada}{}
			friend class adapter;
		public:
			class iterator{
				adapter& a;
				int i;
				will::com_ptr<IDXGIOutput> tmp;
				void enum_outputs(){
					IDXGIOutput* ptr;
					HRESULT hr = a->EnumOutputs(static_cast<UINT>(i), &ptr);
					if(hr == DXGI_ERROR_NOT_FOUND){
						i = -i;
						ptr = nullptr;
					}
					tmp.reset(std::move(ptr));
				}
			public:
				explicit iterator(adapter& ada, UINT m = 0):a{ada}, i{static_cast<int>(m)}, tmp{nullptr}{}
				iterator(iterator&&) = default;
				iterator& operator++(){
					if(i < 0)
						return *this;
					++i;
					enum_outputs();
					return *this;
				}
				iterator operator++(int){
					iterator it{a, static_cast<UINT>(i)};
					++(*this);
					return it;
				}
				expected<output, hresult_error> operator*(){
					if(!tmp)
						enum_outputs();
					return tmp.as<IDXGIOutput1>().map([](will::com_ptr<IDXGIOutput1>&& x){return output{std::move(x)};});
				}
				friend constexpr bool operator==(const iterator& lhs, const iterator& rhs)noexcept{
					return lhs.i == rhs.i || (lhs.i < 0 && rhs.i < 0);
				}
				friend constexpr bool operator!=(const iterator& lhs, const iterator& rhs)noexcept{
					return !(lhs == rhs);
				}
			};
			iterator begin(){return iterator{a, 0};}
			iterator end(){return iterator{a, static_cast<UINT>(std::numeric_limits<int>::min())};}
		};
		enum_outputs_range enum_outputs(){return enum_outputs_range{*this};}
		template<typename T>
		expected<::LARGE_INTEGER, hresult_error> check_interface_support()const{
			::LARGE_INTEGER t;
			const HRESULT hr = (*this)->CheckInterfaceSupport(__uuidof(T), &t);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCION__), hr);
			return t;
		}
		expected<::DXGI_ADAPTER_DESC2, hresult_error> get_desc()const{
			::DXGI_ADAPTER_DESC2 t;
			const HRESULT hr = (*this)->GetDesc2(&t);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return t;
		}
	};
	using device = adapter::device;
	using surface = device::surface;
	using resource = device::resource;
	struct output : dxgi_object<::IDXGIOutput1>{
		using dxgi_object::dxgi_object;
		expected<::DXGI_MODE_DESC1, hresult_error> find_closest_matching_mode(const ::DXGI_MODE_DESC1& mode_to_match, ::IUnknown* d3ddevice = nullptr)const{
			::DXGI_MODE_DESC1 match;
			auto hr = (*this)->FindClosestMatchingMode1(&mode_to_match, &match, d3ddevice);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return match;
		}
		template<typename D3DDevice, std::enable_if_t<!std::is_pointer<std::decay_t<D3DDevice>>::value, std::nullptr_t> = nullptr>
		expected<::DXGI_MODE_DESC1, hresult_error> find_closest_matching_mode(const ::DXGI_MODE_DESC1& mode_to_match, D3DDevice&& d3ddevice)const{
			return find_closest_matching_mode(mode_to_match, d3ddevice.get());
		}
		expected<::DXGI_OUTPUT_DESC, hresult_error> get_desc()const{
			::DXGI_OUTPUT_DESC d;
			::HRESULT hr = (*this)->GetDesc(&d);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return d;
		}
		struct display_mode{
			::UINT mode;
			::DXGI_MODE_DESC1 desc;
		};
		expected<std::vector<display_mode>, hresult_error> get_display_mode_list(::DXGI_FORMAT format, ::UINT flags)const{
			UINT size;
#pragma warning(push)
#pragma warning(disable:6001)
			HRESULT hr = (*this)->GetDisplayModeList1(format, flags, &size, nullptr);
#pragma warning(pop)
			std::vector<display_mode> ret(size);
			for(auto&& x : ret){
				hr = (*this)->GetDisplayModeList1(format, flags, &x.mode, &x.desc);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			}
			return ret;
		}
		expected<::DXGI_FRAME_STATISTICS, hresult_error> get_frame_statistics()const{
			::DXGI_FRAME_STATISTICS s;
			::HRESULT hr = (*this)->GetFrameStatistics(&s);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return s;
		}
		expected<::DXGI_GAMMA_CONTROL, hresult_error> get_gamma_control()const{
			::DXGI_GAMMA_CONTROL g;
			::HRESULT hr = (*this)->GetGammaControl(&g);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return g;
		}
		expected<::DXGI_GAMMA_CONTROL_CAPABILITIES, hresult_error> get_gamma_control_capabilities()const{
			::DXGI_GAMMA_CONTROL_CAPABILITIES g;
			::HRESULT hr = (*this)->GetGammaControlCapabilities(&g);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return g;
		}
		expected<void, hresult_error> set_display_surface(::IDXGISurface* surf){
			::HRESULT hr = (*this)->SetDisplaySurface(surf);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return {};
		}
		template<typename DXGISurface, std::enable_if_t<!std::is_base_of<::IDXGISurface, std::remove_pointer_t<std::decay_t<DXGISurface>>>::value, std::nullptr_t> = nullptr>
		expected<void, hresult_error> set_display_surface(DXGISurface&& surf){
			return set_display_surface(surf.get());
		}
		expected<void, hresult_error> set_gamma_control(const DXGI_GAMMA_CONTROL& d){
			::HRESULT hr = (*this)->SetGammaControl(&d);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return {};
		}
		expected<void, hresult_error> wait_for_v_blank(){
			::HRESULT hr = (*this)->WaitForVBlank();
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return {};
		}
		class duplication : public dxgi_object<IDXGIOutputDuplication>{
			struct frame{
				::DXGI_OUTDUPL_FRAME_INFO info;
				will::dxgi::resource desktop_resource;
			};
			expected<frame, hresult_error> acquire_next_frame(std::chrono::milliseconds timeout){
				::DXGI_OUTDUPL_FRAME_INFO info;
				::IDXGIResource* ptr;
				::HRESULT hr = (*this)->AcquireNextFrame(static_cast<UINT>(timeout.count()), &info, &ptr);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return com_ptr<IDXGIResource>{std::move(ptr)}.as<IDXGIResource1>().map([&info](com_ptr<IDXGIResource1>&& p){return frame{info, will::dxgi::resource{std::move(p)}};});
			}
			expected<void, hresult_error> release_frame(){
				::HRESULT hr = (*this)->ReleaseFrame();
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return {};
			}
			expected<std::vector<::RECT>, hresult_error> get_frame_dirty_rects()const{
				::UINT size;
				::HRESULT hr;
				{
					::RECT dummy;
					hr = (*this)->GetFrameDirtyRects(0, &dummy, &size);
					if(FAILED(hr))
						return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				}
				std::vector<::RECT> ret(size);
				hr = (*this)->GetFrameDirtyRects(size, ret.data(), &size);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return ret;
			}
			expected<std::vector<::DXGI_OUTDUPL_MOVE_RECT>, hresult_error> get_frame_move_rects()const{
				::UINT size;
				::HRESULT hr;
				{
					::DXGI_OUTDUPL_MOVE_RECT dummy;
					hr = (*this)->GetFrameMoveRects(0, &dummy, &size);
					if(FAILED(hr))
						return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				}
				std::vector<::DXGI_OUTDUPL_MOVE_RECT> ret(size);
				hr = (*this)->GetFrameMoveRects(size, ret.data(), &size);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return ret;
			}
			//get_frame_point_shape
			expected<::DXGI_MAPPED_RECT, hresult_error> map_desktop_surface(){
				::DXGI_MAPPED_RECT m;
				::HRESULT hr = (*this)->MapDesktopSurface(&m);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return m;
			}
			expected<void, hresult_error> unmap_desktop_surface(){
				::HRESULT hr = (*this)->UnMapDesktopSurface();
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return {};
			}
			template<typename F>
			expected<std::invoke_result_t<F, ::DXGI_MAPPED_RECT>, hresult_error> apply_desktop_surface(F&& f){
				auto e1 = map_desktop_surface();
				if(!e1)
					return make_unexpected(e1.error());
				auto result = f(*e1);
				if e2 = unmap_desktop_surface();
				if(!e2)
					return make_unexpected(e2.error());
				return result;
			}
		public:
			using dxgi_object::dxgi_object;
			::DXGI_OUTDUPL_DESC get_desc()const{
				::DXGI_OUTDUPL_DESC d;
				(*this)->GetDesc(&d);
				return d;
			}
			class scoped_frame{
				duplication& d;
				frame f;
			public:
				scoped_frame(duplication& d, frame&& f):d{d}, f{std::move(f)}{}
				scoped_frame(const scoped_frame&) = delete;
				scoped_frame(scoped_frame&& other)noexcept:d{other.d}, f{std::move(other.f)}{}
				::DXGI_OUTDUPL_FRAME_INFO& info(){return f.info;}
				will::dxgi::resource& desktop_resource(){return f.desktop_resource;}
				expected<std::vector<::RECT>, hresult_error> get_frame_dirty_rects()const{return d.get_frame_dirty_rects();}
				expected<std::vector<::DXGI_OUTDUPL_MOVE_RECT>, hresult_error> get_frame_move_rects()const{return d.get_frame_move_rects();}
				template<typename F>
				expected<std::invoke_result_t<F, ::DXGI_MAPPED_RECT>, hresult_error> apply_desktop_surface(F&& f){return d.apply_desktop_surface(std::forward<F>(f));}
				~scoped_frame(){if(f.desktop_resource)auto _ [[maybe_unused]] = d.release_frame();}
			};
			expected<scoped_frame, hresult_error> acquire_frame(std::chrono::milliseconds timeout){
				return acquire_next_frame(timeout).map([&](frame&& f){return scoped_frame{*this, std::move(f)};});
			}
		};
		expected<duplication, hresult_error> duplicate_output(IUnknown* d3ddevice)const{
			return will::com_create_resource<IDXGIOutputDuplication>([&](IDXGIOutputDuplication** t){return (*this)->DuplicateOutput(d3ddevice, t);})
				.map([&](IDXGIOutputDuplication* dup){return duplication{std::move(dup)};})
				.emap([](HRESULT hr){return make_unexpected<hresult_error>(_T("will::dxgi::output::duplicate_output"), hr);});
		}
		template<typename D3DDevice, std::enable_if_t<!std::is_pointer<std::decay_t<D3DDevice>>::value, std::nullptr_t> = nullptr>
		expected<duplication, hresult_error> duplicate_output(D3DDevice&& d3ddevice)const{
			return duplicate_output(d3ddevice.get());
		}
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
	struct swap_chain : dxgi_object<IDXGISwapChain1>{
		struct description : detail::property<DXGI_SWAP_CHAIN_DESC1>{
			description():property(){prop.Format = DXGI_FORMAT_R8G8B8A8_UNORM; prop.SampleDesc.Count = 1; prop.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; prop.BufferCount = 2;}
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
		using dxgi_object::dxgi_object;
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
	class enum_adapters_range{
		dxgi& f;
		explicit enum_adapters_range(dxgi& factory):f{factory}{}
		friend class dxgi;
	public:
		class iterator{
			dxgi& f;
			int i;
			will::com_ptr<IDXGIAdapter1> tmp;
			void enum_adapters(){
				IDXGIAdapter1* ptr;
				HRESULT hr = f->EnumAdapters1(static_cast<UINT>(i), &ptr);
				if(hr == DXGI_ERROR_NOT_FOUND){
					i = -i;
					ptr = nullptr;
				}
				tmp.reset(std::move(ptr));
			}
		public:
			explicit iterator(dxgi& factory, UINT m = 0):f{factory}, i{static_cast<int>(m)}, tmp{nullptr}{}
			iterator(iterator&&) = default;
			iterator& operator++(){
				if(i < 0)
					return *this;
				++i;
				enum_adapters();
				return *this;
			}
			iterator operator++(int){
				iterator it{f, static_cast<UINT>(i)};
				++(*this);
				return it;
			}
			expected<adapter, hresult_error> operator*(){
				if(!tmp)
					enum_adapters();
				return tmp.as<IDXGIAdapter2>().map([](will::com_ptr<IDXGIAdapter2>&& x){return adapter{std::move(x)};});
			}
			friend constexpr bool operator==(const iterator& lhs, const iterator& rhs)noexcept{
				return lhs.i == rhs.i || (lhs.i < 0 && rhs.i < 0);
			}
			friend constexpr bool operator!=(const iterator& lhs, const iterator& rhs)noexcept{
				return !(lhs == rhs);
			}
		};
		iterator begin(){return iterator{f, 0};}
		iterator end(){return iterator{f, static_cast<UINT>(std::numeric_limits<int>::min())};}
	};
	enum_adapters_range enum_adapters(){return enum_adapters_range{*this};}
	static expected<dxgi, hresult_error> create_factory(){
		return detail::convert_to_rich_interface<dxgi>(com_create_resource<IDXGIFactory2>([](IDXGIFactory2** ptr){return CreateDXGIFactory1(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(ptr));}), _T(__FUNCTION__));
	}
};

}