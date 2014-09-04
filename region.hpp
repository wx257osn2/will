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
	transform& operator*=(const transform& rhs){
		CombineTransform(&matrix, &matrix, &rhs.matrix);
		return *this;
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
	public:
		data(HRGN rgn):rgn_data(::GetRegionData(rgn, 0, nullptr)){::GetRegionData(rgn, rgn_data.size(), reinterpret_cast<LPRGNDATA>(rgn_data.data()));}
		data(const region& rgn):data(rgn.get()){}
		std::vector<char>::size_type size()const{return rgn_data.size();}
		RGNDATA* get(){return reinterpret_cast<RGNDATA*>(rgn_data.data());}
		const RGNDATA* get()const{return reinterpret_cast<const RGNDATA*>(rgn_data.data());}
	};
private:
	HRGN in = CreateRectRgn(0,0,1,1);
	static region conversion(const data& rgn_data, const XFORM& trans){
		return ExtCreateRegion(&trans, rgn_data.size(), rgn_data.get());
	}
	static region conversion(const region& rgn, const XFORM& trans){
		return conversion(data(rgn), trans);
	}
public:
	region() = default;
	region(HRGN t):in(std::move(t)){}
	region(const region& t){CombineRgn(in, t.in, nullptr, RGN_COPY);}
	region(region&& t):in(nullptr){std::swap(in, t.in);}
	~region(){if(in)DeleteObject(in);}
	region& operator=(const region& rhs){
		CombineRgn(in, rhs.in, nullptr, RGN_COPY);
		return *this;
	}
	region& operator=(region&& rhs){
		std::swap(in, rhs.in);
		return *this;
	}
	region& operator|=(const region& rhs){
		CombineRgn(in, in, rhs.in, RGN_OR);
		return *this;
	}
	region& operator&=(const region& rhs){
		CombineRgn(in, in, rhs.in, RGN_AND);
		return *this;
	}
	region& operator^=(const region& rhs){
		CombineRgn(in, in, rhs.in, RGN_XOR);
		return *this;
	}
	region& operator-=(const region& rhs){
		CombineRgn(in, in, rhs.in, RGN_DIFF);
		return *this;
	}
	template<typename XFORMConvertible>
	region& operator*=(const XFORMConvertible& rhs){
		return *this = std::move(conversion(*this, static_cast<const XFORM&>(rhs)));
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
		return conversion(lhs, static_cast<const XFORM&>(rhs));
	}
	HRGN get()const{return in;}
	explicit operator HRGN()const{return in;}
	int operator()(HWND hwnd, bool redraw = true)     &&{auto ret = ::SetWindowRgn(hwnd, in, redraw); in = nullptr; return ret;}
	int operator()(HWND hwnd, bool redraw = true)const &{HRGN t=::CreateRectRgn(0,0,1,1);::CombineRgn(t, in, nullptr, RGN_COPY);return ::SetWindowRgn(hwnd, t, redraw);}
	template<typename HWNDConvertible>
	int operator()(HWNDConvertible&& w, bool redraw = true)const{return (*this)(static_cast<HWND>(w), redraw);}
	static region rect(const two_dim::xyxy<int>& rect){return ::CreateRectRgn(rect._1.x, rect._1.y, rect._2.x, rect._2.y);}
	static region ellipse(const two_dim::xyxy<int>& rect){return ::CreateEllipticRgn(rect._1.x, rect._1.y, rect._2.x, rect._2.y);}
	static region circle(const two_dim::xy<int>& p, unsigned int radius){return ::CreateEllipticRgn(p.x-radius, p.y-radius, p.x+radius, p.y+radius);}
	static region round(const two_dim::xyxy<int>& rect, const two_dim::wh<int> ellipse){return ::CreateRoundRectRgn(rect._1.x, rect._1.y, rect._2.x, rect._2.y, ellipse.w, ellipse.h);}
	static region polygon(const std::vector<POINT>& points, int fill_mode){return ::CreatePolygonRgn(points.data(), points.size(), fill_mode);}
};
}