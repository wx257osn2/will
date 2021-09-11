//Copyright (C) 2021 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_windows.hpp"
#include<io.h>
#include<cstdio>

namespace will{

namespace console{

static inline expected<void, winapi_last_error> alloc(){
	if(::AllocConsole() == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return {};
}

static inline expected<void, winapi_last_error> free(){
	if(::FreeConsole() == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return {};
}

static inline expected<void, winapi_last_error> attach(DWORD process_id){
	if(::AttachConsole(process_id) == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return {};
}

static inline expected<void, winapi_last_error> set_cp(UINT code_page_id){
	if(::SetConsoleCP(code_page_id) == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return {};
}

static inline expected<UINT, winapi_last_error> get_cp(){
	const auto ret = ::GetConsoleCP();
	if(ret == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return ret;
}

static inline expected<void, winapi_last_error> set_output_cp(UINT code_page_id){
	if(::SetConsoleOutputCP(code_page_id) == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return {};
}

static inline expected<UINT, winapi_last_error> get_output_cp(){
	const auto ret = ::GetConsoleOutputCP();
	if(ret == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return ret;
}

static inline expected<void, winapi_last_error> set_title(LPCTSTR title){
	if(::SetConsoleTitle(title) == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return {};
}

static inline expected<tstring, winapi_last_error> get_original_title(){
	tchar::tstring s(256, _T('\0'));
	auto ret = ::GetConsoleOriginalTitle(s.data(), static_cast<DWORD>(s.size()));
	while(ret == 0){
		const auto le = ::GetLastError();
		if(le == ERROR_SUCCESS){
			s.resize(s.size()*2);
			ret = ::GetConsoleOriginalTitle(s.data(), static_cast<DWORD>(s.size()));
		}
		else
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__), le);
	}
	s.resize(ret);
	return s;
}

static inline expected<tstring, winapi_last_error> get_title(){
	TCHAR dummy_buf[1];
	auto ret = ::GetConsoleTitle(dummy_buf, 1);
	if(ret == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	tstring real_buf(ret+1, _T('\0'));
	if(::GetConsoleTitle(real_buf.data(), static_cast<DWORD>(real_buf.size())) == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	real_buf.resize(ret);
	return real_buf;
}

static inline expected<DWORD, winapi_last_error> get_mode(HANDLE handle){
	DWORD mode;
	if(::GetConsoleMode(handle, &mode) == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return mode;
}

static inline expected<void, winapi_last_error> set_mode(HANDLE handle, DWORD mode){
	if(::SetConsoleMode(handle, mode) == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return {};
}

static inline expected<void, std::error_code> bind_stdstreams(){
	::FILE* _;
	auto ret = ::freopen_s(&_, "CONOUT$", "w", stdout);
	if(ret)
		return will::make_unexpected(std::error_code{ret, std::generic_category()});
	ret = ::freopen_s(&_, "CONOUT$", "w", stderr);
	if(ret)
		return will::make_unexpected(std::error_code{ret, std::generic_category()});
	ret = ::freopen_s(&_, "CONIN$", "r", stdin);
	if(ret)
		return will::make_unexpected(std::error_code{ret, std::generic_category()});
	return {};
}

}

static inline expected<HANDLE, winapi_last_error> get_std_handle(DWORD std_handle){
	auto ret = ::GetStdHandle(std_handle);
	if(ret == INVALID_HANDLE_VALUE)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return ret;
}

static inline expected<HANDLE, winapi_last_error> get_stdin_handle(){
	return get_std_handle(STD_INPUT_HANDLE);
}

static inline expected<HANDLE, winapi_last_error> get_stdout_handle(){
	return get_std_handle(STD_OUTPUT_HANDLE);
}

static inline expected<HANDLE, winapi_last_error> get_stderr_handle(){
	return get_std_handle(STD_ERROR_HANDLE);
}

}
