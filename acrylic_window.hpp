//Copyright (C) 2014-2020 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_windows.hpp"

namespace{

enum WINDOWCOMPOSITIONATTRIB{
	WCA_UNDEFINED,
	WCA_NCRENDERING_ENABLED,
	WCA_NCRENDERING_POLICY,
	WCA_TRANSITIONS_FORCEDISABLED,
	WCA_ALLOW_NCPAINT,
	WCA_CAPTION_BUTTON_BOUNDS,
	WCA_NONCLIENT_RTL_LAYOUT,
	WCA_FORCE_ICONIC_REPRESENTATION,
	WCA_EXTENDED_FRAME_BOUNDS,
	WCA_HAS_ICONIC_BITMAP,
	WCA_THEME_ATTRIBUTES,
	WCA_NCRENDERING_EXILED,
	WCA_NCADORNMENTINFO,
	WCA_EXCLUDED_FROM_LIVEPREVIEW,
	WCA_VIDEO_OVERLAY_ACTIVE,
	WCA_FORCE_ACTIVEWINDOW_APPEARANCE,
	WCA_DISALLOW_PEEK,
	WCA_CLOAK,
	WCA_CLOAKED,
	WCA_ACCENT_POLICY,
	WCA_FREEZE_REPRESENTATION,
	WCA_EVER_UNCLOAKED,
	WCA_VISUAL_OWNER,
	WCA_HOLOGRAPHIC,
	WCA_EXCLUDED_FROM_DDA,
	WCA_PASSIVEUPDATEMODE,
	WCA_USEDARKMODECOLORS,
	WCA_LAST
};
struct WINDOWCOMPOSITIONATTRIBDATA{
	WINDOWCOMPOSITIONATTRIB Attrib;
	PVOID pvData;
	SIZE_T cbData;
};
enum ACCENT_STATE{
	ACCENT_DISABLED,
	ACCENT_ENABLE_GRADIENT,
	ACCENT_ENABLE_TRANSPARENTGRADIENT,
	ACCENT_ENABLE_BLURBEHIND,
	ACCENT_ENABLE_ACRYLICBLURBEHIND,
	ACCENT_ENABLE_HOSTBACKDROP,
	ACCENT_INVALID_STATE
};
struct ACCENT_POLICY{
	ACCENT_STATE AccentState;
	DWORD AccentFlags;
	DWORD GradientColor;
	DWORD AnimationId;
};

}

namespace will{

static inline expected<::WINDOWCOMPOSITIONATTRIBDATA, winapi_last_error> set_window_composition_attribute(HWND hwnd, ::WINDOWCOMPOSITIONATTRIBDATA data){
	return will::load_library(_T("user32.dll")).bind([&](will::module_handle&& user32){return
	user32.get_proc_address<BOOL(WINAPI)(HWND, ::WINDOWCOMPOSITIONATTRIBDATA*)>("SetWindowCompositionAttribute").bind([&](BOOL(WINAPI*SetWindowCompositionAttribute)(HWND, ::WINDOWCOMPOSITIONATTRIBDATA*))->expected<void, winapi_last_error>{
		if(SetWindowCompositionAttribute(hwnd, &data) != TRUE)
			return will::make_unexpected<winapi_last_error>(_T(__FUNCTION__));
		return data;
	});});
}

template<typename Hwnd>
static inline expected<::WINDOWCOMPOSITIONATTRIBDATA, winapi_last_error> set_window_composition_attribute(Hwnd&& hwnd, const ::WINDOWCOMPOSITIONATTRIBDATA& data){
	return set_window_composition_attribute(std::forward<Hwnd>(hwnd).get(), data);
}

static inline expected<void, winapi_last_error> acrylic_window(HWND hwnd, ::ACCENT_STATE as = ACCENT_ENABLE_BLURBEHIND){
	::ACCENT_POLICY accent = {as, 0, 0, 0};
	::WINDOWCOMPOSITIONATTRIBDATA data = {WCA_ACCENT_POLICY, &accent, sizeof(accent)};
	return set_window_composition_attribute(hwnd, data).map([](::WINDOWCOMPOSITIONATTRIBDATA&&){});
}

template<typename Hwnd>
static inline expected<void, winapi_last_error> acrylic_window(Hwnd&& hwnd, ::ACCENT_STATE as = ACCENT_ENABLE_BLURBEHIND){
	return acrylic_window(std::forward<Hwnd>(hwnd).get(), as);
}

}
