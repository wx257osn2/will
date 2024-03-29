//Copyright (C) 2014-2020 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#pragma warning(push)
#pragma warning(disable: 5040)
#include<amp.h>
#pragma warning(pop)
#include"_expected.hpp"
#include"_2dim.hpp"

namespace will{

namespace amp{

using concurrency::index;
using concurrency::extent;
using concurrency::tile_barrier;
using concurrency::tiled_index;
using concurrency::tiled_extent;
using concurrency::array_view;
using concurrency::array;
using concurrency::completion_future;
using concurrency::all_memory_fence;
using concurrency::global_memory_fence;
using concurrency::tile_static_memory_fence;
using concurrency::access_type;
	using concurrency::access_type_none;
	using concurrency::access_type_read;
	using concurrency::access_type_write;
	using concurrency::access_type_read_write;
	using concurrency::access_type_auto;
using concurrency::queuing_mode;
	using concurrency::queuing_mode_immediate;
	using concurrency::queuing_mode_automatic;
using concurrency::accelerator;
using concurrency::accelerator_view;
using concurrency::copy;
using concurrency::copy_async;

inline int atomic_fetch_add(int& dest, int value)restrict(amp){return concurrency::atomic_fetch_add(&dest, value);}
inline unsigned int atomic_fetch_add(unsigned int& dest, unsigned int value)restrict(amp){return concurrency::atomic_fetch_add(&dest, value);}
using concurrency::atomic_fetch_add;
inline int atomic_fetch_sub(int& dest, int value)restrict(amp){return concurrency::atomic_fetch_sub(&dest, value);}
inline unsigned int atomic_fetch_sub(unsigned int& dest, unsigned int value)restrict(amp){return concurrency::atomic_fetch_sub(&dest, value);}
using concurrency::atomic_fetch_sub;
inline int atomic_fetch_inc(int& dest)restrict(amp){return concurrency::atomic_fetch_inc(&dest);}
inline unsigned int atomic_fetch_inc(unsigned int& dest)restrict(amp){return concurrency::atomic_fetch_inc(&dest);}
using concurrency::atomic_fetch_inc;
inline int atomic_fetch_dec(int& dest)restrict(amp){return concurrency::atomic_fetch_dec(&dest);}
inline unsigned int atomic_fetch_dec(unsigned int& dest)restrict(amp){return concurrency::atomic_fetch_dec(&dest);}
using concurrency::atomic_fetch_dec;
inline int atomic_exchange(int& dest, int value)restrict(amp){return concurrency::atomic_exchange(&dest, value);}
inline unsigned int atomic_exchange(unsigned int& dest, unsigned int value)restrict(amp){return concurrency::atomic_exchange(&dest, value);}
inline float atomic_exchange(float& dest, float value)restrict(amp){return concurrency::atomic_exchange(&dest, value);}
using concurrency::atomic_exchange;
inline bool atomic_compare_exchange(int& dest, int& expected_value, int value)restrict(amp){return concurrency::atomic_compare_exchange(&dest, &expected_value, value);}
inline bool atomic_compare_exchange(int& dest, int* expected_value, int value)restrict(amp){return concurrency::atomic_compare_exchange(&dest,  expected_value, value);}
inline bool atomic_compare_exchange(int* dest, int& expected_value, int value)restrict(amp){return concurrency::atomic_compare_exchange( dest, &expected_value, value);}
inline bool atomic_compare_exchange(unsigned int& dest, unsigned int& expected_value, unsigned int value)restrict(amp){return concurrency::atomic_compare_exchange(&dest, &expected_value, value);}
inline bool atomic_compare_exchange(unsigned int& dest, unsigned int* expected_value, unsigned int value)restrict(amp){return concurrency::atomic_compare_exchange(&dest,  expected_value, value);}
inline bool atomic_compare_exchange(unsigned int* dest, unsigned int& expected_value, unsigned int value)restrict(amp){return concurrency::atomic_compare_exchange( dest, &expected_value, value);}
using concurrency::atomic_compare_exchange;
inline int atomic_fetch_max(int& dest, int value)restrict(amp){return concurrency::atomic_fetch_max(&dest, value);}
inline unsigned int atomic_fetch_max(unsigned int& dest, unsigned int value)restrict(amp){return concurrency::atomic_fetch_max(&dest, value);}
using concurrency::atomic_fetch_max;
inline int atomic_fetch_min(int& dest, int value)restrict(amp){return concurrency::atomic_fetch_min(&dest, value);}
inline unsigned int atomic_fetch_min(unsigned int& dest, unsigned int value)restrict(amp){return concurrency::atomic_fetch_min(&dest, value);}
using concurrency::atomic_fetch_min;
inline int atomic_fetch_and(int& dest, int value)restrict(amp){return concurrency::atomic_fetch_and(&dest, value);}
inline unsigned int atomic_fetch_and(unsigned int& dest, unsigned int value)restrict(amp){return concurrency::atomic_fetch_and(&dest, value);}
using concurrency::atomic_fetch_and;
inline int atomic_fetch_or(int& dest, int value)restrict(amp){return concurrency::atomic_fetch_or(&dest, value);}
inline unsigned int atomic_fetch_or(unsigned int& dest, unsigned int value)restrict(amp){return concurrency::atomic_fetch_or(&dest, value);}
using concurrency::atomic_fetch_or;
inline int atomic_fetch_xor(int& dest, int value)restrict(amp){return concurrency::atomic_fetch_xor(&dest, value);}
inline unsigned int atomic_fetch_xor(unsigned int& dest, unsigned int value)restrict(amp){return concurrency::atomic_fetch_xor(&dest, value);}
using concurrency::atomic_fetch_xor;
using concurrency::parallel_for_each;

template<class T>
static constexpr T&& forward(typename std::remove_reference<T>::type& t)restrict(cpu, amp){
	return (static_cast<T&&>(t));
}
template<class T>
static constexpr T&& forward(typename std::remove_reference<T>::type&& t)restrict(cpu, amp){
	static_assert(!std::is_lvalue_reference<T>::value, "bad forward call");
	return (static_cast<T&&>(t));
}
template<class T>
constexpr typename std::remove_reference<T>::type&& move(T&& t)restrict(cpu, amp){
	return (static_cast<typename std::remove_reference<T>::type&&>(t));
}

template<typename T>
class optional{
	union{
		alignas(4) T t;
		alignas(4) bool dummy;
	};
	alignas(4) bool enable;
public:
	explicit constexpr optional()restrict(cpu, amp):dummy{}, enable{false}{}
	constexpr optional(const T& t)restrict(cpu, amp):t{t}, enable{true}{}
	constexpr optional(T&& t)restrict(cpu, amp):t{move(t)}, enable{true}{}
	optional(const optional<T>& t)restrict(cpu, amp):optional{}{(*this) = t;}
	optional(optional<T>&& t)restrict(cpu, amp):optional{}{(*this) = move(t);}
	template<typename U>
	constexpr optional(U&& u)restrict(cpu, amp):t{static_cast<T>(forward<U>(u))}, enable{true}{}
	template<typename U>
	constexpr optional(const optional<U>& t)restrict(cpu, amp):optional{}{(*this) = t;}
	template<typename U>
	constexpr optional(optional<U>&& t)restrict(cpu, amp):optional{}{(*this) = move(t);}
	constexpr bool has_value()const restrict(cpu, amp){return enable;}
	explicit constexpr operator bool()const restrict(cpu, amp){return enable;}
	constexpr T& operator*()restrict(cpu, amp){return t;}
	constexpr const T& operator*()const restrict(cpu, amp){return t;}
	constexpr T* operator->()restrict(cpu, amp){return &t;}
	constexpr const T* operator->()const restrict(cpu, amp){return &t;}
	template<typename U>
	constexpr T value_or(U&& u)const restrict(cpu, amp){
		if(enable)
			return t;
		else
			return static_cast<T>(forward<U>(u));
	}
	optional& operator=(const optional& rhs)restrict(cpu, amp){
		if(rhs.enable)
			t = rhs.t;
		else
			dummy = rhs.dummy;
		enable = rhs.enable;
		return *this;
	}
	optional& operator=(optional&& rhs)restrict(cpu, amp){
		if(rhs.enable)
			t = move(rhs.t);
		else
			dummy = rhs.dummy;
		enable = rhs.enable;
		return *this;
	}
	template<typename U>
	optional& operator=(U&& u)restrict(cpu, amp){
		t = static_cast<T>(forward<U>(u));
		enable = true;
		return *this;
	}
	template<typename U>
	optional& operator=(const optional<U>& rhs)restrict(cpu, amp){
		if(rhs.enable)
			t = rhs.t;
		else
			dummy = rhs.dummy;
		enable = rhs.enable;
		return *this;
	}
	template<typename U>
	optional& operator=(optional<U>&& rhs)restrict(cpu, amp){
		if(rhs.enable)
			t = move(rhs.t);
		else
			dummy = rhs.dummy;
		enable = rhs.enable;
		return *this;
	}
};

namespace detail{
template<typename... Ts>union types_storage;
template<typename T, typename U, typename... Ts>
union types_storage<T, U, Ts...>{
	alignas(4) bool dummy;
	alignas(4) T t;
	alignas(4) types_storage<U, Ts...> u;
};
template<typename T>
union types_storage<T>{
	alignas(4) bool dummy;
	alignas(4) T t;
};
template<typename T, typename U, typename V, typename... Ts>
inline const T& get(const types_storage<U, V, Ts...>& st)restrict(cpu, amp){
	if constexpr(std::is_same<T, U>::value)
		return st.t;
	else
		return get<T>(st.u);
}
template<typename T, typename U>
inline const T& get(const types_storage<U>& st)restrict(cpu, amp){
	if constexpr(std::is_same<T, U>::value)
		return st.t;
}
template<typename T, typename U, typename V, typename... Ts>
inline T& get(types_storage<U, V, Ts...>& st)restrict(cpu, amp){
	if constexpr(std::is_same<T, U>::value)
		return st.t;
	else
		return get<T>(st.u);
}
template<typename T, typename U>
inline T& get(types_storage<U>& st)restrict(cpu, amp){
	if constexpr(std::is_same<T, U>::value)
		return st.t;
}
template<typename T,std::make_signed_t<typename T::value_type>>struct make_integer_range_next;
template<typename I, I... Indices, std::make_signed_t<I> Next>
struct make_integer_range_next<std::integer_sequence<I, Indices...>, Next>{using type = std::integer_sequence<I, Indices..., (Indices+Next)...>;};
template<typename T,std::make_signed_t<typename T::value_type>,typename T::value_type>struct make_integer_range_next_;
template<typename I, I... Indices, std::make_signed_t<I> Next, I Tail>
struct make_integer_range_next_<std::integer_sequence<I, Indices...>, Next, Tail>{using type = std::integer_sequence<I, Indices..., (Indices+Next)..., Tail>;};
template<typename I,I,std::make_signed_t<I>,std::make_signed_t<I>,typename = void>struct make_integer_range_impl;
template<typename I, I Begin, std::make_signed_t<I> Step, std::make_signed_t<I> Next>
struct make_integer_range_impl<I, Begin, Step, Next, typename std::enable_if<(Next==0 || Next==1)>::type>{
	using type = typename std::conditional<Next==0, std::integer_sequence<I>, std::integer_sequence<I, Begin>>::type;
};
template<typename I, I Begin, std::make_signed_t<I> Step, std::make_signed_t<I> Next>
struct make_integer_range_impl<I, Begin, Step, Next, typename std::enable_if<(Next>1)>::type>{
	using type = typename std::conditional<Next % 2 == 1,
		typename make_integer_range_next_<
		typename make_integer_range_impl<I, Begin, Step, Next/2>::type,
		Next/2  * Step,
		Begin + (Next-1) * Step
		>::type,
		typename make_integer_range_next<
		typename make_integer_range_impl<I, Begin, Step, Next/2>::type,
		Next/2  * Step
		>::type
	>::type;
};
template<typename I, I Begin, I End, std::make_signed_t<I> Step = (Begin<End ? 1 : -1)>
using make_integer_range = typename make_integer_range_impl<I, Begin, Step, (End - Begin + (Step>0 ? Step-1 : Step+1)) / Step>::type;

template<typename...>struct type_tuple{};
template<typename>struct tuple_size_impl;
template<template<typename...>class T, typename... Ts>
struct tuple_size_impl<T<Ts...>>{static constexpr int value = sizeof...(Ts);};
template<typename T>
constexpr unsigned int tuple_size = tuple_size_impl<T>::value;
template<typename Types, typename T>
class find_unique_type_index{
	template<typename>struct impl_;
	template<template<typename...>class Ts, typename... As>
	struct impl_<Ts<As...>>{using type = std::integer_sequence<bool, std::is_same<As, T>::value...>;};
	template<typename,typename>struct impl;
	template<int A1, int A2, int... As, bool B1, bool B2, bool... Bs>
	struct impl<std::integer_sequence<int, A1, A2, As...>, std::integer_sequence<bool, B1, B2, Bs...>>{static constexpr int value = (B1 ? A1+1 : 0) + impl<std::integer_sequence<int, A2, As...>, std::integer_sequence<bool, B2, Bs...>>::value;};
	template<int A, bool B>
	struct impl<std::integer_sequence<int, A>, std::integer_sequence<bool, B>>{static constexpr int value = B ? A+1 : 0;};
	using A = make_integer_range<int, 0, tuple_size<Types>>;
	using B = typename impl_<Types>::type;
public:
	static constexpr std::make_signed_t<int> value = impl<A, B>::value - 1;
};
}

template<typename...>class variant;
template<>
class variant<>{
	static void copy(bool&, bool, int)restrict(cpu, amp){}
	static void move(bool&, bool, int)restrict(cpu, amp){}
};
template<typename T, typename... Ts>
class variant<T, Ts...>{
	alignas(4) detail::types_storage<T, Ts...> ts;
	alignas(4) int tag;
	static void copy(detail::types_storage<T, Ts...>& lhs, const detail::types_storage<T, Ts...>& rhs, int tag)restrict(cpu, amp){
		if(tag == 0)
			lhs.t = rhs.t;
		else
			variant<Ts...>::copy(lhs.u, rhs.u, tag-1);
	}
	static void move(detail::types_storage<T, Ts...>& lhs, detail::types_storage<T, Ts...>&& rhs, int tag)restrict(cpu, amp){
		if(tag == 0)
			lhs.t = move(rhs.t);
		else
			variant<Ts...>::move(lhs.u, move(rhs.u), tag-1);
	}
public:
	template<typename U, std::enable_if_t<std::disjunction<std::is_same<T, std::decay_t<U>>, std::is_same<Ts, std::decay_t<U>>...>::value, std::nullptr_t> = nullptr>
	constexpr variant(U&& u)restrict(cpu, amp):ts{false}, tag{detail::find_unique_type_index<detail::types_storage<T, Ts...>, std::decay_t<U>>::value}{
		detail::get<std::decay_t<U>>(ts) = forward<U>(u);
	}
	variant(const variant<T, Ts...>& t)restrict(cpu, amp):ts{t.ts}, tag{t.tag}{}
	variant(variant<T, Ts...>&& t)restrict(cpu, amp):ts{t.ts}, tag{t.tag}{}
	template<typename U>
	bool is()const restrict(cpu, amp){return tag == detail::find_unique_type_index<detail::types_storage<T, Ts...>, std::decay_t<U>>::value;}
	template<typename U>
	const U& get()const restrict(cpu, amp){
		return detail::get<U>(ts);
	}
	template<typename U>
	U& get()restrict(cpu, amp){
		return detail::get<U>(ts);
	}
	template<typename U, typename V>
	U get_or(V&& v)const restrict(cpu, amp){
		if(is<U>())
			return detail::get<U>(ts);
		else
			return static_cast<U>(forward<V>(v));
	}
	variant& operator=(const variant& rhs)restrict(cpu, amp){
		tag = rhs.tag;
		copy(ts, rhs.ts, tag);
		return *this;
	}
	variant& operator=(variant&& rhs)restrict(cpu, amp){
		tag = rhs.tag;
		move(ts, rhs.ts, tag);
		return *this;
	}
};

template<typename T, typename U>
struct pair{
	alignas(4) T first;
	alignas(4) U second;
	pair& operator=(const pair<T, U>& other)restrict(cpu, amp){
		first = other.first;
		second = other.second;
		return *this;
	}
	pair& operator=(pair<T, U>&& other)restrict(cpu, amp){
		first = amp::move(other.first);
		second = amp::move(other.second);
		return *this;
	}
	void swap(pair<T, U>& other)restrict(cpu, amp){
		pair<T, U> tmp = amp::move(*this);
		*this = amp::move(other);
		other = amp::move(tmp);
	}
	bool operator==(const pair<T, U>& rhs)restrict(amp, cpu){
		return first == rhs.first && second == rhs.second;
	}
	bool operator!=(const pair<T, U>& rhs)restrict(amp, cpu){
		return !(*this == rhs);
	}
	bool operator<(const pair<T, U>& rhs)restrict(amp, cpu){
		return first < rhs.first || (!(first > rhs.first) && second < rhs.second);
	}
	bool operator>(const pair<T, U>& rhs)restrict(amp, cpu){
		return rhs < *this;
	}
	bool operator<=(const pair<T, U>& rhs)restrict(amp, cpu){
		return !(rhs < *this);
	}
	bool operator>=(const pair<T, U>& rhs)restrict(amp, cpu){
		return !(*this < rhs);
	}
};

namespace detail{

template<typename>struct is_reference_wrapper : std::false_type{};
template<typename T>struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type{};

struct invoker{
	template<typename MemberFunctionPointer, typename Arg, typename... Args, std::enable_if_t<std::is_member_function_pointer<MemberFunctionPointer>::value && !is_reference_wrapper<std::decay_t<Arg>>::value && !std::is_pointer<std::decay_t<Arg>>::value, std::nullptr_t> = nullptr>
	static inline auto invoke(MemberFunctionPointer member_function_pointer, Arg&& object, Args&&... args)restrict(cpu, amp)->decltype((will::amp::forward<Arg>(object).*member_function_pointer)(will::amp::forward<Args>(args)...)){
		return (will::amp::forward<Arg>(object).*member_function_pointer)(will::amp::forward<Args>(args)...);
	}
	template<typename MemberFunctionPointer, typename Arg, typename... Args, std::enable_if_t<std::is_member_function_pointer<MemberFunctionPointer>::value && is_reference_wrapper<std::decay_t<Arg>>::value, std::nullptr_t> = nullptr>
	static inline auto invoke(MemberFunctionPointer member_function_pointer, Arg&& reference_wrapper, Args&&... args)restrict(cpu, amp)->decltype((will::amp::forward<Arg>(reference_wrapper).get().*member_function_pointer)(will::amp::forward<Args>(args)...)){
		return (will::amp::forward<Arg>(reference_wrapper).get().*member_function_pointer)(will::amp::forward<Args>(args)...);
	}
	template<typename MemberFunctionPointer, typename Arg, typename... Args, std::enable_if_t<std::is_member_function_pointer<MemberFunctionPointer>::value && std::is_pointer<std::decay_t<Arg>>::value, std::nullptr_t> = nullptr>
	static inline auto invoke(MemberFunctionPointer member_function_pointer, Arg&& pointer, Args&&... args)restrict(cpu, amp)-> decltype(((*will::amp::forward<Arg>(pointer)).*member_function_pointer)(will::amp::forward<Args>(args)...)){
		return ((*will::amp::forward<Arg>(pointer)).*member_function_pointer)(will::amp::forward<Args>(args)...);
	}
	template<typename MemberObjectPointer, typename Arg, std::enable_if_t<std::is_member_object_pointer<MemberObjectPointer>::value && !is_reference_wrapper<std::decay_t<Arg>>::value && !std::is_pointer<std::decay_t<Arg>>::value, std::nullptr_t> = nullptr>
	static inline auto invoke(MemberObjectPointer member_object_pointer, Arg&& object)restrict(cpu, amp)->decltype(will::amp::forward<Arg>(object).*member_object_pointer){
		return will::amp::forward<Arg>(object).*member_object_pointer;
	}
	template<typename MemberObjectPointer, typename Arg, std::enable_if_t<std::is_member_object_pointer<MemberObjectPointer>::value && is_reference_wrapper<std::decay_t<Arg>>::value, std::nullptr_t> = nullptr>
	static inline auto invoke(MemberObjectPointer member_object_pointer, Arg&& arg)restrict(cpu, amp)->decltype(will::amp::forward<Arg>(arg).get().*member_object_pointer){
		return will::amp::forward<Arg>(arg).get().*member_object_pointer;
	}
	template<typename MemberObjectPointer, typename Arg, std::enable_if_t<std::is_member_object_pointer<MemberObjectPointer>::value && std::is_pointer<std::decay_t<Arg>>::value, std::nullptr_t> = nullptr>
	static inline auto invoke(MemberObjectPointer member_object_pointer, Arg&& arg)restrict(cpu, amp)->decltype((*will::amp::forward<Arg>(arg)).*member_object_pointer){
		return (*will::amp::forward<Arg>(arg)).*member_object_pointer;
	}
	template<typename Callable, typename... Args, std::enable_if_t<!std::is_member_function_pointer<std::decay_t<Callable>>::value && !std::is_member_object_pointer<std::decay_t<Callable>>::value, std::nullptr_t> = nullptr>
	static inline auto invoke(Callable&& callable, Args&&... args)restrict(cpu, amp)->decltype(will::amp::forward<Callable>(callable)(will::amp::forward<Args>(args)...)){
		return will::amp::forward<Callable>(callable)(will::amp::forward<Args>(args)...);
	}
};

template<typename, typename...>struct invocable_impl : std::false_type{};
template<typename... Ts>struct invocable_impl<std::void_t<decltype(invoker::invoke(std::declval<Ts>()...))>, Ts...> : std::true_type{};

}

template<typename Callable, typename... Args>
inline auto invoke(Callable&& callable, Args&& ... args)restrict(cpu, amp)->decltype(detail::invoker::invoke(will::amp::forward<Callable>(callable), will::amp::forward<Args>(args)...)){
	return detail::invoker::invoke(will::amp::forward<Callable>(callable), will::amp::forward<Args>(args)...);
}

template<typename... Ts>
struct invocable : detail::invocable_impl<void, Ts...>{};
template<typename... Ts>
static constexpr bool invocable_v = invocable<Ts...>::value;
template<typename... Ts>
struct invoke_result{
	using type = decltype(will::amp::invoke(std::declval<Ts>()...));
};
template<typename... Ts>
using invoke_result_t = typename invoke_result<Ts...>::type;

template<typename M, typename T, int Rank = 1>
struct mapped_array_view : array_view<T, Rank>{
	alignas(4) M m;
	using value_type = invoke_result_t<M, T>;
	mapped_array_view(array_view<T, Rank> av) : array_view{av}{}
	mapped_array_view(array_view<T, Rank> av, const M& m) : array_view{av}, m{m}{}
	auto operator[](int i)const restrict(cpu, amp){
		if constexpr(Rank == 1)
			return m(array_view<T, Rank>::operator[](i));
		else
			return mapped_array_view<M, T, Rank-1>{array_view<T, Rank>::operator[](i), m};
	}
	auto operator[](const index<Rank>& ind)const restrict(cpu, amp)->decltype(std::declval<M>()(array_view<T, Rank>::operator[](ind))){
		return m(array_view<T, Rank>::operator[](ind));
	}
	auto operator()(int i)const restrict(cpu, amp){
		return (*this)[i];
	}
	auto operator()(const index<Rank>& ind)const restrict(cpu, amp)->decltype(operator[](ind)){
		return (*this)[ind];
	}
	auto operator()(int i0, int i1)const restrict(cpu, amp)->decltype(std::declval<M>()(array_view<T, Rank>::operator()(i0, i1))){
		return m(array_view<T, Rank>::operator()(i0, i1));
	}
	auto operator()(int i0, int i1, int i2)const restrict(cpu, amp)->decltype(std::declval<M>()(array_view<T, Rank>::operator()(i0, i1, i2))){
		return m(array_view<T, Rank>::operator()(i0, i1, i2));
	}
	mapped_array_view section(const index<Rank>& org, const extent<Rank>& ext)const restrict(cpu, amp){
		return mapped_array_view{array_view<T, Rank>::section(org, ext), m};
	}
	mapped_array_view section(const index<Rank>& org)const restrict(cpu, amp){
		return mapped_array_view{array_view<T, Rank>::section(org), m};
	}
	mapped_array_view section(const extent<Rank>& ext)const restrict(cpu, amp){
		return mapped_array_view{array_view<T, Rank>::section(ext), m};
	}
	mapped_array_view section(int org, int ext)const restrict(cpu, amp){
		return mapped_array_view{array_view<T, Rank>::section(org, ext), m};
	}
	mapped_array_view section(int o0, int o1, int e0, int e1)const restrict(cpu, amp){
		return mapped_array_view{array_view<T, Rank>::section(o0, o1, e0, e1), m};
	}
	mapped_array_view section(int o0, int o1, int o2, int e0, int e1, int e2)const restrict(cpu, amp){
		return mapped_array_view{array_view<T, Rank>::section(o0, o1, o2, e0, e1, e2), m};
	}
	template<typename U>
	mapped_array_view<M, U, Rank> reinterpret_as()const restrict(cpu, amp){
		return mapped_array_view<M, U, Rank>{array_view<T, Rank>::template reinterpret_as<U>(), m};
	}
	template<int NewRank>
	mapped_array_view<M, T, NewRank> view_as(const extent<NewRank>& ext)const restrict(cpu, amp){
		return mapped_array_view<M, T, NewRank>{array_view<T, Rank>::view_as(ext), m};
	}
};

template<typename M, typename T, int Rank>
static inline mapped_array_view<M, T, Rank> map(const array_view<T, Rank>& av){
	return {av};
}
template<typename M, typename T, int Rank>
static inline mapped_array_view<M, T, Rank> map(const array_view<T, Rank>& av, const M& m){
	return {av, m};
}

namespace detail{

template<typename>
struct is_array_view{using type = std::false_type;};
template<typename T, int Rank>
struct is_array_view<array_view<T, Rank>>{using type = std::true_type;};
template<typename M, typename T, int Rank>
struct is_array_view<mapped_array_view<M, T, Rank>>{using type = std::true_type;};

}

}

namespace two_dim{

template<>
struct attribute_traits<amp::extent<2>>{
	using tag_type = tag::size;
	using element_type = int;
	static element_type w(const amp::extent<2>& wh)noexcept{return wh[1];}
	static element_type h(const amp::extent<2>& wh)noexcept{return wh[0];}
	static amp::extent<2> create(element_type w, element_type h)noexcept{return {h, w};}
};

}

}