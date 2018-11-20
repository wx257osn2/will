//Copyright (C) 2018 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_windows.hpp"

namespace will{

inline expected<::OSVERSIONINFOEXW, winapi_last_error> rtl_get_version(){
	auto hmod = load_library(_T("ntdll.dll"));
	if(!hmod)
		return make_unexpected(hmod.error());
	using RtlGetVersionType = void (WINAPI)(OSVERSIONINFOEXW*);
	auto proc_address = hmod->get_proc_address<RtlGetVersionType>("RtlGetVersion");
	if(!proc_address)
		return make_unexpected(proc_address.error());
	RtlGetVersionType*const RtlGetVersion = *proc_address;
	OSVERSIONINFOEXW osversion = {sizeof(OSVERSIONINFOEXW)};
	RtlGetVersion(&osversion);
	return osversion;
}

}
