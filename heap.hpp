//Copyright (C) 2014-2017, 2019 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_windows.hpp"
#include"_resource_property.hpp"
#include<vector>
#include<memory>
namespace will{

class heap{
protected:
	HANDLE h;
public:
	explicit heap(HANDLE h):h(h){}
	heap(const heap&) = default;
	heap(heap&&) = default;
	heap& operator=(const heap&) = default;
	heap& operator=(heap&&) = default;
	~heap() = default;
	explicit operator bool()const{return h != nullptr;}
	explicit operator HANDLE()const{return h;}
	HANDLE get()const{return h;}
	template<typename T>
	class deleter{
		HANDLE h;
	public:
		explicit deleter(const heap& handle):h(handle.h){}
		void operator()(T* ptr){ptr->~T();heap heap{h};free(heap, 0, ptr);}
		friend class memory;
	};
	enum class allocate_option:DWORD{
		generate_exceptions = HEAP_GENERATE_EXCEPTIONS,
		no_serialize = HEAP_NO_SERIALIZE,
		init_by_0 = HEAP_ZERO_MEMORY
	};
	struct allocate_property:detail::property<allocate_option>{
#define DECLPROP(name) allocate_property& name(){prop = static_cast<allocate_option>(static_cast<std::underlying_type_t<allocate_option>>(prop) | static_cast<std::underlying_type_t<allocate_option>>(allocate_option::name)); return *this;}
		DECLPROP(generate_exceptions)
		DECLPROP(no_serialize)
		DECLPROP(init_by_0)
#undef DECLPROP
	};
	enum class reallocate_option:DWORD{
		generate_exceptions = HEAP_GENERATE_EXCEPTIONS,
		no_serialize = HEAP_NO_SERIALIZE,
		init_by_0 = HEAP_ZERO_MEMORY,
		in_place_only = HEAP_REALLOC_IN_PLACE_ONLY
	};
	struct reallocate_property:detail::property<reallocate_option>{
#define DECLPROP(name) reallocate_property& name(){prop = static_cast<reallocate_option>(static_cast<std::underlying_type_t<reallocate_option>>(prop) | static_cast<std::underlying_type_t<reallocate_option>>(reallocate_option::name)); return *this;}
		DECLPROP(generate_exceptions)
		DECLPROP(no_serialize)
		DECLPROP(init_by_0)
		DECLPROP(in_place_only)
#undef DECLPROP
	};
	static void* allocate(heap& heap, std::size_t size, allocate_option op = static_cast<allocate_option>(0)){return ::HeapAlloc(heap.h, static_cast<std::underlying_type_t<allocate_option>>(op), size);}
	static void* reallocate(heap& heap, void* addr, std::size_t new_size, reallocate_option op = static_cast<reallocate_option>(0)){return ::HeapReAlloc(heap.h, static_cast<std::underlying_type_t<reallocate_option>>(op), addr, new_size);}
	static bool expand_size(heap& heap, void* addr, std::size_t new_size)noexcept{try{return reallocate(heap, addr, new_size, reallocate_option::in_place_only) != nullptr;}catch(...){return false;}}
	static bool expand_plus(heap& heap, void* addr, std::size_t add_size)noexcept{try{const auto size = get_size(heap, addr);return size != -1 ? reallocate(heap, addr, size + add_size, reallocate_option::in_place_only) != nullptr : false;}catch(...){return false;}}
	static std::size_t get_size(const heap& heap, void* addr, bool serialize = true){return ::HeapSize(heap.h, serialize ? 0 : HEAP_NO_SERIALIZE, addr);}
	static expected<void, winapi_last_error> free(heap& heap, void* addr, bool serialize = true){if(::HeapFree(heap.h, serialize ? 0 : HEAP_NO_SERIALIZE, addr) != 0)return {};return make_unexpected<winapi_last_error>(_T(__FUNCTION__));}
	static expected<std::size_t, winapi_last_error> compact(heap& heap, bool serialize = true){const auto ret = ::HeapCompact(heap.h, serialize ? 0 : HEAP_NO_SERIALIZE);if(ret != 0)return ret;return make_unexpected<winapi_last_error>(_T(__FUNCTION__));}
	static expected<void, winapi_last_error> enable_terminate_on_corruption(heap& heap){if(::HeapSetInformation(heap.h, ::HeapEnableTerminationOnCorruption, nullptr, 0) != 0)return {};return make_unexpected<winapi_last_error>(_T(__FUNCTION__));}
	static expected<void, winapi_last_error> enable_terminate_on_corruption(){if(::HeapSetInformation(nullptr, ::HeapEnableTerminationOnCorruption, nullptr, 0) != 0)return {};return make_unexpected<winapi_last_error>(_T(__FUNCTION__));}
#ifdef HEAP_OPTIMIZE_RESOURCES_CURRENT_VERSION
	static expected<HEAP_OPTIMIZE_RESOURCES_INFORMATION, winapi_last_error> optimize_resource(heap& heap){
		HEAP_OPTIMIZE_RESOURCES_INFORMATION info;
		if(::HeapSetInformation(heap.h, ::HeapOptimizeResources, &info, sizeof(info)) != 0)
			return info;
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	}
#endif
	static expected<void, winapi_last_error> lock(heap& heap){
		if(::HeapLock(heap.h) == 0)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return {};
	}
	static expected<void, winapi_last_error> unlock(heap& heap){
		if(::HeapUnlock(heap.h) == 0)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return {};
	}
	class process_heap_entry_iterator{
		HANDLE h;
		PROCESS_HEAP_ENTRY entry;
		bool valid;
		DWORD le;
	public:
		explicit process_heap_entry_iterator(HANDLE handle):h(handle), le(ERROR_NO_MORE_ITEMS){}
		process_heap_entry_iterator(const process_heap_entry_iterator&) = default;
		process_heap_entry_iterator(process_heap_entry_iterator&&) = default;
		process_heap_entry_iterator& operator=(const process_heap_entry_iterator&) = default;
		process_heap_entry_iterator& operator=(process_heap_entry_iterator&&) = default;
		using value_type = PROCESS_HEAP_ENTRY;
		value_type& operator*(){return *value();}
		const value_type& operator*()const{return *value();}
		expected<value_type&, winapi_last_error> value(){if(!valid && le != ERROR_NO_MORE_ITEMS)make_unexpected<winapi_last_error>(_T(__FUNCTION__), le);return entry;}
		expected<const value_type&, winapi_last_error> value()const{if(!valid && le != ERROR_NO_MORE_ITEMS)make_unexpected<winapi_last_error>(_T(__FUNCTION__), le);return entry;}
		bool operator==(const process_heap_entry_iterator& rhs)const{return valid == rhs.valid && le == rhs.le;}
		process_heap_entry_iterator& operator++(){valid = ::HeapWalk(h, &entry) != 0;if(!valid)le = ::GetLastError();return *this;}
		process_heap_entry_iterator operator++(int){auto ret = *this;++*this;return ret;}
	};
	struct unlocker{
		HANDLE h;
		bool locked;
		friend heap;
		unlocker(HANDLE handle):h(handle), locked(true){}
	public:
		process_heap_entry_iterator begin()const{return ++end();}
		process_heap_entry_iterator end()const{return process_heap_entry_iterator{h};}
		template<typename F>
		expected<void, winapi_last_error> walk(F&& f){
			for(auto it = begin(); it != end(); ++it){
				auto&& v = it.value();
				if(!v)
					return v.get_unexpected();
				f(v);
			}
			return {};
		}
		expected<void, winapi_last_error> unlock(){
			if(locked){
				heap heap{h};
				return heap::unlock(heap).map([&]{locked = false;});
			}
			return {};
		}
		~unlocker(){unlock();}
	};
	static expected<unlocker, winapi_last_error> scoped_lock(heap& heap){
		return lock(heap).map([&]{return unlocker{heap.h};});
	}
	static bool validate(heap& heap, bool serialize = true, const void* block = nullptr){return ::HeapValidate(heap.h, serialize ? 0 : HEAP_NO_SERIALIZE, block) != 0;}
	template<typename T, typename... Args>
	std::unique_ptr<T, deleter<T>> make_unique(Args&&... args){
		auto ptr = heap::allocate(*this, sizeof(T));
		if(ptr)
			new(ptr) T{std::forward<Args>(args)...};
		return std::unique_ptr<T, deleter<T>>{ptr, deleter<T>{*this}};
	}
	class memory : public std::unique_ptr<void, deleter<void>>{
		heap get_heap()const{return heap{get_deleter().h};}
	public:
		using unique_ptr::unique_ptr;
		void* reallocate(std::size_t new_size, reallocate_option op = reallocate_option{}){heap heap = get_heap();return heap::reallocate(heap, get(), new_size, op);}
		bool expand_size(std::size_t new_size)noexcept{heap heap = get_heap();return heap::expand_size(heap, get(), new_size);}
		bool expand_plus(std::size_t add_size)noexcept{heap heap = get_heap();return heap::expand_plus(heap, get(), add_size);}
		std::size_t size()const{return heap::get_size(get_heap(), get());}
		friend heap;
	};
	memory allocate(std::size_t size, allocate_option op = allocate_option{}){return memory{heap::allocate(*this, size, op), deleter<void>{*this}};}
	expected<std::size_t, winapi_last_error> compact(bool serialize = true){return heap::compact(*this, serialize);}
};

inline heap get_process_heap(){return heap{::GetProcessHeap()};}
inline std::vector<heap> get_process_heaps(){
	std::vector<HANDLE> ret(::GetProcessHeaps(0, nullptr));
	::GetProcessHeaps(static_cast<DWORD>(ret.size()), ret.data());
	std::vector<heap> vec(ret.begin(), ret.end());
	return vec;
}

class private_heap : public heap{
	using heap::heap;
public:
	enum class create_option:DWORD{
		generate_exceptions = HEAP_GENERATE_EXCEPTIONS,
		no_serialize = HEAP_NO_SERIALIZE,
		executable = HEAP_CREATE_ENABLE_EXECUTE,
		trace = HEAP_CREATE_ENABLE_TRACING,
		align16 = HEAP_CREATE_ALIGN_16
	};
	struct create_property:detail::property<create_option>{
#define DECLPROP(name) create_property& name(){prop = static_cast<create_option>(static_cast<std::underlying_type_t<create_option>>(prop) | static_cast<std::underlying_type_t<create_option>>(create_option::name)); return *this;}
		DECLPROP(generate_exceptions)
		DECLPROP(no_serialize)
		DECLPROP(executable)
		DECLPROP(trace)
		DECLPROP(align16)
#undef DECLPROP
	};
	static expected<private_heap, winapi_last_error> create(std::size_t init_size = 0, std::size_t max_size = 0, create_option op = create_option{}){return detail::checked_return(_T(__FUNCTION__), ::HeapCreate(static_cast<std::underlying_type_t<create_option>>(op), init_size, max_size), nullptr).map([](HANDLE h){return private_heap{h};});}
	explicit private_heap(std::size_t init_size = 0, std::size_t max_size = 0, create_option op = create_option{}):private_heap{+create(init_size, max_size, op)}{}
	private_heap(const private_heap&) = delete;
	private_heap(private_heap&& other)noexcept:heap(other.release()){}
	private_heap(const heap&) = delete;
	private_heap(heap&&) = delete;
	private_heap& operator=(const private_heap&) = delete;
	private_heap& operator=(private_heap&& other)noexcept{+destroy().map([&](bool){h = other.release();});return *this;}
	private_heap& operator=(const heap&) = delete;
	private_heap& operator=(heap&&) = delete;
	HANDLE release(){
		auto ret = h;
		h = nullptr;
		return ret;
	}
	expected<bool, winapi_last_error> destroy(){
		if(h == nullptr)
			return false;
		if(::HeapDestroy(h) == 0)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		h = nullptr;
		return true;
	}
	~private_heap(){destroy();}
};

template<typename Deleter>
class moveable_memory{
	static expected<void*, winapi_last_error> lock(HGLOBAL h){
		void* t = ::GlobalLock(h);
		if(t == nullptr)
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return t;
	}
	static expected<unsigned int, winapi_last_error> unlock(HGLOBAL h){
		::BOOL t = ::GlobalUnlock(h);
		if(t == 0){
			::DWORD le = ::GetLastError();
			if(le != NO_ERROR)
				return make_unexpected<winapi_last_error>(_T(__FUNCTION__), le);
		}
		return static_cast<unsigned int>(t);
	}
	Deleter d;
protected:
	HGLOBAL h;
public:
	explicit moveable_memory(HGLOBAL h):h(h){}
	moveable_memory(const moveable_memory&) = default;
	moveable_memory(moveable_memory&&) = default;
	moveable_memory& operator=(const moveable_memory&) = default;
	moveable_memory& operator=(moveable_memory&&) = default;
	~moveable_memory(){if(h != nullptr)d(h);}
	explicit operator bool()const{return h != nullptr;}
	explicit operator HGLOBAL()const{return h;}
	HGLOBAL get()const{return h;}
	HGLOBAL release()&&{HGLOBAL ret = h; h = nullptr; return ret;}
	class scoped_lock_t{
		HGLOBAL h;
		void* ptr;
	public:
		scoped_lock_t(HGLOBAL h, void* ptr):h{h}, ptr{ptr}{}
		void* get()const{return ptr;}
		~scoped_lock_t(){unlock(h);}
	};
	will::expected<scoped_lock_t, winapi_last_error> scoped_lock(){
		return lock(h).map([this](void* ptr){return scoped_lock_t{this->h, ptr};});
	}
};

namespace detail{

struct global_free_deleter{
	will::expected<void, winapi_last_error> operator()(HGLOBAL h)const{
		if(::GlobalFree(h))
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return {};
	}
};

}

static inline expected<moveable_memory<detail::global_free_deleter>, winapi_last_error> global_alloc_moveable(std::size_t size){
	HGLOBAL t = ::GlobalAlloc(GMEM_MOVEABLE, size);
	if(t == nullptr)
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	else
		return moveable_memory<detail::global_free_deleter>{std::move(t)};
}

}
