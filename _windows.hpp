#pragma once
#include<cstddef>
#include<utility>
#include<chrono>
#include<string>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<windows.h>
#include<tchar.h>
#include"_expected.hpp"

namespace will{

inline void sleep(std::chrono::milliseconds ms)noexcept{::Sleep(static_cast<DWORD>(ms.count()));}

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
	template<typename T>
	tstring to_tstring(T&& t){
		return
#ifdef UNICODE
			to_wstring
#else
			to_string
#endif
			(std::forward<T>(t));
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

template<>struct error_traits<class winapi_last_error>;

class winapi_last_error{
	::LPCTSTR func_name;
	::DWORD le;
public:
	winapi_last_error(::LPCTSTR fn)noexcept:func_name(fn), le(::GetLastError()){}
	winapi_last_error(::LPCTSTR fn, ::DWORD ec)noexcept:func_name(fn), le(ec){}
	winapi_last_error(const winapi_last_error&)noexcept = default;
	winapi_last_error(winapi_last_error&&)noexcept = default;
	winapi_last_error& operator=(const winapi_last_error&)noexcept = default;
	winapi_last_error& operator=(winapi_last_error&&)noexcept = default;
	::LPCTSTR get_func_name()const noexcept{return func_name;}
	::DWORD get_error_code()const noexcept{return le;}
	expected<formatted_message, winapi_last_error> get_error_message()const noexcept{
		::LPTSTR ptr;
		if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, le, 0, reinterpret_cast<LPTSTR>(&ptr), 0, nullptr) == 0)
			return make_unexpected(winapi_last_error{_T(__FUNCTION__)});
		return {ptr};
	}
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

class winapi_last_error_exception : public std::runtime_error{
	winapi_last_error e;
public:
	winapi_last_error_exception(const winapi_last_error& ec):std::runtime_error(
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
	winapi_last_error value()const noexcept{return e;}
};

template<>
struct error_traits<winapi_last_error>{
	static constexpr bool can_make_error_from_current_exception = true;
	static winapi_last_error make_error(const winapi_last_error& e)noexcept{
		return e;
	}
	static winapi_last_error make_error(::DWORD ec){
		return winapi_last_error{_T("(unknown)"), ec};
	}
	static winapi_last_error make_error_from_current_exception()try{
		throw;
	}catch(winapi_last_error_exception& e){
		return e.value();
	}catch(...){
		return winapi_last_error{_T("(no_error)"), 0};
	}
	[[noreturn]] static void rethrow(const winapi_last_error& e){
		throw winapi_last_error_exception(e);
	}
};

namespace detail{

template<typename T, typename U>
inline will::expected<std::decay_t<T>, winapi_last_error> checked_return(LPCTSTR fn, T&& t, U&& failed_value){
	return t == failed_value ? make_unexpected(winapi_last_error{fn}) : will::expected<std::decay_t<T>, winapi_last_error>{std::forward<T>(t)};
}

}

}