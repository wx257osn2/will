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
  static expected<SIZE, winapi_last_error> get_client_size(HWND hwnd){RECT rc;if(::GetClientRect(hwnd, &rc) != 0)return SIZE{rc.right - rc.left, rc.bottom - rc.top};return make_unexpected<winapi_last_error>(_T(__FUNCTION__));}
  layered_window(HWND hwnd, const SIZE& s) : hwnd(hwnd), src_pos(), win_pos(), size(s), blend({0, 0, 255, AC_SRC_ALPHA}),
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
public:
  static expected<layered_window, winapi_last_error> create(HWND hwnd){
    return get_client_size(hwnd).map([&](SIZE&& s){return layered_window{hwnd, s};});
  }
  template<typename Hwnd>
  static expected<layered_window, winapi_last_error> create(Hwnd&& hwnd){return create(std::forward<Hwnd>(hwnd).get());}
  explicit layered_window(HWND hwnd) : layered_window(+create(hwnd)){}
  template<typename Hwnd>
  explicit layered_window(Hwnd&& hwnd) : layered_window(std::forward<Hwnd>(hwnd).get()){}
  layered_window(layered_window&& other)noexcept:hwnd(other.hwnd), src_pos(other.src_pos), win_pos(other.win_pos), size(other.size), blend(other.blend),
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
  expected<void, winapi_last_error> update(HDC source){
    info.hdcSrc = source;
    if(UpdateLayeredWindowIndirect(hwnd, &info) != 0)
      return {};
    return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
  }
};
}