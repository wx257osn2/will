//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_windows.hpp"
#include"_2dim.hpp"
#include<vector>
namespace will{
class transform{
	XFORM matrix = XFORM{1.f, 0.f, 0.f, 1.f};
public:
	constexpr transform() = default;
	transform(XFORM x):matrix(std::move(x)){}
	transform& operator=(XFORM x){std::swap(matrix, std::move(x));return *this;}
	expected<transform, winapi_last_error> mul(const transform& lhs, const transform& rhs){
		transform val;
		if(CombineTransform(&val.matrix, &lhs.matrix, &rhs.matrix))
			return val;
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	}
	transform& operator*=(const transform& rhs){
		return *this = +mul(*this, rhs);
	}
	XFORM& get(){return matrix;}
	const XFORM& get()const{return matrix;}
	const XFORM& cget()const{return matrix;}
	explicit operator XFORM&(){return matrix;}
	explicit operator const XFORM&()const{return matrix;}
	friend transform operator*(transform lhs, const transform& rhs){
		return lhs *= rhs;
	}
	transform& operator|=(const transform& rhs){
		return *this *= rhs;
	}
	friend transform operator|(transform lhs, const transform& rhs){
		return lhs *= rhs;
	}
	static transform translate(float x, float y){
		return XFORM{1.f, 0.f, 0.f, 1.f, x, y};
	}
	static transform scale(float x, float y){
		return XFORM{x, 0.f, 0.f, y};
	}
	static transform rotate(float theta, float x = 0.f, float y = 0.f){
		const float cos = std::cos(theta);
		const float sin = std::sin(theta);
		return XFORM{cos, sin, -sin, cos, (1.f - cos) * x + sin * y, (1.f - cos) * y - sin * x};
	}
	static transform skew(float x, float y){
		return XFORM{1.f, y, x, 1.f};
	}
	static transform shear(float x, float y){
		return XFORM{x, 1.f, 1.f, y};
	}
	static transform inverse(bool x, bool y){
		return XFORM{x?-1.f:1.f, 0, 0, y?-1.f:1.f};
	}
};
class region{
public:
	class data{
		std::vector<char> rgn_data;
		data(std::vector<char> d):rgn_data{std::move(d)}{}
	public:
		data(HRGN rgn):data(+create(rgn)){}
		data(const region& rgn):data(rgn.get()){}
		std::vector<char>::size_type size()const{return rgn_data.size();}
		RGNDATA* get(){return reinterpret_cast<RGNDATA*>(rgn_data.data());}
		const RGNDATA* get()const{return reinterpret_cast<const RGNDATA*>(rgn_data.data());}
		static expected<data, winapi_last_error> create(HRGN rgn){
			const auto size = ::GetRegionData(rgn, 0, nullptr);
			std::vector<char> rgn_data(size);
			if(::GetRegionData(rgn, static_cast<DWORD>(rgn_data.size()), reinterpret_cast<LPRGNDATA>(rgn_data.data())) == size)
				return std::move(rgn_data);
			return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		}
	};
	expected<data, winapi_last_error> create_data(){return data::create(get());}
private:
	HRGN in;
	static expected<region, winapi_last_error> conversion(const data& rgn_data, const XFORM& trans){return detail::checked_return(_T(__FUNCTION__), ExtCreateRegion(&trans, static_cast<DWORD>(rgn_data.size()), rgn_data.get()), nullptr).map([](HRGN r){return region{std::move(r)};});}
	static expected<region, winapi_last_error> conversion(const region& rgn, const XFORM& trans){
		return conversion(data(rgn), trans);
	}
	static expected<void, winapi_last_error> combine(region& lhs, const region& rhs, int mode){
		const auto ret = CombineRgn(lhs.in, lhs.in, rhs.in, mode);
		if(ret != ERROR)
			return {};
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	}
public:
	static expected<region, winapi_last_error> create_empty_region(){return detail::checked_return(_T(__FUNCTION__), CreateRectRgn(0, 0, 1, 1), nullptr).map([](HRGN r){return region{std::move(r)};});}
	static expected<region, winapi_last_error> copy(const region& r){
		create_empty_region().bind([&](region&& t)->expected<region, winapi_last_error>{
			const auto ret = CombineRgn(t.in, r.in, nullptr, RGN_COPY);
			if(ret != ERROR)
				return t;
			return make_unexpected<winapi_last_error>(_T("will::region::copy"));
		});
	}
	static expected<region, winapi_last_error> combine(region&& lhs, const region& rhs, int mode){
		const auto ret = CombineRgn(lhs.in, lhs.in, rhs.in, mode);
		if(ret != ERROR)
			return std::move(lhs);
		return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
	}
	static expected<region, winapi_last_error> combine(const region& lhs, const region& rhs, int mode){
		return create_empty_region().bind([&](region&& t)->expected<region, winapi_last_error>{
			const auto ret = CombineRgn(t.in, lhs.in, rhs.in, mode);
			if(ret != ERROR)
				return std::move(t);
			return make_unexpected<winapi_last_error>(_T("will::region::combine"));
		});
	}
	explicit region():region(+create_empty_region()){}
	region(HRGN&& t)noexcept:in(std::move(t)){}
	region(const region& t):region(+copy(t)){}
	region(region&& t)noexcept:in(nullptr){std::swap(in, t.in);}
	~region(){+reset();}
	void detach()noexcept{in = nullptr;}
	HRGN release()noexcept{HRGN ret = in;detach();return ret;}
	expected<void, winapi_last_error> reset(HRGN r = nullptr){
		if(in)
			if(DeleteObject(in) == 0)
				return make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		in = r;
		return {};
	}
	region& operator=(const region& rhs){
		return *this = std::move(+copy(rhs));
	}
	region& operator=(region&& rhs){
		std::swap(in, rhs.in);
		return *this;
	}
	region& operator|=(const region& rhs){
		+combine(*this, rhs, RGN_OR);
		return *this;
	}
	region& operator&=(const region& rhs){
		+combine(*this, rhs, RGN_AND);
		return *this;
	}
	region& operator^=(const region& rhs){
		+combine(*this, rhs, RGN_XOR);
		return *this;
	}
	region& operator-=(const region& rhs){
		+combine(*this, rhs, RGN_DIFF);
		return *this;
	}
	template<typename XFORMConvertible>
	region& operator*=(const XFORMConvertible& rhs){
		return *this = std::move(+conversion(*this, static_cast<const XFORM&>(rhs)));
	}
	friend region operator|(region lhs, const region& rhs){
		return std::move(lhs |= rhs);
	}
	friend region operator&(region lhs, const region& rhs){
		return std::move(lhs &= rhs);
	}
	friend region operator^(region lhs, const region& rhs){
		return std::move(lhs ^= rhs);
	}
	friend region operator-(region lhs, const region& rhs){
		return std::move(lhs -= rhs);
	}
	template<typename XFORMConvertible>
	friend region operator*(const region& lhs, const XFORMConvertible& rhs){
		return +conversion(lhs, static_cast<const XFORM&>(rhs));
	}
	HRGN get()const{return in;}
	explicit operator HRGN()const{return in;}
	expected<void, winapi_last_error> operator()(HWND hwnd, bool redraw = true)     &&{auto ret = ::SetWindowRgn(hwnd, in, redraw);if(ret == 0)return make_unexpected<winapi_last_error>(_T(__FUNCTION__));detach();return {};}
	expected<void, winapi_last_error> operator()(HWND hwnd, bool redraw = true)const &{copy(*this).bind([&](region&& r){return r(hwnd, redraw);});}
	template<typename HWNDConvertible>
	expected<int, winapi_last_error> operator()(HWNDConvertible&& w, bool redraw = true)const&{return (*this)(static_cast<HWND>(w), redraw);}
	template<typename HWNDConvertible>
	expected<int, winapi_last_error> operator()(HWNDConvertible&& w, bool redraw = true)&&{return (*this)(static_cast<HWND>(w), redraw);}
	static expected<region, winapi_last_error> rect(const two_dim::xyxy<int>& rect){return detail::checked_return(_T(__FUNCTION__), CreateRectRgn(rect._1.x, rect._1.y, rect._2.x, rect._2.y), nullptr).map([](HRGN r){return region{std::move(r)};});}
	static expected<region, winapi_last_error> ellipse(const two_dim::xyxy<int>& rect){return detail::checked_return(_T(__FUNCTION__), CreateEllipticRgn(rect._1.x, rect._1.y, rect._2.x, rect._2.y), nullptr).map([](HRGN r){return region{std::move(r)};});}
	static expected<region, winapi_last_error> circle(const two_dim::xy<int>& p, unsigned int radius){return detail::checked_return(_T(__FUNCTION__), CreateEllipticRgn(p.x-radius, p.y-radius, p.x+radius, p.y+radius), nullptr).map([](HRGN r){return region{std::move(r)};});}
	static expected<region, winapi_last_error> round(const two_dim::xyxy<int>& rect, const two_dim::wh<int> ellipse){return detail::checked_return(_T(__FUNCTION__), CreateRoundRectRgn(rect._1.x, rect._1.y, rect._2.x, rect._2.y, ellipse.w, ellipse.h), nullptr).map([](HRGN r){return region{std::move(r)};});}
	static expected<region, winapi_last_error> polygon(const std::vector<POINT>& points, int fill_mode){return detail::checked_return(_T(__FUNCTION__), CreatePolygonRgn(points.data(), static_cast<int>(points.size()), fill_mode), nullptr).map([](HRGN r){return region{std::move(r)};});}
};
}