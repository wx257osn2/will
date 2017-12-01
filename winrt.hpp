//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include<sdkddkver.h>
#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
#include<vector>
#include<string_view>
#include<roapi.h>
#include<winstring.h>
#pragma comment(lib, "runtimeobject.lib")
#include<inspectable.h>
#include"com.hpp"
#include"_resource_property.hpp"

namespace will{
#if 0
class hstring_reference{
	HSTRING_HEADER header;
	HSTRING hstr;
	hstring_reference(HSTRING_HEADER&& h, HSTRING&& s)noexcept:header{std::move(h)}, hstr{std::move(s)}{h = {}; s = nullptr;}
public:
	static expected<hstring_reference, hresult_error> create(const wchar_t* str, std::size_t length){
		HSTRING_HEADER h = {};
		HSTRING hstr = nullptr;
		const auto hr = ::WindowsCreateStringReference(str, static_cast<UINT32>(length), &h, &hstr);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return hstring_reference{std::move(h), std::move(hstr)};
	}
	static expected<hstring_reference, hresult_error> create(const wchar_t* str){return create(str, std::wcslen(str));}
	template<std::size_t N>
	static expected<hstring_reference, hresult_error> create(const wchar_t (&str)[N]){return create(str, N);}
	static expected<hstring_reference, hresult_error> create(std::nullptr_t){return create(nullptr, 0ul);}
	hstring_reference(hstring_reference&& other)noexcept:hstring_reference{std::move(other.header), std::move(other.hstr)}{}
	hstring_reference& operator=(hstring_reference&& other)noexcept{header = std::move(other.header); hstr = std::move(other.hstr); other.header = {}; other.hstr = nullptr; return *this;}
	hstring_reference(const wchar_t* str, std::size_t length):hstring_reference{+create(str, length)}{}
	hstring_reference(const wchar_t* str):hstring_reference{+create(str, std::wcslen(str))}{}
	template<std::size_t N>
	hstring_reference(const wchar_t (&str)[N]):hstring_reference{+create(str, N)}{}
	hstring_reference(std::nullptr_t):hstring_reference{+create(nullptr)}{}
	explicit operator HSTRING()const noexcept{return hstr;}
	HSTRING get()const noexcept{return hstr;}
};
#endif
class hstring{
	HSTRING hstr;
public:
	hstring(HSTRING&& s)noexcept:hstr(std::move(s)){s = nullptr;}
	static expected<hstring, hresult_error> create(const wchar_t* str, std::size_t length){
		HSTRING hs;
		const auto hr = ::WindowsCreateString(str, static_cast<UINT32>(length), &hs);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return hstring(std::move(hs));
	}
	static expected<hstring, hresult_error> create(const wchar_t* str){return create(str, std::wcslen(str));}
	template<std::size_t N>
	static expected<hstring, hresult_error> create(const wchar_t (&str)[N]){return create(str, N);}
	static expected<hstring, hresult_error> create(std::nullptr_t){return create(nullptr, 0ul);}
	hstring(const wchar_t* str, std::size_t length):hstring{+create(str, length)}{}
	hstring(const wchar_t* str):hstring{+create(str, std::wcslen(str))}{}
	template<std::size_t N>
	hstring(const wchar_t (&str)[N]):hstring{+create(str, N)}{}
	hstring(std::nullptr_t):hstring{+create(nullptr)}{}
	void reset(HSTRING&& new_hstr = nullptr)noexcept{::WindowsDeleteString(hstr);hstr = std::move(new_hstr);}
	void detach()noexcept{hstr = nullptr;}
	HSTRING release()noexcept{auto t = hstr;detach();return t;}
	hstring(hstring&& other)noexcept:hstr{std::move(other.release())}{}
	static expected<hstring, hresult_error> copy(HSTRING str){
		HSTRING hstr = nullptr;
		auto hr = ::WindowsDuplicateString(str, &hstr);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return hstring{std::move(hstr)};
	}
	static expected<hstring, hresult_error> copy(const hstring& hstr){return copy(hstr.get());}
#if 0
	static expected<hstring, hresult_error> copy(const hstring_reference& hstr){return copy(hstr.get());}
#endif
	//explicit hstring(HSTRING hstr):hstring{+copy(hstr)}{}
	hstring(const hstring& hstr):hstring{+copy(hstr)}{}
#if 0
	explicit hstring(const hstring_reference& hstr):hstring{+copy(hstr)}{}
#endif
	hstring& operator=(const hstring& rhs){reset(copy(rhs).value().release());return *this;}
	hstring& operator=(hstring&& rhs){reset(rhs.release());return *this;}
	static expected<hstring, hresult_error> concat(HSTRING lhs, HSTRING rhs){
		HSTRING hstr = nullptr;
		auto hr = ::WindowsConcatString(lhs, rhs, &hstr);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return hstring{std::move(hstr)};
	}
	static expected<hstring, hresult_error> concat(HSTRING lhs, const hstring& rhs){return concat(lhs, rhs.get());}
#if 0
	static expected<hstring, hresult_error> concat(HSTRING lhs, const hstring_reference& rhs){return concat(lhs, rhs.get());}
#endif
	static expected<hstring, hresult_error> concat(const hstring& lhs, HSTRING rhs){return concat(lhs.get(), rhs);}
	static expected<hstring, hresult_error> concat(const hstring& lhs, const hstring& rhs){return concat(lhs.get(), rhs.get());}
#if 0
	static expected<hstring, hresult_error> concat(const hstring& lhs, const hstring_reference& rhs){return concat(lhs.get(), rhs.get());}
	static expected<hstring, hresult_error> concat(const hstring_reference& lhs, HSTRING rhs){return concat(lhs.get(), rhs);}
	static expected<hstring, hresult_error> concat(const hstring_reference& lhs, const hstring& rhs){return concat(lhs.get(), rhs.get());}
	static expected<hstring, hresult_error> concat(const hstring_reference& lhs, const hstring_reference& rhs){return concat(lhs.get(), rhs.get());}
#endif
	template<typename T>
	friend hstring operator+(const hstring& lhs, T&& rhs){return +concat(lhs, std::forward<T>(rhs));}
	template<typename T>
	hstring& operator+=(T&& rhs)const{return *this = +concat(lhs, std::forward<T>(rhs));}
	template<typename T>
	friend hstring operator+(T&& lhs, const hstring& rhs){return +concat(std::forward<T>(lhs), rhs);}
	explicit operator HSTRING()const{return hstr;}
	HSTRING get()const{return hstr;}
	std::wstring_view get_raw_buffer_view()const{
		UINT32 length;
		auto ptr = ::WindowsGetStringRawBuffer(hstr, &length);
		if(ptr == nullptr)
			return std::wstring_view{};
		return std::wstring_view{ptr, length};
	}
};

namespace detail{
template<typename T>
struct inspectable : detail::resource<T>{
	using resource::resource;
	expected::expected<std::vector<IID>, hresult_error> get_iids()const{
		ULONG c = 0;
		IID* ptr = nullptr;
		const auto hr = (*this)->GetIids(&c, &ptr);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		std::vector<IID> vec;
		vec.reserve(c);
		for(ULONG i = 0; i < c; ++i)
			vec.emplace_back(ptr[i]);
		return vec;
	}
	expected::expected<hstring, hresult_error> get_runtime_class_name()const{
		HSTRING hstr = nullptr;
		const auto hr = (*this)->GetRuntimeClassName(&hstr);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return hstring{std::move(hstr)};
	}
	expected::expected<TrustLevel, hresult_error> get_trust_level()const{
		TrustLevel t;
		const auto hr = (*this)->GetTrustLevel(&t);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return t;
	}
};
}

}
#endif
