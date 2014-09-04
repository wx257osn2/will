#pragma once
#include"com.hpp"
#include<wincodec.h>
#include<wincodecsdk.h>
#pragma comment(lib, "WIndowsCodecs.lib")
namespace will{
class wic{
	com_ptr<IWICImagingFactory> factory;
public:
	wic():factory(com_create_resource<IWICImagingFactory>([](IWICImagingFactory** t){return ::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(t));})){}
	wic(const wic&) = default;
	wic(wic&&) = default;
	~wic() = default;
	IWICImagingFactory* operator->(){return factory.get();}
	class bitmap{
		wic& fac;
		com_ptr<IWICBitmap> bm;
	public:
		bitmap() = delete;
		bitmap(wic& w, IWICBitmap* t):fac(w), bm(t){}
		bitmap(const bitmap&) = default;
		bitmap(bitmap&&) = default;
		~bitmap() = default;
		IWICBitmap* get(){return bm.get();}
	};
	class converter{
		wic& fac;
		com_ptr<IWICFormatConverter> conv;
	public:
		converter() = delete;
		converter(wic& w, IWICFormatConverter* t):fac(w), conv(t){}
		converter(const converter&) = default;
		converter(converter&&) = default;
		~converter() = default;
		IWICFormatConverter* get()const{return conv.get();}
	};
	class frame{
		wic& fac;
		com_ptr<IWICBitmapFrameDecode> fram;
	public:
		frame() = delete;
		frame(wic& w, IWICBitmapFrameDecode* t):fac(w), fram(t){}
		frame(const frame&) = default;
		frame(frame&&) = default;
		~frame() = default;
		converter create_converter(GUID pixel_format = GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherType dither = WICBitmapDitherTypeNone, IWICPalette* palette = nullptr, double alpha = 0., WICBitmapPaletteType palette_type = WICBitmapPaletteTypeMedianCut){
			return converter(fac, fram ? com_create_resource<IWICFormatConverter>([&](IWICFormatConverter** t){this->fac->CreateFormatConverter(t);return (*t)->Initialize(fram.get(), pixel_format, dither, palette, alpha, palette_type);}) : nullptr);
		}
		converter operator()(GUID pixel_format = GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherType dither = WICBitmapDitherTypeNone, IWICPalette* palette = nullptr, double alpha = 0., WICBitmapPaletteType palette_type = WICBitmapPaletteTypeMedianCut){return create_converter(pixel_format, dither, palette, alpha, palette_type);}
	};
	class decoder{
		wic& fac;
		com_ptr<IWICBitmapDecoder> dec;
	public:
		enum class access:DWORD{read = GENERIC_READ, write = GENERIC_WRITE};
		decoder() = delete;
		decoder(wic& w, IWICBitmapDecoder* t):fac(w), dec(t){}
		decoder(const decoder&) = default;
		decoder(decoder&&) = default;
		~decoder() = default;
		frame get_frame(UINT f = 0u){return frame(fac, dec ? com_create_resource<IWICBitmapFrameDecode>(std::bind(std::mem_fn(&IWICBitmapDecoder::GetFrame), dec.get(), f, std::placeholders::_1)) : nullptr);}
		frame operator()(UINT f = 0u){return get_frame(f);}
	};
	decoder make_decoder(LPCWSTR filename, decoder::access read_or_write = decoder::access::read, WICDecodeOptions decop = WICDecodeMetadataCacheOnLoad){
		return decoder(*this, factory ? com_create_resource<IWICBitmapDecoder>(std::bind(std::mem_fn(&IWICImagingFactory::CreateDecoderFromFilename), factory.get(), filename, nullptr, static_cast<DWORD>(read_or_write), decop, std::placeholders::_1)) : nullptr);
	}
	decoder operator()(LPCWSTR filename){return make_decoder(filename);}
		bitmap make_bitmap(unsigned int w, unsigned int h, GUID pixel_format = GUID_WICPixelFormat32bppPBGRA, WICBitmapCreateCacheOption decop = WICBitmapCacheOnLoad){
		return bitmap(*this, factory ? com_create_resource<IWICBitmap>(std::bind(std::mem_fn(&IWICImagingFactory::CreateBitmap), factory.get(), w, h, pixel_format, decop, std::placeholders::_1)) : nullptr);
	}
};
}