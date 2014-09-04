#pragma once
#include"_windows.hpp"
#include<shellapi.h>
namespace will{
class notify_icon{
	NOTIFYICONDATA nid;
public:
	notify_icon(NOTIFYICONDATA n):nid(std::move(n)){Shell_NotifyIcon(NIM_ADD, &nid);}
	~notify_icon(){Shell_NotifyIcon(NIM_DELETE, &nid);}
};
}