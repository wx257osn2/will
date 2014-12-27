#pragma once
#include<objbase.h>
#include<cassert>
#include<utility>
#include<string>
namespace will{
struct com_apartment{
	enum class thread{
		single = COINIT_APARTMENTTHREADED, 
		multi  = COINIT_MULTITHREADED
	};
	com_apartment(thread t){::CoInitializeEx(nullptr, static_cast<COINIT>(t));}
	~com_apartment(){::CoUninitialize();}
};
template<typename T, typename F>
T* com_create_resource(F&& f){
	T* t;
	return SUCCEEDED(f(&t)) ? t : nullptr;
}
struct bstr_adaptor{
	bstr_adaptor() = delete;
	bstr_adaptor(const bstr_adaptor&) = delete;
	bstr_adaptor(BSTR _):bstr(_){}
	bstr_adaptor(bstr_adaptor&& other):bstr(other.bstr){other.bstr = nullptr;}
	bstr_adaptor(const std::wstring& _):bstr_adaptor(_.c_str()){}
	bstr_adaptor(LPCWSTR _):bstr(SysAllocString(_)){}
	~bstr_adaptor(){SysFreeString(bstr);}
	operator BSTR()const{return bstr;}
private:
	BSTR bstr;
};
struct variant{
	variant():var([]()->VARIANT{VARIANT var; VariantInit(&var);return var;}()){}
	variant(const variant&) = default;
	variant(variant&&) = default;
	~variant(){VariantClear(&var);}
	variant(VARIANT v):var(std::move(v)){}
	variant& operator=(VARIANT v){var = std::move(v); return *this;}
	VARIANT& get(){return var;}
	const VARIANT& get()const{return var;}
private:
	VARIANT var;
};
template <typename T>
class com_ptr{
  T* ptr;
public:
  com_ptr() : ptr(nullptr){}
  com_ptr(T* ptr) : ptr(ptr){}
  com_ptr(const com_ptr& other) : ptr(other.ptr){
    if(ptr)
      ptr->AddRef();
  }
  com_ptr(com_ptr&& other) : ptr(other.ptr){other.detach();}
  ~com_ptr(){reset();}
  void detach(){
    ptr = nullptr;
  }
  void swap(com_ptr& other){
    std::swap(ptr, other.ptr);
  }
  void reset(){
    if(ptr)
      ptr->Release();
    ptr = nullptr;
  }
  T* get()const{return ptr;}
  com_ptr& operator=(com_ptr rhs){
    rhs.swap(*this);
    return *this;
  }
  com_ptr& operator=(T* rhs){
    reset();
    ptr = rhs;
    return *this;
  }
  T* operator->()const{return ptr;}
  T** operator&()const{
    assert(!ptr);
    return &ptr;
  }
  explicit operator bool()const{return ptr != nullptr;}
};
template <typename T>
class com_weak_ptr{
  T* ptr;
public:
  com_weak_ptr() : ptr(nullptr){}
  com_weak_ptr(T* ptr) : ptr(ptr){}
  com_weak_ptr(const com_ptr<T>& other) : ptr(other.ptr){}
  com_weak_ptr(const com_weak_ptr&) = default;
  com_weak_ptr(com_weak_ptr&&) = default;
  ~com_weak_ptr() = default;
  void detach(){
    ptr = nullptr;
  }
  void swap(com_weak_ptr& other){
    std::swap(ptr, other.ptr);
  }
  T* get()const{return ptr;}
  com_weak_ptr& operator=(com_weak_ptr rhs){
    rhs.swap(*this);
    return *this;
  }
  com_weak_ptr& operator=(T* rhs){
    ptr = rhs;
    return *this;
  }
  T* operator->()const{return ptr;}
  T** operator&()const{
    assert(!ptr);
    return &ptr;
  }
  explicit operator bool()const{return ptr != nullptr;}
  com_ptr<T> lock()const{if(ptr)ptr->AddRef();return com_ptr<T>(ptr);}
};
}