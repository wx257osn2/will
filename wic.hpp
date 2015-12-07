#pragma once
#include"com.hpp"
#include"_resouce_property.hpp"
#include"_2dim.hpp"
#include<functional>
#include<wincodec.h>
#include<wincodecsdk.h>
#include<d2d1_1.h>
#pragma comment(lib, "WIndowsCodecs.lib")
namespace will{
struct wic : detail::resource<IWICImagingFactory2>{
	explicit wic():resource(com_create_resource<IWICImagingFactory2>([](IWICImagingFactory2** t){return ::CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory2, reinterpret_cast<void**>(t));})){}
	using bitmap = detail::resource<IWICBitmap>;
	using converter = detail::resource<IWICFormatConverter>;
	struct decoder : detail::resource<IWICBitmapDecoder>{
		struct frame : detail::resource<IWICBitmapFrameDecode>{
			using resource::resource;
			will::two_dim::xy<unsigned int> get_size()const{will::two_dim::xy<unsigned int> size; (*this)->GetSize(&size.x, &size.y); return size;}
		};
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
	converter create_converter(const decoder::frame& fram, GUID pixel_format = GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherType dither = WICBitmapDitherTypeNone, IWICPalette* palette = nullptr, double alpha = 0., WICBitmapPaletteType palette_type = WICBitmapPaletteTypeMedianCut){
		return converter(com_create_resource<IWICFormatConverter>([&](IWICFormatConverter** t){(*this)->CreateFormatConverter(t);return (*t)->Initialize(fram.get(), pixel_format, dither, palette, alpha, palette_type);}));
	}
	converter create_converter(LPCWSTR filename, decoder::access access = decoder::access::read, WICDecodeOptions decop = WICDecodeMetadataCacheOnLoad){
		return create_converter(create_decoder(filename, access, decop).get_frame());
	}
	struct stream : detail::resource<IWICStream>{
		stream(IWICStream* ptr, LPCWSTR filename):resource(ptr){(*this)->InitializeFromFilename(filename, GENERIC_WRITE);}
		stream(stream&&) = default;
		stream& operator=(stream&&) = default;
		~stream(){(*this)->Commit(STGC_DEFAULT);}
	};
	struct encoder : detail::resource<IWICImageEncoder>{
		struct frame : detail::resource<IWICBitmapFrameEncode>{
			explicit frame(IWICBitmapFrameEncode* ptr, IPropertyBag2* prop = nullptr):resource(ptr){(*this)->Initialize(prop);}
			frame(frame&&) = default;
			frame& operator=(frame&&) = default;
			~frame(){if(*this)(*this)->Commit();}
		};
		struct bitmap : detail::resource<IWICBitmapEncoder>{
			bitmap(IWICBitmapEncoder* ptr, const stream& stream, WICBitmapEncoderCacheOption cache_option = WICBitmapEncoderNoCache):resource(ptr){(*this)->Initialize(stream.get(), cache_option);}
			bitmap(bitmap&&) = default;
			bitmap& operator=(bitmap&&) = default;
			frame create_frame(){return frame{com_create_resource<IWICBitmapFrameEncode>([&](IWICBitmapFrameEncode** ptr){return (*this)->CreateNewFrame(ptr, nullptr);})};}
			frame create_frame(IPropertyBag2*& prop){return frame{com_create_resource<IWICBitmapFrameEncode>([&](IWICBitmapFrameEncode** ptr){return (*this)->CreateNewFrame(ptr, &prop);})};}
			~bitmap(){(*this)->Commit();}
		};
		using resource::resource;
		void write(ID2D1Bitmap1* bmp, frame& frame, const WICImageParameters* params = nullptr){(*this)->WriteFrame(bmp, frame.get(), params);}
		template<typename D2DBitmap>
		void write(D2DBitmap&& bmp, frame& frame, const WICImageParameters* params = nullptr){this->write(std::forward<D2DBitmap>(bmp).get(), frame, params);}
	};
	stream create_stream(LPCWSTR filename){return stream{com_create_resource<IWICStream>([&](IWICStream** ptr){return (*this)->CreateStream(ptr);}), filename};}
	encoder::bitmap create_bitmap_encoder(const stream& stream, const GUID& encode = GUID_ContainerFormatPng, WICBitmapEncoderCacheOption cache_option = WICBitmapEncoderNoCache){return encoder::bitmap{com_create_resource<IWICBitmapEncoder>([&](IWICBitmapEncoder** ptr){return (*this)->CreateEncoder(encode, nullptr, ptr);}), stream, cache_option};}
	encoder create_encoder(ID2D1Device* device){return encoder{com_create_resource<IWICImageEncoder>([&](IWICImageEncoder** ptr){return (*this)->CreateImageEncoder(device, ptr);})};}
	template<typename D2DDevice>
	encoder create_encoder(D2DDevice&& device){return create_encoder(std::forward<D2DDevice>(device).get());}
	template<typename D2DDevice, typename D2DBitmap>
	void encode_to_file(LPCWSTR filename, D2DDevice&& device, D2DBitmap&& bmp, const GUID& format = GUID_ContainerFormatPng){
		auto stream = create_stream(filename);
		auto bitmap_encoder = create_bitmap_encoder(stream);
		auto frame_encoder = bitmap_encoder.create_frame();
		auto encoder = create_encoder(std::forward<D2DDevice>(device));
		encoder.write(std::forward<D2DBitmap>(bmp), frame_encoder);
	}
};
}