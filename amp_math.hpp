//Copyright (C) 2014-2019 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include<cmath>
#pragma warning(push)
#pragma warning(disable: 5040)
#include<amp.h>
#include<amp_math.h>
#pragma warning(pop)
#include<nmmintrin.h>

namespace will{

namespace amp{

namespace math{

#define USING_PRECISE_MATH1(name) inline double name(double x)restrict(amp){return concurrency::precise_math::name(x);}
#define USING_PRECISE_MATH2(name) inline double name(double x, double y)restrict(amp){return concurrency::precise_math::name(x, y);}

inline float abs(float x)restrict(amp){
	return concurrency::fast_math::fabs(x);
}
inline double abs(double x)restrict(amp){
	return concurrency::precise_math::fabs(x);
}
using concurrency::direct3d::abs;
using std::abs;

using concurrency::fast_math::acos;
USING_PRECISE_MATH1(acos)
using std::acos;
inline float acosh(float x)restrict(amp){
	return concurrency::fast_math::log(x+concurrency::fast_math::sqrt(x*x-1.f));
}
USING_PRECISE_MATH1(acosh)
using std::acosh;
using concurrency::fast_math::asin;
USING_PRECISE_MATH1(asin)
using std::asin;
inline float asinh(float x)restrict(amp){
	return concurrency::fast_math::log(x+concurrency::fast_math::sqrt(x*x+1.f));
}
USING_PRECISE_MATH1(asinh)
using std::asinh;
using concurrency::fast_math::atan;
USING_PRECISE_MATH1(atan)
using std::atan;
using concurrency::fast_math::atan2;
USING_PRECISE_MATH2(atan2)
using std::atan2;
inline float atanh(float x)restrict(amp){
	return concurrency::fast_math::log((1.f+x)/(1.f-x))/2.f;
}
USING_PRECISE_MATH1(atanh)
using std::atanh;
using concurrency::precise_math::cbrt;
using std::cbrt;
using concurrency::fast_math::ceil;
USING_PRECISE_MATH1(ceil)
using std::ceil;
inline float copysign(float x, float y)restrict(amp){
	const bool is_negative = y < 0;
	return concurrency::fast_math::isnan(x) ?
		is_negative ? -NAN : NAN :
		is_negative ? -concurrency::fast_math::fabsf(x) : concurrency::fast_math::fabsf(x);
}
USING_PRECISE_MATH2(copysign)
using std::copysign;
using concurrency::fast_math::cos;
USING_PRECISE_MATH1(cos)
using std::cos;
using concurrency::fast_math::cosh;
USING_PRECISE_MATH1(cosh)
using std::cosh;
inline float cospi(float x)restrict(amp){
	return concurrency::fast_math::cos(x*3.1415926f);
}
USING_PRECISE_MATH1(cospi)
using concurrency::precise_math::erf;
using std::erf;
using concurrency::precise_math::erfc;
using std::erfc;
using concurrency::precise_math::erfcinv;
using concurrency::precise_math::erff;
using concurrency::precise_math::erfinv;
using concurrency::fast_math::exp;
USING_PRECISE_MATH1(exp)
using std::exp;
using concurrency::fast_math::exp2;
USING_PRECISE_MATH1(exp2)
using std::exp2;
inline float exp10(float x)restrict(amp){
	return exp(x*concurrency::fast_math::log(10.f));
}
USING_PRECISE_MATH1(exp10)
inline float expm1(float x)restrict(amp){
	return exp(x)-1.f;
}
USING_PRECISE_MATH1(expm1)
using std::expm1;
using concurrency::fast_math::floor;
USING_PRECISE_MATH1(floor)
using std::floor;
inline float fma(float x, float y, float z)restrict(amp){
	return x * y + z;
}
inline double fma(double x, double y, double z)restrict(amp){
	return concurrency::precise_math::fma(x, y, z);
}
using std::fma;

inline float fdim(float x, float y)restrict(amp){
	const float diff = x - y;
	return diff > 0.f ? diff : 0.f;
}
inline double fdim(double x, double y)restrict(amp){
	return concurrency::precise_math::fdim(x, y);
}
using std::fdim;

inline float max(float x, float y)restrict(amp){
	return concurrency::fast_math::fmax(x, y);
}
inline double max(double x, double y)restrict(amp){
	return concurrency::precise_math::fmax(x, y);
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
inline double max(double x, double y)restrict(cpu){
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
inline double min(double x, double y)restrict(amp){
	return concurrency::precise_math::fmin(x, y);
}
inline int min(int x, int y)restrict(amp){
	return concurrency::direct3d::imin(x, y);
}
inline unsigned int min(unsigned int x, unsigned int y)restrict(amp){
	return concurrency::direct3d::umin(x, y);
}
inline float min(float x, float y)restrict(cpu){
	return std::max(x, y);
}
inline double min(double x, double y)restrict(cpu){
	return std::max(x, y);
}
inline int min(int x, int y)restrict(cpu){
	return std::max(x, y);
}
inline unsigned int min(unsigned int x, unsigned int y)restrict(cpu){
	return std::max(x, y);
}
template<typename T, typename... Args>
inline T min(T x, T y, T z, Args... args)restrict(cpu, amp){
	return min(min(x, y), z, args...);
}

using concurrency::fast_math::fmod;
USING_PRECISE_MATH2(fmod)
using std::fmod;

inline float mod(float x, float y)restrict(cpu, amp){
	return x - floor(x / y) * y;
}
inline double mod(double x, double y)restrict(cpu, amp){
	return x - floor(x / y) * y;
}

using concurrency::precise_math::fpclassify;
using std::fpclassify;

template<typename Float>
struct frexp_result{
	Float mantissa;
	int exponent;
};
inline frexp_result<double> frexp(double x)restrict(amp){
	int e;
	const auto ret = concurrency::precise_math::frexp(x, &e);
	return {ret, e};
}
inline frexp_result<float> frexp(float x)restrict(amp){
	int e;
	const auto ret = concurrency::fast_math::frexp(x, &e);
	return {ret, e};
}
inline frexp_result<double> frexp(double x)restrict(cpu){
	int e;
	const auto ret = std::frexp(x, &e);
	return {ret, e};
}
inline frexp_result<float> frexp(float x)restrict(cpu){
	int e;
	const auto ret = std::frexp(x, &e);
	return {ret, e};
}

using concurrency::precise_math::ilogb;
using std::ilogb;

inline bool isfinite(float x)restrict(amp){
	return concurrency::fast_math::isfinite(x) != 0;
}
inline bool isfinite(double x)restrict(amp){
	return concurrency::precise_math::isfinite(x) != 0;
}
using std::isfinite;

inline bool isinf(float x)restrict(amp){
	return concurrency::fast_math::isinf(x) != 0;
}
inline bool isinf(double x)restrict(amp){
	return concurrency::precise_math::isinf(x) != 0;
}
using std::isinf;

inline bool isnan(float x)restrict(amp){
	return concurrency::fast_math::isnan(x) != 0;
}
inline bool isnan(double x)restrict(amp){
	return concurrency::precise_math::isnan(x) != 0;
}
using std::isnan;

using concurrency::fast_math::ldexp;
inline double ldexp(double x, int y)restrict(amp){
	return concurrency::precise_math::ldexp(x, y);
}
using std::ldexp;
using concurrency::fast_math::log;
USING_PRECISE_MATH1(log)
using std::log;
using concurrency::fast_math::log10;
USING_PRECISE_MATH1(log10)
using std::log10;
using concurrency::fast_math::log2;
USING_PRECISE_MATH1(log2)
using std::log2;

template<typename Float>
struct lgamma_result{
	Float lgamma;
	int sign;
};
inline lgamma_result<float> lgamma(float x)restrict(amp){
	int sign;
	const auto ret = concurrency::precise_math::lgamma(x, &sign);
	return {ret, sign};
}
inline lgamma_result<double> lgamma(double x)restrict(amp){
	int sign;
	const auto ret = concurrency::precise_math::lgamma(x, &sign);
	return {ret, sign};
}

template<typename Float>
struct modf_result{
	Float integer_portion;
	Float fractional_portion;
};
inline modf_result<float> modf(float x)restrict(amp){
	float integer;
	const auto frac = concurrency::fast_math::modf(x, &integer);
	return {integer, frac};
}
inline modf_result<double> modf(double x)restrict(amp){
	double integer;
	const auto frac = concurrency::precise_math::modf(x, &integer);
	return {integer, frac};
}
inline modf_result<float> modf(float x)restrict(cpu){
	float integer;
	const auto frac = std::modf(x, &integer);
	return {integer, frac};
}
inline modf_result<double> modf(double x)restrict(cpu){
	double integer;
	const auto frac = std::modf(x, &integer);
	return {integer, frac};
}

inline double nan(int x)restrict(amp){
	return concurrency::precise_math::nan(x);
}
inline double nan(int x)restrict(cpu){
	return std::nan(std::to_string(x).c_str());
}

using concurrency::precise_math::nearbyint;
using std::nearbyint;
using concurrency::precise_math::nextafter;
using std::nextafter;

inline float phi(float x)restrict(amp){
	return .5f* erfc(-x * static_cast<float>(M_SQRT1_2));
}
USING_PRECISE_MATH1(phi)
inline float phi(float x)restrict(cpu){
	return .5f * erfc(-x * static_cast<float>(M_SQRT1_2));
}
inline double phi(double x)restrict(cpu){
	return .5f * erfc(-x * M_SQRT1_2);
}

using concurrency::fast_math::pow;
USING_PRECISE_MATH2(pow)
using std::pow;

using concurrency::precise_math::probit;
using concurrency::precise_math::rcbrt;
using concurrency::precise_math::remainder;

template<typename Float>
struct remquo_result{
	Float rem;
	int quo;
};
inline remquo_result<float> remquo(float x, float y)restrict(amp){
	const auto ret = fmod(x, y);
	return {ret, static_cast<int>((x-ret)/y)};
}
inline remquo_result<double> remquo(double x, double y)restrict(amp){
	int quo;
	const auto ret = concurrency::precise_math::remquo(x, y, &quo);
	return {ret, quo};
}
inline remquo_result<float> remquo(float x, float y)restrict(cpu){
	int quo;
	const auto ret = std::remquo(x, y, &quo);
	return {ret, quo};
}
inline remquo_result<double> remquo(double x, double y)restrict(cpu){
	int quo;
	const auto ret = std::remquo(x, y, &quo);
	return {ret, quo};
}

using concurrency::fast_math::round;
USING_PRECISE_MATH1(round)
using std::round;
using concurrency::fast_math::rsqrt;
USING_PRECISE_MATH1(rsqrt)
inline float rsqrt(float x)restrict(cpu){
	return 1.f/std::sqrt(x);
}
inline double rsqrt(double x)restrict(cpu){
	return 1./std::sqrt(x);
}

inline float scalb(float x, float n)restrict(amp){
	return x * concurrency::fast_math::pow(static_cast<float>(FLT_RADIX), n);
}
USING_PRECISE_MATH2(scalb)
inline float scalbn(float x, int n)restrict(amp){
	return x * concurrency::fast_math::pow(static_cast<float>(FLT_RADIX), static_cast<float>(n));
}
inline double scalbn(double x, int n)restrict(amp){
	return concurrency::precise_math::scalbn(x, n);
}

inline bool signbit(float x)restrict(amp){
	return concurrency::fast_math::signbit(x) != 0;
}
inline bool signbit(double x)restrict(amp){
	return concurrency::precise_math::signbit(x) != 0;
}
using std::signbit;

using concurrency::fast_math::sin;
USING_PRECISE_MATH1(sin)

template<typename Float>
struct sincos_result{
	Float sin;
	Float cos;
};
inline sincos_result<float> sincos(float x)restrict(amp){
	float sin, cos;
	concurrency::fast_math::sincos(x, &sin, &cos);
	return {sin, cos};
}
inline sincos_result<double> sincos(double x)restrict(amp){
	double sin, cos;
	concurrency::precise_math::sincos(x, &sin, &cos);
	return {sin, cos};
}
inline sincos_result<float> sincos(float x)restrict(cpu){
	return {std::sin(x), std::cos(x)};
}
inline sincos_result<double> sincos(double x)restrict(cpu){
	return {std::sin(x), std::cos(x)};
}

using concurrency::fast_math::sinh;
USING_PRECISE_MATH1(sinh)
using concurrency::fast_math::sqrt;
USING_PRECISE_MATH1(sqrt)
using concurrency::precise_math::sinpi;
using concurrency::fast_math::tan;
USING_PRECISE_MATH1(tan)
using concurrency::fast_math::tanh;
USING_PRECISE_MATH1(tanh)
inline float tanpi(float x)restrict(amp){
	return concurrency::fast_math::tan(x*3.1415926f);
}
USING_PRECISE_MATH1(tanpi)
using concurrency::precise_math::tgamma;
using concurrency::fast_math::trunc;
USING_PRECISE_MATH1(trunc)

inline int clamp(int x, int min_value, int max_value)restrict(cpu){
	return min(max(x, min_value), max_value);
}
inline float clamp(float x, float min_value, float max_value)restrict(cpu){
	return min(max(x, min_value), max_value);
}

using concurrency::direct3d::clamp;
inline double clamp(double x, double min_value, double max_value)restrict(cpu, amp){
	return min(max(x, min_value), max_value);
}

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
inline double radians(double degrees)restrict(cpu, amp){
	return degrees * 0.01745329251994329;
}
using concurrency::direct3d::radians;

inline float rcp(float x)restrict(cpu){
	return 1.f/x;
}
using concurrency::direct3d::rcp;
using concurrency::direct3d::reversebits;

inline float saturate(float x)restrict(cpu){
	return clamp(0.f, 1.f, x);
}
using concurrency::direct3d::saturate;
inline double saturate(double x)restrict(cpu, amp){
	return clamp(0., 1., x);
}

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
inline double smoothstep(double a, double b, double x)restrict(cpu, amp){
	x = saturate((x-a)/(b-a));
	return x*x*(3-2*x);
}

inline float step(float e, float x)restrict(cpu){
	return x < e ? 0.f : 1.f;
}
using concurrency::direct3d::step;

using std::hypot;
inline float hypot(float a, float b)restrict(amp){return sqrt(a*a+b*b);}
USING_PRECISE_MATH2(hypot)

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

struct xorshift32{
	using result_type = unsigned int;
	using int_type = unsigned int;
private:
	static unsigned int rotate_left(unsigned int x, unsigned int n)restrict(cpu, amp){
		return (x << (n % 32)) | (x >> (32 - (n % 32)));
	}
	int_type iseed[4] = {123456789u, 362436069u, 21288629u, 88675123u};
public:

	static result_type min()restrict(cpu, amp){return 0;}
	static result_type max()restrict(cpu, amp){return 0xFFFFFFFF;}

	xorshift32() = default;

	explicit xorshift32(int_type seed)restrict(cpu, amp){
		iseed[0] ^= seed;
		iseed[1] ^= rotate_left(seed,17);
		iseed[2] ^= rotate_left(seed,31);
		iseed[3] ^= rotate_left(seed,18);
		discard(20);
	}

	xorshift32(int_type x, int_type y, int_type z, int_type w)restrict(cpu, amp) : iseed{x, y, z, w}{
		if(x == 0 && y == 0 && z == 0 && w == 0)
			iseed[0] = 123456789u, iseed[1] = 362436069u, iseed[2] = 21288629u, iseed[3] = 88675123u;
	}

	result_type operator()()restrict(cpu, amp){
		const int_type t = iseed[0] ^ ( iseed[0] << 11 );
		iseed[0] = iseed[1]; iseed[1] = iseed[2]; iseed[2] = iseed[3];
		iseed[3] = (iseed[3] ^ (iseed[3] >> 19)) ^ (t ^ (t >> 8));
		return iseed[3];
	}

	void discard(int_type n)restrict(cpu, amp){
		for(int_type i = 0; i < n; ++i)
			(*this)();
	}

	bool operator==(const xorshift32& rhs)const restrict(cpu, amp){
		return iseed[0] == rhs.iseed[0]
			&& iseed[1] == rhs.iseed[1]
			&& iseed[2] == rhs.iseed[2]
			&& iseed[3] == rhs.iseed[3];
	}
	friend bool operator!=(const xorshift32& lhs, const xorshift32& rhs)restrict(cpu, amp){
		return !(lhs == rhs);
	}
};

namespace detail{
template<typename T>
struct numeric_limits;
template<>
struct numeric_limits<float>{
	static constexpr int digits()restrict(cpu, amp){return FLT_MANT_DIG;}
	static constexpr float max()restrict(cpu, amp){return FLT_MAX;}
	static constexpr float epsilon()restrict(cpu, amp){return FLT_EPSILON;}
};
template<>
struct numeric_limits<double>{
	static constexpr int digits()restrict(cpu, amp){return DBL_MANT_DIG;}
	static constexpr double max()restrict(cpu, amp){return DBL_MAX;}
	static constexpr double epsilon()restrict(cpu, amp){return DBL_EPSILON;}
};
}

template<typename T, unsigned int Bits, typename RNG, std::enable_if_t<std::disjunction<std::is_same<T, float>, std::is_same<T, double>, std::is_same<T, long double>>::value, std::nullptr_t> = nullptr>
inline T generate_canonical(RNG& rng)restrict(cpu, amp){
	const int minbits = min(static_cast<int>(Bits), detail::numeric_limits<T>::digits());

	const T rngmin = static_cast<T>(rng.min());
	const T rngmax = static_cast<T>(rng.max());
	const T x = (rngmax - rngmin) + static_cast<T>(1);

	const int ceil = static_cast<int>(will::amp::math::ceil(static_cast<T>(minbits) / log2(x)));
	const int loop_num = max(ceil, 1);

	T ans = static_cast<T>(0);
	T factor = static_cast<T>(1);

	for (int i = 0; i < loop_num; ++i){
		ans += (static_cast<T>(rng()) - rngmin) * factor;
		factor *= x;
	}

	return ans / factor;
}

template<class T = float>
struct uniform_real_distribution{
	using result_type = T;

	class param_type{
		result_type min;
		result_type max;
	public:
		using distribution_type = uniform_real_distribution<T>;

		explicit param_type(T min = T(0), T max = T(1))restrict(cpu, amp) : min(min), max(max){
			if(!(min <= max && (0 <= min || max <= min + detail::numeric_limits<T>::max())))
				min = 0.f, max = 1.f;
		}

		friend bool operator==(const param_type& lhs, const param_type& rhs)restrict(cpu, amp){
			return lhs.min == rhs.min && lhs.max == rhs.max;
		}
		friend bool operator!=(const param_type& lhs, const param_type& rhs)restrict(cpu, amp){
			return !(lhs == rhs);
		}

		result_type a()const restrict(cpu, amp){
			return min;
		}
		result_type b()const restrict(cpu, amp){
			return max;
		}
	};
private:
	template<class RNG>
	static result_type eval(RNG& rng, const param_type& p)restrict(cpu, amp){
		return generate_canonical<T, std::numeric_limits<T>::digits>(rng) * (p.b() - p.a()) + p.a();
	}
	param_type para;
public:
	explicit uniform_real_distribution(T min = T(0), T max = T(1))restrict(cpu, amp) : para(min, max){}
	explicit uniform_real_distribution(const param_type& p)restrict(cpu, amp) : para(p){}

	result_type a()const restrict(cpu, amp){
		return para.a();
	}
	result_type b()const restrict(cpu, amp){
		return para.b();
	}

	param_type param()const restrict(cpu, amp){
		return para;
	}
	void param(const param_type& p)restrict(cpu, amp){
		para = p;
	}

	result_type min()const restrict(cpu, amp){
		return para.a();
	}
	result_type max()const restrict(cpu, amp){
		return para.b();
	}

	void reset()restrict(cpu, amp){}

	template<class RNG>
	result_type operator()(RNG& rng)const restrict(cpu, amp){
		return eval(rng, para);
	}
	template<class RNG>
	result_type operator()(RNG& rng, const param_type& p)const restrict(cpu, amp){
		return eval(rng, p);
	}
};

#undef USING_PRECISE_MATH2
#undef USING_PRECISE_MATH1

}

}

}