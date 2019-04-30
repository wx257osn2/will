//Copyright (C) 2014-2019 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include<array>
#include<vector>
#include<utility>
#include<iomanip>
#include<mutex>
#include<sstream>
#include<variant>
#include<stdexcept>
#define WILL_INCLUDING_DBGHELP_HPP
#include"_windows.hpp"
#undef WILL_INCLUDING_DBGHELP_HPP
#ifdef UNICODE
#ifndef DBGHELP_TRANSLATE_TCHAR
#define DBGHELP_TRANSLATE_TCHAR
#endif
#endif
#if __has_include(<cvconst.h>) == false
#define _NO_CVCONST_H
#endif
#ifdef _NO_CVCONST_H
enum CV_call_e{};
enum DataKind{};
enum UdtKind{};
enum BasicType{};
#else
#include<cvconst.h>
#endif
#pragma warning(push)
#pragma warning(disable:4091)
#include<DbgHelp.h>
#pragma warning(pop)
#ifndef WILL_DBGHELP_USE_DLL
#pragma comment(lib, "DbgHelp.lib")
#endif

namespace will{

#ifdef UNICODE
using SYMBOL_INFOT = ::SYMBOL_INFOW;
using IMAGEHLP_MODULET64 = ::IMAGEHLP_MODULEW64;
using IMAGEHLP_LINET64 = struct : ::IMAGEHLP_LINEW64{DWORD Displacement;};
#else
using SYMBOL_INFOT = ::SYMBOL_INFO;
using IMAGEHLP_MODULET64 = ::IMAGEHLP_MODULE64;
using IMAGEHLP_LINET64 = struct : ::IMAGEHLP_LINE64{DWORD Displacement;};
#endif

class symbol_information{
	static constexpr std::size_t name_max = MAX_PATH;/*MAX_SYM_NAME*/
	std::array<std::byte, sizeof(SYMBOL_INFOT)+MAX_PATH*sizeof(TCHAR)> data;
public:
	symbol_information():data{}{SYMBOL_INFOT& sym = *reinterpret_cast<SYMBOL_INFOT*>(data.data());sym.SizeOfStruct = sizeof(SYMBOL_INFOT);sym.MaxNameLen = MAX_PATH;}
	const SYMBOL_INFOT* operator&()const{return reinterpret_cast<const SYMBOL_INFOT*>(data.data());}
	const SYMBOL_INFOT* operator->()const{return reinterpret_cast<const SYMBOL_INFOT*>(data.data());}
	operator const SYMBOL_INFOT&()const{return *reinterpret_cast<const SYMBOL_INFOT*>(data.data());}
};

#ifdef WILL_DBGHELP_USE_DLL
namespace detail{

class dbghelp:module_handle{
	dbghelp(module_handle&& mh):module_handle(std::move(mh)){}
public:
#define WILL_DECL_FUNC_STRING(str) #str
#define WILL_DECL_FUNC(name)\
	const decltype(&::name) name = +get_proc_address<decltype(::name)>(WILL_DECL_FUNC_STRING(name))
	WILL_DECL_FUNC(SymInitialize);
	WILL_DECL_FUNC(SymCleanup);
	WILL_DECL_FUNC(SymGetTypeInfo);
	WILL_DECL_FUNC(SymFromAddr);
	WILL_DECL_FUNC(SymGetModuleInfo64);
	WILL_DECL_FUNC(SymGetLineFromAddr64);
	WILL_DECL_FUNC(SymRefreshModuleList);
	WILL_DECL_FUNC(SymGetOptions);
	WILL_DECL_FUNC(SymSetOptions);
#undef WILL_DECL_FUNC
#undef WILL_DECL_FUNC_STRING
	static will::expected<dbghelp, winapi_last_error> create()try{
		return load_library(_T("dbghelp.dll")).map([](module_handle&& mh){return dbghelp{std::move(mh)};});
	}catch(winapi_last_error_exception& e){
		return make_unexpected(std::move(e.value()));
	}
};

class vcruntime140:module_handle{
	vcruntime140(module_handle&& mh):module_handle(std::move(mh)){}
	using undname_type = char*(char*, const char*, int, decltype(::malloc), decltype(::free), unsigned short int);
public:
	undname_type* const __unDName = +get_proc_address<undname_type>("__unDName");
	static will::expected<vcruntime140, winapi_last_error> create()try{
		return load_library(_T("vcruntime140.dll")).map([](module_handle&& mh){return vcruntime140{std::move(mh)};});
	}catch(winapi_last_error_exception& e){
		return make_unexpected(std::move(e.value()));
	}
};

}
#endif

class symbol_handler
#ifdef WILL_DBGHELP_USE_DLL
	:detail::dbghelp
	,detail::vcruntime140
#endif
{
	::HANDLE proc;
	bool is_first;
#ifdef WILL_DBGHELP_USE_DLL
	static expected<::HANDLE, winapi_last_error> initialize(const detail::dbghelp& dbghelp, ::HANDLE proc, const TCHAR* search_path = nullptr, bool invade_proc = true){
		if(dbghelp.
#else
	static expected<::HANDLE, winapi_last_error> initialize(::HANDLE proc, const TCHAR* search_path = nullptr, bool invade_proc = true){
		if(::
#endif
		     SymInitialize(proc, search_path, invade_proc ? TRUE : FALSE) == FALSE)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return proc;
	}
#ifdef WILL_DBGHELP_USE_DLL
	static expected<void, winapi_last_error> cleanup(const detail::dbghelp& dbghelp, ::HANDLE proc){
		if(dbghelp.
#else
	static expected<void, winapi_last_error> cleanup(::HANDLE proc){
		if(::
#endif
		     SymCleanup(proc) == FALSE)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return {};
	}
#ifdef WILL_DBGHELP_USE_DLL
	symbol_handler(detail::dbghelp dbghelp, detail::vcruntime140 vcruntime140, ::HANDLE h, bool f):dbghelp{std::move(dbghelp)}, vcruntime140{std::move(vcruntime140)}, proc{h}, is_first{f}{}
#else
	symbol_handler(::HANDLE h, bool f):proc{h}, is_first{f}{}
#endif
	template<typename T>
	expected<T, winapi_last_error> get_type_info(void* addr, ULONG type_idx, ::IMAGEHLP_SYMBOL_TYPE_INFO f, const TCHAR* func)const{
		T t;
		if(SymGetTypeInfo(proc, reinterpret_cast<DWORD64>(addr), type_idx, f, &t) == FALSE)
			return make_unexpected<winapi_last_error>(func);
		return t;
	}
public:
	static expected<symbol_handler, winapi_last_error> create(::HANDLE proc = ::GetCurrentProcess(), const TCHAR* search_path = nullptr, bool invade_proc = true){
#ifdef WILL_DBGHELP_USE_DLL
		auto dbghelp = detail::dbghelp::create();
		if(!dbghelp)
			return make_unexpected(std::move(dbghelp.error()));
		auto vcruntime140 = detail::vcruntime140::create();
		if(!vcruntime140)
			return make_unexpected(std::move(vcruntime140.error()));
		const auto h = initialize(*dbghelp, proc, search_path, invade_proc);
		if(h)
			return symbol_handler{std::move(*dbghelp), std::move(*vcruntime140), proc, true};
		if(h.error().get_error_code() == ERROR_INVALID_PARAMETER)
			return symbol_handler{std::move(*dbghelp), std::move(*vcruntime140), proc, false};
#else
		const auto h = initialize(proc, search_path, invade_proc);
		if(h)
			return symbol_handler{proc, true};
		if(h.error().get_error_code() == ERROR_INVALID_PARAMETER)
			return symbol_handler{proc, false};
#endif
		return make_unexpected<winapi_last_error>(h.error());
	}
	static expected<symbol_handler, winapi_last_error> create_unique_spin(unsigned int retry = 0, std::chrono::milliseconds wait_time = std::chrono::milliseconds{50}, ::HANDLE proc = ::GetCurrentProcess(), const TCHAR* search_path = nullptr, bool invade_proc = true){
		const bool infinite = retry == 0;
		do{
			auto sym = create(proc, search_path, invade_proc);
			if(!sym || !sym->is_first){
				std::this_thread::sleep_for(wait_time);
				continue;
			}
			return sym;
		}while(infinite || --retry);
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__), ERROR_NO_SYSTEM_RESOURCES);
	}
	static expected<std::pair<symbol_handler, std::unique_lock<std::mutex>>, winapi_last_error> create_unique_mutex(std::mutex& mut, ::HANDLE proc = ::GetCurrentProcess(), const TCHAR* search_path = nullptr, bool invade_proc = true){
		std::unique_lock<std::mutex> lock(mut);
		auto sym = create(proc, search_path, invade_proc);
		if(!sym)
			return make_unexpected(std::move(sym.error()));
		return std::make_pair(std::move(*sym), std::move(lock));
	}
	symbol_handler(const symbol_handler&) = delete;
	symbol_handler(symbol_handler&& other)noexcept:
#ifdef WILL_DBGHELP_USE_DLL
		dbghelp{std::move(static_cast<dbghelp&>(other))},
		vcruntime140{std::move(static_cast<vcruntime140&>(other))},
#endif
		proc{std::move(other.proc)}, is_first{std::move(other.is_first)}{other.proc = nullptr;other.is_first = false;}
	symbol_handler& operator=(symbol_handler&& rhs)noexcept{proc = std::move(rhs.proc);is_first = std::move(rhs.is_first);rhs.proc = nullptr;rhs.is_first = false;return *this;}
	expected<symbol_information, winapi_last_error> symbol(void* addr)const{
		symbol_information info;
		if(SymFromAddr(proc, reinterpret_cast<DWORD64>(addr), nullptr, const_cast<SYMBOL_INFOT*>(&info)) == FALSE)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return info;
	}
	expected<std::pair<symbol_information, DWORD64>, winapi_last_error> symbol_with_displacement(void* addr)const{
		std::pair<symbol_information, DWORD64> d;
		if(SymFromAddr(proc, reinterpret_cast<DWORD64>(addr), &d.second, const_cast<SYMBOL_INFOT*>(&d.first)) == FALSE)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return d;
	}
	expected<IMAGEHLP_MODULET64, winapi_last_error> module(void* addr)const{
		IMAGEHLP_MODULET64 mod = {sizeof(IMAGEHLP_MODULET64)};
		if(SymGetModuleInfo64(proc, reinterpret_cast<DWORD64>(addr), &mod) == FALSE)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return mod;
	}
	expected<IMAGEHLP_LINET64, winapi_last_error> line(void* addr)const{
		IMAGEHLP_LINET64 lin = {};
		lin.SizeOfStruct = sizeof(
#ifdef UNICODE
			::IMAGEHLP_LINEW64
#else
			::IMAGEHLP_LINE64
#endif
		);
		if(SymGetLineFromAddr64(proc, reinterpret_cast<DWORD64>(addr), &lin.Displacement, &lin) == FALSE)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return lin;
	}
	expected<enum ::SymTagEnum, winapi_last_error> type_tag(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_SYMTAG, _T(__FUNCTION__)).map([](DWORD tag){return static_cast<enum ::SymTagEnum>(tag);});
	}
	expected<std::wstring, winapi_last_error> type_name(void* addr, ULONG type_idx)const{
		return get_type_info<WCHAR*>(addr, type_idx, ::TI_GET_SYMNAME, _T(__FUNCTION__)).map([](WCHAR* symn){
			std::wstring ret(symn);
			::LocalFree(symn);
			return ret;
		});
	}
	expected<std::size_t, winapi_last_error> type_length(void* addr, ULONG type_idx)const{
		return get_type_info<ULONG64>(addr, type_idx, ::TI_GET_LENGTH, _T(__FUNCTION__)).map([](ULONG64 t){return static_cast<std::size_t>(t);});
	}
	//TI_GET_TYPE
	expected<ULONG, winapi_last_error> next_type_idx(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_TYPEID, _T(__FUNCTION__));
	}
	expected<enum ::BasicType, winapi_last_error> basic_type(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_BASETYPE, _T(__FUNCTION__)).map([](DWORD bt){return static_cast<enum ::BasicType>(bt);});
	}
	expected<ULONG, winapi_last_error> array_index_type_idx(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_ARRAYINDEXTYPEID, _T(__FUNCTION__));
	}
	expected<std::vector<ULONG>, winapi_last_error> type_children(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_CHILDRENCOUNT, _T("will::symbol_handler::type_children::count")).bind([&](DWORD count)->expected<std::vector<ULONG>, winapi_last_error>{
			std::vector<ULONG> buf(static_cast<std::size_t>(count)+2);
			buf[0] = count;
			buf[1] = 0;
			if(SymGetTypeInfo(proc, reinterpret_cast<DWORD64>(addr), type_idx, ::TI_FINDCHILDREN, buf.data()) == FALSE)
				return make_unexpected<winapi_last_error>(_T("will::symbol_handler::type_children"));
			buf.erase(buf.begin(), buf.begin()+2);
			buf.shrink_to_fit();
			return buf;
		});
	}
	expected<enum ::DataKind, winapi_last_error> data_kind(void* addr, ULONG idx)const{
		return get_type_info<DWORD>(addr, idx, ::TI_GET_ARRAYINDEXTYPEID, _T(__FUNCTION__)).map([](DWORD dk){return static_cast<enum ::DataKind>(dk);});
	}
	//TI_GET_ADDRESSOFFSET
	expected<DWORD, winapi_last_error> type_offset(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_OFFSET, _T(__FUNCTION__));
	}
	//TI_GET_VALUE
	expected<DWORD, winapi_last_error> array_type_count(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_COUNT, _T(__FUNCTION__));
	}
	//TI_GET_CHILDRENCOUNT is fused to TI_FINDCHILDREN
	expected<DWORD, winapi_last_error> type_bit_position(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_BITPOSITION, _T(__FUNCTION__));
	}
	expected<bool, winapi_last_error> type_is_virtually_inherited(void* addr, ULONG type_idx)const{
		return get_type_info<BOOL>(addr, type_idx, ::TI_GET_VIRTUALBASECLASS, _T(__FUNCTION__)).map([](BOOL b){return b != FALSE;});
	}
	//TI_GET_VIRTUALTABLESHAPEID(return type information is undocumented)
	expected<DWORD, winapi_last_error> type_offset_of_virtual_base_pointer(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_VIRTUALBASEPOINTEROFFSET, _T(__FUNCTION__));
	}
	expected<ULONG, winapi_last_error> class_parent_type_idx(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_CLASSPARENTID, _T(__FUNCTION__));
	}
	//TI_GET_NESTED
	//TI_GET_SYMINDEX
	//TI_GET_LEXICALPARENT
	expected<void*, winapi_last_error> type_address(void* addr, ULONG type_idx)const{
		return get_type_info<ULONG64>(addr, type_idx, ::TI_GET_ADDRESS, _T(__FUNCTION__)).map([](ULONG64 i){return reinterpret_cast<void*>(i);});
	}
	expected<DWORD, winapi_last_error> offset_from_this(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_THISADJUST, _T(__FUNCTION__));
	}
	expected<enum ::UdtKind, winapi_last_error> user_defined_type_kind(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_UDTKIND, _T(__FUNCTION__)).map([](DWORD k){return static_cast<enum ::UdtKind>(k);});
	}
	//TI_IS_EQUIV_TO
	expected<enum ::CV_call_e, winapi_last_error> type_calling_convention(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_CALLING_CONVENTION, _T(__FUNCTION__)).map([](DWORD k){return static_cast<enum ::CV_call_e>(k);});
	}
	//TI_IS_CLOSE_EQUIV_TO
	//TI_GTIEX_REQS_VALID(can't use with SymGetTypeInfo)
	expected<DWORD, winapi_last_error> offset_of_virtual_function_table(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_VIRTUALBASEOFFSET, _T(__FUNCTION__));
	}
	expected<DWORD, winapi_last_error> index_into_virtual_base_displacement_table(void* addr, ULONG type_idx)const{
		return get_type_info<DWORD>(addr, type_idx, ::TI_GET_VIRTUALBASEDISPINDEX, _T(__FUNCTION__));
	}
	expected<bool, winapi_last_error> type_is_reference(void* addr, ULONG type_idx)const{
		return get_type_info<BOOL>(addr, type_idx, ::TI_GET_IS_REFERENCE, _T(__FUNCTION__)).map([](BOOL b){return b != FALSE;});
	}
	expected<bool, winapi_last_error> type_is_indirect_virtual_base(void* addr, ULONG type_idx)const{
		return get_type_info<BOOL>(addr, type_idx, ::TI_GET_INDIRECTVIRTUALBASECLASS, _T(__FUNCTION__)).map([](BOOL b){return b != FALSE;});
	}
	expected<void, winapi_last_error> refresh_module_list(){
		if(SymRefreshModuleList(proc) == FALSE)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return {};
	}
	DWORD get_options()const{
		return SymGetOptions();
	}
	DWORD set_options(DWORD option){
		return SymSetOptions(option);
	}
	__declspec(property(get=get_options, put=set_options)) DWORD options;
#ifdef WILL_DBGHELP_USE_DLL
	expected<tstring, winapi_last_error> undecorate_symbol_name(tstring_view decorated_name, DWORD flags = UNDNAME_NO_MS_KEYWORDS)const{
		const auto buf = __unDName(nullptr, (
#ifdef UNICODE
			+to_string
#endif
			(decorated_name)).c_str(), 0, &::malloc, &::free, static_cast<unsigned short>(flags));
		std::string str(buf);
		::free(buf);
		return
#ifdef UNICODE
			+to_wstring
#endif
			(str);
	}
#else
	expected<tstring, winapi_last_error> undecorate_symbol_name(tstring_view decorated_name, DWORD flags = UNDNAME_NO_MS_KEYWORDS)const{
		tstring buf(512, _T('\0'));//How to know the length of undecorated symbol name?
		buf.resize(::UnDecorateSymbolName(decorated_name.data(), buf.data(), static_cast<DWORD>(buf.size()), flags));
		if(buf.size() == 0)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return buf;
	}
#endif
	~symbol_handler(){if(is_first)auto _ [[maybe_unused]] = cleanup(
#ifdef WILL_DBGHELP_USE_DLL
		*this,
#endif
		proc);}
};

class stack_frames{
	symbol_handler sym;
	std::vector<void*> frames;
	class stack_frame{
		const symbol_handler& sym;
		void* p;
		expected<std::wstring, winapi_last_error> _type_name_impl(ULONG type_idx)const{
			auto tag = sym.type_tag(p, type_idx);
			if(!tag)
				return make_unexpected(std::move(tag.error()));
			switch(*tag){
			case ::SymTagFunction:
			case ::SymTagFunctionArgType:
				return sym.next_type_idx(p, type_idx).bind([&](ULONG t){return _type_name_impl(t);});
			case ::SymTagFunctionType:{
				std::wstring type_name;
				{
					auto children = sym.type_children(p, type_idx);
					if(!children)
						return make_unexpected(std::move(children.error()));
					if(children->empty())
						type_name = L'(';
					else for(auto&& child : *children){
						auto x = _type_name_impl(child);
						if(!x)
							return make_unexpected(std::move(x.error()));
						if(type_name.empty())
							type_name = L'(' + *x;
						else
							type_name += L", " + *x;
					}
				}
				auto ret = sym.next_type_idx(p, type_idx).bind([&](ULONG t){return _type_name_impl(t);});
				if(!ret)
					return make_unexpected(std::move(ret.error()));
				return type_name + L")->" + *ret;
			}
			case ::SymTagBaseType:{
				auto bt = sym.basic_type(p, type_idx);
				if(!bt)
					return make_unexpected(std::move(bt.error()));
				switch(*bt){
				case 0/*::btNoType*/:
				default:
					return L"<basic type:unexpected>";
				case 1/*::btVoid*/:
					return L"void";
				case 2/*::btChar*/:
					return L"char";
				case 3/*::btWChar*/:
					return L"wchar_t";
				case 6/*::btInt*/:{
					auto len = sym.type_length(p, type_idx);
					if(!len)
						return make_unexpected(std::move(len.error()));
					switch(*len){
					case 1:
						return L"signed char";
					case 2:
						return L"short";
					case 4:
						return L"int";
					case 8:
						return L"long long";
					}
					return L"<basic type:int"+std::to_wstring(*len*8)+L"_t>";
				}
				case 7/*::btUInt*/:{
					auto len = sym.type_length(p, type_idx);
					if(!len)
						return make_unexpected(std::move(len.error()));
					switch(*len){
					case 1:
						return L"unsigned char";
					case 2:
						return L"unsigned short";
					case 4:
						return L"unsigned int";
					case 8:
						return L"unsigned long long";
					}
					return L"<basic type:uint"+std::to_wstring(*len*8)+L"_t>";
				}
				case 8/*::btFloat*/:{
					auto len = sym.type_length(p, type_idx);
					if(!len)
						return make_unexpected(std::move(len.error()));
					if(*len == 4)
						return L"float";
					if(*len == 8)
						return L"double";
					return L"<basic type:float"+std::to_wstring(*len*8)+L"_t>";
				}
				case 9/*::btBCD*/:
					return L"BCD";
				case 10/*::btBool*/:
					return L"bool";
				case 13/*::btLong*/:
					return L"long";
				case 14/*::btULong*/:
					return L"unsigned long";
				case 25/*::btCurrency*/:
					return L"CY";
				case 26/*::btDate*/:
					return L"DATE";
				case 27/*::btVariant*/:
					return L"VARIANT";
				case 28/*::btComplex*/:
					return L"<basic type:complex>";
				case 29/*::btBit*/:
					return L"<basic type:bit>";
				case 30/*::btBSTR*/:
					return L"BSTR";
				case 31/*::btHresult*/:
					return L"HRESULT";
				case 32/*::btChar16*/:
					return L"char16_t";
				case 33/*::btChar32*/:
					return L"char32_t";
				}
			}
			case ::SymTagUDT:
				return sym.type_name(p, type_idx);
			case ::SymTagArrayType:{
				std::wstring wstr;
				do{
					{
						auto count = sym.array_type_count(p, type_idx);
						if(!count)
							return make_unexpected(std::move(count.error()));
						wstr += L'[' + std::to_wstring(*count) + L']';
					}
					{
						auto next = sym.next_type_idx(p, type_idx);
						if(!next)
							return make_unexpected(std::move(next.error()));
						type_idx = *next;
					}
					auto tag = sym.type_tag(p, type_idx);
					if(!tag)
						return make_unexpected(std::move(tag.error()));
					if(*tag != ::SymTagArrayType)
						break;
				}while(true);
				auto inner = _type_name_impl(type_idx);
				if(!inner)
					return make_unexpected(std::move(inner.error()));
				return *inner + wstr;
			}
			case ::SymTagPointerType:{
				auto next = sym.next_type_idx(p, type_idx);
				if(!next)
					return make_unexpected(std::move(next.error()));
				auto inner = _type_name_impl(*next);
				if(!inner)
					return make_unexpected(std::move(inner.error()));
				return *inner + L"*";
			}
			default:
				return L"<UNEXPECTED TYPE:tag="+std::to_wstring(*tag)+L">";
			}
		}
		static bool starts_with(will::tstring_view s, will::tstring_view prefix)noexcept{
			if(s.size() < prefix.size())
				return false;
			return std::equal(prefix.begin(), prefix.end(), s.begin());
		}
		static tstring remove_keywords(will::tstring_view s){
			static const will::tstring_view keywords[] = {
				  _T("public: ")
				, _T("protected: ")
				, _T("private: ")
				, _T(" __ptr64")
				, _T("__cdecl ")
				, _T("__stdcall ")
				, _T("__fastcall ")
				, _T("__thiscall ")
				, _T("static ")
				, _T("struct ")
				, _T("coclass ")
				, _T("class ")
				, _T("virtual ")
			};
			tstring str;
			str.reserve(s.size());
			for(auto it = s.begin(); it != s.end(); ++it){
				if([&]{
					const auto tv = will::tstring_view{&*it, static_cast<will::tstring_view::size_type>(std::distance(it, s.end()))};
					for(auto&& x : keywords)
						if(starts_with(tv, x)){
							it += x.size()-1;
							return true;
						}
					return false;
				}())
					continue;
				str.push_back(*it);
			}
			return str;
		}
	public:
		stack_frame(const symbol_handler& symh, void* ptr)noexcept:sym{symh}, p{ptr}{}
		stack_frame(const stack_frame&) = default;
		stack_frame(stack_frame&&) = default;
		expected<will::symbol_information, winapi_last_error> symbol_information()const{
			return sym.symbol(p);
		}
		expected<std::pair<will::symbol_information, DWORD64>, winapi_last_error> symbol_with_displacement()const{
			return sym.symbol_with_displacement(p);
		}
		expected<IMAGEHLP_MODULET64, winapi_last_error> module_information()const{
			return sym.module(p);
		}
		expected<IMAGEHLP_LINET64, winapi_last_error> line()const{
			return sym.line(p);
		}
		expected<will::tstring, winapi_last_error> type_name()const{
			will::tstring und;
			{
				const auto opt = sym.options;
				const_cast<symbol_handler&>(sym).options = opt & ~SYMOPT_UNDNAME | SYMOPT_PUBLICS_ONLY;
				auto symu = symbol_information();
				if(!symu)
					return make_unexpected(std::move(symu.error()));
				auto undn = sym.undecorate_symbol_name((*symu)->Name);
				if(!undn)
					return make_unexpected(std::move(undn.error()));
				und = remove_keywords(*undn);
				const_cast<symbol_handler&>(sym).options = opt;
			}
			auto sym = symbol_information();
			if(!sym)
				return make_unexpected(std::move(sym.error()));
			const auto name = remove_keywords((*sym)->Name);
			auto fit = und.find(name+_T('('));
			if(und == name || fit == will::tstring::npos){
				auto type_name = _type_name_impl((*sym)->TypeIndex);
				if(!type_name)
					return make_unexpected(std::move(type_name.error()));
				return 
#ifndef UNICODE
				will::to_string
#endif
				(*type_name);
			}
			if(fit != 0)
				return und.substr(fit+name.size()) + _T("->") + und.substr(0, fit-1);
			return und.substr(fit+name.size());
		}
		friend std::ostream& operator<<(std::ostream& os, const stack_frame& f){
#ifdef UNICODE
#define WILL_CONVERT_WCHAR_TO_CHAR(wstr) +will::to_string(wstr)
#else
#define WILL_CONVERT_WCHAR_TO_CHAR(str) str
#endif
			auto _ = [](BOOL b)->expected<void, winapi_last_error>{
				if(b == FALSE)
					return make_unexpected<winapi_last_error>(_T(""), 0);
				return {};
			};
			auto module = f.module_information();
			if(!module)
				return os << "0x" << f.p;
			os << WILL_CONVERT_WCHAR_TO_CHAR(module->ModuleName);
			auto symbol = _(module->GlobalSymbols).bind([&]{return f.symbol_information();});
			if(module->GlobalSymbols == FALSE || !symbol)
				os << "!0x" << f.p;
			else{
				os << '!' << WILL_CONVERT_WCHAR_TO_CHAR(remove_keywords((*symbol)->Name));
				auto tn = _(module->TypeInfo != FALSE).bind([&]{return f.type_name();});
				if(tn)
					os << WILL_CONVERT_WCHAR_TO_CHAR(*tn);
			}
			auto ln = _(module->LineNumbers != FALSE).bind([&]{return f.line();});
			if(ln)
				os << " at " << WILL_CONVERT_WCHAR_TO_CHAR(ln->FileName) << ':' << ln->LineNumber;
			return os;
#undef WILL_CONVERT_WCHAR_TO_CHAR
		}
		friend std::wostream& operator<<(std::wostream& os, const stack_frame& f){
#ifdef UNICODE
#define WILL_CONVERT_CHAR_TO_WCHAR(wstr) wstr
#else
#define WILL_CONVERT_CHAR_TO_WCHAR(str) +will::to_wstring(str)
#endif
			auto _ = [](BOOL b)->expected<void, winapi_last_error>{
				if(b == FALSE)
					return make_unexpected<winapi_last_error>(_T(""), 0);
				return {};
			};
			auto module = f.module_information();
			if(!module)
				return os << L"0x" << f.p;
			os << WILL_CONVERT_CHAR_TO_WCHAR(module->ModuleName);
			auto symbol = _(module->GlobalSymbols).bind([&]{return f.symbol_information();});
			if(module->GlobalSymbols == FALSE || !symbol)
				os << L"!0x" << f.p;
			else{
				os << L'!' << WILL_CONVERT_CHAR_TO_WCHAR(remove_keywords((*symbol)->Name));
				auto tn = _(module->TypeInfo != FALSE).bind([&]{return f.type_name();});
				if(tn)
					os << WILL_CONVERT_CHAR_TO_WCHAR(*tn);
			}
			auto ln = _(module->LineNumbers != FALSE).bind([&]{return f.line();});
			if(ln)
				os << L" at " << WILL_CONVERT_CHAR_TO_WCHAR(ln->FileName) << L':' << ln->LineNumber;
			return os;
#undef WILL_CONVERT_CHAR_TO_WCHAR
		}
	};
	class stack_frame_iterator{
		const symbol_handler& sym;
		std::vector<void*>::const_iterator it;
	public:
		stack_frame_iterator(const symbol_handler& symh, std::vector<void*>::const_iterator itr)noexcept:sym{symh}, it{itr}{}
		stack_frame_iterator(const stack_frame_iterator&) = default;
		stack_frame_iterator(stack_frame_iterator&&) = default;
		bool operator==(const stack_frame_iterator& rhs)const noexcept{return it == rhs.it;}
		bool operator!=(const stack_frame_iterator& rhs)const noexcept{return it != rhs.it;}
		stack_frame_iterator& operator++()noexcept{++it;return *this;}
		stack_frame_iterator& operator--()noexcept{--it;return *this;}
		stack_frame_iterator  operator++(int)noexcept{stack_frame_iterator copied(sym, it);++it;return copied;}
		stack_frame_iterator  operator--(int)noexcept{stack_frame_iterator copied(sym, it);--it;return copied;}
		stack_frame operator*()const noexcept{return {sym, *it};}
		stack_frame operator->()const noexcept{return {sym, *it};}
	};
public:
	stack_frames(symbol_handler&& sym, const std::vector<void*>& frames)noexcept:sym(std::move(sym)), frames(frames){}
	stack_frames(symbol_handler&& sym, std::vector<void*>&& frames)noexcept:sym(std::move(sym)), frames(std::move(frames)){}
	static auto to_stack_frames(std::vector<void*>&& frames)noexcept{
		return [fs = std::move(frames)](symbol_handler&& symh)mutable{return stack_frames{std::move(symh), std::move(fs)};};
	}
	static auto to_stack_frames(const std::vector<void*>& frames)noexcept{
		return [fs = std::cref(frames)](symbol_handler&& symh){return stack_frames{std::move(symh), fs};};
	}
	stack_frames(const stack_frames&) = delete;
	stack_frames(stack_frames&&) = default;
	stack_frame_iterator begin()const noexcept{return {sym, std::cbegin<std::vector<void*>>(frames)};}
	stack_frame_iterator end  ()const noexcept{return {sym, std::cend  <std::vector<void*>>(frames)};}
	stack_frame operator[](std::size_t i)const{return {sym, frames[i]};}
	constexpr std::size_t size()const noexcept{return std::size(frames);}
	constexpr bool empty()const noexcept{return std::empty(frames);}
	friend std::ostream& operator<<(std::ostream& os, const stack_frames& sf){
		int i = 0;
		for(auto&& x : sf)
			(i == 0 ? os : os << '\n') << std::setw(2) << i++ << "# " << x;
		return os;
	}
	friend std::wostream& operator<<(std::wostream& os, const stack_frames& sf){
		int i = 0;
		for(auto&& x : sf)
			(i == 0 ? os : os << L'\n') << std::setw(2) << i++ << L"# " << x;
		return os;
	}
	std::string to_string()const{
		std::stringstream ss;
		ss << *this;
		return ss.str();
	}
	std::wstring to_wstring()const{
		std::wstringstream ss;
		ss << *this;
		return ss.str();
	}
	will::tstring to_tstring()const{
		will::tstringstream ss;
		ss << *this;
		return ss.str();
	}
};

inline expected<std::vector<void*>> capture_stack_back_trace(DWORD frames_to_skip = 1, DWORD frames_to_capture = 62)noexcept try{
	std::vector<void*> frames(frames_to_capture);
	const auto size = ::RtlCaptureStackBackTrace(frames_to_skip, frames_to_capture, frames.data(), nullptr);
	frames.resize(size);
	return frames;
}catch(...){
	return make_unexpected_from_current_exception();
}

inline expected<std::pair<std::vector<void*>, DWORD>> capture_stack_back_trace_with_hash(DWORD frames_to_skip = 1, DWORD frames_to_capture = 62)try{
	std::pair<std::vector<void*>, DWORD> ret(std::vector<void*>(frames_to_capture), 0);
	const auto size = ::RtlCaptureStackBackTrace(frames_to_skip, frames_to_capture, ret.first.data(), &ret.second);
	ret.first.resize(size);
	return ret;
}catch(...){
	return make_unexpected_from_current_exception();
}

}

#include"_exception_impl.hpp"
