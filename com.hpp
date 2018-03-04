//Copyright (C) 2014-2018 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<objbase.h>
#include<cassert>
#include<utility>
#include<string>
#include<initializer_list>
#include"_windows.hpp"
namespace will{
class hresult_error{
	::LPCTSTR func_name;
	::HRESULT hr;
#ifdef WILL_USE_STACK_TRACE
	std::vector<void*> sf;
#define WILL_WLE_NOEXCEPT
#else
#define WILL_WLE_NOEXCEPT noexcept
#endif
public:
	hresult_error(::LPCTSTR fn, ::HRESULT ec)noexcept:func_name(fn), hr(ec){
#ifdef WILL_USE_STACK_TRACE
		auto csf = capture_stack_back_trace(1, 62);
		if(csf)
			sf = std::move(*csf);
#endif
	}
	hresult_error(const hresult_error&)WILL_WLE_NOEXCEPT = default;
	hresult_error(hresult_error&&)noexcept = default;
	hresult_error& operator=(const hresult_error&)WILL_WLE_NOEXCEPT = default;
	hresult_error& operator=(hresult_error&&)noexcept = default;
	::LPCTSTR get_func_name()const noexcept{return func_name;}
	::HRESULT get_error_code()const noexcept{return hr;}
	expected<formatted_message, winapi_last_error> get_error_message()const noexcept{
		::LPTSTR ptr;
		if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, hr, 0, reinterpret_cast<LPTSTR>(&ptr), 0, nullptr) == 0)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return {ptr};
	}
#ifdef WILL_USE_STACK_TRACE
	will::stack_frames stack_frames(symbol_handler&& symh)const noexcept{
		return will::stack_frames::to_stack_frames(sf)(std::move(symh));
	}
#endif
#undef WILL_WLE_NOEXCEPT
};

class hresult_error_exception : public will::runtime_error{
	hresult_error e;
public:
	hresult_error_exception(const hresult_error& err):runtime_error(
#ifdef UNICODE
		to_string(
#endif
		(tstring(err.get_func_name()) + _T(" : ") + err.get_error_message().value().get()).c_str()
#ifdef UNICODE
		, -1, CP_ACP).value()
#endif
	), e{err}{}
	hresult_error_exception(::LPCTSTR func_name, ::HRESULT ec)noexcept:hresult_error_exception(hresult_error{func_name, ec}){}
	::LPCTSTR get_func_name()const noexcept{return e.get_func_name();}
	::HRESULT get_error_code()const noexcept{return e.get_error_code();}
	expected<formatted_message, winapi_last_error> get_error_message()const noexcept{return e.get_error_message();}
	const hresult_error& value()const noexcept{return e;}
	hresult_error value()noexcept{return std::move(e);}
#ifdef WILL_USE_STACK_TRACE
	virtual const char* what()const noexcept{
		if(cache.empty())
			try{
				auto symh = will::symbol_handler::create();
				if(!symh)
					return message();
				auto stack_frame = value().stack_frames(std::move(*symh));
				if(stack_frame.empty())
					return message();
				std::stringstream ss;
				ss << runtime_error::what() << '\n';
				ss << "------------------------------\n"
				      "STACK BACK TRACE (INNER ERROR)\n"
				      "------------------------------\n";
				ss << stack_frame << std::endl;
				cache = std::move(ss.str());
			}catch(...){
				return message();
			}
		return cache.c_str();
	}
protected:
	virtual const char* exception_name()const noexcept{return "will::hresult_error";}
#endif
};

template<>
struct error_traits<hresult_error>{
	static constexpr bool can_make_error_from_current_exception = true;
	static hresult_error make_error(const hresult_error& e)noexcept{
		return e;
	}
	static hresult_error make_error(::HRESULT ec){
		return hresult_error{_T("(unknown)"), ec};
	}
	static hresult_error make_error_from_current_exception()try{
		throw;
	}catch(hresult_error_exception& e){
		return std::move(e.value());
	}catch(...){
		return hresult_error{_T("(no_error)"), 0};
	}
	[[noreturn]] static void rethrow(const hresult_error& e){
		throw hresult_error_exception(e);
	}
};
class com_apartment{
	bool scope;
	com_apartment(bool t = false):scope(t){}
public:
	com_apartment(com_apartment&& other):scope(other.scope){other.scope = false;}
	enum class thread{
		single = COINIT_APARTMENTTHREADED, 
		multi  = COINIT_MULTITHREADED
	};
	static expected<com_apartment, hresult_error> initialize(thread t){
		const auto hr = ::CoInitializeEx(nullptr, static_cast<COINIT>(t)|COINIT_DISABLE_OLE1DDE);
		if(SUCCEEDED(hr))
			return com_apartment{true};
		return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
	}
	com_apartment(thread t):com_apartment{+initialize(t)}{}
	~com_apartment(){if(scope)::CoUninitialize();}
};
template<typename T, typename F>
inline expected<T*, HRESULT> com_create_resource(F&& f)noexcept(noexcept(f(std::declval<T**>()))){
	T* t;
	const auto hr = f(&t);
	if(SUCCEEDED(hr))
		return t;
	return make_unexpected(hr);
}
template<typename T, typename U>
inline expected<T*, HRESULT> query_interface(U* u){
	if(u == nullptr)
		return make_unexpected(E_POINTER);
	return com_create_resource<T>([&](T** ptr){return u->QueryInterface(__uuidof(T), reinterpret_cast<void**>(ptr));});
}
class bstr{
	BSTR bs;
public:
	struct sys_alloc_string_failed:will::runtime_error{
		using will::runtime_error::runtime_error;
	};
	static expected<bstr> make(LPCWSTR str)noexcept{
		auto ret = ::SysAllocString(str);
		if(ret == nullptr)
			return make_unexpected(sys_alloc_string_failed{"SysAllocString failed"});
		return bstr{std::move(ret)};
	}
	static expected<bstr> make(const std::wstring& str)noexcept{return make(str.c_str());}
	bstr()noexcept = default;
	bstr(const bstr& other):bs(+make(other.bs)){}
	bstr(BSTR&& _)noexcept:bs(_){}
	bstr(bstr&& other)noexcept:bs(other.release()){}
	~bstr()noexcept{reset();}
	operator BSTR()const noexcept{return bs;}
	BSTR get()const noexcept{return bs;}
	void detach()noexcept{
		bs = nullptr;
	}
	BSTR release()noexcept{
		auto ret = bs;
		detach();
		return ret;
	}
	void swap(bstr& other)noexcept{
		std::swap(bs, other.bs);
	}
	void reset(BSTR b = nullptr)noexcept{
		if(bs)
			::SysFreeString(bs);
		bs = b;
	}
	bstr& operator=(const bstr& rhs){
		+make(rhs.bs).map([&](bstr&& b){reset(std::move(b));});
		return *this;
	}
	bstr& operator=(bstr&& rhs)noexcept{
		reset(rhs.release());
		return *this;
	}
	bstr& operator=(BSTR rhs)noexcept{
		reset(rhs);
		return *this;
	}
	bstr& operator=(LPCWSTR rhs){
		return *this =+ make(rhs);
	}
	bstr& operator=(const std::wstring& rhs){
		return *this =+ make(rhs);
	}
	explicit operator bool()const noexcept{return bs != nullptr;}
};
struct variant_bool{VARIANT_BOOL b;operator VARIANT_BOOL&()noexcept{return b;}operator const VARIANT_BOOL&()const noexcept{return b;}VARIANT_BOOL& get()noexcept{return b;}const VARIANT_BOOL& get()const noexcept{return b;}};
struct scode{SCODE s;operator SCODE&()noexcept{return s;}operator const SCODE&()const noexcept{return s;}SCODE& get()noexcept{return s;}const SCODE& get()const noexcept{return s;}};
struct date{DATE d;operator DATE&()noexcept{return d;}operator const DATE&()const noexcept{return d;}DATE& get()noexcept{return d;}const DATE& get()const noexcept{return d;}};
namespace detail{
template<typename>struct type_to_val_impl;
template<>struct type_to_val_impl<long long>{static constexpr long long& type_to_val(VARIANT& var)noexcept{return var.llVal;}static constexpr const long long& type_to_val(const VARIANT& var)noexcept{return var.llVal;}};
template<>struct type_to_val_impl<long>{static constexpr long& type_to_val(VARIANT& var)noexcept{return var.lVal;}static constexpr const long& type_to_val(const VARIANT& var)noexcept{return var.lVal;}};
template<>struct type_to_val_impl<unsigned char>{static constexpr unsigned char& type_to_val(VARIANT& var)noexcept{return var.bVal;}static constexpr const unsigned char& type_to_val(const VARIANT& var)noexcept{return var.bVal;}};
template<>struct type_to_val_impl<short>{static constexpr short& type_to_val(VARIANT& var)noexcept{return var.iVal;}static constexpr const short& type_to_val(const VARIANT& var)noexcept{return var.iVal;}};
template<>struct type_to_val_impl<float>{static constexpr float& type_to_val(VARIANT& var)noexcept{return var.fltVal;}static constexpr const float& type_to_val(const VARIANT& var)noexcept{return var.fltVal;}};
template<>struct type_to_val_impl<double>{static constexpr double& type_to_val(VARIANT& var)noexcept{return var.dblVal;}static constexpr const double& type_to_val(const VARIANT& var)noexcept{return var.dblVal;}};
template<>struct type_to_val_impl<variant_bool>{static constexpr VARIANT_BOOL& type_to_val(VARIANT& var)noexcept{return var.boolVal;}static constexpr const VARIANT_BOOL& type_to_val(const VARIANT& var)noexcept{return var.boolVal;}};
template<>struct type_to_val_impl<scode>{static constexpr SCODE& type_to_val(VARIANT& var)noexcept{return var.scode;}static constexpr const SCODE& type_to_val(const VARIANT& var)noexcept{return var.scode;}};
template<>struct type_to_val_impl<CY>{static constexpr CY& type_to_val(VARIANT& var)noexcept{return var.cyVal;}static constexpr const CY& type_to_val(const VARIANT& var)noexcept{return var.cyVal;}};
template<>struct type_to_val_impl<date>{static constexpr DATE& type_to_val(VARIANT& var)noexcept{return var.date;}static constexpr const DATE& type_to_val(const VARIANT& var)noexcept{return var.date;}};
template<>struct type_to_val_impl<BSTR>{static constexpr BSTR& type_to_val(VARIANT& var)noexcept{return var.bstrVal;}static constexpr const BSTR& type_to_val(const VARIANT& var)noexcept{return var.bstrVal;}};
template<>struct type_to_val_impl<IUnknown*>{static constexpr IUnknown*& type_to_val(VARIANT& var)noexcept{return var.punkVal;}static constexpr IUnknown*const& type_to_val(const VARIANT& var)noexcept{return var.punkVal;}};
template<>struct type_to_val_impl<IDispatch*>{static constexpr IDispatch*& type_to_val(VARIANT& var)noexcept{return var.pdispVal;}static constexpr IDispatch*const& type_to_val(const VARIANT& var)noexcept{return var.pdispVal;}};
template<>struct type_to_val_impl<SAFEARRAY*>{static constexpr SAFEARRAY*& type_to_val(VARIANT& var)noexcept{return var.parray;}static constexpr SAFEARRAY*const& type_to_val(const VARIANT& var)noexcept{return var.parray;}};
template<>struct type_to_val_impl<unsigned char*>{static constexpr unsigned char*& type_to_val(VARIANT& var)noexcept{return var.pbVal;}static constexpr unsigned char*const& type_to_val(const VARIANT& var)noexcept{return var.pbVal;}};
template<>struct type_to_val_impl<short*>{static constexpr short*& type_to_val(VARIANT& var)noexcept{return var.piVal;}static constexpr short*const& type_to_val(const VARIANT& var)noexcept{return var.piVal;}};
template<>struct type_to_val_impl<long*>{static constexpr long*& type_to_val(VARIANT& var)noexcept{return var.plVal;}static constexpr long*const& type_to_val(const VARIANT& var)noexcept{return var.plVal;}};
template<>struct type_to_val_impl<long long*>{static constexpr long long*& type_to_val(VARIANT& var)noexcept{return var.pllVal;}static constexpr long long*const& type_to_val(const VARIANT& var)noexcept{return var.pllVal;}};
template<>struct type_to_val_impl<float*>{static constexpr float*& type_to_val(VARIANT& var)noexcept{return var.pfltVal;}static constexpr float*const& type_to_val(const VARIANT& var)noexcept{return var.pfltVal;}};
template<>struct type_to_val_impl<double*>{static constexpr double*& type_to_val(VARIANT& var)noexcept{return var.pdblVal;}static constexpr double*const& type_to_val(const VARIANT& var)noexcept{return var.pdblVal;}};
template<>struct type_to_val_impl<variant_bool*>{static constexpr VARIANT_BOOL*& type_to_val(VARIANT& var)noexcept{return var.pboolVal;}static constexpr VARIANT_BOOL*const& type_to_val(const VARIANT& var)noexcept{return var.pboolVal;}};
template<>struct type_to_val_impl<scode*>{static constexpr SCODE*& type_to_val(VARIANT& var)noexcept{return var.pscode;}static constexpr SCODE*const& type_to_val(const VARIANT& var)noexcept{return var.pscode;}};
template<>struct type_to_val_impl<CY*>{static constexpr CY*& type_to_val(VARIANT& var)noexcept{return var.pcyVal;}static constexpr CY*const& type_to_val(const VARIANT& var)noexcept{return var.pcyVal;}};
template<>struct type_to_val_impl<date*>{static constexpr DATE*& type_to_val(VARIANT& var)noexcept{return var.pdate;}static constexpr DATE*const& type_to_val(const VARIANT& var)noexcept{return var.pdate;}};
template<>struct type_to_val_impl<BSTR*>{static constexpr BSTR*& type_to_val(VARIANT& var)noexcept{return var.pbstrVal;}static constexpr BSTR*const& type_to_val(const VARIANT& var)noexcept{return var.pbstrVal;}};
template<>struct type_to_val_impl<IUnknown**>{static constexpr IUnknown**& type_to_val(VARIANT& var)noexcept{return var.ppunkVal;}static constexpr IUnknown**const& type_to_val(const VARIANT& var)noexcept{return var.ppunkVal;}};
template<>struct type_to_val_impl<IDispatch**>{static constexpr IDispatch**& type_to_val(VARIANT& var)noexcept{return var.ppdispVal;}static constexpr IDispatch**const& type_to_val(const VARIANT& var)noexcept{return var.ppdispVal;}};
template<>struct type_to_val_impl<SAFEARRAY**>{static constexpr SAFEARRAY**& type_to_val(VARIANT& var)noexcept{return var.pparray;}static constexpr SAFEARRAY**const& type_to_val(const VARIANT& var)noexcept{return var.pparray;}};
template<>struct type_to_val_impl<VARIANT*>{static constexpr VARIANT*& type_to_val(VARIANT& var)noexcept{return var.pvarVal;}static constexpr VARIANT*const& type_to_val(const VARIANT& var)noexcept{return var.pvarVal;}};
template<>struct type_to_val_impl<void*>{static constexpr void*& type_to_val(VARIANT& var)noexcept{return var.byref;}static constexpr void*const& type_to_val(const VARIANT& var)noexcept{return var.byref;}};
template<>struct type_to_val_impl<char>{static constexpr char& type_to_val(VARIANT& var)noexcept{return var.cVal;}static constexpr const char& type_to_val(const VARIANT& var)noexcept{return var.cVal;}};
template<>struct type_to_val_impl<unsigned short>{static constexpr unsigned short& type_to_val(VARIANT& var)noexcept{return var.uiVal;}static constexpr const unsigned short& type_to_val(const VARIANT& var)noexcept{return var.uiVal;}};
template<>struct type_to_val_impl<unsigned long>{static constexpr unsigned long& type_to_val(VARIANT& var)noexcept{return var.ulVal;}static constexpr const unsigned long& type_to_val(const VARIANT& var)noexcept{return var.ulVal;}};
template<>struct type_to_val_impl<unsigned long long>{static constexpr unsigned long long& type_to_val(VARIANT& var)noexcept{return var.ullVal;}static constexpr const unsigned long long& type_to_val(const VARIANT& var)noexcept{return var.ullVal;}};
template<>struct type_to_val_impl<int>{static constexpr int& type_to_val(VARIANT& var)noexcept{return var.intVal;}static constexpr const int& type_to_val(const VARIANT& var)noexcept{return var.intVal;}};
template<>struct type_to_val_impl<unsigned int>{static constexpr unsigned int& type_to_val(VARIANT& var)noexcept{return var.uintVal;}static constexpr const unsigned int& type_to_val(const VARIANT& var)noexcept{return var.uintVal;}};
template<>struct type_to_val_impl<DECIMAL*>{static constexpr DECIMAL*& type_to_val(VARIANT& var)noexcept{return var.pdecVal;}static constexpr DECIMAL*const& type_to_val(const VARIANT& var)noexcept{return var.pdecVal;}};
template<>struct type_to_val_impl<char*>{static constexpr char*& type_to_val(VARIANT& var)noexcept{return var.pcVal;}static constexpr char*const& type_to_val(const VARIANT& var)noexcept{return var.pcVal;}};
template<>struct type_to_val_impl<unsigned short*>{static constexpr unsigned short*& type_to_val(VARIANT& var)noexcept{return var.puiVal;}static constexpr unsigned short*const& type_to_val(const VARIANT& var)noexcept{return var.puiVal;}};
template<>struct type_to_val_impl<unsigned long*>{static constexpr unsigned long*& type_to_val(VARIANT& var)noexcept{return var.pulVal;}static constexpr unsigned long*const& type_to_val(const VARIANT& var)noexcept{return var.pulVal;}};
template<>struct type_to_val_impl<unsigned long long*>{static constexpr unsigned long long*& type_to_val(VARIANT& var)noexcept{return var.pullVal;}static constexpr unsigned long long*const& type_to_val(const VARIANT& var)noexcept{return var.pullVal;}};
template<>struct type_to_val_impl<int*>{static constexpr int*& type_to_val(VARIANT& var)noexcept{return var.pintVal;}static constexpr int*const& type_to_val(const VARIANT& var)noexcept{return var.pintVal;}};
template<>struct type_to_val_impl<unsigned int*>{static constexpr unsigned int*& type_to_val(VARIANT& var)noexcept{return var.puintVal;}static constexpr unsigned int*const& type_to_val(const VARIANT& var)noexcept{return var.puintVal;}};
template<typename>struct type_to_vt_impl;
template<>struct type_to_vt_impl<long long>{static constexpr VARTYPE type_to_vt()noexcept{return VT_I8;}};
template<>struct type_to_vt_impl<long>{static constexpr VARTYPE type_to_vt()noexcept{return VT_I4;}};
template<>struct type_to_vt_impl<unsigned char>{static constexpr VARTYPE type_to_vt()noexcept{return VT_UI1;}};
template<>struct type_to_vt_impl<short>{static constexpr VARTYPE type_to_vt()noexcept{return VT_I2;}};
template<>struct type_to_vt_impl<float>{static constexpr VARTYPE type_to_vt()noexcept{return VT_R4;}};
template<>struct type_to_vt_impl<double>{static constexpr VARTYPE type_to_vt()noexcept{return VT_R8;}};
template<>struct type_to_vt_impl<variant_bool>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BOOL;}};
template<>struct type_to_vt_impl<scode>{static constexpr VARTYPE type_to_vt()noexcept{return VT_ERROR;}};
template<>struct type_to_vt_impl<CY>{static constexpr VARTYPE type_to_vt()noexcept{return VT_CY;}};
template<>struct type_to_vt_impl<date>{static constexpr VARTYPE type_to_vt()noexcept{return VT_DATE;}};
template<>struct type_to_vt_impl<BSTR>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BSTR;}};
template<>struct type_to_vt_impl<IUnknown*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_UNKNOWN;}};
template<>struct type_to_vt_impl<IDispatch*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_DISPATCH;}};
template<>struct type_to_vt_impl<SAFEARRAY*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_ARRAY;}};
template<>struct type_to_vt_impl<unsigned char*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_UI1;}};
template<>struct type_to_vt_impl<short*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_I2;}};
template<>struct type_to_vt_impl<long*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_I4;}};
template<>struct type_to_vt_impl<long long*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_I8;}};
template<>struct type_to_vt_impl<float*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_R4;}};
template<>struct type_to_vt_impl<double*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_R8;}};
template<>struct type_to_vt_impl<variant_bool*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_BOOL;}};
template<>struct type_to_vt_impl<scode*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_ERROR;}};
template<>struct type_to_vt_impl<CY*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_CY;}};
template<>struct type_to_vt_impl<date*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_DATE;}};
template<>struct type_to_vt_impl<BSTR*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_BSTR;}};
template<>struct type_to_vt_impl<IUnknown**>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_UNKNOWN;}};
template<>struct type_to_vt_impl<IDispatch**>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_DISPATCH;}};
template<>struct type_to_vt_impl<SAFEARRAY**>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_ARRAY;}};
template<>struct type_to_vt_impl<VARIANT*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_VARIANT;}};
template<>struct type_to_vt_impl<void*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF;}};
template<>struct type_to_vt_impl<char>{static constexpr VARTYPE type_to_vt()noexcept{return VT_I1;}};
template<>struct type_to_vt_impl<unsigned short>{static constexpr VARTYPE type_to_vt()noexcept{return VT_UI2;}};
template<>struct type_to_vt_impl<unsigned long>{static constexpr VARTYPE type_to_vt()noexcept{return VT_UI4;}};
template<>struct type_to_vt_impl<unsigned long long>{static constexpr VARTYPE type_to_vt()noexcept{return VT_UI8;}};
template<>struct type_to_vt_impl<int>{static constexpr VARTYPE type_to_vt()noexcept{return VT_INT;}};
template<>struct type_to_vt_impl<unsigned int>{static constexpr VARTYPE type_to_vt()noexcept{return VT_UINT;}};
template<>struct type_to_vt_impl<DECIMAL*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_DECIMAL;}};
template<>struct type_to_vt_impl<char*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_I1;}};
template<>struct type_to_vt_impl<unsigned short*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_UI2;}};
template<>struct type_to_vt_impl<unsigned long*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_UI4;}};
template<>struct type_to_vt_impl<unsigned long long*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_UI8;}};
template<>struct type_to_vt_impl<int*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_INT;}};
template<>struct type_to_vt_impl<unsigned int*>{static constexpr VARTYPE type_to_vt()noexcept{return VT_BYREF|VT_UINT;}};
template<typename U>static constexpr VARTYPE type_to_vt()noexcept{return type_to_vt_impl<U>::type_to_vt();}
template<typename T, typename U = T>
class safearray_impl{
	SAFEARRAY* ptr;
public:
	explicit safearray_impl(SAFEARRAY*&& p):ptr{p}{}
	explicit safearray_impl(SAFEARRAYBOUND t):ptr{SafeArrayCreate(type_to_vt<T>(), 1, &t)}{}
	explicit safearray_impl(std::initializer_list<SAFEARRAYBOUND> il):ptr{SafeArrayCreate(type_to_vt<T>(), il.size(), const_cast<SAFEARRAYBOUND*>(il.begin()))}{}
	expected::expected<void, hresult_error> destroy(){
		const auto hr = SafeArrayDestroy(ptr);
		if(SUCCEEDED(hr))
			return {};
		return make_unexpected<hresult_error>(_T("will::safearray<T>::destroy"), hr);
	}
	~safearray_impl(){if(ptr)destroy();}
	expected::expected<safearray_impl, hresult_error> copy(){
		SAFEARRAY* o;
		const auto hr = SafeArrayCopy(ptr, &o);
		if(SUCCEEDED(hr))
			return safearray_impl{std::move(o)};
		return make_unexpected<hresult_error>(_T("will::safearray<T>::copy"), hr);
	}
	safearray_impl(const safearray_impl& other):safearray_impl(+other.copy()){}
	safearray_impl(safearray_impl&& other):ptr(other.release()){}
	safearray_impl& operator=(const safearray_impl& rhs){rhs.copy().value().swap(*this); return *this;}
	safearray_impl& operator=(safearray_impl&& rhs){reset(rhs.release()); return *this;}
	expected::expected<safearray_impl&, hresult_error> redim(SAFEARRAYBOUND t)&{
		const auto hr = SafeArrayRedim(ptr, &t);
		if(SUCCEEDED(hr)){
			ptr = nullptr;
			return *this;
		}
		return make_unexpected<hresult_error>(_T("will::safearray<T>::redim"), hr);
	}
	expected::expected<safearray_impl, hresult_error> redim(SAFEARRAYBOUND t)&&{
		const auto hr = SafeArrayRedim(ptr, &t);
		if(SUCCEEDED(hr)){
			ptr = nullptr;
			return std::move(*this);
		}
		return make_unexpected<hresult_error>(_T("will::safearray<T>::redim"), hr);
	}
	constexpr explicit operator bool()noexcept{return ptr != nullptr;}
	void detach(){ptr = nullptr;}
	SAFEARRAY* release(){auto ret = ptr; detach(); return ret;}
	expected::expected<safearray_impl&, hresult_error> reset(SAFEARRAY*&& p)&{
		return destroy().map([&]{ptr = std::move(p);return *this;});
	}
	expected::expected<safearray_impl, hresult_error> reset(SAFEARRAY*&& p)&&{
		return destroy().map([&]{ptr = std::move(p);return std::move(*this);});
	}
	void swap(safearray_impl& other){std::swap(ptr, other.ptr);}
	SAFEARRAY* get()const noexcept{return ptr;}
	operator SAFEARRAY*()const noexcept{return ptr;}
	class accessor{
		SAFEARRAY* ptr;
		explicit accessor(SAFEARRAY* p):ptr{p}{}
		friend safearray_impl;
	public:
		accessor(const accessor&) = delete;
		accessor(accessor&&) = delete;
		expected::expected<U&, hresult_error> ptr_of_index(std::initializer_list<LONG> il)noexcept{
			if(ptr->cDims != il.size())
				return make_unexpected<hresult_error>(_T("will::safearray<T>::accessor::ptr_of_index"), DISP_E_BADINDEX);
			U* u;
			const auto hr = SafeArrayPtrOfIndex(ptr, il.begin(), reinterpret_cast<void**>(&u));
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T("will::safearray<T>::accessor::ptr_of_index"), hr);
			return *u;
		}
		expected::expected<U&, hresult_error> ptr_of_index(LONG t)noexcept{return ptr_of_index({t});}
		U& operator[](LONG t)noexcept{return *ptr_of_index(t);}
		U& operator[](std::initializer_list<LONG> il)noexcept{return *ptr_of_index(il);}
		expected::expected<void, hresult_error> unlock()noexcept{
			const auto hr = SafeArrayUnlock(ptr);
			if(SUCCEEDED(hr)){
				ptr = nullptr;
				return {};
			}
			return make_unexpected<hresult_error>(_T("will::safearray<T>::accessor::unlock"), hr);
		}
		~accessor(){if(ptr)unlock();}
		constexpr explicit operator bool()noexcept{return ptr != nullptr;}
	};
	expected::expected<accessor, hresult_error> lock()noexcept{
		const auto hr = SafeArrayLock(ptr);
		if(SUCCEEDED(hr))
			return accessor{ptr};
		return make_unexpected<hresult_error>(_T("will::safearray<T>::lock"), hr);
	}
};
template<typename T>struct safearray_{using type = safearray_impl<T>;};
template<>struct safearray_<variant_bool>{using type = safearray_impl<variant_bool, VARIANT_BOOL>;};
template<>struct safearray_<scode>{using type = safearray_impl<scode, SCODE>;};
template<>struct safearray_<date>{using type = safearray_impl<date, DATE>;};
}
template<typename T>
using safearray = typename detail::safearray_<T>::type;
class variant{
	VARIANT var;
	static expected<VARIANT, hresult_error> copy(VARIANT& v)noexcept{
		VARIANT t;
		VariantInit(&t);
		const auto hr = VariantCopy(&t, &v);
		if(SUCCEEDED(hr))
			return t;
		return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
	}
	static expected<void, hresult_error> clear(VARIANT& v)noexcept{
		const auto hr = VariantClear(&v);
		if(SUCCEEDED(hr))
			return {};
		return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
	}
	static expected<VARIANT, hresult_error> change_type(VARIANT& var, VARTYPE vt)noexcept{
		VARIANT v;
		VariantInit(&v);
		const auto hr = VariantChangeType(&v, &var, VARIANT_ALPHABOOL, vt);
		if(SUCCEEDED(hr))
			return v;
		return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
	}
public:
	struct bad_cast:will::runtime_error{using will::runtime_error::runtime_error;};
	using bool_ = variant_bool;
	template<typename U>
	static constexpr VARTYPE type_to_vt()noexcept{return detail::type_to_vt<U>();}
	template<typename U>
	constexpr expected<U&> get()&{if(type_to_vt<U>() == var.vt)return detail::type_to_val_impl<U>::type_to_val(var);return make_unexpected(bad_cast("will::variant bad_cast"));}
	template<typename U>
	constexpr expected<U> get()&&{if(type_to_vt<U>() == var.vt)return std::move(detail::type_to_val_impl<U>::type_to_val(var));return make_unexpected(bad_cast("will::variant bad_cast"));}
	template<typename U>
	constexpr expected<U> get()const&{if(type_to_vt<U>() == var.vt)return detail::type_to_val_impl<U>::type_to_val(var);return make_unexpected(bad_cast("will::variant bad_cast"));}
	explicit variant()noexcept:var([]()->VARIANT{VARIANT v;VariantInit(&v);return v;}()){}
	variant(const variant& other):var(+copy(const_cast<VARIANT&>(other.var))){}
	variant(variant&& other)noexcept:var(std::move(other.release())){}
	~variant()noexcept{reset();}
	variant(const VARIANT& v):var(+copy(const_cast<VARIANT&>(v))){}
	variant(VARIANT&& v)noexcept:var(v){v.vt = VT_EMPTY;}
	template<typename U, std::enable_if_t<!std::is_same<std::decay_t<U>, variant>::value && !std::is_same<std::decay_t<U>, VARIANT>::value && !std::is_same<std::decay_t<U>, bool_>::value && !std::is_same<std::decay_t<U>, scode>::value && !std::is_same<std::decay_t<U>, date>::value && !std::is_same<std::decay_t<U>, bool_*>::value && !std::is_same<std::decay_t<U>, scode*>::value && !std::is_same<std::decay_t<U>, date*>::value && !std::is_same<std::decay_t<U>, bstr>::value && !std::is_same<std::decay_t<U>, bool>::value>* = nullptr>
	explicit variant(U&& u):variant(){var.vt = type_to_vt<std::decay_t<U>>(); *get<std::decay_t<U>>() = std::forward<U>(u);}
	explicit variant(bool_ t):variant(){var.vt = VT_BOOL; var.boolVal = t.b;}
	explicit variant(bool t):variant(bool_{t ? -1 : 0}){}
	explicit variant(scode t):variant(){var.vt = VT_ERROR; var.scode = t.s;}
	explicit variant(date t):variant(){var.vt = VT_DATE; var.date = t.d;}
	explicit variant(bstr&& t):variant(t.release()){}
	template<typename T>
	explicit variant(safearray<T>&& a):variant{a.release()}{}
	explicit variant(bool_* t):variant(){var.vt = VT_BYREF|VT_BOOL; var.pboolVal = &(t->b);}
	explicit variant(scode* t):variant(){var.vt = VT_BYREF|VT_ERROR; var.pscode = &(t->s);}
	explicit variant(date* t):variant(){var.vt = VT_BYREF|VT_DATE; var.pdate = &(t->d);}
	variant& operator=(const variant& v){+reset();var = +copy(const_cast<VARIANT&>(v.var)); return *this;}
	variant& operator=(variant&& v){+reset();var = std::move(v.release()); return *this;}
	variant& operator=(const VARIANT& v){+reset();var = +copy(const_cast<VARIANT&>(v)); return *this;}
	variant& operator=(VARIANT&& v){+reset();var = v; v.vt = VT_EMPTY; return *this;}
	template<typename T, std::enable_if_t<std::is_constructible<variant, T&&>::value>* = nullptr>
	variant& operator=(T&& t){*this = std::move(variant{std::forward<T>(t)}); return *this;}
	VARIANT& get()noexcept{return var;}
	const VARIANT& get()const noexcept{return var;}
	void detach()noexcept{
		var.vt = VT_EMPTY;
	}
	VARIANT release()noexcept{
		auto ret = var;
		detach();
		return ret;
	}
	void swap(variant& other)noexcept{
		std::swap(var, other.var);
	}
	expected<void, hresult_error> reset()noexcept{
		return clear(var);
	}
	expected<variant&, hresult_error> reset(VARIANT&& v)&noexcept{
		return clear(var).map([&]()->variant&{*this = std::move(v);return *this;});
	}
	expected<variant, hresult_error> reset(VARIANT&& v)&&noexcept{
		return clear(var).map([&]{return *this = std::move(v);});
	}
	template<typename U>
	variant as()const{
		return variant{+change_type(const_cast<VARIANT>(var), type_to_vt<U>())};
	}
	explicit operator bool()const noexcept{return static_cast<VARENUM>(var.vt) != VT_EMPTY;}
	friend bool operator==(const variant& lhs, const variant& rhs)noexcept{
		if(lhs.var.vt != rhs.var.vt)
			return false;
		switch(lhs.var.vt){
		case type_to_vt<long long>():return lhs.var.llVal == rhs.var.llVal;
		case type_to_vt<long>():return lhs.var.lVal == rhs.var.lVal;
		case type_to_vt<unsigned char>():return lhs.var.bVal == rhs.var.bVal;
		case type_to_vt<short>():return lhs.var.iVal == rhs.var.iVal;
		case type_to_vt<float>():return lhs.var.fltVal == rhs.var.fltVal;
		case type_to_vt<double>():return lhs.var.dblVal == rhs.var.dblVal;
		case type_to_vt<bool_>():return lhs.var.boolVal == rhs.var.boolVal;
		case type_to_vt<scode>():return lhs.var.scode == rhs.var.scode;
		case type_to_vt<CY>():return lhs.var.cyVal.int64 == rhs.var.cyVal.int64;
		case type_to_vt<date>():return lhs.var.date == rhs.var.date;
		case type_to_vt<BSTR>():return lhs.var.bstrVal == rhs.var.bstrVal;
		case type_to_vt<IUnknown*>():return lhs.var.punkVal == rhs.var.punkVal;
		case type_to_vt<IDispatch*>():return lhs.var.pdispVal == rhs.var.pdispVal;
		case type_to_vt<SAFEARRAY*>():return lhs.var.parray == rhs.var.parray;
		case type_to_vt<unsigned char*>():return lhs.var.pbVal == rhs.var.pbVal;
		case type_to_vt<short*>():return lhs.var.piVal == rhs.var.piVal;
		case type_to_vt<long*>():return lhs.var.plVal == rhs.var.plVal;
		case type_to_vt<long long*>():return lhs.var.pllVal == rhs.var.pllVal;
		case type_to_vt<float*>():return lhs.var.pfltVal == rhs.var.pfltVal;
		case type_to_vt<double*>():return lhs.var.pdblVal == rhs.var.pdblVal;
		case type_to_vt<bool_*>():return lhs.var.pboolVal == rhs.var.pboolVal;
		case type_to_vt<scode*>():return lhs.var.pscode == rhs.var.pscode;
		case type_to_vt<CY*>():return lhs.var.pcyVal == rhs.var.pcyVal;
		case type_to_vt<date*>():return lhs.var.pdate == rhs.var.pdate;
		case type_to_vt<BSTR*>():return lhs.var.pbstrVal == rhs.var.pbstrVal;
		case type_to_vt<IUnknown**>():return lhs.var.ppunkVal == rhs.var.ppunkVal;
		case type_to_vt<IDispatch**>():return lhs.var.ppdispVal == rhs.var.ppdispVal;
		case type_to_vt<SAFEARRAY**>():return lhs.var.pparray == rhs.var.pparray;
		case type_to_vt<VARIANT*>():return lhs.var.pvarVal == rhs.var.pvarVal;
		case type_to_vt<void*>():return lhs.var.byref == rhs.var.byref;
		case type_to_vt<char>():return lhs.var.cVal == rhs.var.cVal;
		case type_to_vt<unsigned short>():return lhs.var.uiVal == rhs.var.uiVal;
		case type_to_vt<unsigned long>():return lhs.var.ulVal == rhs.var.ulVal;
		case type_to_vt<unsigned long long>():return lhs.var.ullVal == rhs.var.ullVal;
		case type_to_vt<int>():return lhs.var.intVal == rhs.var.intVal;
		case type_to_vt<unsigned int>():return lhs.var.uintVal == rhs.var.uintVal;
		case type_to_vt<DECIMAL*>():return lhs.var.pdecVal == rhs.var.pdecVal;
		case type_to_vt<char*>():return lhs.var.pcVal == rhs.var.pcVal;
		case type_to_vt<unsigned short*>():return lhs.var.puiVal == rhs.var.puiVal;
		case type_to_vt<unsigned long*>():return lhs.var.pulVal == rhs.var.pulVal;
		case type_to_vt<unsigned long long*>():return lhs.var.pullVal == rhs.var.pullVal;
		case type_to_vt<int*>():return lhs.var.pintVal == rhs.var.pintVal;
		case type_to_vt<unsigned int*>():return lhs.var.puintVal == rhs.var.puintVal;
		}
		return false;
	}
	template<typename U>
	friend bool operator==(const variant& lhs, const U& rhs)noexcept{
		return lhs == variant{rhs};
	}
	template<typename U>
	friend bool operator==(const U& lhs, const variant& rhs)noexcept{
		return rhs == lhs;
	}
	friend bool operator!=(const variant& lhs, const variant& rhs)noexcept{
		return !(lhs == rhs);
	}
	template<typename U>
	friend bool operator!=(const variant& lhs, const U& rhs)noexcept{
		return !(lhs == rhs);
	}
	template<typename U>
	friend bool operator!=(const U& lhs, const variant rhs)noexcept{
		return !(lhs == rhs);
	}
};
template <typename T>
class com_ptr{
	T* ptr;
public:
	com_ptr()noexcept : ptr(nullptr){}
	com_ptr(T*&& ptr)noexcept : ptr(ptr){}
	com_ptr(const com_ptr& other)noexcept : ptr(other.ptr){
		if(ptr)
			ptr->AddRef();
	}
	com_ptr(com_ptr&& other)noexcept : ptr(other.release()){}
	~com_ptr()noexcept{reset();}
	void detach()noexcept{
		ptr = nullptr;
	}
	T* release()noexcept{
		auto ret = ptr;
		detach();
		return ret;
	}
	void swap(com_ptr& other)noexcept{
		std::swap(ptr, other.ptr);
	}
	void reset(T*&& p = nullptr)noexcept{
		if(ptr)
			ptr->Release();
		ptr = p;
	}
	T* get()const noexcept{return ptr;}
	com_ptr& operator=(com_ptr rhs)noexcept{
		rhs.swap(*this);
		return *this;
	}
	com_ptr& operator=(T*&& rhs)noexcept{
		reset(std::move(rhs));
		return *this;
	}
	T* operator->()const noexcept{return ptr;}
	template<typename U>
	expected<com_ptr<U>, hresult_error> as()const noexcept{
		return query_interface<U>(ptr). map([](U*      u){return com_ptr<U>{std::move(u)};})
		                              .emap([](HRESULT e){return make_unexpected<hresult_error>(_T("will::com_ptr<T>::as"), e);});
	}
	explicit operator bool()const noexcept{return ptr != nullptr;}
	template<typename U>
	friend bool operator==(const com_ptr<T>& lhs, const com_ptr<U>& rhs)noexcept{
		return lhs.ptr == rhs.ptr;
	}
	template<typename U>
	friend bool operator==(const com_ptr<T>& lhs, const U* rhs)noexcept{
		return lhs.ptr == rhs;
	}
	template<typename U>
	friend bool operator==(const U* lhs, const com_ptr<T>& rhs)noexcept{
		return lhs == rhs.ptr;
	}
	friend bool operator==(const com_ptr<T>& lhs, std::nullptr_t)noexcept{
		return !lhs;
	}
	friend bool operator==(std::nullptr_t, const com_ptr<T>& rhs)noexcept{
		return !rhs;
	}
	template<typename U>
	friend bool operator!=(const com_ptr<T>& lhs, const com_ptr<U>& rhs)noexcept{
		return !(lhs == rhs);
	}
	template<typename U>
	friend bool operator!=(const com_ptr<T>& lhs, const U* rhs)noexcept{
		return !(lhs == rhs);
	}
	template<typename U>
	friend bool operator!=(const U* lhs, const com_ptr<T>& rhs)noexcept{
		return !(lhs == rhs);
	}
	friend bool operator!=(const com_ptr<T>& lhs, std::nullptr_t)noexcept{
		return static_cast<bool>(lhs);
	}
	friend bool operator!=(std::nullptr_t, const com_ptr<T>& rhs)noexcept{
		return static_cast<bool>(rhs);
	}
};

namespace detail{

template<typename>struct get_enum_impl;
template<typename T, typename I>
struct get_enum_impl<HRESULT (__stdcall T::*)(I**)>{using type = I;};

template<typename T, typename = typename get_enum_impl<decltype(&T::get__NewEnum)>::type>struct get_enum;
template<typename T>
struct get_enum<T, IUnknown>{
	template<typename U, typename V>
	static expected::expected<com_ptr<U>, hresult_error> get(const V& t)noexcept{
		return com_create_resource<IUnknown>([&](IUnknown** ptr){return t->get__NewEnum(ptr);}).emap([](HRESULT   e){return make_unexpected<hresult_error>(_T("get__NewEnum"), e);})
		                                                                                       .bind([](IUnknown* p){return com_ptr<IUnknown>{std::move(p)}.as<U>();});
	}
};
template<typename T>
struct get_enum<T, IEnumVARIANT>{
	template<typename U, typename V, std::enable_if_t<std::is_same<U, IEnumVARIANT>::value>* = nullptr>
	static expected::expected<com_ptr<U>, hresult_error> get(const V& t)noexcept{
		return com_create_resource<IEnumVARIANT>([&](IEnumVARIANT** ptr){return t->get__NewEnum(ptr);}).emap([](HRESULT e){return make_unexpected<hresult_error>(_T("get__NewEnum"), e);});
	}
};
}

template<typename T, typename U = T*>
class com_enum_iterator{
	com_ptr<IEnumVARIANT> enu;
	variant var;
	HRESULT hr;
	com_enum_iterator(com_ptr<IEnumVARIANT>&& ptr, variant v, HRESULT h):enu(std::move(ptr)), var{v}, hr{h}{}
	static expected<com_ptr<IEnumVARIANT>, hresult_error> copy_enu(const com_ptr<IEnumVARIANT>& p){
		return com_create_resource<IEnumVARIANT>([&](IEnumVARIANT** ptr){return p->Clone(ptr);}).emap([](HRESULT e){return make_unexpected<hresult_error>(_T("IEnumVARIANT::Clone"), e);})
	}
public:
	template<typename Collection>
	static expected<com_enum_iterator, hresult_error> create(const Collection& collection)noexcept{
		return detail::get_enum<T>::get<IEnumVARIANT>(collection).map([&](com_ptr<IEnumVARIANT>&& enu){
			variant var;
			auto hr = enu->Next(1, &var.get(), nullptr);
			return com_enum_iterator{std::move(enu), std::move(var), hr};
		});
	}
	template<typename Collection>
	explicit com_enum_iterator(const Collection& collection):com_enum_iterator(+create(collection)){}
	explicit com_enum_iterator()noexcept:enu(nullptr), var(), hr(S_FALSE){}
	com_enum_iterator(const com_enum_iterator& other):enu(+copy_enu(other.enu)), var(other.var), hr(other.hr){}
	com_enum_iterator(com_enum_iterator&& other):enu(std::move(other.enu)), var(std::move(other.var)), hr(std::move(other.hr)){}
	expected<com_enum_iterator, hresult_error> next(ULONG n = 1){return var.reset().map([&]{hr = enu->Next(n, &var.get(), nullptr);});}
	com_enum_iterator& operator++(){+next(); return *this;}
	com_enum_iterator operator++(int){auto ret = +copy_enu(enu).map([&](com_ptr<IEnumVARIANT>&& t){return com_enum_iterator{std::move(t), var, hr};}); +next(); return std::move(ret);}
	com_enum_iterator& operator+=(ULONG n){+next(n); return *this;}
	friend com_enum_iterator operator+(const com_enum_iterator& lhs, ULONG rhs){return com_enum_iterator{lhs} += rhs;}
	friend com_enum_iterator operator+(ULONG lhs, const com_enum_iterator& rhs){return com_enum_iterator{rhs} += lhs;}
	friend com_enum_iterator operator+(com_enum_iterator&& lhs, ULONG rhs){return std::move(lhs += rhs);}
	friend com_enum_iterator operator+(ULONG lhs, com_enum_iterator&& rhs){return std::move(rhs += lhs);}
	bool operator==(const com_enum_iterator& rhs)const{return hr == rhs.hr && var == rhs.var;}
	bool operator!=(const com_enum_iterator& rhs)const{return !(*this == rhs);}
	expected<U, hresult_error> value()const{
		return query_interface<T>(var.get().pdispVal).map([](T* t){return U{std::move(t)};});
	}
	U operator*()const{return *value();}
};
template<typename T>
class com_base : public T{
protected:
	ULONG ref_count;
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject)override{
		if (iid == IID_IUnknown || iid == __uuidof(T)){
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}
	virtual ULONG STDMETHODCALLTYPE AddRef()override{return InterlockedIncrement(&ref_count);}
	virtual ULONG STDMETHODCALLTYPE Release()override{const auto new_count = InterlockedDecrement(&ref_count);if(new_count == 0)delete this;return new_count;}
	virtual ~com_base() = default;
};
}