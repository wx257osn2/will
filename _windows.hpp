#pragma once
#include<cstddef>
#include<utility>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include<windows.h>
#include<tchar.h>

namespace std{
	template<std::size_t N>
	::errno_t tcscpy(TCHAR (&dst)[N], const TCHAR* src)noexcept{
		return ::_tcscpy_s(dst, src);
	}
	::errno_t tcscpy(const std::pair<TCHAR*, std::size_t>& dst, const TCHAR* src)noexcept{
		return ::_tcscpy_s(dst.first, dst.second, src);
	}
}