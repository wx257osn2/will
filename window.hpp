//Copyright (C) 2014-2017, 2019 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_windows.hpp"
#include"_2dim.hpp"
#include"resource.hpp"
#include<unordered_map>
#include<functional>
#include<chrono>

#include"_2dim_point_rect.hpp"

namespace will{

class window{
	HWND hwnd;
	static LRESULT CALLBACK proc_(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
		::SetLastError(0);
		const auto it = reinterpret_cast<window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if(it == nullptr){
			const auto le = ::GetLastError();
			if(le != 0)
				throw winapi_last_error_exception{_T("will::window::get_long_ptr")};
			return ::DefWindowProc(hwnd, msg, wp, lp);
		}
		return it->procedure(msg, wp, lp);
	}
	static expected<RECT, winapi_last_error> _getwrect(HWND hwnd){RECT r;if(::GetWindowRect(hwnd, &r) != 0)return r;return make_unexpected<winapi_last_error>(_T(__FUNCTION__));}
	static expected<RECT, winapi_last_error> _getcrect(HWND hwnd){RECT r;if(::GetClientRect(hwnd, &r) != 0)return r;return make_unexpected<winapi_last_error>(_T(__FUNCTION__));}
	static expected<void, winapi_last_error> _setrect(HWND hwnd, int x, int y, int w, int h, UINT flags = SWP_NOZORDER){if(::SetWindowPos(hwnd, nullptr, x, y, w, h, flags) != 0)return {};return make_unexpected(winapi_last_error{_T(__FUNCTION__)});}
	class _wxywh{
		window& w;
		class _xy{
			window& w;
		public:
			explicit _xy(window& wi):w{wi}{}
			operator will::two_dim::xy<int>()const{const auto _ =* _getwrect(w.get()); return {_.left, _.top};}
			int get_x()const{return _getwrect(w.get())->left;}
			void set_x(int t){auto _ [[maybe_unused]] = _setrect(w.get(), t, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);}
			__declspec(property(get=get_x, put=set_x)) int x;
			int get_y()const{return _getwrect(w.get())->top;}
			void set_y(int t){auto _ [[maybe_unused]] = _setrect(w.get(), x, t, 0, 0, SWP_NOZORDER | SWP_NOSIZE);}
			__declspec(property(get=get_y, put=set_y)) int y;
			friend _wxywh;
			friend window;
		};
		class _wh{
			window& w_;
		public:
			explicit _wh(window& wi):w_{wi}{}
			operator will::two_dim::wh<int>()const{const auto _ =* _getwrect(w_.get()); return {_.right - _.left, _.bottom - _.top};}
			int get_w()const{const auto r =* _getwrect(w_.get()); return r.right - r.left;}
			void set_w(int t){auto _ [[maybe_unused]] = _setrect(w_.get(), 0, 0, t, h, SWP_NOZORDER | SWP_NOMOVE);}
			__declspec(property(get=get_w, put=set_w)) int w;
			int get_h()const{const auto r =* _getwrect(w_.get()); return r.bottom - r.top;}
			void set_h(int t){auto _ [[maybe_unused]] = _setrect(w_.get(), 0, 0, w, t, SWP_NOZORDER | SWP_NOMOVE);}
			__declspec(property(get=get_h, put=set_h)) int h;
			friend _wxywh;
			friend window;
		};
		template<typename>friend struct will::two_dim::attribute_traits;
	public:
		explicit _wxywh(window& w):w(w){}
		operator will::two_dim::xywh<long>()const{return will::two_dim::attribute(*_getwrect(w.get()));}
		_xy get_xy()const{return _xy{w};}
		void set_xy(const will::two_dim::xy<int>& t){auto _ [[maybe_unused]] = _setrect(w.get(), t.x, t.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);}
		__declspec(property(get=get_xy, put=set_xy)) _xy xy;
		_wh get_wh()const{return _wh{w};}
		void set_wh(const will::two_dim::wh<int>& t){auto _ [[maybe_unused]] = _setrect(w.get(), 0, 0, t.w, t.h, SWP_NOZORDER | SWP_NOMOVE);}
		__declspec(property(get=get_wh, put=set_wh)) _wh wh;
		friend window;
	};
	class _wxyxy{
		window& w;
		class _1t{
			window& w;
		public:
			explicit _1t(window& wi):w{wi}{}
			operator will::two_dim::xy<int>()const{const auto _ =* _getwrect(w.get()); return {_.left, _.top};}
			int get_x()const{return _getwrect(w.get())->left;}
			void set_x(int t){auto _ [[maybe_unused]] = _setrect(w.get(), t, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);}
			__declspec(property(get=get_x, put=set_x)) int x;
			int get_y()const{return _getwrect(w.get())->top;}
			void set_y(int t){auto _ [[maybe_unused]] = _setrect(w.get(), x, t, 0, 0, SWP_NOZORDER | SWP_NOSIZE);}
			__declspec(property(get=get_y, put=set_y)) int y;
			friend _wxyxy;
			friend window;
		};
		class _2t{
			window& w;
		public:
			explicit _2t(window& wi):w{wi}{}
			operator will::two_dim::xy<int>()const{const auto _ =* _getwrect(w.get()); return {_.right, _.bottom};}
			int get_x()const{return _getwrect(w.get())->right;}
			void set_x(int t){const auto v =* _getwrect(w.get()); auto _ [[maybe_unused]] = _setrect(w.get(), 0, 0, t - v.left, v.bottom, SWP_NOZORDER | SWP_NOMOVE);}
			__declspec(property(get=get_x, put=set_x)) int x;
			int get_y()const{return _getwrect(w.get())->bottom;}
			void set_y(int t){const auto v =* _getwrect(w.get()); auto _ [[maybe_unused]] = _setrect(w.get(), 0, 0, v.right, t - v.top, SWP_NOZORDER | SWP_NOMOVE);}
			__declspec(property(get=get_y, put=set_y)) int y;
			friend _wxyxy;
			friend window;
		};
		template<typename>friend struct will::two_dim::attribute_traits;
	public:
		explicit _wxyxy(window& w):w(w){}
		operator will::two_dim::xyxy<long>()const{return will::two_dim::attribute<will::two_dim::xyxy<long>>(*_getwrect(w.get()));}
		_1t get_1()const{return _1t{w};}
		void set_1(const will::two_dim::xy<int>& t){auto _ [[maybe_unused]] = _setrect(w.get(), t.x, t.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);}
		__declspec(property(get=get_1, put=set_1)) _1t _1;
		_2t get_2()const{return _2t{w};}
		void set_2(const will::two_dim::xy<int>& t){const auto v =* _getwrect(w.get()); auto _ [[maybe_unused]] = _setrect(w.get(), 0, 0, t.x - v.left, t.y - v.top, SWP_NOZORDER | SWP_NOSIZE);}
		__declspec(property(get=get_2, put=set_2)) _2t _2;
	};
	class _cxywh{
		window& w;
		class _xy{
			window& w;
		public:
			explicit _xy(window& wi):w{wi}{}
			operator will::two_dim::xy<int>()const{const auto _ =* _getcrect(w.get()); return {_.left, _.top};}
			int get_x()const{return _getcrect(w.get())->left;}
			void set_x(int t){const will::two_dim::xy<int> _w = w.xy; auto _ [[maybe_unused]] = _setrect(w.get(), _w.x - get_x() + t, _w.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);}
			__declspec(property(get=get_x, put=set_x)) int x;
			int get_y()const{return _getcrect(w.get())->top;}
			void set_y(int t){const will::two_dim::xy<int> _w = w.xy; auto _ [[maybe_unused]] = _setrect(w.get(), _w.x, _w.y - get_y() + t, 0, 0, SWP_NOZORDER | SWP_NOSIZE);}
			__declspec(property(get=get_y, put=set_y)) int y;
			friend _cxywh;
			friend window;
		};
		class _wh{
			window& w_;
		public:
			explicit _wh(window& wi):w_{wi}{}
			operator will::two_dim::wh<int>()const{const auto _ =* _getcrect(w_.get()); return {_.right, _.bottom};}
			int get_w()const{const auto r =* _getcrect(w_.get()); return r.right - r.left;}
			void set_w(int t){const will::two_dim::wh<int> _w = w_.wh; auto _ [[maybe_unused]] = _setrect(w_.get(), 0, 0, w_.w - get_w() + t, _w.h, SWP_NOZORDER | SWP_NOMOVE);}
			__declspec(property(get=get_w, put=set_w)) int w;
			int get_h()const{const auto r =* _getcrect(w_.get()); return r.bottom - r.top;}
			void set_h(int t){const will::two_dim::wh<int> _w = w_.wh; auto _ [[maybe_unused]] = _setrect(w_.get(), 0, 0, w_.w, w_.h - get_h() + t, SWP_NOZORDER | SWP_NOMOVE);}
			__declspec(property(get=get_h, put=set_h)) int h;
			friend _cxywh;
			friend window;
		};
		template<typename>friend struct will::two_dim::attribute_traits;
	public:
		explicit _cxywh(window& w):w(w){}
		operator will::two_dim::xywh<long>()const{return will::two_dim::attribute(*_getcrect(w.get()));}
		_xy get_xy()const{return _xy{w};}
		void set_xy(const will::two_dim::xy<int>& t){const will::two_dim::xy<int> v = get_xy(), _w = w.xy;auto _ [[maybe_unused]] = _setrect(w.get(), _w.x - v.x + t.x, _w.y - v.y + t.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);}
		__declspec(property(get=get_xy, put=set_xy)) _xy xy;
		_wh get_wh()const{return _wh{w};}
		void set_wh(const will::two_dim::wh<int>& t){const will::two_dim::wh<int> v = get_wh(), _w = w.wh;auto _ [[maybe_unused]] = _setrect(w.get(), 0, 0, _w.w - v.w + t.w, _w.h - v.h + t.h, SWP_NOZORDER | SWP_NOMOVE);}
		__declspec(property(get=get_wh, put=set_wh)) _wh wh;
		friend window;
	};
	template<typename T>friend struct will::two_dim::attribute_traits;
	expected<LONG_PTR, winapi_last_error> set_long_ptr(int index, LONG_PTR value)noexcept{
		::SetLastError(0);
		const auto prev = ::SetWindowLongPtr(get(), index, value);
		if(prev == 0){
			const auto le = ::GetLastError();
			if(le != 0)
				return make_unexpected(winapi_last_error{_T(__FUNCTION__), le});
		}
		return _setrect(get(), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED).map([&](void){return prev;});
	}
	template<typename T>
	expected<T, winapi_last_error> set_long_ptr(int index, T t)noexcept{return set_long_ptr(index, static_cast<LONG_PTR>(t)).map([](LONG_PTR ret){return static_cast<T>(ret);});}
	expected<LONG_PTR, winapi_last_error> get_long_ptr(int index)const noexcept{
		::SetLastError(0);
		const auto ret = ::GetWindowLongPtr(get(), index);
		if(ret == 0){
			const auto le = ::GetLastError();
			if(le != 0)
				return make_unexpected(winapi_last_error{_T(__FUNCTION__), le});
		}
		return ret;
	}
	template<typename T>
	expected<T, winapi_last_error> get_long_ptr(int index)const noexcept{return get_long_ptr(index).map([](LONG_PTR ret){return static_cast<T>(ret);});}
	window(HWND hw):hwnd{hw}{+set_long_ptr(GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));}
	static LRESULT wm_destroy(window&, WPARAM, LPARAM){if(std::uncaught_exceptions() == 0)::PostQuitMessage(0);return 0;}
public:
	static int exception_handler()try{
		throw;
	}catch(std::exception& e){
		::MessageBoxA(nullptr, e.what(), "exception", MB_OK|MB_ICONERROR|MB_SETFOREGROUND);
		::PostQuitMessage(0);
		return 56;
	}
	window(const window&) = delete;
	window(window&& other)noexcept(false):hwnd{other.hwnd}{+set_long_ptr(GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)); messenger = std::move(other.messenger); other.hwnd = nullptr;}
	struct property{
		DWORD _exstyle = 0;
		LPCTSTR _window_name = nullptr;
		DWORD _style = WS_OVERLAPPEDWINDOW;
		int _x = 0, _y = 0, _w = 1, _h = 1;
		HWND _parent = nullptr;
		HMENU _menu = nullptr;
		LPVOID _param = nullptr;
		property(){}
#define PROPERTYDECL(name, type, membername) property& name(type t)&{membername = t;return *this;} property&& name(type t)&&{membername = t;return std::move(*this);}
		PROPERTYDECL(extended_style, DWORD, _exstyle)
		PROPERTYDECL(title, LPCTSTR, _window_name)
		PROPERTYDECL(style, DWORD, _style)
		PROPERTYDECL(x, int, _x)
		PROPERTYDECL(y, int, _y)
		PROPERTYDECL(w, int, _w)
		PROPERTYDECL(h, int, _h)
		PROPERTYDECL(parent_window_handle, HWND, _parent)
		PROPERTYDECL(menu_handle, HMENU, _menu)
		PROPERTYDECL(parameter, LPVOID, _param)
#undef  PROPERTYDECL
	};
	class class_{
		LPCTSTR name;
		HINSTANCE hinst;
		class_(LPCTSTR n, HINSTANCE hi)noexcept:name{n}, hinst{hi}{}
	public:
		class property{
			WNDCLASSEX wcx;
			using procedure_type = LRESULT (CALLBACK*)(HWND, UINT, WPARAM, LPARAM);
		public:
			property():wcx(){wcx.cbSize = sizeof(WNDCLASSEX); wcx.lpfnWndProc = window::proc_; wcx.cbWndExtra = sizeof(window*); wcx.hCursor = *resource::load_cursor(IDC_ARROW);}
	#define PROPERTYDECL(name, type, membername) property& name(type t)&{wcx.membername = t;return *this;} property&& name(type t)&&{wcx.membername = t;return std::move(*this);}
			PROPERTYDECL(style, UINT, style)
			PROPERTYDECL(icon_handle, HICON, hIcon)
			PROPERTYDECL(cursor_handle, HCURSOR, hCursor)
			PROPERTYDECL(background, HBRUSH, hbrBackground)
			PROPERTYDECL(class_name, LPCTSTR, lpszClassName)
			PROPERTYDECL(instance_handle, HINSTANCE, hInstance)
			PROPERTYDECL(procedure, procedure_type, lpfnWndProc)
	#undef  PROPERTYDECL
			operator const WNDCLASSEX&()const&{return wcx;}
			const WNDCLASSEX& get()const&{return wcx;}
		};
		static expected<class_, winapi_last_error> register_(const WNDCLASSEX& prop)noexcept{
			if(::RegisterClassEx(&prop) == 0)
				return make_unexpected(winapi_last_error{_T(__FUNCTION__)});
			return class_{prop.lpszClassName, prop.hInstance};
		}
		expected<void, winapi_last_error> unregister()noexcept{
			if(hinst == nullptr)
				return {};
			if(::UnregisterClass(name, hinst) == 0)
				return make_unexpected(winapi_last_error{_T(__FUNCTION__)});
			hinst = nullptr;
			return {};
		}
		class_(const WNDCLASSEX& prop):class_(*register_(prop)){}
		class_(const class_&) = delete;
		class_(class_&& other)noexcept:name{other.name}, hinst{other.hinst}{other.hinst = nullptr;}
		class_& operator=(const class_&) = delete;
		class_& operator=(class_&& other)noexcept{name = other.name; hinst = other.hinst; other.hinst = nullptr; return *this;}
		~class_(){if(hinst != nullptr)auto _ [[maybe_unused]] = unregister();}
		HINSTANCE get_hinst()const{return hinst;}
		LPCTSTR get_name()const{return name;}
		expected<window, winapi_last_error> create_window(const window::property& property)const{
			auto hwnd = ::CreateWindowEx(
				property._exstyle,
				get_name(),
				property._window_name,
				property._style,
				property._x,
				property._y,
				property._w,
				property._h,
				property._parent,
				property._menu,
				get_hinst(),
				property._param
			);
			if(hwnd == nullptr)
				return make_unexpected(winapi_last_error{_T(__FUNCTION__)});
			window w{hwnd};
			w.messenger[WM_DESTROY] = wm_destroy;
			return std::move(w);
		}
	};
	std::unordered_map<UINT, std::function<LRESULT(window&, WPARAM, LPARAM)>> messenger;
	_wxywh get_wxywh(){return _wxywh{*this};}
	void set_wxywh(const will::two_dim::xywh<int>& t){auto _ [[maybe_unused]] = _setrect(get(), t.xy.x, t.xy.y, t.wh.w, t.wh.h);}
	__declspec(property(get=get_wxywh, put=set_wxywh)) _wxywh xywh;
	_wxywh::_xy get_wxy(){return _wxywh::_xy{*this};}
	void set_wxy(const will::two_dim::xy<int>& t){xywh.xy = t;}
	__declspec(property(get=get_wxy, put=set_wxy)) _wxywh::_xy xy;
	_wxywh::_wh get_wwh(){return _wxywh::_wh{*this};}
	void set_wwh(const will::two_dim::wh<int>& t){xywh.wh = t;}
	__declspec(property(get=get_wwh, put=set_wwh)) _wxywh::_wh wh;
	int get_wx(){return xywh.xy.x;}
	void set_wx(int t){xywh.xy.x = t;}
	__declspec(property(get=get_wx, put=set_wx)) int x;
	int get_wy(){return xywh.xy.y;}
	void set_wy(int t){xywh.xy.y = t;}
	__declspec(property(get=get_wy, put=set_wy)) int y;
	int get_ww(){return xywh.wh.w;}
	void set_ww(int t){xywh.wh.w = t;}
	__declspec(property(get=get_ww, put=set_ww)) int w;
	int get_wh(){return xywh.wh.h;}
	void set_wh(int t){xywh.wh.h = t;}
	__declspec(property(get=get_wh, put=set_wh)) int h;
	_wxyxy get_wxyxy(){return _wxyxy{*this};}
	void set_wxyxy(const will::two_dim::xyxy<int>& t){auto _ [[maybe_unused]] = _setrect(get(), t._1.x, t._1.y, t._2.x - t._1.x, t._2.y - t._1.y);}
	__declspec(property(get=get_wxyxy, put=set_wxyxy)) _wxyxy xyxy;
	_cxywh get_cxywh(){return _cxywh{*this};}
	void set_cxywh(const will::two_dim::xywh<int>& t){auto _ [[maybe_unused]] = _setrect(get(), t.xy.x, t.xy.y, t.wh.w, t.wh.h);}
	__declspec(property(get=get_cxywh, put=set_cxywh)) _cxywh client_xywh;
	_cxywh::_xy get_cxy(){return _cxywh::_xy{*this};}
	void set_cxy(const will::two_dim::xy<int>& t){client_xywh.xy = t;}
	__declspec(property(get=get_cxy, put=set_cxy)) _cxywh::_xy client_xy;
	_cxywh::_wh get_cwh(){return _cxywh::_wh{*this};}
	void set_cwh(const will::two_dim::wh<int>& t){client_xywh.wh = t;}
	__declspec(property(get=get_cwh, put=set_cwh)) _cxywh::_wh client_wh;
	int get_cx(){return client_xywh.xy.x;}
	void set_cx(int t){client_xywh.xy.x = t;}
	__declspec(property(get=get_cx, put=set_cx)) int client_x;
	int get_cy(){return client_xywh.xy.y;}
	void set_cy(int t){client_xywh.xy.y = t;}
	__declspec(property(get=get_cy, put=set_cy)) int client_y;
	int get_cw(){return client_xywh.wh.w;}
	void set_cw(int t){client_xywh.wh.w = t;}
	__declspec(property(get=get_cw, put=set_cw)) int client_w;
	int get_ch(){return client_xywh.wh.h;}
	void set_ch(int t){client_xywh.wh.h = t;}
	__declspec(property(get=get_ch, put=set_ch)) int client_h;
	expected<two_dim::xywh<long>, winapi_last_error> get_window_rect()const noexcept{return _getwrect(get()).map([](const RECT& r){return will::two_dim::attribute<two_dim::xywh<long>>(r);});}
	template<typename T>
	expected<T, winapi_last_error> get_window_rect()const{return _getwrect(get()).map([](const RECT& r){return will::two_dim::attribute<T>(r);});}
	expected<two_dim::xywh<long>, winapi_last_error> get_client_rect()const noexcept{return _getcrect(get()).map([](const RECT& r){return will::two_dim::attribute<two_dim::xywh<long>>(r);});}
	template<typename T>
	expected<T, winapi_last_error> get_client_rect()const{return _getcrect(get()).map([](const RECT& r){return will::two_dim::attribute<T>(r);});}
	template<typename T>
	expected<void, winapi_last_error> set_window_rect(T&& r, UINT flags = SWP_NOZORDER){auto&& _ = will::two_dim::attribute<xywh<long>>(r);return _setrect(get(), _.x, _.y, _.w, _.h, flags);}
	expected<tstring, winapi_last_error> get_title()const{tstring str(GetWindowTextLength(get()), _T('\0'));if(str.size() == 0)return make_unexpected(winapi_last_error{_T(__FUNCTION__)});if(GetWindowText(get(), &str[0], static_cast<int>(str.size())) == 0)return make_unexpected(winapi_last_error{_T(__FUNCTION__)});return str;}
	expected<void, winapi_last_error> set_title(LPCTSTR str){return SetWindowText(get(), str) == 0 ? make_unexpected(winapi_last_error{_T(__FUNCTION__)}) : expected<void, winapi_last_error>{};}
	expected<void, winapi_last_error> set_title(const tstring& str){return set_title(str.c_str());}
	tstring _get_title_unsecured()const{tstring str(GetWindowTextLength(get()), _T('\0'));GetWindowText(get(), &str[0], static_cast<int>(str.size()));return str;}
	void _set_title_unsecured(LPCTSTR str){SetWindowText(get(), str);}
	void _set_title_unsecured(const tstring& str){_set_title_unsecured(str.c_str());}
	__declspec(property(get=_get_title_unsecured, put=_set_title_unsecured)) tstring title;
	expected<DWORD, winapi_last_error> get_extended_style()const{return get_long_ptr<DWORD>(GWL_EXSTYLE);}
	expected<DWORD, winapi_last_error> set_extended_style(DWORD ex_style){return set_long_ptr(GWL_EXSTYLE, ex_style);}
	DWORD _get_extended_style_unsecured()const{return static_cast<DWORD>(::GetWindowLongPtr(get(), GWL_EXSTYLE));}
	DWORD _set_extended_style_unsecured(DWORD ex_style)const{return static_cast<DWORD>(::SetWindowLongPtr(get(), GWL_EXSTYLE, ex_style));}
	__declspec(property(get=_get_extended_style_unsecured, put=_set_extended_style_unsecured)) DWORD extended_style;
	expected<DWORD, winapi_last_error> get_style()const{return get_long_ptr<DWORD>(GWL_STYLE);}
	expected<DWORD, winapi_last_error> set_style(DWORD style){return set_long_ptr(GWL_STYLE, style);}
	DWORD _get_style_unsecured()const{return static_cast<DWORD>(::GetWindowLongPtr(get(), GWL_STYLE));}
	DWORD _set_style_unsecured(DWORD style)const{return static_cast<DWORD>(::SetWindowLongPtr(get(), GWL_STYLE, style));}
	__declspec(property(get=_get_style_unsecured, put=_set_style_unsecured)) DWORD style;
	expected<HINSTANCE, winapi_last_error> get_hinstance()const{return get_long_ptr(GWLP_HINSTANCE).map([](LONG_PTR t){return reinterpret_cast<HINSTANCE>(t);});}
	expected<HINSTANCE, winapi_last_error> set_hinstance(HINSTANCE hinstance){return set_long_ptr(GWLP_HINSTANCE, reinterpret_cast<LONG_PTR>(hinstance)).map([](LONG_PTR t){return reinterpret_cast<HINSTANCE>(t);});}
	static expected<window, winapi_last_error> create(const class_& c, const property& prop){
		return c.create_window(prop);
	}
	window(const class_& cl, const property& prop):window(*create(cl, prop)){}
	~window(){
		if(hwnd != nullptr)
			::DestroyWindow(hwnd);
	}
	LRESULT procedure(UINT msg, WPARAM wp, LPARAM lp){
		const auto it = messenger.find(msg);
		return it != messenger.end() ? it->second(*this, wp, lp) : DefWindowProc(hwnd, msg, wp, lp);
	}
	class device_context_handle{
		const window& w_;
		HDC hdc;
	public:
		device_context_handle(const window& w):w_(w),hdc(::GetDC(w_.get())){}
		device_context_handle(const device_context_handle&) = delete;
		device_context_handle(device_context_handle&& other)noexcept:w_{other.w_}, hdc{other.hdc}{other.hdc = nullptr;}
		~device_context_handle(){if(hdc != nullptr)::ReleaseDC(w_.get(), hdc);}
		HDC get()const noexcept{return hdc;}
		explicit operator HDC()const noexcept{return hdc;}
	};
	HWND get()const noexcept{return hwnd;}
	HWND get_hwnd()const noexcept{return hwnd;}
	HWND get_window_handle()const noexcept{return hwnd;}
	device_context_handle get_dc(){return device_context_handle{*this};}
	device_context_handle get_device_context_handle()const{return device_context_handle{*this};}
	bool is_active()const noexcept{return ::GetActiveWindow() == hwnd;}
	void show(int cmd = SW_SHOW){
		::ShowWindow(hwnd, cmd);
		::UpdateWindow(hwnd);
	}
	void hide(){::ShowWindow(hwnd, SW_HIDE);}
	void show(bool enable){
		enable ? show() : hide();
	}
	class message_process_result{
		std::pair<int, bool> result;
	public:
		message_process_result() = default;
		message_process_result(const std::pair<int, bool>& r)noexcept:result(r){}
		message_process_result& operator=(const std::pair<int, bool>& r)noexcept{result = r; return *this;}
		explicit operator bool()const noexcept{return result.second;}
		int get()const noexcept{return result.first;}
		friend window;
	};
	bool has_message()const{
		MSG msg;
		return ::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE) != FALSE;
	}
	message_process_result process_message(){
		MSG msg;
		if([](BOOL t)->bool{return t == 0 || t == -1;}(::GetMessage(&msg, nullptr, 0, 0)))
			return std::make_pair(static_cast<int>(msg.wParam), false);
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		return std::make_pair(0, true);
	}
	message_process_result sleep_wait(std::chrono::milliseconds wait_time, int default_return_value){
		const auto start_point = std::chrono::high_resolution_clock().now();
		do{
			if(!has_message())
				continue;
			auto ret = process_message();
			if(!ret)
				return ret;
		}while(std::chrono::high_resolution_clock().now() - start_point < wait_time);
		return std::make_pair(default_return_value, true);
	}
	std::chrono::milliseconds rest_of_time(std::chrono::milliseconds elapsed, std::chrono::milliseconds limit){
		return elapsed < limit ? limit - elapsed : std::chrono::milliseconds{0};
	}
	message_process_result message_loop(const std::chrono::steady_clock::time_point& start, int frame_per_second){
		return sleep_wait(
			rest_of_time(
				std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock().now() - start),
				std::chrono::milliseconds{1000} / frame_per_second
			), 0);
	}
	explicit operator bool()const{return hwnd != nullptr;}
	explicit operator HWND()const{return hwnd;}
};
using window_class = window::class_;

class timer{
	HWND hwnd;
	UINT_PTR timer_id;
public:
	timer(HWND hwnd, UINT_PTR id, std::chrono::milliseconds elapse):hwnd(hwnd),timer_id(id){::SetTimer(hwnd,id,static_cast<UINT>(elapse.count()),nullptr);}
	~timer(){::KillTimer(hwnd, timer_id);}
};

two_dim::xy<int> get_dpi(){
	const auto hdc = ::GetDC(nullptr);
	const auto dpi = two_dim::xy<int>{::GetDeviceCaps(hdc, LOGPIXELSX), ::GetDeviceCaps(hdc, LOGPIXELSY)};
	::ReleaseDC(nullptr, hdc);
	return dpi;
}

namespace two_dim {

template<>
struct attribute_traits<window::_wxywh::_xy>{
	using tag_type = tag::point;
	using element_type = int;
	static element_type x(const window::_wxywh::_xy& xy)noexcept{return xy.x;}
	static element_type y(const window::_wxywh::_xy& xy)noexcept{return xy.y;}
};

template<>
struct attribute_traits<window::_wxywh::_wh>{
	using tag_type = tag::size;
	using element_type = int;
	static element_type w(const window::_wxywh::_wh& wh)noexcept{return wh.w;}
	static element_type h(const window::_wxywh::_wh& wh)noexcept{return wh.h;}
};

template<>
struct attribute_traits<window::_cxywh::_xy>{
	using tag_type = tag::point;
	using element_type = int;
	static element_type x(const window::_cxywh::_xy& xy)noexcept{return xy.x;}
	static element_type y(const window::_cxywh::_xy& xy)noexcept{return xy.y;}
};

template<>
struct attribute_traits<window::_cxywh::_wh>{
	using tag_type = tag::size;
	using element_type = int;
	static element_type w(const window::_cxywh::_wh& wh)noexcept{return wh.w;}
	static element_type h(const window::_cxywh::_wh& wh)noexcept{return wh.h;}
};

template<>
struct attribute_traits<window::_wxyxy::_1t>{
	using tag_type = tag::point;
	using element_type = int;
	static element_type x(const window::_wxyxy::_1t& xy)noexcept{return xy.x;}
	static element_type y(const window::_wxyxy::_1t& xy)noexcept{return xy.y;}
};

template<>
struct attribute_traits<window::_wxyxy::_2t>{
	using tag_type = tag::point;
	using element_type = int;
	static element_type x(const window::_wxyxy::_2t& xy)noexcept{return xy.x;}
	static element_type y(const window::_wxyxy::_2t& xy)noexcept{return xy.y;}
};

template<>
struct attribute_traits<window::_wxyxy>{
	using tag_type = tag::point_pair;
	using element_type = int;
	static xy<int> _1(const window::_wxyxy& xyxy)noexcept{return xyxy._1;}
	static xy<int> _2(const window::_wxyxy& xyxy)noexcept{return xyxy._2;}
	static int x1(const window::_wxyxy& xyxy)noexcept{return xyxy._1.x;}
	static int y1(const window::_wxyxy& xyxy)noexcept{return xyxy._1.y;}
	static int x2(const window::_wxyxy& xyxy)noexcept{return xyxy._2.x;}
	static int y2(const window::_wxyxy& xyxy)noexcept{return xyxy._2.y;}
};

template<>
struct attribute_traits<window::_wxywh>{
	using tag_type = tag::point_and_size;
	using point_element_type = int;
	using size_element_type = int;
	static xy<int> xy(const window::_wxywh& xywh)noexcept{return xywh.xy;}
	static wh<int> wh(const window::_wxywh& xywh)noexcept{return xywh.wh;}
	static int x(const window::_wxywh& xywh)noexcept{return xywh.xy.x;}
	static int y(const window::_wxywh& xywh)noexcept{return xywh.xy.y;}
	static int w(const window::_wxywh& xywh)noexcept{return xywh.wh.w;}
	static int h(const window::_wxywh& xywh)noexcept{return xywh.wh.h;}
};

template<>
struct attribute_traits<window::_cxywh>{
	using tag_type = tag::point_and_size;
	using point_element_type = int;
	using size_element_type = int;
	static xy<int> xy(const window::_cxywh& xywh)noexcept{return xywh.xy;}
	static wh<int> wh(const window::_cxywh& xywh)noexcept{return xywh.wh;}
	static int x(const window::_cxywh& xywh)noexcept{return xywh.xy.x;}
	static int y(const window::_cxywh& xywh)noexcept{return xywh.xy.y;}
	static int w(const window::_cxywh& xywh)noexcept{return xywh.wh.w;}
	static int h(const window::_cxywh& xywh)noexcept{return xywh.wh.h;}
};

}

}
