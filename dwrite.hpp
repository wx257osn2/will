//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"com.hpp"
#include"_windows.hpp"
#include<dwrite_1.h>
#pragma comment(lib, "DWrite.lib")
#include<vector>
#include<utility>
namespace will{
class dwrite : public detail::resource<IDWriteFactory1>{
	using resource::resource;
public:
	static expected<dwrite, hresult_error> create_factory(DWRITE_FACTORY_TYPE type = DWRITE_FACTORY_TYPE_SHARED){
		return detail::convert_to_rich_interface<dwrite>(com_create_resource<IDWriteFactory1>([&](IDWriteFactory1** x){return DWriteCreateFactory(type, __uuidof(IDWriteFactory1), reinterpret_cast<IUnknown**>(x));}), _T(__FUNCTION__));
	}
	explicit dwrite(DWRITE_FACTORY_TYPE type = DWRITE_FACTORY_TYPE_SHARED):dwrite{+create_factory()}{}
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
			property(WString&& font_name, FLOAT font_size):name_(std::forward<WString>(font_name)), size_(font_size){}
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
		class alignment_property{
			DWRITE_TEXT_ALIGNMENT text_;
			DWRITE_PARAGRAPH_ALIGNMENT paragraph_;
		public:
			explicit constexpr alignment_property(DWRITE_TEXT_ALIGNMENT t = DWRITE_TEXT_ALIGNMENT_JUSTIFIED, DWRITE_PARAGRAPH_ALIGNMENT p = DWRITE_PARAGRAPH_ALIGNMENT_NEAR) : text_(t), paragraph_(p){}
			alignment_property& text(DWRITE_TEXT_ALIGNMENT t){text_ = t; return *this;}
			alignment_property& paragraph(DWRITE_PARAGRAPH_ALIGNMENT p){paragraph_ = p; return *this;}
			friend format;
		};
		static constexpr alignment_property center_alignment(){return alignment_property{DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER};}
		expected<format, hresult_error> text_alignment(DWRITE_TEXT_ALIGNMENT t){const auto hr = (*this)->SetTextAlignment(t);if(SUCCEEDED(hr))return std::move(*this);return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<format, hresult_error> paragraph_alignment(DWRITE_PARAGRAPH_ALIGNMENT p){const auto hr = (*this)->SetParagraphAlignment(p);if(SUCCEEDED(hr))return std::move(*this);return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<format, hresult_error> alignment(const alignment_property& prop){return text_alignment(prop.text_).bind([&](format&& f){return f.paragraph_alignment(prop.paragraph_);});}
	};
	struct font : detail::resource<IDWriteFont1>{
		using resource::resource;
		struct file : detail::resource<IDWriteFontFile>{
			using resource::resource;
			struct reference_key{
				const void* data;
				UINT32 size;
			};
			struct loader : detail::resource<IDWriteFontFileLoader>{
				using resource::resource;
				class registrar{
					dwrite& factory_;
					com_ptr<IDWriteFontFileLoader> loader_;
					registrar(dwrite& fac, IDWriteFontFileLoader* ldr):factory_{fac}, loader_{std::move(ldr)}{}
					friend dwrite;
				public:
					registrar(const registrar&) = delete;
					registrar(registrar&&) = default;
					expected<void, hresult_error> unregister(){
						const auto hr = factory_->UnregisterFontFileLoader(loader_.get());
						if(SUCCEEDED(hr)){
							loader_.reset();
							return {};
						}
						return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
					}
					~registrar(){if(loader_)unregister();}
				};
			};
			expected<loader, hresult_error> get_loader(){return detail::convert_to_rich_interface<loader>(com_create_resource<IDWriteFontFileLoader>([&](IDWriteFontFileLoader** ptr){return (*this)->GetLoader(ptr);}), _T(__FUNCTION__));}
			expected<reference_key, hresult_error> get_reference_key(){reference_key k;const auto hr = (*this)->GetReferenceKey(&k.data, &k.size);if(SUCCEEDED(hr))return k;return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		};
		struct face : detail::resource<IDWriteFontFace1>{
			using resource::resource;
			expected<std::vector<file>, hresult_error> get_files(){
				UINT32 num;
				auto hr = (*this)->GetFiles(&num, nullptr);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				std::vector<IDWriteFontFile*> ff(num, nullptr);
				hr = (*this)->GetFiles(&num, reinterpret_cast<IDWriteFontFile**>(ff.data()));
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return std::move(*reinterpret_cast<std::vector<file>*>(&ff));
			}
			expected<file, hresult_error> get_file(){
				UINT32 num;
				const auto hr = (*this)->GetFiles(&num, nullptr);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				if(num != 1)
					return make_unexpected<hresult_error>(_T(__FUNCTION__), S_FALSE); //Use get_files() instead of get_file()
				return detail::convert_to_rich_interface<file>(com_create_resource<IDWriteFontFile>([&](IDWriteFontFile** ptr){return (*this)->GetFiles(&num, ptr);}), _T(__FUNCTION__));
			}
		};
		struct collection : detail::resource<IDWriteFontCollection>{
			using resource::resource;
			struct loader : detail::resource<IDWriteFontCollectionLoader>{
				using resource::resource;
				class registrar{
					dwrite& factory_;
					com_ptr<IDWriteFontCollectionLoader> loader_;
					registrar(dwrite& fac, IDWriteFontCollectionLoader* ldr):factory_{fac}, loader_{std::move(ldr)}{}
					friend dwrite;
				public:
					registrar(const registrar&) = delete;
					registrar(registrar&&) = default;
					expected<void, hresult_error> unregister(){
						const auto hr = factory_->UnregisterFontCollectionLoader(loader_.get());
						if(SUCCEEDED(hr)){
							loader_.reset();
							return {};
						}
						return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
					}
					~registrar(){if(loader_)unregister();}
				};
			};
			struct find_result{
				UINT32 index;
				bool exists;
				UINT32 get()const noexcept{return index;}
				explicit operator bool()const noexcept{return exists;}
			};
			expected<find_result, hresult_error> find(const wchar_t* family_name){
				UINT32 i;
				BOOL e;
				const auto hr = (*this)->FindFamilyName(family_name, &i, &e);
				if(FAILED(hr))
					return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
				return find_result{i, e != FALSE};
			}
		};
	};
	struct gdi_interop : detail::resource<IDWriteGdiInterop>{
		using resource::resource;
		expected<LOGFONTW, hresult_error> convert(IDWriteFontFace* fontface){
			LOGFONTW log;
			const auto hr = (*this)->ConvertFontFaceToLOGFONT(fontface, &log);
			if(SUCCEEDED(hr))
				return log;
			return make_unexpected<hresult_error>(_T("will::dwrite::gdi_interop::convert_font_face_to_logfont"), hr);
		}
		struct convert_font_to_logfont_result{
			LOGFONTW logfont;
			bool is_system_font;
		};
		expected<convert_font_to_logfont_result, hresult_error> convert(IDWriteFont1* font){
			LOGFONTW log;
			BOOL flag;
			const auto hr = (*this)->ConvertFontToLOGFONT(font, &log, &flag);
			if(SUCCEEDED(hr))
				return convert_font_to_logfont_result{log, flag != FALSE};
			return make_unexpected<hresult_error>(_T("will::dwrite::gdi_interop::convert_font_to_logfont"), hr);
		}
		template<typename Font>
		auto convert(Font&& font)->decltype(convert(std::forward<Font>(font).get())){return convert(std::forward<Font>(font).get());}
		expected<font::face, hresult_error> create_font_face(HDC hdc){return detail::convert_to_rich_interface<font::face>(com_create_resource<IDWriteFontFace1>([&](IDWriteFontFace1** ptr){return (*this)->CreateFontFaceFromHdc(hdc, reinterpret_cast<IDWriteFontFace**>(ptr));}), _T(__FUNCTION__));}
		expected<font::face, hresult_error> create_font_face(HDC hdc, HFONT font){const auto obj = ::SelectObject(hdc, font);auto ff = create_font_face(hdc);::SelectObject(hdc, obj);return ff;}
		expected<font, hresult_error> create_font(const LOGFONTW& logical_font){return detail::convert_to_rich_interface<font>(com_create_resource<IDWriteFont1>([&](IDWriteFont1** ptr){return (*this)->CreateFontFromLOGFONT(&logical_font, reinterpret_cast<IDWriteFont**>(ptr));}), _T(__FUNCTION__));}
	};
	struct rendering_params : detail::resource<IDWriteRenderingParams1>{
		using resource::resource;
		FLOAT get_clear_type_level()const{return (*this)->GetClearTypeLevel();}
		FLOAT get_enhanced_contrast()const{return (*this)->GetEnhancedContrast();}
		FLOAT get_gamma()const{return (*this)->GetGamma();}
		DWRITE_PIXEL_GEOMETRY get_pixel_geometry()const{return (*this)->GetPixelGeometry();}
		DWRITE_RENDERING_MODE get_rendering_mode()const{return (*this)->GetRenderingMode();}
		FLOAT get_grayscale_enhanced_contrast()const{return (*this)->GetGrayscaleEnhancedContrast();}
	};
	expected<format, hresult_error> create_format(const wchar_t* fontname, FLOAT size, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL, const wchar_t* locale = L"ja-jp"){
		return detail::convert_to_rich_interface<format>(com_create_resource<IDWriteTextFormat>([&](IDWriteTextFormat** ptr){return (*this)->CreateTextFormat(fontname, nullptr, weight, style, stretch, size, locale, ptr);}), _T(__FUNCTION__));
	}
	expected<format, hresult_error> create_format(const wchar_t* fontname, FLOAT size, IDWriteFontCollection* collection, DWRITE_FONT_WEIGHT weight= DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL, const wchar_t* locale = L"ja-jp"){
		return [&]()->expected<font::collection::find_result, hresult_error>{
			UINT32 i;
			BOOL e;
			const auto hr = collection->FindFamilyName(fontname, &i, &e);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T("will::dwrite::font::collection::find"), hr);
			return font::collection::find_result{i, e != FALSE};
		}().bind([&](font::collection::find_result&& result){
			return detail::convert_to_rich_interface<format>(com_create_resource<IDWriteTextFormat>([&](IDWriteTextFormat** ptr){return (*this)->CreateTextFormat(fontname, result?collection:nullptr, weight, style, stretch, size, locale, ptr);}), _T("will::dwrite::create_format"));
		});
	}
	template<typename FontCollection, std::enable_if_t<!std::is_base_of<IDWriteFontCollection, std::remove_pointer_t<std::decay_t<FontCollection>>>::value>* = nullptr>
	expected<format, hresult_error> create_format(const wchar_t* fontname, FLOAT size, FontCollection&& collection, DWRITE_FONT_WEIGHT weight= DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL, const wchar_t* locale = L"ja-jp"){return create_format(fontname, size, std::forward<FontCollection>(collection).get(), weight, style, stretch, locale);}
	expected<format, hresult_error> create_format(const format::property& prop){return create_format(prop.name_.c_str(), prop.size_, prop.weight_, prop.style_, prop.stretch_, prop.locale_.c_str());}
	expected<format, hresult_error> create_format(const format::property& prop, IDWriteFontCollection* collection){return create_format(prop.name_.c_str(), prop.size_, collection, prop.weight_, prop.style_, prop.stretch_, prop.locale_.c_str());}
	template<typename FontCollection, std::enable_if_t<!std::is_base_of<IDWriteFontCollection, std::remove_pointer_t<std::decay_t<FontCollection>>>::value>* = nullptr>
	expected<format, hresult_error> create_format(const format::property& prop, FontCollection&& collection){return create_format(prop.name_.c_str(), prop.size_, std::forward<FontCollection>(collection).get(), prop.weight_, prop.style_, prop.stretch_, prop.locale_.c_str());}
	expected<gdi_interop, hresult_error> get_gdi_interop(){
		return detail::convert_to_rich_interface<gdi_interop>(com_create_resource<IDWriteGdiInterop>([&](IDWriteGdiInterop** ptr){return (*this)->GetGdiInterop(ptr);}), _T(__FUNCTION__));
	}
	expected<font::file, hresult_error> create_custom_font_file_reference(const font::file::reference_key& ref_key, IDWriteFontFileLoader* loader){
		return detail::convert_to_rich_interface<font::file>(com_create_resource<IDWriteFontFile>([&](IDWriteFontFile** ptr){return (*this)->CreateCustomFontFileReference(ref_key.data, ref_key.size, loader, ptr);}), _T(__FUNCTION__));
	}
	template<typename FontFileLoader>
	expected<font::file, hresult_error> create_custom_font_file_reference(const font::file::reference_key& ref_key, FontFileLoader&& loader){return create_custom_font_file_reference(ref_key, std::forward<FontFileLoader>(loader).get());}
	expected<font::file::loader::registrar, hresult_error> register_loader(IDWriteFontFileLoader* loader){
		loader->AddRef();
		const auto hr = (*this)->RegisterFontFileLoader(loader);
		if(SUCCEEDED(hr))
			return font::file::loader::registrar{*this, loader};
		loader->Release();
		return make_unexpected<hresult_error>(_T("will::dwrite::register_font_file_loader"), hr);
	}
	expected<font::collection::loader::registrar, hresult_error> register_loader(IDWriteFontCollectionLoader* loader){
		loader->AddRef();
		const auto hr = (*this)->RegisterFontCollectionLoader(loader);
		if(SUCCEEDED(hr))
			return font::collection::loader::registrar{*this, loader};
		loader->Release();
		return make_unexpected<hresult_error>(_T("will::dwrite::register_font_collection_loader"), hr);
	}
	template<typename T, std::enable_if_t<!std::is_base_of<IDWriteFontFileLoader, std::remove_pointer_t<std::decay_t<T>>>::value && !std::is_base_of<IDWriteFontCollectionLoader, std::remove_pointer_t<std::decay_t<T>>>::value>* = nullptr>
	auto register_loader(T&& t)->decltype(register_loader(std::forward<T>(t).get())){return register_loader(std::forward<T>(t).get());}
	expected<font::collection, hresult_error> create_custom_font_collection(IDWriteFontCollectionLoader* loader, const std::vector<font::file::reference_key>& keys){
		return detail::convert_to_rich_interface<font::collection>(com_create_resource<IDWriteFontCollection>([&](IDWriteFontCollection** ptr){return (*this)->CreateCustomFontCollection(loader, keys.data(), static_cast<UINT32>(keys.size() * sizeof(font::file::reference_key)), ptr);}), _T(__FUNCTION__));
	}
	expected<rendering_params, hresult_error> create_monitor_rendering_params(HMONITOR monitor){
		return detail::convert_to_rich_interface<rendering_params>(com_create_resource<IDWriteRenderingParams1>([&](IDWriteRenderingParams1** ptr){return (*this)->CreateMonitorRenderingParams(monitor, reinterpret_cast<IDWriteRenderingParams**>(ptr));}), _T(__FUNCTION__));
	}
	template<typename Monitor>
	expected<rendering_params, hresult_error> create_monitor_rendering_params(Monitor&& monitor){return create_monitor_rendering_params(std::forward<Monitor>(monitor).get());}
};

struct font_registrar{
	class property{
		LPCTSTR filename;
		bool private_ = true;
		bool enumable = false;
		DESIGNVECTOR* pdv = nullptr;
		DWORD generate_flag()const{return (private_ ? FR_PRIVATE : 0) | (enumable ? 0 : FR_NOT_ENUM);}
	public:
		property(LPCTSTR file_name) : filename(file_name){}
		property(const property&) = default;
		property(property&&) = default;
		property& operator=(const property&) = default;
		property& operator=(property&&) = default;
		~property() = default;
#define PROPERTYDECL(name, type, membername) property& name(type t){membername = t;return *this;}
		PROPERTYDECL(to_private, bool, private_)
		PROPERTYDECL(to_enumable, bool, enumable)
		PROPERTYDECL(design_vector, DESIGNVECTOR*, pdv)
#undef  PROPERTYDECL
		friend font_registrar;
	};
	explicit font_registrar(const property& prop) : prop(prop), fonts(AddFontResourceEx(prop.filename, prop.generate_flag(), prop.pdv)){}
	~font_registrar(){if(fonts)RemoveFontResourceEx(prop.filename, prop.generate_flag(), prop.pdv);}
	explicit operator bool()const noexcept{return fonts != 0;}
private:
	property prop;
	int fonts;
};

class logical_fonta : public detail::property<LOGFONTA>{
public:
	logical_fonta(const char* font_name):property{}{strcpy_s(prop.lfFaceName, font_name);}
	logical_fonta(const std::string& font_name):logical_fonta{font_name.c_str()}{}
#define PROPERTYDECL(name, type, membername) logical_fonta& name(type t){prop.membername = t;return *this;}
		PROPERTYDECL(height, LONG, lfHeight)
		PROPERTYDECL(width, LONG, lfWidth)
		PROPERTYDECL(escapement, LONG, lfEscapement)
		PROPERTYDECL(orientation, LONG, lfOrientation)
		PROPERTYDECL(weight, LONG, lfWeight)
		PROPERTYDECL(italic, bool, lfItalic)
		PROPERTYDECL(underline, bool, lfUnderline)
		PROPERTYDECL(strike, bool, lfStrikeOut)
		PROPERTYDECL(char_set, BYTE, lfCharSet)
		PROPERTYDECL(out_precision, BYTE, lfOutPrecision)
		PROPERTYDECL(clip_precision, BYTE, lfClipPrecision)
		PROPERTYDECL(quality, BYTE, lfQuality)
#undef  PROPERTYDECL
	logical_fonta& pitch(BYTE p){prop.lfPitchAndFamily = (prop.lfPitchAndFamily&0b11110000)|p;}
	logical_fonta& font_family(BYTE ff){prop.lfPitchAndFamily = (prop.lfPitchAndFamily&0b11)|ff;}
	logical_fonta& name(const char* font_name){strcpy_s(prop.lfFaceName, font_name);return *this;}
	logical_fonta& name(const std::string& font_name){return name(font_name.c_str());}
	operator LOGFONTA&(){return prop;}
	operator const LOGFONTA&()const{return prop;}
	LOGFONTA& get(){return prop;}
	const LOGFONTA& get()const{return prop;}
};
class logical_fontw : public detail::property<LOGFONTW>{
public:
	logical_fontw(const wchar_t* font_name):property{}{wcscpy_s(prop.lfFaceName, font_name);}
	logical_fontw(const std::wstring& font_name):logical_fontw{font_name.c_str()}{}
#define PROPERTYDECL(name, type, membername) logical_fontw& name(type t){prop.membername = t;return *this;}
	PROPERTYDECL(height, LONG, lfHeight)
	PROPERTYDECL(width, LONG, lfWidth)
	PROPERTYDECL(escapement, LONG, lfEscapement)
	PROPERTYDECL(orientation, LONG, lfOrientation)
	PROPERTYDECL(weight, LONG, lfWeight)
	PROPERTYDECL(italic, bool, lfItalic)
	PROPERTYDECL(underline, bool, lfUnderline)
	PROPERTYDECL(strike, bool, lfStrikeOut)
	PROPERTYDECL(char_set, BYTE, lfCharSet)
	PROPERTYDECL(out_precision, BYTE, lfOutPrecision)
	PROPERTYDECL(clip_precision, BYTE, lfClipPrecision)
	PROPERTYDECL(quality, BYTE, lfQuality)
#undef  PROPERTYDECL
	logical_fontw& pitch(BYTE p){prop.lfPitchAndFamily = (prop.lfPitchAndFamily&0b11110000)|p;}
	logical_fontw& font_family(BYTE ff){prop.lfPitchAndFamily = (prop.lfPitchAndFamily&0b11)|ff;}
	logical_fontw& name(const wchar_t* font_name){wcscpy_s(prop.lfFaceName, font_name);return *this;}
	logical_fontw& name(const std::wstring& font_name){return name(font_name.c_str());}
	operator LOGFONTW&(){return prop;}
	operator const LOGFONTW&()const{return prop;}
	LOGFONTW& get(){return prop;}
	const LOGFONTW& get()const{return prop;}
};

using logical_font = 
#ifdef UNICODE
	logical_fontw
#else
	logical_fonta
#endif
	;

struct font_handle{
	HFONT h;
public:
	font_handle(const LOGFONTA& log):h{::CreateFontIndirectA(&log)}{}
	font_handle(const LOGFONTW& log):h{::CreateFontIndirectW(&log)}{}
	operator HFONT()const{return h;}
	HFONT get()const{return h;}
	void detach(){h = nullptr;}
	HFONT release(){auto ret = h;detach();return ret;}
	void reset(){if(h)::DeleteObject(h);h = nullptr;}
	void reset(HFONT n){if(h)::DeleteObject(h);h = n;}
	explicit operator bool()const noexcept{return h != nullptr;}
	~font_handle(){reset();}
};

class registered_font_collection_loader : public com_base<IDWriteFontCollectionLoader>{
	class font_file_enumerator : public com_base<IDWriteFontFileEnumerator>{
		com_ptr<IDWriteFactory> factory_;
		com_ptr<IDWriteFontFile> current_file = nullptr;
		std::vector<dwrite::font::file::reference_key> resources;
	public:
		font_file_enumerator(IDWriteFactory* factory) : factory_(std::move(factory)){if(factory_)factory_->AddRef();}
		HRESULT add_resources(const dwrite::font::file::reference_key* ids, UINT32 count)noexcept try{
			resources.resize(count);
			std::copy(ids, ids+count, resources.rbegin());
			return S_OK;
		}catch(std::bad_alloc&){
			return E_OUTOFMEMORY;
		}
		HRESULT STDMETHODCALLTYPE MoveNext(OUT BOOL* hasCurrentFile)override{
			*hasCurrentFile = FALSE;
			current_file.reset();
			if(resources.empty())
				return S_OK;
			auto file = unpack_from<dwrite::font::file, IDWriteFontFile>(resources.back());
			static auto to_hresult = [](hresult_error&& e){return make_unexpected(e.get_error_code());};
			const auto hr =
				file.get_reference_key()
					.emap(to_hresult).bind([&](dwrite::font::file::reference_key&& ref_key){return
				file.get_loader()
					.emap(to_hresult).bind([&](dwrite::font::file::loader&& loader)->expected<void, HRESULT>{return
				com_create_resource<IDWriteFontFile>([&](IDWriteFontFile** ptr){return factory_->CreateCustomFontFileReference(ref_key.data, ref_key.size, loader.get(), ptr);})
					.map([&](IDWriteFontFile* ff){
				*hasCurrentFile = TRUE;
				current_file = std::move(ff);
					});});})
				.error_or(S_OK);
			resources.pop_back();
			return hr;
		}
		HRESULT STDMETHODCALLTYPE GetCurrentFontFile(OUT IDWriteFontFile** fontFile)override{
			*fontFile = current_file.get();
			if(current_file)current_file->AddRef();
			return current_file ? S_OK : E_FAIL;
		}
	};
	static IDWriteFontCollectionLoader*& get_instance_(){
		static IDWriteFontCollectionLoader* instance = nullptr;
		return instance;
	}
	registered_font_collection_loader()noexcept = default;
public:
	template<typename T>
	static dwrite::font::file::reference_key pack_to_reference_key(T&& t){return {std::forward<T>(t).get(), sizeof(std::forward<T>(t).get())};}
	template<typename T, typename I>
	static T unpack_from(const dwrite::font::file::reference_key& ref_key){auto f = const_cast<I*>(static_cast<const I*>(ref_key.data));f->AddRef();return T{std::move(f)};}
	static IDWriteFontCollectionLoader* get_instance()noexcept{
		static auto&& ptr = get_instance_();
		if(ptr == nullptr)
			ptr = new(std::nothrow) registered_font_collection_loader();
		return ptr;
	}
	HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(IDWriteFactory* factory, void const* collectionKey, UINT32 collectionKeySize, OUT IDWriteFontFileEnumerator** fontFileEnumerator)override{
		*fontFileEnumerator = nullptr;
		font_file_enumerator* enumerator = new(std::nothrow) font_file_enumerator(factory);
		if (enumerator == nullptr)
			return E_OUTOFMEMORY;
		const auto hr = enumerator->add_resources(static_cast<const dwrite::font::file::reference_key*>(collectionKey), static_cast<UINT32>(collectionKeySize / sizeof(const dwrite::font::file::reference_key)));
		if(FAILED(hr)){
			delete enumerator;
			return hr;
		}
		*fontFileEnumerator = enumerator;
		enumerator->AddRef();
		return hr;
	}
	ULONG STDMETHODCALLTYPE Release()override{
		const auto new_count = InterlockedDecrement(&ref_count);
		if(new_count == 0){
			get_instance_() = nullptr;
			delete this;
		}
		return new_count;
	}
	class font_files{
		std::vector<dwrite::font::file> data;
	public:
		font_files() = default;
		font_files(font_files&&) = default;
		template<typename T, typename... Args, std::enable_if_t<std::is_constructible<std::vector<dwrite::font::file>, T&&, Args&&...>::value>* = nullptr>
		font_files(T&& t, Args&&... args):data(std::forward<T>(t), std::forward<Args>(args)...){}
		font_files& add(dwrite::font::file&& x)&{data.emplace_back(std::move(x));return *this;}
		font_files&& add(dwrite::font::file&& x)&&{data.emplace_back(std::move(x));return std::move(*this);}
		font_files& add(std::vector<dwrite::font::file>&& xs)&{for(auto&& x : xs)add(std::move(x));return *this;}
		font_files&& add(std::vector<dwrite::font::file>&& xs)&&{for(auto&& x : xs)add(std::move(x));return std::move(*this);}
		std::vector<dwrite::font::file::reference_key> pack_to_reference_keys()const{
			std::vector<dwrite::font::file::reference_key> keys;
			keys.reserve(data.size());
			for(auto&& x : data)
				keys.emplace_back(pack_to_reference_key(x));
			return keys;
		}
		friend registered_font_collection_loader;
	};
	static expected<font_files, hresult_error> enum_additional_fonts(dwrite& dw, HDC hdc, BYTE charset = DEFAULT_CHARSET, LPCWSTR face_name = L""){
		return dw.get_gdi_interop().map([&](dwrite::gdi_interop&& gdii){
		LOGFONTW l = logical_fontw{face_name}.char_set(charset);
		font_files files;
		using lparam = std::tuple<dwrite::gdi_interop*, std::vector<dwrite::font::file>*, HDC>;
		const lparam params = {&gdii, &files.data, hdc};
		const auto old = ::SelectObject(hdc, nullptr);
		::EnumFontFamiliesExW(hdc, &l, [](const LOGFONTW* logfont, const TEXTMETRICW*, DWORD, LPARAM _)->BOOL{
			auto&& params = *reinterpret_cast<const lparam*>(_);
			auto&& lf = *logfont;
			if(lf.lfFaceName[0] == L'@')
				return 1;
			auto&& gdii = *std::get<0>(params);
			if(gdii.create_font(lf))
				return 1;
			auto hfont = font_handle{lf};
			hfont.reset(reinterpret_cast<HFONT>(::SelectObject(std::get<2>(params), hfont.release())));
			if(auto ff = gdii.create_font_face(std::get<2>(params)))
				if(auto files = ff->get_files())
					for(auto&& x : *files)
						std::get<1>(params)->emplace_back(std::move(x));

			return 1;
		}, reinterpret_cast<LPARAM>(&params), 0);
		if(const auto n = ::SelectObject(hdc, old))
			::DeleteObject(n);
		return files;
		});
	}
	template<typename DeviceContextHandle>
	static expected<font_files, hresult_error> enum_additional_fonts(dwrite& dw, DeviceContextHandle&& dc, BYTE charset = DEFAULT_CHARSET, LPCWSTR face_name = L""){return enum_additional_fonts(dw, std::forward<DeviceContextHandle>(dc).get(), charset, face_name);}
	struct font_collection:private dwrite::font::collection::loader::registrar, public dwrite::font::collection{
		font_collection(dwrite::font::collection::loader::registrar&& reg, dwrite::font::collection&& c):registrar{std::move(reg)}, collection{std::move(c)}{}
	};
	static expected<font_collection, hresult_error> create_font_collection(dwrite& dw, const font_files& ff){
		return dw.register_loader(registered_font_collection_loader::get_instance())
		         .bind([&](dwrite::font::collection::loader::registrar&& fclr){return
		       dw.create_custom_font_collection(registered_font_collection_loader::get_instance(), ff.pack_to_reference_keys())
		         .map([&](dwrite::font::collection&& fc){
		return font_collection{std::move(fclr), std::move(fc)};
		         });});
	}
	struct font_collection_with_font_files:private font_files, public font_collection{
		font_collection_with_font_files(font_files&& ff, font_collection&& fc):font_files(std::move(ff)), font_collection(std::move(fc)){}
	};
	static expected<font_collection_with_font_files, hresult_error> create_font_collection(dwrite& dw, HDC hdc){
		return enum_additional_fonts(dw, hdc)
		         .bind([&](font_files&& ff){return 
		       create_font_collection(dw, ff)
		         .map([&](font_collection&& fc){
		return font_collection_with_font_files{std::move(ff), std::move(fc)};
		         });});
	}
	template<typename DeviceContextHandle, std::enable_if_t<!std::is_same<std::decay_t<DeviceContextHandle>, HDC>::value && !std::is_same<std::decay_t<DeviceContextHandle>, font_files>::value>* = nullptr>
	static font_collection_with_font_files create_font_collection(dwrite& dw, DeviceContextHandle&& dc){return create_font_collection(dw, std::forward<DeviceContextHandle>(dc).get());}
};

}