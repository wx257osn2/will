#pragma once
#include"com.hpp"
#include<dwrite_1.h>
#pragma comment(lib, "DWrite.lib")
namespace will{
class dwrite : public detail::resource<IDWriteFactory1>{
	using resource::resource;
public:
	explicit dwrite(DWRITE_FACTORY_TYPE type = DWRITE_FACTORY_TYPE_SHARED):resource(com_create_resource<IDWriteFactory1>([&](IDWriteFactory1** x){return DWriteCreateFactory(type, __uuidof(IDWriteFactory1), reinterpret_cast<IUnknown**>(x));})){}
	struct format : detail::resource<IDWriteTextFormat>{
		using resource::resource;
		class property{
			std::wstring name_;
			FLOAT size_;
			DWRITE_FONT_WEIGHT weight_ = DWRITE_FONT_WEIGHT_NORMAL;
			DWRITE_FONT_STYLE style_ = DWRITE_FONT_STYLE_NORMAL;
			DWRITE_FONT_STRETCH stretch_ = DWRITE_FONT_STRETCH_NORMAL;
			std::wstring locale_ = L"ja-jp";
		public:
			explicit property() = default;
			template<typename WString>
			property(WString&& font_name, FLOAT font_size):name(std::forward<WString>(font_name)), size(font_size){}
	#define PROPERTYDECL(name, type, membername) property& name(type t){membername = t;return *this;}
			template<typename WString>
			property& name(WString&& t){name_ = std::forward<WString>(t);return *this;}
			PROPERTYDECL(size, FLOAT, size_)
			PROPERTYDECL(weight, DWRITE_FONT_WEIGHT, weight_)
			PROPERTYDECL(style, DWRITE_FONT_STYLE, style_)
			PROPERTYDECL(stretch, DWRITE_FONT_STRETCH, stretch_)
			template<typename WString>
			property& locale(WString&& t){locale_ = std::forward<WString>(t);return *this;}
	#undef  PROPERTYDECL
			friend dwrite;
		};
	};
	format create_format(const wchar_t* fontname, FLOAT size, DWRITE_FONT_WEIGHT weight= DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL, const wchar_t* locale = L"ja-jp"){return format{com_create_resource<IDWriteTextFormat>([&](IDWriteTextFormat** ptr){return (*this)->CreateTextFormat(fontname, nullptr, weight, style, stretch, size, locale, ptr);})};}
	format create_format(const format::property& prop){return std::move(create_format(prop.name_.c_str(), prop.size_, prop.weight_, prop.style_, prop.stretch_, prop.locale_.c_str()));}
};
}