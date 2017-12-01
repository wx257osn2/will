//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include<sdkddkver.h>
#if _WIN32_WINNT >= _WIN32_WINNT_WINBLUE
#include<utility>
#include<windows.foundation.h>
#include"_2dim.hpp"
namespace will{

namespace two_dim{

namespace detail{

template<>struct attribute<ABI::Windows::Foundation::Size, two_dim::wh<float>>{
	static const ABI::Windows::Foundation::Size& impl(const wh<float>& wh){return reinterpret_cast<const ABI::Windows::Foundation::Size&>(wh);}
	static ABI::Windows::Foundation::Size impl(wh<float>&& wh){return {wh.w, wh.h};}
};
template<typename T>struct attribute<std::enable_if_t<!std::is_same<T, float>::value, ABI::Windows::Foundation::Size>, two_dim::wh<T>>{static ABI::Windows::Foundation::Size impl(const wh<T>& p){return p.cast<float>().attribute<ABI::Windows::Foundation::Size>();}};
template<>struct attribute<two_dim::wh<float>, ABI::Windows::Foundation::Size>{
	static const wh<float>& impl(const ABI::Windows::Foundation::Size& wh){return reinterpret_cast<const two_dim::wh<float>&>(wh);}
	static wh<float> impl(ABI::Windows::Foundation::Size&& wh){return {wh.Width, wh.Height};}
};
template<typename T>struct attribute<two_dim::wh<T>, std::enable_if_t<!std::is_same<T, float>::value, ABI::Windows::Foundation::Size>>{static two_dim::wh<T> impl(const ABI::Windows::Foundation::Size& p){return two_dim::attribute<wh<float>>(p).cast<T>();}};

}

}

}
#endif