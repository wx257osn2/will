#pragma once
#include"_windows.hpp"
namespace will{
class mouse{
	static POINT _getpos(){POINT p;GetCursorPos(&p);return p;}
	struct _x{
		operator int()const{return _getpos().x;}
		_x& operator=(int t){SetCursorPos(t, _getpos().y);return *this;}
	};
	struct _y{
		operator int()const{return _getpos().y;}
		_y& operator=(int t){SetCursorPos(_getpos().x, t);return *this;}
	};
	struct _xy{
		operator two_dim::xy<int>()const{auto pos = _getpos(); return {pos.x, pos.y};}
		_xy& operator=(const two_dim::xy<int>& t){SetCursorPos(t.x, t.y);return *this;}
	};
	HCURSOR cursor = nullptr;
public:
	_x x;
	_y y;
	_xy xy;
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