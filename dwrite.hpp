#pragma once
#include"com.hpp"
#include<dwrite.h>
#pragma comment(lib, "DWrite.lib")
namespace will{
class dwrite{
	com_ptr<IDWriteFactory> factory;
public:
	dwrite(DWRITE_FACTORY_TYPE type = DWRITE_FACTORY_TYPE_SHARED):factory(com_create_resource<IDWriteFactory>([](auto&& x){return DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(x));})){}
	~dwrite() = default;
	IDWriteFactory* operator->()const{return factory.get();}
	class format{
		com_ptr<IDWriteTextFormat> tf;
	public:
		format(IDWriteTextFormat* t):tf(t){}
		IDWriteTextFormat* operator->()const{return tf.get();}
		IDWriteTextFormat* get(){return tf.get();}
	};
	format create_format(const wchar_t* fontname, FLOAT size, DWRITE_FONT_WEIGHT weight= DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL, const wchar_t* locale = L"ja-jp"){return com_create_resource<IDWriteTextFormat>(std::bind(std::mem_fn(&IDWriteFactory::CreateTextFormat), factory.get(), fontname, nullptr, weight, style, stretch, size, locale, std::placeholders::_1));}
};
}