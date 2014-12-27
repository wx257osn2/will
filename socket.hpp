#pragma once
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
  static bool initialize(){
    WSADATA wsaData;
    _initialized() = WSAStartup(0x0002,&wsaData)==0;
    return _initialized();
  }
  static bool finalize(){
    WSACleanup();
    _initialized() = false;
    return _initialized();
  }
  static bool is_initialized(){
    return _initialized();
  }
};
}