#pragma once
#include<cstddef>
#include<utility>
#include<chrono>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<windows.h>
#include<tchar.h>

namespace will{

inline void sleep(std::chrono::milliseconds ms){::Sleep(static_cast<DWORD>(ms.count()));}

}

namespace std{
	template<std::size_t N>
	inline ::errno_t tcscpy(TCHAR (&dst)[N], const TCHAR* src)noexcept{
		return ::_tcscpy_s(dst, src);
	}
	inline ::errno_t tcscpy(const std::pair<TCHAR*, std::size_t>& dst, const TCHAR* src)noexcept{
		return ::_tcscpy_s(dst.first, dst.second, src);
	}
}
