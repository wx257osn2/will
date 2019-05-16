//Copyright (C) 2019 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include<optional>
#include"_windows.hpp"
#include"_2dim.hpp"

namespace will{

struct input_data : ::INPUT{
	enum down_up_flag : DWORD{
		down = 0,
		up = KEYEVENTF_KEYUP
	};
	enum forward_backward_flag : LONG{
		forward = 120,
		backward = -120
	};
	enum right_left_flag : LONG {
		right = 120,
		left = -120
	};
	struct mouse : ::MOUSEINPUT{
		friend mouse operator|(mouse lhs, const mouse& rhs){
			lhs.dx += rhs.dx;
			lhs.dy += rhs.dy;
			lhs.dwFlags |= rhs.dwFlags;
			return lhs;
		}
	};
	using keyboard = ::KEYBDINPUT;
	using hardware = ::HARDWAREINPUT;
	input_data(const mouse& mi) : ::INPUT{INPUT_MOUSE, {mi}}{}
	input_data(const keyboard& ki) : ::INPUT{INPUT_KEYBOARD}{this->ki = ki; }
	input_data(const hardware& hi) : ::INPUT{INPUT_HARDWARE}{this->hi = hi; }
	static mouse mouse_move(will::two_dim::xy<::LONG> pos, will::two_dim::wh<::LONG> resolution_of_primary_monitor = {::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN)}, bool silent = false, ::DWORD time = 0, ::ULONG_PTR extra_info = 0) {
		return mouse{static_cast<::LONG>(pos.x*(65535.f/resolution_of_primary_monitor.w)), static_cast<::LONG>(pos.y*(65535.f/resolution_of_primary_monitor.h)), 0, MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | (silent ? MOUSEEVENTF_MOVE_NOCOALESCE : 0u), time, extra_info};
	}
	static mouse mouse_move(will::two_dim::wh<::LONG> delta, bool silent = false, ::DWORD time = 0, ::ULONG_PTR extra_info = 0){
		return mouse{delta.w, delta.h, 0, MOUSEEVENTF_MOVE | (silent ? MOUSEEVENTF_MOVE_NOCOALESCE : 0u), time, extra_info};
	}
	static mouse vertical_wheel(forward_backward_flag f_or_b, ::DWORD time = 0, ::ULONG_PTR extra_info = 0){
		return mouse{0, 0, static_cast<DWORD>(f_or_b), MOUSEEVENTF_WHEEL, time, extra_info};
	}
	static mouse horizontal_wheel(right_left_flag r_or_l, ::DWORD time = 0, ::ULONG_PTR extra_info = 0){
		return mouse{0, 0, static_cast<DWORD>(r_or_l), MOUSEEVENTF_HWHEEL, time, extra_info};
	}
	static mouse mouse_l_button(down_up_flag flag, ::DWORD time = 0, ::ULONG_PTR extra_info = 0){
		return mouse{0, 0, 0, static_cast<::DWORD>(flag == KEYEVENTF_KEYUP ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_LEFTDOWN), time, extra_info};
	}
	static mouse mouse_r_button(down_up_flag flag, ::DWORD time = 0, ::ULONG_PTR extra_info = 0){
		return mouse{0, 0, 0, static_cast<::DWORD>(flag == KEYEVENTF_KEYUP ? MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_RIGHTDOWN), time, extra_info};
	}
	static mouse mouse_middle_button(down_up_flag flag, ::DWORD time = 0, ::ULONG_PTR extra_info = 0){
		return mouse{0, 0, 0, static_cast<::DWORD>(flag == KEYEVENTF_KEYUP ? MOUSEEVENTF_MIDDLEUP : MOUSEEVENTF_MIDDLEDOWN), time, extra_info};
	}
	static mouse mouse_x_button(DWORD xbutton, down_up_flag flag, ::DWORD time = 0, ::ULONG_PTR extra_info = 0){
		return mouse{0, 0, xbutton, static_cast<::DWORD>(flag == KEYEVENTF_KEYUP ? MOUSEEVENTF_XUP : MOUSEEVENTF_XDOWN), time, extra_info};
	}
	static keyboard virtual_key(::WORD virtual_key_code, down_up_flag flag = down, ::DWORD time = 0, ::ULONG_PTR extra_info = 0){
		return keyboard{virtual_key_code, 0, is_extended_key(virtual_key_code) | flag, time, extra_info};
	}
	static keyboard scan_code(::WORD scan_code, down_up_flag flag = down, ::DWORD time = 0, ::ULONG_PTR extra_info = 0){
		return keyboard{0, scan_code, KEYEVENTF_SCANCODE | flag, time, extra_info};
	}
	static keyboard unicode(wchar_t ch, down_up_flag flag = down, ::DWORD time = 0, ::ULONG_PTR extra_info = 0){
		return keyboard{0, ch, KEYEVENTF_UNICODE | flag, time, extra_info};
	}
private:
	static DWORD is_extended_key(::WORD virtual_key_code){
		switch(virtual_key_code){
		case VK_CANCEL:
		case VK_PRIOR:
		case VK_NEXT:
		case VK_END:
		case VK_HOME:
		case VK_LEFT:
		case VK_UP:
		case VK_RIGHT:
		case VK_DOWN:
		case VK_SNAPSHOT:
		case VK_INSERT:
		case VK_DELETE:
		case VK_LWIN:
		case VK_RWIN:
		case VK_APPS:
		case VK_SLEEP:
		case VK_DIVIDE:
		case VK_NUMLOCK:
			return KEYEVENTF_EXTENDEDKEY;
		default:
			return 0;
		}
	}
};

inline expected<::UINT, winapi_last_error> send_input(std::vector<input_data> inputs){
	::UINT ret = ::SendInput(static_cast<::UINT>(inputs.size()), inputs.data(), sizeof(INPUT));
	if(ret == 0u)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return ret;
}

inline expected<::UINT, winapi_last_error> send_input(const input_data& input){
	::UINT ret = ::SendInput(1u, &const_cast<input_data&>(input), sizeof(INPUT));
	if(ret == 0u)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	return ret;
}

}
