//Copyright (C) 2014-2017, 2019, 2021 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_windows.hpp"

namespace will{

class handle{
	HANDLE h;
public:
	handle():h{INVALID_HANDLE_VALUE}{}
	explicit handle(HANDLE&& h):h{h}{}
	handle(handle&& other):h{other.release()}{}
	handle& operator=(handle&& other){h = other.release();return *this;}
	expected<void, winapi_last_error> close(){
		if(::CloseHandle(h) == 0)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return {};
	}
	expected<void, winapi_last_error> reset(::HANDLE&& other){
		return close().map([&]{h = std::move(other);});
	}
	void swap(handle& other)noexcept{
		std::swap(h, other.h);
	}
	HANDLE release(){
		auto h = this->h;
		this->h = INVALID_HANDLE_VALUE;
		return h;
	}
	HANDLE get()const{return h;}
	explicit operator bool()const{return h == INVALID_HANDLE_VALUE;}
	~handle(){
		if(h != INVALID_HANDLE_VALUE)
			close();
	}
};

inline expected<handle, winapi_last_error> create_file(LPCTSTR filename, DWORD desired_access, DWORD share_mode, LPSECURITY_ATTRIBUTES security_attributes, DWORD creation_disposition, DWORD flags_and_attributes, HANDLE template_file = nullptr){
	auto h = ::CreateFile(filename, desired_access, share_mode, security_attributes, creation_disposition, flags_and_attributes, template_file);
	if(h == INVALID_HANDLE_VALUE)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return handle{std::move(h)};
}

inline expected<handle, winapi_last_error> create_file(LPCTSTR filename, DWORD desired_access, DWORD share_mode, DWORD creation_disposition, DWORD flags_and_attributes, HANDLE template_file = nullptr){
	return create_file(filename, desired_access, share_mode, nullptr, creation_disposition, flags_and_attributes, template_file);
}

}
