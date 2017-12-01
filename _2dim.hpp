//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
namespace will{
namespace two_dim{
namespace detail{
	template<typename T, typename U>struct attribute;
}
	template<typename T, typename U>inline auto attribute(const U& u)noexcept(noexcept(detail::attribute<std::decay_t<T>, std::decay_t<U>>::impl(u))){return detail::attribute<std::decay_t<T>, std::decay_t<U>>::impl(u);}
	template<typename T>
	struct xy{T x, y;template<typename U>constexpr operator xy<U>()const noexcept{return {static_cast<U>(x), static_cast<U>(y)};}template<typename U>constexpr xy<U> cast()const noexcept{return static_cast<xy<U>>(*this);}template<typename U>auto attribute()const noexcept(noexcept(will::two_dim::attribute<U>(*this))){return will::two_dim::attribute<U>(*this);}};
	template<typename T>
	struct wh{T w, h;template<typename U>constexpr operator wh<U>()const noexcept{return {static_cast<U>(w), static_cast<U>(h)};}template<typename U>constexpr wh<U> cast()const noexcept{return static_cast<wh<U>>(*this);}template<typename U>auto attribute()const noexcept(noexcept(will::two_dim::attribute<U>(*this))){return will::two_dim::attribute<U>(*this);}};
	template<typename>struct xywh;
	template<typename T>
	struct xyxy{xy<T> _1, _2;constexpr operator xywh<T>()const noexcept;template<typename U>constexpr operator xyxy<U>()const noexcept{return {{static_cast<U>(_1.x), static_cast<U>(_1.y)}, {static_cast<U>(_2.x), static_cast<U>(_2.y)}};}template<typename U>constexpr xyxy<U> cast()const noexcept{return static_cast<xyxy<U>>(*this);}template<typename U>auto attribute()const noexcept{return will::two_dim::attribute<U>(*this);}};
	template<typename T>
	struct xywh{xy<T> xy; wh<T> wh;constexpr operator xyxy<T>()const noexcept{return {{xy.x, xy.y}, {xy.x+wh.w, xy.y+wh.h}};}template<typename U>constexpr operator xywh<U>()const noexcept{return {{static_cast<U>(xy.x), static_cast<U>(xy.y)}, {static_cast<U>(wh.w), static_cast<U>(wh.h)}};}template<typename U>constexpr xywh<U> cast()const noexcept{return static_cast<xywh<U>>(*this);}template<typename U>auto attribute()const noexcept(noexcept(will::two_dim::attribute<U>(*this))){return will::two_dim::attribute<U>(*this);}};
	template<typename T>
	constexpr xyxy<T>::operator xywh<T>()const noexcept{return {{_1.x, _1.y}, {_2.x-_1.x, _2.y-_1.y}};}

	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xy<T>& operator*=(xy<T>& lhs, U&& rhs)noexcept{lhs.x *= rhs; lhs.y *= rhs; return lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xy<T> operator*(xy<T> lhs, U&& rhs)noexcept{return lhs *= rhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xy<T> operator*(U&& lhs, xy<T> rhs)noexcept{return rhs *= lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xy<T>& operator/=(xy<T>& lhs, U&& rhs)noexcept{lhs.x /= rhs; lhs.y /= rhs; return lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xy<T> operator/(xy<T> lhs, U&& rhs)noexcept{return lhs /= rhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xy<T> operator/(U&& lhs, xy<T> rhs)noexcept{return rhs /= lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline wh<T>& operator*=(wh<T>& lhs, U&& rhs)noexcept{lhs.w *= rhs; lhs.h *= rhs; return lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline wh<T> operator*(wh<T> lhs, U&& rhs)noexcept{return lhs *= rhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline wh<T> operator*(U&& lhs, wh<T> rhs)noexcept{return rhs *= lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline wh<T>& operator/=(wh<T>& lhs, U&& rhs)noexcept{lhs.w /= rhs; lhs.h /= rhs; return lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline wh<T> operator/(wh<T> lhs, U&& rhs)noexcept{return lhs /= rhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline wh<T> operator/(U&& lhs, wh<T> rhs)noexcept{return rhs /= lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xyxy<T>& operator*=(xyxy<T>& lhs, U&& rhs)noexcept{lhs._1 *= rhs; lhs._2 *= rhs; return lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xyxy<T> operator*(xyxy<T> lhs, U&& rhs)noexcept{return lhs *= rhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xyxy<T> operator*(U&& lhs, xyxy<T> rhs)noexcept{return rhs *= lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xyxy<T>& operator/=(xyxy<T>& lhs, U&& rhs)noexcept{lhs._1 /= rhs; lhs._2 /= rhs; return lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xyxy<T> operator/(xyxy<T> lhs, U&& rhs)noexcept{return lhs /= rhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xyxy<T> operator/(U&& lhs, xyxy<T> rhs)noexcept{return rhs /= lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xywh<T>& operator*=(xywh<T>& lhs, U&& rhs)noexcept{lhs.xy *= rhs; lhs.wh *= rhs; return lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xywh<T> operator*(xywh<T> lhs, U&& rhs)noexcept{return lhs *= rhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xywh<T> operator*(U&& lhs, xywh<T> rhs)noexcept{return rhs *= lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xywh<T>& operator/=(xywh<T>& lhs, U&& rhs)noexcept{lhs.xy /= rhs; lhs.wh /= rhs; return lhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xywh<T> operator/(xywh<T> lhs, U&& rhs)noexcept{return lhs /= rhs;}
	template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
	inline xywh<T> operator/(U&& lhs, xywh<T> rhs)noexcept{return rhs /= lhs;}
}
}