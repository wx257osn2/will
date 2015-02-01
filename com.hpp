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
template<typename T, typename U>
T* query_interface(U* u){
	return com_create_resource<T>([&](T** ptr){return u->QueryInterface(__uuidof(T), reinterpret_cast<void**>(ptr));});
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
  template<typename U>
  com_ptr<U> as()const{
    assert(ptr);
    return query_interface<U>(ptr);
  }
  explicit operator bool()const{return ptr != nullptr;}
};
template<typename T, typename U = T*>
class com_enum_iterator{
	com_ptr<IEnumVARIANT> enu;
	variant var;
	HRESULT hr;
public:
	template<typename Collection>
	explicit com_enum_iterator(const Collection& collection):enu(com_ptr<IUnknown>{com_create_resource<IUnknown>([&](IUnknown** ptr){return collection->get__NewEnum(ptr);})}.as<IEnumVARIANT>()), var(), hr(enu->Next(1, &var.get(), nullptr)){}
	explicit com_enum_iterator():enu(nullptr), var(), hr(S_FALSE){}
	com_enum_iterator& operator++(){VariantClear(&var.get()); hr = enu->Next(1, &var.get(), nullptr); return *this;}
	bool operator==(const com_enum_iterator& rhs)const{return hr == rhs.hr;}
	bool operator!=(const com_enum_iterator& rhs)const{return !(*this == rhs);}
	U operator*()const{
		return U(query_interface<T>(var.get().pdispVal));
	}
};
}