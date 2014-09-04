#pragma once
#include<dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
namespace will{
class blur_behind{
	DWM_BLURBEHIND bb;
	blur_behind& _enable(bool flag){bb.fEnable = flag; bb.dwFlags |= DWM_BB_ENABLE; return *this;}
public:
	blur_behind():bb(){};
	blur_behind(bool flag):bb(){_enable(flag);}
	blur_behind& enable(){return _enable(true);}
	blur_behind& disable(){return _enable(false);}
	blur_behind& region(HRGN rgn){bb.hRgnBlur = rgn; bb.dwFlags |= DWM_BB_BLURREGION; return *this;}
	template<typename Region>
	blur_behind& region(Region&& rgn){return region(rgn.cget());}
	blur_behind& transition_on_maximized(bool f){bb.fTransitionOnMaximized = f; bb.dwFlags |= DWM_BB_TRANSITIONONMAXIMIZED; return *this;}
	HRESULT operator()(HWND hwnd)const{return ::DwmEnableBlurBehindWindow(hwnd, &bb);}
	HRESULT operator()(const window& w)const{return (*this)(w.get_hwnd());}
};
}