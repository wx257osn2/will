//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"xaudio2.hpp"
#include"media_foundation.hpp"
#include<array>
namespace will{
class audio : public xaudio2, public xaudio2::mastering_voice{
	media_foundation mf;
	audio(xaudio2&& x, xaudio2::mastering_voice&& mb, media_foundation&& m):xaudio2(std::move(x)), mastering_voice(std::move(mb)), mf(std::move(m)){}
public:
	static expected<audio, hresult_error> create(UINT32 flags, XAUDIO2_PROCESSOR processor, xaudio2::mastering_voice::property&& prop, media_foundation&& mf){
		return xaudio2::create(flags, processor).bind([&](xaudio2&& x){
			return x.create_mastering_voice(std::move(prop)).map([&](xaudio2::mastering_voice&& mb){
				return audio(std::move(x), std::move(mb), std::move(mf));
			});
		});
	}
	static expected<audio, hresult_error> create(UINT32 flags, XAUDIO2_PROCESSOR processor, media_foundation&& mf){
		return xaudio2::create(flags, processor).bind([&](xaudio2&& x){
			return x.create_mastering_voice().map([&](xaudio2::mastering_voice&& mb){
				return audio(std::move(x), std::move(mb), std::move(mf));
			});
		});
	}
	static expected<audio, hresult_error> create(UINT32 flags, media_foundation&& mf){
		return xaudio2::create(flags).bind([&](xaudio2&& x){
			return x.create_mastering_voice().map([&](xaudio2::mastering_voice&& mb){
				return audio(std::move(x), std::move(mb), std::move(mf));
			});
		});
	}
	static expected<audio, hresult_error> create(media_foundation&& mf){
		return xaudio2::create().bind([&](xaudio2&& x){
			return x.create_mastering_voice().map([&](xaudio2::mastering_voice&& mb){
				return audio(std::move(x), std::move(mb), std::move(mf));
			});
		});
	}
	class source : private media_foundation::source_reader, public media_foundation::media_type, public xaudio2::source_voice{
		source(media_foundation::source_reader&& sr, media_foundation::media_type&& mt, xaudio2::source_voice&& sv):source_reader{std::move(sr)}, media_type{std::move(mt)}, source_voice{std::move(sv)}{}
		friend audio;
		will::expected<std::vector<std::uint8_t>, will::hresult_error> read(const std::chrono::milliseconds& length, std::vector<std::uint8_t>&& data = {})const{
			return this->get_audio_bytes_per_second()
				.bind([&](UINT32 bps)->will::expected<std::vector<std::uint8_t>, will::hresult_error>{
			bool exists = false;
			data.reserve((length.count()+500) * bps / 1000);
			data.clear();
			will::expected<void, will::hresult_error> _;
			while(static_cast<long long>(data.size()) < length.count() * bps / 1000){
				_ = this->read_sample(MF_SOURCE_READER_FIRST_AUDIO_STREAM)
					.bind([&](media_foundation::source_reader::read_sample_result&& result)->will::expected<std::vector<std::uint8_t>, will::hresult_error>{
				if(result.is_eos())
					return will::make_unexpected<will::hresult_error>(_T("will::audio::source::read"), S_FALSE);
				return result.sample.convert_to_contiguous_buffer()
					.bind([&](auto&& mb){
				return mb.lock()
					.bind([&](auto&& mbl){
				return mbl.copy();
					});});}).map([&](auto&& vec){
				data.insert(data.end(), vec.begin(), vec.end());
				exists = true;
					});
				if(!_)
					break;
			}
			if(exists)
				return data;
			else
				return _.get_unexpected();
				});
		}
		will::expected<std::vector<std::uint8_t>, will::hresult_error> read(std::vector<std::uint8_t>&& data = {})const{
			return this->get_audio_bytes_per_second()
				.bind([&](UINT32 bps)->will::expected<std::vector<std::uint8_t>, will::hresult_error>{
			bool exists = false;
			data.clear();
			will::expected<void, will::hresult_error> _;
			while(true){
				_ = this->read_sample(MF_SOURCE_READER_FIRST_AUDIO_STREAM)
					.bind([&](media_foundation::source_reader::read_sample_result&& result)->will::expected<std::vector<std::uint8_t>, will::hresult_error>{
				if(result.is_eos())
					return will::make_unexpected<will::hresult_error>(_T("will::audio::source::read"), S_FALSE);
				return result.sample.convert_to_contiguous_buffer()
					.bind([&](auto&& mb){
				return mb.lock()
					.bind([&](auto&& mbl){
				return mbl.copy();
					});});}).map([&](auto&& vec){
				data.insert(data.end(), vec.begin(), vec.end());
				exists = true;
					});
				if(!_)
					break;
			}
			if(exists)
				return data;
			else
				return _.get_unexpected();
				});
		}
	public:
		using source_reader::set_current_position;
		template<std::size_t N>
		struct buffer{
			std::array<std::vector<std::uint8_t>, N> buf;
			std::uint8_t index;
		public:
			constexpr buffer()noexcept = default;
			xaudio2::buffer_info get()const{
				return xaudio2::buffer_info{buf[index]};
			}
			expected<void, hresult_error> update(const source& s, const std::chrono::milliseconds& length){
				return s.read(length)
					.map([&](auto&& lsb){
				const auto next = (index+1)%N;
				buf[next] = std::move(lsb);
				index = static_cast<std::uint8_t>(next);
					});
			}
			constexpr std::size_t size()const noexcept{return buf.size();}
		};
		template<std::size_t N>
		expected<void, hresult_error> play(buffer<N>& buf, const std::chrono::milliseconds& buffering_length){
			if(get_state().BuffersQueued <= buf.size() - 1)
				return buf.update(*this, buffering_length)
					.bind([&]{
				return submit_buffer(buf.get());
					}).catch_error([&](auto&& err){
				if(err.get_error_code() == S_FALSE){
					using namespace std::chrono_literals;
					set_current_position(0s);
				}
					});
			return start();
		}
	};
	expected<source, hresult_error> read_audio_file(const wchar_t* file_path)const{
		return mf.create_source_reader(file_path)
			.bind([&](media_foundation::source_reader&& sr){return
		sr.set_stream_selection(MF_SOURCE_READER_ALL_STREAMS, false)
			.bind([&]{return
		sr.set_stream_selection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, true)
			.bind([&]{
		return mf.create_media_type()
			.bind([&](media_foundation::media_type&& mt){return
		mt.set_major_type(MFMediaType_Audio)
			.bind([&]{return
		mt.set_subtype(MFAudioFormat_PCM)
			.bind([&]{return
		sr.set_current_media_type(MF_SOURCE_READER_FIRST_AUDIO_STREAM, mt)
			.bind([&]{
		return sr.get_current_media_type(MF_SOURCE_READER_FIRST_AUDIO_STREAM)
			.bind([&](media_foundation::media_type&& t){
		return this->create_source_voice(t)
			.map([&](xaudio2::source_voice&& sv){
		return source{std::move(sr), std::move(t), std::move(sv)};
			});});});});});});});});});
	}
	expected<source, hresult_error> read_audio_file(const std::wstring& file_path)const{return read_audio_file(file_path.c_str());}
};
}