#pragma once
namespace will{
namespace two_dim{
namespace detail{
	template<typename T, typename U>struct attribute;
}
	template<typename T, typename U>inline T attribute(const U& u){return detail::attribute<T, U>::impl(u);}
	template<typename T>
	struct xy{T x, y;};
	template<typename T>
	struct wh{T w, h;};
	template<typename>struct xywh;
	template<typename T>
	struct xyxy{xy<T> _1, _2;operator xywh<T>()const;template<typename U>operator xyxy<U>()const{return {{static_cast<U>(_1.x), static_cast<U>(_1.y)}, {static_cast<U>(_2.x), static_cast<U>(_2.y)}};}template<typename U>xyxy<U> cast()const{return static_cast<xyxy<U>>(*this);}template<typename U>U attribute()const{return will::two_dim::attribute<U>(*this);}};
	template<typename T>
	struct xywh{xy<T> xy; wh<T> wh;operator xyxy<T>()const{return {{xy.x, xy.y}, {xy.x+wh.w, xy.y+wh.h}};}template<typename U>operator xywh<U>()const{return {{static_cast<U>(xy.x), static_cast<U>(xy.y)}, {static_cast<U>(wh.w), static_cast<U>(wh.h)}};}template<typename U>xywh<U> cast()const{return static_cast<xywh<U>>(*this);}template<typename U>U attribute()const{return will::two_dim::attribute<U>(*this);}};
	template<typename T>
	inline xyxy<T>::operator xywh<T>()const{return {{_1.x, _1.y}, {_2.x-_1.x, _2.y-_1.y}};}
}
}