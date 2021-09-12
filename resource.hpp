//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_windows.hpp"
#include<vector>
namespace will{

namespace resource{

template<std::size_t StringLengthHint>
inline expected<tstring, winapi_last_error> load_string(HINSTANCE hinst, UINT resource_id){
	std::vector<TCHAR> buf(StringLengthHint);
	if(::LoadString(hinst, resource_id, buf.data(), static_cast<int>(buf.size())) == 0)
		return make_unexpected(winapi_last_error{_T(__FUNCTION__)});
	return tstring(buf.begin(), buf.end());
}
inline expected<tstring, winapi_last_error> load_string(HINSTANCE hinst, UINT resource_id){return load_string<256>(hinst, resource_id);}
template<std::size_t StringLengthHint>
inline expected<tstring, winapi_last_error> load_string(UINT resource_id){return load_string<StringLengthHint>(nullptr, resource_id);}
inline expected<tstring, winapi_last_error> load_string(UINT resource_id){return load_string<256>(nullptr, resource_id);}
inline expected<HRSRC, winapi_last_error> find_string(HMODULE mod, LPCTSTR name){return detail::checked_return(_T(__FUNCTION__), ::FindResource(mod, name, RT_STRING), nullptr);}
inline expected<HRSRC, winapi_last_error> find_string(LPCTSTR name){return find_string(nullptr, name);}

inline expected<HRSRC, winapi_last_error> find_cursor(HMODULE mod, LPCTSTR name){return detail::checked_return(_T(__FUNCTION__), ::FindResource(mod, name, RT_CURSOR), nullptr);}
inline expected<HRSRC, winapi_last_error> find_cursor(LPCTSTR name){return find_cursor(nullptr, name);}
inline expected<HCURSOR, winapi_last_error> load_cursor(HINSTANCE hinst, LPCTSTR name, int x = 0, int y = 0, UINT option = 0){return detail::checked_return(_T(__FUNCTION__), reinterpret_cast<HCURSOR>(::LoadImage(hinst, name, IMAGE_CURSOR, x, y, option)), nullptr);}
inline expected<HCURSOR, winapi_last_error> load_cursor(LPCTSTR name, int x = 0, int y = 0, UINT option = LR_SHARED | LR_DEFAULTSIZE){return load_cursor(nullptr, name, x, y, option);}
inline expected<HCURSOR, winapi_last_error> load_cursor(HMODULE mod, HRSRC res){return detail::checked_return(_T(__FUNCTION__), reinterpret_cast<HCURSOR>(::LoadResource(mod, res)), nullptr);}

inline expected<HRSRC, winapi_last_error> find_icon(HMODULE mod, LPCTSTR name){return detail::checked_return(_T(__FUNCTION__), ::FindResource(mod, name, RT_ICON), nullptr);}
inline expected<HRSRC, winapi_last_error> find_icon(LPCTSTR name){return find_icon(nullptr, name);}
inline expected<HICON, winapi_last_error> load_icon(HINSTANCE hinst, LPCTSTR name, int x = 0, int y = 0, UINT option = 0){return detail::checked_return(_T(__FUNCTION__), reinterpret_cast<HICON>(::LoadImage(hinst, name, IMAGE_ICON, x, y, option)), nullptr);}
inline expected<HICON, winapi_last_error> load_icon(LPCTSTR name, int x = 0, int y = 0, UINT option = LR_SHARED | LR_DEFAULTSIZE){return load_icon(nullptr, name, x, y, option);}
inline expected<HICON, winapi_last_error> load_icon(HMODULE mod, HRSRC res){return detail::checked_return(_T(__FUNCTION__), reinterpret_cast<HICON>(::LoadResource(mod, res)), nullptr);}

inline expected<HRSRC, winapi_last_error> find_font(HMODULE mod, LPCTSTR name){return detail::checked_return(_T(__FUNCTION__), ::FindResource(mod, name, RT_FONT), nullptr);}
inline expected<HRSRC, winapi_last_error> find_font(LPCTSTR name){return find_font(nullptr, name);}
inline expected<HFONT, winapi_last_error> load_font(HINSTANCE hinst, LPCTSTR name){if(auto t = find_font(hinst, name))return reinterpret_cast<HFONT>(::LoadResource(hinst, *t));return make_unexpected(winapi_last_error{_T(__FUNCTION__)});}
inline expected<HFONT, winapi_last_error> load_font(LPCTSTR name){return load_font(nullptr, name);}
inline expected<HFONT, winapi_last_error> load_font(HMODULE mod, HRSRC res){return detail::checked_return(_T(__FUNCTION__), reinterpret_cast<HFONT>(::LoadResource(mod, res)), nullptr);}

template<typename H>
inline void* lock(H h){return ::LockResource(reinterpret_cast<HANDLE>(h));}
inline expected<std::size_t, winapi_last_error> size_of(HMODULE mod, HRSRC res){return detail::checked_return(_T(__FUNCTION__), static_cast<std::size_t>(::SizeofResource(mod, res)), 0);}

}

}