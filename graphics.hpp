//Copyright (C) 2014-2020 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"amp_math.hpp"
#include"d3d.hpp"
#include"dxgi.hpp"
#include"d2d.hpp"
#include"wic.hpp"
#include"dwrite.hpp"
#include"dcompo.hpp"
#include"amp.hpp"
#include"amp_graphics.hpp"
#include<memory>
#include<vector>
#include<array>
#include<tuple>

namespace will{

class pixel_format_converter{
	will::d3d::buffer vb;
	will::d3d::input_layout il;
	will::d3d::vertex_shader vs;
	will::d3d::pixel_shader ps;
	will::d3d::sampler_state ss;
	will::d3d::device::context dc;
	struct vertex{
		float pos[3];
		float uv[2];
	};
	static constexpr std::array<vertex, 4> vertexes{{
		{{-1.f, -1.f, .5f}, {0.f, 1.f}},
		{{-1.f,  1.f, .5f}, {0.f, 0.f}},
		{{ 1.f, -1.f, .5f}, {1.f, 1.f}},
		{{ 1.f,  1.f, .5f}, {1.f, 0.f}}
	}};
	explicit pixel_format_converter(will::d3d::buffer&& vertex_buffer, will::d3d::input_layout&& input_layout, will::d3d::vertex_shader&& vertex_shader, will::d3d::pixel_shader&& pixel_shader, will::d3d::sampler_state&& sampler_state, will::d3d::device::context&& deffered_context) : vb{std::move(vertex_buffer)}, il{std::move(input_layout)}, vs{std::move(vertex_shader)}, ps{std::move(pixel_shader)}, ss{std::move(sampler_state)}, dc{std::move(deffered_context)}{}
public:
	static will::expected<pixel_format_converter, hresult_error> create(will::d3d::device& dev){
		static constexpr std::array<BYTE, 616> vertex_shader = {{
			 68,  88,  66,  67, 204, 140,  66, 148, 129,  67,  51, 171, 232, 192, 138, 125, 194, 219,  25,  26,   1,   0,   0,   0,
			104,   2,   0,   0,   5,   0,   0,   0,  52,   0,   0,   0, 172,   0,   0,   0,   0,   1,   0,   0,  88,   1,   0,   0,
			204,   1,   0,   0,  82,  68,  69,  70, 112,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			 60,   0,   0,   0,   0,   5, 254, 255,   0,   1,   0,   0,  60,   0,   0,   0,  82,  68,  49,  49,  60,   0,   0,   0,
			 24,   0,   0,   0,  32,   0,   0,   0,  40,   0,   0,   0,  36,   0,   0,   0,  12,   0,   0,   0,   0,   0,   0,   0,
			 77, 105,  99, 114, 111, 115, 111, 102, 116,  32,  40,  82,  41,  32,  72,  76,  83,  76,  32,  83, 104,  97, 100, 101,
			114,  32,  67, 111, 109, 112, 105, 108, 101, 114,  32,  54,  46,  51,  46,  57,  54,  48,  48,  46,  49,  54,  51,  56,
			 52,   0, 171, 171,  73,  83,  71,  78,  76,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,  56,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,  15,  15,   0,   0,  65,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   1,   0,   0,   0,   3,   3,   0,   0,  80,  79,  83,  73,
			 84,  73,  79,  78,   0,  84,  69,  88,  67,  79,  79,  82,  68,   0, 171, 171,  79,  83,  71,  78,  80,   0,   0,   0,
			  2,   0,   0,   0,   8,   0,   0,   0,  56,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   3,   0,   0,   0,
			  0,   0,   0,   0,  15,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,
			  1,   0,   0,   0,   3,  12,   0,   0,  83,  86,  95,  80,  79,  83,  73,  84,  73,  79,  78,   0,  84,  69,  88,  67,
			 79,  79,  82,  68,   0, 171, 171, 171,  83,  72,  69,  88, 108,   0,   0,   0,  80,   0,   1,   0,  27,   0,   0,   0,
			106,   8,   0,   1,  95,   0,   0,   3, 242,  16,  16,   0,   0,   0,   0,   0,  95,   0,   0,   3,  50,  16,  16,   0,
			  1,   0,   0,   0, 103,   0,   0,   4, 242,  32,  16,   0,   0,   0,   0,   0,   1,   0,   0,   0, 101,   0,   0,   3,
			 50,  32,  16,   0,   1,   0,   0,   0,  54,   0,   0,   5, 242,  32,  16,   0,   0,   0,   0,   0,  70,  30,  16,   0,
			  0,   0,   0,   0,  54,   0,   0,   5,  50,  32,  16,   0,   1,   0,   0,   0,  70,  16,  16,   0,   1,   0,   0,   0,
			 62,   0,   0,   1,  83,  84,  65,  84, 148,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			  4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		}};
		static constexpr std::array<BYTE, 660> pixel_shader = {{
			 68,  88,  66,  67,   3,  10,  79, 110,  55, 128, 140,  49,  84, 243,  55, 101, 125,  13,   8, 175,   1,   0,   0,   0,
			148,   2,   0,   0,   5,   0,   0,   0,  52,   0,   0,   0, 244,   0,   0,   0,  76,   1,   0,   0, 128,   1,   0,   0,
			248,   1,   0,   0,  82,  68,  69,  70, 184,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,
			 60,   0,   0,   0,   0,   5, 255, 255,   0,   1,   0,   0, 132,   0,   0,   0,  82,  68,  49,  49,  60,   0,   0,   0,
			 24,   0,   0,   0,  32,   0,   0,   0,  40,   0,   0,   0,  36,   0,   0,   0,  12,   0,   0,   0,   0,   0,   0,   0,
			124,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			  1,   0,   0,   0,   1,   0,   0,   0, 128,   0,   0,   0,   2,   0,   0,   0,   5,   0,   0,   0,   4,   0,   0,   0,
			255, 255, 255, 255,   0,   0,   0,   0,   1,   0,   0,   0,  13,   0,   0,   0, 115,  97, 109,   0, 116, 101, 120,   0,
			 77, 105,  99, 114, 111, 115, 111, 102, 116,  32,  40,  82,  41,  32,  72,  76,  83,  76,  32,  83, 104,  97, 100, 101,
			114,  32,  67, 111, 109, 112, 105, 108, 101, 114,  32,  54,	 46,  51,  46,  57,  54,  48,  48,  46,  49,  54,  51,  56,
			 52,   0, 171, 171,  73,  83,  71,  78,  80,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,  56,   0,   0,   0,
			  0,   0,   0,   0,   1,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,  15,   0,   0,   0,  68,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   1,   0,   0,   0,   3,   3,   0,   0,  83,  86,  95,  80,
			 79,  83,  73,  84,  73,  79,  78,   0,  84,  69,  88,  67,  79,  79,  82,  68,   0, 171, 171, 171,  79,  83,  71,  78,
			 44,   0,   0,   0,   1,   0,   0,   0,   8,   0,   0,   0,  32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			  3,   0,   0,   0,   0,   0,   0,   0,  15,   0,   0,   0,  83,  86,  95,  84,  97, 114, 103, 101, 116,   0, 171, 171,
			 83,  72,  69,  88, 112,   0,   0,   0,  80,   0,   0,   0,  28,   0,   0,   0, 106,   8,   0,   1,  90,   0,   0,   3,
			  0,  96,  16,   0,   0,   0,   0,   0,  88,  24,   0,   4,   0, 112,  16,   0,   0,   0,   0,   0,  85,  85,   0,   0,
			 98,  16,   0,   3,  50,  16,  16,   0,   1,   0,   0,   0, 101,   0,   0,   3, 242,  32,  16,   0,   0,   0,   0,   0,
			 69,   0,   0, 139, 194,   0,   0, 128,  67,  85,  21,   0, 242,  32,  16,   0,   0,   0,   0,   0,  70,  16,  16,   0,
			  1,   0,   0,   0,  70, 126,  16,   0,   0,   0,   0,   0,   0,  96,  16,   0,   0,   0,   0,   0,  62,   0,   0,   1,
			 83,  84,  65,  84, 148,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
			}};
			static constexpr std::array<D3D11_INPUT_ELEMENT_DESC, 2> input_layout{{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			}};
		return
			dev.create_buffer(will::d3d::buffer::description{}.byte_width(static_cast<::UINT>(sizeof(vertexes))).bind_flags(D3D11_BIND_VERTEX_BUFFER), will::d3d::subresource::data{}.memory(vertexes.data())).bind([&](will::d3d::buffer&& vb){return
			dev.create_input_layout<sizeof(input_layout)/sizeof(D3D11_INPUT_ELEMENT_DESC), std::remove_const_t<std::remove_reference_t<decltype(vertex_shader[0])>>, sizeof(vertex_shader)>(input_layout, vertex_shader).bind([&](will::d3d::input_layout&& il){return
			dev.create_vertex_shader<std::remove_const_t<std::remove_reference_t<decltype(vertex_shader[0])>>, sizeof(vertex_shader)>(vertex_shader).bind([&](will::d3d::vertex_shader&& vs){return
			dev.create_pixel_shader<std::remove_const_t<std::remove_reference_t<decltype(pixel_shader[0])>>, sizeof(pixel_shader)>(pixel_shader).bind([&](will::d3d::pixel_shader&& ps){return
			dev.create_sampler_state().bind([&](will::d3d::sampler_state&& ss){return
			dev.create_deferred_context().map([&](will::d3d::device::context&& dc){
			return pixel_format_converter(std::move(vb), std::move(il), std::move(vs), std::move(ps), std::move(ss), std::move(dc));
			});});});});});});
	}
	will::expected<will::d3d::texture2d, hresult_error> convert(const will::d3d::texture2d& tex, will::d3d::texture2d::description desc, will::d3d::device& dev){
		auto d = tex.get_desc();
		if(desc.get().Width == 0)
			desc.width(d.Width);
		if(desc.get().Height == 0)
			desc.height(d.Height);
		if(will::dxgi::format::same_family(desc.get().Format, d.Format))
			return dev.create_texture2d(desc).map([&](will::d3d::texture2d&& t){
				dev.get_immediate_context().copy(t, tex);
				return std::move(t);
			});
		if((d.BindFlags & ::D3D11_BIND_SHADER_RESOURCE) != 0){
			desc.get().BindFlags |= ::D3D11_BIND_RENDER_TARGET;
			auto dst = dev.create_texture2d(desc);
			return dst.bind([&](will::d3d::texture2d&& dst){return
			dev.create_render_target_view(dst).bind([&](will::d3d::render_target_view&& rtv){return
			dev.create_shader_resource_view(tex, will::d3d::shader_resource_view::description{}.format(will::dxgi::format::is_typeless(tex.get_desc().Format) ? will::dxgi::format::to_unorm(tex.get_desc().Format) : tex.get_desc().Format).texture2d({0, tex.get_desc().MipLevels})).bind([&](will::d3d::shader_resource_view&& srv){return dc.finish_command_list([&](will::d3d::device::context& dc){
			dc.ia_set_input_layout(il);
			dc.ia_set_vertex_buffer(0, vb, sizeof(vertexes[0]), 0);
			dc.ia_set_primitive_topology(::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			dc.vs_set_shader(vs);
			dc.ps_set_sampler(0, ss);
			dc.ps_set_shader(ps);
			dc.rs_set_viewport(std::array<::D3D11_VIEWPORT, 1>{{{0, 0, static_cast<float>(d.Width), static_cast<float>(d.Height), 0.f, 1.f}}});
			dc.ps_set_shader_resource(0, srv);
			dc.om_set_render_targets(rtv, nullptr);
			dc.clear_render_target_view(rtv, {0.f, 0.f, 0.f, 0.f});
			dc.draw(4u);}).map([&](will::d3d::command_list&& cl){
			dev.get_immediate_context().execute_command_list(cl);
			});});}).map([&]{return std::move(dst);});});
		}
		d.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		return dev.create_texture2d(d).bind([&](will::d3d::texture2d&& t){
			dev.get_immediate_context().copy(t, tex);
			return convert(t, desc, dev);
		});
	}
};

namespace interop{

namespace detail{

template<typename, typename>struct memory_data2d;
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, will::d2d::bitmap::memory_data>{
	static will::d2d::bitmap::memory_data&& conv(will::d2d::bitmap::memory_data&& d){return std::move(d);}
	static const will::d2d::bitmap::memory_data& conv(const will::d2d::bitmap::memory_data& d){return d;}
};
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, will::d2d::bitmap::scoped_mapped_rect>{
	static will::d2d::bitmap::memory_data conv(will::d2d::bitmap::scoped_mapped_rect&& d){return {d.pitch, d.bits};}
	static const will::d2d::bitmap::memory_data& conv(const will::d2d::bitmap::scoped_mapped_rect& d){return d;}
};
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, will::d2d::bitmap::scoped_readonly_mapped_rect>{
	static will::d2d::bitmap::memory_data conv(will::d2d::bitmap::scoped_readonly_mapped_rect&& d){return {d.pitch, d.bits};}
	static const will::d2d::bitmap::memory_data& conv(const will::d2d::bitmap::scoped_readonly_mapped_rect& d){return d;}
};
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, will::d3d::device::subresource>{
	static will::d2d::bitmap::memory_data conv(const will::d3d::device::subresource& d){return {d.RowPitch, d.pData};}
};
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, will::d3d::device::context::scoped_subresource>{
	static will::d2d::bitmap::memory_data conv(const will::d3d::device::context::scoped_subresource& d){return {d.RowPitch, d.pData};}
};
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, ::D3D11_MAPPED_SUBRESOURCE>{
	static will::d2d::bitmap::memory_data conv(const ::D3D11_MAPPED_SUBRESOURCE& d){return {d.RowPitch, d.pData};}
};
template<>
struct memory_data2d<will::d2d::bitmap::memory_data, ::D3D11_SUBRESOURCE_DATA>{
	static will::d2d::bitmap::memory_data conv(const ::D3D11_SUBRESOURCE_DATA& d){return {d.SysMemPitch, d.pSysMem};}
};

template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, will::d2d::bitmap::memory_data>{
	static ::D3D11_SUBRESOURCE_DATA conv(const will::d2d::bitmap::memory_data& d){return {d.data, d.pitch};}
};
template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, will::d2d::bitmap::scoped_mapped_rect>{
	static ::D3D11_SUBRESOURCE_DATA conv(const will::d2d::bitmap::scoped_mapped_rect& d){return {d.bits, d.pitch};}
};
template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, will::d2d::bitmap::scoped_readonly_mapped_rect>{
	static ::D3D11_SUBRESOURCE_DATA conv(const will::d2d::bitmap::scoped_readonly_mapped_rect& d){return {d.bits, d.pitch};}
};
template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, will::d3d::device::subresource>{
	static ::D3D11_SUBRESOURCE_DATA conv(will::d3d::device::subresource&& d){return {d.pData, d.RowPitch};}
	static const ::D3D11_SUBRESOURCE_DATA& conv(const will::d3d::device::subresource& d){return reinterpret_cast<const ::D3D11_SUBRESOURCE_DATA&>(d);}
};
template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, will::d3d::device::context::scoped_subresource>{
	static ::D3D11_SUBRESOURCE_DATA conv(will::d3d::device::context::scoped_subresource&& d){return {d.pData, d.RowPitch};}
	static const ::D3D11_SUBRESOURCE_DATA& conv(const will::d3d::device::context::scoped_subresource& d){return reinterpret_cast<const ::D3D11_SUBRESOURCE_DATA&>(d);}
};
template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, ::D3D11_MAPPED_SUBRESOURCE>{
	static ::D3D11_SUBRESOURCE_DATA conv(::D3D11_MAPPED_SUBRESOURCE&& d){return {d.pData, d.RowPitch};}
	static const ::D3D11_SUBRESOURCE_DATA& conv(const ::D3D11_MAPPED_SUBRESOURCE& d){return reinterpret_cast<const ::D3D11_SUBRESOURCE_DATA&>(d);}
};
template<>
struct memory_data2d<::D3D11_SUBRESOURCE_DATA, ::D3D11_SUBRESOURCE_DATA>{
	static ::D3D11_SUBRESOURCE_DATA&& conv(::D3D11_SUBRESOURCE_DATA&& d){return std::move(d);}
	static const ::D3D11_SUBRESOURCE_DATA& conv(const ::D3D11_SUBRESOURCE_DATA& d){return d;}
};

}

template<typename To, typename From>
inline decltype(auto) memory_data2d(From&& from){return detail::memory_data2d<To, std::decay_t<From>>::conv(std::forward<From>(from));}
template<typename From>
inline decltype(auto) memory_data2d(From&& from){return detail::memory_data2d<will::d2d::bitmap::memory_data, std::decay_t<From>>::conv(std::forward<From>(from));}

namespace detail{

template<typename, typename>struct reinterpret_conversion_impl;
template<>
struct reinterpret_conversion_impl<will::d2d::bitmap, will::d2d::bitmap>{
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::d2d::bitmap& bmp, const will::d2d::bitmap::property& prop, const will::d2d::device::context& devcont){
		if(will::dxgi::format::same_family(bmp.get_pixel_format().format, prop.get().pixelFormat.format))
			return bmp.clone(devcont);
		if((bmp.get_bitmap_options() & D2D1_BITMAP_OPTIONS_CPU_READ) != 0){
			return bmp.scoped_map().bind([&](will::d2d::bitmap::scoped_readonly_mapped_rect&& rect){
				return devcont.create_bitmap(bmp.get_pixel_size(), rect);
			});
		}
		auto p = prop.get();
		p.bitmapOptions |= ::D2D1_BITMAP_OPTIONS_CPU_READ | ::D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
		return devcont.create_bitmap(bmp.get_pixel_size(), p).bind([&](will::d2d::bitmap&& t){
			return t.copy_from(bmp).bind([&](){
				return conv(t, prop, devcont);});});
	}
};
template<>
struct reinterpret_conversion_impl<will::d2d::bitmap, will::d3d::texture2d>{
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::d3d::texture2d& tex, const will::d2d::bitmap::property& prop, const will::d3d::device& dev, const will::d2d::device::context& devcont){
		auto desc = tex.get_desc();
		if(will::dxgi::format::same_family(desc.Format, prop.get().pixelFormat.format))
			return devcont.create_bitmap({desc.Width, desc.Height}, prop).map([&](will::d2d::bitmap&& bmp){
				auto _ [[maybe_unused]] = bmp.get_surface().map([&](will::dxgi::surface&& surf){
					dev.get_immediate_context().copy(will::d3d::texture2d{std::move(surf)}, tex);
				});
				return std::move(bmp);
			});
		if(desc.Usage == D3D11_USAGE_STAGING && (desc.CPUAccessFlags & D3D11_CPU_ACCESS_READ) != 0)
			return dev.get_immediate_context()
			          .create_scoped_subresource(tex, 0, D3D11_MAP_READ)
			          .bind([&](will::d3d::device::context::scoped_subresource&& sr){
				return devcont.create_bitmap({desc.Width, desc.Height}, will::interop::memory_data2d(std::move(sr)), prop);
			});
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.BindFlags = 0;
		return dev.create_texture2d(desc).bind([&](const will::d3d::texture2d& t){
			dev.get_immediate_context().copy(t, tex);
			return conv(t, prop, dev, devcont);
		});
	}
	template<typename RenderTarget>
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::d3d::texture2d& tex, const will::d2d::bitmap::property& prop, RenderTarget&& rt){return conv(tex, prop, rt.get_d3d_device(), rt);}
};
template<>
struct reinterpret_conversion_impl<will::d3d::texture2d, will::d3d::texture2d>{
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d3d::texture2d& tex, will::d3d::texture2d::description desc, const will::d3d::device& dev){
		auto d = tex.get_desc();
		if(desc.get().Width == 0)
			desc.width(d.Width);
		if(desc.get().Height == 0)
			desc.height(d.Height);
		if(will::dxgi::format::same_family(desc.get().Format, tex.get_desc().Format))
			return dev.create_texture2d(desc).map([&](will::d3d::texture2d&& t){
				dev.get_immediate_context().copy(t, tex);
				return std::move(t);
			});
		if(tex.get_desc().Usage == D3D11_USAGE_STAGING && (tex.get_desc().CPUAccessFlags & D3D11_CPU_ACCESS_READ) != 0)
			return dev.get_immediate_context()
			          .create_scoped_subresource(tex, 0, D3D11_MAP_READ)
			          .bind([&](will::d3d::device::context::scoped_subresource&& sr){
				return dev.create_texture2d(desc, will::interop::memory_data2d<::D3D11_SUBRESOURCE_DATA>(std::move(sr)));
			});
		d.Usage = D3D11_USAGE_STAGING;
		d.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		d.BindFlags = 0;
		return dev.create_texture2d(d).bind([&](will::d3d::texture2d&& t){
			dev.get_immediate_context().copy(t, tex);
			return conv(t, desc, dev);
		});
	}
	template<typename RenderTarget>
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d3d::texture2d& tex, will::d3d::texture2d::description desc, RenderTarget&& rt){return conv(tex, desc, rt.get_d3d_device());}
};
template<>
struct reinterpret_conversion_impl<will::d3d::texture2d, will::d2d::bitmap>{
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d2d::bitmap& bmp, will::d3d::texture2d::description desc, const will::d3d::device& dev, const will::d2d::device::context& devcont){
		{
			const auto p = bmp.get_pixel_size();
			if(desc.get().Width == 0)
				desc.width(p.width);
			if(desc.get().Height == 0)
				desc.height(p.height);
		}
		if(will::dxgi::format::same_family(desc.get().Format, bmp.get_pixel_format().format))
			return dev.create_texture2d(desc).map([&](will::d3d::texture2d&& tex){
				auto _ [[maybe_unused]] = bmp.get_surface().map([&](will::dxgi::surface&& surf){
					dev.get_immediate_context().copy(tex, will::d3d::texture2d{std::move(surf)});
				});
				return std::move(tex);
			});
		if((bmp.get_bitmap_options() & D2D1_BITMAP_OPTIONS_CPU_READ) != 0)
			return bmp.scoped_map()
				.bind([&](will::d2d::bitmap::scoped_readonly_mapped_rect&& sr){
				return dev.create_texture2d(desc, will::interop::memory_data2d<::D3D11_SUBRESOURCE_DATA>(sr));
			});
		const auto dpi = bmp.get_dpi();
		return devcont.create_bitmap(bmp.get_pixel_size(), D2D1::BitmapProperties1(::D2D1_BITMAP_OPTIONS_CPU_READ | ::D2D1_BITMAP_OPTIONS_CANNOT_DRAW, bmp.get_pixel_format(), dpi.x, dpi.y, bmp.get_color_context())).bind([&](will::d2d::bitmap&& t){
			return t.copy_from(bmp).bind([&](){
				return conv(t, desc, dev, devcont);});});
	}
	template<typename RenderTarget>
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d2d::bitmap& bmp, const will::d3d::texture2d::description& desc, RenderTarget&& rt){return conv(bmp, desc, rt.get_d3d_device(), rt);}
};
template<typename ValueType, int Rank>
struct reinterpret_conversion_impl<will::d2d::bitmap, concurrency::graphics::texture_view<ValueType, Rank>>{
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::amp::graphics::texture_view<ValueType, Rank>& texv, const will::d2d::bitmap::property& prop, const will::d3d::device& dev, const will::d2d::device::context& devcont){
		return reinterpret_conversion_impl<will::d2d::bitmap, will::d3d::texture2d>::conv(will::amp::graphics::direct3d::get_texture(texv), prop, dev, devcont);
	}
	template<typename RenderTarget>
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::amp::graphics::texture_view<ValueType, Rank>& texv, const will::d2d::bitmap::property& prop, RenderTarget&& rt){return conv(texv, prop, rt.get_d3d_device(), rt);}
};
template<typename ValueType, int Rank>
struct reinterpret_conversion_impl<concurrency::graphics::texture<ValueType, Rank>, will::d2d::bitmap>{
	static will::expected<will::amp::graphics::texture<ValueType, Rank>, hresult_error> conv(const will::d2d::bitmap& bmp, const will::d3d::device& dev, const will::d2d::device::context& devcont, const will::amp::accelerator_view& accv){
		constexpr auto fn = _T(__FUNCTION__);
		return reinterpret_conversion_impl<will::d3d::texture2d, will::d2d::bitmap>::conv(bmp, will::d3d::texture2d::description{}.format(DXGI_FORMAT_R8G8B8A8_UNORM).bind_flags(D3D11_BIND_UNORDERED_ACCESS|D3D11_BIND_SHADER_RESOURCE), dev, devcont).bind([&](will::d3d::texture2d&& tex){
			return will::amp::graphics::direct3d::make_texture<will::amp::graphics::unorm_4, 2>(accv, tex)
			.emap([&](std::exception_ptr&& ptr){try{std::rethrow_exception(ptr);}catch(concurrency::runtime_exception& e){return make_unexpected<hresult_error>(fn, e.get_error_code());}});});
	}
	template<typename RenderTarget>
	static will::expected<will::amp::graphics::texture<ValueType, Rank>, hresult_error> conv(const will::d2d::bitmap& bmp, RenderTarget&& rt){return conv(bmp, rt.get_d3d_device(), rt, rt.get_accelerator_view());}
};

}

template<typename To, typename From, typename... Args>
inline expected<To, hresult_error> reinterpret_convert(From&& from, Args&&... args){return detail::reinterpret_conversion_impl<To, std::decay_t<From>>::conv(std::forward<From>(from), std::forward<Args>(args)...);}

namespace detail{

template<typename, typename>struct pixel_format_conversion_impl;
template<>
struct pixel_format_conversion_impl<will::d3d::texture2d, will::d3d::texture2d>{
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d3d::texture2d& tex, will::d3d::texture2d::description desc, pixel_format_converter& pfc, will::d3d::device& dev){
		return pfc.convert(tex, desc, dev);
	}
	template<typename RenderTarget>
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d3d::texture2d& tex, will::d3d::texture2d::description desc, pixel_format_converter& pfc, RenderTarget&& rt){return conv(tex, desc, pfc, rt.get_d3d_device());}
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d3d::texture2d& tex, will::d3d::texture2d::description desc, will::d3d::device& dev){
		return pixel_format_converter::create(dev).bind([&](pixel_format_converter&& pfc){return pfc.convert(tex, desc, dev);});
	}
	template<typename RenderTarget>
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d3d::texture2d& tex, will::d3d::texture2d::description desc, RenderTarget&& rt){return conv(tex, desc, rt.get_d3d_device());}
};
template<>
struct pixel_format_conversion_impl<will::d2d::bitmap, will::d3d::texture2d>{
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::d3d::texture2d& tex, const will::d2d::bitmap::property& prop, pixel_format_converter& pfc, will::d3d::device& dev, const will::d2d::device::context& devcont){
		auto desc = tex.get_desc();
		if(will::dxgi::format::same_family(desc.Format, prop.get().pixelFormat.format))
			return conv(tex, prop, dev, devcont);
		return pixel_format_conversion_impl<will::d3d::texture2d, will::d3d::texture2d>::conv(tex, will::d3d::texture2d::description{}.format(prop.get().pixelFormat.format), pfc, dev)
		          .bind([&](will::d3d::texture2d c){return conv(c, prop, dev, devcont);});
	}
	template<typename RenderTarget>
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::d3d::texture2d& tex, const will::d2d::bitmap::property& prop, pixel_format_converter& pfc, RenderTarget&& rt){return conv(tex, prop, pfc, rt.get_d3d_device(), rt);}
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::d3d::texture2d& tex, const will::d2d::bitmap::property& prop, will::d3d::device& dev, const will::d2d::device::context& devcont){
		auto desc = tex.get_desc();
		if(will::dxgi::format::same_family(desc.Format, prop.get().pixelFormat.format))
			return devcont.create_bitmap({desc.Width, desc.Height}, prop).bind([&](will::d2d::bitmap&& bmp){
				return bmp.get_surface().map([&](will::dxgi::surface&& surf){
					dev.get_immediate_context().copy(will::d3d::texture2d{std::move(surf)}, tex);
					return std::move(bmp);
				});
			});
		return pixel_format_converter::create(dev).bind([&](pixel_format_converter pfc){return
			pixel_format_conversion_impl<will::d3d::texture2d, will::d3d::texture2d>::conv(tex, will::d3d::texture2d::description{}.format(prop.get().pixelFormat.format), pfc, dev)
		          .bind([&](will::d3d::texture2d c){return conv(c, prop, dev, devcont);});});
	}
	template<typename RenderTarget>
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::d3d::texture2d& tex, const will::d2d::bitmap::property& prop, RenderTarget&& rt){return conv(tex, prop, rt.get_d3d_device(), rt);}
};
template<>
struct pixel_format_conversion_impl<will::d3d::texture2d, will::d2d::bitmap>{
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d2d::bitmap& bmp, will::d3d::texture2d::description desc, pixel_format_converter& pfc, will::d3d::device& dev, const will::d2d::device::context& devcont){
		{
			const auto p = bmp.get_pixel_size();
			if(desc.get().Width == 0)
				desc.width(p.width);
			if(desc.get().Height == 0)
				desc.height(p.height);
		}
		if(will::dxgi::format::same_family(desc.get().Format, bmp.get_pixel_format().format))
			return conv(bmp, desc, dev, devcont);
		return bmp.get_surface().bind([&](will::dxgi::surface&& surf){
			will::d3d::texture2d tex{surf};
			return pixel_format_conversion_impl<will::d3d::texture2d, will::d3d::texture2d>::conv(tex, desc, pfc, dev);});
	}
	template<typename RenderTarget>
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d2d::bitmap& bmp, const will::d3d::texture2d::description& desc, pixel_format_converter& pfc, RenderTarget&& rt){return conv(bmp, desc, pfc, rt.get_d3d_device(), rt);}
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d2d::bitmap& bmp, will::d3d::texture2d::description desc, will::d3d::device& dev, const will::d2d::device::context& devcont){
		{
			const auto p = bmp.get_pixel_size();
			if(desc.get().Width == 0)
				desc.width(p.width);
			if(desc.get().Height == 0)
				desc.height(p.height);
		}
		if(will::dxgi::format::same_family(desc.get().Format, bmp.get_pixel_format().format))
			return dev.create_texture2d(desc).map([&](will::d3d::texture2d&& tex){
				auto _ [[maybe_unused]] = bmp.get_surface().map([&](will::dxgi::surface&& surf){
					dev.get_immediate_context().copy(tex, will::d3d::texture2d{std::move(surf)});
				});
				return std::move(tex);
			});
		return pixel_format_converter::create(dev).bind([&](pixel_format_converter pfc){return
			conv(bmp, desc, pfc, dev, devcont);});
	}
	template<typename RenderTarget>
	static will::expected<will::d3d::texture2d, hresult_error> conv(const will::d2d::bitmap& bmp, const will::d3d::texture2d::description& desc, RenderTarget&& rt){return conv(bmp, desc, rt.get_d3d_device(), rt);}
};
template<typename ValueType, int Rank>
struct pixel_format_conversion_impl<will::d2d::bitmap, concurrency::graphics::texture_view<ValueType, Rank>>{
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::amp::graphics::texture_view<ValueType, Rank>& texv, const will::d2d::bitmap::property& prop, pixel_format_converter& pfc, will::d3d::device& dev, const will::d2d::device::context& devcont){
		return pixel_format_conversion_impl<will::d2d::bitmap, will::d3d::texture2d>::conv(will::amp::graphics::direct3d::get_texture(texv), prop, pfc, dev, devcont);
	}
	template<typename RenderTarget>
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::amp::graphics::texture_view<ValueType, Rank>& texv, const will::d2d::bitmap::property& prop, pixel_format_converter& pfc, RenderTarget&& rt){return conv(texv, prop, pfc, rt.get_d3d_device(), rt);}
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::amp::graphics::texture_view<ValueType, Rank>& texv, const will::d2d::bitmap::property& prop, will::d3d::device& dev, const will::d2d::device::context& devcont){
		return pixel_format_conversion_impl<will::d2d::bitmap, will::d3d::texture2d>::conv(will::amp::graphics::direct3d::get_texture(texv), prop, dev, devcont);
	}
	template<typename RenderTarget>
	static will::expected<will::d2d::bitmap, hresult_error> conv(const will::amp::graphics::texture_view<ValueType, Rank>& texv, const will::d2d::bitmap::property& prop, RenderTarget&& rt){return conv(texv, prop, rt.get_d3d_device(), rt);}
};
template<typename ValueType, int Rank>
struct pixel_format_conversion_impl<concurrency::graphics::texture<ValueType, Rank>, will::d2d::bitmap>{
	static will::expected<will::amp::graphics::texture<ValueType, Rank>, hresult_error> conv(const will::d2d::bitmap& bmp, pixel_format_converter& pfc, will::d3d::device& dev, const will::d2d::device::context& devcont, const will::amp::accelerator_view& accv){
		constexpr auto fn = _T(__FUNCTION__);
		return pixel_format_conversion_impl<will::d3d::texture2d, will::d2d::bitmap>::conv(bmp, will::d3d::texture2d::description{}.format(DXGI_FORMAT_R8G8B8A8_UNORM).bind_flags(D3D11_BIND_UNORDERED_ACCESS|D3D11_BIND_SHADER_RESOURCE), pfc, dev, devcont).bind([&](will::d3d::texture2d&& tex){
			return will::amp::graphics::direct3d::make_texture<will::amp::graphics::unorm_4, 2>(accv, tex)
			.emap([&](std::exception_ptr&& ptr){try{std::rethrow_exception(ptr);}catch(concurrency::runtime_exception& e){return make_unexpected<hresult_error>(fn, e.get_error_code());}});});
	}
	template<typename RenderTarget>
	static will::expected<will::amp::graphics::texture<ValueType, Rank>, hresult_error> conv(const will::d2d::bitmap& bmp, pixel_format_converter& pfc, RenderTarget&& rt){return conv(bmp, rt.get_d3d_device(), rt, pfc, rt.get_accelerator_view());}
	static will::expected<will::amp::graphics::texture<ValueType, Rank>, hresult_error> conv(const will::d2d::bitmap& bmp, will::d3d::device& dev, const will::d2d::device::context& devcont, const will::amp::accelerator_view& accv){
		constexpr auto fn = _T(__FUNCTION__);
		return pixel_format_conversion_impl<will::d3d::texture2d, will::d2d::bitmap>::conv(bmp, will::d3d::texture2d::description{}.format(DXGI_FORMAT_R8G8B8A8_UNORM).bind_flags(D3D11_BIND_UNORDERED_ACCESS|D3D11_BIND_SHADER_RESOURCE), dev, devcont).bind([&](will::d3d::texture2d&& tex){
			return will::amp::graphics::direct3d::make_texture<will::amp::graphics::unorm_4, 2>(accv, tex)
			.emap([&](std::exception_ptr&& ptr){try{std::rethrow_exception(ptr);}catch(concurrency::runtime_exception& e){return make_unexpected<hresult_error>(fn, e.get_error_code());}});});
	}
	template<typename RenderTarget>
	static will::expected<will::amp::graphics::texture<ValueType, Rank>, hresult_error> conv(const will::d2d::bitmap& bmp, RenderTarget&& rt){return conv(bmp, rt.get_d3d_device(), rt, rt.get_accelerator_view());}
};

}

}

class hwnd_render_target:protected d3d::device, protected dxgi::swap_chain, public d2d::device::context, public dwrite, public dcomposition::desktop_device{
	static expected<dxgi::swap_chain, hresult_error> create_swap_chain(HWND hwnd, d3d::device& dev, DXGI_SWAP_CHAIN_DESC1 desc = will::dxgi::swap_chain::description{}.format(DXGI_FORMAT_B8G8R8A8_UNORM).scaling(DXGI_SCALING_STRETCH).swap_effect(DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL).alpha_mode(DXGI_ALPHA_MODE_PREMULTIPLIED)){
		return [](HWND hwnd)->expected<RECT, hresult_error>{RECT r;if(::GetClientRect(hwnd, &r) != 0)return r;return make_unexpected<hresult_error>(_T(__FUNCTION__), ::GetLastError());}(hwnd).map([&](RECT&& r){
			if(desc.Width == 0u)
				desc.Width = r.right;
			if(desc.Height == 0u)
				desc.Height = r.bottom;
		}).bind([&]{
			return dxgi::device::create(dev).bind([&](dxgi::device&& dev2){
				return dev2.get_factory().bind([&](dxgi&& dxgi_fac){
					return dxgi_fac.create_swap_chain(dev, desc).bind([&](dxgi::swap_chain&& sc){
						return dev2.set_maximum_frame_latency(1).map([&]()->dxgi::swap_chain{
							return std::move(sc);
						});
					});
				});
			});
		});
	}
	HWND hwnd;
	HRESULT status;
	amp::accelerator_view av;
	dcomposition::visual v;
	dcomposition::target t;
	hwnd_render_target(d3d::device&& dev, dxgi::swap_chain&& sc, d2d::device::context&& devcon, dwrite&& dw, dcomposition::desktop_device&& dcompo, HWND hw, amp::accelerator_view&& accv, dcomposition::visual&& v, dcomposition::target&& t) : device{std::move(dev)}, swap_chain{std::move(sc)}, d2d::device::context{std::move(devcon)}, dwrite{std::move(dw)}, dcomposition::desktop_device{std::move(dcompo)}, hwnd(std::move(hw)), status(0ul), av{std::move(accv)}, v{std::move(v)}, t{std::move(t)}{}
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const DXGI_SWAP_CHAIN_DESC1& desc, const D2D1_CREATION_PROPERTIES* prop, DWRITE_FACTORY_TYPE type, will::amp::queuing_mode qm){
		return d3d::device::create(flag).bind([&](d3d::device&& d3d_dev){
			return create_swap_chain(hwnd, d3d_dev, desc).bind([&](dxgi::swap_chain&& sc){
				return sc.get_buffer().bind([&](dxgi::surface&& surf){
					return d2d::device::context::create(surf, prop).bind([&](d2d::device::context&& devcon){
						return dwrite::create_factory(type).bind([&](dwrite&& dw){
							return dcomposition::desktop_device::create(d3d_dev).bind([&](dcomposition::desktop_device&& dcompo){
								return dcompo.create_visual().bind([&](dcomposition::visual&& visual){
									return std::move(visual).set_content(sc).bind([&](dcomposition::visual&& visual){
										return dcompo.create_target(hwnd, true).bind([&](dcomposition::target&& target){
											return std::move(target).set_root(visual).bind([&](dcomposition::target&& target){
												return dcompo.commit().bind([&]{
													return amp::direct3d::create_accelerator_view(d3d_dev, qm)
														.emap([](std::exception_ptr&& ptr){try{std::rethrow_exception(ptr);}catch(concurrency::runtime_exception& e){return make_unexpected<hresult_error>(_T("will::hwnd_render_target::create"), e.get_error_code());}}).map([&](amp::accelerator_view&& av){
														return hwnd_render_target{std::move(d3d_dev), std::move(sc), std::move(devcon), std::move(dw), std::move(dcompo), hwnd, std::move(av), std::move(visual), std::move(target)};
													});
												});
											});
										});
									});
								});
							});
						});
					});
				});
			});
		});
	}
public:
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd, const D2D1_CREATION_PROPERTIES& prop){return create(hwnd, D3D11_CREATE_DEVICE_BGRA_SUPPORT, will::dxgi::swap_chain::description{}.format(DXGI_FORMAT_B8G8R8A8_UNORM), &prop, DWRITE_FACTORY_TYPE_SHARED, will::amp::queuing_mode_automatic);}
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd,
#ifdef _DEBUG
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_WARNING, D2D1_DEVICE_CONTEXT_OPTIONS_NONE}
#else
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS}
#endif
	){return create(hwnd, flag, will::dxgi::swap_chain::description{}.format(DXGI_FORMAT_B8G8R8A8_UNORM).scaling(DXGI_SCALING_STRETCH).swap_effect(DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL).alpha_mode(DXGI_ALPHA_MODE_PREMULTIPLIED), &prop, DWRITE_FACTORY_TYPE_SHARED, will::amp::queuing_mode_automatic);}
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const DXGI_SWAP_CHAIN_DESC1& desc,
#ifdef _DEBUG
		const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_WARNING, D2D1_DEVICE_CONTEXT_OPTIONS_NONE}
#else
		const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS}
#endif
	){return create(hwnd, flag, desc, &prop, DWRITE_FACTORY_TYPE_SHARED, will::amp::queuing_mode_automatic);}
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const DXGI_SWAP_CHAIN_DESC1& desc, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type){return create(hwnd, flag, desc, &prop, type, will::amp::queuing_mode_automatic);}
	static expected<hwnd_render_target, hresult_error> create(HWND hwnd, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const DXGI_SWAP_CHAIN_DESC1& desc, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type, amp::queuing_mode qm){return create(hwnd, flag, desc, &prop, type, qm);}
	template<typename Window, typename... Args, std::enable_if_t<!std::is_same<std::decay_t<Window>, HWND>::value>* = nullptr>
	static expected<hwnd_render_target, hresult_error> create(Window&& w, Args&&... args){return create(w.get(), std::forward<Args>(args)...);}
	hwnd_render_target(HWND hwnd, const D2D1_CREATION_PROPERTIES& prop):hwnd_render_target{+create(hwnd, prop)}{}
	explicit hwnd_render_target(HWND hwnd,
#ifdef _DEBUG
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_WARNING, D2D1_DEVICE_CONTEXT_OPTIONS_NONE}
#else
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS}
#endif
	):hwnd_render_target{+create(hwnd, flag, prop)}{}
	explicit hwnd_render_target(HWND hwnd, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const DXGI_SWAP_CHAIN_DESC1& desc,
#ifdef _DEBUG
		const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_WARNING, D2D1_DEVICE_CONTEXT_OPTIONS_NONE}
#else
		const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS}
#endif
	):hwnd_render_target{+create(hwnd, flag, desc, prop)}{}
	hwnd_render_target(HWND hwnd, D3D11_CREATE_DEVICE_FLAG flag, const DXGI_SWAP_CHAIN_DESC1& desc, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type):hwnd_render_target{+create(hwnd, flag, desc, prop, type)}{}
	hwnd_render_target(HWND hwnd, D3D11_CREATE_DEVICE_FLAG flag, const DXGI_SWAP_CHAIN_DESC1& desc, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type, amp::queuing_mode qm):hwnd_render_target{+create(hwnd, flag, desc, prop, type, qm)}{}
	template<typename Window, typename... Args, std::enable_if_t<!std::is_same<std::decay_t<Window>, HWND>::value>* = nullptr>
	explicit hwnd_render_target(Window&& w, Args&&... args):hwnd_render_target(w.get(), std::forward<Args>(args)...){}
	using d2d::device::context::get;
	using d2d::device::context::operator->;
	using d2d::device::context::operator bool;
	template<typename F>
	expected<void, hresult_error> draw(F&& f){
		using namespace std::literals::chrono_literals;
		DXGI_PRESENT_PARAMETERS para = {};
		if((status & DXGI_STATUS_OCCLUDED) != 0){
			status = (*static_cast<dxgi::swap_chain*>(this))->Present1(1, DXGI_PRESENT_TEST, &para);
			return make_unexpected<hresult_error>(_T(__FUNCTION__), status);
		}
		return d2d::device::context::draw(std::forward<F>(f)).bind([&]()->expected<void, hresult_error>{
			status = dxgi::swap_chain::get()->Present1(1, 0, &para);
			if(SUCCEEDED(status))
				return {};
			return make_unexpected<hresult_error>(_T("will::hwnd_render_target::draw"), status);
		});
	}
	d3d::device& get_d3d_device(){return *this;}
	const d3d::device& get_d3d_device()const{return *this;}
	dwrite& get_dwrite_factory(){return *this;}
	const dwrite& get_dwrite_factory()const{return *this;}
	amp::accelerator_view& get_accelerator_view(){return av;}
	const amp::accelerator_view& get_accelerator_view()const{return av;}
	expected<hwnd_render_target, hresult_error> recreate_render_target(){
		static_cast<d2d::device::context&>(*this).~context();
		static_cast<dxgi::swap_chain&>(*this).~swap_chain();
		return create_swap_chain(hwnd, this->get_d3d_device()).bind([&](dxgi::swap_chain&& sc){
			return get_buffer().map([&](dxgi::surface&& sf){
				new (static_cast<dxgi::swap_chain*>(this)) dxgi::swap_chain(std::move(sc));
				new (static_cast<d2d::device::context*>(this)) d2d::device::context(std::move(sf));
				return std::move(*this);
			});
		});
	}
	template<typename To, typename From, typename... Args>
	expected<To, hresult_error> reinterpret_convert(From&& from, Args&&... args)const{return interop::reinterpret_convert<To>(std::forward<From>(from), std::forward<Args>(args)..., *this);}
	template<typename To, typename From, typename... Args>
	expected<To, hresult_error> pixel_format_convert(From&& from, Args&&... args){return interop::detail::pixel_format_conversion_impl<To, std::decay_t<From>>::conv(std::forward<From>(from), std::forward<Args>(args)..., *this);}
	auto wm_size_resize_backbuffer(const ::DXGI_SWAP_CHAIN_DESC1& desc = will::dxgi::swap_chain::description{}.format(::DXGI_FORMAT_B8G8R8A8_UNORM).alpha_mode(::DXGI_ALPHA_MODE_PREMULTIPLIED)){
		return [&, this, buffer_count = desc.BufferCount, format = desc.Format, alpha = static_cast<::D2D1_ALPHA_MODE>(desc.AlphaMode), flags = desc.Flags](auto&&, WPARAM wparam, LPARAM){
			if(wparam == SIZE_MINIMIZED || wparam == SIZE_MAXSHOW || wparam == SIZE_MAXHIDE)
				return 0;
			(*this).unset_target();
			RECT r;
			if(::GetClientRect(hwnd, &r) == 0)
				throw winapi_last_error_exception(_T(__FUNCTION__));
			(*this).resize_buffers(buffer_count, r.right, r.bottom, format, flags).bind([&](){return
			(*this).get_buffer().bind([&](will::dxgi::surface&& surf){return
			(*this).create_bitmap(surf, will::d2d::bitmap::property{}.format(format).alpha_mode(alpha).option(::D2D1_BITMAP_OPTIONS_TARGET | ::D2D1_BITMAP_OPTIONS_CANNOT_DRAW)).map([&](will::d2d::bitmap&& bmp){
			(*this).set_target(bmp);});});})
			.value();
			return 0;
		};
	}
};
class gdi_compatible_render_target:protected d3d::device, public dxgi::surface, public d2d::device::context, public dwrite{
	static expected<dxgi::surface, hresult_error> create_surface(unsigned int w, unsigned int h, d3d::device& dev){
		return dev.create_texture2d(will::d3d::texture2d::description{}.width(w).height(h).mip_levels(1).array_size(1).format(DXGI_FORMAT_B8G8R8A8_UNORM).sample_count(1).sample_quality(0).bind_flags(D3D11_BIND_RENDER_TARGET).misc_flags(D3D11_RESOURCE_MISC_GDI_COMPATIBLE)).bind([](d3d::texture2d&& tex){return dxgi::surface::create(std::move(tex));});
	}
	gdi_compatible_render_target(d3d::device&& dev, dxgi::surface&& surf, d2d::device::context&& devcont, dwrite&& dw, amp::accelerator_view&& accv):device{std::move(dev)}, surface{std::move(surf)}, d2d::device::context{std::move(devcont)}, dwrite{std::move(dw)}, av{std::move(accv)}{}
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<unsigned int>& wh, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const D2D1_CREATION_PROPERTIES* prop, DWRITE_FACTORY_TYPE type, amp::queuing_mode qm){
		return d3d::device::create(flag).bind([&](d3d::device&& d3d_dev){
			return create_surface(wh.w, wh.h, d3d_dev).bind([&](dxgi::surface&& surf){
				return d2d::device::context::create(surf, prop).bind([&](d2d::device::context&& devcon){
					return dwrite::create_factory(type).bind([&](dwrite&& dw){
						return amp::direct3d::create_accelerator_view(d3d_dev, qm)
							.emap([](std::exception_ptr&& ptr){try{std::rethrow_exception(ptr);}catch(concurrency::runtime_exception& e){return make_unexpected<hresult_error>(_T("will::gdi_compatible_render_target::create"), e.get_error_code());}}).map([&](amp::accelerator_view&& av){
							return gdi_compatible_render_target{std::move(d3d_dev), std::move(surf), std::move(devcon), std::move(dw), std::move(av)};
						});
					});
				});
			});
		});
	}
	amp::accelerator_view av;
public:
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<unsigned int>& wh, const D2D1_CREATION_PROPERTIES& prop){return create(wh, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &prop, DWRITE_FACTORY_TYPE_SHARED, amp::queuing_mode_automatic);}
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<unsigned int>& wh,
#ifdef _DEBUG
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_WARNING, D2D1_DEVICE_CONTEXT_OPTIONS_NONE}
#else
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS}
#endif
	){return create(wh, flag, &prop, DWRITE_FACTORY_TYPE_SHARED, amp::queuing_mode_automatic);}
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<unsigned int>& wh, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type){return create(wh, flag, &prop, type, amp::queuing_mode_automatic);}
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<unsigned int>& wh, std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type, amp::queuing_mode qm){return create(wh, flag, &prop, type, qm);}
	template<typename T, typename... Args, std::enable_if_t<!std::is_same<T, unsigned int>::value, std::nullptr_t> = nullptr>
	static expected<gdi_compatible_render_target, hresult_error> create(const will::two_dim::wh<T>& wh, Args&&... args){return create(will::two_dim::attribute(wh).convert<will::two_dim::wh<unsigned int>>(), std::forward<Args>(args)...);}
	gdi_compatible_render_target(const will::two_dim::wh<unsigned int>& wh, const D2D1_CREATION_PROPERTIES& prop) : gdi_compatible_render_target{+create(wh, D3D11_CREATE_DEVICE_BGRA_SUPPORT, prop)}{}
	explicit gdi_compatible_render_target(const will::two_dim::wh<unsigned int>& wh,
#ifdef _DEBUG
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_WARNING, D2D1_DEVICE_CONTEXT_OPTIONS_NONE}
#else
		std::underlying_type_t<D3D11_CREATE_DEVICE_FLAG> flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT, const D2D1_CREATION_PROPERTIES& prop = {D2D1_THREADING_MODE_MULTI_THREADED, D2D1_DEBUG_LEVEL_NONE, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS}
#endif
	) : gdi_compatible_render_target{+create(wh, flag, prop)}{}
	gdi_compatible_render_target(const will::two_dim::wh<unsigned int>& wh, D3D11_CREATE_DEVICE_FLAG flag, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type) : gdi_compatible_render_target{+create(wh, flag, prop, type)}{}
	gdi_compatible_render_target(const will::two_dim::wh<unsigned int>& wh, D3D11_CREATE_DEVICE_FLAG flag, const D2D1_CREATION_PROPERTIES& prop, DWRITE_FACTORY_TYPE type, amp::queuing_mode qm) : gdi_compatible_render_target{+create(wh, flag, prop, type, qm)}{}
	using d2d::device::context::get;
	using d2d::device::context::operator->;
	using d2d::device::context::operator bool;
	using d2d::device::context::draw;
	using d2d::device::context::get_device;
	template<typename F>
	expected<d2d::bitmap, hresult_error> prerender(const D2D1_SIZE_F& desired_size, F&& f){
		return create_compatible_render_target(desired_size, D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE).bind([&](d2d::device::bitmap_render_target&& rt){
			return rt.draw(std::forward<F>(f)).bind([&]{
				return rt.get_bitmap();
			});
		});
	}
	template<typename F>
	expected<d2d::bitmap, hresult_error> prerender(const D2D1_SIZE_U& desired_pixel_size, F&& f){
		return create_compatible_render_target(desired_pixel_size, D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE).bind([&](d2d::device::bitmap_render_target&& rt){
			return rt.draw(std::forward<F>(f)).bind([&]{
				return rt.get_bitmap();
			});
		});
	}
	template<typename F>
	expected<d2d::bitmap, hresult_error> prerender(F&& f){
		return create_compatible_render_target(D2D1::PixelFormat(), D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_GDI_COMPATIBLE).bind([&](d2d::device::bitmap_render_target&& rt){
			return rt.draw(std::forward<F>(f)).bind([&]{
				return rt.get_bitmap();
			});
		});
	}
	dxgi::surface& get_dxgi_surface(){return *this;}
	const dxgi::surface& get_dxgi_surface()const{return *this;}
	d3d::device& get_d3d_device(){return *this;}
	const d3d::device& get_d3d_device()const{return *this;}
	dwrite& get_dwrite_factory(){return *this;}
	const dwrite& get_dwrite_factory()const{return *this;}
	amp::accelerator_view& get_accelerator_view(){return av;}
	const amp::accelerator_view& get_accelerator_view()const{return av;}
	template<typename To, typename From, typename... Args>
	expected<To, hresult_error> reinterpret_convert(From&& from, Args&&... args)const{return interop::reinterpret_convert<To>(std::forward<From>(from), std::forward<Args>(args)..., *this);}
	template<typename To, typename From, typename... Args>
	expected<To, hresult_error> pixel_format_convert(From&& from, Args&&... args){return interop::detail::pixel_format_conversion_impl<To, std::decay_t<From>>::conv(std::forward<From>(from), std::forward<Args>(args)...);}
};
}