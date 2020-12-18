// Copyright 2020 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "tizen_surface_gl_ecore_wl.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "flutter/shell/platform/tizen/logger.h"

TizenSurfaceGLEcoreWl::TizenSurfaceGLEcoreWl(int32_t x, int32_t y,
                                             int32_t width, int32_t height)
    : TizenSurface(x, y, width, height) {
  LoggerD("x =[%d] y = [%d], width = [%d], height = [%d]", x, y, width, height);
  InitalizeDisplay();
}

bool TizenSurfaceGLEcoreWl::IsValid() { return display_valid_; }

bool TizenSurfaceGLEcoreWl::OnMakeCurrent() {
  if (!display_valid_) {
    LoggerE("Invalid Display");
    return false;
  }
  if (eglMakeCurrent(eglData_.display, eglData_.surface, eglData_.surface, eglData_.context) !=
      EGL_TRUE) {
    LoggerE("Could not make the onscreen context current");
    return false;
  }
  return true;
}

bool TizenSurfaceGLEcoreWl::OnMakeResourceCurrent() {
  if (!display_valid_) {
    LoggerE("Invalid Display");
    return false;
  }
  if (eglMakeCurrent(eglData_.display, eglData_.resource_surface, eglData_.resource_surface,
                     eglData_.resource_context) != EGL_TRUE) {
    LoggerE("Could not make the offscreen context current");
    return false;
  }
  return true;
}

bool TizenSurfaceGLEcoreWl::OnClearCurrent() {
  if (!display_valid_) {
    LoggerE("Invalid display");
    return false;
  }

  if (eglMakeCurrent(eglData_.display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                     EGL_NO_CONTEXT) != EGL_TRUE) {
    LoggerE("Could not clear context");
    return false;
  }
  return true;
}

bool TizenSurfaceGLEcoreWl::OnPresent() {
  if (!display_valid_) {
    LoggerE("Invalid display");
    return false;
  }

  if (eglSwapBuffers(eglData_.display, eglData_.surface) != EGL_TRUE) {
    LoggerE("Could not swap EGl buffer");
    return false;
  }
  return true;
}

uint32_t TizenSurfaceGLEcoreWl::OnGetFBO() {
  if (!display_valid_) {
    LoggerE("Invalid display");
    return 999;
  }
  LoggerD("OnApplicationGetOnscreenFBO");
  return 0;  // FBO0
}

#define GL_FUNC(FunctionName)                     \
  else if (strcmp(name, #FunctionName) == 0) {    \
    return reinterpret_cast<void*>(FunctionName); \
  }
void* TizenSurfaceGLEcoreWl::OnProcResolver(const char* name) {
  auto address = eglGetProcAddress(name);
  if (address != nullptr) {
    return reinterpret_cast<void*>(address);
  }
  GL_FUNC(eglGetCurrentDisplay)
  GL_FUNC(eglQueryString)
  GL_FUNC(glActiveTexture)
  GL_FUNC(glAttachShader)
  GL_FUNC(glBindAttribLocation)
  GL_FUNC(glBindBuffer)
  GL_FUNC(glBindFramebuffer)
  GL_FUNC(glBindRenderbuffer)
  GL_FUNC(glBindTexture)
  GL_FUNC(glBlendColor)
  GL_FUNC(glBlendEquation)
  GL_FUNC(glBlendFunc)
  GL_FUNC(glBufferData)
  GL_FUNC(glBufferSubData)
  GL_FUNC(glCheckFramebufferStatus)
  GL_FUNC(glClear)
  GL_FUNC(glClearColor)
  GL_FUNC(glClearStencil)
  GL_FUNC(glColorMask)
  GL_FUNC(glCompileShader)
  GL_FUNC(glCompressedTexImage2D)
  GL_FUNC(glCompressedTexSubImage2D)
  GL_FUNC(glCopyTexSubImage2D)
  GL_FUNC(glCreateProgram)
  GL_FUNC(glCreateShader)
  GL_FUNC(glCullFace)
  GL_FUNC(glDeleteBuffers)
  GL_FUNC(glDeleteFramebuffers)
  GL_FUNC(glDeleteProgram)
  GL_FUNC(glDeleteRenderbuffers)
  GL_FUNC(glDeleteShader)
  GL_FUNC(glDeleteTextures)
  GL_FUNC(glDepthMask)
  GL_FUNC(glDisable)
  GL_FUNC(glDisableVertexAttribArray)
  GL_FUNC(glDrawArrays)
  GL_FUNC(glDrawElements)
  GL_FUNC(glEnable)
  GL_FUNC(glEnableVertexAttribArray)
  GL_FUNC(glFinish)
  GL_FUNC(glFlush)
  GL_FUNC(glFramebufferRenderbuffer)
  GL_FUNC(glFramebufferTexture2D)
  GL_FUNC(glFrontFace)
  GL_FUNC(glGenBuffers)
  GL_FUNC(glGenerateMipmap)
  GL_FUNC(glGenFramebuffers)
  GL_FUNC(glGenRenderbuffers)
  GL_FUNC(glGenTextures)
  GL_FUNC(glGetBufferParameteriv)
  GL_FUNC(glGetError)
  GL_FUNC(glGetFramebufferAttachmentParameteriv)
  GL_FUNC(glGetIntegerv)
  GL_FUNC(glGetProgramInfoLog)
  GL_FUNC(glGetProgramiv)
  GL_FUNC(glGetRenderbufferParameteriv)
  GL_FUNC(glGetShaderInfoLog)
  GL_FUNC(glGetShaderiv)
  GL_FUNC(glGetShaderPrecisionFormat)
  GL_FUNC(glGetString)
  GL_FUNC(glGetUniformLocation)
  GL_FUNC(glIsTexture)
  GL_FUNC(glLineWidth)
  GL_FUNC(glLinkProgram)
  GL_FUNC(glPixelStorei)
  GL_FUNC(glReadPixels)
  GL_FUNC(glRenderbufferStorage)
  GL_FUNC(glScissor)
  GL_FUNC(glShaderSource)
  GL_FUNC(glStencilFunc)
  GL_FUNC(glStencilFuncSeparate)
  GL_FUNC(glStencilMask)
  GL_FUNC(glStencilMaskSeparate)
  GL_FUNC(glStencilOp)
  GL_FUNC(glStencilOpSeparate)
  GL_FUNC(glTexImage2D)
  GL_FUNC(glTexParameterf)
  GL_FUNC(glTexParameterfv)
  GL_FUNC(glTexParameteri)
  GL_FUNC(glTexParameteriv)
  GL_FUNC(glTexSubImage2D)
  GL_FUNC(glUniform1f)
  GL_FUNC(glUniform1fv)
  GL_FUNC(glUniform1i)
  GL_FUNC(glUniform1iv)
  GL_FUNC(glUniform2f)
  GL_FUNC(glUniform2fv)
  GL_FUNC(glUniform2i)
  GL_FUNC(glUniform2iv)
  GL_FUNC(glUniform3f)
  GL_FUNC(glUniform3fv)
  GL_FUNC(glUniform3i)
  GL_FUNC(glUniform3iv)
  GL_FUNC(glUniform4f)
  GL_FUNC(glUniform4fv)
  GL_FUNC(glUniform4i)
  GL_FUNC(glUniform4iv)
  GL_FUNC(glUniformMatrix2fv)
  GL_FUNC(glUniformMatrix3fv)
  GL_FUNC(glUniformMatrix4fv)
  GL_FUNC(glUseProgram)
  GL_FUNC(glVertexAttrib1f)
  GL_FUNC(glVertexAttrib2fv)
  GL_FUNC(glVertexAttrib3fv)
  GL_FUNC(glVertexAttrib4fv)
  GL_FUNC(glVertexAttribPointer)
  GL_FUNC(glViewport)

  LoggerW("Could not resolve: %s", name);
  return nullptr;
}
#undef GL_FUNC

TizenSurfaceGLEcoreWl::~TizenSurfaceGLEcoreWl() {
  if (IsValid()) {
    display_valid_ = false;
    Destroy();
  }
}

void TizenSurfaceGLEcoreWl::PrintEGLError() {
  EGLint error = eglGetError();
  switch (error) {
    case EGL_BAD_DISPLAY: {
      LoggerE("EGL_BAD_DISPLAY : Display is not an EGL display connection\n");
      break;
    }
    case EGL_NOT_INITIALIZED: {
      LoggerE("EGL_NOT_INITIALIZED : Display has not been initialized\n");
      break;
    }
    case EGL_BAD_SURFACE: {
      LoggerE("EGL_BAD_SURFACE : Draw or read is not an EGL surface\n");
      break;
    }
    case EGL_BAD_CONTEXT: {
      LoggerE("EGL_BAD_CONTEXT : Context is not an EGL rendering context\n");
      break;
    }
    case EGL_BAD_CONFIG: {
      LoggerE(
          "EGL_BAD_CONFIG : Config is not an EGL frame buffer configuration\n");
      break;
    }
    case EGL_BAD_MATCH: {
      LoggerE(
          "EGL_BAD_MATCH : Draw or read are not compatible with context, or if "
          "context is set to EGL_NO_CONTEXT and draw or read are not set to "
          "EGL_NO_SURFACE, or if draw or read are set to EGL_NO_SURFACE and "
          "context is not set to EGL_NO_CONTEXT\n");
      break;
    }
    case EGL_BAD_ACCESS: {
      LoggerE("EGL_BAD_ACCESS : Context is current to some other thread\n");
      break;
    }
    case EGL_BAD_NATIVE_PIXMAP: {
      LoggerE(
          "EGL_BAD_NATIVE_PIXMAP : A native pixmap underlying either draw or "
          "read is no longer valid\n");
      break;
    }
    case EGL_BAD_NATIVE_WINDOW: {
      LoggerE(
          "EGL_BAD_NATIVE_WINDOW : A native window underlying either draw or "
          "read is no longer valid\n");
      break;
    }
    case EGL_BAD_CURRENT_SURFACE: {
      LoggerE(
          "EGL_BAD_CURRENT_SURFACE : The previous context has unflushed "
          "commands and the previous surface is no longer valid\n");
      break;
    }
    case EGL_BAD_ALLOC: {
      LoggerE(
          "EGL_BAD_ALLOC : Allocation of ancillary buffers for draw or read "
          "were delayed until eglMakeCurrent is called, and there are not "
          "enough resources to allocate them\n");
      break;
    }
    case EGL_CONTEXT_LOST: {
      LoggerE(
          "EGL_CONTEXT_LOST : If a power management event has occurred. The "
          "application must destroy all contexts and reinitialise OpenGL ES "
          "state and objects to continue rendering\n");
      break;
    }
    case EGL_BAD_PARAMETER: {
      LoggerE("Invalid parameter is passed\n");
      break;
    }
    case EGL_BAD_ATTRIBUTE: {
      LoggerE(
          "The parameter configAttribs contains an invalid frame buffer "
          "configuration attribute or an attribute value that is unrecognized "
          "or out of range\n");
      break;
    }
    default: {
      LoggerE("Unknown error with code: %d\n", error);
      break;
    }
  }
}

bool TizenSurfaceGLEcoreWl::InitRenderSurface() {
  EGLint* ptr = NULL;
  wl_display_ = ecore_wl_display_get();
  assert(NULL != wl_display_);

  if (!InitializeEGL()) {
    LoggerE("EGL intialization is failed");
    return false;
  }

  wl_egl_window_= wl_egl_window_create(ecore_wl_window_surface_get(ecore_wl_window_),
                                     window_width_, window_height_);
  if (NULL == wl_egl_window_) {
    LoggerE("wl_egl_window_create is Failed");
    return false;
  }

  const EGLint attribs[] = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};
  eglData_.surface = eglCreateWindowSurface(
      eglData_.display, eglData_.config, (EGLNativeWindowType)wl_egl_window_, ptr);
  if (eglData_.surface == EGL_NO_SURFACE) {
    LoggerE("eglCreateWindowSurface is Failed");
    return false;
  }
  eglData_.resource_surface =
      eglCreatePbufferSurface(eglData_.display, eglData_.config, attribs);
  if (eglData_.resource_surface == EGL_NO_SURFACE) {
    LoggerE("eglCreatePbufferSurface is Failed");
    return false;
  }
  return true;
}

bool TizenSurfaceGLEcoreWl::InitializeEGL() {
  // egl CONTEXT
  const EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
  EGLint configAttribs[] = {EGL_SURFACE_TYPE,
                            EGL_WINDOW_BIT,
                            EGL_RED_SIZE,
                            8,
                            EGL_GREEN_SIZE,
                            8,
                            EGL_BLUE_SIZE,
                            8,
                            EGL_ALPHA_SIZE,
                            EGL_DONT_CARE,
                            EGL_RENDERABLE_TYPE,
                            EGL_OPENGL_ES2_BIT,
                            EGL_SAMPLE_BUFFERS,
                            EGL_DONT_CARE,
                            EGL_SAMPLES,
                            EGL_DONT_CARE,
                            EGL_NONE};

  EGLint major = 0;
  EGLint minor = 0;
  int bufferSize = 32;
  eglData_.display = eglGetDisplay((EGLNativeDisplayType)wl_display_);
  if (EGL_NO_DISPLAY == eglData_.display) {
    LoggerE("EGL Get Display is failed");
    return false;
  }

  if (!eglInitialize(eglData_.display, &major, &minor)) {
    LoggerE("EGL Intialize is Failed major [%d] minor [%d]", major, minor);
    PrintEGLError();
    return false;
  }

  if (!eglBindAPI(EGL_OPENGL_ES_API)) {
    PrintEGLError();
    return false;
  }

  EGLint numOfConfig = 0;
  // Query all framebuffer configurations
  if (!eglGetConfigs(eglData_.display, NULL, 0, &numOfConfig)) {
    LoggerE("eglGetConfigs is Failed!!");
    PrintEGLError();
    return false;
  }

  EGLConfig* configs = (EGLConfig*)calloc(numOfConfig, sizeof(EGLConfig));
  EGLint n;
  // Get the List of EGL framebuffer configuration matches with configAttribs in
  // list "configs"
  if (!eglChooseConfig(eglData_.display, configAttribs, configs, numOfConfig,
                       &n)) {
    free(configs);
    configs = NULL;
    PrintEGLError();
    return false;
  }

  EGLint size;
  for (int i = 0; i < n; i++) {
    eglGetConfigAttrib(eglData_.display, configs[i], EGL_BUFFER_SIZE, &size);
    if (bufferSize == size) {
      eglData_.config = configs[i];
      break;
    }
  }
  free(configs);
  configs = NULL;

  eglData_.context = eglCreateContext(eglData_.display, eglData_.config,
                                      EGL_NO_CONTEXT, contextAttribs);
  if (EGL_NO_CONTEXT == eglData_.context) {
    PrintEGLError();
    return false;
  }

  eglData_.resource_context = eglCreateContext(
      eglData_.display, eglData_.config, EGL_NO_CONTEXT, contextAttribs);
  if (EGL_NO_CONTEXT == eglData_.resource_context) {
    PrintEGLError();
    return false;
  }
  return true;
}

bool TizenSurfaceGLEcoreWl::InitalizeDisplay() {
  LoggerD("x =[%d] y = [%d], width = [%d], height = [%d]", x_, y_,
          window_width_, window_height_);
  if (window_width_ == 0 || window_height_ == 0) {
    return false;
  }

  // ecore_wl INIT
  if (!ecore_wl_init(NULL)) {
    LoggerE("Could not initialize ecore_wl");
    return false;
  }
  // ecore_wl WINDOW
  ecore_wl_window_ =
      ecore_wl_window_new(nullptr, x_, y_, window_width_, window_height_,
                          ECORE_WL_WINDOW_BUFFER_TYPE_EGL_WINDOW);
  LoggerD("ecore_wl_window_: %p", ecore_wl_window_);
  //ecore_wl_window_type_set(ecore_wl_window_, ECORE_WL_WINDOW_TYPE_TOPLEVEL);
  //ecore_wl_window_alpha_set(ecore_wl_window_, EINA_FALSE);
  //ecore_wl_window_show(ecore_wl_window_);
  //ecore_wl_window_aux_hint_add(ecore_wl_window_, 0, "wm.policy.win.user.geometry",
  //                             "1");

  // egl WINDOW
  wl_surface_ = ecore_wl_window_surface_create(ecore_wl_window_);
  if (nullptr == wl_surface_) {
    LoggerE("ecore_wl_window_surface_create fail");
    return false;
  }

  if(!InitRenderSurface()){
    LoggerE( "Failed to InitRenderSurface" );
    Destroy();
    return false;
  }
  display_valid_ = true;
  ecore_wl_window_show( ecore_wl_window_ );
  return true;
}

void TizenSurfaceGLEcoreWl::Destroy() {
  if (EGL_NO_DISPLAY != eglData_.display) {
    eglMakeCurrent(eglData_.display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                   EGL_NO_CONTEXT);

    if (EGL_NO_SURFACE != eglData_.surface) {
      eglDestroySurface(eglData_.display, eglData_.surface);
      eglData_.surface = EGL_NO_SURFACE;
    }

    if (EGL_NO_CONTEXT != eglData_.context) {
      eglDestroyContext(eglData_.display, eglData_.context);
      eglData_.context = EGL_NO_CONTEXT;
    }

    if (EGL_NO_SURFACE != eglData_.resource_surface) {
      eglDestroySurface(eglData_.display, eglData_.resource_surface);
      eglData_.resource_surface = EGL_NO_SURFACE;
    }

    if (EGL_NO_CONTEXT != eglData_.resource_context) {
      eglDestroyContext(eglData_.display, eglData_.resource_context);
      eglData_.resource_context = EGL_NO_CONTEXT;
    }
  }
  if (wl_egl_window_) {
    wl_egl_window_destroy(wl_egl_window_);
    wl_egl_window_ = nullptr;
  }
  if (ecore_wl_window_) {
    ecore_wl_window_free(ecore_wl_window_);
    ecore_wl_window_ = nullptr;
  }
  ecore_wl_shutdown();
}

void TizenSurfaceGLEcoreWl::SetSize(int32_t width, int32_t height) {
  LoggerD("Resize egl window %d %d", width, height);
  //ecore_wl_egl_window_resize_with_rotation(wl_egl_window_, 0, 0, width, height,
   //                                         0);
}
