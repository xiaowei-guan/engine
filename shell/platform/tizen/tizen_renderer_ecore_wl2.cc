// Copyright 2020 Samsung Electronics Co., Ltd. All rights reserved.
// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "tizen_renderer_ecore_wl2.h"

#include "flutter/shell/platform/tizen/logger.h"

TizenRendererEcoreWl2::TizenRendererEcoreWl2(int32_t x, int32_t y, int32_t w,
                                             int32_t h) {
  InitializeRenderer(x, y, w, h);
}
TizenRendererEcoreWl2::~TizenRendererEcoreWl2() { DestoryRenderer(); }
bool TizenRendererEcoreWl2::SetupDisplay() {
  if (!ecore_wl2_init()) {
    LoggerE("Could not initialize ecore_wl2");
    return false;
  }
  ecore_wl2_display_ = ecore_wl2_display_connect(nullptr);
  if (ecore_wl2_display_ == nullptr) {
    LoggerE("Disyplay not found");
    return false;
  }
  LoggerD("ecore_wl2_display_: %p", ecore_wl2_display_);
  ecore_wl2_sync();
  return true;
}

bool TizenRendererEcoreWl2::SetupEcoreWlWindow(int32_t x, int32_t y, int32_t w,
                                               int32_t h) {
  if (w == 0 || h == 0) {
    LoggerE("Failed to create because of the wrong size");
  }
  ecore_wl2_window_ =
      ecore_wl2_window_new(ecore_wl2_display_, nullptr, x, y, w, h);
  ecore_wl2_window_type_set(ecore_wl2_window_, ECORE_WL2_WINDOW_TYPE_TOPLEVEL);
  ecore_wl2_window_alpha_set(ecore_wl2_window_, EINA_FALSE);
  ecore_wl2_window_aux_hint_add(ecore_wl2_window_, 0,
                                "wm.policy.win.user.geometry", "1");
  return true;
}

bool TizenRendererEcoreWl2::SetupEglWindow(int32_t w, int32_t h) {
  ecore_wl2_egl_window_ = ecore_wl2_egl_window_create(ecore_wl2_window_, w, h);
  return true;
}

EGLDisplay TizenRendererEcoreWl2::GetEGLDisplay() {
  return eglGetDisplay(
      (EGLNativeDisplayType)ecore_wl2_display_get(ecore_wl2_display_));
}

EGLNativeWindowType TizenRendererEcoreWl2::GetEGLNativeWindowType() {
  return (EGLNativeWindowType)ecore_wl2_egl_window_native_get(
      ecore_wl2_egl_window_);
}

void TizenRendererEcoreWl2::DestoryEglWindow() {
  if (ecore_wl2_egl_window_) {
    ecore_wl2_egl_window_destroy(ecore_wl2_egl_window_);
    ecore_wl2_egl_window_ = nullptr;
  }
}
void TizenRendererEcoreWl2::DestoryEcoreWlWindow() {
  if (ecore_wl2_window_) {
    ecore_wl2_window_free(ecore_wl2_window_);
    ecore_wl2_window_ = nullptr;
  }
}
void TizenRendererEcoreWl2::ShutdownDisplay() {
  if (ecore_wl2_display_) {
    ecore_wl2_display_disconnect(ecore_wl2_display_);
    ecore_wl2_display_ = nullptr;
  }
  ecore_wl2_shutdown();
}

TizenRenderer::TizenWindowGeometry TizenRendererEcoreWl2::GetGeometry() {
  TizenWindowGeometry result;
  ecore_wl2_window_geometry_get(ecore_wl2_window_, &result.x, &result.y,
                                &result.w, &result.h);
  return result;
}

void TizenRendererEcoreWl2::SetSize(int32_t width, int32_t height) {
  // FIXME : I think we have to find another way.
  LoggerD("Resize egl window %d %d", width, height);
  ecore_wl2_egl_window_resize_with_rotation(ecore_wl2_egl_window_, 0, 0, width,
                                            height, 0);
}

int TizenRendererEcoreWl2::GetEcoreWindowId() {
  return ecore_wl2_window_id_get(ecore_wl2_window_);
}