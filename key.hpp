//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_windows.hpp"
namespace will{
class key{
  template<typename T, typename U>
  static constexpr bool bit_and_mask(T src, U mask)noexcept{return (src & mask) == mask;}
 public:
  enum class keep:unsigned char{enable, repeat};
  bool operator()(unsigned char what, keep keep)const{
    const auto value = GetAsyncKeyState(what);
    return bit_and_mask(value, 0x8000) && (keep == keep::enable || bit_and_mask(value, 1));
  }
};
}