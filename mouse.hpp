//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_windows.hpp"
#include"_2dim.hpp"
namespace will{
class mouse{
	static expected<POINT, winapi_last_error> _getpos(){POINT p;if(GetCursorPos(&p) != 0)return p;return make_unexpected<winapi_last_error>(_T(__FUNCTION__));}
	static expected<void, winapi_last_error> _setpos(int x, int y){if(SetCursorPos(x, y) != 0)return {};return make_unexpected<winapi_last_error>(_T(__FUNCTION__));}
	HCURSOR cursor = nullptr;
public:
	expected<two_dim::xy<int>, winapi_last_error> get_pos()const{return _getpos().map([](POINT&& p){return two_dim::xy<int>{p.x, p.y};});}
	expected<void, winapi_last_error> set_pos(int x, int y){return _setpos(x, y);}
	expected<void, winapi_last_error> set_pos(const two_dim::xy<int>& xy){return _setpos(xy.x, xy.y);}
	int get_x()const{return get_pos()--->x;}
	void set_x(int x){+set_pos(x, get_pos()--->y);}
	__declspec(property(get=get_x, put=set_x)) int x;
	int get_y()const{return get_pos()--->y;}
	void set_y(int y){+set_pos(get_pos()--->x, y);}
	__declspec(property(get=get_y, put=set_y)) int y;
	two_dim::xy<int> get_xy()const{return +get_pos();}
	void set_xy(const two_dim::xy<int>& xy){+set_pos(x, y);}
	__declspec(property(get=get_xy, put=set_xy)) two_dim::xy<int> xy;
	bool show(){
	  if(cursor)
	    SetCursor(cursor);
	  return cursor != nullptr;
	}
	bool hide(){
	  if(cursor)
	    SetCursor(nullptr);
	  else
	    cursor = SetCursor(nullptr);
	  return true;
	}
	bool show(bool enable){
	  return enable ? show() : hide();
	}
};
}