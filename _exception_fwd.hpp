//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include<stdexcept>
#include<iosfwd>
#include<vector>
#include<string>

namespace will{

#ifdef WILL_USE_STACK_TRACE
class symbol_handler;
class stack_frames;
inline expected<std::vector<void*>> capture_stack_back_trace(DWORD, DWORD)noexcept;
#endif

class runtime_error:public std::runtime_error{
#ifdef WILL_USE_STACK_TRACE
	std::vector<void*> sf;
protected:
	mutable std::string cache;
	virtual const char* exception_name()const noexcept{return "will::runtime_error";}
public:
	virtual const char* what()const noexcept;
	class stack_frames stack_frames(symbol_handler&&)const noexcept;
#endif
public:
	explicit runtime_error(const char* message) : std::runtime_error(message){
#ifdef WILL_USE_STACK_TRACE
		auto csf = capture_stack_back_trace(2, 62);
		if(csf)
			sf = std::move(*csf);
#endif
	}
	explicit runtime_error(const std::string& message) : runtime_error(message.c_str()){}
	virtual ~runtime_error() = default;
	runtime_error& operator=(const runtime_error&) = default;
	virtual const char* message()const noexcept{return std::runtime_error::what();}
};

}