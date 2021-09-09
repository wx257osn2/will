//Copyright (C) 2019-2020 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_windows.hpp"
#include"heap.hpp"

namespace will{

namespace detail{

template<typename Self>
class basic_clipboard{
	bool flag;
protected:
	explicit basic_clipboard():flag{true}{}
public:
	basic_clipboard(basic_clipboard&& other):flag{other.flag}{other.flag = false;}
	basic_clipboard& operator=(basic_clipboard&& other){flag = other.flag; other.flag = false; return *this;}
	~basic_clipboard(){close();}
	static will::expected<Self, winapi_last_error> open(HWND hwnd){
		if(::OpenClipboard(hwnd) == 0)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return Self();
	}
	struct nop_deleter{void operator()(HGLOBAL)noexcept{}};
	will::expected<moveable_memory<nop_deleter>, winapi_last_error> get_data(::UINT format)const{
		::HANDLE h = ::GetClipboardData(format);
		if(h == nullptr)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return moveable_memory<nop_deleter>{h};
	}
	will::expected<void, winapi_last_error> make_empty(){
		::BOOL t = ::EmptyClipboard();
		if(t == 0)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return {};
	}
	will::expected<void, winapi_last_error> close(){
		if(flag == false)
			return {};
		if(::CloseClipboard() == 0)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		flag = false;
		return {};
	}
};

}

class readonly_clipboard : public detail::basic_clipboard<readonly_clipboard>{
	readonly_clipboard() = default;
	friend detail::basic_clipboard<readonly_clipboard>;
public:
	static will::expected<readonly_clipboard, winapi_last_error> open(){
		return detail::basic_clipboard<readonly_clipboard>::open(nullptr);
	}
};

class clipboard : public detail::basic_clipboard<clipboard>{
	clipboard() = default;
public:
	using detail::basic_clipboard<clipboard>::open;
	template<typename Deleter>
	will::expected<::HANDLE, winapi_last_error> set(::UINT format, moveable_memory<Deleter>&& data){
		::HANDLE h = ::SetClipboardData(format, data.get());
		if(h == nullptr)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		data.release();
		return h;
	}
};

}