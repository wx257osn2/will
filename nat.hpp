#pragma once
#include"socket.hpp"
#include"com.hpp"
#include<natupnp.h>
namespace will{
class upnp_nat{
	com_ptr<IUPnPNAT> nat;
public:
	explicit upnp_nat():nat(com_create_resource<IUPnPNAT>([](IUPnPNAT** ptr){return ::CoCreateInstance(CLSID_UPnPNAT,nullptr,CLSCTX_ALL,IID_IUPnPNAT,reinterpret_cast<void**>(ptr));})){}
	upnp_nat(const upnp_nat&) = default;
	upnp_nat(upnp_nat&&) = default;
	~upnp_nat() = default;
	explicit operator bool()const{return static_cast<bool>(nat);}
	com_ptr<IUPnPNAT> operator->()const{
		return nat;
	}
	class port_mappings;
	class port_mapping{
		friend port_mappings;
		com_ptr<IStaticPortMapping> map;
		bstr_adaptor get_(HRESULT(__stdcall IStaticPortMapping::*f)(BSTR*))const{
			return [&]()->bstr_adaptor{
				BSTR str;
				(map.get()->*f)(&str);
				return str;
			}();
		}
	public:
		explicit port_mapping(IStaticPortMapping* portm):map(portm){}
		port_mapping(const port_mapping&) = default;
		port_mapping(port_mapping&&) = default;
		~port_mapping() = default;
		explicit operator bool()const{return static_cast<bool>(map);}
		HRESULT set_enable(bool enbl){
			return map->Enable(enbl ? VARIANT_TRUE : VARIANT_FALSE);
		}
		HRESULT set_description(bstr_adaptor str){
			return map->EditDescription(str);
		}
		std::wstring get_description()const{
			return std::wstring(get_(&IStaticPortMapping::get_Description));
		}
		bool is_enabled()const{
			VARIANT_BOOL b;
			map->get_Enabled(&b);
			return b == VARIANT_TRUE;
		}
		std::wstring get_external_ip_addr()const{
			return std::wstring(get_(&IStaticPortMapping::get_ExternalIPAddress));
		}
		long get_external_port()const{
			long p;
			map->get_ExternalPort(&p);
			return p;
		}
		std::wstring get_internal_client()const{
			return std::wstring(get_(&IStaticPortMapping::get_InternalClient));
		}
		long get_internal_port()const{
			long p;
			map->get_InternalPort(&p);
			return p;
		}
		std::wstring get_protocol()const{
			return std::wstring(get_(&IStaticPortMapping::get_Protocol));
		}
	};
	class port_mappings{
		com_ptr<IStaticPortMappingCollection> portmaps;
	public:
		explicit port_mappings(const upnp_nat& nat):portmaps(com_create_resource<IStaticPortMappingCollection>([&](IStaticPortMappingCollection** ptr){return nat->get_StaticPortMappingCollection(ptr);})){}
		port_mappings(const port_mappings&) = default;
		port_mappings(port_mappings&&) = default;
		~port_mappings() = default;
		explicit operator bool()const{return static_cast<bool>(portmaps);}
		std::size_t size()const{
			long s;
			portmaps->get_Count(&s);
			return s;
		}
		port_mapping add(long port, bool if_true_then_TCP_else_UDP, std::wstring local_ip, std::wstring registered_name){
			return add(port, bstr_adaptor(if_true_then_TCP_else_UDP ? L"TCP" : L"UDP"), bstr_adaptor(local_ip), bstr_adaptor(registered_name));
		}
		port_mapping add(long port, bstr_adaptor TCP_or_UDP, bstr_adaptor local_ip, bstr_adaptor registered_name){
			return port_mapping(com_create_resource<IStaticPortMapping>([&](IStaticPortMapping** ptr){return portmaps->Add(port, TCP_or_UDP, port, local_ip, VARIANT_TRUE, registered_name, ptr);}));
		}
		HRESULT remove(port_mapping&& pm){
			return remove(pm.get_external_port(), pm.get_protocol());
		}
		HRESULT remove(long port, bool if_true_then_TCP_else_UDP){
			return remove(port, bstr_adaptor(if_true_then_TCP_else_UDP ? L"TCP" : L"UDP"));
		}
		HRESULT remove(long port, bstr_adaptor TCP_or_UDP){
			return portmaps->Remove(port, TCP_or_UDP);
		}
		com_ptr<IStaticPortMappingCollection> operator->()const{
			return portmaps;
		}
		class range_t{
			com_ptr<IUnknown> unk;
		public:
			explicit range_t(const port_mappings& collection):unk(com_create_resource<IUnknown>([&](IUnknown** ptr){return collection->get__NewEnum(ptr);})){}
			class iterator{
				com_ptr<IEnumVARIANT> enu;
				variant var;
				HRESULT hr;
			public:
				explicit iterator(const com_ptr<IUnknown>& unk):enu(com_create_resource<IEnumVARIANT>([&](IEnumVARIANT** ptr){return unk->QueryInterface(IID_IEnumVARIANT, reinterpret_cast<void**>(ptr));})), var(), hr(enu->Next(1, &var.get(), nullptr)){}
				explicit iterator():enu(nullptr), var(), hr(S_FALSE){}
				iterator& operator++(){VariantClear(&var.get()); hr = enu->Next(1, &var.get(), nullptr); return *this;}
				bool operator==(const iterator& rhs)const{return hr == rhs.hr;}
				bool operator!=(const iterator& rhs)const{return !(*this == rhs);}
				port_mapping operator*()const{
					return port_mapping(com_create_resource<IStaticPortMapping>([&](IStaticPortMapping** ptr){return var.get().pdispVal->QueryInterface(IID_IStaticPortMapping, reinterpret_cast<void**>(ptr));}));
				}
			};
			iterator begin()const{return iterator{unk};}
			iterator end()const{return iterator{};}
		};
		range_t range()const{return range_t(*this);}
	};
	port_mappings make_port_mappings()const{
		return port_mappings(*this);
	}
};
}