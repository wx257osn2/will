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