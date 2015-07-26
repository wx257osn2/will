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
	HCURSOR cursor = nullptr;
public:
	_x x;
	_y y;
	bool show(){
	  if(cursor)
	    SetCursor(cursor);
	  return cursor;
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