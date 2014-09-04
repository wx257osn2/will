#pragma once
namespace will{
namespace two_dim{
	template<typename T>
	struct xy{T x, y;};
	template<typename T>
	struct wh{T w, h;};
	template<typename>struct xywh;
	template<typename T>
	struct xyxy{xy<T> _1, _2;operator xywh<T>()const;};
	template<typename T>
	struct xywh{xy<T> p; wh<T> r;operator xyxy<T>()const{return {{p.x, p.y}, {p.x+r.w, p.y+r.h}};}};
	template<typename T>
	inline xyxy<T>::operator xywh<T>()const{return {{_1.x, _1.y}, {_2.x-_1.x, _2.y-_1.y}};}
}
}