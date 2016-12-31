#pragma once
#include"com.hpp"
#include"_resource_property.hpp"
#include<vector>
#include<xaudio2.h>
#include<mfidl.h>
#include<mfapi.h>
#ifndef XAUDIO2_DLL
#pragma comment(lib, "xmcore.lib")
#endif
#pragma comment(lib, "xaudio2.lib")

namespace will{
class xaudio2 : public detail::resource<IXAudio2>{
	using resource::resource;
	template<typename T>
	class voice_resource{
		T* ptr;
	public:
		explicit voice_resource(T*&& resource):ptr(std::move(resource)){}
		voice_resource(const voice_resource&) = delete;
		voice_resource(voice_resource&& other) : ptr(other.ptr){other.ptr = nullptr;}
		voice_resource& operator=(const voice_resource&) = delete;
		voice_resource& operator=(voice_resource&& other){reset(); std::swap(ptr, other.ptr); return *this;}
		~voice_resource(){reset();}
		void swap(voice_resource& other){
			std::swap(ptr, other.ptr);
		}
		T* get()const{return ptr;}
		T* operator->()const{return ptr;}
		explicit operator bool()const{return ptr != nullptr;}
		void reset(T*&& other = nullptr){
			if(ptr)
				ptr->DestroyVoice();
			ptr = std::move(other);
		}
		expected<void, hresult_error> enable_effect(UINT32 index, UINT32 opration_set = XAUDIO2_COMMIT_NOW){const auto hr = (*this)->EnableEffect(index, operation_set);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<void, hresult_error> disable_effect(UINT32 index, UINT32 opration_set = XAUDIO2_COMMIT_NOW){const auto hr = (*this)->DisableEffect(index, operation_set);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		bool is_enabled_effect(UINT32 index)const{BOOL enabled;(*this)->GetEffectState(index, &enabled);return enabled == TRUE;}
		XAUDIO2_VOICE_DETAILS get_voice_details(){XAUDIO2_VOICE_DETAILS det;(*this)->GetVoiceDetails(&det);return det;}
		float get_volume()const{float vol;(*this)->GetVolume(&vol);return vol;}
		expected<void, hresult_error> set_volume(float vol, UINT32 operation_set = XAUDIO2_COMMIT_NOW){const auto hr = (*this)->SetVolume(vol, operation_set);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<void, hresult_error> set_effect_chain(const XAUDIO2_EFFECT_CHAIN& ec){const auto hr = (*this)->SetEffectChain(&ec);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<void, hresult_error> erase_effect_chain(){const auto hr = (*this)->SetEffectChain(nullptr);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
	};
	template<typename I>
	struct slave_voice : voice_resource<I>{
		using voice_resource::voice_resource;
		std::vector<float> get_channel_volumes(UINT32 channels)const{std::vector<float> data(channels);(*this)->GetChannelVolumes(channels, data.data());return std::move(data);}
		expected<void, hresult_error> set_channel_volumes(const std::vector<float>& volumes, UINT32 operation_set = XAUDIO2_COMMIT_NOW){const auto hr = (*this)->SetChannelVolumes(static_cast<UINT32>(volumes.size()), volumes.data(), operation_set);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		XAUDIO2_FILTER_PARAMETERS get_filter_parameters()const{XAUDIO2_FILTER_PARAMETERS param;(*this)->GetFilterParameters(&param);return param;}
		XAUDIO2_FILTER_PARAMETERS get_filter_parameters(IXAudio2Voice* ptr)const{XAUDIO2_FILTER_PARAMETERS param;(ptr, *this)->GetFilterParameters(&param);return param;}
		template<typename Voice, std::enable_if_t<!std::is_base_of<IXAudio2Voice, std::remove_pointer_t<std::decay_t<Voice>>>::value>* = nullptr>
		XAUDIO2_FILTER_PARAMETERS get_filter_parameters(Voice&& t)const{return get_filter_parameters(std::forward<Voice>(t).get());}
		expected<void, hresult_error> set_filter_parameters(const XAUDIO2_FILTER_PARAMETERS& param, UINT32 operation_set = XAUDIO2_COMMIT_NOW){const auto hr = (*this)->SetFilterParameters(&param, operation_set);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		std::vector<float> get_output_matrix(IXAudio2Voice* destination_voice, UINT32 source_channels, UINT32 destination_channels)const{std::vector<float> data(source_channels*destination_channels);(*this)->GetOutputMatrix(destination_voice, source_channels, destination_channels, data.data());return std::move(data);}
		std::vector<float> get_output_matrix(UINT32 source_channels, UINT32 destination_channels)const{return get_output_matrix(nullptr, source_channels, destination_channels);}
		template<typename Voice, std::enable_if_t<!std::is_base_of<IXAudio2Voice, std::remove_pointer_t<std::decay_t<Voice>>>::value>* = nullptr>
		std::vector<float> get_output_matrix(Voice&& v, UINT32 source_channels, UINT32 destination_channels)const{return get_output_matrix(std::forward<Voice>(v).get(), source_channels, destination_channels);}
		expected<void, hresult_error> set_output_matrix(IXAudio2Voice* destination_voice, UINT32 source_channels, UINT32 destination_channels, const std::vector<float>& matrix, UINT32 operation_set = XAUDIO2_COMMIT_NOW){const auto hr = (*this)->SetOutputMatrix(destination_voice, source_channels, destination_channels, matrix.data(), operation_set);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<void, hresult_error> set_output_matrix(UINT32 source_channels, UINT32 destination_channels, const std::vector<float>& matrix, UINT32 operation_set = XAUDIO2_COMMIT_NOW){return set_output_matrix(nullptr, source_channels, destination_channels, matrix, operation_set);}
		template<typename Voice, std::enable_if_t<!std::is_base_of<IXAudio2Voice, std::remove_pointer_t<std::decay_t<Voice>>>::value>* = nullptr>
		expected<void, hresult_error> set_output_matrix(Voice&& v, UINT32 source_channels, UINT32 destination_channels, const std::vector<float>& matrix, UINT32 operation_set = XAUDIO2_COMMIT_NOW){return set_output_matrix(std::forward<Voice>(v), source_channels, destination_channels, matrix, operation_set);}
		expected<void, hresult_error> set_output_voices(const XAUDIO2_VOICE_SENDS& send_list){const auto hr = (*this)->SetOutputVoices(&send_list);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
		expected<void, hresult_error> set_output_voice_to_current_master(){const auto hr = (*this)->SetOutputVoices(nullptr);if(SUCCEEDED(hr))return {};return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);}
	};
public:
	static expected<xaudio2, hresult_error> create(UINT32 flags = 0U, XAUDIO2_PROCESSOR processor = 1U){
		return detail::convert_to_rich_interface<xaudio2>(com_create_resource<IXAudio2>([&](IXAudio2** ptr){return ::XAudio2Create(ptr, flags, processor);}), _T(__FUNCTION__));
	}
	explicit xaudio2(UINT32 flags = 0U, XAUDIO2_PROCESSOR processor = 1U):xaudio2(+create(flags, processor)){}
	struct mastering_voice :  voice_resource<IXAudio2MasteringVoice>{
		class property{
			UINT32 inchanls = XAUDIO2_DEFAULT_CHANNELS;
			UINT32 insamprate = XAUDIO2_DEFAULT_SAMPLERATE;
			UINT32 flag_ = 0;
			LPCWSTR dev_id = 0;
			const XAUDIO2_EFFECT_CHAIN* effectchain = nullptr;
			AUDIO_STREAM_CATEGORY category = AudioCategory_GameEffects;
		public:
	#define PROPERTYDECL(name, type, membername) property& name(type t){membername = t;return *this;}
			PROPERTYDECL(channels, UINT32, inchanls)
			PROPERTYDECL(sampling_rate, UINT32, insamprate)
			PROPERTYDECL(flag, UINT32, flag_)
			PROPERTYDECL(device_identifier, LPCWSTR, dev_id)
			PROPERTYDECL(stream_category, AUDIO_STREAM_CATEGORY, category)
	#undef  PROPERTYDECL
			property& effect_chain(const XAUDIO2_EFFECT_CHAIN& t){effectchain = &t;return *this;}
			friend xaudio2;
		};
		using voice_resource::voice_resource;
	};
	class buffer_info : public detail::property<XAUDIO2_BUFFER>{
		class play_{
			buffer_info& buf;
		public:
			explicit play_(buffer_info& t):buf(t){}
			buffer_info& begin(UINT32 offset){buf.prop.PlayBegin = offset; return buf;}
			buffer_info& length(UINT32 length){buf.prop.PlayLength = length; return buf;}
			buffer_info& operator()(UINT32 offset, UINT32 length){begin(offset); return this->length(length);}
		};
		class loop_{
			buffer_info& buf;
		public:
			explicit loop_(buffer_info& t):buf(t){}
			buffer_info& begin(UINT32 offset){buf.prop.LoopBegin = offset; return buf;}
			buffer_info& length(UINT32 length){buf.prop.LoopLength = length; return buf;}
			buffer_info& count(UINT32 count){buf.prop.LoopCount = count; return buf;}
			buffer_info& operator()(UINT32 offset, UINT32 length, UINT32 count){begin(offset); this->length(length); return this->count(count);}
		};
		friend play_;
		friend loop_;
	public:
		explicit buffer_info() = default;
		template<typename T>
		explicit buffer_info(const T& t){data(t.data(), t.size());}
		buffer_info(const buffer_info&) = default;
		buffer_info(buffer_info&&) = default;
		buffer_info& operator=(const buffer_info&) = default;
		buffer_info& operator=(buffer_info&&) = default;
		~buffer_info() = default;
		buffer_info& flags(UINT32 t){prop.Flags = t; return *this;}
		template<typename T>
		buffer_info& data(const T& t){return data(t.data(), t.size());}
		buffer_info& data(const BYTE* buffer_head, std::size_t length){prop.pAudioData = buffer_head; prop.AudioBytes = static_cast<UINT32>(length); return *this;}
		play_ play{*this};
		loop_ loop{*this};
		template<typename T>
		buffer_info& context(T&& context_value){prop.pContext = static_cast<void*>(std::addressof(std::forward<T>(t))); return *this;}
		operator const XAUDIO2_BUFFER&()const{return prop;}
		const XAUDIO2_BUFFER& get()const{return *this;}
	};
	struct source_voice : slave_voice<IXAudio2SourceVoice>{
		class property : public detail::property<WAVEFORMATEX>{
			UINT32 flag_ = 0;
			float max_freq_ratio = XAUDIO2_DEFAULT_FREQ_RATIO;
			IXAudio2VoiceCallback* callback_ = nullptr;
			const XAUDIO2_VOICE_SENDS* sendlist = nullptr;
			const XAUDIO2_EFFECT_CHAIN* effectchain = nullptr;
			static expected<property, hresult_error> convert_from_mf_media_type(IMFMediaType* m){
				static const auto get = [](IMFMediaType* mt, const GUID& key)->expected<UINT32, hresult_error>{UINT32 t;const auto hr = mt->GetUINT32(key, &t);if(FAILED(hr))return make_unexpected<hresult_error>(_T("will::media_foundation::attributes::get_uint32"), hr);return t;};
				return get(m, MF_MT_AUDIO_NUM_CHANNELS).bind([&](UINT32 channels){
				return get(m, MF_MT_AUDIO_BITS_PER_SAMPLE).bind([&](UINT32 bps){
				return get(m, MF_MT_AUDIO_SAMPLES_PER_SECOND).map([&](UINT32 sps){
				return property().format(WAVE_FORMAT_PCM).channels(channels).bits_per_sample(bps).samples_per_sec(sps);
				});});});
			}
		public:
			using detail::property<WAVEFORMATEX>::property;
	#define PROPERTYDECL(name, type, membername) property& name(type t){prop.membername = t;return *this;}
			PROPERTYDECL(format, WORD, wFormatTag)
			PROPERTYDECL(channels, WORD, nChannels)
			PROPERTYDECL(bits_per_sample, WORD, wBitsPerSample)
			PROPERTYDECL(samples_per_sec, WORD, nSamplesPerSec)
	#undef  PROPERTYDECL
	#define PROPERTYDECL(name, type, membername) property& name(type t){membername = t;return *this;}
			PROPERTYDECL(flag, UINT32, flag_)
			PROPERTYDECL(max_frequency_ratio, float, max_freq_ratio)
	#undef  PROPERTYDECL
	#define PROPERTYDECL(name, type, membername) property& name(type t){membername = &t;return *this;}
			PROPERTYDECL(callback, IXAudio2VoiceCallback&, callback_)
			PROPERTYDECL(send_list, const XAUDIO2_VOICE_SENDS&, sendlist)
			PROPERTYDECL(effect_chain, const XAUDIO2_EFFECT_CHAIN&, effectchain)
	#undef  PROPERTYDECL
			operator WAVEFORMATEX&(){
				prop.nBlockAlign = prop.wBitsPerSample / 8 * prop.nChannels;
				prop.nAvgBytesPerSec = prop.nSamplesPerSec * prop.nBlockAlign;
				return prop;
			}
			WAVEFORMATEX& get(){return *this;}
			friend xaudio2;
		};
		using slave_voice::slave_voice;
		expected<void, hresult_error> submit_buffer(const XAUDIO2_BUFFER& buf){const auto hr = (*this)->SubmitSourceBuffer(&buf);if(FAILED(hr))return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);return {};}
		expected<void, hresult_error> start(UINT32 operation_set = XAUDIO2_COMMIT_NOW){const auto hr = (*this)->Start(0U, operation_set);if(FAILED(hr))return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);return {};}
		expected<void, hresult_error> stop (UINT32 flags = 0U, UINT32 operation_set = XAUDIO2_COMMIT_NOW){const auto hr = (*this)->Stop(flags, operation_set);if(FAILED(hr))return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);return {};}
		XAUDIO2_VOICE_STATE get_state(UINT32 flags = 0U)const{XAUDIO2_VOICE_STATE state; (*this)->GetState(&state, flags); return state;}
	};
	expected<mastering_voice, hresult_error> create_mastering_voice()const{return com_create_resource<IXAudio2MasteringVoice>([&](IXAudio2MasteringVoice** ptr){return (*this)->CreateMasteringVoice(ptr);}).emap([](HRESULT e){return make_unexpected<hresult_error>(_T(__FUNCTION__), e);}).map([&](IXAudio2MasteringVoice* t){return mastering_voice{std::move(t)};});}
	expected<mastering_voice, hresult_error> create_mastering_voice(mastering_voice::property&& format)const{return com_create_resource<IXAudio2MasteringVoice>([&](IXAudio2MasteringVoice** ptr){return (*this)->CreateMasteringVoice(ptr, format.inchanls, format.insamprate, format.flag_, format.dev_id, format.effectchain, format.category);}).emap([](HRESULT e){return make_unexpected<hresult_error>(_T(__FUNCTION__), e);}).map([&](IXAudio2MasteringVoice* t){return mastering_voice{std::move(t)};});}
	expected<source_voice, hresult_error> create_source_voice(source_voice::property&& format)const{return com_create_resource<IXAudio2SourceVoice>([&](IXAudio2SourceVoice** ptr){return (*this)->CreateSourceVoice(ptr, &format.get(), format.flag_, format.max_freq_ratio, format.callback_, format.sendlist, format.effectchain);}).emap([](HRESULT e){return make_unexpected<hresult_error>(_T(__FUNCTION__), e);}).map([&](IXAudio2SourceVoice* t){return source_voice{std::move(t)};});}
	expected<source_voice, hresult_error> create_source_voice(source_voice::property& format)const{return com_create_resource<IXAudio2SourceVoice>([&](IXAudio2SourceVoice** ptr){return (*this)->CreateSourceVoice(ptr, &format.get(), format.flag_, format.max_freq_ratio, format.callback_, format.sendlist, format.effectchain);}).emap([](HRESULT e){return make_unexpected<hresult_error>(_T(__FUNCTION__), e);}).map([&](IXAudio2SourceVoice* t){return source_voice{std::move(t)};});}
	expected<source_voice, hresult_error> create_source_voice(IMFMediaType* media_type)const{return source_voice::property::convert_from_mf_media_type(media_type).bind([&](source_voice::property&& prop){return create_source_voice(std::move(prop));});}
	template<typename MediaType>
	expected<source_voice, hresult_error> create_source_voice(MediaType&& m)const{return create_source_voice(std::forward<MediaType>(m).get());}
};
}