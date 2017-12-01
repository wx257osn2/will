//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef WILL_INCLUDING_DBGHELP_HPP
#pragma once
#ifdef WILL_USE_STACK_TRACE
#include"dbghelp.hpp"

namespace will{

inline const char* runtime_error::what()const noexcept{
	if(cache.empty())
		try{
			auto sfs = will::symbol_handler::create().map(will::stack_frames::to_stack_frames(sf));
			if(!sfs)
				return message();
			std::stringstream ss;
			ss << exception_name() << ": " << message() << '\n';
			ss << "----------------\n"
			      "STACK BACK TRACE\n"
			      "----------------\n";
			ss << *sfs << std::endl;
			cache = std::move(ss.str());
		}catch(...){
			return message();
		}
	return cache.c_str();
}
inline will::stack_frames runtime_error::stack_frames(symbol_handler&& symh)const noexcept{
	return will::stack_frames::to_stack_frames(sf)(std::move(symh));
}

inline will::stack_frames winapi_last_error::stack_frames(symbol_handler&& symh)const noexcept{
	return will::stack_frames::to_stack_frames(sf)(std::move(symh));
}

inline const char* winapi_last_error_exception::what()const noexcept{
	if(cache.empty())
		try{
			auto symh = will::symbol_handler::create();
			if(!symh)
				return message();
			auto stack_frame = value().stack_frames(std::move(*symh));
			if(stack_frame.empty())
				return message();
			std::stringstream ss;
			ss << runtime_error::what() << '\n';
			ss << "------------------------------\n"
			      "STACK BACK TRACE (INNER ERROR)\n"
			      "------------------------------\n";
			ss << stack_frame << std::endl;
			cache = std::move(ss.str());
		}catch(...){
			return message();
		}
	return cache.c_str();
}

}

#endif
#endif