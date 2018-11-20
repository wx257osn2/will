//Copyright (C) 2014-2018 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#pragma warning(push)
#pragma warning(disable: 5040)
#include<amp.h>
#include<amp_graphics.h>
#pragma warning(pop)
#include<d3d11.h>
#include"d3d.hpp"
#include"amp_math.hpp"

namespace will{

namespace amp{

namespace graphics{

namespace detail{

template<typename ValueType, int Rank>
struct texture{using type = concurrency::graphics::texture<ValueType, Rank>;};
template<typename ValueType, int Rank>
struct texture_view{using type = concurrency::graphics::texture_view<ValueType, Rank>;};

}

template<typename ValueType, int Rank>
using texture = typename detail::texture<ValueType, Rank>::type;
template<typename ValueType, int Rank>
using texture_view = typename detail::texture_view<ValueType, Rank>::type;

template<typename ValueType, int Rank>
inline texture_view<ValueType, Rank> get_view(concurrency::graphics::texture<ValueType, Rank>& tex, unsigned int mipmap_level = 0u)restrict(cpu){
	return texture_view<ValueType, Rank>(tex, mipmap_level);
}
template<typename ValueType, int Rank>
inline texture_view<ValueType, Rank> get_view(concurrency::graphics::texture<ValueType, Rank>& tex)restrict(amp){
	return texture_view<ValueType, Rank>(tex);
}
template<typename ValueType, int Rank>
inline texture_view<const ValueType, Rank> get_view(const concurrency::graphics::texture<ValueType, Rank>& tex)restrict(cpu, amp){
	return texture_view<const ValueType, Rank>(tex);
}

using concurrency::graphics::filter_mode;
	using concurrency::graphics::filter_point;
	using concurrency::graphics::filter_linear;
	using concurrency::graphics::filter_unknown;
using concurrency::graphics::address_mode;
	using concurrency::graphics::address_wrap;
	using concurrency::graphics::address_mirror;
	using concurrency::graphics::address_clamp;
	using concurrency::graphics::address_border;
	using concurrency::graphics::address_unknown;
using concurrency::graphics::sampler;
using concurrency::graphics::norm;
using concurrency::graphics::uint;
using concurrency::graphics::unorm;
using concurrency::graphics::copy;
using concurrency::graphics::copy_async;

inline namespace direct3d{

template<typename ValueType, int Rank>
inline expected<texture<ValueType, Rank>> make_texture(const concurrency::accelerator_view& av, IUnknown* d3d_texture, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN)restrict(cpu)try{
	return concurrency::graphics::direct3d::make_texture<ValueType, Rank>(av, d3d_texture, format);
}catch(...){
	return make_unexpected_from_current_exception();
}
template<typename ValueType, int Rank, typename D3DTexture, std::enable_if_t<!std::is_base_of<IUnknown, std::remove_pointer_t<std::decay_t<D3DTexture>>>::value, std::nullptr_t> = nullptr>
inline expected<texture<ValueType, Rank>> make_texture(const concurrency::accelerator_view& av, D3DTexture&& d3d_texture, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN)restrict(cpu){
	return make_texture<ValueType, Rank>(av, d3d_texture.get(), format);
}

template<typename ValueT, int Rank>
inline will::d3d::texture2d get_texture(const concurrency::graphics::texture_view<ValueT, Rank>& texture)restrict(cpu){
	return will::d3d::texture2d{reinterpret_cast<ID3D11Texture2D*>(concurrency::graphics::direct3d::get_texture(texture))};
}

}

using concurrency::graphics::double_2;
using concurrency::graphics::double_3;
using concurrency::graphics::double_4;
using concurrency::graphics::float_2;
using concurrency::graphics::float_3;
using concurrency::graphics::float_4;
using concurrency::graphics::int_2;
using concurrency::graphics::int_3;
using concurrency::graphics::int_4;
using concurrency::graphics::norm_2;
using concurrency::graphics::norm_3;
using concurrency::graphics::norm_4;
using concurrency::graphics::uint_2;
using concurrency::graphics::uint_3;
using concurrency::graphics::uint_4;
using concurrency::graphics::unorm_2;
using concurrency::graphics::unorm_3;
using concurrency::graphics::unorm_4;

template<typename T>
struct short_vector_traits : concurrency::graphics::short_vector_traits<T>{};

namespace detail{
template<typename T, typename = void>struct is_short_vector_impl{using type = std::false_type;};
template<typename T>struct is_short_vector_impl<T, std::enable_if_t<1 <= short_vector_traits<T>::size && short_vector_traits<T>::size <= 4>>{using type = std::true_type;};
}
template<typename T>
using is_short_vector = typename detail::is_short_vector_impl<std::decay_t<T>>::type;

inline norm noise(float x)restrict(amp){return norm{concurrency::direct3d::noise(x)};}

}

inline namespace direct3d{

inline expected<accelerator_view> create_accelerator_view(ID3D11Device* d3d_device, queuing_mode mode = queuing_mode_automatic)try{
	auto accv = concurrency::direct3d::create_accelerator_view(d3d_device, mode);
	return std::move(accv);
}catch(concurrency::runtime_exception&){
	return make_unexpected_from_current_exception();
}
template<typename D3DDevice, std::enable_if_t<!std::is_base_of<IUnknown, std::remove_pointer_t<std::decay_t<D3DDevice>>>::value, std::nullptr_t> = nullptr>
inline expected<accelerator_view> create_accelerator_view(D3DDevice&& d3d_device, queuing_mode mode = queuing_mode_automatic){
	return create_accelerator_view(std::forward<D3DDevice>(d3d_device).get(), mode);
}
inline expected<accelerator_view> create_accelerator_view(accelerator& acc, bool disable_timeout, queuing_mode mode = queuing_mode_automatic)try{
	return concurrency::direct3d::create_accelerator_view(acc, disable_timeout, mode);
}catch(concurrency::runtime_exception&){
	return make_unexpected_from_current_exception();
}

}

namespace shader{

using namespace math;
using namespace graphics;

namespace detail{

template<typename, int> struct original_short_vector;
template<> struct original_short_vector<double, 2>{using type = concurrency::graphics::double_2;};
template<> struct original_short_vector<double, 3>{using type = concurrency::graphics::double_3;};
template<> struct original_short_vector<double, 4>{using type = concurrency::graphics::double_4;};
template<> struct original_short_vector<float,  2>{using type = concurrency::graphics:: float_2;};
template<> struct original_short_vector<float,  3>{using type = concurrency::graphics:: float_3;};
template<> struct original_short_vector<float,  4>{using type = concurrency::graphics:: float_4;};
template<> struct original_short_vector<int,    2>{using type = concurrency::graphics::   int_2;};
template<> struct original_short_vector<int,    3>{using type = concurrency::graphics::   int_3;};
template<> struct original_short_vector<int,    4>{using type = concurrency::graphics::   int_4;};
template<> struct original_short_vector<norm,   2>{using type = concurrency::graphics::  norm_2;};
template<> struct original_short_vector<norm,   3>{using type = concurrency::graphics::  norm_3;};
template<> struct original_short_vector<norm,   4>{using type = concurrency::graphics::  norm_4;};
template<> struct original_short_vector<uint,   2>{using type = concurrency::graphics::  uint_2;};
template<> struct original_short_vector<uint,   3>{using type = concurrency::graphics::  uint_3;};
template<> struct original_short_vector<uint,   4>{using type = concurrency::graphics::  uint_4;};
template<> struct original_short_vector<unorm,  2>{using type = concurrency::graphics:: unorm_2;};
template<> struct original_short_vector<unorm,  3>{using type = concurrency::graphics:: unorm_3;};
template<> struct original_short_vector<unorm,  4>{using type = concurrency::graphics:: unorm_4;};

template<int N, typename... Args>struct is_same_size_short_vectors{
	template<typename, bool Second>struct second{static constexpr bool value = Second;};
	template<bool...>struct impl;
	static constexpr bool value = std::is_same<impl<(short_vector_traits<std::decay_t<Args>>::size == N)...>, impl<second<Args, true>::value...>>::value;
};


template<typename, bool, int...>struct vec_mat_construct_impl;
template<int N, int... Ns, int M, int... Q>struct vec_mat_construct_impl<std::integer_sequence<int, N, Ns...>,  true, M, Q...> : vec_mat_construct_impl<std::integer_sequence<int, N - M, Ns...>, (N >= M), Q...>{
	using next = vec_mat_construct_impl<std::integer_sequence<int, N - M, Ns...>, (N >= M), Q...>;
	static constexpr int size = N < M ? N : M ;
};
template<       int... Ns, int M, int... Q>struct vec_mat_construct_impl<std::integer_sequence<int, 0, Ns...>,  true, M, Q...> : vec_mat_construct_impl<std::integer_sequence<int,        Ns...>, true, M, Q...>{};
template<int N, int... Ns, int M, int... Q>struct vec_mat_construct_impl<std::integer_sequence<int, N, Ns...>, false, M, Q...> : vec_mat_construct_impl<std::integer_sequence<int,        Ns...>, true, M, Q...>{};
template<int I, int... Ns>struct vec_mat_construct_impl<std::integer_sequence<int, I, Ns...>,  true> : std::false_type{};
template<int I>struct vec_mat_construct_impl<std::integer_sequence<int, I>, false> : std::true_type{};
template<     >struct vec_mat_construct_impl<std::integer_sequence<int, 0>,  true> : std::true_type{};

template<typename T, int... Ns>class vec_mat_construct{
	struct sentinel{};
	template<typename Impl, typename V, typename... Ts, std::enable_if_t<Impl::size == 1 &&  short_vector_traits<V>::size == 1 >* = nullptr>
	static T impl2(const V& v, Ts&&... ts)restrict(cpu, amp){return impl<typename Impl::next>(forward<Ts>(ts)..., v);}
	template<typename Impl, typename V, typename... Ts, std::enable_if_t<Impl::size == 1 &&  short_vector_traits<V>::size != 1 >* = nullptr>
	static T impl2(const V& v, Ts&&... ts)restrict(cpu, amp){return impl<typename Impl::next>(forward<Ts>(ts)..., v.x);}
	template<typename Impl, typename V, typename... Ts, std::enable_if_t<Impl::size == 2 && (short_vector_traits<V>::size >= 2)>* = nullptr>
	static T impl2(const V& v, Ts&&... ts)restrict(cpu, amp){return impl<typename Impl::next>(forward<Ts>(ts)..., v.x, v.y);}
	template<typename Impl, typename V, typename... Ts, std::enable_if_t<Impl::size == 3 && (short_vector_traits<V>::size >= 3)>* = nullptr>
	static T impl2(const V& v, Ts&&... ts)restrict(cpu, amp){return impl<typename Impl::next>(forward<Ts>(ts)..., v.x, v.y, v.z);}
	template<typename Impl, typename V, typename... Ts, std::enable_if_t<Impl::size == 4 &&  short_vector_traits<V>::size == 4 >* = nullptr>
	static T impl2(const V& v, Ts&&... ts)restrict(cpu, amp){return impl<typename Impl::next>(forward<Ts>(ts)..., v.x, v.y, v.z, v.w);}
	template<typename Impl, typename... Ts>
	static T impl(sentinel&&, Ts&&... ts)restrict(cpu, amp){return T{static_cast<typename T::value_type>(ts)...};}
	template<typename Impl, typename V, typename... Ts>
	static T impl(V&& v, Ts&&... ts)restrict(cpu, amp){return impl2<Impl>(forward<V>(v), forward<Ts>(ts)...);}
	template<typename... Args, typename Impl = vec_mat_construct_impl<std::integer_sequence<int, Ns...>, true, short_vector_traits<std::decay_t<Args>>::size...>>
	static T dispatch1(Args&&... args)restrict(cpu, amp){return impl<Impl>(forward<Args>(args)..., sentinel{});}
	template<typename V, typename... Args, typename Impl = vec_mat_construct_impl<std::integer_sequence<int, Ns...>, true, short_vector_traits<std::decay_t<Args>>::size...>>
	static T dispatch2(V&& v, Args&&... args)restrict(cpu, amp){return impl<Impl>(forward<Args>(args)..., sentinel{}, forward<V>(v));}
public:
	template<typename V, typename... Args, std::enable_if_t< is_short_vector<V>::value>* = nullptr>
	static T create(V&& v, Args&&... args)restrict(cpu, amp){return dispatch1(forward<V>(v), forward<Args>(args)...);}
	template<typename V, typename... Args, std::enable_if_t<!is_short_vector<V>::value>* = nullptr>
	static T create(V&& v, Args&&... args)restrict(cpu, amp){return dispatch2(forward<V>(v), forward<Args>(args)...);}
};


}

#define WILL_AMP_DECL_VEC2(A, B) vec<T, 2> get_##A##B()const restrict(cpu, amp){return {this->A, this->B};} __declspec(property(get=get_##A##B)) vec<T, 2> A##B;
#define WILL_AMP_DECL_VEC3(A, B, C) vec<T, 3> get_##A##B##C()const restrict(cpu, amp){return {this->A, this->B, this->C};} __declspec(property(get=get_##A##B##C)) vec<T, 3> A##B##C;
#define WILL_AMP_DECL_VEC4(A, B, C, D) vec<T, 4> get_##A##B##C##D()const restrict(cpu, amp){return {this->A, this->B, this->C, this->D};} __declspec(property(get=get_##A##B##C##D)) vec<T, 4> A##B##C##D;
template<typename, int>class vec;
template<typename T>
class vec<T, 2> : public detail::original_short_vector<T, 2>::type{
	using base_type = typename detail::original_short_vector<T, 2>::type;
public:
	using base_type::base_type;
	vec()restrict(cpu, amp):base_type{}{}
	vec(const vec& other)restrict(cpu, amp):base_type{static_cast<const base_type&>(other)}{}
	vec(vec&& other)restrict(cpu, amp):base_type{static_cast<base_type&&>(other)}{}
	vec& operator=(const vec& other)restrict(cpu, amp){*static_cast<base_type*>(this) = static_cast<const base_type&>(other);return *this;}
	vec& operator=(vec&& other)restrict(cpu, amp){*static_cast<base_type*>(this) = static_cast<base_type&&>(other);return *this;}
	template<typename U, std::enable_if_t<short_vector_traits<U>::size == 1>* = nullptr>
	vec(U _1)restrict(cpu, amp):vec(static_cast<T>(_1)){}
	template<typename U, typename... Args>
	vec(U&& u, Args&&... args)restrict(cpu, amp):base_type{detail::vec_mat_construct<base_type, base_type::size>::create(forward<U>(u), forward<Args>(args)...)}{}
	T& operator[](unsigned int n)restrict(cpu, amp){
		switch(n%2){
		case 0:
			return ref_x();
		case 1:
			return ref_y();
		}
	}
	T operator[](unsigned int n)const restrict(cpu, amp){
		switch(n%2){
		case 0:
			return get_x();
		case 1:
			return get_y();
		}
	}
	WILL_AMP_DECL_VEC2(x, x)
	WILL_AMP_DECL_VEC2(y, y)
	WILL_AMP_DECL_VEC3(x, x, x)
	WILL_AMP_DECL_VEC3(x, x, y)
	WILL_AMP_DECL_VEC3(x, y, x)
	WILL_AMP_DECL_VEC3(x, y, y)
	WILL_AMP_DECL_VEC3(y, x, x)
	WILL_AMP_DECL_VEC3(y, x, y)
	WILL_AMP_DECL_VEC3(y, y, x)
	WILL_AMP_DECL_VEC3(y, y, y)
	WILL_AMP_DECL_VEC4(x, x, x, x)
	WILL_AMP_DECL_VEC4(x, x, x, y)
	WILL_AMP_DECL_VEC4(x, x, y, x)
	WILL_AMP_DECL_VEC4(x, x, y, y)
	WILL_AMP_DECL_VEC4(x, y, x, x)
	WILL_AMP_DECL_VEC4(x, y, x, y)
	WILL_AMP_DECL_VEC4(x, y, y, x)
	WILL_AMP_DECL_VEC4(x, y, y, y)
	WILL_AMP_DECL_VEC4(y, x, x, x)
	WILL_AMP_DECL_VEC4(y, x, x, y)
	WILL_AMP_DECL_VEC4(y, x, y, x)
	WILL_AMP_DECL_VEC4(y, x, y, y)
	WILL_AMP_DECL_VEC4(y, y, x, x)
	WILL_AMP_DECL_VEC4(y, y, x, y)
	WILL_AMP_DECL_VEC4(y, y, y, x)
	WILL_AMP_DECL_VEC4(y, y, y, y)
	WILL_AMP_DECL_VEC2(r, r)
	WILL_AMP_DECL_VEC2(g, g)
	WILL_AMP_DECL_VEC3(r, r, r)
	WILL_AMP_DECL_VEC3(r, r, g)
	WILL_AMP_DECL_VEC3(r, g, r)
	WILL_AMP_DECL_VEC3(r, g, g)
	WILL_AMP_DECL_VEC3(g, r, r)
	WILL_AMP_DECL_VEC3(g, r, g)
	WILL_AMP_DECL_VEC3(g, g, r)
	WILL_AMP_DECL_VEC3(g, g, g)
	WILL_AMP_DECL_VEC4(r, r, r, r)
	WILL_AMP_DECL_VEC4(r, r, r, g)
	WILL_AMP_DECL_VEC4(r, r, g, r)
	WILL_AMP_DECL_VEC4(r, r, g, g)
	WILL_AMP_DECL_VEC4(r, g, r, r)
	WILL_AMP_DECL_VEC4(r, g, r, g)
	WILL_AMP_DECL_VEC4(r, g, g, r)
	WILL_AMP_DECL_VEC4(r, g, g, g)
	WILL_AMP_DECL_VEC4(g, r, r, r)
	WILL_AMP_DECL_VEC4(g, r, r, g)
	WILL_AMP_DECL_VEC4(g, r, g, r)
	WILL_AMP_DECL_VEC4(g, r, g, g)
	WILL_AMP_DECL_VEC4(g, g, r, r)
	WILL_AMP_DECL_VEC4(g, g, r, g)
	WILL_AMP_DECL_VEC4(g, g, g, r)
	WILL_AMP_DECL_VEC4(g, g, g, g)
};
template<typename T>
class vec<T, 3> : public detail::original_short_vector<T, 3>::type{
	using base_type = typename detail::original_short_vector<T, 3>::type;
public:
	using base_type::base_type;
	vec()restrict(cpu, amp):base_type{}{}
	vec(const vec& other)restrict(cpu, amp):base_type{static_cast<const base_type&>(other)}{}
	vec(vec&& other)restrict(cpu, amp):base_type{static_cast<base_type&&>(other)}{}
	vec& operator=(const vec& other)restrict(cpu, amp){*static_cast<base_type*>(this) = static_cast<const base_type&>(other);return *this;}
	vec& operator=(vec&& other)restrict(cpu, amp){*static_cast<base_type*>(this) = static_cast<base_type&&>(other);return *this;}
	template<typename U, std::enable_if_t<short_vector_traits<U>::size == 1>* = nullptr>
	vec(U _1)restrict(cpu, amp):vec{static_cast<T>(_1)}{}
	template<typename U, typename... Args>
	vec(U&& u, Args&&... args)restrict(cpu, amp):base_type{detail::vec_mat_construct<base_type, base_type::size>::create(forward<U>(u), forward<Args>(args)...)}{}
	T& operator[](unsigned int n)restrict(cpu, amp){
		switch(n%3){
		case 0:
			return ref_x();
		case 1:
			return ref_y();
		case 2:
			return ref_z();
		}
	}
	T operator[](unsigned int n)const restrict(cpu, amp){
		switch(n%3){
		case 0:
			return get_x();
		case 1:
			return get_y();
		case 2:
			return get_z();
		}
	}
	WILL_AMP_DECL_VEC2(x, x)
	WILL_AMP_DECL_VEC2(y, y)
	WILL_AMP_DECL_VEC2(z, z)
	WILL_AMP_DECL_VEC3(x, x, x)
	WILL_AMP_DECL_VEC3(x, x, y)
	WILL_AMP_DECL_VEC3(x, x, z)
	WILL_AMP_DECL_VEC3(x, y, x)
	WILL_AMP_DECL_VEC3(x, y, y)
	WILL_AMP_DECL_VEC3(x, z, x)
	WILL_AMP_DECL_VEC3(x, z, z)
	WILL_AMP_DECL_VEC3(y, x, x)
	WILL_AMP_DECL_VEC3(y, x, y)
	WILL_AMP_DECL_VEC3(y, y, x)
	WILL_AMP_DECL_VEC3(y, y, y)
	WILL_AMP_DECL_VEC3(y, y, z)
	WILL_AMP_DECL_VEC3(y, z, y)
	WILL_AMP_DECL_VEC3(y, z, z)
	WILL_AMP_DECL_VEC3(z, x, x)
	WILL_AMP_DECL_VEC3(z, x, z)
	WILL_AMP_DECL_VEC3(z, y, y)
	WILL_AMP_DECL_VEC3(z, y, z)
	WILL_AMP_DECL_VEC3(z, z, x)
	WILL_AMP_DECL_VEC3(z, z, y)
	WILL_AMP_DECL_VEC3(z, z, z)
	WILL_AMP_DECL_VEC4(x, x, x, x)
	WILL_AMP_DECL_VEC4(x, x, x, y)
	WILL_AMP_DECL_VEC4(x, x, x, z)
	WILL_AMP_DECL_VEC4(x, x, y, x)
	WILL_AMP_DECL_VEC4(x, x, y, y)
	WILL_AMP_DECL_VEC4(x, x, y, z)
	WILL_AMP_DECL_VEC4(x, x, z, x)
	WILL_AMP_DECL_VEC4(x, x, z, y)
	WILL_AMP_DECL_VEC4(x, x, z, z)
	WILL_AMP_DECL_VEC4(x, y, x, x)
	WILL_AMP_DECL_VEC4(x, y, x, y)
	WILL_AMP_DECL_VEC4(x, y, x, z)
	WILL_AMP_DECL_VEC4(x, y, y, x)
	WILL_AMP_DECL_VEC4(x, y, y, y)
	WILL_AMP_DECL_VEC4(x, y, y, z)
	WILL_AMP_DECL_VEC4(x, y, z, x)
	WILL_AMP_DECL_VEC4(x, y, z, y)
	WILL_AMP_DECL_VEC4(x, y, z, z)
	WILL_AMP_DECL_VEC4(x, z, x, x)
	WILL_AMP_DECL_VEC4(x, z, x, y)
	WILL_AMP_DECL_VEC4(x, z, x, z)
	WILL_AMP_DECL_VEC4(x, z, y, x)
	WILL_AMP_DECL_VEC4(x, z, y, y)
	WILL_AMP_DECL_VEC4(x, z, y, z)
	WILL_AMP_DECL_VEC4(x, z, z, x)
	WILL_AMP_DECL_VEC4(x, z, z, y)
	WILL_AMP_DECL_VEC4(x, z, z, z)
	WILL_AMP_DECL_VEC4(y, x, x, x)
	WILL_AMP_DECL_VEC4(y, x, x, y)
	WILL_AMP_DECL_VEC4(y, x, x, z)
	WILL_AMP_DECL_VEC4(y, x, y, x)
	WILL_AMP_DECL_VEC4(y, x, y, y)
	WILL_AMP_DECL_VEC4(y, x, y, z)
	WILL_AMP_DECL_VEC4(y, x, z, x)
	WILL_AMP_DECL_VEC4(y, x, z, y)
	WILL_AMP_DECL_VEC4(y, x, z, z)
	WILL_AMP_DECL_VEC4(y, y, x, x)
	WILL_AMP_DECL_VEC4(y, y, x, y)
	WILL_AMP_DECL_VEC4(y, y, x, z)
	WILL_AMP_DECL_VEC4(y, y, y, x)
	WILL_AMP_DECL_VEC4(y, y, y, y)
	WILL_AMP_DECL_VEC4(y, y, y, z)
	WILL_AMP_DECL_VEC4(y, y, z, x)
	WILL_AMP_DECL_VEC4(y, y, z, y)
	WILL_AMP_DECL_VEC4(y, y, z, z)
	WILL_AMP_DECL_VEC4(y, z, x, x)
	WILL_AMP_DECL_VEC4(y, z, x, y)
	WILL_AMP_DECL_VEC4(y, z, x, z)
	WILL_AMP_DECL_VEC4(y, z, y, x)
	WILL_AMP_DECL_VEC4(y, z, y, y)
	WILL_AMP_DECL_VEC4(y, z, y, z)
	WILL_AMP_DECL_VEC4(y, z, z, x)
	WILL_AMP_DECL_VEC4(y, z, z, y)
	WILL_AMP_DECL_VEC4(y, z, z, z)
	WILL_AMP_DECL_VEC4(z, x, x, x)
	WILL_AMP_DECL_VEC4(z, x, x, y)
	WILL_AMP_DECL_VEC4(z, x, x, z)
	WILL_AMP_DECL_VEC4(z, x, y, x)
	WILL_AMP_DECL_VEC4(z, x, y, y)
	WILL_AMP_DECL_VEC4(z, x, y, z)
	WILL_AMP_DECL_VEC4(z, x, z, x)
	WILL_AMP_DECL_VEC4(z, x, z, y)
	WILL_AMP_DECL_VEC4(z, x, z, z)
	WILL_AMP_DECL_VEC4(z, y, x, x)
	WILL_AMP_DECL_VEC4(z, y, x, y)
	WILL_AMP_DECL_VEC4(z, y, x, z)
	WILL_AMP_DECL_VEC4(z, y, y, x)
	WILL_AMP_DECL_VEC4(z, y, y, y)
	WILL_AMP_DECL_VEC4(z, y, y, z)
	WILL_AMP_DECL_VEC4(z, y, z, x)
	WILL_AMP_DECL_VEC4(z, y, z, y)
	WILL_AMP_DECL_VEC4(z, y, z, z)
	WILL_AMP_DECL_VEC4(z, z, x, x)
	WILL_AMP_DECL_VEC4(z, z, x, y)
	WILL_AMP_DECL_VEC4(z, z, x, z)
	WILL_AMP_DECL_VEC4(z, z, y, x)
	WILL_AMP_DECL_VEC4(z, z, y, y)
	WILL_AMP_DECL_VEC4(z, z, y, z)
	WILL_AMP_DECL_VEC4(z, z, z, x)
	WILL_AMP_DECL_VEC4(z, z, z, y)
	WILL_AMP_DECL_VEC4(z, z, z, z)
	WILL_AMP_DECL_VEC2(r, r)
	WILL_AMP_DECL_VEC2(g, g)
	WILL_AMP_DECL_VEC2(b, b)
	WILL_AMP_DECL_VEC3(r, r, r)
	WILL_AMP_DECL_VEC3(r, r, g)
	WILL_AMP_DECL_VEC3(r, r, b)
	WILL_AMP_DECL_VEC3(r, g, r)
	WILL_AMP_DECL_VEC3(r, g, g)
	WILL_AMP_DECL_VEC3(r, b, r)
	WILL_AMP_DECL_VEC3(r, b, b)
	WILL_AMP_DECL_VEC3(g, r, r)
	WILL_AMP_DECL_VEC3(g, r, g)
	WILL_AMP_DECL_VEC3(g, g, r)
	WILL_AMP_DECL_VEC3(g, g, g)
	WILL_AMP_DECL_VEC3(g, g, b)
	WILL_AMP_DECL_VEC3(g, b, g)
	WILL_AMP_DECL_VEC3(g, b, b)
	WILL_AMP_DECL_VEC3(b, r, r)
	WILL_AMP_DECL_VEC3(b, r, b)
	WILL_AMP_DECL_VEC3(b, g, g)
	WILL_AMP_DECL_VEC3(b, g, b)
	WILL_AMP_DECL_VEC3(b, b, r)
	WILL_AMP_DECL_VEC3(b, b, g)
	WILL_AMP_DECL_VEC3(b, b, b)
	WILL_AMP_DECL_VEC4(r, r, r, r)
	WILL_AMP_DECL_VEC4(r, r, r, g)
	WILL_AMP_DECL_VEC4(r, r, r, b)
	WILL_AMP_DECL_VEC4(r, r, g, r)
	WILL_AMP_DECL_VEC4(r, r, g, g)
	WILL_AMP_DECL_VEC4(r, r, g, b)
	WILL_AMP_DECL_VEC4(r, r, b, r)
	WILL_AMP_DECL_VEC4(r, r, b, g)
	WILL_AMP_DECL_VEC4(r, r, b, b)
	WILL_AMP_DECL_VEC4(r, g, r, r)
	WILL_AMP_DECL_VEC4(r, g, r, g)
	WILL_AMP_DECL_VEC4(r, g, r, b)
	WILL_AMP_DECL_VEC4(r, g, g, r)
	WILL_AMP_DECL_VEC4(r, g, g, g)
	WILL_AMP_DECL_VEC4(r, g, g, b)
	WILL_AMP_DECL_VEC4(r, g, b, r)
	WILL_AMP_DECL_VEC4(r, g, b, g)
	WILL_AMP_DECL_VEC4(r, g, b, b)
	WILL_AMP_DECL_VEC4(r, b, r, r)
	WILL_AMP_DECL_VEC4(r, b, r, g)
	WILL_AMP_DECL_VEC4(r, b, r, b)
	WILL_AMP_DECL_VEC4(r, b, g, r)
	WILL_AMP_DECL_VEC4(r, b, g, g)
	WILL_AMP_DECL_VEC4(r, b, g, b)
	WILL_AMP_DECL_VEC4(r, b, b, r)
	WILL_AMP_DECL_VEC4(r, b, b, g)
	WILL_AMP_DECL_VEC4(r, b, b, b)
	WILL_AMP_DECL_VEC4(g, r, r, r)
	WILL_AMP_DECL_VEC4(g, r, r, g)
	WILL_AMP_DECL_VEC4(g, r, r, b)
	WILL_AMP_DECL_VEC4(g, r, g, r)
	WILL_AMP_DECL_VEC4(g, r, g, g)
	WILL_AMP_DECL_VEC4(g, r, g, b)
	WILL_AMP_DECL_VEC4(g, r, b, r)
	WILL_AMP_DECL_VEC4(g, r, b, g)
	WILL_AMP_DECL_VEC4(g, r, b, b)
	WILL_AMP_DECL_VEC4(g, g, r, r)
	WILL_AMP_DECL_VEC4(g, g, r, g)
	WILL_AMP_DECL_VEC4(g, g, r, b)
	WILL_AMP_DECL_VEC4(g, g, g, r)
	WILL_AMP_DECL_VEC4(g, g, g, g)
	WILL_AMP_DECL_VEC4(g, g, g, b)
	WILL_AMP_DECL_VEC4(g, g, b, r)
	WILL_AMP_DECL_VEC4(g, g, b, g)
	WILL_AMP_DECL_VEC4(g, g, b, b)
	WILL_AMP_DECL_VEC4(g, b, r, r)
	WILL_AMP_DECL_VEC4(g, b, r, g)
	WILL_AMP_DECL_VEC4(g, b, r, b)
	WILL_AMP_DECL_VEC4(g, b, g, r)
	WILL_AMP_DECL_VEC4(g, b, g, g)
	WILL_AMP_DECL_VEC4(g, b, g, b)
	WILL_AMP_DECL_VEC4(g, b, b, r)
	WILL_AMP_DECL_VEC4(g, b, b, g)
	WILL_AMP_DECL_VEC4(g, b, b, b)
	WILL_AMP_DECL_VEC4(b, r, r, r)
	WILL_AMP_DECL_VEC4(b, r, r, g)
	WILL_AMP_DECL_VEC4(b, r, r, b)
	WILL_AMP_DECL_VEC4(b, r, g, r)
	WILL_AMP_DECL_VEC4(b, r, g, g)
	WILL_AMP_DECL_VEC4(b, r, g, b)
	WILL_AMP_DECL_VEC4(b, r, b, r)
	WILL_AMP_DECL_VEC4(b, r, b, g)
	WILL_AMP_DECL_VEC4(b, r, b, b)
	WILL_AMP_DECL_VEC4(b, g, r, r)
	WILL_AMP_DECL_VEC4(b, g, r, g)
	WILL_AMP_DECL_VEC4(b, g, r, b)
	WILL_AMP_DECL_VEC4(b, g, g, r)
	WILL_AMP_DECL_VEC4(b, g, g, g)
	WILL_AMP_DECL_VEC4(b, g, g, b)
	WILL_AMP_DECL_VEC4(b, g, b, r)
	WILL_AMP_DECL_VEC4(b, g, b, g)
	WILL_AMP_DECL_VEC4(b, g, b, b)
	WILL_AMP_DECL_VEC4(b, b, r, r)
	WILL_AMP_DECL_VEC4(b, b, r, g)
	WILL_AMP_DECL_VEC4(b, b, r, b)
	WILL_AMP_DECL_VEC4(b, b, g, r)
	WILL_AMP_DECL_VEC4(b, b, g, g)
	WILL_AMP_DECL_VEC4(b, b, g, b)
	WILL_AMP_DECL_VEC4(b, b, b, r)
	WILL_AMP_DECL_VEC4(b, b, b, g)
	WILL_AMP_DECL_VEC4(b, b, b, b)
};
template<typename T>
class vec<T, 4> : public detail::original_short_vector<T, 4>::type{
	using base_type = typename detail::original_short_vector<T, 4>::type;
public:
	using base_type::base_type;
	vec()restrict(cpu, amp):base_type{}{}
	vec(const vec& other)restrict(cpu, amp):base_type{static_cast<const base_type&>(other)}{}
	vec(vec&& other)restrict(cpu, amp):base_type{static_cast<base_type&&>(other)}{}
	vec& operator=(const vec& other)restrict(cpu, amp){*static_cast<base_type*>(this) = static_cast<const base_type&>(other);return *this;}
	vec& operator=(vec&& other)restrict(cpu, amp){*static_cast<base_type*>(this) = static_cast<base_type&&>(other);return *this;}
	template<typename U, std::enable_if_t<short_vector_traits<U>::size == 1>* = nullptr>
	vec(U _1)restrict(cpu, amp):vec{static_cast<T>(_1)}{}
	template<typename U, typename... Args>
	vec(U&& u, Args&&... args)restrict(cpu, amp):base_type{detail::vec_mat_construct<base_type, base_type::size>::create(forward<U>(u), forward<Args>(args)...)}{}
	T& operator[](unsigned int n)restrict(cpu, amp){
		switch(n%4){
		case 0:
			return ref_x();
		case 1:
			return ref_y();
		case 2:
			return ref_z();
		case 3:
			return ref_w();
		}
	}
	T operator[](unsigned int n)const restrict(cpu, amp){
		switch(n%4){
		case 0:
			return get_x();
		case 1:
			return get_y();
		case 2:
			return get_z();
		case 3:
			return get_w();
		}
	}
	WILL_AMP_DECL_VEC2(x, x)
	WILL_AMP_DECL_VEC2(y, y)
	WILL_AMP_DECL_VEC2(z, z)
	WILL_AMP_DECL_VEC2(w, w)
	WILL_AMP_DECL_VEC3(x, x, x)
	WILL_AMP_DECL_VEC3(x, x, y)
	WILL_AMP_DECL_VEC3(x, x, z)
	WILL_AMP_DECL_VEC3(x, x, w)
	WILL_AMP_DECL_VEC3(x, y, x)
	WILL_AMP_DECL_VEC3(x, y, y)
	WILL_AMP_DECL_VEC3(x, z, x)
	WILL_AMP_DECL_VEC3(x, z, z)
	WILL_AMP_DECL_VEC3(x, w, x)
	WILL_AMP_DECL_VEC3(x, w, w)
	WILL_AMP_DECL_VEC3(y, x, x)
	WILL_AMP_DECL_VEC3(y, x, y)
	WILL_AMP_DECL_VEC3(y, y, x)
	WILL_AMP_DECL_VEC3(y, y, y)
	WILL_AMP_DECL_VEC3(y, y, z)
	WILL_AMP_DECL_VEC3(y, y, w)
	WILL_AMP_DECL_VEC3(y, z, y)
	WILL_AMP_DECL_VEC3(y, z, z)
	WILL_AMP_DECL_VEC3(y, w, y)
	WILL_AMP_DECL_VEC3(y, w, w)
	WILL_AMP_DECL_VEC3(z, x, x)
	WILL_AMP_DECL_VEC3(z, x, z)
	WILL_AMP_DECL_VEC3(z, y, y)
	WILL_AMP_DECL_VEC3(z, y, z)
	WILL_AMP_DECL_VEC3(z, z, x)
	WILL_AMP_DECL_VEC3(z, z, y)
	WILL_AMP_DECL_VEC3(z, z, z)
	WILL_AMP_DECL_VEC3(z, z, w)
	WILL_AMP_DECL_VEC3(z, w, z)
	WILL_AMP_DECL_VEC3(z, w, w)
	WILL_AMP_DECL_VEC3(w, x, x)
	WILL_AMP_DECL_VEC3(w, x, w)
	WILL_AMP_DECL_VEC3(w, y, y)
	WILL_AMP_DECL_VEC3(w, y, w)
	WILL_AMP_DECL_VEC3(w, z, z)
	WILL_AMP_DECL_VEC3(w, z, w)
	WILL_AMP_DECL_VEC3(w, w, x)
	WILL_AMP_DECL_VEC3(w, w, y)
	WILL_AMP_DECL_VEC3(w, w, z)
	WILL_AMP_DECL_VEC3(w, w, w)
	WILL_AMP_DECL_VEC4(x, x, x, x)
	WILL_AMP_DECL_VEC4(x, x, x, y)
	WILL_AMP_DECL_VEC4(x, x, x, z)
	WILL_AMP_DECL_VEC4(x, x, x, w)
	WILL_AMP_DECL_VEC4(x, x, y, x)
	WILL_AMP_DECL_VEC4(x, x, y, y)
	WILL_AMP_DECL_VEC4(x, x, y, z)
	WILL_AMP_DECL_VEC4(x, x, y, w)
	WILL_AMP_DECL_VEC4(x, x, z, x)
	WILL_AMP_DECL_VEC4(x, x, z, y)
	WILL_AMP_DECL_VEC4(x, x, z, z)
	WILL_AMP_DECL_VEC4(x, x, z, w)
	WILL_AMP_DECL_VEC4(x, x, w, x)
	WILL_AMP_DECL_VEC4(x, x, w, y)
	WILL_AMP_DECL_VEC4(x, x, w, z)
	WILL_AMP_DECL_VEC4(x, x, w, w)
	WILL_AMP_DECL_VEC4(x, y, x, x)
	WILL_AMP_DECL_VEC4(x, y, x, y)
	WILL_AMP_DECL_VEC4(x, y, x, z)
	WILL_AMP_DECL_VEC4(x, y, x, w)
	WILL_AMP_DECL_VEC4(x, y, y, x)
	WILL_AMP_DECL_VEC4(x, y, y, y)
	WILL_AMP_DECL_VEC4(x, y, y, z)
	WILL_AMP_DECL_VEC4(x, y, y, w)
	WILL_AMP_DECL_VEC4(x, y, z, x)
	WILL_AMP_DECL_VEC4(x, y, z, y)
	WILL_AMP_DECL_VEC4(x, y, z, z)
	WILL_AMP_DECL_VEC4(x, y, w, x)
	WILL_AMP_DECL_VEC4(x, y, w, y)
	WILL_AMP_DECL_VEC4(x, y, w, w)
	WILL_AMP_DECL_VEC4(x, z, x, x)
	WILL_AMP_DECL_VEC4(x, z, x, y)
	WILL_AMP_DECL_VEC4(x, z, x, z)
	WILL_AMP_DECL_VEC4(x, z, x, w)
	WILL_AMP_DECL_VEC4(x, z, y, x)
	WILL_AMP_DECL_VEC4(x, z, y, y)
	WILL_AMP_DECL_VEC4(x, z, y, z)
	WILL_AMP_DECL_VEC4(x, z, z, x)
	WILL_AMP_DECL_VEC4(x, z, z, y)
	WILL_AMP_DECL_VEC4(x, z, z, z)
	WILL_AMP_DECL_VEC4(x, z, z, w)
	WILL_AMP_DECL_VEC4(x, z, w, x)
	WILL_AMP_DECL_VEC4(x, z, w, z)
	WILL_AMP_DECL_VEC4(x, z, w, w)
	WILL_AMP_DECL_VEC4(x, w, x, x)
	WILL_AMP_DECL_VEC4(x, w, x, y)
	WILL_AMP_DECL_VEC4(x, w, x, z)
	WILL_AMP_DECL_VEC4(x, w, x, w)
	WILL_AMP_DECL_VEC4(x, w, y, x)
	WILL_AMP_DECL_VEC4(x, w, y, y)
	WILL_AMP_DECL_VEC4(x, w, y, w)
	WILL_AMP_DECL_VEC4(x, w, z, x)
	WILL_AMP_DECL_VEC4(x, w, z, z)
	WILL_AMP_DECL_VEC4(x, w, z, w)
	WILL_AMP_DECL_VEC4(x, w, w, x)
	WILL_AMP_DECL_VEC4(x, w, w, y)
	WILL_AMP_DECL_VEC4(x, w, w, z)
	WILL_AMP_DECL_VEC4(x, w, w, w)
	WILL_AMP_DECL_VEC4(y, x, x, x)
	WILL_AMP_DECL_VEC4(y, x, x, y)
	WILL_AMP_DECL_VEC4(y, x, x, z)
	WILL_AMP_DECL_VEC4(y, x, x, w)
	WILL_AMP_DECL_VEC4(y, x, y, x)
	WILL_AMP_DECL_VEC4(y, x, y, y)
	WILL_AMP_DECL_VEC4(y, x, y, z)
	WILL_AMP_DECL_VEC4(y, x, y, w)
	WILL_AMP_DECL_VEC4(y, x, z, x)
	WILL_AMP_DECL_VEC4(y, x, z, y)
	WILL_AMP_DECL_VEC4(y, x, z, z)
	WILL_AMP_DECL_VEC4(y, x, w, x)
	WILL_AMP_DECL_VEC4(y, x, w, y)
	WILL_AMP_DECL_VEC4(y, x, w, w)
	WILL_AMP_DECL_VEC4(y, y, x, x)
	WILL_AMP_DECL_VEC4(y, y, x, y)
	WILL_AMP_DECL_VEC4(y, y, x, z)
	WILL_AMP_DECL_VEC4(y, y, x, w)
	WILL_AMP_DECL_VEC4(y, y, y, x)
	WILL_AMP_DECL_VEC4(y, y, y, y)
	WILL_AMP_DECL_VEC4(y, y, y, z)
	WILL_AMP_DECL_VEC4(y, y, y, w)
	WILL_AMP_DECL_VEC4(y, y, z, x)
	WILL_AMP_DECL_VEC4(y, y, z, y)
	WILL_AMP_DECL_VEC4(y, y, z, z)
	WILL_AMP_DECL_VEC4(y, y, z, w)
	WILL_AMP_DECL_VEC4(y, y, w, x)
	WILL_AMP_DECL_VEC4(y, y, w, y)
	WILL_AMP_DECL_VEC4(y, y, w, z)
	WILL_AMP_DECL_VEC4(y, y, w, w)
	WILL_AMP_DECL_VEC4(y, z, x, x)
	WILL_AMP_DECL_VEC4(y, z, x, y)
	WILL_AMP_DECL_VEC4(y, z, x, z)
	WILL_AMP_DECL_VEC4(y, z, y, x)
	WILL_AMP_DECL_VEC4(y, z, y, y)
	WILL_AMP_DECL_VEC4(y, z, y, z)
	WILL_AMP_DECL_VEC4(y, z, y, w)
	WILL_AMP_DECL_VEC4(y, z, z, x)
	WILL_AMP_DECL_VEC4(y, z, z, y)
	WILL_AMP_DECL_VEC4(y, z, z, z)
	WILL_AMP_DECL_VEC4(y, z, z, w)
	WILL_AMP_DECL_VEC4(y, z, w, y)
	WILL_AMP_DECL_VEC4(y, z, w, z)
	WILL_AMP_DECL_VEC4(y, z, w, w)
	WILL_AMP_DECL_VEC4(y, w, x, x)
	WILL_AMP_DECL_VEC4(y, w, x, y)
	WILL_AMP_DECL_VEC4(y, w, x, w)
	WILL_AMP_DECL_VEC4(y, w, y, x)
	WILL_AMP_DECL_VEC4(y, w, y, y)
	WILL_AMP_DECL_VEC4(y, w, y, z)
	WILL_AMP_DECL_VEC4(y, w, y, w)
	WILL_AMP_DECL_VEC4(y, w, z, y)
	WILL_AMP_DECL_VEC4(y, w, z, z)
	WILL_AMP_DECL_VEC4(y, w, z, w)
	WILL_AMP_DECL_VEC4(y, w, w, x)
	WILL_AMP_DECL_VEC4(y, w, w, y)
	WILL_AMP_DECL_VEC4(y, w, w, z)
	WILL_AMP_DECL_VEC4(y, w, w, w)
	WILL_AMP_DECL_VEC4(z, x, x, x)
	WILL_AMP_DECL_VEC4(z, x, x, y)
	WILL_AMP_DECL_VEC4(z, x, x, z)
	WILL_AMP_DECL_VEC4(z, x, x, w)
	WILL_AMP_DECL_VEC4(z, x, y, x)
	WILL_AMP_DECL_VEC4(z, x, y, y)
	WILL_AMP_DECL_VEC4(z, x, y, z)
	WILL_AMP_DECL_VEC4(z, x, z, x)
	WILL_AMP_DECL_VEC4(z, x, z, y)
	WILL_AMP_DECL_VEC4(z, x, z, z)
	WILL_AMP_DECL_VEC4(z, x, z, w)
	WILL_AMP_DECL_VEC4(z, x, w, x)
	WILL_AMP_DECL_VEC4(z, x, w, z)
	WILL_AMP_DECL_VEC4(z, x, w, w)
	WILL_AMP_DECL_VEC4(z, y, x, x)
	WILL_AMP_DECL_VEC4(z, y, x, y)
	WILL_AMP_DECL_VEC4(z, y, x, z)
	WILL_AMP_DECL_VEC4(z, y, y, x)
	WILL_AMP_DECL_VEC4(z, y, y, y)
	WILL_AMP_DECL_VEC4(z, y, y, z)
	WILL_AMP_DECL_VEC4(z, y, y, w)
	WILL_AMP_DECL_VEC4(z, y, z, x)
	WILL_AMP_DECL_VEC4(z, y, z, y)
	WILL_AMP_DECL_VEC4(z, y, z, z)
	WILL_AMP_DECL_VEC4(z, y, z, w)
	WILL_AMP_DECL_VEC4(z, y, w, y)
	WILL_AMP_DECL_VEC4(z, y, w, z)
	WILL_AMP_DECL_VEC4(z, y, w, w)
	WILL_AMP_DECL_VEC4(z, z, x, x)
	WILL_AMP_DECL_VEC4(z, z, x, y)
	WILL_AMP_DECL_VEC4(z, z, x, z)
	WILL_AMP_DECL_VEC4(z, z, x, w)
	WILL_AMP_DECL_VEC4(z, z, y, x)
	WILL_AMP_DECL_VEC4(z, z, y, y)
	WILL_AMP_DECL_VEC4(z, z, y, z)
	WILL_AMP_DECL_VEC4(z, z, y, w)
	WILL_AMP_DECL_VEC4(z, z, z, x)
	WILL_AMP_DECL_VEC4(z, z, z, y)
	WILL_AMP_DECL_VEC4(z, z, z, z)
	WILL_AMP_DECL_VEC4(z, z, z, w)
	WILL_AMP_DECL_VEC4(z, z, w, x)
	WILL_AMP_DECL_VEC4(z, z, w, y)
	WILL_AMP_DECL_VEC4(z, z, w, z)
	WILL_AMP_DECL_VEC4(z, z, w, w)
	WILL_AMP_DECL_VEC4(z, w, x, x)
	WILL_AMP_DECL_VEC4(z, w, x, z)
	WILL_AMP_DECL_VEC4(z, w, x, w)
	WILL_AMP_DECL_VEC4(z, w, y, y)
	WILL_AMP_DECL_VEC4(z, w, y, z)
	WILL_AMP_DECL_VEC4(z, w, y, w)
	WILL_AMP_DECL_VEC4(z, w, z, x)
	WILL_AMP_DECL_VEC4(z, w, z, y)
	WILL_AMP_DECL_VEC4(z, w, z, z)
	WILL_AMP_DECL_VEC4(z, w, z, w)
	WILL_AMP_DECL_VEC4(z, w, w, x)
	WILL_AMP_DECL_VEC4(z, w, w, y)
	WILL_AMP_DECL_VEC4(z, w, w, z)
	WILL_AMP_DECL_VEC4(z, w, w, w)
	WILL_AMP_DECL_VEC4(w, x, x, x)
	WILL_AMP_DECL_VEC4(w, x, x, y)
	WILL_AMP_DECL_VEC4(w, x, x, z)
	WILL_AMP_DECL_VEC4(w, x, x, w)
	WILL_AMP_DECL_VEC4(w, x, y, x)
	WILL_AMP_DECL_VEC4(w, x, y, y)
	WILL_AMP_DECL_VEC4(w, x, y, w)
	WILL_AMP_DECL_VEC4(w, x, z, x)
	WILL_AMP_DECL_VEC4(w, x, z, z)
	WILL_AMP_DECL_VEC4(w, x, z, w)
	WILL_AMP_DECL_VEC4(w, x, w, x)
	WILL_AMP_DECL_VEC4(w, x, w, y)
	WILL_AMP_DECL_VEC4(w, x, w, z)
	WILL_AMP_DECL_VEC4(w, x, w, w)
	WILL_AMP_DECL_VEC4(w, y, x, x)
	WILL_AMP_DECL_VEC4(w, y, x, y)
	WILL_AMP_DECL_VEC4(w, y, x, w)
	WILL_AMP_DECL_VEC4(w, y, y, x)
	WILL_AMP_DECL_VEC4(w, y, y, y)
	WILL_AMP_DECL_VEC4(w, y, y, z)
	WILL_AMP_DECL_VEC4(w, y, y, w)
	WILL_AMP_DECL_VEC4(w, y, z, y)
	WILL_AMP_DECL_VEC4(w, y, z, z)
	WILL_AMP_DECL_VEC4(w, y, z, w)
	WILL_AMP_DECL_VEC4(w, y, w, x)
	WILL_AMP_DECL_VEC4(w, y, w, y)
	WILL_AMP_DECL_VEC4(w, y, w, z)
	WILL_AMP_DECL_VEC4(w, y, w, w)
	WILL_AMP_DECL_VEC4(w, z, x, x)
	WILL_AMP_DECL_VEC4(w, z, x, z)
	WILL_AMP_DECL_VEC4(w, z, x, w)
	WILL_AMP_DECL_VEC4(w, z, y, y)
	WILL_AMP_DECL_VEC4(w, z, y, z)
	WILL_AMP_DECL_VEC4(w, z, y, w)
	WILL_AMP_DECL_VEC4(w, z, z, x)
	WILL_AMP_DECL_VEC4(w, z, z, y)
	WILL_AMP_DECL_VEC4(w, z, z, z)
	WILL_AMP_DECL_VEC4(w, z, z, w)
	WILL_AMP_DECL_VEC4(w, z, w, x)
	WILL_AMP_DECL_VEC4(w, z, w, y)
	WILL_AMP_DECL_VEC4(w, z, w, z)
	WILL_AMP_DECL_VEC4(w, z, w, w)
	WILL_AMP_DECL_VEC4(w, w, x, x)
	WILL_AMP_DECL_VEC4(w, w, x, y)
	WILL_AMP_DECL_VEC4(w, w, x, z)
	WILL_AMP_DECL_VEC4(w, w, x, w)
	WILL_AMP_DECL_VEC4(w, w, y, x)
	WILL_AMP_DECL_VEC4(w, w, y, y)
	WILL_AMP_DECL_VEC4(w, w, y, z)
	WILL_AMP_DECL_VEC4(w, w, y, w)
	WILL_AMP_DECL_VEC4(w, w, z, x)
	WILL_AMP_DECL_VEC4(w, w, z, y)
	WILL_AMP_DECL_VEC4(w, w, z, z)
	WILL_AMP_DECL_VEC4(w, w, z, w)
	WILL_AMP_DECL_VEC4(w, w, w, x)
	WILL_AMP_DECL_VEC4(w, w, w, y)
	WILL_AMP_DECL_VEC4(w, w, w, z)
	WILL_AMP_DECL_VEC4(w, w, w, w)
	WILL_AMP_DECL_VEC2(r, r)
	WILL_AMP_DECL_VEC2(g, g)
	WILL_AMP_DECL_VEC2(b, b)
	WILL_AMP_DECL_VEC2(a, a)
	WILL_AMP_DECL_VEC3(r, r, r)
	WILL_AMP_DECL_VEC3(r, r, g)
	WILL_AMP_DECL_VEC3(r, r, b)
	WILL_AMP_DECL_VEC3(r, r, a)
	WILL_AMP_DECL_VEC3(r, g, r)
	WILL_AMP_DECL_VEC3(r, g, g)
	WILL_AMP_DECL_VEC3(r, b, r)
	WILL_AMP_DECL_VEC3(r, b, b)
	WILL_AMP_DECL_VEC3(r, a, r)
	WILL_AMP_DECL_VEC3(r, a, a)
	WILL_AMP_DECL_VEC3(g, r, r)
	WILL_AMP_DECL_VEC3(g, r, g)
	WILL_AMP_DECL_VEC3(g, g, r)
	WILL_AMP_DECL_VEC3(g, g, g)
	WILL_AMP_DECL_VEC3(g, g, b)
	WILL_AMP_DECL_VEC3(g, g, a)
	WILL_AMP_DECL_VEC3(g, b, g)
	WILL_AMP_DECL_VEC3(g, b, b)
	WILL_AMP_DECL_VEC3(g, a, g)
	WILL_AMP_DECL_VEC3(g, a, a)
	WILL_AMP_DECL_VEC3(b, r, r)
	WILL_AMP_DECL_VEC3(b, r, b)
	WILL_AMP_DECL_VEC3(b, g, g)
	WILL_AMP_DECL_VEC3(b, g, b)
	WILL_AMP_DECL_VEC3(b, b, r)
	WILL_AMP_DECL_VEC3(b, b, g)
	WILL_AMP_DECL_VEC3(b, b, b)
	WILL_AMP_DECL_VEC3(b, b, a)
	WILL_AMP_DECL_VEC3(b, a, b)
	WILL_AMP_DECL_VEC3(b, a, a)
	WILL_AMP_DECL_VEC3(a, r, r)
	WILL_AMP_DECL_VEC3(a, r, a)
	WILL_AMP_DECL_VEC3(a, g, g)
	WILL_AMP_DECL_VEC3(a, g, a)
	WILL_AMP_DECL_VEC3(a, b, b)
	WILL_AMP_DECL_VEC3(a, b, a)
	WILL_AMP_DECL_VEC3(a, a, r)
	WILL_AMP_DECL_VEC3(a, a, g)
	WILL_AMP_DECL_VEC3(a, a, b)
	WILL_AMP_DECL_VEC3(a, a, a)
	WILL_AMP_DECL_VEC4(r, r, r, r)
	WILL_AMP_DECL_VEC4(r, r, r, g)
	WILL_AMP_DECL_VEC4(r, r, r, b)
	WILL_AMP_DECL_VEC4(r, r, r, a)
	WILL_AMP_DECL_VEC4(r, r, g, r)
	WILL_AMP_DECL_VEC4(r, r, g, g)
	WILL_AMP_DECL_VEC4(r, r, g, b)
	WILL_AMP_DECL_VEC4(r, r, g, a)
	WILL_AMP_DECL_VEC4(r, r, b, r)
	WILL_AMP_DECL_VEC4(r, r, b, g)
	WILL_AMP_DECL_VEC4(r, r, b, b)
	WILL_AMP_DECL_VEC4(r, r, b, a)
	WILL_AMP_DECL_VEC4(r, r, a, r)
	WILL_AMP_DECL_VEC4(r, r, a, g)
	WILL_AMP_DECL_VEC4(r, r, a, b)
	WILL_AMP_DECL_VEC4(r, r, a, a)
	WILL_AMP_DECL_VEC4(r, g, r, r)
	WILL_AMP_DECL_VEC4(r, g, r, g)
	WILL_AMP_DECL_VEC4(r, g, r, b)
	WILL_AMP_DECL_VEC4(r, g, r, a)
	WILL_AMP_DECL_VEC4(r, g, g, r)
	WILL_AMP_DECL_VEC4(r, g, g, g)
	WILL_AMP_DECL_VEC4(r, g, g, b)
	WILL_AMP_DECL_VEC4(r, g, g, a)
	WILL_AMP_DECL_VEC4(r, g, b, r)
	WILL_AMP_DECL_VEC4(r, g, b, g)
	WILL_AMP_DECL_VEC4(r, g, b, b)
	WILL_AMP_DECL_VEC4(r, g, a, r)
	WILL_AMP_DECL_VEC4(r, g, a, g)
	WILL_AMP_DECL_VEC4(r, g, a, a)
	WILL_AMP_DECL_VEC4(r, b, r, r)
	WILL_AMP_DECL_VEC4(r, b, r, g)
	WILL_AMP_DECL_VEC4(r, b, r, b)
	WILL_AMP_DECL_VEC4(r, b, r, a)
	WILL_AMP_DECL_VEC4(r, b, g, r)
	WILL_AMP_DECL_VEC4(r, b, g, g)
	WILL_AMP_DECL_VEC4(r, b, g, b)
	WILL_AMP_DECL_VEC4(r, b, b, r)
	WILL_AMP_DECL_VEC4(r, b, b, g)
	WILL_AMP_DECL_VEC4(r, b, b, b)
	WILL_AMP_DECL_VEC4(r, b, b, a)
	WILL_AMP_DECL_VEC4(r, b, a, r)
	WILL_AMP_DECL_VEC4(r, b, a, b)
	WILL_AMP_DECL_VEC4(r, b, a, a)
	WILL_AMP_DECL_VEC4(r, a, r, r)
	WILL_AMP_DECL_VEC4(r, a, r, g)
	WILL_AMP_DECL_VEC4(r, a, r, b)
	WILL_AMP_DECL_VEC4(r, a, r, a)
	WILL_AMP_DECL_VEC4(r, a, g, r)
	WILL_AMP_DECL_VEC4(r, a, g, g)
	WILL_AMP_DECL_VEC4(r, a, g, a)
	WILL_AMP_DECL_VEC4(r, a, b, r)
	WILL_AMP_DECL_VEC4(r, a, b, b)
	WILL_AMP_DECL_VEC4(r, a, b, a)
	WILL_AMP_DECL_VEC4(r, a, a, r)
	WILL_AMP_DECL_VEC4(r, a, a, g)
	WILL_AMP_DECL_VEC4(r, a, a, b)
	WILL_AMP_DECL_VEC4(r, a, a, a)
	WILL_AMP_DECL_VEC4(g, r, r, r)
	WILL_AMP_DECL_VEC4(g, r, r, g)
	WILL_AMP_DECL_VEC4(g, r, r, b)
	WILL_AMP_DECL_VEC4(g, r, r, a)
	WILL_AMP_DECL_VEC4(g, r, g, r)
	WILL_AMP_DECL_VEC4(g, r, g, g)
	WILL_AMP_DECL_VEC4(g, r, g, b)
	WILL_AMP_DECL_VEC4(g, r, g, a)
	WILL_AMP_DECL_VEC4(g, r, b, r)
	WILL_AMP_DECL_VEC4(g, r, b, g)
	WILL_AMP_DECL_VEC4(g, r, b, b)
	WILL_AMP_DECL_VEC4(g, r, a, r)
	WILL_AMP_DECL_VEC4(g, r, a, g)
	WILL_AMP_DECL_VEC4(g, r, a, a)
	WILL_AMP_DECL_VEC4(g, g, r, r)
	WILL_AMP_DECL_VEC4(g, g, r, g)
	WILL_AMP_DECL_VEC4(g, g, r, b)
	WILL_AMP_DECL_VEC4(g, g, r, a)
	WILL_AMP_DECL_VEC4(g, g, g, r)
	WILL_AMP_DECL_VEC4(g, g, g, g)
	WILL_AMP_DECL_VEC4(g, g, g, b)
	WILL_AMP_DECL_VEC4(g, g, g, a)
	WILL_AMP_DECL_VEC4(g, g, b, r)
	WILL_AMP_DECL_VEC4(g, g, b, g)
	WILL_AMP_DECL_VEC4(g, g, b, b)
	WILL_AMP_DECL_VEC4(g, g, b, a)
	WILL_AMP_DECL_VEC4(g, g, a, r)
	WILL_AMP_DECL_VEC4(g, g, a, g)
	WILL_AMP_DECL_VEC4(g, g, a, b)
	WILL_AMP_DECL_VEC4(g, g, a, a)
	WILL_AMP_DECL_VEC4(g, b, r, r)
	WILL_AMP_DECL_VEC4(g, b, r, g)
	WILL_AMP_DECL_VEC4(g, b, r, b)
	WILL_AMP_DECL_VEC4(g, b, g, r)
	WILL_AMP_DECL_VEC4(g, b, g, g)
	WILL_AMP_DECL_VEC4(g, b, g, b)
	WILL_AMP_DECL_VEC4(g, b, g, a)
	WILL_AMP_DECL_VEC4(g, b, b, r)
	WILL_AMP_DECL_VEC4(g, b, b, g)
	WILL_AMP_DECL_VEC4(g, b, b, b)
	WILL_AMP_DECL_VEC4(g, b, b, a)
	WILL_AMP_DECL_VEC4(g, b, a, g)
	WILL_AMP_DECL_VEC4(g, b, a, b)
	WILL_AMP_DECL_VEC4(g, b, a, a)
	WILL_AMP_DECL_VEC4(g, a, r, r)
	WILL_AMP_DECL_VEC4(g, a, r, g)
	WILL_AMP_DECL_VEC4(g, a, r, a)
	WILL_AMP_DECL_VEC4(g, a, g, r)
	WILL_AMP_DECL_VEC4(g, a, g, g)
	WILL_AMP_DECL_VEC4(g, a, g, b)
	WILL_AMP_DECL_VEC4(g, a, g, a)
	WILL_AMP_DECL_VEC4(g, a, b, g)
	WILL_AMP_DECL_VEC4(g, a, b, b)
	WILL_AMP_DECL_VEC4(g, a, b, a)
	WILL_AMP_DECL_VEC4(g, a, a, r)
	WILL_AMP_DECL_VEC4(g, a, a, g)
	WILL_AMP_DECL_VEC4(g, a, a, b)
	WILL_AMP_DECL_VEC4(g, a, a, a)
	WILL_AMP_DECL_VEC4(b, r, r, r)
	WILL_AMP_DECL_VEC4(b, r, r, g)
	WILL_AMP_DECL_VEC4(b, r, r, b)
	WILL_AMP_DECL_VEC4(b, r, r, a)
	WILL_AMP_DECL_VEC4(b, r, g, r)
	WILL_AMP_DECL_VEC4(b, r, g, g)
	WILL_AMP_DECL_VEC4(b, r, g, b)
	WILL_AMP_DECL_VEC4(b, r, b, r)
	WILL_AMP_DECL_VEC4(b, r, b, g)
	WILL_AMP_DECL_VEC4(b, r, b, b)
	WILL_AMP_DECL_VEC4(b, r, b, a)
	WILL_AMP_DECL_VEC4(b, r, a, r)
	WILL_AMP_DECL_VEC4(b, r, a, b)
	WILL_AMP_DECL_VEC4(b, r, a, a)
	WILL_AMP_DECL_VEC4(b, g, r, r)
	WILL_AMP_DECL_VEC4(b, g, r, g)
	WILL_AMP_DECL_VEC4(b, g, r, b)
	WILL_AMP_DECL_VEC4(b, g, g, r)
	WILL_AMP_DECL_VEC4(b, g, g, g)
	WILL_AMP_DECL_VEC4(b, g, g, b)
	WILL_AMP_DECL_VEC4(b, g, g, a)
	WILL_AMP_DECL_VEC4(b, g, b, r)
	WILL_AMP_DECL_VEC4(b, g, b, g)
	WILL_AMP_DECL_VEC4(b, g, b, b)
	WILL_AMP_DECL_VEC4(b, g, b, a)
	WILL_AMP_DECL_VEC4(b, g, a, g)
	WILL_AMP_DECL_VEC4(b, g, a, b)
	WILL_AMP_DECL_VEC4(b, g, a, a)
	WILL_AMP_DECL_VEC4(b, b, r, r)
	WILL_AMP_DECL_VEC4(b, b, r, g)
	WILL_AMP_DECL_VEC4(b, b, r, b)
	WILL_AMP_DECL_VEC4(b, b, r, a)
	WILL_AMP_DECL_VEC4(b, b, g, r)
	WILL_AMP_DECL_VEC4(b, b, g, g)
	WILL_AMP_DECL_VEC4(b, b, g, b)
	WILL_AMP_DECL_VEC4(b, b, g, a)
	WILL_AMP_DECL_VEC4(b, b, b, r)
	WILL_AMP_DECL_VEC4(b, b, b, g)
	WILL_AMP_DECL_VEC4(b, b, b, b)
	WILL_AMP_DECL_VEC4(b, b, b, a)
	WILL_AMP_DECL_VEC4(b, b, a, r)
	WILL_AMP_DECL_VEC4(b, b, a, g)
	WILL_AMP_DECL_VEC4(b, b, a, b)
	WILL_AMP_DECL_VEC4(b, b, a, a)
	WILL_AMP_DECL_VEC4(b, a, r, r)
	WILL_AMP_DECL_VEC4(b, a, r, b)
	WILL_AMP_DECL_VEC4(b, a, r, a)
	WILL_AMP_DECL_VEC4(b, a, g, g)
	WILL_AMP_DECL_VEC4(b, a, g, b)
	WILL_AMP_DECL_VEC4(b, a, g, a)
	WILL_AMP_DECL_VEC4(b, a, b, r)
	WILL_AMP_DECL_VEC4(b, a, b, g)
	WILL_AMP_DECL_VEC4(b, a, b, b)
	WILL_AMP_DECL_VEC4(b, a, b, a)
	WILL_AMP_DECL_VEC4(b, a, a, r)
	WILL_AMP_DECL_VEC4(b, a, a, g)
	WILL_AMP_DECL_VEC4(b, a, a, b)
	WILL_AMP_DECL_VEC4(b, a, a, a)
	WILL_AMP_DECL_VEC4(a, r, r, r)
	WILL_AMP_DECL_VEC4(a, r, r, g)
	WILL_AMP_DECL_VEC4(a, r, r, b)
	WILL_AMP_DECL_VEC4(a, r, r, a)
	WILL_AMP_DECL_VEC4(a, r, g, r)
	WILL_AMP_DECL_VEC4(a, r, g, g)
	WILL_AMP_DECL_VEC4(a, r, g, a)
	WILL_AMP_DECL_VEC4(a, r, b, r)
	WILL_AMP_DECL_VEC4(a, r, b, b)
	WILL_AMP_DECL_VEC4(a, r, b, a)
	WILL_AMP_DECL_VEC4(a, r, a, r)
	WILL_AMP_DECL_VEC4(a, r, a, g)
	WILL_AMP_DECL_VEC4(a, r, a, b)
	WILL_AMP_DECL_VEC4(a, r, a, a)
	WILL_AMP_DECL_VEC4(a, g, r, r)
	WILL_AMP_DECL_VEC4(a, g, r, g)
	WILL_AMP_DECL_VEC4(a, g, r, a)
	WILL_AMP_DECL_VEC4(a, g, g, r)
	WILL_AMP_DECL_VEC4(a, g, g, g)
	WILL_AMP_DECL_VEC4(a, g, g, b)
	WILL_AMP_DECL_VEC4(a, g, g, a)
	WILL_AMP_DECL_VEC4(a, g, b, g)
	WILL_AMP_DECL_VEC4(a, g, b, b)
	WILL_AMP_DECL_VEC4(a, g, b, a)
	WILL_AMP_DECL_VEC4(a, g, a, r)
	WILL_AMP_DECL_VEC4(a, g, a, g)
	WILL_AMP_DECL_VEC4(a, g, a, b)
	WILL_AMP_DECL_VEC4(a, g, a, a)
	WILL_AMP_DECL_VEC4(a, b, r, r)
	WILL_AMP_DECL_VEC4(a, b, r, b)
	WILL_AMP_DECL_VEC4(a, b, r, a)
	WILL_AMP_DECL_VEC4(a, b, g, g)
	WILL_AMP_DECL_VEC4(a, b, g, b)
	WILL_AMP_DECL_VEC4(a, b, g, a)
	WILL_AMP_DECL_VEC4(a, b, b, r)
	WILL_AMP_DECL_VEC4(a, b, b, g)
	WILL_AMP_DECL_VEC4(a, b, b, b)
	WILL_AMP_DECL_VEC4(a, b, b, a)
	WILL_AMP_DECL_VEC4(a, b, a, r)
	WILL_AMP_DECL_VEC4(a, b, a, g)
	WILL_AMP_DECL_VEC4(a, b, a, b)
	WILL_AMP_DECL_VEC4(a, b, a, a)
	WILL_AMP_DECL_VEC4(a, a, r, r)
	WILL_AMP_DECL_VEC4(a, a, r, g)
	WILL_AMP_DECL_VEC4(a, a, r, b)
	WILL_AMP_DECL_VEC4(a, a, r, a)
	WILL_AMP_DECL_VEC4(a, a, g, r)
	WILL_AMP_DECL_VEC4(a, a, g, g)
	WILL_AMP_DECL_VEC4(a, a, g, b)
	WILL_AMP_DECL_VEC4(a, a, g, a)
	WILL_AMP_DECL_VEC4(a, a, b, r)
	WILL_AMP_DECL_VEC4(a, a, b, g)
	WILL_AMP_DECL_VEC4(a, a, b, b)
	WILL_AMP_DECL_VEC4(a, a, b, a)
	WILL_AMP_DECL_VEC4(a, a, a, r)
	WILL_AMP_DECL_VEC4(a, a, a, g)
	WILL_AMP_DECL_VEC4(a, a, a, b)
	WILL_AMP_DECL_VEC4(a, a, a, a)
};
#undef WILL_AMP_DECL_VEC4
#undef WILL_AMP_DECL_VEC3
#undef WILL_AMP_DECL_VEC2

using double2 = vec<double, 2>;
using double3 = vec<double, 3>;
using double4 = vec<double, 4>;
using float2 = vec<float, 2>;
using float3 = vec<float, 3>;
using float4 = vec<float, 4>;
using int2 = vec<int, 2>;
using int3 = vec<int, 3>;
using int4 = vec<int, 4>;
using norm2 = vec<norm, 2>;
using norm3 = vec<norm, 3>;
using norm4 = vec<norm, 4>;
using uint2 = vec<uint, 2>;
using uint3 = vec<uint, 3>;
using uint4 = vec<uint, 4>;
using unorm2 = vec<unorm, 2>;
using unorm3 = vec<unorm, 3>;
using unorm4 = vec<unorm, 4>;

template<typename, int> struct short_vector;
template<> struct short_vector<double, 1>{using type = double ;};
template<> struct short_vector<double, 2>{using type = double2;};
template<> struct short_vector<double, 3>{using type = double3;};
template<> struct short_vector<double, 4>{using type = double4;};
template<> struct short_vector<float,  1>{using type =  float ;};
template<> struct short_vector<float,  2>{using type =  float2;};
template<> struct short_vector<float,  3>{using type =  float3;};
template<> struct short_vector<float,  4>{using type =  float4;};
template<> struct short_vector<int,    1>{using type =    int ;};
template<> struct short_vector<int,    2>{using type =    int2;};
template<> struct short_vector<int,    3>{using type =    int3;};
template<> struct short_vector<int,    4>{using type =    int4;};
template<> struct short_vector<norm,   1>{using type =   norm ;};
template<> struct short_vector<norm,   2>{using type =   norm2;};
template<> struct short_vector<norm,   3>{using type =   norm3;};
template<> struct short_vector<norm,   4>{using type =   norm4;};
template<> struct short_vector<uint,   1>{using type =   uint ;};
template<> struct short_vector<uint,   2>{using type =   uint2;};
template<> struct short_vector<uint,   3>{using type =   uint3;};
template<> struct short_vector<uint,   4>{using type =   uint4;};
template<> struct short_vector<unorm,  1>{using type =  unorm ;};
template<> struct short_vector<unorm,  2>{using type =  unorm2;};
template<> struct short_vector<unorm,  3>{using type =  unorm3;};
template<> struct short_vector<unorm,  4>{using type =  unorm4;};

}

namespace graphics{

template<typename T, int N>
struct short_vector_traits<amp::shader::vec<T, N>>{
	using value_type = T;
	static constexpr int size = N;
};

namespace detail{

template<typename T, int Dim, int Rank>
struct texture<amp::shader::vec<T, Dim>, Rank>{using type = concurrency::graphics::texture<typename amp::shader::detail::original_short_vector<T, Dim>::type, Rank>;};
template<typename T, int Dim, int Rank>
struct texture<const amp::shader::vec<T, Dim>, Rank>{using type = concurrency::graphics::texture<const typename amp::shader::detail::original_short_vector<T, Dim>::type, Rank>;};
template<typename T, int Dim, int Rank>
struct texture_view<amp::shader::vec<T, Dim>, Rank>{using type = concurrency::graphics::texture_view<typename amp::shader::detail::original_short_vector<T, Dim>::type, Rank>;};
template<typename T, int Dim, int Rank>
struct texture_view<const amp::shader::vec<T, Dim>, Rank>{using type = concurrency::graphics::texture_view<const typename amp::shader::detail::original_short_vector<T, Dim>::type, Rank>;};

}

}

namespace shader{

template<typename F, typename T, typename... Args, std::enable_if_t<detail::is_same_size_short_vectors<1, T, Args...>::value>* = nullptr>
auto map(F&& f, T t, Args... args)restrict(cpu, amp){return f(t, args...);}
template<typename F, typename T, typename... Args, std::enable_if_t<detail::is_same_size_short_vectors<2, T, Args...>::value>* = nullptr>
auto map(F&& f, T&& t, Args&&... args)restrict(cpu, amp){return T{f(t.x, args.x...), f(t.y, args.y...)};}
template<typename F, typename T, typename... Args, std::enable_if_t<detail::is_same_size_short_vectors<3, T, Args...>::value>* = nullptr>
auto map(F&& f, T&& t, Args&&... args)restrict(cpu, amp){return T{f(t.x, args.x...), f(t.y, args.y...), f(t.z, args.z...)};}
template<typename F, typename T, typename... Args, std::enable_if_t<detail::is_same_size_short_vectors<4, T, Args...>::value>* = nullptr>
auto map(F&& f, T&& t, Args&&... args)restrict(cpu, amp){return T{f(t.x, args.x...), f(t.y, args.y...), f(t.z, args.z...), f(t.w, args.w...)};}
template<typename F, typename T, std::enable_if_t<short_vector_traits<std::decay_t<T>>::size == 1>* = nullptr>
auto reduce(F&&, T t)restrict(cpu, amp)->decltype(t){return t;}
template<typename F, typename T, std::enable_if_t<short_vector_traits<std::decay_t<T>>::size == 2>* = nullptr>
auto reduce(F&& f, T&& t)restrict(cpu, amp)->decltype(f(t.x, t.y)){return f(t.x, t.y);}
template<typename F, typename T, std::enable_if_t<short_vector_traits<std::decay_t<T>>::size == 3>* = nullptr>
auto reduce(F&& f, T&& t)restrict(cpu, amp)->decltype(f(f(t.x, t.y), t.z)){return f(f(t.x, t.y), t.z);}
template<typename F, typename T, std::enable_if_t<short_vector_traits<std::decay_t<T>>::size == 4>* = nullptr>
auto reduce(F&& f, T&& t)restrict(cpu, amp)->decltype(f(f(f(t.x, t.y), t.z), t.w)){return f(f(f(t.x, t.y), t.z), t.w);}

using vec2 = vec<float, 2>;
using vec3 = vec<float, 3>;
using vec4 = vec<float, 4>;

inline float2 get_coord(const amp::index<2>& idx)restrict(cpu, amp){return {static_cast<float>(idx[1]), static_cast<float>(idx[0])};}
inline float2 get_coord(const amp::index<2>& idx, const float2& resolution)restrict(cpu, amp){return {static_cast<float>(idx[1]), resolution.y - idx[0]};}
inline float2 get_position(const float2& coord, const float2& resolution)restrict(cpu, amp){return (coord.xy * 2.f - resolution) / math::min(resolution.x, resolution.y);}
inline float2 get_position(const amp::index<2>& idx, const float2& resolution)restrict(cpu, amp){return get_position(get_coord(idx), resolution);}

template<typename T, typename U, std::enable_if_t<(short_vector_traits<T>::size >= 2) && (short_vector_traits<U>::size == 1)>* = nullptr>
inline T operator*(const T& t, U u)restrict(cpu, amp){return map([u](typename short_vector_traits<T>::value_type v)restrict(cpu, amp){return v*u;}, t);}
template<typename T, typename U, std::enable_if_t<(short_vector_traits<U>::size >= 2) && (short_vector_traits<T>::size == 1)>* = nullptr>
inline U operator*(T t, const U& u)restrict(cpu, amp){return map([t](typename short_vector_traits<U>::value_type v)restrict(cpu, amp){return t*v;}, u);}

template<typename T, typename U, std::enable_if_t<(short_vector_traits<T>::size >= 2) && (short_vector_traits<U>::size == 1)>* = nullptr>
inline T operator/(const T& t, U u)restrict(cpu, amp){return map([u](typename short_vector_traits<T>::value_type v)restrict(cpu, amp){return v/u;}, t);}
template<typename T, typename U, std::enable_if_t<(short_vector_traits<U>::size >= 2) && (short_vector_traits<T>::size == 1)>* = nullptr>
inline U operator/(T t, const U& u)restrict(cpu, amp){return map([t](typename short_vector_traits<U>::value_type v)restrict(cpu, amp){return t/v;}, u);}

inline float length(float x)restrict(cpu, amp){return math::abs(x);}
template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline auto length(const T& vec)restrict(cpu, amp){
	using vt = typename short_vector_traits<T>::value_type;
	return reduce([](vt a, vt b)restrict(cpu, amp){return hypot(a, b);}, vec);
}

template<typename T, typename U, std::enable_if_t<short_vector_traits<T>::size == short_vector_traits<U>::size>* = nullptr>
inline auto distance(const T& a, const U& b)restrict(cpu, amp){
	return length(a - b);
}

inline float normalize(float)restrict(cpu, amp){
	return 1.f;
}
template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T normalize(const T& vec)restrict(cpu, amp){
	if(length(vec) < math::detail::numeric_limits<typename short_vector_traits<T>::value_type>::epsilon())
		return vec / math::detail::numeric_limits<typename short_vector_traits<T>::value_type>::epsilon();
	else
		return vec / length(vec);
}

template<typename T, typename U, std::enable_if_t<short_vector_traits<T>::size == short_vector_traits<U>::size>* = nullptr>
inline auto dot(const T& lhs, const U& rhs)restrict(cpu, amp){
	using vt = typename short_vector_traits<T>::value_type;
	return reduce([](vt a, vt b)restrict(cpu, amp){return a + b;}, map([](vt lhs, vt rhs)restrict(cpu, amp){return lhs * rhs;}, lhs, rhs));
}

template<typename T, typename U, std::enable_if_t<short_vector_traits<T>::size == 3 && short_vector_traits<U>::size == 3>* = nullptr>
inline T cross(const T& lhs, const U& rhs)restrict(cpu, amp){
	return T{lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x};
}

template<typename T, typename U, std::enable_if_t<short_vector_traits<T>::size == short_vector_traits<U>::size>* = nullptr>
inline auto reflect(const T& i, const U& n)restrict(cpu, amp){return i - 2.f * dot(n, i) * n;}

//i and n should be normalized
template<typename T, typename U, std::enable_if_t<short_vector_traits<T>::size == short_vector_traits<U>::size>* = nullptr>
inline auto refract(const T& i, const U& n, float eta)restrict(cpu, amp){
	const auto k = 1.f - eta * eta * (1.f - dot(n, i) * dot(n, i));
	return k < 0.f ? 0.f : eta * i - (eta * dot(n, i) * math::sqrt(k))*n;
}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T abs(const T& t)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::abs(x);}, t);}

template<typename T, typename U, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T pow(const T& t, const U& u)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x, typename short_vector_traits<U>::value_type y)restrict(cpu, amp){return math::pow(x, y);}, t, u);}

template<typename T, typename U, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<T>::size == short_vector_traits<U>::size>* = nullptr>
inline T mod(const T& t, const U& u)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x, typename short_vector_traits<U>::value_type y)restrict(cpu, amp){return math::mod(x, y);}, t, u);}
template<typename T, typename U, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<std::decay_t<U>>::size == 1>* = nullptr>
inline T mod(const T& t, U s)restrict(cpu, amp){return map([s](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::mod(x, s);}, t);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T floor(const T& t)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::floor(x);}, t);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T ceil(const T& t)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::ceil(x);}, t);}

template<typename T>
inline T fract(const T& t)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return x - math::floor(x);}, t);}

template<typename T, typename U, typename V, std::enable_if_t<short_vector_traits<T>::size == short_vector_traits<U>::size && short_vector_traits<U>::size == short_vector_traits<V>::size>* = nullptr>
inline T mix(const T& t, const U& u, const V& v)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x, typename short_vector_traits<U>::value_type y, typename short_vector_traits<V>::value_type z)restrict(cpu, amp){return x * (1.f-z) + y * z;}, t, u, v);}
template<typename T, typename U, typename V, std::enable_if_t<(short_vector_traits<T>::size >= 2) && (short_vector_traits<T>::size == short_vector_traits<U>::size) && short_vector_traits<V>::size == 1>* = nullptr>
inline T mix(const T& t, const U& u, V v)restrict(cpu, amp){return map([v](typename short_vector_traits<T>::value_type x, typename short_vector_traits<U>::value_type y)restrict(cpu, amp){return mix(x, y, v);}, t, u);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T sin(const T& t)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::sin(x);}, t);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T cos(const T& t)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::cos(x);}, t);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T tan(const T& t)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::tan(x);}, t);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T asin(const T& t)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::asin(x);}, t);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T acos(const T& t)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::acos(x);}, t);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T atan(const T& t)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::atan(x);}, t);}
template<typename T, typename U, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<T>::size == short_vector_traits<U>::size>* = nullptr>
inline T atan(const T& t, const U& u)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type y, typename short_vector_traits<U>::value_type x)restrict(cpu, amp){return math::atan(y, x);}, t, u);}

template<typename T, typename U, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<T>::size == short_vector_traits<U>::size>* = nullptr>
inline T min(const T& t, const U& u)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x, typename short_vector_traits<U>::value_type y)restrict(cpu, amp){return math::min(x, y);}, t, u);}
template<typename T, typename U, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<U>::size == 1>* = nullptr>
inline T min(const T& t, U s)restrict(cpu, amp){return map([s](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::min(x, s);}, t);}

template<typename T, typename U, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<T>::size == short_vector_traits<U>::size>* = nullptr>
inline T max(const T& t, const U& u)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x, typename short_vector_traits<U>::value_type y)restrict(cpu, amp){return math::max(x, y);}, t, u);}
template<typename T, typename U, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<U>::size == 1>* = nullptr>
inline T max(const T& t, U s)restrict(cpu, amp){return map([s](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::max(x, s);}, t);}

template<typename T, typename U, typename V, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<T>::size == short_vector_traits<U>::size && short_vector_traits<U>::size == short_vector_traits<V>::size>* = nullptr>
inline T clamp(const T& t, const U& min, const V& max)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x, typename short_vector_traits<U>::value_type y, typename short_vector_traits<V>::value_type z)restrict(cpu, amp){return math::clamp(x, y, z);}, t, min, max);}
template<typename T, typename U, typename V, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<U>::size == 1 && short_vector_traits<V>::size == 1>* = nullptr>
inline T clamp(const T& t, U min, V max)restrict(cpu, amp){return map([min, max](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::clamp(x, min, max);}, t);}

template<typename T, typename U, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<T>::size == short_vector_traits<U>::size>* = nullptr>
inline T step(const T& t, const U& u)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x, typename short_vector_traits<U>::value_type y)restrict(cpu, amp){return math::step(x, y);}, t, u);}
template<typename T, typename U, std::enable_if_t<short_vector_traits<T>::size == 1 && (short_vector_traits<U>::size >= 2)>* = nullptr>
inline U step(T s, const U& u)restrict(cpu, amp){return map([s](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::step(s, x);}, u);}

template<typename T, typename U, typename V, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<T>::size == short_vector_traits<U>::size && short_vector_traits<U>::size == short_vector_traits<V>::size>* = nullptr>
inline T smoothstep(const T& min, const U& max, const V& v)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x, typename short_vector_traits<U>::value_type y, typename short_vector_traits<V>::value_type z)restrict(cpu, amp){return math::smoothstep(x, y, z);}, min, max, v);}
template<typename T, typename U, typename V, std::enable_if_t<short_vector_traits<T>::size == 1 && short_vector_traits<U>::size == 1 && (short_vector_traits<V>::size >= 2)>* = nullptr>
inline V smoothstep(T min, U max, const V& v)restrict(cpu, amp){return map([min, max](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::smoothstep(min, max, x);}, v);}

template<typename T, typename U, typename V, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<T>::size == short_vector_traits<U>::size && short_vector_traits<U>::size == short_vector_traits<V>::size>* = nullptr>
inline T lerp(const T& a, const U& b, const V& w)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x, typename short_vector_traits<U>::value_type y, typename short_vector_traits<V>::value_type z)restrict(cpu, amp){return math::lerp(x, y, z);}, a, b, w);}
template<typename T, typename U, typename V, std::enable_if_t<(short_vector_traits<T>::size >= 2) && short_vector_traits<T>::size == short_vector_traits<U>::size && short_vector_traits<V>::size == 1>* = nullptr>
inline T lerp(const T& a, const U& b, V w)restrict(cpu, amp){return map([w](typename short_vector_traits<T>::value_type x, typename short_vector_traits<U>::value_type y)restrict(cpu, amp){return math::lerp(x, y, w);}, a, b);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T sqrt(const T& a)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::sqrt(x);}, a);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T radians(const T& a)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::radians(x);}, a);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T degrees(const T& a)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::degrees(x);}, a);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T exp(const T& a)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::exp(x);}, a);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T log(const T& a)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::log(x);}, a);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T exp2(const T& a)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::exp2(x);}, a);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T log2(const T& a)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::log2(x);}, a);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T inversesqrt(const T& a)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::inversesqrt(x);}, a);}

template<typename T, std::enable_if_t<(short_vector_traits<T>::size >= 2)>* = nullptr>
inline T sign(const T& a)restrict(cpu, amp){return map([](typename short_vector_traits<T>::value_type x)restrict(cpu, amp){return math::sign(x);}, a);}

template<typename T, typename U, typename V, std::enable_if_t<short_vector_traits<T>::size == short_vector_traits<U>::size && short_vector_traits<U>::size == short_vector_traits<V>::size>* = nullptr>
inline auto faceforward(const T& n, const U& i, const V& n_ref)restrict(cpu, amp){return dot(i, n_ref) < 0 ? n : -n;}

template<typename F, typename G>
class connector{
	alignas(4) F f;
	alignas(4) G g;
public:
	template<typename T, typename U>
	connector(T&& t, U&& u)restrict(cpu, amp) : f(amp::forward<T>(t)), g(amp::forward<U>(u)){}
	template<typename T, typename... Args>
	auto operator()(T&& t, Args&&... args)const restrict(amp){return g(f(amp::forward<T>(t), amp::forward<Args>(args)...), amp::forward<Args>(args)...);}
	template<typename T, typename... Args>
	auto operator()(T&& t, Args&&... args)restrict(amp){return g(f(amp::forward<T>(t), amp::forward<Args>(args)...), amp::forward<Args>(args)...);}
	template<typename N>
	connector<connector<F, G>, std::decay_t<N>> operator|(N&& n)const{return {*this, amp::forward<N>(n)};}
};

namespace detail{

template<typename>struct is_connector : std::false_type{};
template<typename F, typename G>
struct is_connector<connector<F, G>> : std::true_type{};

template<typename F>
class shader{
	alignas(4) F f;
public:
	template<typename T>
	shader(T&& t)restrict(cpu, amp) : f(amp::forward<T>(t)){}
	template<typename... Args>
	auto operator()(Args&&... args)const restrict(amp){return f(amp::forward<Args>(args)...);}
	template<typename... Args>
	auto operator()(Args&&... args)restrict(amp){return f(amp::forward<Args>(args)...);}
	template<typename N>
	connector<F, std::decay_t<N>> operator|(N&& n)const{return {f, amp::forward<N>(n)};}
	template<typename N, std::enable_if_t<!is_connector<std::decay_t<N>>::value, std::nullptr_t> = nullptr>
	friend connector<std::decay_t<N>, F> operator|(N&& n, const shader<F>& f){return {amp::forward<N>(n), f.f};}
};

template<typename F>
struct is_connector<shader<F>> : std::true_type{};

template<typename ValueType, int Rank>
class reader_from_cv{
	texture_view<const ValueType, Rank> texv;
public:
	reader_from_cv(const concurrency::graphics::texture_view<const ValueType, Rank>& tv):texv{tv}{}
	template<typename T, typename U, typename... Args>
	auto operator()(T&&, U&&, index<2> idx, Args&&... unused)const restrict(amp){return texv[idx];}
};

}

template<typename F>
inline detail::shader<std::decay_t<F>> shader(F&& f){return {amp::forward<F>(f)};}

static const auto to_bgra = shader([](auto t, auto&&... unused)restrict(amp){return t.bgra;});
static const auto straighten = shader([](auto t, auto&&... unused)restrict(amp){using v = vec<typename decltype(t)::value_type, decltype(t)::size>;return t.a == 0 ? v{1, 1, 1, 0} : v{t.rgb/t.a, t.a};});
static const auto premultiply = shader([](auto t, auto&&... unused)restrict(amp){return vec<typename decltype(t)::value_type, decltype(t)::size>{t.rgb * t.a, t.a};});
static inline auto gamma_correction(float f = 2.2f){
	return shader([g = 1.f/f](auto t, auto&&... unused)restrict(amp){return pow(t, vec<float, decltype(t)::size>{g});});
}
static const auto to_ldr = shader([](float_4 t, auto&&... unused)restrict(amp){return unorm_4{t};});

template<typename ValueType, int Rank>
inline detail::shader<detail::reader_from_cv<ValueType, Rank>> source(const concurrency::graphics::texture_view<const ValueType, Rank>& tv){return shader(detail::reader_from_cv<ValueType, Rank>{tv});}

template<typename ValueType, int Rank, typename F>
inline void apply(concurrency::graphics::texture_view<ValueType, Rank>& tex, F&& f){
	amp::parallel_for_each(tex.extent, [f, tex](amp::index<Rank> idx)restrict(amp){tex.set(idx, f(0, tex, idx));});
}
template<typename ValueType, int Rank, typename F>
inline void apply(accelerator_view& accv, concurrency::graphics::texture_view<ValueType, Rank>& tex, F&& f){
	amp::parallel_for_each(accv, tex.extent, [f, tex](amp::index<Rank> idx)restrict(amp){tex.set(idx, f(0, tex, idx));});
}

}

}

}
