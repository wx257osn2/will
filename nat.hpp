//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"socket.hpp"
#include"com.hpp"
#include"_resource_property.hpp"
#include<natupnp.h>
namespace will{
class upnp_nat{
	com_ptr<IUPnPNAT> nat;
	upnp_nat(com_ptr<IUPnPNAT>&& ptr):nat(std::move(ptr)){}
	upnp_nat(IUPnPNAT*&& ptr):nat(std::move(ptr)){}
	static auto bstr_failed(const TCHAR* fn){return [&](std::exception_ptr){return make_unexpected<hresult_error>(fn, E_OUTOFMEMORY);};}
public:
	static expected<upnp_nat, hresult_error> create(){
		return com_create_resource<IUPnPNAT>([](IUPnPNAT** ptr){return ::CoCreateInstance(CLSID_UPnPNAT,nullptr,CLSCTX_ALL,IID_IUPnPNAT,reinterpret_cast<void**>(ptr));}).emap([](HRESULT e){return make_unexpected<hresult_error>(_T(__FUNCTION__), e);}).map([](IUPnPNAT* t){return upnp_nat{std::move(t)};});
	}
	explicit upnp_nat():upnp_nat(+create()){}
	upnp_nat(const upnp_nat&) = delete;
	upnp_nat(upnp_nat&&) = default;
	~upnp_nat() = default;
	explicit operator bool()const{return static_cast<bool>(nat);}
	const com_ptr<IUPnPNAT>& operator->()const{return nat;}
	class port_mappings;
	class port_mapping{
		friend port_mappings;
		com_ptr<IStaticPortMapping> map;
		expected<std::wstring, hresult_error> get_(HRESULT(__stdcall IStaticPortMapping::*f)(BSTR*), const TCHAR* fn)const{
			BSTR str;
			const auto hr = (map.get()->*f)(&str);
			if(SUCCEEDED(hr))
				return std::wstring{bstr{std::move(str)}};
			return make_unexpected<hresult_error>(fn, hr);
		}
	public:
		explicit port_mapping(IStaticPortMapping*&& portm):map(std::move(portm)){}
		port_mapping(const port_mapping&) = default;
		port_mapping(port_mapping&&) = default;
		~port_mapping() = default;
		explicit operator bool()const{return static_cast<bool>(map);}
		expected<void, hresult_error> set_enable(bool enbl){
			const auto hr = map->Enable(enbl ? VARIANT_TRUE : VARIANT_FALSE);
			if(SUCCEEDED(hr))
				return {};
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		expected<void, hresult_error> set_description(bstr str){
			const auto hr = map->EditDescription(str);
			if(SUCCEEDED(hr))
				return {};
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		expected<std::wstring, hresult_error> get_description()const{
			return get_(&IStaticPortMapping::get_Description, _T(__FUNCTION__));
		}
		expected<bool, hresult_error> is_enabled()const{
			VARIANT_BOOL b;
			const auto hr = map->get_Enabled(&b);
			if(SUCCEEDED(hr))
				return b == VARIANT_TRUE;
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		expected<std::wstring, hresult_error> get_external_ip_addr()const{
			return get_(&IStaticPortMapping::get_ExternalIPAddress, _T(__FUNCTION__));
		}
		expected<long, hresult_error> get_external_port()const{
			long p;
			map->get_ExternalPort(&p);
			return p;
		}
		expected<std::wstring, hresult_error> get_internal_client()const{
			return get_(&IStaticPortMapping::get_InternalClient, _T(__FUNCTION__));
		}
		expected<long, hresult_error> get_internal_port()const{
			long p;
			const auto hr = map->get_InternalPort(&p);
			if(SUCCEEDED(hr))
				return p;
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		expected<std::wstring, hresult_error> get_protocol()const{
			return get_(&IStaticPortMapping::get_Protocol, _T(__FUNCTION__));
		}
	};
	class port_mappings{
		com_ptr<IStaticPortMappingCollection> portmaps;
		port_mappings(IStaticPortMappingCollection* ptr):portmaps(std::move(ptr)){}
		friend upnp_nat;
	public:
		port_mappings(const port_mappings&) = delete;
		port_mappings(port_mappings&&) = default;
		~port_mappings() = default;
		explicit operator bool()const{return static_cast<bool>(portmaps);}
		expected<std::size_t, hresult_error> size()const{
			long s;
			const auto hr = portmaps->get_Count(&s);
			if(SUCCEEDED(hr))
				return s;
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		expected<port_mapping, hresult_error> add(long port, bool if_true_then_TCP_else_UDP, std::wstring local_ip, std::wstring registered_name){
			auto&& e = bstr_failed(_T(__FUNCTION__));
			return bstr::make(if_true_then_TCP_else_UDP ? L"TCP" : L"UDP").emap(e).bind([&](bstr&& protocol){
				return bstr::make(local_ip).emap(e).bind([&](bstr&& ip){
					return bstr::make(registered_name).emap(e).bind([&](bstr&& name){
						return add(port, protocol, ip, name);
					});
				});
			});
		}
		expected<port_mapping, hresult_error> add(long port, bstr TCP_or_UDP, bstr local_ip, bstr registered_name){
			return com_create_resource<IStaticPortMapping>([&](IStaticPortMapping** ptr){return portmaps->Add(port, TCP_or_UDP, port, local_ip, VARIANT_TRUE, registered_name, ptr);}).emap([](HRESULT e){return make_unexpected<hresult_error>(_T("will::upnp_nat::port_mappings::add"), e);}).map([&](IStaticPortMapping* t){return port_mapping{std::move(t)};});
		}
		expected<void, hresult_error> remove(port_mapping&& pm){
			return pm.get_external_port().bind([&](long port){
				return pm.get_protocol().bind([&](std::wstring&& protoc){
					return bstr::make(protoc).emap(bstr_failed(_T("will::upnp_nat::port_mappings::remove"))).bind([&](bstr&& protocol){
						return remove(port, protocol);
					});
				});
			});
		}
		expected<void, hresult_error> remove(long port, bool if_true_then_TCP_else_UDP){
			return bstr::make(if_true_then_TCP_else_UDP ? L"TCP" : L"UDP").emap(bstr_failed(_T(__FUNCTION__))).bind([&](bstr&& b){return remove(port, b);});
		}
		expected<void, hresult_error> remove(long port, bstr TCP_or_UDP){
			const auto hr = portmaps->Remove(port, TCP_or_UDP);
			if(SUCCEEDED(hr))
				return {};
			return make_unexpected<hresult_error>(_T(__FUNCTION__), hr);
		}
		const com_ptr<IStaticPortMappingCollection>& operator->()const{
			return portmaps;
		}
		using iterator = com_enum_iterator<IStaticPortMappingCollection, port_mapping>;
		iterator begin()const{return iterator{portmaps};}
		iterator end()const{return iterator{};}
	};
	expected<port_mappings, hresult_error> create_port_mappings()const{
		return com_create_resource<IStaticPortMappingCollection>([&](IStaticPortMappingCollection** ptr){return nat->get_StaticPortMappingCollection(ptr);}).emap([](HRESULT e){return make_unexpected<hresult_error>(_T("will::upnp_nat::create_port_mappings"), e);}).map([](IStaticPortMappingCollection* t){return port_mappings{std::move(t)};});
	}
};
}