#pragma once
#include"_windows.hpp"
#include<unordered_map>
#include<functional>
#include<chrono>
namespace will{
class window{
	HWND hwnd;
	ATOM atom;
	HINSTANCE hinst;
	static LRESULT CALLBACK proc_(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
		const auto it = reinterpret_cast<window*>(GetWindowLongPtr(hwnd, 0));
		return it ? it->procedure(msg, wp, lp) : DefWindowProc(hwnd, msg, wp, lp);
	}
public:
	class window_class_property{
		WNDCLASSEX wcx;
	public:
		window_class_property():wcx(){wcx.cbSize = sizeof WNDCLASSEX; wcx.lpfnWndProc = window::proc_; wcx.cbWndExtra = sizeof(window*); wcx.hCursor = (HCURSOR)LoadImage(nullptr,MAKEINTRESOURCE(IDC_ARROW),IMAGE_CURSOR,0,0,LR_DEFAULTSIZE | LR_SHARED);}
#define PROPERTYDECL(name, type, membername) window_class_property& name(type t){wcx.membername = t;return *this;}
		PROPERTYDECL(style, UINT, style)
		PROPERTYDECL(icon_handle, HICON, hIcon)
		PROPERTYDECL(cursor_handle, HCURSOR, hCursor)
		PROPERTYDECL(background, HBRUSH, hbrBackground)
		PROPERTYDECL(class_name, LPCTSTR, lpszClassName)
		PROPERTYDECL(instance_handle, HINSTANCE, hInstance)
#undef  PROPERTYDECL
		operator WNDCLASSEX&(){return wcx;}
		WNDCLASSEX& get(){return wcx;}
	};
	struct window_property{
		DWORD _exstyle = 0;
		LPCTSTR _window_name;
		DWORD _style = WS_OVERLAPPEDWINDOW;
		int _x, _y, _w, _h;
		HWND _parent = nullptr;
		HMENU _menu = nullptr;
		LPVOID _param = nullptr;
		window_property(){}
#define PROPERTYDECL(name, type, membername) window_property& name(type t){membername = t;return *this;}
		PROPERTYDECL(extra_style, DWORD, _exstyle)
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
	std::unordered_map<UINT, std::function<LRESULT(window&, WPARAM, LPARAM)>> messenger;

	window(WNDCLASSEX&& wcx, window_property&& property):hwnd(nullptr), hinst(wcx.hInstance){create(std::move(wcx), std::move(property));}
	window(window_class_property&& wcp, window_property&& property):window(std::move(wcp.get()), std::move(property)){}
	window():hwnd(nullptr){}
	window(const window&) = delete;
	window(window&&) = default;
	~window(){
		if(hwnd != nullptr){
			DestroyWindow(hwnd);
			UnregisterClass(reinterpret_cast<LPCSTR>(0ull | atom), hinst);
		}
	}
	HWND create(WNDCLASSEX&& wcx, window_property&& property){
		if((atom = RegisterClassEx(&wcx)) == 0)return nullptr;
		messenger[WM_DESTROY] = [](auto&&, auto&&, auto&&){::PostQuitMessage(0);return 0;};
		SetWindowLongPtr(hwnd = CreateWindowEx(
			property._exstyle,
			wcx.lpszClassName,
			property._window_name,
			property._style,
			property._x,
			property._y,
			property._w,
			property._h,
			property._parent,
			property._menu,
			wcx.hInstance,
			property._param
		), 0, reinterpret_cast<LONG_PTR>(this));
		return hwnd;
	}
	LRESULT procedure(UINT msg, WPARAM wp, LPARAM lp){
		const auto it = messenger.find(msg);
		return it != messenger.end() ? it->second(*this,wp,lp) : DefWindowProc(this->hwnd, msg, wp, lp );
	}
	HWND get_hwnd()const{return hwnd;}
	bool is_active()const{return ::GetActiveWindow() == hwnd;}
	explicit operator bool()const{return hwnd != nullptr;}
	explicit operator HWND()const{return hwnd;} 
};
class timer{
	HWND hwnd;
	UINT_PTR timer_id;
public:
	timer(HWND hwnd, UINT_PTR id, std::chrono::milliseconds elapse):hwnd(hwnd),timer_id(id){::SetTimer(hwnd,id,static_cast<UINT>(elapse.count()),nullptr);}
	~timer(){::KillTimer(hwnd, timer_id);}
};
}