//Copyright (C) 2014-2017, 2019 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_windows.hpp"
#include"_resource_property.hpp"
#include<shellapi.h>

namespace will{

struct shell_execute_info:detail::property<::SHELLEXECUTEINFO>{
	struct verbs{
		static constexpr const TCHAR* edit = _T("edit");
		static constexpr const TCHAR* explore = _T("explore");
		static constexpr const TCHAR* find = _T("find");
		static constexpr const TCHAR* open = _T("open");
		static constexpr const TCHAR* print = _T("print");
		static constexpr const TCHAR* properties = _T("properties");
	};
	shell_execute_info(){prop.cbSize = sizeof(::SHELLEXECUTEINFO);}
#define DECLPROP(name, type, member_name) shell_execute_info& name(type value){prop.member_name = value; return *this;}
	DECLPROP(option, ::ULONG, fMask)
	DECLPROP(hwnd, ::HWND, hwnd)
	DECLPROP(verb, ::LPCTSTR, lpVerb)
	DECLPROP(file, ::LPCTSTR, lpFile)
	DECLPROP(parameters, ::LPCTSTR, lpParameters)
	DECLPROP(directory, ::LPCTSTR, lpDirectory)
	DECLPROP(show, int, nShow)
	DECLPROP(result, ::HINSTANCE, hInstApp)
	DECLPROP(id_list, ::LPVOID, lpIDList)
	DECLPROP(class_name, ::LPCTSTR, lpClass)
	DECLPROP(key_class, ::HKEY, hkeyClass)
	DECLPROP(hot_key, ::DWORD, dwHotKey)
	DECLPROP(icon, ::HANDLE, hIcon)
	DECLPROP(monitor, ::HANDLE, hMonitor)
	DECLPROP(process, ::HANDLE, hProcess)
#undef  DECLPROP
};

static inline will::expected<void, winapi_last_error> shell_execute(shell_execute_info e){
	::BOOL t = ::ShellExecuteEx(&(e.get()));
	if(t == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return {};
}

}
