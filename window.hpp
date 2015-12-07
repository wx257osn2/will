#pragma once
#include"_windows.hpp"
#include"_2dim.hpp"
#include<unordered_map>
#include<functional>
#include<chrono>
namespace will{
template<typename T, typename F>
inline std::pair<T, bool> sleep_wait(std::chrono::milliseconds wait_time, T&& default_return_value, F&& f, std::chrono::milliseconds sleep_time_per_loop = std::chrono::milliseconds{1}){
	const auto start_point = std::chrono::high_resolution_clock().now();
	do{
		auto ret = f();
		if(ret.second == false)
			return ret;
		if(wait_time.count() > 0)
			sleep(sleep_time_per_loop);
	}while(std::chrono::high_resolution_clock().now() - start_point < wait_time);
	return std::make_pair(std::forward<T>(default_return_value), true);
}
inline std::chrono::milliseconds rest_of_time(std::chrono::milliseconds elapsed, std::chrono::milliseconds limit){
	return elapsed < limit ? limit - elapsed : std::chrono::milliseconds{0};
}

class window{
	HWND hwnd;
	ATOM atom;
	HINSTANCE hinst;
	static LRESULT CALLBACK proc_(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
		const auto it = reinterpret_cast<window*>(GetWindowLongPtr(hwnd, 0));
		return it ? it->procedure(msg, wp, lp) : DefWindowProc(hwnd, msg, wp, lp);
	}
	static RECT _getwrect(HWND hwnd){RECT r;::GetWindowRect(hwnd, &r);return r;}
	static RECT _getcrect(HWND hwnd){RECT r;::GetClientRect(hwnd, &r);return r;}
	static BOOL _setrect(HWND hwnd, int x, int y, int w, int h, UINT flags = SWP_NOZORDER){return ::SetWindowPos(hwnd, nullptr, x, y, w, h, flags);}
	struct _x{
		window& w;
		operator int()const{return _getwrect(w.get_hwnd()).left;}
		_x& operator=(int t){_setrect(w.get_hwnd(), t, w.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);}
	};
	struct _y{
		window& w;
		operator int()const{return _getwrect(w.get_hwnd()).top;}
		_x& operator=(int t){_setrect(w.get_hwnd(), w.x, t, 0, 0, SWP_NOZORDER | SWP_NOSIZE);}
	};
	struct _w{
		window& w;
		operator int()const{const auto r = _getwrect(w.get_hwnd()); return r.right - r.left;}
		_x& operator=(int t){_setrect(w.get_hwnd(), 0, 0, t, w.h, SWP_NOZORDER | SWP_NOMOVE);}
	};
	struct _h{
		window& w;
		operator int()const{const auto r = _getwrect(w.get_hwnd()); return r.bottom - r.top;}
		_x& operator=(int t){_setrect(w.get_hwnd(), 0, 0, w.w, t, SWP_NOZORDER | SWP_NOMOVE);}
	};
public:
	class window_class_property{
		WNDCLASSEX wcx;
	public:
		window_class_property():wcx(){wcx.cbSize = sizeof(WNDCLASSEX); wcx.lpfnWndProc = window::proc_; wcx.cbWndExtra = sizeof(window*); wcx.hCursor = reinterpret_cast<HCURSOR>(LoadImage(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));}
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
	_x x{*this};
	_y y{*this};
	_w w{*this};
	_h h{*this};
	window(WNDCLASSEX&& wcx, window_property&& property):hwnd(nullptr), hinst(wcx.hInstance){create(std::move(wcx), std::move(property));}
	window(window_class_property&& wcp, window_property&& property):window(std::move(wcp.get()), std::move(property)){}
	window():hwnd(nullptr){}
	window(const window&) = delete;
	window(window&&) = default;
	~window(){
		if(hwnd != nullptr){
			DestroyWindow(hwnd);
			UnregisterClass(reinterpret_cast<LPCTSTR>(0ull | atom), hinst);
		}
	}
	HWND create(WNDCLASSEX&& wcx, window_property&& property){
		if((atom = RegisterClassEx(&wcx)) == 0)return nullptr;
		messenger[WM_DESTROY] = [](window&, WPARAM, LPARAM)->LRESULT{::PostQuitMessage(0);return 0;};
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
		return it != messenger.end() ? it->second(*this,wp,lp) : DefWindowProc(this->hwnd, msg, wp, lp);
	}
	HWND get_hwnd()const{return hwnd;}
	will::two_dim::xywh<int> get_window_rect()const{const auto r = _getwrect(get_hwnd()); return {{r.left, r.top}, {r.right - r.left, r.bottom - r.top}};}
	will::two_dim::xywh<int> get_client_rect()const{const auto r = _getcrect(get_hwnd()); return {{r.left, r.top}, {r.right - r.left, r.bottom - r.top}};}
	bool is_active()const{return ::GetActiveWindow() == hwnd;}
	void show(){
		::ShowWindow(hwnd, SW_SHOW);
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
		message_process_result(const std::pair<int, bool>& r):result(r){}
		message_process_result& operator=(const std::pair<int, bool>& r){result = r; return *this;}
		explicit operator bool()const{return result.second;}
		int get()const{return result.first;}
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
	message_process_result check_and_process_message(){
	  if(has_message())
	    return process_message();
	  return std::make_pair(0, true);
	}
	message_process_result message_loop(const std::chrono::steady_clock::time_point& start, int frame_per_second){
		return sleep_wait(
			will::rest_of_time(
				std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - start),
				std::chrono::milliseconds{1000} / frame_per_second
			), 0, [&]{return check_and_process_message().result;});
	}
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
