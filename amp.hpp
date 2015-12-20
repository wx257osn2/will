#pragma once
#include<amp.h>
#include<amp_math.h>
#include<amp_graphics.h>
#undef min
#undef max
#include<d3d11.h>

namespace will{

namespace ampm{

using namespace concurrency::fast_math;

inline float hypot(float a, float b)restrict(amp){
	return sqrt(a*a+b*b);
}

}

namespace ampg = concurrency::graphics;

namespace amp{

using namespace concurrency;

namespace shader{

using namespace ampg;

struct inputs{
	texture_view<unorm_4, 2> texture;
	float_2 resolution;
	float time;
};

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
inline float length(const T& vec)restrict(amp){
	return ampm::hypot(vec.x, vec.y);
}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
inline float length(const T& vec)restrict(amp){
	return ampm::hypot(vec.x, ampm::hypot(vec.y, vec.z));
}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
inline float length(const T& vec)restrict(amp){
	return ampm::hypot(vec.x, ampm::hypot(vec.y, ampm::hypot(vec.z, vec.w)));
}
template<typename T>
inline T normalize(const T& vec)restrict(amp){
	return vec / length(vec);
}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
inline typename short_vector_traits<T>::value_type dot(const T& lhs, const T& rhs)restrict(cpu, amp){
	return lhs.x * rhs.x + lhs.y * rhs.y;
}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
inline typename short_vector_traits<T>::value_type dot(const T& lhs, const T& rhs)restrict(cpu, amp){
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
inline typename short_vector_traits<T>::value_type dot(const T& lhs, const T& rhs)restrict(cpu, amp){
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
inline T cross(const T& lhs, const T& rhs)restrict(cpu, amp){
	return T{lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x};
}

inline float min(float a, float b)restrict(amp){return concurrency::fast_math::fmin(a, b);}
inline float max(float a, float b)restrict(amp){return concurrency::fast_math::fmax(a, b);}
inline float abs(float v)restrict(amp){return concurrency::fast_math::fabsf(v);}
inline float mod(float a, float b)restrict(amp){return a - concurrency::fast_math::floor(a / b) * b;}
inline float fract(float v)restrict(amp){return v - concurrency::fast_math::floor(v);}
inline float mix(float x, float y, float a)restrict(amp){return x * (1.f-a) + y * a;}
inline float dot(float x, float y)restrict(amp){return x * y;}
template<typename T, std::enable_if_t<1 <= short_vector_traits<T>::size && short_vector_traits<T>::size <= 4>* = nullptr>
inline T reflect(const T& i, const T& n)restrict(amp){return i - 2.f * dot(n, i) * n;}
inline float clamp(float x, float min_val, float max_val)restrict(amp){return min(max(x, min_val), max_val);}

inline float_2 get_coord(const amp::index<2>& idx)restrict(cpu, amp){return {static_cast<float>(idx[1]), static_cast<float>(idx[0])};}
inline float_2 get_coord(const amp::index<2>& idx, const float_2& resolution)restrict(cpu, amp){return {static_cast<float>(idx[1]), resolution.y - idx[0]};}
inline float_2 get_position(const float_2& coord, const float_2& resolution)restrict(amp){return (coord.xy * 2.f - resolution) / min(resolution.x, resolution.y);}
inline float_2 get_position(const amp::index<2>& idx, const float_2& resolution)restrict(amp){return get_position(get_coord(idx), resolution);}

template<typename, int> struct create_short_vector;
template<> struct create_short_vector<double, 2>{using type = double_2;};
template<> struct create_short_vector<double, 3>{using type = double_3;};
template<> struct create_short_vector<double, 4>{using type = double_4;};
template<> struct create_short_vector<float,  2>{using type =  float_2;};
template<> struct create_short_vector<float,  3>{using type =  float_3;};
template<> struct create_short_vector<float,  4>{using type =  float_4;};
template<> struct create_short_vector<int,    2>{using type =    int_2;};
template<> struct create_short_vector<int,    3>{using type =    int_3;};
template<> struct create_short_vector<int,    4>{using type =    int_4;};
template<> struct create_short_vector<norm,   2>{using type =   norm_2;};
template<> struct create_short_vector<norm,   3>{using type =   norm_3;};
template<> struct create_short_vector<norm,   4>{using type =   norm_4;};
template<> struct create_short_vector<uint,   2>{using type =   uint_2;};
template<> struct create_short_vector<uint,   3>{using type =   uint_3;};
template<> struct create_short_vector<uint,   4>{using type =   uint_4;};
template<> struct create_short_vector<unorm,  2>{using type =  unorm_2;};
template<> struct create_short_vector<unorm,  3>{using type =  unorm_3;};
template<> struct create_short_vector<unorm,  4>{using type =  unorm_4;};

template<typename T>
typename create_short_vector<T, 2>::type vec2(T _1)restrict(cpu, amp){return {_1};}
template<typename T>
typename create_short_vector<T, 2>::type vec2(T _1, T _2)restrict(cpu, amp){return {_1, _2};}
template<typename T>
typename create_short_vector<T, 2>::type vec2(const typename create_short_vector<T, 2>::type& _12)restrict(cpu, amp){return _12;}
template<typename T, typename U, std::enable_if_t<(short_vector_traits<U>::size > 2)>* = nullptr>
typename create_short_vector<T, 2>::type vec2(const U& _12n)restrict(cpu, amp){return static_cast<typename create_short_vector<T, 2>::type>(_12n.xy);}
template<typename T, typename U, std::enable_if_t<!std::is_same<T, U>::value && short_vector_traits<U>::size == 1>* = nullptr>
typename create_short_vector<T, 2>::type vec2(U _1)restrict(cpu, amp){return vec2(static_cast<T>(_1));}
template<typename T, typename U, typename V, std::enable_if_t<(!std::is_same<T, U>::value || !std::is_same<T, V>::value) && short_vector_traits<U>::size == 1 && short_vector_traits<V>::size == 1>* = nullptr>
typename create_short_vector<T, 2>::type vec2(U _1, V _2)restrict(cpu, amp){return vec2(static_cast<T>(_1), static_cast<T>(_2));}
template<typename T, typename U, std::enable_if_t<!std::is_same<T, typename short_vector_traits<U>::value_type>::value && short_vector_traits<U>::size == 2>* = nullptr>
typename create_short_vector<T, 2>::type vec2(const U& _12)restrict(cpu, amp){return static_cast<typename create_short_vector<T, 2>::type>(_12);}
template<typename T>
typename create_short_vector<T, 3>::type vec3(T _1)restrict(cpu, amp){return {_1};}
template<typename T>
typename create_short_vector<T, 3>::type vec3(T _1, T _2, T _3)restrict(cpu, amp){return {_1, _2, _3};}
template<typename T>
typename create_short_vector<T, 3>::type vec3(T _1, const typename create_short_vector<T, 2>::type& _23)restrict(cpu, amp){return {_1, _23.x, _23.y};}
template<typename T>
typename create_short_vector<T, 3>::type vec3(const typename create_short_vector<T, 2>::type& _12, T _3)restrict(cpu, amp){return {_12.x, _12.y, _3};}
template<typename T>
typename create_short_vector<T, 3>::type vec3(const typename create_short_vector<T, 3>::type& _123)restrict(cpu, amp){return _123;}
template<typename T, typename U, std::enable_if_t<(short_vector_traits<U>::size > 3)>* = nullptr>
typename create_short_vector<T, 3>::type vec3(const U& _123n)restrict(cpu, amp){return static_cast<typename create_short_vector<T, 3>::type>(_123n.xyz);}
template<typename T, typename U, std::enable_if_t<!std::is_same<T, U>::value && short_vector_traits<U>::size == 1>* = nullptr>
typename create_short_vector<T, 3>::type vec3(U _1)restrict(cpu, amp){return vec3(static_cast<T>(_1));}
template<typename T, typename U, typename V, typename W, std::enable_if_t<(!std::is_same<T, U>::value || !std::is_same<T, V>::value || !std::is_same<T, W>::value) && short_vector_traits<U>::size == 1 && short_vector_traits<V>::size == 1 && short_vector_traits<W>::size == 1>* = nullptr>
typename create_short_vector<T, 3>::type vec3(U _1, V _2, W _3)restrict(cpu, amp){return vec3(static_cast<T>(_1), static_cast<T>(_2), static_cast<T>(_3));}
template<typename T, typename U, typename V, std::enable_if_t<(!std::is_same<T, U>::value || !std::is_same<T, typename short_vector_traits<V>::value_type>::value) && short_vector_traits<U>::size == 1 && short_vector_traits<V>::size == 2>* = nullptr>
typename create_short_vector<T, 3>::type vec3(U _1, const V& _23)restrict(cpu, amp){return vec3(static_cast<T>(_1), static_cast<typename create_short_vector<T, 2>::type>(_23));}
template<typename T, typename U, typename V, std::enable_if_t<(!std::is_same<T, typename short_vector_traits<U>::value_type>::value || !std::is_same<T, V>::value) && short_vector_traits<U>::size == 2 && short_vector_traits<V>::size == 1>* = nullptr>
typename create_short_vector<T, 3>::type vec3(const U& _12, V _3)restrict(cpu, amp){return vec3(static_cast<typename create_short_vector<T, 2>::type>(_12), static_cast<T>(_3));}
template<typename T, typename U, std::enable_if_t<!std::is_same<T, typename short_vector_traits<U>::value_type>::value && short_vector_traits<U>::size == 3>* = nullptr>
typename create_short_vector<T, 3>::type vec3(const U& _123)restrict(cpu, amp){return static_cast<typename create_short_vector<T, 3>::type>(_123);}
template<typename T>
typename create_short_vector<T, 4>::type vec4(T _1)restrict(cpu, amp){return {_1};}
template<typename T>
typename create_short_vector<T, 4>::type vec4(T _1, T _2, T _3, T _4)restrict(cpu, amp){return {_1, _2, _3, _4};}
template<typename T>
typename create_short_vector<T, 4>::type vec4(T _1, T _2, const typename create_short_vector<T, 2>::type& _34)restrict(cpu, amp){return {_1, _2, _34.x, _34.y};}
template<typename T>
typename create_short_vector<T, 4>::type vec4(T _1, const typename create_short_vector<T, 2>::type& _23, T _4)restrict(cpu, amp){return {_1, _23.x, _23.y, _4};}
template<typename T>
typename create_short_vector<T, 4>::type vec4(const typename create_short_vector<T, 2>::type& _12, T _3, T _4)restrict(cpu, amp){return {_12.x, _12.y, _3, _4};}
template<typename T>
typename create_short_vector<T, 4>::type vec4(const typename create_short_vector<T, 2>::type& _12, const typename create_short_vector<T, 2>::type& _34)restrict(cpu, amp){return {_12.x, _12.y, _34.x, _34.y};}
template<typename T>
typename create_short_vector<T, 4>::type vec4(T _1, const typename create_short_vector<T, 3>::type& _234)restrict(cpu, amp){return {_1, _234.x, _234.y, _234.z};}
template<typename T>
typename create_short_vector<T, 4>::type vec4(const typename create_short_vector<T, 3>::type& _123, T _4)restrict(cpu, amp){return {_123.x, _123.y, _123.z, _4};}
template<typename T>
typename create_short_vector<T, 4>::type vec4(const typename create_short_vector<T, 4>::type& _1234)restrict(cpu, amp){return _1234;}
template<typename T, typename U, std::enable_if_t<!std::is_same<T, U>::value && short_vector_traits<U>::size == 1>* = nullptr>
typename create_short_vector<T, 4>::type vec4(U _1)restrict(cpu, amp){return vec4(static_cast<T>(_1));}
template<typename T, typename U, typename V, typename W, typename S, std::enable_if_t<(!std::is_same<T, U>::value || !std::is_same<T, V>::value || !std::is_same<T, W>::value || !std::is_same<T, S>::value) && short_vector_traits<U>::size == 1 && short_vector_traits<V>::size == 1 && short_vector_traits<W>::size == 1 && short_vector_traits<S>::size == 1>* = nullptr>
typename create_short_vector<T, 4>::type vec4(U _1, V _2, W _3, S _4)restrict(cpu, amp){return vec3(static_cast<T>(_1), static_cast<T>(_2), static_cast<T>(_3), static_cast<S>(_4));}
template<typename T, typename U, typename V, typename W, std::enable_if_t<(!std::is_same<T, U>::value || !std::is_same<T, V>::value || !std::is_same<T, typename short_vector_traits<W>::value_type>::value) && short_vector_traits<U>::size == 1 && short_vector_traits<V>::size == 1 && short_vector_traits<W>::size == 2>* = nullptr>
typename create_short_vector<T, 4>::type vec4(U _1, V _2, const W& _34)restrict(cpu, amp){return vec4(static_cast<T>(_1), static_cast<T>(_2), static_cast<typename create_short_vector<T, 2>::type>(_34));}
template<typename T, typename U, typename V, typename W, std::enable_if_t<(!std::is_same<T, U>::value || !std::is_same<T, typename short_vector_traits<V>::value_type>::value || !std::is_same<T, W>::value) && short_vector_traits<U>::size == 1 && short_vector_traits<V>::size == 2 && short_vector_traits<W>::size == 1>* = nullptr>
typename create_short_vector<T, 4>::type vec4(U _1, const V& _23, W _4)restrict(cpu, amp){return vec4(static_cast<T>(_1), static_cast<typename create_short_vector<T, 2>::type>(_23), static_cast<T>(_4));}
template<typename T, typename U, typename V, typename W, std::enable_if_t<(!std::is_same<T, typename short_vector_traits<U>::value_type>::value || !std::is_same<T, V>::value || !std::is_same<T, W>::value) && short_vector_traits<U>::size == 2 && short_vector_traits<V>::size == 1 && short_vector_traits<W>::size == 1>* = nullptr>
typename create_short_vector<T, 4>::type vec4(const U& _12, V _3, W _4)restrict(cpu, amp){return vec4(static_cast<typename create_short_vector<T, 2>::type>(_12), static_cast<T>(_3), static_cast<T>(_4));}
template<typename T, typename U, typename V, std::enable_if_t<(!std::is_same<T, typename short_vector_traits<U>::value_type>::value || !std::is_same<T, typename short_vector_traits<V>::value_type>::value) && short_vector_traits<U>::size == 2 && short_vector_traits<V>::size == 2>* = nullptr>
typename create_short_vector<T, 4>::type vec4(const U& _12, const V& _34)restrict(cpu, amp){return vec4(static_cast<typename create_short_vector<T, 2>::type>(_12), static_cast<typename create_short_vector<T, 2>::type>(_34));}
template<typename T, typename U, typename V, std::enable_if_t<(!std::is_same<T, U>::value || !std::is_same<T, typename short_vector_traits<V>::value_type>::value) && short_vector_traits<U>::size == 1 && short_vector_traits<V>::size == 3>* = nullptr>
typename create_short_vector<T, 4>::type vec4(U _1, const V& _234)restrict(cpu, amp){return vec4(static_cast<T>(_1), static_cast<typename create_short_vector<T, 3>::type>(_234));}
template<typename T, typename U, typename V, std::enable_if_t<(!std::is_same<T, typename short_vector_traits<U>::value_type>::value || !std::is_same<T, V>::value) && short_vector_traits<U>::size == 3 && short_vector_traits<V>::size == 1>* = nullptr>
typename create_short_vector<T, 4>::type vec4(const U& _123, V _4)restrict(cpu, amp){return vec4(static_cast<typename create_short_vector<T, 3>::type>(_123), static_cast<T>(_4));}
template<typename T, typename U, std::enable_if_t<!std::is_same<T, typename short_vector_traits<U>::value_type>::value && short_vector_traits<U>::size == 4>* = nullptr>
typename create_short_vector<T, 4>::type vec4(const U& _1234)restrict(cpu, amp){return static_cast<typename create_short_vector<T, 4>::type>(_1234);}

template<typename T, typename U, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T operator*(const T& t, U u)restrict(cpu, amp){return {t.x * u, t.y * u};}
template<typename T, typename U, std::enable_if_t<short_vector_traits<U>::size == 2>* = nullptr>
U operator*(T t, const U& u)restrict(cpu, amp){return {t * u.x, t * u.y};}
template<typename T, typename U, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T operator*(const T& t, U u)restrict(cpu, amp){return {t.x * u, t.y * u, t.z * u};}
template<typename T, typename U, std::enable_if_t<short_vector_traits<U>::size == 3>* = nullptr>
U operator*(T t, const U& u)restrict(cpu, amp){return {t * u.x, t * u.y, t * u.z};}
template<typename T, typename U, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T operator*(const T& t, U u)restrict(cpu, amp){return {t.x * u, t.y * u, t.z * u, t.w * u};}
template<typename T, typename U, std::enable_if_t<short_vector_traits<U>::size == 4>* = nullptr>
U operator*(T t, const U& u)restrict(cpu, amp){return {t * u.x, t * u.y, t * u.z, t * u.w};}

template<typename T, typename U, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T operator/(const T& t, U u)restrict(cpu, amp){return {t.x / u, t.y / u};}
template<typename T, typename U, std::enable_if_t<short_vector_traits<U>::size == 2>* = nullptr>
U operator/(T t, const U& u)restrict(cpu, amp){return {t / u.x, t / u.y};}
template<typename T, typename U, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T operator/(const T& t, U u)restrict(cpu, amp){return {t.x / u, t.y / u, t.z / u};}
template<typename T, typename U, std::enable_if_t<short_vector_traits<U>::size == 3>* = nullptr>
U operator/(T t, const U& u)restrict(cpu, amp){return {t / u.x, t / u.y, t / u.z};}
template<typename T, typename U, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T operator/(const T& t, U u)restrict(cpu, amp){return {t.x / u, t.y / u, t.z / u, t.w / u};}
template<typename T, typename U, std::enable_if_t<short_vector_traits<U>::size == 4>* = nullptr>
U operator/(T t, const U& u)restrict(cpu, amp){return {t / u.x, t / u.y, t / u.z, t / u.w};}

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T abs(const T& t)restrict(amp){return {abs(t.x), abs(t.y)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T abs(const T& t)restrict(amp){return {abs(t.x), abs(t.y), abs(t.z)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T abs(const T& t)restrict(amp){return {abs(t.x), abs(t.y), abs(t.z), abs(t.w)};}

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T pow(const T& t, const T& s)restrict(amp){return {concurrency::fast_math::pow(t.x, s.x), concurrency::fast_math::pow(t.y, s.y)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T pow(const T& t, const T& s)restrict(amp){return {concurrency::fast_math::pow(t.x, s.x), concurrency::fast_math::pow(t.y, s.y), concurrency::fast_math::pow(t.z, s.z)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T pow(const T& t, const T& s)restrict(amp){return {concurrency::fast_math::pow(t.x, s.x), concurrency::fast_math::pow(t.y, s.y), concurrency::fast_math::pow(t.z, s.z), concurrency::fast_math::pow(t.w, s.w)};}

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T mod(const T& t, const T& s)restrict(amp){return {mod(t.x, s.x), mod(t.y, s.y)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T mod(const T& t, const T& s)restrict(amp){return {mod(t.x, s.x), mod(t.y, s.y), mod(t.z, s.z)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T mod(const T& t, const T& s)restrict(amp){return {mod(t.x, s.x), mod(t.y, s.y), mod(t.z, s.z), mod(t.w, s.w)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T mod(const T& t, float s)restrict(amp){return {mod(t.x, s), mod(t.y, s)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T mod(const T& t, float s)restrict(amp){return {mod(t.x, s), mod(t.y, s), mod(t.z, s)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T mod(const T& t, float s)restrict(amp){return {mod(t.x, s), mod(t.y, s), mod(t.z, s), mod(t.w, s)};}

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T floor(const T& t)restrict(amp){return {concurrency::fast_math::floor(t.x), concurrency::fast_math::floor(t.y)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T floor(const T& t)restrict(amp){return {concurrency::fast_math::floor(t.x), concurrency::fast_math::floor(t.y), concurrency::fast_math::floor(t.z)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T floor(const T& t)restrict(amp){return {concurrency::fast_math::floor(t.x), concurrency::fast_math::floor(t.y), concurrency::fast_math::floor(t.z), concurrency::fast_math::floor(t.w)};}

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T ceil(const T& t)restrict(amp){return {concurrency::fast_math::ceil(t.x), concurrency::fast_math::ceil(t.y)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T ceil(const T& t)restrict(amp){return {concurrency::fast_math::ceil(t.x), concurrency::fast_math::ceil(t.y), concurrency::fast_math::ceil(t.z)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T ceil(const T& t)restrict(amp){return {concurrency::fast_math::ceil(t.x), concurrency::fast_math::ceil(t.y), concurrency::fast_math::ceil(t.z), concurrency::fast_math::ceil(t.w)};}

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T fract(const T& t)restrict(amp){return {fract(t.x), fract(t.y)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T fract(const T& t)restrict(amp){return {fract(t.x), fract(t.y), fract(t.z)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T fract(const T& t)restrict(amp){return {fract(t.x), fract(t.y), fract(t.z), fract(t.w)};}

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T mix(const T& x, const T& y, const T& a)restrict(amp){return {mix(x.x, y.x, a.x), mix(x.y, y.y, a.y)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T mix(const T& x, const T& y, const T& a)restrict(amp){return {mix(x.x, y.x, a.x), mix(x.y, y.y, a.y), mix(x.z, y.z, a.z)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T mix(const T& x, const T& y, const T& a)restrict(amp){return {mix(x.x, y.x, a.x), mix(x.y, y.y, a.y), mix(x.z, y.z, a.z), mix(x.w, y.w, a.w)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T mix(const T& x, const T& y, float a)restrict(amp){return {mix(x.x, y.x, a), mix(x.y, y.y, a)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T mix(const T& x, const T& y, float a)restrict(amp){return {mix(x.x, y.x, a), mix(x.y, y.y, a), mix(x.z, y.z, a)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T mix(const T& x, const T& y, float a)restrict(amp){return {mix(x.x, y.x, a), mix(x.y, y.y, a), mix(x.z, y.z, a), mix(x.w, y.w, a)};}

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T sin(const T& t)restrict(amp){return {concurrency::fast_math::sin(t.x), concurrency::fast_math::sin(t.y)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T sin(const T& t)restrict(amp){return {concurrency::fast_math::sin(t.x), concurrency::fast_math::sin(t.y), concurrency::fast_math::sin(t.z)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T sin(const T& t)restrict(amp){return {concurrency::fast_math::sin(t.x), concurrency::fast_math::sin(t.y), concurrency::fast_math::sin(t.z), concurrency::fast_math::sin(t.w)};}

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T cos(const T& t)restrict(amp){return {concurrency::fast_math::cos(t.x), concurrency::fast_math::cos(t.y)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T cos(const T& t)restrict(amp){return {concurrency::fast_math::cos(t.x), concurrency::fast_math::cos(t.y), concurrency::fast_math::cos(t.z)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T cos(const T& t)restrict(amp){return {concurrency::fast_math::cos(t.x), concurrency::fast_math::cos(t.y), concurrency::fast_math::cos(t.z), concurrency::fast_math::cos(t.w)};}

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T min(const T& t, const T& s)restrict(amp){return {min(t.x, s.x), min(t.y, s.y)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T min(const T& t, const T& s)restrict(amp){return {min(t.x, s.x), min(t.y, s.y), min(t.z, s.z)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T min(const T& t, const T& s)restrict(amp){return {min(t.x, s.x), min(t.y, s.y), min(t.z, s.z), min(t.w, s.w)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T min(const T& t, float s)restrict(amp){return {min(t.x, s), min(t.y, s)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T min(const T& t, float s)restrict(amp){return {min(t.x, s), min(t.y, s), min(t.z, s)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T min(const T& t, float s)restrict(amp){return {min(t.x, s), min(t.y, s), min(t.z, s), min(t.w, s)};}

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T max(const T& t, const T& s)restrict(amp){return {max(t.x, s.x), max(t.y, s.y)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T max(const T& t, const T& s)restrict(amp){return {max(t.x, s.x), max(t.y, s.y), max(t.z, s.z)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T max(const T& t, const T& s)restrict(amp){return {max(t.x, s.x), max(t.y, s.y), max(t.z, s.z), max(t.w, s.w)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T max(const T& t, float s)restrict(amp){return {max(t.x, s), max(t.y, s)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T max(const T& t, float s)restrict(amp){return {max(t.x, s), max(t.y, s), max(t.z, s)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T max(const T& t, float s)restrict(amp){return {max(t.x, s), max(t.y, s), max(t.z, s), max(t.w, s)};}

template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T clamp(const T& t, const T& min, const T& max)restrict(amp){return {clamp(t.x, min.x, max.x), clamp(t.y, min.y, max.y)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T clamp(const T& t, const T& min, const T& max)restrict(amp){return {clamp(t.x, min.x, max.x), clamp(t.y, min.y, max.y), clamp(t.z, min.z, max.z)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T clamp(const T& t, const T& min, const T& max)restrict(amp){return {clamp(t.x, min.x, max.x), clamp(t.y, min.y, max.y), clamp(t.z, min.z, max.z), clamp(t.w, min.w, max.w)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 2>* = nullptr>
T clamp(const T& t, float min, float max)restrict(amp){return {clamp(t.x, min, max), clamp(t.y, min, max)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 3>* = nullptr>
T clamp(const T& t, float min, float max)restrict(amp){return {clamp(t.x, min, max), clamp(t.y, min, max), clamp(t.z, min, max)};}
template<typename T, std::enable_if_t<short_vector_traits<T>::size == 4>* = nullptr>
T clamp(const T& t, float min, float max)restrict(amp){return {clamp(t.x, min, max), clamp(t.y, min, max), clamp(t.z, min, max), clamp(t.w, min, max)};}

}

}

}