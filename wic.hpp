#pragma once
#include"com.hpp"
#include<functional>
#include<wincodec.h>
#include<wincodecsdk.h>
#pragma comment(lib, "WIndowsCodecs.lib")
namespace will{
class wic{
	com_ptr<IWICImagingFactory> factory;
public:
	wic():factory(com_create_resource<IWICImagingFactory>([](IWICImagingFactory** t){return ::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(t));})){}
	wic(const wic&) = delete;
	wic(wic&&) = default;
	~wic() = default;
	wic& operator=(const wic&) = delete;
	wic& operator=(wic&&) = default;
	IWICImagingFactory* get()const{return factory.get();}
	IWICImagingFactory* operator->()const{return factory.get();}
	explicit operator bool()const{return static_cast<bool>(factory);}
	class bitmap{
		com_ptr<IWICBitmap> bm;
	public:
		explicit bitmap(IWICBitmap* t):bm(t){}
		bitmap(const bitmap&) = delete;
		bitmap(bitmap&&) = default;
		~bitmap() = default;
		bitmap& operator=(const bitmap&) = delete;
		bitmap& operator=(bitmap&&) = default;
		IWICBitmap* get()const{return bm.get();}
		IWICBitmap* operator->()const{return bm.get();}
		explicit operator bool()const{return static_cast<bool>(bm);}
	};
	class converter{
		com_ptr<IWICFormatConverter> conv;
	public:
		explicit converter(IWICFormatConverter* t):conv(t){}
		converter(const converter&) = delete;
		converter(converter&&) = default;
		~converter() = default;
		converter& operator=(const converter&) = delete;
		converter& operator=(converter&&) = default;
		IWICFormatConverter* get()const{return conv.get();}
		IWICFormatConverter* operator->()const{return conv.get();}
		explicit operator bool()const{return static_cast<bool>(conv);}
	};
	class frame{
		com_ptr<IWICBitmapFrameDecode> fram;
	public:
		explicit frame(IWICBitmapFrameDecode* t):fram(t){}
		frame(const frame&) = delete;
		frame(frame&&) = default;
		~frame() = default;
		frame& operator=(const frame&) = delete;
		frame& operator=(frame&&) = default;
		IWICBitmapFrameDecode* get()const{return fram.get();}
		IWICBitmapFrameDecode* operator->()const{return fram.get();}
		explicit operator bool()const{return static_cast<bool>(fram);}
	};
	class decoder{
		com_ptr<IWICBitmapDecoder> dec;
	public:
		explicit decoder(IWICBitmapDecoder* t):dec(t){}
		enum class access:DWORD{read = GENERIC_READ, write = GENERIC_WRITE};
		decoder(const decoder&) = delete;
		decoder(decoder&&) = default;
		~decoder() = default;
		decoder& operator=(const decoder&) = delete;
		decoder& operator=(decoder&&) = default;
		IWICBitmapDecoder* get()const{return dec.get();}
		IWICBitmapDecoder* operator->()const{return dec.get();}
		frame get_frame(UINT f = 0u){return frame(com_create_resource<IWICBitmapFrameDecode>(std::bind(std::mem_fn(&IWICBitmapDecoder::GetFrame), dec.get(), f, std::placeholders::_1)));}
		explicit operator bool()const{return static_cast<bool>(dec);}
	};
	decoder make_decoder(LPCWSTR filename, decoder::access read_or_write = decoder::access::read, WICDecodeOptions decop = WICDecodeMetadataCacheOnLoad){
		return decoder(com_create_resource<IWICBitmapDecoder>(std::bind(std::mem_fn(&IWICImagingFactory::CreateDecoderFromFilename), factory.get(), filename, nullptr, static_cast<DWORD>(read_or_write), decop, std::placeholders::_1)));
	}
	bitmap make_bitmap(unsigned int w, unsigned int h, GUID pixel_format = GUID_WICPixelFormat32bppPBGRA, WICBitmapCreateCacheOption decop = WICBitmapCacheOnLoad){
		return bitmap(com_create_resource<IWICBitmap>(std::bind(std::mem_fn(&IWICImagingFactory::CreateBitmap), factory.get(), w, h, pixel_format, decop, std::placeholders::_1)));
	}
	converter create_converter(const frame& fram, GUID pixel_format = GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherType dither = WICBitmapDitherTypeNone, IWICPalette* palette = nullptr, double alpha = 0., WICBitmapPaletteType palette_type = WICBitmapPaletteTypeMedianCut){
		return converter(com_create_resource<IWICFormatConverter>([&](IWICFormatConverter** t){factory->CreateFormatConverter(t);return (*t)->Initialize(fram.get(), pixel_format, dither, palette, alpha, palette_type);}));
	}
};
}