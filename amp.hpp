//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#pragma once
#include<variant>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<amp.h>
#include"_expected.hpp"

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

}

}