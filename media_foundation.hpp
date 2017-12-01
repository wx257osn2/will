//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"com.hpp"
#include"_resource_property.hpp"
#include"xaudio2.hpp"
#include<vector>
#include<string>
#include<mfapi.h>
#include<mfidl.h>
#include<mferror.h>
#include<mfreadwrite.h>
#include<wmcodecdsp.h>
#include<evr.h>

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")

namespace will{
class media_foundation{
	bool scope;
	explicit media_foundation(bool t = false):scope(t){}
	template<typename Interface>
	struct basic_attributes : detail::resource<Interface>{
		using resource::resource;
		using resource::get;
		expected<void, hresult_error> copy_to(IMFAttributes* dest)const{const auto hr = (*this)->CopyAllItems(dest);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<bool, hresult_error> compare(IMFAttributes* other, MF_ATTRIBUTES_MATCH_TYPE match_type)const{
			BOOL result;
			const auto hr = (*this)->Compare(other, match_type, &result);
			if(FAILED(hr))
				return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
			return result == TRUE;
		}
		template<typename Attributes, std::enable_if_t<!std::is_base_of<IMFAttributes, std::remove_pointer_t<std::decay_t<Attributes>>>::value>* = nullptr>
		expected<bool, hresult_error> compare(Attributes&& a, MF_ATTRIBUTES_MATCH_TYPE match_type)const{return compare(std::forward<Attributes>(a), match_type);}
		expected<UINT32, hresult_error> get_count()const{UINT32 count;const auto hr = (*this)->GetCount(&count);if(SUCCEEDED(hr))return count;return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		UINT32 size()const{*get_count();}
		expected<void, hresult_error> erase(const GUID& key){const auto hr = (*this)->DeleteItem(key);if(SUCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		template<typename T, std::enable_if_t<std::is_same<T, std::vector<UINT8>>::value>* = nullptr>
		expected<T, hresult_error> get(const GUID& key)const{UINT32 size;auto hr = (*this)->GetBlobSize(key, &size);if(FAILED(hr))return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::get_blob_size"), hr);T t(size);hr = (*this)->GetBlob(key, t.data(), size, nullptr);if(FAILED(hr))return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::get_blob"), hr);return std::move(t);}
		template<typename T, std::enable_if_t<std::is_same<T, std::wstring>::value>* = nullptr>
		expected<T, hresult_error> get(const GUID& key)const{UINT32 size;auto hr = (*this)->GetStringLength(key, &size);if(FAILED(hr))return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::get_string_length"), hr);T t(size+1, L'\0');hr = (*this)->GetString(key, &t[0], size, nullptr);if(FAILED(hr))return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::get_blob"), hr);t.resize(std::wcslen(t.c_str()));t.shrink_to_fit();return std::move(t);}
		template<typename T, std::enable_if_t<std::is_same<T, double>::value>* = nullptr>
		expected<T, hresult_error> get(const GUID& key)const{T t;const auto hr = (*this)->GetDouble(key, &t);if(FAILED(hr))return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::get_double"), hr);return t;}
		template<typename T, std::enable_if_t<std::is_same<T, GUID>::value>* = nullptr>
		expected<T, hresult_error> get(const GUID& key)const{T t;const auto hr = (*this)->GetGUID(key, &t);if(FAILED(hr))return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::get_guid"), hr);return t;}
		template<typename T, std::enable_if_t<std::is_same<T, UINT32>::value>* = nullptr>
		expected<T, hresult_error> get(const GUID& key)const{T t;const auto hr = (*this)->GetUINT32(key, &t);if(FAILED(hr))return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::get_uint32"), hr);return t;}
		template<typename T, std::enable_if_t<std::is_same<T, UINT64>::value>* = nullptr>
		expected<T, hresult_error> get(const GUID& key)const{T t;const auto hr = (*this)->GetUINT64(key, &t);if(FAILED(hr))return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::get_uint64"), hr);return t;}
		template<typename T, std::enable_if_t<std::is_base_of<IUnknown, T>::value>* = nullptr>
		expected<com_ptr<T>, hresult_error> get(const GUID& key)const{T* t;const auto hr = (*this)->GetUnknown(key, __uuidof(T), reinterpret_cast<void**>(&t));if(FAILED(hr))return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::get_unknown"), hr);return com_ptr<T>{std::move(t)};}
		expected<void, hresult_error> set(const GUID& key, const std::vector<UINT8>& data){const auto hr = (*this)->SetBlob(key, data.data(), static_cast<UINT32>(data.size()));if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::set_blob"), hr);}
		expected<void, hresult_error> set(const GUID& key, const wchar_t* data){const auto hr = (*this)->SetString(key, data);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::set_string"), hr);}
		expected<void, hresult_error> set(const GUID& key, const std::wstring& data){const auto hr = (*this)->SetString(key, data.c_str());if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::set_string"), hr);}
		expected<void, hresult_error> set(const GUID& key, double data){const auto hr = (*this)->SetDouble(key, data);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::set_double"), hr);}
		expected<void, hresult_error> set(const GUID& key, const GUID& data){const auto hr = (*this)->SetGUID(key, data);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::set_guid"), hr);}
		expected<void, hresult_error> set(const GUID& key, UINT32 data){const auto hr = (*this)->SetUINT32(key, data);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::set_uint32"), hr);}
		expected<void, hresult_error> set(const GUID& key, UINT64 data){const auto hr = (*this)->SetUINT64(key, data);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::set_uint64"), hr);}
		expected<void, hresult_error> set(const GUID& key, IUnknown* data){const auto hr = (*this)->SetUnknown(key, data);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::set_unknown"), hr);}
		expected<MF_ATTRIBUTE_TYPE, hresult_error> get_type(const GUID& key)const{MF_ATTRIBUTE_TYPE t;const auto hr = (*this)->GetItemType(key, &t);if(FAILED(hr))return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::get_item_type"), hr);return t;}
		class unlocker{
			basic_attributes* p;
			explicit unlocker(basic_attributes& t):p(&t){}
			friend basic_attributes;
			unlocker(const unlocker&) = delete;
		public:
			unlocker(unlocker&& other):p(other.p){other.p = nullptr;}
			unlocker& operator=(unlocker&& rhs){p = rhs.p;rhs.p = nullptr;return *this;}
			expected<void, hresult_error> unlock(){if(p == nullptr)return {};const auto hr = (*p)->UnlockStore();if(FAILED(hr))return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);p = nullptr;return {};}
			~unlocker(){unlock();}
		};
		expected<unlocker, hresult_error> lock(){const auto hr = (*this)->LockStore();if(SUCCEEDED(hr))return unlocker{*this};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
	};
public:
	media_foundation(const media_foundation&):scope(false){}
	media_foundation(media_foundation&& other):scope(other.scope){other.scope = false;}
	media_foundation& operator=(const media_foundation&) = delete;
	media_foundation& operator=(media_foundation&&) = delete;
	using _100nanoseconds = std::chrono::duration<long long, std::ratio<1, 10000000>>;
	enum class flags:DWORD{
		no_socket = MFSTARTUP_NOSOCKET,
		full = MFSTARTUP_FULL
	};
	static expected<media_foundation, hresult_error> startup(flags t = flags::full){
		const auto hr = ::MFStartup(MF_VERSION, static_cast<DWORD>(t));
		if(SUCCEEDED(hr))
			return media_foundation{true};
		return make_unexpected(hresult_error{_T(__FUNCTION__), hr});
	}
	expected<void, hresult_error> shutdown(){
		if(!scope)
			return {};
		const auto hr = ::MFShutdown();
		if(FAILED(hr))
			return make_unexpected(hresult_error{_T(__FUNCTION__), hr});
		scope = false;
		return {};
	}
	~media_foundation(){if(scope)shutdown();}
	class attributes : public basic_attributes<::IMFAttributes>{
		using basic_attributes::basic_attributes;
		friend media_foundation;
	public:
		explicit attributes(UINT32 initial_size):basic_attributes{+media_foundation{}.create_attributes(initial_size)}{}
		attributes(const attributes& other):basic_attributes{+other.get_count().bind([&](UINT32 size){return media_foundation{}.create_attributes(size);}).bind([&](basic_attributes&& attr){return other.copy_to(attr.get()).map([&]{return std::move(attr);});})}{}
		expected<void, hresult_error> set_source_reader_async_callback(IMFSourceReaderCallback* ptr){return this->set(MF_SOURCE_READER_ASYNC_CALLBACK, ptr);}
		template<typename SourceReaderCallback, std::enable_if_t<!std::is_base_of<IMFSourceReaderCallback, std::remove_pointer_t<std::decay_t<SourceReaderCallback>>>::value>* = nullptr>
		expected<void, hresult_error> set_source_reader_async_callback(SourceReaderCallback&& c){return set_source_reader_async_callback(std::forward<SourceReaderCallback>(c).get());}
		expected<com_ptr<IMFSourceReaderCallback>, hresult_error> get_source_reader_async_callback()const{return this->get<IMFSourceReaderCallback>(MF_SOURCE_READER_ASYNC_CALLBACK);}
	};
	expected<attributes, hresult_error> create_attributes(UINT32 initial_size)const{return detail::convert_to_rich_interface<attributes>(com_create_resource<IMFAttributes>([&](IMFAttributes** ptr){return ::MFCreateAttributes(ptr, initial_size);}), _T(__FUNCTION__));}
	class source_reader;
	class sample;
	class media_type : public basic_attributes<::IMFMediaType>{
		using basic_attributes::basic_attributes;
		friend media_foundation;
		friend source_reader;
	public:
		explicit media_type():media_type(+media_foundation{}.create_media_type()){}
		expected<GUID, hresult_error> get_major_type()const{GUID id;const auto hr = (*this)->GetMajorType(&id);if(SUCCEEDED(hr))return id;return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<void, hresult_error> set_major_type(const GUID& id){return this->set(MF_MT_MAJOR_TYPE, id);}
		expected<GUID, hresult_error> get_subtype()const{return this->get<GUID>(MF_MT_SUBTYPE);}
		expected<void, hresult_error> set_subtype(const GUID& id){return this->set(MF_MT_SUBTYPE, id);}
		expected<bool, hresult_error> is_compressed_format()const{BOOL compressed;const auto hr = (*this)->IsCompressedFormat(&compressed);if(SUCCEEDED(hr))return compressed == TRUE;return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<DWORD, hresult_error> is_equal(IMFMediaType* other)const{DWORD flags;const auto hr = (*this)->IsEqual(other, &flags);if(SUCCEEDED(hr))return flags;return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		template<typename MediaType>
		expected<DWORD, hresult_error> is_equal(MediaType&& other)const{return is_equal(std::forward<MediaType>(other).get());}
		expected<UINT32, hresult_error> get_audio_channels()const{return this->get<UINT32>(MF_MT_AUDIO_NUM_CHANNELS);}
		expected<UINT32, hresult_error> get_audio_bits_per_sample()const{return this->get<UINT32>(MF_MT_AUDIO_BITS_PER_SAMPLE);}
		expected<UINT32, hresult_error> get_audio_samples_per_second()const{return this->get<UINT32>(MF_MT_AUDIO_SAMPLES_PER_SECOND);}
		expected<UINT32, hresult_error> get_audio_samples_per_block()const{return this->get<UINT32>(MF_MT_AUDIO_SAMPLES_PER_BLOCK);}
		expected<UINT32, hresult_error> get_audio_bytes_per_second()const{return this->get<UINT32>(MF_MT_AUDIO_AVG_BYTES_PER_SECOND);}
	};
	expected<media_type, hresult_error> create_media_type()const{IMFMediaType* ptr;const auto hr = ::MFCreateMediaType(&ptr);if(SUCCEEDED(hr))return media_type{std::move(ptr)};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
	class media_buffer : public detail::resource<::IMFMediaBuffer>{
		using resource::resource;
		friend sample;
	public:
		explicit media_buffer(DWORD max_length):media_buffer(+media_foundation{}.create_media_buffer(max_length)){}
		media_buffer(DWORD max_length, DWORD alignment_power_of_2_under_512):media_buffer(+media_foundation{}.create_media_buffer(max_length, alignment_power_of_2_under_512)){}
		expected<DWORD, hresult_error> get_max_length()const{DWORD l;const auto hr = (*this)->GetMaxLength(&l);if(SUCCEEDED(hr))return l;return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<DWORD, hresult_error> get_current_length()const{DWORD l;const auto hr = (*this)->GetCurrentLength(&l);if(SUCCEEDED(hr))return l;return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<void, hresult_error> set_current_length(DWORD length)const{const auto hr = (*this)->SetCurrentLength(length);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		struct buffer_head{
			BYTE* p;
			media_buffer* mb;
			buffer_head(media_buffer& m, BYTE* ptr):p{ptr}, mb{&m}{}
			buffer_head(const buffer_head&) = delete;
			friend media_buffer;
		public:
			buffer_head(buffer_head&& other):p(other.p), mb(other.mb){other.p = nullptr;other.mb = nullptr;}
			buffer_head& operator=(buffer_head&& rhs){p = rhs.p;mb = rhs.mb;rhs.p = nullptr;rhs.mb = nullptr;return *this;}
			BYTE* get()const{return p;}
			operator BYTE*()const{return p;}
			BYTE& operator[](std::ptrdiff_t i){return *(p+i);}
			const BYTE& operator[](std::ptrdiff_t i)const{return *(p+i);}
			BYTE& operator*(){return *p;}
			const BYTE& operator*()const{return *p;}
			expected<DWORD, hresult_error> get_max_length()const{if(mb != nullptr)return mb->get_max_length();return make_unexpected<hresult_error>(_T(__FUNCTION__), E_POINTER);}
			expected<DWORD, hresult_error> get_current_length()const{if(mb != nullptr)return mb->get_current_length();return make_unexpected<hresult_error>(_T(__FUNCTION__), E_POINTER);}
			expected<void, hresult_error> set_current_length(DWORD length){if(mb != nullptr)return mb->set_current_length(length);return make_unexpected<hresult_error>(_T(__FUNCTION__), E_POINTER);}
			expected<std::vector<BYTE>, hresult_error> copy()const{return get_current_length().map([&](DWORD length){return std::vector<BYTE>(get(), get()+length);});}
			expected<void, hresult_error> unlock(){if(mb == nullptr)return {};const auto hr = (*mb)->Unlock();if(FAILED(hr))return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);p = nullptr;mb = nullptr;return {};}
			~buffer_head(){unlock();}
		};
		expected<buffer_head, hresult_error> lock(){BYTE* ptr;const auto hr = (*this)->Lock(&ptr, nullptr, nullptr);if(SUCCEEDED(hr))return buffer_head{*this, ptr};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<media_buffer, hresult_error> create_wrapper(DWORD offset = 0)const{return get_current_length().bind([&](DWORD length)->expected<media_buffer, hresult_error>{IMFMediaBuffer* ptr;const auto hr = ::MFCreateMediaBufferWrapper(get(), offset, length - offset, &ptr);if(SUCCEEDED(hr))return media_buffer{std::move(ptr)};return make_unexpected<hresult_error>(_T("will::media_foundation::media_buffer::create_wrapper"), hr);});}
	};
	expected<media_buffer, hresult_error> create_media_buffer(DWORD max_length)const{IMFMediaBuffer* ptr;const auto hr = ::MFCreateMemoryBuffer(max_length, &ptr);if(SUCCEEDED(hr))return media_buffer{std::move(ptr)};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
	expected<media_buffer, hresult_error> create_media_buffer(DWORD max_length, DWORD alignment_power_of_2_under_512)const{
		alignment_power_of_2_under_512 = [&]{
			switch(alignment_power_of_2_under_512){
			case 1:return MF_1_BYTE_ALIGNMENT;
			case 2:return MF_2_BYTE_ALIGNMENT;
			case 4:return MF_4_BYTE_ALIGNMENT;
			case 8:return MF_8_BYTE_ALIGNMENT;
			case 16:return MF_16_BYTE_ALIGNMENT;
			case 32:return MF_32_BYTE_ALIGNMENT;
			case 64:return MF_64_BYTE_ALIGNMENT;
			case 128:return MF_128_BYTE_ALIGNMENT;
			case 256:return MF_256_BYTE_ALIGNMENT;
			case 512:return MF_512_BYTE_ALIGNMENT;
			}
			return -1;
		}();
		if(alignment_power_of_2_under_512 == -1)
			return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);
		IMFMediaBuffer* ptr;
		const auto hr = ::MFCreateAlignedMemoryBuffer(max_length, alignment_power_of_2_under_512, &ptr);
		if(SUCCEEDED(hr))
			return media_buffer{std::move(ptr)};
		return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
	}
	class sample : public basic_attributes<::IMFSample>{
		using basic_attributes::basic_attributes;
		friend source_reader;
	public:
		expected<void, hresult_error> add(IMFMediaBuffer* ptr){const auto hr = (*this)->AddBuffer(ptr);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		template<typename MediaBuffer>
		expected<void, hresult_error> add(MediaBuffer&& m){return add(std::forward<MediaBuffer>(m).get());}
		expected<void, hresult_error> clear(){const auto hr = (*this)->RemoveAllBuffers();if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<DWORD, hresult_error> get_count()const{DWORD count;const auto hr = (*this)->GetBufferCount(&count);if(SUCCEEDED(hr))return count;return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		DWORD size()const{*get_count();}
		expected<DWORD, hresult_error> get_total_length()const{DWORD length;const auto hr = (*this)->GetTotalLength(&length);if(SUCCEEDED(hr))return length;return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<media_buffer, hresult_error> get(DWORD index)const{IMFMediaBuffer* ptr;const auto hr = (*this)->GetBufferByIndex(index, &ptr);if(SUCCEEDED(hr))return media_buffer{std::move(ptr)};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<void, hresult_error> remove(DWORD index){const auto hr = (*this)->RemoveBufferByIndex(index);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<_100nanoseconds, hresult_error> get_sample_duration()const{long long duration;const auto hr = (*this)->GetSampleDuration(&duration);if(SUCCEEDED(hr))return _100nanoseconds{duration};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<void, hresult_error> set_sample_duration(const _100nanoseconds& duration){const auto hr = (*this)->SetSampleDuration(duration.count());if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<_100nanoseconds, hresult_error> get_sample_time()const{long long time;const auto hr = (*this)->GetSampleTime(&time);if(SUCCEEDED(hr))return _100nanoseconds{time};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<void, hresult_error> set_sample_time(const _100nanoseconds& time){const auto hr = (*this)->SetSampleTime(time.count());if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<media_buffer, hresult_error> convert_to_contiguous_buffer()const{IMFMediaBuffer* ptr;const auto hr = (*this)->ConvertToContiguousBuffer(&ptr);if(SUCCEEDED(hr))return media_buffer{std::move(ptr)};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
	};
	class source_reader : public detail::resource<::IMFSourceReader>{
		using resource::resource;
		friend media_foundation;
	public:
		explicit source_reader(const wchar_t* url, IMFAttributes* attributes = nullptr):source_reader{+media_foundation{}.create_source_reader(url, attributes)}{}
		explicit source_reader(const std::wstring& url, IMFAttributes* attributes = nullptr):source_reader{+media_foundation{}.create_source_reader(url, attributes)}{}
		template<typename Attributes>
		explicit source_reader(const wchar_t* url, Attributes&& a):source_reader{+media_foundation{}.create_source_reader(url, std::forward<Attributes>(a))}{}
		template<typename Attributes>
		explicit source_reader(const std::wstring& url, Attributes&& a):source_reader{+media_foundation{}.create_source_reader(url, std::forward<Attributes>(a))}{}
		expected<void, hresult_error> flush(DWORD stream_index){const auto hr = (*this)->Flush(stream_index);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<media_type, hresult_error> get_current_media_type(DWORD stream_index)const{return detail::convert_to_rich_interface<media_type>(com_create_resource<IMFMediaType>([&](IMFMediaType** ptr){return (*this)->GetCurrentMediaType(stream_index, ptr);}), _T(__FUNCTION__));}
		expected<void, hresult_error> set_current_media_type(DWORD stream_index, IMFMediaType* ptr){const auto hr = (*this)->SetCurrentMediaType(stream_index, nullptr, ptr);if(SUCCEEDED(hr))return {}; return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		template<typename MediaType>
		expected<void, hresult_error> set_current_media_type(DWORD stream_index, MediaType&& m){return set_current_media_type(stream_index, std::forward<MediaType>(m).get());}
		expected<media_type, hresult_error> get_native_media_type(DWORD stream_index, DWORD media_type_index)const{return detail::convert_to_rich_interface<media_type>(com_create_resource<IMFMediaType>([&](IMFMediaType** ptr){return (*this)->GetNativeMediaType(stream_index, media_type_index, ptr);}), _T(__FUNCTION__));}
		expected<bool, hresult_error> get_stream_selection(DWORD stream_index)const{BOOL selected;const auto hr = (*this)->GetStreamSelection(stream_index, &selected);if(SUCCEEDED(hr))return selected == TRUE;return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<void, hresult_error> set_stream_selection(DWORD stream_index, bool selected){const auto hr = (*this)->SetStreamSelection(stream_index, selected?TRUE:FALSE);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<void, hresult_error> set_current_position(const _100nanoseconds& position){PROPVARIANT p;p.vt = VT_I8;p.hVal.QuadPart = position.count();auto hr = (*this)->SetCurrentPosition(GUID_NULL, p);if(FAILED(hr))return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);hr = PropVariantClear(&p);if(FAILED(hr))return make_unexpected<hresult_error>(_T(__FUNCTION__ "::propvariant_clear"), hr);return {};}
		expected<void, hresult_error> read_sample_async(DWORD stream_index, DWORD control_flags = 0)const{const auto hr = (*this)->ReadSample(stream_index, control_flags, nullptr, nullptr, nullptr, nullptr);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		struct read_sample_result{
			DWORD stream_index;
			DWORD stream_flags;
			_100nanoseconds timestamp;
			sample sample;
			bool is_eos()const{return stream_flags & MF_SOURCE_READERF_ENDOFSTREAM;}
		};
		expected<read_sample_result, hresult_error> read_sample(DWORD stream_index, DWORD control_flags = 0)const{DWORD actual_stream_index;DWORD stream_flags;long long timestamp;IMFSample* ptr;const auto hr = (*this)->ReadSample(stream_index, control_flags, &actual_stream_index, &stream_flags, &timestamp, &ptr);if(SUCCEEDED(hr) && (stream_flags & MF_SOURCE_READERF_ERROR) == 0)return read_sample_result{actual_stream_index, stream_flags, _100nanoseconds{timestamp}, sample{std::move(ptr)}};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
	};
	expected<source_reader, hresult_error> create_source_reader(const wchar_t* url, IMFAttributes* attributes = nullptr)const{return detail::convert_to_rich_interface<source_reader>(com_create_resource<IMFSourceReader>([&](IMFSourceReader** ptr){return ::MFCreateSourceReaderFromURL(url, attributes, ptr);}), _T(__FUNCTION__));}
	expected<source_reader, hresult_error> create_source_reader(const std::wstring& url, IMFAttributes* attributes = nullptr)const{return create_source_reader(url.c_str(), attributes);}
	template<typename Attributes>
	expected<source_reader, hresult_error> create_source_reader(const wchar_t* url, Attributes&& a)const{return create_source_reader(url, std::forward<Attributes>(a).get());}
	template<typename Attributes>
	expected<source_reader, hresult_error> create_source_reader(const std::wstring& url, Attributes&& a)const{return create_source_reader(url, std::forward<Attributes>(a).get());}
};
}
