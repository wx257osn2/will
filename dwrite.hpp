#pragma once
#include"com.hpp"
#include<dwrite_1.h>
#pragma comment(lib, "DWrite.lib")
namespace will{
class dwrite{
	com_ptr<IDWriteFactory1> factory;
public:
	explicit dwrite(DWRITE_FACTORY_TYPE type = DWRITE_FACTORY_TYPE_SHARED):factory(com_create_resource<IDWriteFactory1>([&](IDWriteFactory1** x){return DWriteCreateFactory(type, __uuidof(IDWriteFactory1), reinterpret_cast<IUnknown**>(x));})){}
	dwrite(const dwrite&) = delete;
	dwrite(dwrite&&) = default;
	dwrite& operator=(const dwrite&) = delete;
	dwrite& operator=(dwrite&&) = default;
	~dwrite() = default;
	IDWriteFactory* operator->()const{return factory.get();}
	class format{
		com_ptr<IDWriteTextFormat> tf;
	public:
		explicit format(IDWriteTextFormat* t):tf(t){}
		format(const format&) = delete;
		format(format&&) = default;
		format& operator=(const format&) = delete;
		format& operator=(format&&) = default;
		~format() = default;
		IDWriteTextFormat* operator->()const{return tf.get();}
		IDWriteTextFormat* get()const{return tf.get();}
	};
	format create_format(const wchar_t* fontname, FLOAT size, DWRITE_FONT_WEIGHT weight= DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL, const wchar_t* locale = L"ja-jp"){return format{com_create_resource<IDWriteTextFormat>(std::bind(std::mem_fn(&IDWriteFactory::CreateTextFormat), factory.get(), fontname, nullptr, weight, style, stretch, size, locale, std::placeholders::_1))};}
};
}