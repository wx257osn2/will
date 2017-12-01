//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include<sdkddkver.h>
#if _WIN32_WINNT >= _WIN32_WINNT_WINBLUE
#include<utility>
#include<string_view>
#include<future>
#include<shcore.h>
#pragma comment(lib, "Shcore.lib")
#include<windows.storage.h>
#include<windows.data.pdf.h>
#include<windows.data.pdf.interop.h>
#pragma comment(lib, "Windows.data.pdf.lib")
#include"winrt.hpp"
#include"windows.foundation.hpp"
#include"winrt.storage.streams.hpp"
#include"dxgi.hpp"

namespace will{	
struct pdf{
	struct renderer_native : detail::resource<::IPdfRendererNative>{
		using resource::resource;
		class parameter{
			PDF_RENDER_PARAMS param;
		public:
			parameter():param{{0.f, 0.f, 0.f, 0.f}, 0u, 0u, {1.f, 1.f, 1.f, 1.f}, TRUE}{}
			parameter& source_rect(const D2D_RECT_F& v){param.SourceRect = v;return *this;}
			parameter& destination_width(UINT32 v){param.DestinationWidth = v;return *this;}
			parameter& destination_height(UINT32 v){param.DestinationHeight = v;return *this;}
			parameter& destination_size(const two_dim::wh<UINT32>& v){return destination_width(v.w).destination_height(v.h);}
			parameter& background_color(const D2D_COLOR_F& v){param.BackgroundColor = v;return *this;}
			parameter& ignore_high_contrast(bool v){param.IgnoreHighContrast = v ? TRUE : FALSE;return *this;}
			const PDF_RENDER_PARAMS& get()const{return param;}
			operator const PDF_RENDER_PARAMS&()const{return param;}
		};
		static expected<renderer_native, hresult_error> create(IDXGIDevice* dev){
			return detail::convert_to_rich_interface<renderer_native>(com_create_resource<::IPdfRendererNative>([&](::IPdfRendererNative** ptr){return ::PdfCreateRenderer(dev, ptr);}), _T(__FUNCTION__));
		}
		template<typename DXGIDevice, std::enable_if_t<!std::is_base_of<IDXGIDevice, std::remove_pointer_t<std::decay_t<DXGIDevice>>>::value, std::nullptr_t> = nullptr>
		static expected<renderer_native, hresult_error> create(DXGIDevice&& dev){return create(std::forward<DXGIDevice>(dev).get());}
		renderer_native(IDXGIDevice* dev):renderer_native{+create(dev)}{}
		template<typename DXGIDevice, std::enable_if_t<!std::is_base_of<IDXGIDevice, std::remove_pointer_t<std::decay_t<DXGIDevice>>>::value, std::nullptr_t> = nullptr>
		renderer_native(DXGIDevice&& dev):renderer_native{+create(std::forward<DXGIDevice>(dev))}{}
		expected<renderer_native&, hresult_error> operator()(ABI::Windows::Data::Pdf::IPdfPage* page, ::ID2D1DeviceContext* devcon){
			const auto hr = (*this)->RenderPageToDeviceContext(page, devcon, nullptr);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return *this;
		}
		expected<renderer_native&, hresult_error> operator()(ABI::Windows::Data::Pdf::IPdfPage* page, ::ID2D1DeviceContext* devcon, const PDF_RENDER_PARAMS& param){
			const auto hr = (*this)->RenderPageToDeviceContext(page, devcon, const_cast<PDF_RENDER_PARAMS*>(&param));
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return *this;
		}
		template<typename PDFPage>
		expected<renderer_native&, hresult_error> operator()(PDFPage&& page, ::ID2D1DeviceContext* devcon){return operator()(std::forward<PDFPage>(page).get(), devcon);}
		template<typename D2DDeviceContext>
		expected<renderer_native&, hresult_error> operator()(ABI::Windows::Data::Pdf::IPdfPage* page, D2DDeviceContext&& devcon){return operator()(page, std::forward<D2DDeviceContext>(devcon).get());}
		template<typename PDFPage, typename D2DDeviceContext>
		expected<renderer_native&, hresult_error> operator()(PDFPage&& page, D2DDeviceContext&& devcon){return operator()(std::forward<PDFPage>(page).get(), std::forward<D2DDeviceContext>(devcon).get());}
		template<typename PDFPage>
		expected<renderer_native&, hresult_error> operator()(PDFPage&& page, ::ID2D1DeviceContext* devcon, const PDF_RENDER_PARAMS& param){return operator()(std::forward<PDFPage>(page).get(), devcon, param);}
		template<typename D2DDeviceContext>
		expected<renderer_native&, hresult_error> operator()(ABI::Windows::Data::Pdf::IPdfPage* page, D2DDeviceContext&& devcon, const PDF_RENDER_PARAMS& param){return operator()(page, std::forward<D2DDeviceContext>(devcon).get(), param);}
		template<typename PDFPage, typename D2DDeviceContext>
		expected<renderer_native&, hresult_error> operator()(PDFPage&& page, D2DDeviceContext&& devcon, const PDF_RENDER_PARAMS& param){return operator()(std::forward<PDFPage>(page).get(), std::forward<D2DDeviceContext>(devcon).get(), param);}
		expected<renderer_native&, hresult_error> operator()(ABI::Windows::Data::Pdf::IPdfPage* page, ::IDXGISurface* surf, const POINT& offset){
			const auto hr = (*this)->RenderPageToSurface(page, surf, offset, nullptr);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return *this;
		}
		expected<renderer_native&, hresult_error> operator()(ABI::Windows::Data::Pdf::IPdfPage* page, ::IDXGISurface* surf, const POINT& offset, const PDF_RENDER_PARAMS& param){
			const auto hr = (*this)->RenderPageToSurface(page, surf, offset, const_cast<PDF_RENDER_PARAMS*>(&param));
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return *this;
		}
		template<typename PDFPage>
		expected<renderer_native&, hresult_error> operator()(PDFPage&& page, ::IDXGISurface* surf, const POINT& offset){return operator()(std::forward<PDFPage>(page).get(), surf, offset);}
		template<typename DXGISurface, std::enable_if_t<!std::is_base_of<IDXGISurface, std::remove_pointer_t<std::decay_t<DXGISurface>>>::value, std::nullptr_t> = nullptr>
		expected<renderer_native&, hresult_error> operator()(ABI::Windows::Data::Pdf::IPdfPage* page, DXGISurface&& surf, const POINT& offset){return operator()(page, std::forward<DXGISurface>(surf).get(), offset);}
		template<typename PDFPage, typename DXGISurface, std::enable_if_t<!std::is_base_of<IDXGISurface, std::remove_pointer_t<std::decay_t<DXGISurface>>>::value, std::nullptr_t> = nullptr>
		expected<renderer_native&, hresult_error> operator()(PDFPage&& page, DXGISurface&& surf, const POINT& offset){return operator()(std::forward<PDFPage>(page).get(), std::forward<DXGISurface>(surf).get(), offset);}
		template<typename PDFPage>
		expected<renderer_native&, hresult_error> operator()(PDFPage&& page, ::IDXGISurface* surf, const POINT& offset, const PDF_RENDER_PARAMS& param){return operator()(std::forward<PDFPage>(page).get(), surf, offset, param);}
		template<typename DXGISurface, std::enable_if_t<!std::is_base_of<IDXGISurface, std::remove_pointer_t<std::decay_t<DXGISurface>>>::value, std::nullptr_t> = nullptr>
		expected<renderer_native&, hresult_error> operator()(ABI::Windows::Data::Pdf::IPdfPage* page, DXGISurface&& surf, const POINT& offset, const PDF_RENDER_PARAMS& param){return operator()(page, std::forward<DXGISurface>(surf).get(), offset, param);}
		template<typename PDFPage, typename DXGISurface, std::enable_if_t<!std::is_base_of<IDXGISurface, std::remove_pointer_t<std::decay_t<DXGISurface>>>::value, std::nullptr_t> = nullptr>
		expected<renderer_native&, hresult_error> operator()(PDFPage&& page, DXGISurface&& surf, const POINT& offset, const PDF_RENDER_PARAMS& param){return operator()(std::forward<PDFPage>(page).get(), std::forward<DXGISurface>(surf).get(), offset, param);}
	};
	struct page : detail::inspectable<ABI::Windows::Data::Pdf::IPdfPage>{
		struct dimensions : detail::inspectable<ABI::Windows::Data::Pdf::IPdfPageDimensions>{
			using inspectable::inspectable;
			expected<ABI::Windows::Foundation::Rect, hresult_error> art_box()const{
				ABI::Windows::Foundation::Rect value;
				const auto hr = (*this)->get_ArtBox(&value);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return value;
			}
			expected<ABI::Windows::Foundation::Rect, hresult_error> bleed_box()const{
				ABI::Windows::Foundation::Rect value;
				const auto hr = (*this)->get_BleedBox(&value);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return value;
			}
			expected<ABI::Windows::Foundation::Rect, hresult_error> crop_box()const{
				ABI::Windows::Foundation::Rect value;
				const auto hr = (*this)->get_CropBox(&value);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return value;
			}
			expected<ABI::Windows::Foundation::Rect, hresult_error> media_box()const{
				ABI::Windows::Foundation::Rect value;
				const auto hr = (*this)->get_MediaBox(&value);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return value;
			}
			expected<ABI::Windows::Foundation::Rect, hresult_error> trim_box()const{
				ABI::Windows::Foundation::Rect value;
				const auto hr = (*this)->get_TrimBox(&value);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return value;
			}
		};
		using inspectable::inspectable;
		expected<UINT32, hresult_error> get_index()const{
			UINT32 value;
			const auto hr = (*this)->get_Index(&value);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return value;
		}
		expected<ABI::Windows::Foundation::Size, hresult_error> get_size()const{
			ABI::Windows::Foundation::Size value;
			const auto hr = (*this)->get_Size(&value);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return value;
		}
		expected<dimensions, hresult_error> get_dimensions()const{
			return detail::convert_to_rich_interface<struct dimensions>(com_create_resource<ABI::Windows::Data::Pdf::IPdfPageDimensions>([&](ABI::Windows::Data::Pdf::IPdfPageDimensions** ptr){return (*this)->get_Dimensions(ptr);}), _T(__FUNCTION__));
		}
		expected<ABI::Windows::Data::Pdf::PdfPageRotation, hresult_error> get_rotation()const{
			ABI::Windows::Data::Pdf::PdfPageRotation value;
			const auto hr = (*this)->get_Rotation(&value);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return value;
		}
		expected<float, hresult_error> get_preferred_zoom()const{
			float value;
			const auto hr = (*this)->get_PreferredZoom(&value);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return value;
		}
		expected<dxgi::surface, hresult_error> render(const will::dxgi::device& dev, DXGI_SURFACE_DESC desc, PDF_RENDER_PARAMS param)const{
			return renderer_native::create(dev).bind([&](renderer_native&& rn){
				return get_size().bind([&](ABI::Windows::Foundation::Size&& size){
					if(param.DestinationWidth == 0u)
						param.DestinationWidth = static_cast<UINT32>(size.Width);
					if(param.DestinationHeight == 0u)
						param.DestinationHeight = static_cast<UINT32>(size.Height);
					if(desc.Width == 0u)
						desc.Width = param.DestinationWidth;
					if(desc.Height == 0u)
						desc.Height = param.DestinationHeight;
					return dev.create_surface(desc, 1, DXGI_USAGE_RENDER_TARGET_OUTPUT).bind([&](dxgi::surface&& surf){
						return rn(*this, surf, {}, param).map([&](renderer_native&){
							return std::move(surf);});});});});
		}
	};
	struct document : detail::inspectable<ABI::Windows::Data::Pdf::IPdfDocument>{
		struct statics : detail::inspectable<ABI::Windows::Data::Pdf::IPdfDocumentStatics>{
			using inspectable::inspectable;
			static expected<statics, hresult_error> create(){
				auto hstr = hstring{::RuntimeClass_Windows_Data_Pdf_PdfDocument};
				auto t = detail::convert_to_rich_interface<statics>(com_create_resource<ABI::Windows::Data::Pdf::IPdfDocumentStatics>([&](ABI::Windows::Data::Pdf::IPdfDocumentStatics** ptr){return ::RoGetActivationFactory(hstr.get(), ABI::Windows::Data::Pdf::IID_IPdfDocumentStatics, reinterpret_cast<void**>(ptr));}), _T(__FUNCTION__));
				return t;
			}
			statics() : statics{+create()}{}
			statics(statics&&) = default;
			expected<std::future<document>, hresult_error> load_async(ABI::Windows::Storage::IStorageFile* file)const{
				return load_impl(file).bind(make_future);
			}
			expected<std::future<document>, hresult_error> load_async(ABI::Windows::Storage::IStorageFile* file, HSTRING hstr)const{
				return load_impl(file, hstr).bind(make_future);
			}
			template<typename HString>
			expected<std::future<document>, hresult_error> load_async(ABI::Windows::Storage::IStorageFile* file, HString&& hstr)const{return load_async(file, std::forward<HString>(hstr).get());}
			expected<std::future<document>, hresult_error> load_async(ABI::Windows::Storage::Streams::IRandomAccessStream* stream)const{
				return load_impl(stream).bind(make_future);
			}
			expected<std::future<document>, hresult_error> load_async(ABI::Windows::Storage::Streams::IRandomAccessStream* stream, HSTRING hstr)const{
				return load_impl(stream, hstr).bind(make_future);
			}
			template<typename HString>
			expected<std::future<document>, hresult_error> load_async(ABI::Windows::Storage::Streams::IRandomAccessStream* stream, HString&& hstr)const{return load_async(stream, std::forward<HString>(hstr).get());}
			template<typename T>
			expected<std::future<document>, hresult_error> load_async(T&& t)const{return load_async(std::forward<T>(t).get());}
			template<typename T>
			expected<std::future<document>, hresult_error> load_async(T&& t, HSTRING hstr)const{return load_async(std::forward<T>(t).get(), hstr);}
			template<typename T, typename HString>
			expected<std::future<document>, hresult_error> load_async(T&& t, HString&& hstr)const{return load_async(std::forward<T>(t).get(), std::forward<HString>(hstr).get());}
			expected<document, hresult_error> load(ABI::Windows::Storage::IStorageFile* file)const{return load_async(file).bind(get_future);}
			expected<document, hresult_error> load(ABI::Windows::Storage::IStorageFile* file, HSTRING hstr)const{return load_async(file, hstr).bind(get_future);}
			template<typename HString>
			expected<document, hresult_error> load(ABI::Windows::Storage::IStorageFile* file, HString&& hstr)const{return load(file, std::forward<HString>(hstr).get());}
			expected<document, hresult_error> load(ABI::Windows::Storage::Streams::IRandomAccessStream* stream)const{return load_async(stream).bind(get_future);}
			expected<document, hresult_error> load(ABI::Windows::Storage::Streams::IRandomAccessStream* stream, HSTRING hstr)const{return load_async(stream, hstr).bind(get_future);}
			template<typename HString>
			expected<document, hresult_error> load(ABI::Windows::Storage::Streams::IRandomAccessStream* stream, HString&& hstr)const{return load(stream, std::forward<HString>(hstr).get());}
			template<typename T>
			expected<document, hresult_error> load(T&& t)const{return load(std::forward<T>(t).get());}
			template<typename T>
			expected<document, hresult_error> load(T&& t, HSTRING hstr)const{return load(std::forward<T>(t).get(), hstr);}
			template<typename T, typename HString>
			expected<document, hresult_error> load(T&& t, HString&& hstr)const{return load(std::forward<T>(t).get(), std::forward<HString>(hstr).get());}
		private:
			static expected<std::future<document>, hresult_error> make_future(com_ptr<ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>>&& async){
				std::promise<document> p;
				auto f = p.get_future();
				struct invoker : ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::Windows::Data::Pdf::PdfDocument*>{
					invoker(const invoker&) = delete;
					invoker& operator=(const invoker&) = delete;
					static com_ptr<ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::Windows::Data::Pdf::PdfDocument*>> create(std::promise<document>&& p){return new invoker(std::move(p));}
					virtual ~invoker()noexcept{refcount_ = -(std::numeric_limits<long>::max()/2);}
					virtual HRESULT __stdcall QueryInterface(const GUID& riid, void **ppvObject){
						if(inline_is_equal_guid(riid, __uuidof(IUnknown))){
							*ppvObject = static_cast<IUnknown*>(static_cast<ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::Windows::Data::Pdf::PdfDocument*>*>(this));
							static_cast<IUnknown*>(*ppvObject)->AddRef();
							return S_OK;
						}
						if(inline_is_equal_guid(riid, __uuidof(ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::Windows::Data::Pdf::PdfDocument*>))){
							*ppvObject = static_cast<ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::Windows::Data::Pdf::PdfDocument*>*>(this);
							static_cast<IUnknown*>(*ppvObject)->AddRef();
							return S_OK;
						}
						return E_NOINTERFACE;
					}
					virtual HRESULT __stdcall Invoke(ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>* async, ABI::Windows::Foundation::AsyncStatus status)noexcept{
						if(status != ABI::Windows::Foundation::AsyncStatus::Completed)
							return S_FALSE;
						ABI::Windows::Data::Pdf::IPdfDocument* doc = nullptr;
						const auto hr = async->GetResults(&doc);
						if(SUCCEEDED(hr))
							p.set_value(document{std::move(doc)});
						else
							p.set_exception(std::make_exception_ptr(hresult_error_exception{_T("__FUNCTION__"), hr}));
						return hr;
					}
					virtual ULONG __stdcall AddRef()noexcept{
						return InterlockedIncrement(&refcount_);
					}
					virtual ULONG __stdcall Release()noexcept{
						ULONG ref = InterlockedDecrement(&refcount_);
						if (ref == 0)
							delete this;
						return ref;
					}
				private:
					volatile long refcount_;
					std::promise<document> p;
					invoker(std::promise<document>&& prom):refcount_(1), p{std::move(prom)}{}
					static int inline_is_equal_guid(const GUID& rguid1, const GUID& rguid2){
						return reinterpret_cast<const unsigned long*>(&rguid1)[0] == reinterpret_cast<const unsigned long*>(&rguid2)[0] && reinterpret_cast<const unsigned long*>(&rguid1)[1] == reinterpret_cast<const unsigned long*>(&rguid2)[1] && reinterpret_cast<const unsigned long*>(&rguid1)[2] == reinterpret_cast<const unsigned long*>(&rguid2)[2] && reinterpret_cast<const unsigned long*>(&rguid1)[3] == reinterpret_cast<const unsigned long*>(&rguid2)[3];
					}
				};
				auto callback = invoker::create(std::move(p));
				const auto hr = async->put_Completed(callback.get());
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return f;
			}
			static expected<document, hresult_error> get_future(std::future<document>&& f)try{
				return f.get();
			}catch(hresult_error_exception& e){
				return make_unexpected(e.value());
			}
			expected<com_ptr<ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>>, hresult_error> load_impl(ABI::Windows::Storage::IStorageFile* file)const{
				ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>* async_;
				const auto hr = (*this)->LoadFromFileAsync(file, &async_);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T("will::pdf::document::statics::load_from_file_async"), hr);
				return com_ptr<ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>>{std::move(async_)};
			}
			expected<com_ptr<ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>>, hresult_error> load_impl(ABI::Windows::Storage::IStorageFile* file, HSTRING hstr)const{
				ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>* async_;
				const auto hr = (*this)->LoadFromFileWithPasswordAsync(file, hstr, &async_);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T("will::pdf::document::statics::load_from_file_with_password_async"), hr);
				return com_ptr<ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>>{std::move(async_)};
			}
			expected<com_ptr<ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>>, hresult_error> load_impl(ABI::Windows::Storage::Streams::IRandomAccessStream* stream)const{
				ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>* async_;
				const auto hr = (*this)->LoadFromStreamAsync(stream, &async_);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T("will::pdf::document::statics::load_from_stream_async"), hr);
				return com_ptr<ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>>{std::move(async_)};
			}
			expected<com_ptr<ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>>, hresult_error> load_impl(ABI::Windows::Storage::Streams::IRandomAccessStream* stream, HSTRING hstr)const{
				ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>* async_;
				const auto hr = (*this)->LoadFromStreamWithPasswordAsync(stream, hstr, &async_);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T("will::pdf::document::statics::load_from_stream_with_password_async"), hr);
				return com_ptr<ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Data::Pdf::PdfDocument*>>{std::move(async_)};
			}
		};
		using inspectable::inspectable;
		static expected<document, hresult_error> open(const std::wstring_view& file_name){
			return random_access_stream::create_on_file(file_name, ABI::Windows::Storage::FileAccessMode_Read).bind([](random_access_stream&& stream){
				return statics::create().bind([&](statics&& s){
					return s.load(stream);});});
		}
		expected<page, hresult_error> get_page(UINT32 index)const{
			return detail::convert_to_rich_interface<page>(com_create_resource<ABI::Windows::Data::Pdf::IPdfPage>([&](ABI::Windows::Data::Pdf::IPdfPage** ptr){return (*this)->GetPage(index, ptr);}), _T(__FUNCTION__));
		}
		expected<UINT32, hresult_error> get_page_count()const{
			UINT32 v;
			const auto hr = (*this)->get_PageCount(&v);
			if(FAILED(hr))
				make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return v;
		}
		expected<bool, hresult_error> is_password_protected()const{
			boolean v;
			const auto hr = (*this)->get_IsPasswordProtected(&v);
			if(FAILED(hr))
				make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return v != 0;
		}
		class const_iterator{
			const document* self;
			UINT32 v;
		public:
			constexpr const_iterator(const document* parent, UINT32 b)noexcept : self{parent}, v{b}{}
			~const_iterator() = default;
			constexpr const_iterator(const const_iterator&) = default;
			constexpr const_iterator(const_iterator&&) = default;
			constexpr const_iterator& operator=(const const_iterator&) = default;
			constexpr const_iterator& operator=(const_iterator&&) = default;
			using value_type = page;
			friend constexpr bool operator==(const const_iterator& lhs, const const_iterator& rhs)noexcept{return lhs.v == rhs.v;}
			friend constexpr bool operator!=(const const_iterator& lhs, const const_iterator& rhs)noexcept{return lhs.v != rhs.v;}
			value_type operator*()const{return self->get_page(v).value();}
		};
		constexpr const_iterator begin()const noexcept{return {this, 0};}
		constexpr const_iterator end()const{return {this, get_page_count().value()};}
	private:
		document():inspectable{nullptr}{}
		friend std::_Associated_state<document>;//Workaround
	};
};
}
#endif
