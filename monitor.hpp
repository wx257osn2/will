﻿#pragma once
#include"_windows.hpp"

namespace will{

class monitor{
	HMONITOR h;
	template<typename T>
	expected<T, winapi_last_error> get_info_impl()const{
		T mon{};
		mon.cbSize = sizeof(T);
		if(::GetMonitorInfo(h, &mon) != 0)
			return mon;
		return make_unexpected<winapi_last_error>(_T("will::monitor::get_info"));
	}
public:
	enum class default_to:DWORD{
		null = MONITOR_DEFAULTTONULL,
		nearest = MONITOR_DEFAULTTONEAREST,
		primary = MONITOR_DEFAULTTOPRIMARY
	};
	explicit monitor(HMONITOR h):h(h){}
	monitor(const monitor&) = default;
	monitor(monitor&&) = default;
	monitor& operator=(const monitor&) = default;
	monitor& operator=(monitor&&) = default;
	~monitor() = default;
	explicit operator bool()const noexcept{return h != nullptr;}
	explicit operator HMONITOR()const noexcept{return h;}
	HMONITOR get()const{return h;}
	friend bool operator==(const monitor& lhs, const monitor& rhs)noexcept{return lhs.h == rhs.h;}
	friend bool operator==(const monitor& lhs, HMONITOR rhs)noexcept{return lhs.h == rhs;}
	friend bool operator==(HMONITOR lhs, const monitor& rhs)noexcept{return lhs == rhs.h;}
	friend bool operator!=(const monitor& lhs, const monitor& rhs)noexcept{return !(lhs == rhs);}
	friend bool operator!=(const monitor& lhs, HMONITOR rhs)noexcept{return !(lhs == rhs);}
	friend bool operator!=(HMONITOR lhs, const monitor& rhs)noexcept{return !(lhs == rhs);}
	static monitor from_point(const POINT& p, default_to d = default_to::null){return monitor{::MonitorFromPoint(p, static_cast<std::underlying_type_t<default_to>>(d))};}
	static monitor from_rect(const RECT& r, default_to d = default_to::null){return monitor{::MonitorFromRect(&r, static_cast<std::underlying_type_t<default_to>>(d))};}
	static monitor from_window(HWND hwnd, default_to d = default_to::null){return monitor{::MonitorFromWindow(hwnd, static_cast<std::underlying_type_t<default_to>>(d))};}
	template<typename Window, std::enable_if_t<!std::is_same<std::decay_t<Window>, HWND>::value>* = nullptr>
	static monitor from_window(Window&& w, default_to d = default_to::null){return from_window(std::forward<Window>(w).get(), d);}
	template<typename BOOL（HMONITOR，const_RECT＆）>
	static expected<void, winapi_last_error> enum_display(BOOL（HMONITOR，const_RECT＆）&& f){
		using F = BOOL（HMONITOR，const_RECT＆）;
		if(::EnumDisplayMonitors(nullptr, nullptr, [](HMONITOR hm, HDC, LPRECT lpr, LPARAM f)->BOOL{
			return (std::is_pointer<F>::value ? reinterpret_cast<F>(f) : *reinterpret_cast<F*>(f))(monitor{hm}, *lpr);
		}, std::is_pointer<F>::value ? reinterpret_cast<LPARAM>(f) : reinterpret_cast<LPARAM>(&f)) != 0)return {};
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	}
	template<typename BOOL（HMONITOR，HDC，const_RECT＆）>
	static bool enum_display(BOOL（HMONITOR，HDC，const_RECT＆）&& f, HDC hdc){
		using F = BOOL（HMONITOR，HDC，const_RECT＆）;
		if(::EnumDisplayMonitors(hdc, nullptr, [](HMONITOR hm, HDC hdc, LPRECT lpr, LPARAM f)->BOOL{
			return (std::is_pointer<F>::value ? reinterpret_cast<F>(f) : *reinterpret_cast<F*>(f))(monitor{hm}, hdc, *lpr);
		}, std::is_pointer<F>::value ? reinterpret_cast<LPARAM>(f) : reinterpret_cast<LPARAM>(&f)) != 0)return {};
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	}
	template<typename BOOL（HMONITOR，const_RECT＆）>
	static bool enum_display(BOOL（HMONITOR，const_RECT＆）&& f, const RECT& r){
		using F = BOOL（HMONITOR，const_RECT＆）;
		if(::EnumDisplayMonitors(nullptr, &r, [](HMONITOR hm, HDC, LPRECT lpr, LPARAM f)->BOOL{
			return (std::is_pointer<F>::value ? reinterpret_cast<F>(f) : *reinterpret_cast<F*>(f))(monitor{hm}, *lpr);
		}, std::is_pointer<F>::value ? reinterpret_cast<LPARAM>(f) : reinterpret_cast<LPARAM>(&f)) != 0)return {};
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	}
	template<typename BOOL（HMONITOR，HDC，const_RECT＆）>
	static bool enum_display(BOOL（HMONITOR，HDC，const_RECT＆）&& f, HDC hdc, const RECT& r){
		using F = BOOL（HMONITOR，HDC，const_RECT＆）;
		if(::EnumDisplayMonitors(hdc, &r, [](HMONITOR hm, HDC hdc, LPRECT lpr, LPARAM f)->BOOL{
			return (std::is_pointer<F>::value ? reinterpret_cast<F>(f) : *reinterpret_cast<F*>(f))(monitor{hm}, hdc, *lpr);
		}, std::is_pointer<F>::value ? reinterpret_cast<LPARAM>(f) : reinterpret_cast<LPARAM>(&f)) != 0)return {};
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	}
	expected<::MONITORINFO, winapi_last_error> get_info()const{
		return get_info_impl<::MONITORINFO>();
	}
	expected<::MONITORINFOEX, winapi_last_error> get_info_ex()const{
		return get_info_impl<::MONITORINFOEX>();
	}
};

}
