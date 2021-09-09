//Copyright (C) 2014-2020 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"com.hpp"
#include"_resource_property.hpp"
#include<dcomp.h>
#pragma comment(lib, "dcomp")

namespace will{

struct dcomposition{
	struct target : detail::resource<IDCompositionTarget>{
		using resource::resource;
		expected<target&, hresult_error> set_root(IDCompositionVisual2* visual)&{
			const auto hr = (*this)->SetRoot(visual);
			if(FAILED(hr))
				return will::make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return *this;
		}
		template<typename DCompositionVisual, std::enable_if_t<!std::is_base_of<IDCompositionVisual, std::remove_pointer_t<std::decay_t<DCompositionVisual>>>::value, std::nullptr_t> = nullptr>
		expected<target&, hresult_error> set_root(DCompositionVisual&& visual)&{
			return set_root(std::forward<DCompositionVisual>(visual).get());
		}
		expected<target, hresult_error> set_root(IDCompositionVisual2* visual)&&{
			const auto hr = (*this)->SetRoot(visual);
			if(FAILED(hr))
				return will::make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return std::move(*this);
		}
		template<typename DCompositionVisual, std::enable_if_t<!std::is_base_of<IDCompositionVisual, std::remove_pointer_t<std::decay_t<DCompositionVisual>>>::value, std::nullptr_t> = nullptr>
		expected<target, hresult_error> set_root(DCompositionVisual&& visual)&&{
			return std::move(*this).set_root(std::forward<DCompositionVisual>(visual).get());
		}
	};
	struct visual : detail::resource<IDCompositionVisual2>{
		using resource::resource;
		expected<visual&, hresult_error> set_content(IUnknown* surf)&{
			const auto hr = (*this)->SetContent(surf);
			if(FAILED(hr))
				return will::make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return *this;
		}
		template<typename Surf, std::enable_if_t<!std::is_base_of<IUnknown, std::remove_pointer_t<std::decay_t<Surf>>>::value, std::nullptr_t> = nullptr>
		expected<visual&, hresult_error> set_content(Surf&& surf)&{
			return set_content(std::forward<Surf>(surf).get());
		}
		expected<visual, hresult_error> set_content(IUnknown* surf)&&{
			const auto hr = (*this)->SetContent(surf);
			if(FAILED(hr))
				return will::make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return std::move(*this);
		}
		template<typename Surf, std::enable_if_t<!std::is_base_of<IUnknown, std::remove_pointer_t<std::decay_t<Surf>>>::value, std::nullptr_t> = nullptr>
		expected<visual, hresult_error> set_content(Surf&& surf)&&{
			return std::move(*this).set_content(std::forward<Surf>(surf).get());
		}
	};
	class desktop_device : public detail::resource<IDCompositionDesktopDevice>{
		using resource::resource;
	public:
		static expected<desktop_device, hresult_error> create(ID2D1Device* d2ddev)noexcept{
			return detail::convert_to_rich_interface<desktop_device>(com_create_resource<IDCompositionDesktopDevice>([&](IDCompositionDesktopDevice** ptr){return ::DCompositionCreateDevice3(d2ddev, __uuidof(IDCompositionDesktopDevice), reinterpret_cast<void**>(ptr));}), _T(__FUNCTION__));
		}
		static expected<desktop_device, hresult_error> create(ID3D11Device1* d3ddev)noexcept{
			return detail::convert_to_rich_interface<desktop_device>(com_create_resource<IDCompositionDesktopDevice>([&](IDCompositionDesktopDevice** ptr){return ::DCompositionCreateDevice3(d3ddev, __uuidof(IDCompositionDesktopDevice), reinterpret_cast<void**>(ptr));}), _T(__FUNCTION__));
		}
		static expected<desktop_device, hresult_error> create(IDXGIDevice2* dxgidev)noexcept{
			return detail::convert_to_rich_interface<desktop_device>(com_create_resource<IDCompositionDesktopDevice>([&](IDCompositionDesktopDevice** ptr){return ::DCompositionCreateDevice3(dxgidev, __uuidof(IDCompositionDesktopDevice), reinterpret_cast<void**>(ptr));}), _T(__FUNCTION__));
		}
		template<typename Device>
		static expected<desktop_device, hresult_error> create(Device&& dev)noexcept{return create(std::forward<Device>(dev).get());}
		expected<target, hresult_error> create_target(HWND hwnd, bool top_most){
			return detail::convert_to_rich_interface<target>(com_create_resource<IDCompositionTarget>([&](IDCompositionTarget** ptr){return (*this)->CreateTargetForHwnd(hwnd, top_most, ptr);}), _T(__FUNCTION__));
		}
		template<typename D2DDevice, std::enable_if_t<!std::is_same<desktop_device, std::decay_t<D2DDevice>>::value, std::nullptr_t> = nullptr>
		explicit desktop_device(D2DDevice&& d2ddev):desktop_device(std::move(create(std::forward<D2DDevice>(d2ddev)).value())){}
		desktop_device(desktop_device&&) = default;
		template<typename Window>
		expected<target, hresult_error> create_target(Window&& hwnd, bool top_most){
			return create_target(std::forward<Window>(hwnd).get(), top_most);
		}
		expected<visual, hresult_error> create_visual(){
			return detail::convert_to_rich_interface<visual>(com_create_resource<IDCompositionVisual2>([this](IDCompositionVisual2** ptr){return (*this)->CreateVisual(ptr);}), _T(__FUNCTION__));
		}
		expected<void, hresult_error> commit(){
			const auto hr = (*this)->Commit();
			if(FAILED(hr))
				return will::make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return {};
		}
	};
#if 0
	class device : public detail::resource<IDCompositionDevice3>{
		friend dcomposition;
		using resource::resource;
	public:
		static expected<device, hresult_error> create(IDCompositionDesktopDevice* ddev)noexcept{
			return detail::convert_to_rich_interface<device>(query_interface<IDCompositionDevice3>(ddev), _T(__FUNCTION__));
		}
		template<typename DCompositionDesktopDevice>
		static expected<device, hresult_error> create(DCompositionDesktopDevice&& ddev)noexcept{return create(std::forward<DCompositionDesktopDevice>(ddev).get());}
	};
#endif
};

}
