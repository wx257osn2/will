//Copyright (C) 2014-2019 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"com.hpp"
#include"_resource_property.hpp"
#include"_2dim.hpp"
#include<functional>
#include<wincodec.h>
#include<wincodecsdk.h>
#include<d2d1_1.h>
#include<vector>
#pragma comment(lib, "WIndowsCodecs.lib")
namespace will{
namespace two_dim{

template<>
struct attribute_traits<::WICRect>{
	using tag_type = tag::point_and_size;
	using point_element_type = ::INT;
	using size_element_type = ::INT;
	static constexpr two_dim::xy<point_element_type> xy(const ::WICRect& t)noexcept{return {t.X, t.Y};}
	static constexpr two_dim::wh<size_element_type> wh(const ::WICRect& t)noexcept{return {t.Width, t.Height};}
	static constexpr point_element_type x(const ::WICRect& t)noexcept{return t.X;}
	static constexpr point_element_type y(const ::WICRect& t)noexcept{return t.Y;}
	static constexpr size_element_type w(const ::WICRect& t)noexcept{return t.Width;}
	static constexpr size_element_type h(const ::WICRect& t)noexcept{return t.Height;}
	static constexpr ::WICRect create(point_element_type x, point_element_type y, size_element_type w, size_element_type h)noexcept{return {x, y, w, h};}
	static constexpr ::WICRect create(const two_dim::xy<point_element_type>& xy, const two_dim::wh<size_element_type>& wh)noexcept{return {xy.x, xy.y, wh.w, wh.h};}
};

}

using property_bag = detail::resource<IPropertyBag2>;

struct wic : detail::resource<IWICImagingFactory2>{
	using resource::resource;
	static unsigned int bits_per_pixel(const WICPixelFormatGUID& pixel_format){
		     if( pixel_format == GUID_WICPixelFormat1bppIndexed
		      || pixel_format == GUID_WICPixelFormatBlackWhite )
			return 1u;
		else if( pixel_format == GUID_WICPixelFormat2bppIndexed
		      || pixel_format == GUID_WICPixelFormat2bppGray )
			return 2u;
		else if( pixel_format == GUID_WICPixelFormat4bppIndexed
		      || pixel_format == GUID_WICPixelFormat4bppGray )
			return 4u;
		else if( pixel_format == GUID_WICPixelFormat8bppIndexed
		      || pixel_format == GUID_WICPixelFormat8bppGray
		      || pixel_format == GUID_WICPixelFormat8bppY
		      || pixel_format == GUID_WICPixelFormat8bppCb
		      || pixel_format == GUID_WICPixelFormat8bppCr )
			return 8u;
		else if( pixel_format == GUID_WICPixelFormat16bppBGR555
		      || pixel_format == GUID_WICPixelFormat16bppBGR565
		      || pixel_format == GUID_WICPixelFormat16bppBGRA5551
		      || pixel_format == GUID_WICPixelFormat16bppGray
		      || pixel_format == GUID_WICPixelFormat16bppGrayFixedPoint
		      || pixel_format == GUID_WICPixelFormat16bppGrayHalf
		      || pixel_format == GUID_WICPixelFormat16bppCbCr )
			return 16u;
		else if( pixel_format == GUID_WICPixelFormat24bppRGB
		      || pixel_format == GUID_WICPixelFormat24bppBGR
		      || pixel_format == GUID_WICPixelFormat24bpp3Channels )
			return 24u;
		else if( pixel_format == GUID_WICPixelFormat32bppBGR101010
		      || pixel_format == GUID_WICPixelFormat32bppRGBA1010102
		      || pixel_format == GUID_WICPixelFormat32bppRGBA1010102XR
		      || pixel_format == GUID_WICPixelFormat32bppGrayFloat
		      || pixel_format == GUID_WICPixelFormat32bppGrayFixedPoint
		      || pixel_format == GUID_WICPixelFormat32bppBGR
		      || pixel_format == GUID_WICPixelFormat32bppRGBA
		      || pixel_format == GUID_WICPixelFormat32bppBGRA
		      || pixel_format == GUID_WICPixelFormat32bppRGBE
	          || pixel_format == GUID_WICPixelFormat32bppPRGBA
		      || pixel_format == GUID_WICPixelFormat32bppPBGRA
		      || pixel_format == GUID_WICPixelFormat32bppRGB
		      || pixel_format == GUID_WICPixelFormat32bpp3ChannelsAlpha )
			return 32u;
		else if( pixel_format == GUID_WICPixelFormat40bppCMYKAlpha
		      || pixel_format == GUID_WICPixelFormat40bpp4ChannelsAlpha
		      || pixel_format == GUID_WICPixelFormat40bpp5Channels )
			return 40u;
		else if( pixel_format == GUID_WICPixelFormat48bppRGB
		      || pixel_format == GUID_WICPixelFormat48bppBGR
		      || pixel_format == GUID_WICPixelFormat48bppRGBFixedPoint
		      || pixel_format == GUID_WICPixelFormat48bppBGRFixedPoint
		      || pixel_format == GUID_WICPixelFormat48bppRGBHalf
		      || pixel_format == GUID_WICPixelFormat48bpp3Channels
		      || pixel_format == GUID_WICPixelFormat48bpp5ChannelsAlpha
		      || pixel_format == GUID_WICPixelFormat48bpp6Channels )
			return 48u;
		else if( pixel_format == GUID_WICPixelFormat56bpp6ChannelsAlpha
		      || pixel_format == GUID_WICPixelFormat56bpp7Channels )
			return 56u;
		else if( pixel_format == GUID_WICPixelFormat64bppRGBA
		      || pixel_format == GUID_WICPixelFormat64bppBGRA
		      || pixel_format == GUID_WICPixelFormat64bppPRGBA
		      || pixel_format == GUID_WICPixelFormat64bppPBGRA
		      || pixel_format == GUID_WICPixelFormat64bppRGBFixedPoint
		      || pixel_format == GUID_WICPixelFormat64bppRGBAFixedPoint
		      || pixel_format == GUID_WICPixelFormat64bppBGRAFixedPoint
		      || pixel_format == GUID_WICPixelFormat64bppRGBHalf
		      || pixel_format == GUID_WICPixelFormat64bppRGBAHalf
	          || pixel_format == GUID_WICPixelFormat64bppRGB
		      || pixel_format == GUID_WICPixelFormat64bppPRGBAHalf
		      || pixel_format == GUID_WICPixelFormat64bppCMYK
		      || pixel_format == GUID_WICPixelFormat64bpp3ChannelsAlpha
		      || pixel_format == GUID_WICPixelFormat64bpp4Channels
		      || pixel_format == GUID_WICPixelFormat64bpp7ChannelsAlpha
		      || pixel_format == GUID_WICPixelFormat64bpp8Channels )
			return 64u;
		else if( pixel_format == GUID_WICPixelFormat72bpp8ChannelsAlpha )
			return 72u;
		else if( pixel_format == GUID_WICPixelFormat80bppCMYKAlpha
		      || pixel_format == GUID_WICPixelFormat80bpp4ChannelsAlpha
		      || pixel_format == GUID_WICPixelFormat80bpp5Channels )
			return 80u;
		else if( pixel_format == GUID_WICPixelFormat96bppRGBFixedPoint
		      || pixel_format == GUID_WICPixelFormat96bppRGBFloat
		      || pixel_format == GUID_WICPixelFormat96bpp5ChannelsAlpha
		      || pixel_format == GUID_WICPixelFormat96bpp6Channels )
			return 96u;
		else if( pixel_format == GUID_WICPixelFormat112bpp6ChannelsAlpha
		      || pixel_format == GUID_WICPixelFormat112bpp7Channels )
			return 112u;
		else if( pixel_format == GUID_WICPixelFormat128bppRGBFloat
		      || pixel_format == GUID_WICPixelFormat128bppRGBAFloat
		      || pixel_format == GUID_WICPixelFormat128bppPRGBAFloat
		      || pixel_format == GUID_WICPixelFormat128bppRGBFixedPoint
		      || pixel_format == GUID_WICPixelFormat128bppRGBAFixedPoint
		      || pixel_format == GUID_WICPixelFormat128bpp7ChannelsAlpha
		      || pixel_format == GUID_WICPixelFormat128bpp8Channels )
			return 128u;
		else if( pixel_format == GUID_WICPixelFormat144bpp8ChannelsAlpha )
			return 144u;
		else //GUID_WICPixelFormat8bppAlpha : 32bpp in doc.(https://msdn.microsoft.com/ja-jp/library/windows/desktop/ee719797.aspx) TBC
		     //GUID_WICPixelFormatUndefined, GUID_WICPixelFormatDontCare : don't care
			return 0u;
	}
private:
	template<typename I>
	struct bitmap_source : detail::resource<I>{
		using resource::resource;
		expected<WICPixelFormatGUID, hresult_error> get_pixel_format()const{
			WICPixelFormatGUID p;
			const auto hr = (*this)->GetPixelFormat(&p);
			if(SUCCEEDED(hr))
				return p;
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		expected<will::two_dim::wh<UINT>, hresult_error> get_size()const{
			will::two_dim::wh<UINT> s;
			const auto hr = (*this)->GetSize(&s.w, &s.h);
			if(SUCCEEDED(hr))
				return s;
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		expected<will::two_dim::xy<double>, hresult_error> get_dpi()const{
			will::two_dim::xy<double> d;
			const auto hr = (*this)->GetResolution(&d.x, &d.y);
			if(SUCCEEDED(hr))
				return d;
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		expected<void, hresult_error> copy_pixels(const WICRect& rect, UINT buf_size, BYTE* buf)const{
			const auto hr = (*this)->CopyPixels(&rect, rect.Width * bits_per_pixel(get_pixel_format()) / 8, buf_size, buf);
			if(SUCCEEDED(hr))
				return {};
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		expected<void, hresult_error> copy_pixels(UINT buf_size, BYTE* buf)const{
			const auto hr = (*this)->CopyPixels(nullptr, get_size().w * bits_per_pixel(get_pixel_format()) / 8, buf_size, buf);
			if(SUCCEEDED(hr))
				return {};
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		expected<std::vector<BYTE>, hresult_error> copy_pixels(const will::two_dim::xywh<INT>& rect)const{
			std::vector<BYTE> vec(rect.wh.w * rect.wh.h * bits_per_pixel(get_pixel_format()) / 8);
			return copy_pixels(rect.attribute<WICRect>(), static_cast<UINT>(vec.size()), vec.data()).map([&]{return std:move(vec);});
		}
		expected<std::vector<BYTE>, hresult_error> copy_pixels()const{
			return copy_pixels({{0, 0}, get_size().cast<INT>()});
		}
	};
public:
	static expected<wic, hresult_error> create_factory(){
		return detail::convert_to_rich_interface<wic>(com_create_resource<IWICImagingFactory2>([](IWICImagingFactory2** t){return ::CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory2, reinterpret_cast<void**>(t));}), _T(__FUNCTION__));
	}
	explicit wic():wic(+create_factory()){}
	using bitmap = bitmap_source<IWICBitmap>;
	struct converter : bitmap_source<IWICFormatConverter>{
		using bitmap_source::bitmap_source;
		expected<bool, hresult_error> can_convert(const WICPixelFormatGUID& src, const WICPixelFormatGUID& dst){
			BOOL ret;
			const auto hr = (*this)->CanConvert(src, dst, &ret);
			if(SUCCEEDED(hr))
				return ret != FALSE;
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
	};
	struct decoder : detail::resource<IWICBitmapDecoder>{
		using frame = bitmap_source<IWICBitmapFrameDecode>;
		using resource::resource;
		enum class access:DWORD{read = GENERIC_READ, write = GENERIC_WRITE};
		expected<frame, hresult_error> get_frame(UINT f = 0u)const{
			return detail::convert_to_rich_interface<frame>(com_create_resource<IWICBitmapFrameDecode>([&](IWICBitmapFrameDecode** ptr){return (*this)->GetFrame(f, ptr);}), _T(__FUNCTION__));
		}
	};
	expected<decoder, hresult_error> create_decoder(LPCWSTR filename, decoder::access read_or_write = decoder::access::read, WICDecodeOptions decop = WICDecodeMetadataCacheOnLoad){
		return detail::convert_to_rich_interface<decoder>(com_create_resource<IWICBitmapDecoder>([&](IWICBitmapDecoder** ptr){return (*this)->CreateDecoderFromFilename(filename, nullptr, static_cast<DWORD>(read_or_write), decop, ptr);}), _T(__FUNCTION__));
	}
	expected<bitmap, hresult_error> create_bitmap(unsigned int w, unsigned int h, GUID pixel_format = GUID_WICPixelFormat32bppPBGRA, WICBitmapCreateCacheOption decop = WICBitmapCacheOnLoad){
		return detail::convert_to_rich_interface<bitmap>(com_create_resource<IWICBitmap>([&](IWICBitmap** ptr){return (*this)->CreateBitmap(w, h, pixel_format, decop, ptr);}), _T(__FUNCTION__));
	}
	expected<converter, hresult_error> create_converter(const decoder::frame& fram, GUID pixel_format = GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherType dither = WICBitmapDitherTypeNone, IWICPalette* palette = nullptr, double alpha = 0., WICBitmapPaletteType palette_type = WICBitmapPaletteTypeMedianCut){
		return com_create_resource<IWICFormatConverter>([&](IWICFormatConverter** t){return (*this)->CreateFormatConverter(t);})
		         .emap([](HRESULT e){return make_unexpected<hresult_error>(_T("will::wic::create_converter"), e);})
		         .bind([&](IWICFormatConverter* p)->expected<converter, hresult_error>{
		           const auto hr = p->Initialize(fram.get(), pixel_format, dither, palette, alpha, palette_type);
		           if(SUCCEEDED(hr))
		             return converter{std::move(p)};
		           return make_unexpected<hresult_error>(_T("will::wic::create_converter::initialize"), hr);
		         });
	}
	expected<converter, hresult_error> create_converter(LPCWSTR filename, decoder::access access = decoder::access::read, WICDecodeOptions decop = WICDecodeMetadataCacheOnLoad){
		return create_decoder(filename, access, decop)
		         .bind([](decoder&& d){return d.get_frame();})
		         .bind([&](decoder::frame&& f){return create_converter(f);});
	}
	class stream : public detail::resource<IWICStream>{
		using resource::resource;
	public:
		using access = decoder::access;
		stream(stream&&) = default;
		stream& operator=(stream&&) = default;
		expected<void, hresult_error> commit(STGC flag = STGC_DEFAULT)&{
			const auto hr = (*this)->Commit(flag);
			if(SUCCEEDED(hr))
				return {};
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		expected<void, hresult_error> commit(STGC flag = STGC_DEFAULT)&&{
			const auto hr = (*this)->Commit(flag);
			resource<IWICStream>{}.swap(*this);
			if(SUCCEEDED(hr))
				return {};
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		~stream(){if(get())auto _ [[maybe_unused]] = commit();}
		friend wic;
	};
	class image_parameters : public detail::property<WICImageParameters>{
		class dpi_property{
			image_parameters* prop;
		public:
			constexpr dpi_property(image_parameters* p):prop(p){}
			dpi_property(const dpi_property&) = delete;
			dpi_property(dpi_property&&) = delete;
			dpi_property& operator=(const dpi_property&) = delete;
			dpi_property& operator=(dpi_property&&) = delete;
			image_parameters& operator()(FLOAT x, FLOAT y)const{prop->prop.DpiX = x; prop->prop.DpiY = y; return *prop;}
			image_parameters& operator()(const two_dim::xy<FLOAT>& xy)const{return (*this)(xy.x, xy.y);}
			image_parameters& x(FLOAT x)const{prop->prop.DpiX = x; return *prop;}
			image_parameters& y(FLOAT y)const{prop->prop.DpiY = y; return *prop;}
		};
		class pixel_property{
			image_parameters* prop;
		public:
			constexpr pixel_property(image_parameters* p):prop(p){}
			pixel_property(const pixel_property&) = delete;
			pixel_property(pixel_property&&) = delete;
			pixel_property& operator=(const pixel_property&) = delete;
			pixel_property& operator=(pixel_property&&) = delete;
			image_parameters& operator()(UINT32 w, UINT32 h)const{prop->prop.PixelWidth = w; prop->prop.PixelHeight = h; return *prop;}
			image_parameters& operator()(const two_dim::wh<UINT32>& wh)const{return (*this)(wh.w, wh.h);}
			image_parameters& w(UINT32 w)const{prop->prop.PixelWidth = w; return *prop;}
			image_parameters& h(UINT32 h)const{prop->prop.PixelHeight = h; return *prop;}
		};
		friend dpi_property;
		friend pixel_property;
	public:
		using property::property;
#define PROPERTYDECL(name, type, membername) image_parameters& name(type t){prop.membername = t;return *this;}
		PROPERTYDECL(pixel_format, D2D1_PIXEL_FORMAT, PixelFormat)
		const dpi_property dpi = this;
		PROPERTYDECL(top, FLOAT, Top)
		PROPERTYDECL(left, FLOAT, Left)
		const pixel_property pixel = this;
#undef  PROPERTYDECL
	};
	struct encoder : detail::resource<IWICImageEncoder>{
		struct frame : detail::resource<IWICBitmapFrameEncode>{
			using resource::resource;
			frame(frame&&) = default;
			frame& operator=(frame&&) = default;
			expected<void, hresult_error> commit(){
				if(!*this)
					return make_unexpected<hresult_error>(_T(__FUNCTION__), E_POINTER);
				const auto hr = (*this)->Commit();
				if(SUCCEEDED(hr)){
					resource{}.swap(*this);
					return {};
				}
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			}
			~frame(){if(*this)auto _ [[maybe_unused]] = commit();}
		};
		struct bitmap : detail::resource<IWICBitmapEncoder>{
			using resource::resource;
			bitmap(bitmap&&) = default;
			bitmap& operator=(bitmap&&) = default;
			expected<frame, hresult_error> create_frame(){
				return com_create_resource<IWICBitmapFrameEncode>([&](IWICBitmapFrameEncode** ptr){return (*this)->CreateNewFrame(ptr, nullptr);})
				         .emap([](HRESULT e){return make_unexpected<hresult_error>(_T("will::wic::encoder::bitmap::create_frame"), e);})
				         .bind([](IWICBitmapFrameEncode* t)->expected<frame, hresult_error>{
				            const auto hr = t->Initialize(nullptr);
				            if(SUCCEEDED(hr))
				              return frame{std::move(t)};
				            return make_unexpected<hresult_error>(_T("will::wic::encoder::bitmap::create_frame::initialize"), hr);
				         });
			}
			template<typename F>
			expected<frame, hresult_error> create_frame_with_property_bag(F&& f){
				IPropertyBag2* pb;
				return com_create_resource<IWICBitmapFrameEncode>([&](IWICBitmapFrameEncode** ptr){return (*this)->CreateNewFrame(ptr, &pb);})
				         .emap([](HRESULT e){return make_unexpected<hresult_error>(_T("will::wic::encoder::bitmap::create_frame_with_property_bag"), e);})
				         .bind([&](IWICBitmapFrameEncode* t)->expected<frame, hresult_error>{
				            property_bag prob{std::move(pb)};
				            auto ret = f(prob);
				            if(!ret)
				              return ret.get_unexpected();
				            const auto hr = t->Initialize(prob.get());
				            if(SUCCEEDED(hr))
				              return frame{std::move(t)};
				            return make_unexpected<hresult_error>(_T("will::wic::encoder::bitmap::create_frame_with_property_bag"), hr);
				         });
			}
			expected<void, hresult_error> commit(){
				if(!*this)
					return make_unexpected<hresult_error>(_T(__FUNCTION__), E_POINTER);
				const auto hr = (*this)->Commit();
				if(SUCCEEDED(hr)){
					resource{}.swap(*this);
					return {};
				}
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			}
			~bitmap(){if(*this)auto _ [[maybe_unused]] = commit();}
		};
	public:
		using resource::resource;
		expected<void, hresult_error> write(ID2D1Bitmap1* bmp, const frame& frame, const WICImageParameters* params = nullptr){
			const auto hr = (*this)->WriteFrame(bmp, frame.get(), params);
			if(SUCCEEDED(hr))
				return {};
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		expected<void, hresult_error> write(ID2D1Bitmap1* bmp, const frame& frame, const WICImageParameters& params){return write(bmp, frame, &params);}
		template<typename D2DBitmap>
		expected<void, hresult_error> write(D2DBitmap&& bmp, const frame& frame, const WICImageParameters* params = nullptr){return write(std::forward<D2DBitmap>(bmp).get(), frame, params);}
		template<typename D2DBitmap>
		expected<void, hresult_error> write(D2DBitmap&& bmp, const frame& frame, const WICImageParameters& params){return write(std::forward<D2DBitmap>(bmp).get(), frame, params);}
	};
	expected<stream, hresult_error> create_stream(LPCWSTR filename, stream::access access = stream::access::write){
		return com_create_resource<IWICStream>([&](IWICStream** ptr){return (*this)->CreateStream(ptr);})
		         .emap([](HRESULT e){return make_unexpected<hresult_error>(_T("will::wic::create_stream"), e);})
		         .bind([&](IWICStream* s)->expected<stream, hresult_error>{
		            const auto hr = s->InitializeFromFilename(filename, static_cast<DWORD>(access));
		            if(SUCCEEDED(hr))
		              return stream{std::move(s)};
		            return make_unexpected<hresult_error>(_T("will::wic::create_stream::initialize"), hr);
		         });
	}
	expected<encoder::bitmap, hresult_error> create_bitmap_encoder(const stream& stream, const GUID& encode = GUID_ContainerFormatPng, WICBitmapEncoderCacheOption cache_option = WICBitmapEncoderNoCache){
		return com_create_resource<IWICBitmapEncoder>([&](IWICBitmapEncoder** ptr){return (*this)->CreateEncoder(encode, nullptr, ptr);})
		         .bind([&](IWICBitmapEncoder* p)->expected<encoder::bitmap, hresult_error>{
		            const auto hr = p->Initialize(stream.get(), cache_option);
		            if(SUCCEEDED(hr))
		              return encoder::bitmap{std::move(p)};
		            return make_unexpected<hresult_error>(_T("will::wic::create_bitmap_encoder"), hr);
		         });
	}
	expected<encoder, hresult_error> create_encoder(ID2D1Device* device){
		return detail::convert_to_rich_interface<encoder>(com_create_resource<IWICImageEncoder>([&](IWICImageEncoder** ptr){return (*this)->CreateImageEncoder(device, ptr);}), _T(__FUNCTION__));
	}
	template<typename D2DDevice>
	expected<encoder, hresult_error> create_encoder(D2DDevice&& device){return create_encoder(std::forward<D2DDevice>(device).get());}
	template<typename D2DDevice, typename D2DBitmap>
	expected<void, hresult_error> encode_to_file(LPCWSTR filename, D2DDevice&& device, D2DBitmap&& bmp, const GUID& format = GUID_ContainerFormatPng){
		return create_stream(filename)
		         .bind([&, this](stream&& stream){return this->create_bitmap_encoder(stream, format);})
		         .bind([&, this](encoder::bitmap&& bitmap_encoder){return bitmap_encoder.create_frame()
		            .bind([&, this](encoder::frame&& frame_encoder){return this->create_encoder(std::forward<D2DDevice>(device))
		               .bind([&](encoder&& encoder){return encoder.write(std::forward<D2DBitmap>(bmp), frame_encoder);})
		               .bind([&]{return frame_encoder.commit();});
		            })
		            .bind([&]{return bitmap_encoder.commit();});
		         });
	}
};
}