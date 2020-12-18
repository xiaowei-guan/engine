// Copyright 2020 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EMBEDDER_TIZEN_SURFACE_GL_ECORE_WL_H_
#define EMBEDDER_TIZEN_SURFACE_GL_ECORE_WL_H_

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <wayland-client.h>
#include <wayland-egl.h>

#include <algorithm>
#include <string>
#include <vector>
#define EFL_BETA_API_SUPPORT
//#include <Ecore_Input.h>
#include <Ecore_Wayland.h>


#include "flutter/shell/platform/embedder/embedder.h"
#include "flutter/shell/platform/tizen/tizen_surface.h"

class TizenSurfaceGLEcoreWl : public TizenSurface {
 private:
  struct EglData {
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLContext resource_context;
    EGLSurface resource_surface;
    EGLConfig config;
    EglData()
        : display(EGL_NO_DISPLAY),
          context(EGL_NO_CONTEXT),
          surface(EGL_NO_SURFACE),
          resource_context(EGL_NO_CONTEXT),
          resource_surface(EGL_NO_SURFACE),
          config(nullptr) {}
  };
  bool display_valid_ = false;
  Ecore_Wl_Window* ecore_wl_window_ = nullptr;
  wl_egl_window* wl_egl_window_ = nullptr;
  wl_display* wl_display_= nullptr;
  wl_surface* wl_surface_ = nullptr;
  
  EglData eglData_;

 public:
  TizenSurfaceGLEcoreWl(int32_t x, int32_t y, int32_t width, int32_t height);
  ~TizenSurfaceGLEcoreWl();
  bool OnMakeCurrent();
  bool OnClearCurrent();
  bool OnMakeResourceCurrent();
  bool OnPresent();
  uint32_t OnGetFBO();
  void* OnProcResolver(const char* name);
  bool IsValid();
  bool InitalizeDisplay();
  bool InitRenderSurface();
  bool InitializeEGL();
  void PrintEGLError();
  void Destroy();
  void SetSize(int32_t width, int32_t height);
};

#endif  // EMBEDDER_TIZEN_SURFACE_GL_H_
