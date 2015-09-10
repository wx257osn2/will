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
	using frame = detail::resource<IWICBitmapFrameDecode>;
	struct decoder : detail::resource<IWICBitmapDecoder>{
		using resource::resource;
		enum class access:DWORD{read = GENERIC_READ, write = GENERIC_WRITE};
		frame get_frame(UINT f = 0u){return frame(com_create_resource<IWICBitmapFrameDecode>(std::bind(std::mem_fn(&IWICBitmapDecoder::GetFrame), get(), f, std::placeholders::_1)));}
	};
	decoder make_decoder(LPCWSTR filename, decoder::access read_or_write = decoder::access::read, WICDecodeOptions decop = WICDecodeMetadataCacheOnLoad){
		return decoder(com_create_resource<IWICBitmapDecoder>(std::bind(std::mem_fn(&IWICImagingFactory::CreateDecoderFromFilename), get(), filename, nullptr, static_cast<DWORD>(read_or_write), decop, std::placeholders::_1)));
	}
	bitmap make_bitmap(unsigned int w, unsigned int h, GUID pixel_format = GUID_WICPixelFormat32bppPBGRA, WICBitmapCreateCacheOption decop = WICBitmapCacheOnLoad){
		return bitmap(com_create_resource<IWICBitmap>(std::bind(std::mem_fn(&IWICImagingFactory::CreateBitmap), get(), w, h, pixel_format, decop, std::placeholders::_1)));
	}
	converter create_converter(const frame& fram, GUID pixel_format = GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherType dither = WICBitmapDitherTypeNone, IWICPalette* palette = nullptr, double alpha = 0., WICBitmapPaletteType palette_type = WICBitmapPaletteTypeMedianCut){
		return converter(com_create_resource<IWICFormatConverter>([&](IWICFormatConverter** t){(*this)->CreateFormatConverter(t);return (*t)->Initialize(fram.get(), pixel_format, dither, palette, alpha, palette_type);}));
	}
};
}