//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include<exception>
#include<stdexcept>
#include<system_error>
#include<utility>
#include<initializer_list>
#include<type_traits>

namespace will{

template<typename E = std::exception_ptr>
class unexpected_type{
  E error;
 public:
  using value_type = E;
  unexpected_type() = delete;
  template<bool Dummy = std::is_copy_constructible<E>::value, std::enable_if_t<(Dummy, std::is_copy_constructible<E>::value)>* = nullptr>
  constexpr explicit unexpected_type(const E& e)noexcept(std::is_nothrow_copy_constructible<E>::value) : error(e){}
  template<bool Dummy = std::is_move_constructible<E>::value, std::enable_if_t<(Dummy, std::is_move_constructible<E>::value)>* = nullptr>
  constexpr explicit unexpected_type(E&& e)noexcept(std::is_nothrow_move_constructible<E>::value) : error(std::move(e)){}
  template<typename T, typename U, typename... Args, std::enable_if_t<std::is_constructible<E, T&&, U&&, Args&&...>::value>* = nullptr>
  constexpr explicit unexpected_type(T&& t, U&& u, Args&&... args)noexcept(std::is_nothrow_constructible<E, T&&, U&&, Args&&...>::value) : error(std::forward<T>(t), std::forward<U>(u), std::forward<Args>(args)...){}
  constexpr const E& value()const& noexcept{
    return error;
  }
  constexpr E& value()& noexcept{
    return error;
  }
  constexpr E&& value()&& noexcept{
    return std::move(error);
  }
};

template<typename E>
constexpr unexpected_type<std::decay_t<E>> make_unexpected(E&& ex)noexcept(std::is_nothrow_constructible<std::decay_t<E>, E&&>::value){
  return unexpected_type<std::decay_t<E>>(std::forward<E>(ex));
}

template<typename E, typename... Args>
constexpr unexpected_type<E> make_unexpected(Args&&... args)noexcept(std::is_nothrow_constructible<E, Args&&...>::value){
  return unexpected_type<E>(std::forward<Args>(args)...);
}

template<>
struct unexpected_type<std::exception_ptr>{
  std::exception_ptr error;
 public:
  using value_type = std::exception_ptr;
  unexpected_type() = delete;
  explicit unexpected_type(const std::exception_ptr& e)noexcept(std::is_nothrow_copy_constructible<std::exception_ptr>::value) : error(e){}
  explicit unexpected_type(std::exception_ptr&& e)noexcept(std::is_nothrow_move_constructible<std::exception_ptr>::value) : error(std::move(e)){}
  template<typename E>
  explicit unexpected_type(E e)noexcept(std::is_nothrow_move_constructible<std::exception_ptr>::value) : error(std::make_exception_ptr(e)){}
  const std::exception_ptr& value()const noexcept{
    return error;
  }
};

template<typename E>
constexpr bool operator==(const unexpected_type<E>& x, const unexpected_type<E>& y)noexcept(noexcept(std::declval<E>() == std::declval<E>())){
  return x.value() == y.value();
}
template<typename E>
constexpr bool operator!=(const unexpected_type<E>& x, const unexpected_type<E>& y)noexcept(noexcept(std::declval<E>() == std::declval<E>())){
  return !(x == y);
}
template<typename E>
constexpr bool operator<(const unexpected_type<E>& x, const unexpected_type<E>& y)noexcept(noexcept(std::declval<E>() < std::declval<E>())){
  return x.value() < y.value();
}
template<typename E>
constexpr bool operator>(const unexpected_type<E>& x, const unexpected_type<E>& y)noexcept(noexcept(std::declval<E>() < std::declval<E>())){
  return (y < x);
}
template<typename E>
constexpr bool operator<=(const unexpected_type<E>& x, const unexpected_type<E>& y)noexcept(noexcept(std::declval<E>() < std::declval<E>())){
  return !(y < x);
}
template<typename E>
constexpr bool operator>=(const unexpected_type<E>& x, const unexpected_type<E>& y)noexcept(noexcept(std::declval<E>() < std::declval<E>())){
  return !(x < y);
}

constexpr bool operator<(const unexpected_type<std::exception_ptr>&, const unexpected_type<std::exception_ptr>&)noexcept{
  return false;
}
constexpr bool operator>(const unexpected_type<std::exception_ptr>&, const unexpected_type<std::exception_ptr>&)noexcept{
  return false;
}
constexpr bool operator<=(const unexpected_type<std::exception_ptr>& x, const unexpected_type<std::exception_ptr>& y)noexcept(noexcept(std::declval<std::exception_ptr>() == std::declval<std::exception_ptr>())){
  return x==y;
}
constexpr bool operator>=(const unexpected_type<std::exception_ptr>& x, const unexpected_type<std::exception_ptr>& y)noexcept(noexcept(std::declval<std::exception_ptr>() == std::declval<std::exception_ptr>())){
  return x==y;
}

namespace detail{

namespace expected{

template<typename>
struct is_unexpected_impl : std::false_type{};
template<typename E>
struct is_unexpected_impl<unexpected_type<E>> : std::true_type{};

}

}

template<typename T>
using is_unexpected = detail::expected::is_unexpected_impl<std::decay_t<T>>;

inline unexpected_type<std::exception_ptr> make_unexpected_from_current_exception()noexcept(std::is_nothrow_move_constructible<std::exception_ptr>::value){
  return unexpected_type<std::exception_ptr>(std::current_exception());
}

template<typename Error>
class bad_expected_access : public std::logic_error{
  Error error_value;
 public:
  typedef Error error_type;
  bad_expected_access(const Error& e) : std::logic_error("Found an error instead of the expected value."), error_value(e){}
  error_type& error()noexcept{return error_value;}
  const error_type& error()const noexcept{return error_value;}
  explicit operator error_type&()noexcept{return error();}
  explicit operator const error_type&()const noexcept{return error();}
};

template<typename Error>
struct error_traits{
  static constexpr bool can_make_error_from_current_exception = std::is_constructible<Error, std::exception&>::value;
  template<typename... Args>
  static Error make_error(Args&&... args)noexcept(std::is_nothrow_constructible<Error, Args&&...>::value){
    return Error{std::forward<Args>(args)...};
  }
  static Error make_error_from_current_exception()noexcept(std::is_nothrow_constructible<Error, std::exception&>::value && std::is_nothrow_default_constructible<Error>::value)try{
    throw;
  }catch(std::exception& e){
    return make_error(e);
  }catch(...){
    return Error{};
  }
  [[noreturn]] static void rethrow(const Error& e){
    throw bad_expected_access<Error>{e};
  }
};
template<>
struct error_traits<std::exception_ptr>{
  static constexpr bool can_make_error_from_current_exception = true;
  template<typename Exception>
  static std::exception_ptr make_error(const Exception&e)noexcept{
    return std::make_exception_ptr(e);
  }
  static std::exception_ptr make_error_from_current_exception()noexcept{
    return std::current_exception();
  }
  [[noreturn]] static void rethrow(const std::exception_ptr& e){
    std::rethrow_exception(e);
  }
};
template<>
struct error_traits<std::error_code>{
  static constexpr bool can_make_error_from_current_exception = true;
  static std::error_code make_error(const std::system_error& e)noexcept{
    return e.code();
  }
  template<typename Exception, std::enable_if_t<std::is_base_of<std::system_error, std::decay_t<Exception>>::value>* = nullptr>
  static std::error_code make_error(const Exception& e)noexcept(noexcept(e.code())){
    return e.code();
  }
  static std::error_code make_error_from_current_exception()noexcept try{
    throw;
  }catch(std::system_error& e){
    return make_error(e);
  }catch(...){
    return std::error_code();
  }
  [[noreturn]] static void rethrow(const std::error_code& e){
    throw std::system_error(e);
  }
};

struct expect_t{}   static constexpr expect   = {};
struct unexpect_t{} static constexpr unexpect = {};

namespace detail{

namespace expected{

struct only_set_initialized_t{} static constexpr only_set_initialized = {};

template<typename T, typename E>
union trivial_storage{
 private:
  unsigned char dummy;
 public:
  T value;
  E error;
  constexpr trivial_storage(only_set_initialized_t)noexcept : dummy(){}
  template<bool Dummy = std::is_default_constructible<T>::value, std::enable_if_t<(Dummy, std::is_default_constructible<T>::value)>* = nullptr>
  constexpr trivial_storage()noexcept(std::is_nothrow_default_constructible<T>::value) : value(){}
  template<typename... Args>
  constexpr trivial_storage(expect_t, Args&&... args)noexcept(std::is_nothrow_constructible<T, Args&&...>::value) : value(std::forward<Args>(args)...){}
  constexpr trivial_storage(unexpect_t, const E& e)noexcept(std::is_nothrow_copy_constructible<E>::value) : error(e){}
  constexpr trivial_storage(unexpect_t, E&& e)noexcept(std::is_nothrow_move_constructible<E>::value) : error(std::move(e)){}
  template<typename U, std::enable_if_t<!std::is_same<std::decay_t<U>, E>::value>* = nullptr>
  constexpr trivial_storage(unexpect_t, U&& u)noexcept(noexcept(error_traits<E>::make_error(std::forward<U>(u)))) : error(error_traits<E>::make_error(std::forward<U>(u))){}
  template<typename U, typename V, typename... Args>
  constexpr trivial_storage(unexpect_t, U&& u, V&& v, Args&&... args)noexcept(noexcept(error_traits<E>::make_error(std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...))) : error(error_traits<E>::make_error(std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...)){}
  ~trivial_storage() = default;
};

template<typename E>
union trivial_storage<void, E>{
 private:
  unsigned char dummy;
 public:
  E error;
  constexpr trivial_storage()noexcept : dummy(){}
  constexpr trivial_storage(expect_t)noexcept : dummy(){}
  constexpr trivial_storage(unexpect_t, const E& e)noexcept(std::is_nothrow_copy_constructible<E>::value) : error(e){}
  constexpr trivial_storage(unexpect_t, E&& e)noexcept(std::is_nothrow_move_constructible<E>::value) : error(std::move(e)){}
  template<typename U, std::enable_if_t<!std::is_same<std::decay_t<U>, E>::value>* = nullptr>
  constexpr trivial_storage(unexpect_t, U&& u)noexcept(noexcept(error_traits<E>::make_error(std::forward<U>(u)))) : error(error_traits<E>::make_error(std::forward<U>(u))){}
  template<typename U, typename V, typename... Args>
  constexpr trivial_storage(unexpect_t, U&& u, V&& v, Args&&... args)noexcept(noexcept(error_traits<E>::make_error(std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...))) : error(error_traits<E>::make_error(std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...)){}
  ~trivial_storage() = default;
};

template<typename T, typename E>
union no_trivial_storage{
 private:
  unsigned char dummy;
 public:
  T value;
  E error;
  constexpr no_trivial_storage(only_set_initialized_t)noexcept : dummy(){}
  template<bool Dummy = std::is_default_constructible<T>::value, std::enable_if_t<(Dummy, std::is_default_constructible<T>::value)>* = nullptr>
  constexpr no_trivial_storage()noexcept(std::is_nothrow_default_constructible<T>::value) : value(){}
  template<typename... Args>
  constexpr no_trivial_storage(expect_t, Args&&... args)noexcept(std::is_nothrow_constructible<T, Args&&...>::value) : value(std::forward<Args>(args)...){}
  constexpr no_trivial_storage(unexpect_t, const E& e)noexcept(std::is_nothrow_copy_constructible<E>::value) : error(e){}
  constexpr no_trivial_storage(unexpect_t, E&& e)noexcept(std::is_nothrow_move_constructible<E>::value) : error(std::move(e)){}
  template<typename U, std::enable_if_t<!std::is_same<std::decay_t<U>, E>::value>* = nullptr>
  constexpr no_trivial_storage(unexpect_t, U&& u)noexcept(noexcept(error_traits<E>::make_error(std::forward<U>(u)))) : error(error_traits<E>::make_error(std::forward<U>(u))){}
  template<typename U, typename V, typename... Args>
  constexpr no_trivial_storage(unexpect_t, U&& u, V&& v, Args&&... args)noexcept(noexcept(error_traits<E>::make_error(std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...))) : error(error_traits<E>::make_error(std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...)){}
  ~no_trivial_storage()noexcept{}
};

template<typename E>
union no_trivial_storage<void, E>{
 private:
  unsigned char dummy;
 public:
  E error;
  constexpr no_trivial_storage(only_set_initialized_t)noexcept : dummy(){}
  constexpr no_trivial_storage()noexcept : dummy(){}
  constexpr no_trivial_storage(expect_t)noexcept : dummy(){}
  constexpr no_trivial_storage(unexpect_t, const E& e)noexcept(std::is_nothrow_copy_constructible<E>::value) : error(e){}
  constexpr no_trivial_storage(unexpect_t, E&& e)noexcept(std::is_nothrow_move_constructible<E>::value) : error(std::move(e)){}
  template<typename U, std::enable_if_t<!std::is_same<std::decay_t<U>, E>::value>* = nullptr>
  constexpr no_trivial_storage(unexpect_t, U&& u)noexcept(noexcept(error_traits<E>::make_error(std::forward<U>(u)))) : error(error_traits<E>::make_error(std::forward<U>(u))){}
  template<typename U, typename V, typename... Args>
  constexpr no_trivial_storage(unexpect_t, U&& u, V&& v, Args&&... args)noexcept(noexcept(error_traits<E>::make_error(std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...))) : error(error_traits<E>::make_error(std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...)){}
  ~no_trivial_storage()noexcept{}
};


template<typename T, typename E>
struct trivial_expected_base{
  bool has_value;
  trivial_storage<T, E> storage;
  template<bool Dummy = std::is_default_constructible<T>::value, std::enable_if_t<(Dummy, std::is_default_constructible<T>::value)>* = nullptr>
  constexpr trivial_expected_base()noexcept(std::is_nothrow_default_constructible<T>::value) : has_value(true), storage(){}
  template<typename... Args>
  explicit constexpr trivial_expected_base(expect_t, Args&&... args)noexcept(std::is_nothrow_constructible<T, Args&&...>::value) : has_value(true), storage(expect, std::forward<Args>(args)...){}
  template<typename... Args>
  explicit constexpr trivial_expected_base(unexpect_t, Args&&... args)noexcept(std::is_nothrow_constructible<E, Args&&...>::value) : has_value(false), storage(unexpect, std::forward<Args>(args)...){}
  trivial_expected_base(const trivial_expected_base& rhs)noexcept(std::is_nothrow_copy_constructible<T>::value && std::is_nothrow_copy_constructible<E>::value):storage(only_set_initialized){
    if(rhs.has_value)
      ::new(value_ptr()) T(rhs.storage.value);
    else
      ::new(error_ptr()) E(rhs.storage.error);
    has_value = rhs.has_value;
  }
  template<bool Dummy = std::is_move_constructible<T>::value, std::enable_if_t<(Dummy, std::is_move_constructible<T>::value && std::is_move_constructible<E>::value)>* = nullptr>
  trivial_expected_base(trivial_expected_base&& rhs)noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_constructible<E>::value):storage(only_set_initialized){
    if(rhs.has_value)
      ::new(value_ptr()) T(std::move(rhs.storage.value));
    else
      ::new(error_ptr()) E(std::move(rhs.storage.error));
    has_value = rhs.has_value;
  }
  ~trivial_expected_base() = default;
  constexpr T* value_ptr()noexcept{return std::addressof(storage.value);}
  constexpr const T* value_ptr()const noexcept{return std::addressof(storage.value);}
  constexpr E* error_ptr()noexcept{return std::addressof(storage.error);}
  constexpr const E* error_ptr()const noexcept{return std::addressof(storage.error);}
};

template<typename E>
struct trivial_expected_base<void, E>{
  bool has_value;
  trivial_storage<void, E> storage;
  constexpr trivial_expected_base()noexcept : has_value(true), storage(){}
  constexpr trivial_expected_base(expect_t)noexcept : has_value(true), storage(expect){}
  template<typename... Args>
  explicit constexpr trivial_expected_base(unexpect_t, Args&&... args)noexcept(std::is_nothrow_constructible<E, Args&&...>::value) : has_value(false), storage(unexpect, std::forward<Args>(args)...){}
  trivial_expected_base(const trivial_expected_base& rhs)noexcept(std::is_nothrow_copy_constructible<E>::value){
    if(!rhs.has_value)
      ::new(error_ptr()) E(rhs.storage.error);
    has_value = rhs.has_value;
  }
  template<bool Dummy = std::is_move_constructible<E>::value, std::enable_if_t<(Dummy, std::is_move_constructible<E>::value)>* = nullptr>
  trivial_expected_base(trivial_expected_base&& rhs)noexcept(std::is_nothrow_move_constructible<E>::value){
    if(!rhs.has_value)
      ::new(error_ptr()) E(std::move(rhs.storage.error));
    has_value = rhs.has_value;
  }
  ~trivial_expected_base() = default;
  constexpr E* error_ptr()noexcept{return std::addressof(storage.error);}
  constexpr const E* error_ptr()const noexcept{return std::addressof(storage.error);}
};

template<typename T, typename E>
struct no_trivial_expected_base{
  bool has_value;
  no_trivial_storage<T, E> storage;
  template<bool Dummy = std::is_default_constructible<T>::value, std::enable_if_t<(Dummy, std::is_default_constructible<T>::value)>* = nullptr>
  constexpr no_trivial_expected_base()noexcept(std::is_nothrow_default_constructible<T>::value) : has_value(true), storage(){}
  template<typename... Args>
  explicit constexpr no_trivial_expected_base(expect_t, Args&&... args)noexcept(std::is_nothrow_constructible<T, Args&&...>::value) : has_value(true), storage(expect, std::forward<Args>(args)...){}
  template<typename... Args>
  explicit constexpr no_trivial_expected_base(unexpect_t, Args&&... args)noexcept(std::is_nothrow_constructible<E, Args&&...>::value) : has_value(false), storage(unexpect, std::forward<Args>(args)...){}
  no_trivial_expected_base(const no_trivial_expected_base& rhs)noexcept(std::is_nothrow_copy_constructible<T>::value && std::is_nothrow_copy_constructible<E>::value) : has_value(rhs.has_value), storage(only_set_initialized){
    if(rhs.has_value)
      ::new(value_ptr()) T(rhs.storage.value);
    else
      ::new(error_ptr()) E(rhs.storage.error);
  }
  template<bool Dummy = std::is_move_constructible<T>::value, std::enable_if_t<(Dummy, std::is_move_constructible<T>::value && std::is_move_constructible<E>::value)>* = nullptr>
  no_trivial_expected_base(no_trivial_expected_base&& rhs)noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_constructible<E>::value) : has_value(rhs.has_value), storage(only_set_initialized){
    if(rhs.has_value)
      ::new(value_ptr()) T(std::move(rhs.storage.value));
    else
      ::new(error_ptr()) E(std::move(rhs.storage.error));
  }
  ~no_trivial_expected_base()noexcept(std::is_nothrow_destructible<T>::value && std::is_nothrow_destructible<E>::value){
    if(has_value)
      storage.value.~T();
    else
      storage.error.~E();
  }
  constexpr T* value_ptr()noexcept{return std::addressof(storage.value);}
  constexpr const T* value_ptr()const noexcept{return std::addressof(storage.value);}
  constexpr E* error_ptr()noexcept{return std::addressof(storage.error);}
  constexpr const E* error_ptr()const noexcept{return std::addressof(storage.error);}
};

template<typename E>
struct no_trivial_expected_base<void, E>{
  bool has_value;
  no_trivial_storage<void, E> storage;
  constexpr no_trivial_expected_base()noexcept : has_value(true), storage(){}
  constexpr no_trivial_expected_base(expect_t)noexcept : has_value(true), storage(expect){}
  template<typename... Args>
  explicit constexpr no_trivial_expected_base(unexpect_t, Args&&... args)noexcept(std::is_nothrow_constructible<E, Args&&...>::value) : has_value(false), storage(unexpect, std::forward<Args>(args)...){}
  no_trivial_expected_base(const no_trivial_expected_base& rhs)noexcept(std::is_nothrow_copy_constructible<E>::value){
    if(!rhs.has_value)
      ::new(error_ptr()) E(rhs.storage.error);
    has_value = rhs.has_value;
  }
  template<bool Dummy = std::is_move_constructible<E>::value, std::enable_if_t<(Dummy, std::is_move_constructible<E>::value)>* = nullptr>
  no_trivial_expected_base(no_trivial_expected_base&& rhs)noexcept(std::is_nothrow_move_constructible<E>::value){
    if(!rhs.has_value)
      ::new(error_ptr()) E(std::move(rhs.storage.error));
    has_value = rhs.has_value;
  }
  ~no_trivial_expected_base()noexcept(std::is_nothrow_destructible<E>::value){
    if(!has_value)
      storage.error.~E();
  }
  constexpr E* error_ptr()noexcept{return std::addressof(storage.error);}
  constexpr const E* error_ptr()const noexcept{return std::addressof(storage.error);}
};

template<typename T, typename E>
using expected_base = std::conditional_t<
  std::is_trivially_destructible<T>::value && std::is_trivially_destructible<E>::value,
  trivial_expected_base<T,E>,
  no_trivial_expected_base<T,E>
>;

struct holder;

}//End : namespace expected

}//End : namespace detail

template<typename T = detail::expected::holder, typename E = std::exception_ptr>
class expected;

namespace detail{

namespace expected{

template<typename T = detail::expected::holder, typename E = std::exception_ptr>using expected = will::expected<T, E>;

template<typename>struct unwrap_result_type;
template<typename T, typename E>
struct unwrap_result_type<expected<T, E>>{
  using type = expected<T, E>;
};
template<typename T, typename E>
struct unwrap_result_type<expected<expected<T, E>, E>>{
  using type = expected<T, E>;
};
template<typename C>
using unwrap_result_type_t = typename unwrap_result_type<C>::type;

template<typename T, typename E>
constexpr expected<T, E> unwrap(const expected<expected<T, E>, E>& ee)noexcept(std::is_nothrow_copy_constructible<expected<T, E>>::value){
  return ee ? *ee : ee.get_unexpected();
}
template<typename T, typename E>
constexpr expected<T, E> unwrap(expected<expected<T, E>, E>&& ee)noexcept(std::is_nothrow_move_constructible<expected<T, E>>::value){
  return ee ? std::move(*ee) : ee.get_unexpected();
}
template<typename T, typename E>
constexpr expected<T, E> unwrap(expected<expected<T, E>, E>& ee)noexcept(std::is_nothrow_copy_constructible<expected<T, E>>::value){
  return ee ? *ee : ee.get_unexpected();
}
template<typename T, typename E>
constexpr expected<T, E> unwrap(const expected<T, E>& e)noexcept(std::is_nothrow_copy_constructible<expected<T, E>>::value){
  return e;
}
template<typename T, typename E>
constexpr expected<T, E> unwrap(expected<T, E>&& e)noexcept(std::is_nothrow_move_constructible<expected<T, E>>::value){
  return std::move(e);
}
template<typename T, typename E>
constexpr expected<T, E> unwrap(expected<T, E>& e)noexcept(std::is_nothrow_copy_constructible<expected<T, E>>::value){
  return e;
}

template<typename>struct unwrap_all_result_type;
template<typename T, typename E>
struct unwrap_all_result_type<expected<T, E>>{
  using type = expected<T, E>;
};
template<typename T, typename E>
struct unwrap_all_result_type<expected<expected<T, E>, E>>{
  using type = typename unwrap_all_result_type<expected<T, E>>::type;
};
template<typename C>
using unwrap_all_result_type_t = typename unwrap_all_result_type<C>::type;

template<typename T, typename E>
constexpr unwrap_all_result_type_t<expected<T, E>> unwrap_all(const expected<expected<T, E>, E>& ee)noexcept(std::is_nothrow_copy_constructible<expected<T, E>>::value){
  return ee ? unwrap_all(*ee) : ee.get_unexpected();
}
template<typename T, typename E>
constexpr unwrap_all_result_type_t<expected<T, E>> unwrap_all(expected<expected<T, E>, E>&& ee)noexcept(std::is_nothrow_move_constructible<expected<T, E>>::value){
  return ee ? unwrap_all(std::move(*ee)) : ee.get_unexpected();
}
template<typename T, typename E>
constexpr unwrap_all_result_type_t<expected<T, E>> unwrap_all(expected<expected<T, E>, E>& ee)noexcept(std::is_nothrow_copy_constructible<expected<T, E>>::value){
  return ee ? unwrap_all(*ee) : ee.get_unexpected();
}
template<typename T, typename E>
constexpr expected<T, E> unwrap_all(const expected<T, E>& e)noexcept(std::is_nothrow_copy_constructible<expected<T, E>>::value){
  return e;
}
template<typename T, typename E>
constexpr expected<T, E> unwrap_all(expected<T, E>&& e)noexcept(std::is_nothrow_move_constructible<expected<T, E>>::value){
  return std::move(e);
}
template<typename T, typename E>
constexpr expected<T, E> unwrap_all(expected<T, E>& e)noexcept(std::is_nothrow_copy_constructible<expected<T, E>>::value){
  return e;
}

template<typename T, typename E, bool = true>
struct catch_all{
  template<typename F>
  static expected<void, E> type_void(F&& f, T&& t)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    f(std::move(t));
    return {};
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static expected<void, E> type_void(F&& f, T& t)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    f(t);
    return {};
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static std::result_of_t<F(T)> type_type(F&& f, T&& t)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f(std::move(t));
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static std::result_of_t<F(T)> type_type(F&& f, T& t)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f(t);
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static expected<std::result_of_t<F(T)>, E> type_etype(F&& f, T&& t)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f(std::move(t));
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static expected<std::result_of_t<F(T)>, E> type_etype(F&& f, T& t)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f(t);
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static expected<void, E> etype_void(F&& f, expected<T, E>&& t)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    f(std::move(t));
    return {};
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static std::result_of_t<F(expected<T, E>)> etype_type(F&& f, expected<T, E>&& t)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f(std::move(t));
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static expected<std::result_of_t<F(expected<T, E>)>, E> etype_etype(F&& f, expected<T, E>&& t)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f(std::move(t));
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static std::result_of_t<F(E)> error_type(F&& f, E&& e)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f(std::move(e));
  }catch(...){
    return make_unexpected(error_traits<typename std::result_of_t<F(E)>::error_type>::make_error_from_current_exception());
  }
  template<typename F>
  static expected<T, typename std::result_of_t<F(E)>::value_type> error_error(F&& f, E&& e)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f(std::move(e));
  }catch(...){
    return make_unexpected(error_traits<typename std::result_of_t<F(E)>::value_type>::make_error_from_current_exception());
  }
};

template<typename T, typename E>
struct catch_all<T, E, false>{
  template<typename F>
  static expected<void, E> type_void(F&& f, T&& t)noexcept(noexcept(f(std::declval<T>()))){
    f(std::move(t));
    return {};
  }
  template<typename F>
  static expected<void, E> type_void(F&& f, T& t)noexcept(noexcept(f(std::declval<T&>()))){
    f(t);
    return {};
  }
  template<typename F>
  static std::result_of_t<F(T)> type_type(F&& f, T&& t)noexcept(noexcept(f(std::declval<T>()))){
    return f(std::move(t));
  }
  template<typename F>
  static std::result_of_t<F(T)> type_type(F&& f, T& t)noexcept(noexcept(f(std::declval<T&>()))){
    return f(t);
  }
  template<typename F>
  static expected<std::result_of_t<F(T)>, E> type_etype(F&& f, T&& t)noexcept(noexcept(f(std::declval<T>())) && std::is_nothrow_constructible<expected<std::result_of_t<F(T)>, E>, std::result_of_t<F(T)>>::value){
    return f(std::move(t));
  }
  template<typename F>
  static expected<std::result_of_t<F(T)>, E> type_etype(F&& f, T& t)noexcept(noexcept(f(std::declval<T&>())) && std::is_nothrow_constructible<expected<std::result_of_t<F(T&)>, E>, std::result_of_t<F(T&)>>::value){
    return f(t);
  }
  template<typename F>
  static expected<void, E> etype_void(F&& f, expected<T, E>&& t)noexcept(noexcept(f(std::declval<expected<T, E>>()))){
    f(std::move(t));
    return {};
  }
  template<typename F>
  static std::result_of_t<F(expected<T, E>)> etype_type(F&& f, expected<T, E>&& t)noexcept(noexcept(f(std::declval<expected<T, E>>()))){
    return f(std::move(t));
  }
  template<typename F>
  static expected<std::result_of_t<F(expected<T, E>)>, E> etype_etype(F&& f, expected<T, E>&& t)noexcept(noexcept(f(std::declval<expected<T, E>>())) && std::is_nothrow_constructible<expected<std::result_of_t<F(expected<T, E>)>, E>, std::result_of_t<F(expected<T, E>)>>::value){
    return f(std::move(t));
  }
  template<typename F>
  static std::result_of_t<F(E)> error_type(F&& f, E&& e)noexcept(noexcept(f(std::declval<E>()))){
    return f(std::move(e));
  }
  template<typename F>
  static expected<T, typename std::result_of_t<F(E)>::value_type> error_error(F&& f, E&& e)noexcept(noexcept(f(std::declval<E>()))){
    return f(std::move(e));
  }
};

template<typename E>
struct catch_all<void, E, true>{
  template<typename F>
  static expected<void, E> void_void(F&& f)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    f();
    return {};
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static std::result_of_t<F()> void_type(F&& f)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f();
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static expected<std::result_of_t<F()>, E> void_etype(F&& f)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f();
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static expected<void, E> etype_void(F&& f, expected<void, E>&& t)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    f(std::move(t));
    return {};
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static std::result_of_t<F(expected<void, E>)> etype_type(F&& f, expected<void, E>&& t)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f(std::move(t));
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static expected<std::result_of_t<F(expected<void, E>)>, E> etype_etype(F&& f, expected<void, E>&& t)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f(std::move(t));
  }catch(...){
    return make_unexpected(error_traits<E>::make_error_from_current_exception());
  }
  template<typename F>
  static std::result_of_t<F(E)> error_type(F&& f, E&& e)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f(std::move(e));
  }catch(...){
    return make_unexpected(error_traits<typename std::result_of_t<F(E)>::error_type>::make_error_from_current_exception());
  }
  template<typename F>
  static expected<void, typename std::result_of_t<F(E)>::value_type> error_error(F&& f, E&& e)noexcept(noexcept(make_unexpected(error_traits<E>::make_error_from_current_exception())))try{
    return f(std::move(e));
  }catch(...){
    return make_unexpected(error_traits<typename std::result_of_t<F(E)>::value_type>::make_error_from_current_exception());
  }
};

template<typename E>
struct catch_all<void, E, false>{
  template<typename F>
  static expected<void, E> void_void(F&& f)noexcept(noexcept(f())){
    f();
    return {};
  }
  template<typename F>
  static std::result_of_t<F()> void_type(F&& f)noexcept(noexcept(f())){
    return f();
  }
  template<typename F>
  static expected<std::result_of_t<F()>, E> void_etype(F&& f)noexcept(noexcept(f()) && std::is_nothrow_constructible<expected<std::result_of_t<F()>, E>, std::result_of_t<F()>>::value){
    return f();
  }
  template<typename F>
  static expected<void, E> etype_void(F&& f, expected<void, E>&& t)noexcept(noexcept(f(std::declval<expected<void, E>>()))){
    f(std::move(t));
    return {};
  }
  template<typename F>
  static std::result_of_t<F(expected<void, E>)> etype_type(F&& f, expected<void, E>&& t)noexcept(noexcept(f(std::declval<expected<void, E>>()))){
    return f(std::move(t));
  }
  template<typename F>
  static expected<std::result_of_t<F(expected<void, E>)>, E> etype_etype(F&& f, expected<void, E>&& t)noexcept(noexcept(f(std::declval<expected<void, E>>())) && std::is_nothrow_constructible<expected<std::result_of_t<F(expected<void, E>)>, E>, std::result_of_t<F(expected<void, E>)>>::value){
    return f(std::move(t));
  }
  template<typename F>
  static std::result_of_t<F(E)> error_type(F&& f, E&& e)noexcept(noexcept(f(std::declval<E>()))){
    return f(std::move(e));
  }
  template<typename F>
  static expected<void, typename std::result_of_t<F(E)>::value_type> error_error(F&& f, E&& e)noexcept(noexcept(f(std::declval<E>()))){
    return f(std::move(e));
  }
};

template<typename>
struct is_expected_impl : std::false_type{};
template<typename T, typename E>
struct is_expected_impl<expected<T, E>> : std::true_type{};

}//End : namespace expected

}//End : namespace detail

template<typename T>
using is_expected = detail::expected::is_expected_impl<std::decay_t<T>>;

template<typename T, typename E>
class expected : detail::expected::expected_base<T, E>{
  using this_type = expected<T, E>;
  using base_type =  detail::expected::expected_base<T, E>;
 public:
  using value_type = T;
  using error_type = E;
  using errored_type = unexpected_type<error_type>;
  static_assert(!is_unexpected<value_type>::value           , "expected : bad value_type(unexpected_type)");
  static_assert(!std::is_same<value_type, unexpect_t>::value, "expected : bad value_type(unexpect_t)");
  static_assert(!std::is_same<value_type, expect_t>::value  , "expected : bad value_type(expect_t)");
  static_assert(!is_unexpected<error_type>::value           , "expected : bad error_type(unexpected_type)");
  static_assert(!std::is_same<error_type, unexpect_t>::value, "expected : bad error_type(unexpect_t)");
  static_assert(!std::is_same<error_type, expect_t>::value  , "expected : bad error_type(expect_t)");
  template<typename U>
  using rebind = expected<U, error_type>;
  using type_constructor = expected<detail::expected::holder, error_type>;
  template<bool Dummy = std::is_default_constructible<value_type>::value, std::enable_if_t<(Dummy, std::is_default_constructible<value_type>::value)>* = nullptr>
  constexpr expected()noexcept(std::is_nothrow_default_constructible<value_type>::value) : base_type(){}
  template<bool Dummy = std::is_copy_constructible<value_type>::value, std::enable_if_t<(Dummy, std::is_copy_constructible<value_type>::value)>* = nullptr>
  constexpr expected(const value_type& v)noexcept(std::is_nothrow_copy_constructible<value_type>::value) : base_type(expect, v){}
  template<bool Dummy = std::is_move_constructible<value_type>::value, std::enable_if_t<(Dummy, std::is_move_constructible<value_type>::value)>* = nullptr>
  constexpr expected(value_type&& v)noexcept(std::is_nothrow_move_constructible<value_type>::value) : base_type(expect, std::move(v)){}
  expected(const expected& rhs)noexcept(std::is_nothrow_copy_constructible<value_type>::value && std::is_nothrow_copy_constructible<error_type>::value) : base_type(static_cast<const base_type&>(rhs)){}
  template<bool Dummy = std::is_move_constructible<value_type>::value, std::enable_if_t<(Dummy, std::is_move_constructible<value_type>::value && std::is_move_constructible<error_type>::value)>* = nullptr>
  expected(expected&& rhs)noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_constructible<error_type>::value) : base_type(static_cast<base_type&&>(rhs)){}
  template<bool Dummy = std::is_copy_constructible<error_type>::value, std::enable_if_t<(Dummy, std::is_copy_constructible<error_type>::value)>* = nullptr>
  constexpr expected(const unexpected_type<error_type>& e)noexcept(std::is_nothrow_copy_constructible<error_type>::value) : base_type(unexpect, e.value()){}
  template<bool Dummy = std::is_move_constructible<error_type>::value, std::enable_if_t<(Dummy, std::is_move_constructible<error_type>::value)>* = nullptr>
  constexpr expected(unexpected_type<error_type>&& e)noexcept(std::is_nothrow_move_constructible<error_type>::value) : base_type(unexpect, std::move(e.value())){}
  template<typename Err, std::enable_if_t<std::is_constructible<error_type, Err>::value>* = nullptr>
  constexpr expected(const unexpected_type<Err>& e)noexcept(std::is_nothrow_constructible<error_type, Err>::value) : base_type(unexpect, e){}
  template<typename Err>
  constexpr expected(unexpected_type<Err>&& e)noexcept(std::is_nothrow_constructible<error_type, Err>::value) : base_type(unexpect, std::move(e.value())){}
  template<typename... Args, std::enable_if_t<std::is_constructible<error_type, Args&...>::value>* = nullptr>
  constexpr expected(unexpect_t, Args&&... args)noexcept(std::is_nothrow_copy_constructible<error_type>::value) : base_type(unexpect, std::forward<Args>(args)...){}
  template<typename... Args, std::enable_if_t<std::is_constructible<value_type, std::decay_t<Args>...>::value>* = nullptr>
  constexpr explicit expected(expect_t, Args&&... args)noexcept(std::is_nothrow_constructible<value_type, Args&&...>::value) : base_type(expect, std::forward<Args>(args)...){}
  template<typename U, typename... Args, std::enable_if_t<std::is_constructible<value_type, std::initializer_list<U>>::value>* = nullptr>
  constexpr explicit expected(expect_t, std::initializer_list<U> il, Args&&... args)noexcept(std::is_nothrow_constructible<value_type, std::initializer_list<U>, Args&&...>::value) : base_type(expect, il, std::forward<Args>(args)...){}
  ~expected() = default;
  expected& operator=(const expected& e)noexcept(std::is_nothrow_copy_constructible<this_type>::value && std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_constructible<error_type>::value){
    this_type(e).swap(*this);
    return *this;
  }
  expected& operator=(expected&& e)noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_constructible<error_type>::value){
    this_type(std::move(e)).swap(*this);
    return *this;
  }
  template<typename U, std::enable_if_t<std::is_same<std::decay_t<U>, value_type>::value>* = nullptr>
  expected& operator=(U&& value)noexcept(noexcept(this_type(std::forward<U>(value)).swap(std::declval<this_type&>()))){
    this_type(std::forward<U>(value)).swap(*this);
    return *this;
  }
  template<typename... Args, std::enable_if_t<std::is_constructible<value_type, Args&&...>::value>* = nullptr>
  constexpr void emplace(Args&&... args)noexcept(noexcept(this_type(expect, std::forward<Args>(args)...).swap(std::declval<this_type&>()))){
    this_type(expect, std::forward<Args>(args)...).swap(*this);
  }
  template<typename U, typename... Args, std::enable_if_t<std::is_constructible<value_type, std::initializer_list<U>, Args&&...>::value>* = nullptr>
  constexpr void emplace(std::initializer_list<U> il, Args&&... args)noexcept(noexcept(this_type(expect, il, std::forward<Args>(args)...).swap(std::declval<this_type&>()))){
    this_type(expect, il, std::forward<Args>(args)...).swap(*this);
  }
  void swap(expected& rhs)noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_constructible<error_type>::value){
    if(valid()){
      if(rhs.valid()){
        using std::swap;
        swap(**this, *rhs);
      }
      else{
        error_type t = std::move(rhs.error());
        ::new(rhs.value_ptr()) value_type(std::move(**this));
        ::new(this->error_ptr()) error_type(std::move(t));
        std::swap(this->has_value, rhs.has_value);
      }
    }
    else{
      if(rhs.valid())
        rhs.swap(*this);
      else{
        using std::swap;
        swap(error(), rhs.error());
      }
    }
  }
  constexpr bool valid()const noexcept{
    return this->has_value;
  }
  constexpr bool operator!()const noexcept{
    return !valid();
  }
  constexpr explicit operator bool()const noexcept{
    return valid();
  }
  constexpr const value_type& value()const&{
    if(!valid()) error_traits<error_type>::rethrow(this->storage.error);
    return this->storage.value;
  }
  constexpr value_type& value()&{
    if(!valid()) error_traits<error_type>::rethrow(this->storage.error);
    return this->storage.value;
  }
  constexpr value_type&& value()&&{
    if(!valid()) error_traits<error_type>::rethrow(std::move(this->storage.error));
    return std::move(this->storage.value);
  }
  constexpr const value_type& operator*()const& noexcept{
    return this->storage.value;
  }
  constexpr value_type& operator*()& noexcept{
    return this->storage.value;
  }
  constexpr value_type&& operator*()&& noexcept{
    return std::move(this->storage.value);
  }
  constexpr const value_type& operator+()const&{
    return value();
  }
  constexpr value_type& operator+()&{
    return value();
  }
  constexpr value_type&& operator+()&&{
    return std::move(value());
  }
  constexpr const value_type& operator++(int)const&{
    return value();
  }
  constexpr value_type& operator++(int)&{
    return value();
  }
  constexpr value_type&& operator++(int)&&{
    return std::move(value());
  }
  constexpr const value_type* operator->()const noexcept{
    return this->value_ptr();
  }
  constexpr value_type* operator->()noexcept{
    return this->value_ptr();
  }
  constexpr const value_type* operator--(int)const{
    if(!valid()) error_traits<error_type>::rethrow(this->storage.error);
    return this->value_ptr();
  }
  constexpr value_type* operator--(int){
    if(!valid()) error_traits<error_type>::rethrow(this->storage.error);
    return this->value_ptr();
  }
  constexpr const error_type& error()const& noexcept{
    return this->storage.error;
  }
  constexpr error_type& error()& noexcept{
    return this->storage.error;
  }
  constexpr error_type&& error()&& noexcept{
    return std::move(this->storage.error);
  }
  template<bool Dummy = std::is_copy_constructible<error_type>::value, std::enable_if_t<(Dummy, std::is_copy_constructible<error_type>::value)>* = nullptr>
  constexpr unexpected_type<error_type> get_unexpected()const& noexcept(std::is_nothrow_copy_constructible<error_type>::value){
    return unexpected_type<error_type>(error());
  }
  template<bool Dummy = std::is_move_constructible<error_type>::value, std::enable_if_t<(Dummy, std::is_move_constructible<error_type>::value)>* = nullptr>
  constexpr unexpected_type<error_type> get_unexpected()&& noexcept(std::is_nothrow_move_constructible<error_type>::value){
    return unexpected_type<error_type>(std::move(error()));
  }
  template<typename V>
  constexpr value_type value_or(V&& v)const& noexcept(std::is_nothrow_copy_constructible<value_type>::value && std::is_nothrow_constructible<value_type, V&&>::value){
    return *this
      ? **this
      : static_cast<value_type>(std::forward<V>(v));
  }
  template<typename V>
  constexpr value_type value_or(V&& v)&& noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_constructible<value_type, V&&>::value){
    return *this
      ? std::move(**this)
      : static_cast<value_type>(std::forward<V>(v));
  }
  template<typename X>
  constexpr error_type error_or(X&& e)const& noexcept(std::is_nothrow_constructible<error_type, X&&>::value && std::is_nothrow_copy_constructible<error_type>::value){
    return *this
      ? static_cast<error_type>(std::forward<X>(e))
      : error();
  }
  template<typename X>
  constexpr error_type error_or(X&& e)&& noexcept(std::is_nothrow_constructible<error_type, X&&>::value && std::is_nothrow_move_constructible<error_type>::value){
    return *this
      ? static_cast<value_type>(std::forward<X>(e))
      : std::move(error());
  }
  template<typename Exception>
  constexpr value_type value_or_throw()const&{
    return *this
      ? **this
      : throw Exception(error());
  }
  template<typename Exception>
  constexpr value_type value_or_throw()&&{
    return *this
      ? std::move(value())
      : throw Exception(error());
  }
  constexpr detail::expected::unwrap_result_type_t<expected> unwrap()const& noexcept(std::is_nothrow_copy_constructible<expected>::value){
    return detail::expected::unwrap(*this);
  }
  constexpr detail::expected::unwrap_result_type_t<expected> unwrap()&& noexcept(std::is_nothrow_move_constructible<expected>::value){
    return detail::expected::unwrap(*this);
  }
  constexpr detail::expected::unwrap_result_type_t<expected> unwrap()& noexcept(std::is_nothrow_copy_constructible<expected>::value){
    return detail::expected::unwrap(*this);
  }
  constexpr detail::expected::unwrap_all_result_type_t<expected> unwrap_all()const& noexcept(std::is_nothrow_copy_constructible<expected>::value){
    return detail::expected::unwrap_all(*this);
  }
  constexpr detail::expected::unwrap_all_result_type_t<expected> unwrap_all()&& noexcept(std::is_nothrow_move_constructible<expected>::value){
    return detail::expected::unwrap_all(*this);
  }
  constexpr detail::expected::unwrap_all_result_type_t<expected> unwrap_all()& noexcept(std::is_nothrow_copy_constructible<expected>::value){
    return detail::expected::unwrap_all(*this);
  }
  template<typename F, std::enable_if_t<std::is_same<std::result_of_t<F(value_type)>, void>::value>* = nullptr>
  rebind<void> map(F&& f)noexcept(noexcept(detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::type_void(std::forward<F>(f), std::declval<value_type>())) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::type_void(std::forward<F>(f), std::move(**this))
         : rebind<void>(get_unexpected())
         ;
  }
  template<typename F, std::enable_if_t<!std::is_same<std::result_of_t<F(value_type)>, void>::value>* = nullptr>
  rebind<std::result_of_t<F(value_type)>> map(F&& f)noexcept(noexcept(detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::type_etype(std::forward<F>(f), std::declval<value_type>())) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::type_etype(std::forward<F>(f), std::move(**this))
         : rebind<std::result_of_t<F(value_type)>>(get_unexpected())
         ;
  }
  template<typename F, std::enable_if_t<is_expected<std::result_of_t<F(value_type)>>::value>* = nullptr>
  std::result_of_t<F(value_type)> bind(F&& f)noexcept(noexcept(detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::type_type(std::forward<F>(f), std::declval<value_type>())) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::type_type(std::forward<F>(f), std::move(**this))
         : std::result_of_t<F(value_type)>(get_unexpected())
         ;
  }
  template<typename F, std::enable_if_t<std::is_same<std::result_of_t<F(expected)>, void>::value>* = nullptr>
  rebind<void> then(F&& f)noexcept(noexcept(detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_void(std::forward<F>(f), std::declval<this_type>()))){
    return detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_void(std::forward<F>(f), std::move(*this));
  }
  template<typename F, std::enable_if_t<!std::is_same<std::result_of_t<F(expected)>, void>::value && !is_expected<std::result_of_t<F(expected)>>::value>* = nullptr>
  rebind<std::result_of_t<F(expected)>> then(F&& f)noexcept(noexcept(detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_etype(std::forward<F>(f), std::declval<this_type>()))){
    return detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_etype(std::forward<F>(f), std::move(*this));
  }
  template<typename F, std::enable_if_t<is_expected<std::result_of_t<F(expected)>>::value>* = nullptr>
  std::result_of_t<F(expected)> then(F&& f)noexcept(noexcept(detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_type(std::forward<F>(f), std::declval<this_type>()))){
    return detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_type(std::forward<F>(f), std::move(*this));
  }
  template<typename F, std::enable_if_t<is_unexpected<std::result_of_t<F(error_type)>>::value>* = nullptr>
  expected<value_type, typename std::result_of_t<F(error_type)>::value_type> emap(F&& f)noexcept(std::is_nothrow_move_constructible<value_type>::value && noexcept(detail::expected::catch_all<value_type, error_type, error_traits<typename std::result_of_t<F(error_type)>::value_type>::can_make_error_from_current_exception>::error_error(std::forward<F>(f), std::declval<error_type>())) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? expected<value_type, typename std::result_of_t<F(error_type)>::value_type>{std::move(**this)}
         : detail::expected::catch_all<value_type, error_type, error_traits<typename std::result_of_t<F(error_type)>::value_type>::can_make_error_from_current_exception>::error_error(std::forward<F>(f), std::move(error()))
         ;
  }
  template<typename F, std::enable_if_t<is_expected<std::result_of_t<F(error_type)>>::value && std::is_same<typename std::result_of_t<F(error_type)>::value_type, T>::value>* = nullptr>
  std::result_of_t<F(error_type)> emap(F&& f)noexcept(std::is_nothrow_constructible<std::result_of_t<F(error_type)>, value_type&&>::value && noexcept(detail::expected::catch_all<value_type, error_type, error_traits<typename std::result_of_t<F(error_type)>::error_type>::can_make_error_from_current_exception>::type_etype(std::forward<F>(f), std::declval<error_type>())) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? std::result_of_t<F(error_type)>{std::move(**this)}
         : detail::expected::catch_all<value_type, error_type, error_traits<typename std::result_of_t<F(error_type)>::error_type>::can_make_error_from_current_exception>::error_type(std::forward<F>(f), std::move(error()))
         ;
  }
  template<typename F, std::enable_if_t<std::is_same<std::result_of_t<F(error_type)>, value_type>::value || std::is_same<std::result_of_t<F(error_type)>, unexpected_type<error_type>>::value>* = nullptr>
  this_type catch_error(F&& f)noexcept(noexcept(this_type(f(std::declval<E>())))){
    return !valid()
         ? this_type(f(error()))
         : std::move_if_noexcept(*this);
         ;
  }
  template<typename F, std::enable_if_t<std::is_same<std::result_of_t<F(error_type)>, this_type>::value>* = nullptr>
  this_type catch_error(F&& f)noexcept(noexcept(f(std::declval<E>()))){
    return !valid()
         ? f(error())
         : std::move_if_noexcept(*this);
         ;
  }
  template<typename Exception, typename F, std::enable_if_t<std::is_same<error_type, std::exception_ptr>::value && std::is_same<std::result_of_t<F(Exception&)>, this_type>::value>* = nullptr>
  this_type catch_exception(F&& f)noexcept(noexcept(f(std::declval<Exception&>()))){
    try{
      if(!valid())
        std::rethrow_exception(error());
    }
    catch(Exception& e){
      return f(e);
    }
    catch(...){}
    return std::move_if_noexcept(*this);
  }
  template<typename Exception, typename F, std::enable_if_t<std::is_same<error_type, std::exception_ptr>::value && std::is_same<std::result_of_t<F(Exception&)>, value_type>::value>* = nullptr>
  this_type catch_exception(F&& f)noexcept(noexcept(this_type(f(std::declval<Exception&>())))){
    try{
      if(!valid())
        std::rethrow_exception(error());
    }
    catch(Exception& e){
      return this_type(f(e));
    }
    catch(...){}
    return std::move_if_noexcept(*this);
  }
  template<typename Exception, bool Dummy = std::is_same<error_type, std::exception_ptr>::value, std::enable_if_t<(Dummy, std::is_same<error_type, std::exception_ptr>::value)>* = nullptr>
  bool has_exception()const noexcept{
    try{
      if(!valid())
        std::rethrow_exception(error());
    }
    catch(Exception&){
      return true;
    }
    catch(...){}
    return false;
  }
  template<typename Error>
  constexpr bool has_error(const Error& err)const noexcept(noexcept(std::declval<error_type>() == std::declval<Error>())){
    return !valid() && error() == err;
  }
};

template<typename E>
class expected<detail::expected::holder, E>{
public:
  template<typename T>
  using type = expected<std::decay_t<T>, E>;
  template<typename T>
  constexpr expected<std::decay_t<T>, E> make(T&& v)noexcept(std::is_nothrow_constructible<std::decay_t<T>, T&&>::value){
    return expected<std::decay_t<T>, E>(std::forward<T>(v));
  }
};

template<typename T>
using exception_or = expected<T, std::exception_ptr>;

template<typename T, typename E>
class expected<T&, E> : detail::expected::expected_base<T*, E>{
  using this_type = expected<T, E>;
  using base_type =  detail::expected::expected_base<T*, E>;
 public:
  using value_type = T&;
  using error_type = E;
  using errored_type = unexpected_type<error_type>;
  static_assert(!is_unexpected<std::remove_reference_t<value_type>>::value           , "expected : bad value_type(unexpected_type)");
  static_assert(!std::is_same<std::remove_reference_t<value_type>, unexpect_t>::value, "expected : bad value_type(unexpect_t)");
  static_assert(!std::is_same<std::remove_reference_t<value_type>, expect_t>::value  , "expected : bad value_type(expect_t)");
  static_assert(!is_unexpected<error_type>::value                                    , "expected : bad error_type(unexpected_type)");
  static_assert(!std::is_same<error_type, unexpect_t>::value                         , "expected : bad error_type(unexpect_t)");
  static_assert(!std::is_same<error_type, expect_t>::value                           , "expected : bad error_type(expect_t)");
  template<typename U>
  using rebind = expected<U, error_type>;
  using type_constructor = expected<detail::expected::holder, error_type>;
  constexpr expected(value_type v)noexcept : base_type(expect, &v){}
  expected(const expected& rhs)noexcept(std::is_nothrow_copy_constructible<value_type>::value && std::is_nothrow_copy_constructible<error_type>::value) : base_type(static_cast<const base_type&>(rhs)){}
  expected(expected&& rhs)noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_constructible<error_type>::value) : base_type(static_cast<base_type&&>(rhs)){}
  template<bool Dummy = std::is_copy_constructible<error_type>::value, std::enable_if_t<(Dummy, std::is_copy_constructible<error_type>::value)>* = nullptr>
  constexpr expected(const unexpected_type<error_type>& e)noexcept(std::is_nothrow_copy_constructible<error_type>::value) : base_type(unexpect, e.value()){}
  template<bool Dummy = std::is_move_constructible<error_type>::value, std::enable_if_t<(Dummy, std::is_move_constructible<error_type>::value)>* = nullptr>
  constexpr expected(unexpected_type<error_type>&& e)noexcept(std::is_nothrow_move_constructible<error_type>::value) : base_type(unexpect, std::move(e.value())){}
  template<typename Err, std::enable_if_t<std::is_constructible<error_type, Err>::value>* = nullptr>
  constexpr expected(const unexpected_type<Err>& e)noexcept(std::is_nothrow_constructible<error_type, Err>::value) : base_type(unexpect, e){}
  template<typename Err>
  constexpr expected(unexpected_type<Err>&& e)noexcept(std::is_nothrow_constructible<error_type, Err>::value) : base_type(unexpect, std::move(e.value())){}
  template<typename... Args, std::enable_if_t<std::is_constructible<error_type, Args&...>::value>* = nullptr>
  constexpr expected(unexpect_t, Args&&... args)noexcept(std::is_nothrow_copy_constructible<error_type>::value) : base_type(unexpect, std::forward<Args>(args)...){}
  constexpr explicit expected(expect_t, value_type arg)noexcept : base_type(expect, &arg){}
  ~expected() = default;
  expected& operator=(const expected& e)noexcept(std::is_nothrow_move_constructible<error_type>::value){
    this_type(e).swap(*this);
    return *this;
  }
  expected& operator=(expected&& e)noexcept(std::is_nothrow_move_constructible<error_type>::value){
    this_type(std::move(e)).swap(*this);
    return *this;
  }
  expected& operator=(value_type value)noexcept{
    this_type(value).swap(*this);
    return *this;
  }
  constexpr void emplace(value_type arg)noexcept{
    this_type(expect, arg).swap(*this);
  }
  void swap(expected& rhs)noexcept(std::is_nothrow_move_constructible<error_type>::value){
    if(valid()){
      if(rhs.valid()){
        using std::swap;
        swap(this->storage.value, rhs.storage.value);
      }
      else{
        error_type t = std::move(rhs.error());
        ::new(rhs.value_ptr()) T*(std::move(this->storage.value));
        ::new(this->error_ptr()) error_type(std::move(t));
        std::swap(this->has_value, rhs.has_value);
      }
    }
    else{
      if(rhs.valid())
        rhs.swap(*this);
      else{
        using std::swap;
        swap(error(), rhs.error());
      }
    }
  }
  constexpr bool valid()const noexcept{
    return this->has_value;
  }
  constexpr bool operator!()const noexcept{
    return !valid();
  }
  constexpr explicit operator bool()const noexcept{
    return valid();
  }
  constexpr value_type value()const{
    if(!valid()) error_traits<error_type>::rethrow(this->storage.error);
    return *this->storage.value;
  }
  constexpr value_type operator*()const noexcept{
    return *this->storage.value;
  }
  constexpr value_type operator+()const{
    return value();
  }
  constexpr value_type operator++(int)const{
    return value();
  }
  constexpr T* operator->()const noexcept{
    return this->storage.value;
  }
  constexpr T* operator--(int)const{
    if(!valid()) error_traits<error_type>::rethrow(this->storage.error);
    return this->storage.value;
  }
  constexpr const error_type& error()const& noexcept{
    return this->storage.error;
  }
  constexpr error_type& error()& noexcept{
    return this->storage.error;
  }
  constexpr error_type&& error()&& noexcept{
    return std::move(this->storage.error);
  }
  template<bool Dummy = std::is_copy_constructible<error_type>::value, std::enable_if_t<(Dummy, std::is_copy_constructible<error_type>::value)>* = nullptr>
  constexpr unexpected_type<error_type> get_unexpected()const& noexcept(std::is_nothrow_copy_constructible<error_type>::value){
    return unexpected_type<error_type>(error());
  }
  template<bool Dummy = std::is_move_constructible<error_type>::value, std::enable_if_t<(Dummy, std::is_move_constructible<error_type>::value)>* = nullptr>
  constexpr unexpected_type<error_type> get_unexpected()&& noexcept(std::is_nothrow_move_constructible<error_type>::value){
    return unexpected_type<error_type>(std::move(error()));
  }
  template<typename V>
  constexpr value_type value_or(V&& v)const noexcept(std::is_nothrow_constructible<value_type, V&&>::value){
    return *this
      ? **this
      : static_cast<value_type>(std::forward<V>(v));
  }
  template<typename X>
  constexpr error_type error_or(X&& e)const& noexcept(std::is_nothrow_constructible<error_type, X&&>::value && std::is_nothrow_copy_constructible<error_type>::value){
    return *this
      ? static_cast<error_type>(std::forward<X>(e))
      : error();
  }
  template<typename X>
  constexpr error_type error_or(X&& e)&& noexcept(std::is_nothrow_constructible<error_type, X&&>::value && std::is_nothrow_move_constructible<error_type>::value){
    return *this
      ? static_cast<value_type>(std::forward<X>(e))
      : std::move(error());
  }
  template<typename Exception>
  constexpr value_type value_or_throw()const{
    return *this
      ? **this
      : throw Exception(error());
  }
  constexpr detail::expected::unwrap_result_type_t<expected> unwrap()const& noexcept(std::is_nothrow_copy_constructible<expected>::value){
    return detail::expected::unwrap(*this);
  }
  constexpr detail::expected::unwrap_result_type_t<expected> unwrap()&& noexcept(std::is_nothrow_move_constructible<expected>::value){
    return detail::expected::unwrap(*this);
  }
  constexpr detail::expected::unwrap_result_type_t<expected> unwrap()& noexcept(std::is_nothrow_copy_constructible<expected>::value){
    return detail::expected::unwrap(*this);
  }
  constexpr detail::expected::unwrap_all_result_type_t<expected> unwrap_all()const& noexcept(std::is_nothrow_copy_constructible<expected>::value){
    return detail::expected::unwrap_all(*this);
  }
  constexpr detail::expected::unwrap_all_result_type_t<expected> unwrap_all()&& noexcept(std::is_nothrow_move_constructible<expected>::value){
    return detail::expected::unwrap_all(*this);
  }
  constexpr detail::expected::unwrap_all_result_type_t<expected> unwrap_all()& noexcept(std::is_nothrow_copy_constructible<expected>::value){
    return detail::expected::unwrap_all(*this);
  }
  template<typename F, std::enable_if_t<std::is_same<std::result_of_t<F(value_type)>, void>::value>* = nullptr>
  rebind<void> map(F&& f)noexcept(noexcept(detail::expected::catch_all<T, error_type, error_traits<error_type>::can_make_error_from_current_exception>::type_void(std::forward<F>(f), std::declval<value_type>())) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? detail::expected::catch_all<T, error_type, error_traits<error_type>::can_make_error_from_current_exception>::type_void(std::forward<F>(f), **this)
         : rebind<void>(get_unexpected())
         ;
  }
  template<typename F, std::enable_if_t<!std::is_same<std::result_of_t<F(value_type)>, void>::value>* = nullptr>
  rebind<std::result_of_t<F(value_type)>> map(F&& f)noexcept(noexcept(detail::expected::catch_all<T, error_type, error_traits<error_type>::can_make_error_from_current_exception>::type_etype(std::forward<F>(f), std::declval<value_type>())) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? detail::expected::catch_all<T, error_type, error_traits<error_type>::can_make_error_from_current_exception>::type_etype(std::forward<F>(f), **this)
         : rebind<std::result_of_t<F(value_type)>>(get_unexpected())
         ;
  }
  template<typename F, std::enable_if_t<is_expected<std::result_of_t<F(value_type)>>::value>* = nullptr>
  std::result_of_t<F(value_type)> bind(F&& f)noexcept(noexcept(detail::expected::catch_all<T, error_type, error_traits<error_type>::can_make_error_from_current_exception>::type_type(std::forward<F>(f), std::declval<value_type>())) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? detail::expected::catch_all<T, error_type, error_traits<error_type>::can_make_error_from_current_exception>::type_type(std::forward<F>(f), **this)
         : std::result_of_t<F(value_type)>(get_unexpected())
         ;
  }
  template<typename F, std::enable_if_t<std::is_same<std::result_of_t<F(expected)>, void>::value>* = nullptr>
  rebind<void> then(F&& f)noexcept(noexcept(detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_void(std::forward<F>(f), std::declval<this_type>()))){
    return detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_void(std::forward<F>(f), std::move(*this));
  }
  template<typename F, std::enable_if_t<!std::is_same<std::result_of_t<F(expected)>, void>::value && !is_expected<std::result_of_t<F(expected)>>::value>* = nullptr>
  rebind<std::result_of_t<F(expected)>> then(F&& f)noexcept(noexcept(detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_etype(std::forward<F>(f), std::declval<this_type>()))){
    return detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_etype(std::forward<F>(f), std::move(*this));
  }
  template<typename F, std::enable_if_t<is_expected<std::result_of_t<F(expected)>>::value>* = nullptr>
  std::result_of_t<F(expected)> then(F&& f)noexcept(noexcept(detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_type(std::forward<F>(f), std::declval<this_type>()))){
    return detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_type(std::forward<F>(f), std::move(*this));
  }
  template<typename F, std::enable_if_t<is_unexpected<std::result_of_t<F(error_type)>>::value>* = nullptr>
  expected<value_type, typename std::result_of_t<F(error_type)>::value_type> emap(F&& f)noexcept(noexcept(detail::expected::catch_all<value_type, error_type, error_traits<typename std::result_of_t<F(error_type)>::value_type>::can_make_error_from_current_exception>::error_error(std::forward<F>(f), std::declval<error_type>())) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? expected<value_type, typename std::result_of_t<F(error_type)>::value_type>{**this}
         : detail::expected::catch_all<value_type, error_type, error_traits<typename std::result_of_t<F(error_type)>::value_type>::can_make_error_from_current_exception>::error_error(std::forward<F>(f), std::move(error()))
         ;
  }
  template<typename F, std::enable_if_t<is_expected<std::result_of_t<F(error_type)>>::value && std::is_same<typename std::result_of_t<F(error_type)>::value_type, T>::value>* = nullptr>
  std::result_of_t<F(error_type)> emap(F&& f)noexcept(noexcept(detail::expected::catch_all<value_type, error_type, error_traits<typename std::result_of_t<F(error_type)>::error_type>::can_make_error_from_current_exception>::type_etype(std::forward<F>(f), std::declval<error_type>())) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? std::result_of_t<F(error_type)>{**this}
         : detail::expected::catch_all<value_type, error_type, error_traits<typename std::result_of_t<F(error_type)>::error_type>::can_make_error_from_current_exception>::error_type(std::forward<F>(f), std::move(error()))
         ;
  }
  template<typename F, std::enable_if_t<std::is_same<std::result_of_t<F(error_type)>, value_type>::value || std::is_same<std::result_of_t<F(error_type)>, unexpected_type<error_type>>::value>* = nullptr>
  this_type catch_error(F&& f)noexcept(noexcept(this_type(f(std::declval<E>())))){
    return !valid()
         ? this_type(f(error()))
         : std::move_if_noexcept(*this);
         ;
  }
  template<typename F, std::enable_if_t<std::is_same<std::result_of_t<F(error_type)>, this_type>::value>* = nullptr>
  this_type catch_error(F&& f)noexcept(noexcept(f(std::declval<E>()))){
    return !valid()
         ? f(error())
         : std::move_if_noexcept(*this);
         ;
  }
  template<typename Exception, typename F, std::enable_if_t<std::is_same<error_type, std::exception_ptr>::value && std::is_same<std::result_of_t<F(Exception&)>, this_type>::value>* = nullptr>
  this_type catch_exception(F&& f)noexcept(noexcept(f(std::declval<Exception&>()))){
    try{
      if(!valid())
        std::rethrow_exception(error());
    }
    catch(Exception& e){
      return f(e);
    }
    catch(...){}
    return std::move_if_noexcept(*this);
  }
  template<typename Exception, typename F, std::enable_if_t<std::is_same<error_type, std::exception_ptr>::value && std::is_same<std::result_of_t<F(Exception&)>, value_type>::value>* = nullptr>
  this_type catch_exception(F&& f)noexcept(noexcept(this_type(f(std::declval<Exception&>())))){
    try{
      if(!valid())
        std::rethrow_exception(error());
    }
    catch(Exception& e){
      return this_type(f(e));
    }
    catch(...){}
    return std::move_if_noexcept(*this);
  }
  template<typename Exception, bool Dummy = std::is_same<error_type, std::exception_ptr>::value, std::enable_if_t<(Dummy, std::is_same<error_type, std::exception_ptr>::value)>* = nullptr>
  bool has_exception()const noexcept{
    try{
      if(!valid())
        std::rethrow_exception(error());
    }
    catch(Exception&){
      return true;
    }
    catch(...){}
    return false;
  }
  template<typename Error>
  constexpr bool has_error(const Error& err)const noexcept(noexcept(std::declval<error_type>() == std::declval<Error>())){
    return !valid() && error() == err;
  }
};

template<typename E>
class expected<void, E> : detail::expected::expected_base<void, E>{
  using this_type = expected<void, E>;
  using base_type = detail::expected::expected_base<void, E>;
 public:
  typedef void value_type;
  typedef E error_type;
  using errored_type = unexpected_type<error_type>;
  static_assert(!is_unexpected<error_type>::value           , "expected : bad error_type(unexpected_type)");
  static_assert(!std::is_same<error_type, unexpect_t>::value, "expected : bad errot_type(unexpect_t)");
  static_assert(!std::is_same<error_type, expect_t>::value  , "expected : bad error_type(expect_t)");
  template<typename T>
  using rebind = expected<T, error_type>;
  using type_constructor = expected<detail::expected::holder, error_type>;
  expected(const expected& rhs)noexcept(std::is_nothrow_copy_constructible<error_type>::value) : base_type(static_cast<const base_type&>(rhs)){}
  template<bool Dummy = std::is_move_constructible<error_type>::value, std::enable_if_t<(Dummy, std::is_move_constructible<error_type>::value)>* = nullptr>
  expected(expected&& rhs)noexcept(std::is_nothrow_move_constructible<error_type>::value) : base_type(static_cast<base_type&&>(rhs)){}
  constexpr explicit expected(expect_t)noexcept : base_type(expect){}
  constexpr expected()noexcept = default;
  template<bool Dummy = std::is_copy_constructible<error_type>::value, std::enable_if_t<(Dummy, std::is_copy_constructible<error_type>::value)>* = nullptr>
  constexpr expected(const unexpected_type<error_type>& e)noexcept(std::is_nothrow_copy_constructible<error_type>::value) : base_type(unexpect, e.value()){}
  template<bool Dummy = std::is_move_constructible<error_type>::value, std::enable_if_t<(Dummy, std::is_move_constructible<error_type>::value)>* = nullptr>
  constexpr expected(unexpected_type<error_type>&& e)noexcept(std::is_nothrow_move_constructible<error_type>::value) : base_type(unexpect, std::move(e.value())){}
  template<typename Err, std::enable_if_t<std::is_constructible<error_type, Err>::value>* = nullptr>
  constexpr expected(const unexpected_type<Err>& e)noexcept(std::is_nothrow_constructible<error_type, Err>::value) : base_type(unexpect, e.value()){}
  template<typename Err>
  constexpr expected(unexpected_type<Err>&& e): base_type(unexpect, std::move(e.value())){}
  template<typename... Args, std::enable_if_t<std::is_constructible<error_type, Args&&...>::value>* = nullptr>
  constexpr expected(unexpect_t, Args&&... args)noexcept(std::is_nothrow_constructible<error_type, Args&&...>::value) : base_type(unexpect, std::forward<Args>(args)...){}
  ~expected() = default;
  expected& operator=(const expected& e)noexcept(std::is_nothrow_copy_constructible<this_type>::value && std::is_nothrow_move_constructible<error_type>::value){
    this_type(e).swap(*this);
    return *this;
  }
  expected& operator=(expected&& e)noexcept(std::is_nothrow_move_constructible<error_type>::value){
    this_type(std::move(e)).swap(*this);
    return *this;
  }
  void emplace()noexcept(std::is_nothrow_move_constructible<error_type>::value){
    this_type().swap(*this);
  }
  void swap(expected& rhs)noexcept(std::is_nothrow_move_constructible<error_type>::value){
    if(valid()){
      if(!rhs.valid()){
        error_type t = std::move(rhs.error());
        ::new(this->error_ptr()) error_type(std::move(t));
        std::swap(this->has_value, rhs.has_value);
      }
    }
    else{
      if(rhs.valid())
        rhs.swap(*this);
      else{
        using std::swap;
        swap(this->error(), rhs.error());
      }
    }
  }
  constexpr bool valid()const noexcept{
    return this->has_value;
  }
  constexpr bool operator!()const noexcept{
    return !valid();
  }
  constexpr explicit operator bool()const noexcept{
    return valid();
  }
  constexpr void value()const{
    if(!valid())
      error_traits<error_type>::rethrow(error());
  }
  constexpr void operator+()const{
    value();
  }
  constexpr error_type const& error()const& noexcept{
    return this->storage.error;
  }
  constexpr error_type& error()& noexcept{
    return this->storage.error;
  }
  constexpr error_type&& error()&& noexcept{
    return std::move(this->stroage.error);
  }
  constexpr unexpected_type<error_type> get_unexpected()const& noexcept(std::is_nothrow_copy_constructible<error_type>::value){
    return unexpected_type<error_type>(error());
  }
  constexpr unexpected_type<error_type> get_unexpected()&& noexcept(std::is_nothrow_move_constructible<error_type>::value){
    return unexpected_type<error_type>(std::move(error()));
  }
  template<typename X>
  constexpr error_type error_or(X&& e)const& noexcept(std::is_nothrow_constructible<error_type, X&&>::value){
    return *this
      ? static_cast<error_type>(std::forward<X>(e))
      : error();
  }
  template<typename X>
  constexpr error_type error_or(X&& e)&& noexcept(std::is_nothrow_constructible<error_type, X&&>::value){
    return *this
      ? static_cast<error_type>(std::forward<X>(e))
      : std::move(error());
  }
  constexpr detail::expected::unwrap_result_type_t<expected> unwrap()const& noexcept(std::is_nothrow_copy_constructible<expected>::value){
    return detail::expected::unwrap(*this);
  }
  constexpr detail::expected::unwrap_result_type_t<expected> unwrap()&& noexcept(std::is_nothrow_move_constructible<expected>::value){
    return detail::expected::unwrap(*this);
  }
  constexpr detail::expected::unwrap_result_type_t<expected> unwrap()& noexcept(std::is_nothrow_copy_constructible<expected>::value){
    return detail::expected::unwrap(*this);
  }
  template<typename F, std::enable_if_t<std::is_same<std::result_of_t<F(void)>, void>::value>* = nullptr>
  rebind<void> map(F&& f)noexcept(noexcept(detail::expected::catch_all<void, error_type, error_traits<error_type>::can_make_error_from_current_exception>::void_void(std::forward<F>(f))) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? detail::expected::catch_all<value_type, error_type, error_traits<error_type>::can_make_error_from_current_exception>::void_void(std::forward<F>(f))
         : rebind<void>(get_unexpected())
         ;
  }
  template<typename F, std::enable_if_t<!std::is_same<std::result_of_t<F(void)>, void>::value>* = nullptr>
  rebind<std::result_of_t<F(void)>> map(F&& f)noexcept(noexcept(detail::expected::catch_all<void, error_type, error_traits<error_type>::can_make_error_from_current_exception>::void_etype(std::forward<F>(f))) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? detail::expected::catch_all<void, error_type, error_traits<error_type>::can_make_error_from_current_exception>::void_etype(std::forward<F>(f))
         : rebind<std::result_of_t<F(void)>>(get_unexpected())
         ;
  }
  template<typename F, std::enable_if_t<is_expected<std::result_of_t<F(void)>>::value>* = nullptr>
  std::result_of_t<F()> bind(F&& f)noexcept(noexcept(detail::expected::catch_all<void, error_type, error_traits<error_type>::can_make_error_from_current_exception>::void_type(std::forward<F>(f))) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? detail::expected::catch_all<void, error_type, error_traits<error_type>::can_make_error_from_current_exception>::void_type(std::forward<F>(f))
         : std::result_of_t<F()>(get_unexpected())
         ;
  }
  template<typename F, std::enable_if_t<std::is_same<std::result_of_t<F(expected)>, void>::value>* = nullptr>
  rebind<void> then(F&& f)noexcept(noexcept(detail::expected::catch_all<void, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_void(std::forward<F>(f), std::declval<this_type>()))){
    return detail::expected::catch_all<void, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_void(std::forward<F>(f), std::move(*this));
  }
  template<typename F, std::enable_if_t<!is_expected<std::result_of_t<F(expected)>>::value>* = nullptr>
  rebind<std::result_of_t<F(expected)>> then(F&& f)noexcept(noexcept(detail::expected::catch_all<void, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_etype(std::forward<F>(f), std::declval<this_type>()))){
    return detail::expected::catch_all<void, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_etype(std::forward<F>(f), std::move(*this));
  }
  template<typename F, std::enable_if_t<!std::is_same<std::result_of_t<F(expected)>, void>::value && is_expected<std::result_of_t<F(expected)>>::value>* = nullptr>
  std::result_of_t<F(expected)> then(F&& f)noexcept(noexcept(detail::expected::catch_all<void, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_type(std::forward<F>(f), std::declval<this_type>()))){
    return detail::expected::catch_all<void, error_type, error_traits<error_type>::can_make_error_from_current_exception>::etype_type(std::forward<F>(f), std::move(*this));
  }
  template<typename F, std::enable_if_t<is_unexpected<std::result_of_t<F(error_type)>>::value>* = nullptr>
  expected<void, typename std::result_of_t<F(error_type)>::value_type> emap(F&& f)noexcept(noexcept(detail::expected::catch_all<void, error_type, error_traits<typename std::result_of_t<F(error_type)>::value_type>::can_make_error_from_current_exception>::error_error(std::forward<F>(f), std::declval<error_type>())) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? *this
         : detail::expected::catch_all<void, error_type, error_traits<typename std::result_of_t<F(error_type)>::value_type>::can_make_error_from_current_exception>::error_error(std::forward<F>(f), std::move(error()))
         ;
  }
  template<typename F, std::enable_if_t<is_expected<std::result_of_t<F(error_type)>>::value && std::is_same<typename std::result_of_t<F(error_type)>::value_type, void>::value>* = nullptr>
  std::result_of_t<F(error_type)> emap(F&& f)noexcept(noexcept(detail::expected::catch_all<void, error_type, error_traits<typename std::result_of_t<F(error_type)>::error_type>::can_make_error_from_current_exception>::type_etype(std::forward<F>(f), std::declval<error_type>())) && std::is_nothrow_move_constructible<error_type>::value){
    return valid()
         ? *this
         : detail::expected::catch_all<void, error_type, error_traits<typename std::result_of_t<F(error_type)>::error_type>::can_make_error_from_current_exception>::error_type(std::forward<F>(f), std::move(error()))
         ;
  }
  template<typename F, std::enable_if_t<std::is_same<std::result_of_t<F(error_type)>, value_type>::value>* = nullptr>
  this_type catch_error(F&& f)noexcept(noexcept(f(error())) && std::is_nothrow_copy_constructible<this_type>::value){
    if(!valid()){
      f(error());
      return this_type();
    }
    return std::move_if_noexcept(*this);
  }
  template<typename F, std::enable_if_t<!std::is_same<std::result_of_t<F(error_type)>, value_type>::value>* = nullptr>
  this_type catch_error(F&& f)noexcept(noexcept(f(error())) && std::is_nothrow_constructible<this_type, std::result_of_t<F(error_type)>>::value && std::is_nothrow_copy_constructible<this_type>::value){
    return !valid()
         ? f(error())
         : std::move_if_noexcept(*this)
         ;
  }
  template<typename Exception, typename F, std::enable_if_t<std::is_same<std::result_of_t<F(Exception&)>, this_type>::value>* = nullptr>
  this_type catch_exception(F&& f)noexcept(noexcept(f(std::declval<Exception&>())) && std::is_nothrow_constructible<this_type, std::result_of_t<F(Exception&)>>::value && std::is_nothrow_copy_constructible<this_type>::value){
    try{
      if(!valid())
        std::rethrow_exception(error());
    }
    catch(Exception& e){
      return f(e);
    }
    catch(...){}
    return std::move_if_noexcept(*this);
  }
  template<typename Exception, typename F, std::enable_if_t<std::is_same<std::result_of_t<F(Exception&)>, value_type>::value>* = nullptr>
  this_type catch_exception(F&& f)noexcept(noexcept(f(std::declval<Exception&>())) && std::is_nothrow_copy_constructible<this_type>::value){
    try{
      if(!valid())
        std::rethrow_exception(error());
    }
    catch(Exception& e){
      f(e);
      return this_type();
    }
    catch(...){}
    return std::move_if_noexcept(*this);
  }
  template<typename Exception>
  bool has_exception()const noexcept{
    try{
      if(!valid())
        std::rethrow_exception(error());
    }
    catch(Exception&){
      return true;
    }
    catch(...){}
    return false;
  }
  template<typename Error>
  constexpr bool has_error(const Error& err)const noexcept(noexcept(std::declval<error_type>() == std::declval<Error>())){
    return !valid() && error() == err;
  }
};

template<typename T, typename E>
constexpr bool operator==(const expected<T,E>& x, const expected<T,E>& y)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<T>() == std::declval<T>()) && noexcept(std::declval<E>() == std::declval<E>())){
  return (x && y && *x == *y) || (!x && !y && x.get_unexpected() == y.get_unexpected());
}
template<typename E>
constexpr bool operator==(const expected<void, E>& x, const expected<void, E>& y)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() == std::declval<E>())){
  return (x && y) || (!x && !y && x.get_unexpected() == y.get_unexpected());
}
template<typename T, typename E>
constexpr bool operator!=(const expected<T,E>& x, const expected<T,E>& y)noexcept(noexcept(x == y)){
  return !(x == y);
}
template<typename T, typename E>
constexpr bool operator<(const expected<T,E>& x, const expected<T,E>& y)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<T>() < std::declval<T>()) && noexcept(std::declval<E>() < std::declval<E>())){
  return (x && y && *x < *y) || (!x && (y || x.get_unexpected() < y.get_unexpected()));
}

template<typename E>
constexpr bool operator<(const expected<void, E>& x, const expected<void, E>& y)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() < std::declval<E>())){
  return !x && (y || x.get_unexpected() < y.get_unexpected());
}

template<typename T, typename E>
constexpr bool operator>(const expected<T,E>& x, const expected<T,E>& y)noexcept(noexcept(y < x)){
  return y < x;
}

template<typename T, typename E>
constexpr bool operator<=(const expected<T,E>& x, const expected<T,E>& y)noexcept(noexcept(y < x)){
  return !(y < x);
}

template<typename T, typename E>
constexpr bool operator>=(const expected<T,E>& x, const expected<T,E>& y)noexcept(noexcept(x < y)){
  return !(x < y);
}
template<typename T, typename E>
constexpr bool operator==(const expected<T,E>& x, const T& v)noexcept(noexcept(std::declval<T>() == std::declval<T>())){
  return x && *x == v;
}
template<typename T, typename E>
constexpr bool operator==(const T& v, const expected<T, E>& x)noexcept(noexcept(std::declval<T>() == std::declval<T>())){
  return x == v;
}
template<typename T, typename E>
constexpr bool operator!=(const expected<T,E>& x, const T& v)noexcept(noexcept(std::declval<T>() == std::declval<T>())){
  return !(x == v);
}
template<typename T, typename E>
constexpr bool operator!=(const T& v, const expected<T,E>& x)noexcept(noexcept(std::declval<T>() == std::declval<T>())){
  return x != v;
}
template<typename T, typename E>
constexpr bool operator<(const expected<T,E>& x, const T& v)noexcept(noexcept(std::declval<T>() < std::declval<T>())){
  return x || *x < v;
}
template<typename T, typename E>
constexpr bool operator<(const T& v, const expected<T,E>& x)noexcept(noexcept(std::declval<T>() < std::declval<T>())){
  return x && v < *x;
}
template<typename T, typename E>
constexpr bool operator>(const expected<T,E>& x, const T& v)noexcept(noexcept(std::declval<T>() < std::declval<T>())){
  return v < x;
}
template<typename T, typename E>
constexpr bool operator>(const T& v, const expected<T,E>& x)noexcept(noexcept(std::declval<T>() < std::declval<T>())){
  return x < v;
}
template<typename T, typename E>
constexpr bool operator<=(const expected<T,E>& x, const T& v)noexcept(noexcept(std::declval<T>() < std::declval<T>())){
  return !(v < x);
}
template<typename T, typename E>
constexpr bool operator<=(const T& v, const expected<T,E>& x)noexcept(noexcept(std::declval<T>() < std::declval<T>())){
  return !(x < v);
}
template<typename T, typename E>
constexpr bool operator>=(const expected<T,E>& x, const T& v)noexcept(noexcept(std::declval<T>() < std::declval<T>())){
  return !(x < v);
}
template<typename T, typename E>
constexpr bool operator>=(const T& v, const expected<T,E>& x)noexcept(noexcept(std::declval<T>() < std::declval<T>())){
  return !(v < x);
}
template<typename T, typename E>
constexpr bool operator==(const expected<T,E>& x, const unexpected_type<E>& e)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() == std::declval<E>())){
  return !x && x.get_unexpected() == e;
}
template<typename T, typename E>
constexpr bool operator==(const unexpected_type<E>& e, const expected<T,E>& x)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() == std::declval<E>())){
  return x == e;
}
template<typename T, typename E>
constexpr bool operator!=(const expected<T,E>& x, const unexpected_type<E>& e)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() == std::declval<E>())){
  return !(x == e);
}
template<typename T, typename E>
constexpr bool operator!=(const unexpected_type<E>& e, const expected<T,E>& x)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() == std::declval<E>())){
  return !(x == e);
}
template<typename T, typename E>
constexpr bool operator<(const expected<T,E>& x, const unexpected_type<E>& e)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() < std::declval<E>())){
  return !x && x.get_unexpected() < e;
}
template<typename T, typename E>
constexpr bool operator<(const unexpected_type<E>& e, const expected<T,E>& x)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() < std::declval<E>())){
  return !x || e < x.get_unexpected();
}
template<typename T, typename E>
constexpr bool operator>(const expected<T,E>& x, const unexpected_type<E>& e)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() < std::declval<E>())){
  return e < x;
}
template<typename T, typename E>
constexpr bool operator>(const unexpected_type<E>& e, const expected<T,E>& x)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() < std::declval<E>())){
  return x < e;
}
template<typename T, typename E>
constexpr bool operator<=(const expected<T,E>& x, const unexpected_type<E>& e)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() < std::declval<E>())){
  return !(e < x);
}
template<typename T, typename E>
constexpr bool operator<=(const unexpected_type<E>& e, const expected<T,E>& x)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() < std::declval<E>())){
  return !(x < e);
}
template<typename T, typename E>
constexpr bool operator>=(const expected<T,E>& x, const unexpected_type<E>& e)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() < std::declval<E>())){
  return !(e > x);
}
template<typename T, typename E>
constexpr bool operator>=(const unexpected_type<E>& e, const expected<T,E>& x)noexcept(std::is_nothrow_copy_constructible<E>::value && noexcept(std::declval<E>() < std::declval<E>())){
  return !(x > e);
}
template<typename T, typename E>
void swap(expected<T,E>& x, expected<T,E>& y)noexcept(noexcept(x.swap(y))){
  x.swap(y);
}
template<typename T>
constexpr expected<std::decay_t<T>> make_expected(T&& v)noexcept(std::is_nothrow_constructible<expected<std::decay_t<T>>, T&&>::value){
  return expected<std::decay_t<T>>(std::forward<T>(v));
}
inline expected<void> make_expected()noexcept{
  return {};
}
template<typename T>
inline expected<T> make_expected_from_current_exception()noexcept{
  return expected<T>(make_unexpected_from_current_exception());
}
template<typename T>
inline expected<T> make_expected_from_exception(std::exception_ptr e)noexcept(std::is_nothrow_move_constructible<std::exception_ptr>::value){
  return expected<T>(unexpected_type<>(e));
}
template<typename T, typename E>
inline expected<T> make_expected_from_exception(E&& e)noexcept(std::is_nothrow_constructible<std::exception_ptr, E&&>::value){
  return expected<T>(unexpected_type<>(std::forward<E>(e)));
}
template<typename T, typename E>
constexpr expected<T, std::decay_t<E>> make_expected_from_error(E&& e)noexcept(std::is_nothrow_constructible<std::decay_t<E>, E&&>::value){
  return expected<T, std::decay_t<E>>(make_unexpected(std::forward<E>(e)));
}
template<typename T, typename E, typename U>
constexpr expected<T, E> make_expected_from_error(U&& u)noexcept(std::is_nothrow_constructible<E, U&&>::value && std::is_nothrow_move_constructible<E>::value){
  return expected<T, E>(make_unexpected(E(std::forward<U>(u))));
}
template<typename F, std::enable_if_t<!std::is_same<std::result_of_t<F()>, void>::value>* = nullptr>
inline expected<std::result_of_t<F()>> make_expected_from_call(F&& f)noexcept try{
  return make_expected(f());
}
catch(...){
  return make_unexpected_from_current_exception();
}
template<typename F, std::enable_if_t<std::is_same<std::result_of_t<F()>, void>::value>* = nullptr>
inline expected<void> make_expected_from_call(F&& f)noexcept try{
  f();
  return make_expected();
}
catch(...){
  return make_unexpected_from_current_exception();
}
template<typename T, typename E>
constexpr unexpected_type<E> make_unexpected(expected<T,E>& ex)noexcept(std::is_nothrow_copy_constructible<E>::value){
  return unexpected_type<E>(ex.error());
}

template<typename Ex, typename F, std::enable_if_t<!std::is_same<std::decay_t<decltype(std::declval<F>()())>, void>::value>* = nullptr>
auto do_(F&& f)->expected<decltype(f()), Ex>
try{
  return f();
}catch(...){
  return make_unexpected(error_traits<Ex>::make_error_from_current_exception());
}

template<typename Ex, typename F, std::enable_if_t<std::is_same<std::decay_t<decltype(std::declval<F>()())>, void>::value>* = nullptr>
auto do_(F&& f)->expected<void, Ex>
try{
  f();
  return {};
}catch(...){
  return make_unexpected(error_traits<Ex>::make_error_from_current_exception());
}

template<typename F, std::enable_if_t<!std::is_same<std::decay_t<decltype(std::declval<F>()())>, void>::value>* = nullptr>
auto do_(F&& f)->expected<decltype(f())>
try{
  return f();
}catch(...){
  return make_unexpected_from_current_exception();
}

template<typename F, std::enable_if_t<std::is_same<std::decay_t<decltype(std::declval<F>()())>, void>::value>* = nullptr>
auto do_(F&& f)->expected<void>
try{
  f();
  return {};
}catch(...){
  return make_unexpected_from_current_exception();
}

}//End : namespace will
