#pragma once
#include"com.hpp"
#include"_resouce_property.hpp"
#include<functional>
#include<wincodec.h>
#include<wincodecsdk.h>
#pragma comment(lib, "WIndowsCodecs.lib")
namespace will{
struct wic : detail::resource<IWICImagingFactory>{
	wic():resource(com_create_resource<IWICImagingFactory>([](IWICImagingFactory** t){return ::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(t));})){}
	using bitmap = detail::resource<IWICBitmap>;
	using converter = detail::resource<IWICFormatConverter>;
	struct frame : detail::resource<IWICBitmapFrameDecode>{
		using resource::resource;
		will::two_dim::xy<unsigned int> get_size()const{will::two_dim::xy<unsigned int> size; (*this)->GetSize(&size.x, &size.y); return size;}
	};
	struct decoder : detail::resource<IWICBitmapDecoder>{
		using resource::resource;
		enum class access:DWORD{read = GENERIC_READ, write = GENERIC_WRITE};
		frame get_frame(UINT f = 0u)const{return frame(com_create_resource<IWICBitmapFrameDecode>([&](IWICBitmapFrameDecode** ptr){return (*this)->GetFrame(f, ptr);}));}
	};
	decoder create_decoder(LPCWSTR filename, decoder::access read_or_write = decoder::access::read, WICDecodeOptions decop = WICDecodeMetadataCacheOnLoad){
		return decoder(com_create_resource<IWICBitmapDecoder>([&](IWICBitmapDecoder** ptr){return (*this)->CreateDecoderFromFilename(filename, nullptr, static_cast<DWORD>(read_or_write), decop, ptr);}));
	}
	bitmap create_bitmap(unsigned int w, unsigned int h, GUID pixel_format = GUID_WICPixelFormat32bppPBGRA, WICBitmapCreateCacheOption decop = WICBitmapCacheOnLoad){
		return bitmap(com_create_resource<IWICBitmap>([&](IWICBitmap** ptr){return (*this)->CreateBitmap(w, h, pixel_format, decop, ptr);}));
	}
	converter create_converter(const frame& fram, GUID pixel_format = GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherType dither = WICBitmapDitherTypeNone, IWICPalette* palette = nullptr, double alpha = 0., WICBitmapPaletteType palette_type = WICBitmapPaletteTypeMedianCut){
		return converter(com_create_resource<IWICFormatConverter>([&](IWICFormatConverter** t){(*this)->CreateFormatConverter(t);return (*t)->Initialize(fram.get(), pixel_format, dither, palette, alpha, palette_type);}));
	}
	converter create_converter(LPCWSTR filename, decoder::access access = decoder::access::read, WICDecodeOptions decop = WICDecodeMetadataCacheOnLoad){
		return create_converter(create_decoder(filename, access, decop).get_frame());
	}
};
}