//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<amp.h>
#include<amp_math.h>
#include<nmmintrin.h>

namespace will{

namespace amp{

namespace math{

inline float abs(float x)restrict(amp){
	return concurrency::fast_math::fabs(x);
}
using concurrency::direct3d::abs;
using std::abs;

using concurrency::fast_math::acos;
using concurrency::fast_math::asin;
using concurrency::fast_math::atan;
using concurrency::fast_math::atan2;
using concurrency::fast_math::ceil;
using concurrency::fast_math::cos;
using concurrency::fast_math::cosh;
using concurrency::fast_math::exp;
using concurrency::fast_math::exp2;
using concurrency::fast_math::floor;

inline float max(float x, float y)restrict(amp){
	return concurrency::fast_math::fmax(x, y);
}
inline int max(int x, int y)restrict(amp){
	return concurrency::direct3d::imax(x, y);
}
inline unsigned int max(unsigned int x, unsigned int y)restrict(amp){
	return concurrency::direct3d::umax(x, y);
}
inline float max(float x, float y)restrict(cpu){
	return std::max(x, y);
}
inline int max(int x, int y)restrict(cpu){
	return std::max(x, y);
}
inline unsigned int max(unsigned int x, unsigned int y)restrict(cpu){
	return std::max(x, y);
}
template<typename T, typename... Args>
inline T max(T x, T y, T z, Args... args)restrict(cpu, amp){
	return max(max(x, y), z, args...);
}

inline float min(float x, float y)restrict(amp){
	return concurrency::fast_math::fmin(x, y);
}
inline int min(int x, int y)restrict(amp){
	return concurrency::direct3d::imin(x, y);
}
inline unsigned int min(unsigned int x, unsigned int y)restrict(amp){
	return concurrency::direct3d::umin(x, y);
}
inline float min(float x, float y)restrict(cpu){
	return std::min(x, y);
}
inline int min(int x, int y)restrict(cpu){
	return std::min(x, y);
}
inline unsigned int min(unsigned int x, unsigned int y)restrict(cpu){
	return std::min(x, y);
}
template<typename T, typename... Args>
inline T min(T x, T y, T z, Args... args)restrict(cpu, amp){
	return min(min(x, y), z, args...);
}

inline float fmod(float x, float y)restrict(amp){
	return concurrency::fast_math::fmod(x, y);
}
using std::fmod;

inline float mod(float x, float y)restrict(cpu, amp){
	return x - floor(x / y) * y;
}

struct frexp_result{
	float mantissa;
	int exponent;
};
inline frexp_result frexp(float x)restrict(amp){
	int e;
	const auto ret = concurrency::fast_math::frexp(x, &e);
	return {ret, e};
}
inline frexp_result frexp(float x)restrict(cpu){
	int e;
	const auto ret = std::frexp(x, &e);
	return {ret, e};
}

inline bool isfinite(float x)restrict(amp){
	return concurrency::fast_math::isfinite(x) != 0;
}
using std::isfinite;

inline bool isinf(float x)restrict(amp){
	return concurrency::fast_math::isinf(x) != 0;
}
using std::isinf;

inline bool isnan(float x)restrict(amp){
	return concurrency::fast_math::isnan(x) != 0;
}
using std::isnan;

using concurrency::fast_math::ldexp;
using concurrency::fast_math::log;
using concurrency::fast_math::log10;
using concurrency::fast_math::log2;

struct modf_result{
	float integer_portion;
	float fractional_portion;
};
inline modf_result modf(float x)restrict(amp){
	float integer;
	const auto frac = concurrency::fast_math::modf(x, &integer);
	return {integer, frac};
}
inline modf_result modf(float x)restrict(cpu){
	float integer;
	const auto frac = std::modf(x, &integer);
	return {integer, frac};
}

using concurrency::fast_math::pow;
using concurrency::fast_math::round;
using concurrency::fast_math::rsqrt;

inline bool signbit(float x)restrict(amp){
	return concurrency::fast_math::signbit(x) != 0;
}
using std::signbit;

using concurrency::fast_math::sin;

struct sincos_result{
	float sin;
	float cos;
};
inline sincos_result sincos(float x)restrict(amp){
	float sin, cos;
	concurrency::fast_math::sincos(x, &sin, &cos);
	return {sin, cos};
}
inline sincos_result sincos(float x)restrict(cpu){
	return {std::sin(x), std::cos(x)};
}

using concurrency::fast_math::sinh;
using concurrency::fast_math::sqrt;
using concurrency::fast_math::tan;
using concurrency::fast_math::tanh;
using concurrency::fast_math::trunc;

inline int clamp(int x, int min_value, int max_value)restrict(cpu){
	return min(max(x, min_value), max_value);
}
inline float clamp(float x, float min_value, float max_value)restrict(cpu){
	return min(max(x, min_value), max_value);
}

using concurrency::direct3d::clamp;

inline unsigned int countbits(unsigned int x)restrict(cpu){
	return ::__popcnt(x);
}

using concurrency::direct3d::countbits;

inline unsigned int firstbithigh(int x)restrict(cpu){
	if(x < 0)
		x &= 0x7fffffff;
	return ::__lzcnt(x)+1;
}

using concurrency::direct3d::firstbithigh;

inline int firstbitlow(int x)restrict(cpu){
	return countbits((~x)&(x-1));
}

using concurrency::direct3d::firstbitlow;

inline float mad(float x, float y, float z)restrict(cpu){
	return x*y+z;
}
inline double mad(double x, double y, double z)restrict(cpu){
	return x*y+z;
}
inline int mad(int x, int y, int z)restrict(cpu){//TODO: FMA3 intrinsics
	return x*y+z;
}
inline unsigned int mad(unsigned int x, unsigned int y, unsigned int z)restrict(cpu){//TODO: FMA3 intrinsics
	return x*y+z;
}

using concurrency::direct3d::mad;

inline float radians(float degrees)restrict(cpu){
	return degrees * 0.0174532f;
}
inline double radians(double degrees)restrict(cpu){
	return degrees * 0.01745329251994329;
}
using concurrency::direct3d::radians;

inline float rcp(float x)restrict(cpu){
	return 1/x;
}
using concurrency::direct3d::rcp;
using concurrency::direct3d::reversebits;

inline float saturate(float x)restrict(cpu){
	return clamp(0.f, 1.f, x);
}
using concurrency::direct3d::saturate;

inline float sign(float x)restrict(cpu, amp){
	return x > 0.f ? 1.f
	               : x < 0.f ? -1.f
	                         :  0.f;
}
using concurrency::direct3d::sign;

inline float smoothstep(float a, float b, float x)restrict(cpu){
	x = saturate((x-a)/(b-a));
	return x*x*(3-2*x);
}
using concurrency::direct3d::smoothstep;

inline float step(float e, float x)restrict(cpu){
	return x < e ? 0.f : 1.f;
}
using concurrency::direct3d::step;

template<typename T>
inline float hypot(T a, T b)restrict(cpu, amp){
	return sqrt(a*a+b*b);
}

inline float degrees(float radians)restrict(cpu, amp){
	return radians * 57.2957f;
}
inline double degrees(double radians)restrict(cpu, amp){
	return radians * 57.29577951308232;
}

inline float lerp(float x, float y, float s)restrict(cpu, amp){
	return x + s*(y-x);
}

inline float inversesqrt(float x)restrict(cpu, amp){
	return 1.f / sqrt(x);
}

}

}

}