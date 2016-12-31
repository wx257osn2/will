#pragma once
#include"_expected.hpp"
#include"_windows.hpp"
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

namespace will{
class socket{
  static bool& _initialized(){
    static bool impl = false;
    return impl;
  }
public:
  struct scope_init{
    scope_init(){socket::initialize();}
    ~scope_init(){if(socket::_initialized())socket::finalize();}
  };
  static expected<WSADATA, winapi_last_error> initialize(){
    WSADATA wsaData;
    const auto ret = WSAStartup(0x0002,&wsaData);
	if(ret != 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__), ret);
	_initialized() = true;
	return wsaData;
  }
  static expected<void, winapi_last_error> finalize(){
    const auto ret = WSACleanup();
	if(ret != 0)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__), ::WSAGetLastError());
    _initialized() = false;
	return {};
  }
  static bool is_initialized()noexcept{
    return _initialized();
  }
};
}