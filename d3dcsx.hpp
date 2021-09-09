#pragma once
#include"com.hpp"
#include"d3d.hpp"
#include"amp.hpp"
#include"amp_graphics.hpp"
#include<d3dcsx.h>
#pragma comment(lib, "d3dcsx")

namespace will{

namespace d3dx11{

class fft : public detail::resource<::ID3DX11FFT>{
	using resource::resource;
	static expected<will::com_ptr<::ID3D11DeviceContext>, hresult_error> get_immediate_context(const concurrency::accelerator_view& av){
		will::com_ptr<::IUnknown> d = concurrency::direct3d::get_device(av);
		const TCHAR* t = _T(__FUNCTION__);
		if(!d)
			return will::make_unexpected<hresult_error>(t, E_INVALIDARG);
		return d.as<::ID3D11Device1>().map/*bind*/([&](will::com_ptr<::ID3D11Device1>&& dev){
			::ID3D11DeviceContext* ptr;
			dev->GetImmediateContext(&ptr);
			return will::com_ptr<::ID3D11DeviceContext>{std::move(ptr)};//.as<::ID3D11DeviceContext1>().map([](will::com_ptr<::ID3D11DeviceContext1>&& ptr){return d3d::device::context(std::move(ptr));});
		});
	}
	static std::vector<::ID3D11UnorderedAccessView*> unwrap_uav_vec(std::vector<::ID3D11UnorderedAccessView*> vec)noexcept{return std::move(vec);}
	template<typename D3D11UAV>
	static std::vector<::ID3D11UnorderedAccessView*> unwrap_uav_vec(const std::vector<D3D11UAV>& vec){
		std::vector<decltype(vec.front().get())> v;
		v.reserve(vec.size());
		for(auto&& x : vec)
			v.emplace_back(x.get());
		return unwrap_uav_vec(std::move(v));
	}
public:
	static inline expected<std::pair<fft, ::D3DX11_FFT_BUFFER_INFO>, hresult_error> create(::ID3D11DeviceContext* devcont, const ::D3DX11_FFT_DESC& desc, UINT flags = 0u){
		::D3DX11_FFT_BUFFER_INFO info;
		const TCHAR* t = _T(__FUNCTION__);
		return com_create_resource<::ID3DX11FFT>([&](::ID3DX11FFT** ptr){return ::D3DX11CreateFFT(devcont, &desc, flags, &info, ptr);})
			.map([&info](::ID3DX11FFT* v){return std::make_pair(fft{std::move(v)}, std::move(info));}).emap([&](HRESULT e){return make_unexpected<hresult_error>(t, e);});
	}
	template<typename D3D11DeviceContext, std::enable_if_t<!std::is_same<concurrency::accelerator_view, std::decay_t<D3D11DeviceContext>>::value && !std::is_base_of<::ID3D11DeviceContext, std::remove_pointer_t<std::decay_t<D3D11DeviceContext>>>::value, std::nullptr_t> = nullptr>
	static inline expected<std::pair<fft, ::D3DX11_FFT_BUFFER_INFO>, hresult_error> create(D3D11DeviceContext&& devcont, const ::D3DX11_FFT_DESC& desc, UINT flags = 0u){
		return create(std::forward<D3D11DeviceContext>(devcont).get(), desc, flags);
	}
	static inline expected<std::pair<fft, ::D3DX11_FFT_BUFFER_INFO>, hresult_error> create(will::amp::accelerator_view& av, const ::D3DX11_FFT_DESC& desc, UINT flags = 0u){
		will::com_ptr<::IUnknown> d = concurrency::direct3d::get_device(av);
		const TCHAR* t = _T(__FUNCTION__);
		if(!d)
			return will::make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);
		return get_immediate_context(av).bind([&](will::com_ptr<::ID3D11DeviceContext>&& ptr){return create(ptr, desc, flags);});
	}
	struct description : detail::property<::D3DX11_FFT_DESC>{
		using property::property;
		description& element_lengths(const concurrency::extent<1>& ext){
			prop.NumDimensions = 1u;
			prop.ElementLengths[0] = ext[0];
			prop.DimensionMask = ::D3DX11_FFT_DIM_MASK_1D;
			return *this;
		}
		description& element_lengths(const concurrency::extent<2>& ext){
			prop.NumDimensions = 2u;
			prop.ElementLengths[0] = ext[0];
			prop.ElementLengths[1] = ext[1];
			prop.DimensionMask = ::D3DX11_FFT_DIM_MASK_2D;
			return *this;
		}
		description& element_lengths(const concurrency::extent<3>& ext){
			prop.NumDimensions = 3u;
			prop.ElementLengths[0] = ext[0];
			prop.ElementLengths[1] = ext[1];
			prop.ElementLengths[2] = ext[2];
			prop.DimensionMask = ::D3DX11_FFT_DIM_MASK_3D;
			return *this;
		}
		description& type(::D3DX11_FFT_DATA_TYPE t){prop.Type = t; return *this;}
	};
	static ::D3D11_BUFFER_DESC buffer_description(UINT temp_buffer_float_size){
		return {temp_buffer_float_size * sizeof(float), ::D3D11_USAGE_DEFAULT, ::D3D11_BIND_UNORDERED_ACCESS, 0u, ::D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS, 0u};
	}
	static ::D3D11_UNORDERED_ACCESS_VIEW_DESC unordered_access_view_description(UINT temp_buffer_float_size){
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavd{::DXGI_FORMAT_R32_TYPELESS, ::D3D11_UAV_DIMENSION_BUFFER};
		uavd.Buffer.FirstElement = 0u;
		uavd.Buffer.NumElements = temp_buffer_float_size;
		uavd.Buffer.Flags = ::D3D11_BUFFER_UAV_FLAG_RAW;
		return uavd;
	}
	expected<fft&, hresult_error> set_forward_scale(FLOAT forward_scale)&{
		const auto hr = (*this)->SetForwardScale(forward_scale);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return *this;
	}
	expected<fft, hresult_error> set_forward_scale(FLOAT forward_scale)&&{
		const auto hr = (*this)->SetForwardScale(forward_scale);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return std::move(*this);
	}
	expected<fft&, hresult_error> set_inverse_scale(FLOAT inverse_scale)&{
		const auto hr = (*this)->SetInverseScale(inverse_scale);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return *this;
	}
	expected<fft, hresult_error> set_inverse_scale(FLOAT inverse_scale)&&{
		const auto hr = (*this)->SetInverseScale(inverse_scale);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return std::move(*this);
	}
	FLOAT get_forward_scale()const{return (*this)->GetForwardScale();}
	FLOAT get_inverse_scale()const{return (*this)->GetInverseScale();}
	__declspec(property(get=get_forward_scale, put=set_forward_scale)) FLOAT forward_scale;
	__declspec(property(get=get_inverse_scale, put=set_inverse_scale)) FLOAT inverse_scale;
	expected<fft&, hresult_error> attach_buffers_and_precompute(std::size_t num_temp_buffers, ::ID3D11UnorderedAccessView*const* temp_buffers, std::size_t num_precompute_buffers, ::ID3D11UnorderedAccessView*const* precompute_buffers)&{
		const auto hr = (*this)->AttachBuffersAndPrecompute(static_cast<UINT>(num_temp_buffers), temp_buffers, static_cast<UINT>(num_precompute_buffers), precompute_buffers);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return *this;
	}
	expected<fft, hresult_error> attach_buffers_and_precompute(std::size_t num_temp_buffers, ::ID3D11UnorderedAccessView*const* temp_buffers, std::size_t num_precompute_buffers, ::ID3D11UnorderedAccessView*const* precompute_buffers)&&{
		const auto hr = (*this)->AttachBuffersAndPrecompute(static_cast<UINT>(num_temp_buffers), temp_buffers, static_cast<UINT>(num_precompute_buffers), precompute_buffers);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return std::move(*this);
	}
	expected<fft&, hresult_error> attach_buffers_and_precompute(const std::vector<::ID3D11UnorderedAccessView*>& temp_buffers, const std::vector<::ID3D11UnorderedAccessView*>& precompute_buffers)&{
		return attach_buffers_and_precompute(temp_buffers.size(), temp_buffers.data(), precompute_buffers.size(), precompute_buffers.data());
	}
	expected<fft, hresult_error> attach_buffers_and_precompute(const std::vector<::ID3D11UnorderedAccessView*>& temp_buffers, const std::vector<::ID3D11UnorderedAccessView*>& precompute_buffers)&&{
		return std::move(*this).attach_buffers_and_precompute(temp_buffers.size(), temp_buffers.data(), precompute_buffers.size(), precompute_buffers.data());
	}
	template<typename D3D11UAV1, typename D3D11UAV2>
	expected<fft&, hresult_error> attach_buffers_and_precompute(const std::vector<D3D11UAV1>& temp_buffers, const std::vector<D3D11UAV2>& precompute_buffers)&{
		return attach_buffers_and_precompute(unwrap_uav_vec(temp_buffers), unwrap_uav_vec(precompute_buffers));
	}
	template<typename D3D11UAV1, typename D3D11UAV2>
	expected<fft, hresult_error> attach_buffers_and_precompute(const std::vector<D3D11UAV1>& temp_buffers, const std::vector<D3D11UAV2>& precompute_buffers)&&{
		return std::move(*this).attach_buffers_and_precompute(unwrap_uav_vec(temp_buffers), unwrap_uav_vec(precompute_buffers));
	}
	expected<d3d::unordered_access_view, hresult_error> forward_transform(const ::ID3D11UnorderedAccessView* input_buffer){
		return detail::convert_to_rich_interface<d3d::unordered_access_view>(com_create_resource<ID3D11UnorderedAccessView>([&](::ID3D11UnorderedAccessView** ptr){*ptr = nullptr; return (*this)->ForwardTransform(input_buffer, ptr);}), _T(__FUNCTION__));
	}
	expected<d3d::unordered_access_view, hresult_error> inverse_transform(const ::ID3D11UnorderedAccessView* input_buffer){
		return detail::convert_to_rich_interface<d3d::unordered_access_view>(com_create_resource<ID3D11UnorderedAccessView>([&](::ID3D11UnorderedAccessView** ptr){*ptr = nullptr; return (*this)->InverseTransform(input_buffer, ptr);}), _T(__FUNCTION__));
	}
	expected<void, hresult_error> forward_transform(const ::ID3D11UnorderedAccessView* input_buffer, ::ID3D11UnorderedAccessView* output_buffer){
		const auto hr = (*this)->ForwardTransform(input_buffer, &output_buffer);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return {};
	}
	expected<void, hresult_error> inverse_transform(const ::ID3D11UnorderedAccessView* input_buffer, ::ID3D11UnorderedAccessView* output_buffer){
		const auto hr = (*this)->InverseTransform(input_buffer, &output_buffer);
		if(FAILED(hr))
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		return {};
	}
	template<typename D3D11UAV, std::enable_if_t<!std::is_same<::ID3D11UnorderedAccessView, std::remove_pointer_t<std::decay_t<D3D11UAV>>>::value, std::nullptr_t> = nullptr>
	expected<d3d::unordered_access_view, hresult_error> forward_transform(D3D11UAV&& input_buffer){
		return forward_transform(std::forward<D3D11UAV>(input_buffer).get());
	}
	template<typename D3D11UAV, std::enable_if_t<!std::is_same<::ID3D11UnorderedAccessView, std::remove_pointer_t<std::decay_t<D3D11UAV>>>::value, std::nullptr_t> = nullptr>
	expected<d3d::unordered_access_view, hresult_error> inverse_transform(D3D11UAV&& input_buffer){
		return inverse_transform(std::forward<D3D11UAV>(input_buffer).get());
	}
};

}

namespace amp{

namespace detail{

template<typename T>struct d3dx11_fft_type_traits{static constexpr bool is_supported = false;};
template<>struct d3dx11_fft_type_traits<float>{
	static constexpr bool is_supported = true;
	using element_type_on_amp = float;
	static constexpr ::D3DX11_FFT_DATA_TYPE fft_data_type = ::D3DX11_FFT_DATA_TYPE_REAL;
	static constexpr std::size_t elements_on_each_type = sizeof(float) / sizeof(element_type_on_amp);
};
template<>struct d3dx11_fft_type_traits<concurrency::graphics::float_2>{
	static constexpr bool is_supported = true;
	using element_type_on_amp = float;
	static constexpr ::D3DX11_FFT_DATA_TYPE fft_data_type = ::D3DX11_FFT_DATA_TYPE_COMPLEX;
	static constexpr std::size_t elements_on_each_type = sizeof(concurrency::graphics::float_2) / sizeof(element_type_on_amp);
};
template<>struct d3dx11_fft_type_traits<will::amp::shader::float2> : d3dx11_fft_type_traits<concurrency::graphics::float_2>{};

}

template<typename T, int Dim>
class fft{
	accelerator_view& av;
	d3dx11::fft impl;
	d3d::device dev;
	std::vector<d3d::unordered_access_view> temps;
	std::vector<d3d::unordered_access_view> precomputes;
	UINT total_size;
	concurrency::extent<Dim> extent;
	explicit fft(accelerator_view& av, d3dx11::fft&& fft, d3d::device&& dev, std::vector<d3d::unordered_access_view>&& temps, std::vector<d3d::unordered_access_view>&& precomps, concurrency::extent<Dim>&& ext):av{av}, impl{std::move(fft)}, dev{std::move(dev)}, temps(std::move(temps)), precomputes(std::move(precomps)), extent{std::move(ext)}{}
	static expected<d3d::device, hresult_error> get_device(concurrency::accelerator_view& av){
		will::com_ptr<::IUnknown> d = concurrency::direct3d::get_device(av);
		const TCHAR* t = _T(__FUNCTION__);
		if(!d)
			return will::make_unexpected<hresult_error>(t, E_INVALIDARG);
		return d.as<::ID3D11Device1>().map([&](will::com_ptr<::ID3D11Device1>&& dev){
			return d3d::device{std::move(dev)};
		});
	}
	template<typename U, int Dimension>
	static expected<d3d::buffer, hresult_error> get_buffer(const concurrency::array<U, Dimension>& arr){
		will::com_ptr<::IUnknown> d = concurrency::direct3d::get_buffer(arr);
		return d.as<::ID3D11Buffer>().map([](will::com_ptr<::ID3D11Buffer>&& ptr){return d3d::buffer{std::move(ptr)};});
	}
	template<bool Forward, typename I, typename O>
	expected<void, hresult_error> transform(const concurrency::array<I, Dim>& input, concurrency::array<O, Dim>& output){
		if(input.extent != extent || output.extent != extent)
			return make_unexpected<hresult_error>(_T(__FUNCTION__), E_INVALIDARG);
		constexpr bool is_real = detail::d3dx11_fft_type_traits<T>::fft_data_type == ::D3DX11_FFT_DATA_TYPE_REAL;
		const auto i_size = total_size * (is_real && !Forward ? 2u : 1u);
		const auto o_size = total_size * (is_real &&  Forward ? 2u : 1u);
		auto i = get_buffer(input);
		auto o = get_buffer(output);
		//{
			const auto ti = impl.unordered_access_view_description(i_size);
			std::cout << ti.Format << std::endl;
			std::cout << ti.ViewDimension << std::endl;
			std::cout << ti.Buffer.FirstElement << std::endl;
			std::cout << ti.Buffer.NumElements << std::endl;
			std::cout << ti.Buffer.Flags << std::endl;
			const auto to = impl.unordered_access_view_description(o_size);
			std::cout << to.Format << std::endl;
			std::cout << to.ViewDimension << std::endl;
			std::cout << to.Buffer.FirstElement << std::endl;
			std::cout << to.Buffer.NumElements << std::endl;
			std::cout << to.Buffer.Flags << std::endl;
		//}
		::ID3D11UnorderedAccessView* uav;
		if(FAILED(dev->CreateUnorderedAccessView(i--->get(), &ti, &uav)))
			throw std::runtime_error("po!w");
		std::cout << "did it" << std::endl;
		throw std::runtime_error("did it");
#if 0
		return i.bind([&](d3d::buffer& buf){return dev.create_unordered_access_view(buf, ti/*impl.unordered_access_view_description(i_size)*/).bind([&](d3d::unordered_access_view&& i){
		return o.bind([&](d3d::buffer& buf){return dev.create_unordered_access_view(buf, to/*impl.unordered_access_view_description(o_size)*/).bind([&](d3d::unordered_access_view&& o){
		if constexpr(Forward)
			return impl.forward_transform(i.get(), o.get());
		else
			return impl.inverse_transform(i.get(), o.get());
		});});});});
#endif
	}
public:
	static_assert(1 <= Dim && Dim <= 3);
	static_assert(detail::d3dx11_fft_type_traits<T>::is_supported, "fft only supports float and concurrency::graphics::float_2 as template parameter T");
	using array_type = concurrency::array<T, Dim>;
	using fourier_transformed_array_type = concurrency::array<will::amp::shader::vec<typename detail::d3dx11_fft_type_traits<T>::element_type_on_amp, 2>, Dim>;
	static expected<fft, hresult_error> create(accelerator_view& av, concurrency::extent<Dim> ext, float forward_scale = 0.f, float inverse_scale = 0.f){
		return get_device(av).bind([&](d3d::device&& dev){
		return d3dx11::fft::create(dev.get_immediate_context(), d3dx11::fft::description{}.element_lengths(ext), ::D3DX11_FFT_CREATE_FLAG_NO_PRECOMPUTE_BUFFERS).bind([&](std::pair<d3dx11::fft, ::D3DX11_FFT_BUFFER_INFO>&& fs)->expected<fft, hresult_error>{
			UINT total_size = static_cast<UINT>(detail::d3dx11_fft_type_traits<T>::elements_on_each_type);
			for(unsigned int i = 0; i < static_cast<unsigned int>(Dim); ++i)
				total_size *= ext[i];
			std::vector<d3d::unordered_access_view> temps;
			temps.reserve(fs.second.NumTempBufferSizes);
			for(UINT i = 0; i < fs.second.NumTempBufferSizes; ++i){
				auto buf = dev.create_buffer(d3dx11::fft::buffer_description(fs.second.TempBufferFloatSizes[i]));
				if(!buf)
					return make_unexpected(buf.error());
				auto uav = dev.create_unordered_access_view(*buf, d3dx11::fft::unordered_access_view_description(fs.second.TempBufferFloatSizes[i]));
				if(!uav)
					return make_unexpected(uav.error());
				temps.emplace_back(std::move(*uav));
			}
			std::vector<d3d::unordered_access_view> precomputes;
			temps.reserve(fs.second.NumPrecomputeBufferSizes);
			for(UINT i = 0; i < fs.second.NumPrecomputeBufferSizes; ++i){
				auto buf = dev.create_buffer(fs.first.buffer_description(fs.second.PrecomputeBufferFloatSizes[i]));
				if(!buf)
					return make_unexpected(buf.error());
				auto uav = dev.create_unordered_access_view(*buf, d3dx11::fft::unordered_access_view_description(fs.second.PrecomputeBufferFloatSizes[i]));
				if(!uav)
					return make_unexpected(uav.error());
				precomputes.emplace_back(std::move(*uav));
			}
		return std::move(fs).first.attach_buffers_and_precompute(temps, precomputes).bind([&](d3dx11::fft&& f){
		return std::move(f).set_forward_scale(forward_scale).bind([&](d3dx11::fft&& f){
		return std::move(f).set_inverse_scale(inverse_scale).map([&](d3dx11::fft&& f){
		return fft<T, Dim>{av, std::move(f), std::move(dev), std::move(temps), std::move(precomputes), std::move(ext)};});});});});});
	}
	expected<void, hresult_error> forward_transform(const array_type& input, fourier_transformed_array_type& output){
		return transform<true>(input, output);
	}
	expected<fourier_transformed_array_type, hresult_error> forward_transform(const array_type& input){
		fourier_transformed_array_type output(input.extent, av);
		return forward_transform(input, output).map([&]{return std::move(output);});
	}
	expected<void, hresult_error> inverse_transform(const fourier_transformed_array_type& input, array_type& output){
		return transform<false>(input, output);
	}
	expected<array_type, hresult_error> inverse_transform(const fourier_transformed_array_type& input){
		array_type output(input.extent, av);
		return inverse_transform(input, output).map([&]{return std::move(output);});
	}
};

}

}