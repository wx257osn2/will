#pragma once
#include<sdkddkver.h>
#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
#include"winrt.hpp"
#include<shcore.h>
#pragma comment(lib, "Shcore.lib")
#include<windows.storage.streams.h>

namespace will{
struct random_access_stream : detail::inspectable<ABI::Windows::Storage::Streams::IRandomAccessStream>{
	using inspectable::inspectable;
	expected<void, hresult_error> set_size(unsigned long long v){
		const auto hr = (*this)->put_Size(v);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return {};
	}
	expected<unsigned long long, hresult_error> get_size(){
		unsigned long long t;
		const auto hr = (*this)->get_Size(&t);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return t;
	}
	static expected<random_access_stream, hresult_error> create_on_file(std::wstring_view file_path, ABI::Windows::Storage::FileAccessMode acc){
		return detail::convert_to_rich_interface<random_access_stream>(com_create_resource<ABI::Windows::Storage::Streams::IRandomAccessStream>([&](ABI::Windows::Storage::Streams::IRandomAccessStream** ptr){return ::CreateRandomAccessStreamOnFile(file_path.data(), acc, ABI::Windows::Storage::Streams::IID_IRandomAccessStream, reinterpret_cast<void**>(ptr));}), _T(__FUNCTION__));
	}
};
}
#endif
