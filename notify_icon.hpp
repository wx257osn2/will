#pragma once
#include"_windows.hpp"
#include<shellapi.h>
namespace will{
class notify_icon{
	NOTIFYICONDATA nid;
public:
	notify_icon(const NOTIFYICONDATA& n):nid(n){Shell_NotifyIcon(NIM_ADD, &nid); Shell_NotifyIcon(NIM_SETVERSION, &nid);}
	notify_icon(NOTIFYICONDATA&& n):nid(std::move(n)){Shell_NotifyIcon(NIM_ADD, &nid); Shell_NotifyIcon(NIM_SETVERSION, &nid);}
	BOOL operator()(HICON icon){nid.hIcon = icon; return Shell_NotifyIcon(NIM_MODIFY, &nid);}
	BOOL operator()(const NOTIFYICONDATA& data){nid = data; return Shell_NotifyIcon(NIM_MODIFY, &nid);}
	BOOL operator()(NOTIFYICONDATA&& data){nid = std::move(data); return Shell_NotifyIcon(NIM_MODIFY, &nid);}
	BOOL release_focus()noexcept{return Shell_NotifyIcon(NIM_SETFOCUS, &nid);}
	~notify_icon(){Shell_NotifyIcon(NIM_DELETE, &nid);}
	class data{
		NOTIFYICONDATA nid;
	public:
		data(HWND hwnd):nid(){nid.cbSize = sizeof(NOTIFYICONDATA); nid.hWnd = hwnd; nid.uVersion = NOTIFYICON_VERSION_4;}
		data(const NOTIFYICONDATA& data):nid(data){}
		data(NOTIFYICONDATA&& data):nid(std::move(data)){}
		data(const data&) = default;
		data(data&&) = default;
		~data() = default;
#define PROPERTYDECL(name, type, membername, flag) data& name(type t){nid.membername = t; nid.uFlags flag; return *this;}
		PROPERTYDECL(id, UINT, uID, &= (~NIF_GUID))
		PROPERTYDECL(id, const GUID&, guidItem, |= NIF_GUID)
		PROPERTYDECL(callback_message, UINT, uCallbackMessage, |= NIF_MESSAGE)
		PROPERTYDECL(icon, HICON, hIcon, |= NIF_ICON)
		data& tooltip(const TCHAR* t, bool standard_tooltip = true){tcscpy(nid.szTip, t); if(standard_tooltip)nid.uFlags |= NIF_SHOWTIP; else nid.uFlags &= (~NIF_SHOWTIP); nid.uFlags |= NIF_TIP; return *this;}
		data& hidden(bool flag){if(flag)nid.dwState |= NIS_HIDDEN; else nid.dwState &= (~NIS_HIDDEN); nid.dwStateMask |= NIS_HIDDEN; nid.uFlags |= NIF_STATE; return *this;}
		data& shared(bool flag){if(flag)nid.dwState |= NIS_SHAREDICON; else nid.dwState &= (~NIS_SHAREDICON); nid.dwStateMask |= NIS_SHAREDICON; nid.uFlags |= NIF_STATE; return *this;}
		data& balloon(const TCHAR* title, const TCHAR* info, DWORD flags, bool real_time = false){if(title)tcscpy(nid.szInfoTitle, title); if(info)tcscpy(nid.szInfo, info); nid.dwInfoFlags = flags; if(real_time)nid.uFlags |= NIF_REALTIME; else nid.uFlags &= (~NIF_REALTIME); nid.uFlags |= NIF_INFO; return *this;}
		data& balloon_icon(HICON t){nid.hBalloonIcon = t; return *this;}
#undef  PROPERTYDECL
		operator NOTIFYICONDATA&()noexcept{return nid;}
		NOTIFYICONDATA& get()noexcept{return nid;}
	}get()const noexcept{return data{nid};};
};
}