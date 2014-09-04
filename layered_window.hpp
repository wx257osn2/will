#pragma once
#include"_windows.hpp"
namespace will{
class layered_window{
  HWND hwnd;
  POINT src_pos;
  POINT win_pos;
  SIZE size;
  BLENDFUNCTION blend;
  UPDATELAYEREDWINDOWINFO info;
public:
  layered_window(HWND hwnd) : hwnd(hwnd), src_pos(), win_pos(), size([hwnd]{RECT rc;::GetClientRect(hwnd, &rc);return SIZE{rc.right - rc.left, rc.bottom - rc.top};}()), blend({0, 0, 255, AC_SRC_ALPHA}),
    info({
      sizeof(UPDATELAYEREDWINDOWINFO),
      nullptr,
      &win_pos,
      &size,
      nullptr,
      &src_pos,
      0,
      &blend,
      ULW_ALPHA,
      nullptr
    }){}
  void update(HDC source){
    info.hdcSrc = source;
    UpdateLayeredWindowIndirect(hwnd, &info);
  }
};
}