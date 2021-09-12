//Copyright (C) 2014-2017, 2019, 2021 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include<cstddef>
#include<utility>
#include<chrono>
#include<thread>
#include<string>
#include<string_view>
#include<filesystem>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<windows.h>
#include<tchar.h>
#include"_expected.hpp"
#include"_exception_fwd.hpp"

namespace will{

inline namespace tchar{
	template<std::size_t N>
	inline ::errno_t tcscpy(TCHAR (&dst)[N], const TCHAR* src)noexcept{
		return ::_tcscpy_s(dst, src);
	}
	inline ::errno_t tcscpy(const std::pair<TCHAR*, std::size_t>& dst, const TCHAR* src)noexcept{
		return ::_tcscpy_s(dst.first, dst.second, src);
	}
	inline int tcscmp(const TCHAR* a, const TCHAR* b)noexcept{
		return _tcscmp(a, b);
	}
	using tstring = std::basic_string<TCHAR>;
	using tstringstream = std::basic_stringstream<TCHAR>;
	using tstring_view = std::basic_string_view<TCHAR>;
	template<typename T>
	tstring to_tstring(T&& t){
		using std::to_string;
		using std::to_wstring;
		return
#ifdef UNICODE
			to_wstring
#else
			to_string
#endif
			(std::forward<T>(t));
	}
	tstring to_tstring(const std::filesystem::path& p){
#ifdef UNICODE
		return p.wstring();
#else
		return p.string();
#endif
	}
}

class formatted_message{
	::LPTSTR ptr;
public:
	formatted_message(::LPTSTR t):ptr(t){}
	formatted_message(const formatted_message&) = delete;
	formatted_message(formatted_message&& other)noexcept:ptr(other.ptr){
		other.ptr = nullptr;
	}
	~formatted_message(){if(ptr)::LocalFree(ptr);}
	operator ::LPCTSTR()const{return ptr;}
	::LPCTSTR get()const{return ptr;}
};

class winapi_last_error;

template<>
struct error_traits<winapi_last_error>{
	static constexpr bool can_make_error_from_current_exception = true;
	static winapi_last_error make_error(const winapi_last_error& e)noexcept;
	static winapi_last_error make_error(::DWORD ec);
	static winapi_last_error make_error_from_current_exception();
	[[noreturn]] static void rethrow(const winapi_last_error& e);
};

#ifdef WILL_USE_STACK_TRACE
inline expected<std::vector<void*>> capture_stack_back_trace(DWORD, DWORD)noexcept;
#endif

class winapi_last_error{
	::LPCTSTR func_name;
	::DWORD le;
#ifdef WILL_USE_STACK_TRACE
	std::vector<void*> sf;
#define WILL_WLE_NOEXCEPT
#else
#define WILL_WLE_NOEXCEPT noexcept
#endif
public:
	winapi_last_error(::LPCTSTR fn, ::DWORD ec)noexcept:func_name(fn), le(ec){
#ifdef WILL_USE_STACK_TRACE
		auto csf = capture_stack_back_trace(1, 62);
		if(csf)
			sf = std::move(*csf);
#endif
	}
public:
	winapi_last_error(::LPCTSTR fn)noexcept:winapi_last_error(fn, ::GetLastError()){}
	winapi_last_error(const winapi_last_error&)WILL_WLE_NOEXCEPT = default;
	winapi_last_error& operator=(const winapi_last_error&)WILL_WLE_NOEXCEPT = default;
	winapi_last_error(winapi_last_error&&)noexcept = default;
	winapi_last_error& operator=(winapi_last_error&&)noexcept = default;
	::LPCTSTR get_func_name()const noexcept{return func_name;}
	::DWORD get_error_code()const noexcept{return le;}
	expected<formatted_message, winapi_last_error> get_error_message()const noexcept{
		::LPTSTR ptr;
		if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, le, 0, reinterpret_cast<LPTSTR>(&ptr), 0, nullptr) == 0)
			return make_unexpected(winapi_last_error{_T(__FUNCTION__)});
		return {ptr};
	}
#ifdef WILL_USE_STACK_TRACE
	class stack_frames stack_frames(symbol_handler&&)const noexcept;
#endif
#undef WILL_WLE_NOEXCEPT
};

inline expected<formatted_message, winapi_last_error> get_system_error_message(DWORD error_code){
	::LPTSTR ptr = nullptr;
	if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error_code, 0, reinterpret_cast<LPTSTR>(&ptr), 0, nullptr) == 0)
		return make_unexpected(winapi_last_error{_T(__FUNCTION__)});
	return {ptr};
}

inline expected<std::string, winapi_last_error> to_string(const wchar_t* str, int len, UINT code_page){
	std::string buf(::WideCharToMultiByte(code_page, 0, str, len, nullptr, 0, nullptr, nullptr), '\0');
	if(::WideCharToMultiByte(code_page, 0, str, len, &buf[0], static_cast<int>(buf.size()), nullptr, nullptr) == 0)
		return make_unexpected(winapi_last_error{_T(__FUNCTION__)});
	buf.resize(std::strlen(buf.c_str()));
	buf.shrink_to_fit();
	return buf;
}
inline expected<std::string, winapi_last_error> to_string(const wchar_t* str, UINT code_page){return to_string(str, -1, code_page);}
inline expected<std::string, winapi_last_error> to_string(const wchar_t* str){return to_string(str, CP_ACP);}
template<std::size_t N>
inline expected<std::string, winapi_last_error> to_string(const wchar_t (&str)[N], UINT code_page = CP_ACP){return to_string(str, static_cast<int>(N), code_page);}
inline expected<std::string, winapi_last_error> to_string(const std::wstring& str, UINT code_page = CP_ACP){return to_string(str.c_str(), static_cast<int>(str.size()), code_page);}
inline expected<std::string, winapi_last_error> to_string(const std::wstring_view& str, UINT code_page = CP_ACP){return to_string(str.data(), static_cast<int>(str.size()), code_page);}

inline expected<std::wstring, winapi_last_error> to_wstring(const char* str, int len, UINT code_page){
	std::wstring buf(::MultiByteToWideChar(code_page, 0, str, len, nullptr, 0), L'\0');
	if(::MultiByteToWideChar(code_page, 0, str, len, &buf[0], static_cast<int>(buf.size())) == 0)
		return make_unexpected(winapi_last_error{_T(__FUNCTION__)});
	buf.resize(std::wcslen(buf.c_str()));
	buf.shrink_to_fit();
	return buf;
}
inline expected<std::wstring, winapi_last_error> to_wstring(const char* str, UINT code_page){return to_wstring(str, -1, code_page);}
inline expected<std::wstring, winapi_last_error> to_wstring(const char* str){return to_wstring(str, CP_ACP);}
template<std::size_t N>
inline expected<std::wstring, winapi_last_error> to_wstring(const char (&str)[N], UINT code_page = CP_ACP){return to_wstring(str, static_cast<int>(N), code_page);}
inline expected<std::wstring, winapi_last_error> to_wstring(const std::string& str, UINT code_page = CP_ACP){return to_wstring(str.c_str(), static_cast<int>(str.size()), code_page);}
inline expected<std::wstring, winapi_last_error> to_wstring(const std::string_view& str, UINT code_page = CP_ACP){return to_wstring(str.data(), static_cast<int>(str.size()), code_page);}

class winapi_last_error_exception : public will::runtime_error{
	winapi_last_error e;
protected:
	virtual const char* exception_name()const noexcept{return "will::winapi_last_error";}
public:
	winapi_last_error_exception(const winapi_last_error& ec):runtime_error(
#ifdef UNICODE
		to_string(
#endif
		(tstring(ec.get_func_name()) + _T(" : ") + ec.get_error_message().value().get()).c_str()
#ifdef UNICODE
		, -1, CP_ACP).value()
#endif
	), e{ec}{}
	winapi_last_error_exception(::LPCTSTR func_name):winapi_last_error_exception({func_name, ::GetLastError()}){}
	::LPCTSTR get_func_name()const noexcept{return e.get_func_name();}
	::DWORD get_error_code()const noexcept{return e.get_error_code();}
	const winapi_last_error& value()const noexcept{return e;}
	winapi_last_error value()noexcept{return std::move(e);}
#ifdef WILL_USE_STACK_TRACE
	virtual const char* what()const noexcept;
#endif
};

inline winapi_last_error error_traits<winapi_last_error>::make_error(const winapi_last_error& e)noexcept{
	return e;
}
inline winapi_last_error error_traits<winapi_last_error>::make_error(::DWORD ec){
	return winapi_last_error{_T("(unknown)"), ec};
}
inline winapi_last_error error_traits<winapi_last_error>::make_error_from_current_exception()try{
	throw;
}catch(winapi_last_error_exception& e){
	return std::move(e.value());
}catch(...){
	return winapi_last_error{_T("(no_error)"), 0};
}
[[noreturn]] inline void error_traits<winapi_last_error>::rethrow(const winapi_last_error& e){
	throw winapi_last_error_exception(e);
}

namespace detail{

template<typename T, typename U>
inline will::expected<std::decay_t<T>, winapi_last_error> checked_return(LPCTSTR fn, T&& t, U&& failed_value){
	return t == failed_value ? make_unexpected(winapi_last_error{fn}) : will::expected<std::decay_t<T>, winapi_last_error>{std::forward<T>(t)};
}

template<typename T>
struct get_proc_addr_impl{
	using result_type = T*;
	static result_type impl(HMODULE hm, LPCSTR pn){return reinterpret_cast<result_type>(::GetProcAddress(hm, pn));}
};
template<typename Ret, typename... Args>
struct get_proc_addr_impl<Ret(Args...)>{
	using result_type = Ret(*)(Args...);
	static result_type impl(HMODULE hm, LPCSTR pn){return reinterpret_cast<result_type>(::GetProcAddress(hm, pn));}
};

}

class module_handle{
	::HMODULE hmodule;
public:
	explicit module_handle(::HMODULE&& hmod):hmodule{hmod}{}
	module_handle(module_handle&& other)noexcept:hmodule{std::move(other.release())}{}
	module_handle& operator=(module_handle&& rhs)noexcept{this->reset(std::move(rhs.release()));return *this;}
	expected<void, winapi_last_error> free(){
		if(hmodule){
			if(::FreeLibrary(hmodule) == FALSE)
				return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
			hmodule = nullptr;
		}
		return {};
	}
	expected<void, winapi_last_error> reset(::HMODULE&& other){
		return free().map([&]{hmodule = std::move(other);});
	}
	void swap(module_handle& other)noexcept{
		std::swap(hmodule, other.hmodule);
	}
	::HMODULE release()noexcept{
		const auto hmod = hmodule;
		hmodule = nullptr;
		return hmod;
	}
	bool is_datafile()const{return (reinterpret_cast<ULONG_PTR>(hmodule) & static_cast<ULONG_PTR>(1)) != 0;}
	bool is_image_mapping()const{return (reinterpret_cast<ULONG_PTR>(hmodule) & static_cast<ULONG_PTR>(2)) != 0;}
	bool is_resource()const{return is_image_mapping() || is_datafile();}
	template<typename T>
	expected<typename detail::get_proc_addr_impl<T>::result_type, winapi_last_error> get_proc_address(LPCSTR proc_name)const{
		auto addr = detail::get_proc_addr_impl<T>::impl(hmodule, proc_name);
		if(addr == nullptr)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return addr;
	}
	template<typename T>
	expected<typename detail::get_proc_addr_impl<T>::result_type, winapi_last_error> get_proc_address(const std::string& proc_name)const{return get_proc_address<T>(proc_name.c_str());}
	~module_handle(){auto _ [[maybe_unused]] = free();}
};

inline expected<module_handle, winapi_last_error> load_library(LPCTSTR file_name, DWORD flags = 0){
	auto mod = ::LoadLibraryEx(file_name, nullptr, flags);
	if(mod == nullptr)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return module_handle{std::move(mod)};
}
inline expected<module_handle, winapi_last_error> load_library(const tchar::tstring& file_name, DWORD flags = 0){return load_library(file_name.c_str(), flags);}

template<typename T, typename... Args>
inline expected<decltype(std::declval<T>()(std::declval<Args>()...)), winapi_last_error> call_dll_function(::LPCTSTR dll_name, ::LPCSTR function_name, Args&&... args){
	return load_library(dll_name).bind([&](module_handle&& mh){return mh.get_proc_address<T>(function_name);}).map([&](typename detail::get_proc_addr_impl<T>::result_type&& proc_addr){return proc_addr(std::forward<Args>(args)...);});
}

namespace detail{

inline will::expected<HMODULE, winapi_last_error> get_module_handle_ex(DWORD flags, LPCTSTR module_name){
	HMODULE handle;
	if(::GetModuleHandleEx(flags, module_name, &handle) != 0)
		return handle;
	return make_unexpected(winapi_last_error{_T(__FUNCTION__)});
}

template<typename T, typename... Args>
inline will::expected<HMODULE, winapi_last_error> get_module_handle_ex(DWORD flags, T(*fp)(Args...)){
	return get_module_handle_ex(flags | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<LPCTSTR>(fp));
}

}

inline expected<module_handle, winapi_last_error> get_current_module(){
	return detail::get_module_handle_ex(0, &get_current_module).map([](HMODULE&& hmod){return module_handle{std::move(hmod)};});
}

inline expected<module_handle, winapi_last_error> get_module_handle(LPCTSTR file_name){
	return detail::get_module_handle_ex(0, file_name).map([](HMODULE&& hmod){return module_handle{std::move(hmod)};});
}
inline expected<module_handle, winapi_last_error> get_module_handle(const tchar::tstring& file_name){return get_module_handle(file_name.c_str());}

inline expected<will::tstring, winapi_last_error> get_system_directory(){
	will::tstring buf(1, _T('\0'));
	{
		const auto size = static_cast<std::size_t>(::GetSystemDirectory(buf.data(), static_cast<UINT>(buf.size())));
		if(size == 0)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		buf.resize(size);
	}
	const auto size = static_cast<std::size_t>(::GetSystemDirectory(buf.data(), static_cast<UINT>(buf.size())));
	if(size == 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	buf.resize(size);
	return buf;
}

inline expected<will::tstring, winapi_last_error> get_module_file_name(HINSTANCE mod){
	will::tstring buf(MAX_PATH, _T('\0'));
	std::size_t ret = static_cast<std::size_t>(::GetModuleFileName(mod, buf.data(), static_cast<DWORD>(buf.size())));
	if(ret == 0)
		return will::make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	while(ret >= buf.size()){
		buf.resize(buf.size()*2);
		ret = static_cast<std::size_t>(::GetModuleFileName(mod, buf.data(), static_cast<DWORD>(buf.size())));
		if(ret == 0)
			return will::make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	}
	buf.resize(ret);
	return buf;
}

}

#include"_exception_impl.hpp"
