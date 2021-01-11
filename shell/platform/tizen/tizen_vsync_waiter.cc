// Copyright 2020 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "tizen_vsync_waiter.h"

#include <Ecore.h>

#include "flutter/shell/platform/tizen/tizen_embedder_engine.h"
#include "flutter/shell/platform/tizen/tizen_log.h"

static void RequestVblank(void* data, Ecore_Thread* thread) {
  TizenVsyncWaiter* tizen_vsync_waiter =
      reinterpret_cast<TizenVsyncWaiter*>(data);
  tizen_vsync_waiter->HandleVblankLoopRequest();
}

TizenVsyncWaiter::TizenVsyncWaiter(TizenEmbedderEngine* engine)
    : engine_(engine) {
  if (!CreateTDMVblank()) {
    FT_LOGE("Failed to create TDM vblank");
  }
}

TizenVsyncWaiter::~TizenVsyncWaiter() {
  if (vblank_) {
    tdm_client_vblank_destroy(vblank_);
  }
  if (client_) {
    tdm_client_destroy(client_);
  }
}

void TizenVsyncWaiter::AsyncWaitForVsync(intptr_t baton) {
  baton_ = baton;
  ecore_thread_run(RequestVblank, NULL, NULL, this);
}

bool TizenVsyncWaiter::CreateTDMVblank() {
  tdm_error ret;
  client_ = tdm_client_create(&ret);
  if (ret != TDM_ERROR_NONE && client_ != NULL) {
    FT_LOGE("create client fail");
    return false;
  }

  output_ = tdm_client_get_output(client_, const_cast<char*>("default"), &ret);
  if (ret != TDM_ERROR_NONE && output_ != NULL) {
    FT_LOGE("get output fail");
    return false;
  }

  vblank_ = tdm_client_output_create_vblank(output_, &ret);
  if (ret != TDM_ERROR_NONE && vblank_ != NULL) {
    FT_LOGE("create vblank fail");
    return false;
  }

  tdm_client_vblank_set_enable_fake(vblank_, 1);
  return true;
}

void TizenVsyncWaiter::TdmClientVblankCallback(
    tdm_client_vblank* vblank, tdm_error error, unsigned int sequence,
    unsigned int tv_sec, unsigned int tv_usec, void* user_data) {
  TizenVsyncWaiter* tizen_vsync_waiter =
      reinterpret_cast<TizenVsyncWaiter*>(user_data);

  FT_ASSERT(tizen_vsync_waiter != nullptr);
  FT_ASSERT(tizen_vsync_waiter->engine_ != nullptr);
  FT_ASSERT(tizen_vsync_waiter->engine_->flutter_engine != nullptr);

  uint64_t frame_start_time_nanos = tv_sec * 1e9 + tv_usec * 1e3;
  uint64_t frame_target_time_nanos = 16.6 * 1e6 + frame_start_time_nanos;
  FlutterEngineOnVsync(tizen_vsync_waiter->engine_->flutter_engine,
                       tizen_vsync_waiter->baton_, frame_start_time_nanos,
                       frame_target_time_nanos);
}

void TizenVsyncWaiter::HandleVblankLoopRequest() {
  tdm_error ret;
  ret = tdm_client_vblank_wait(vblank_, 1, TdmClientVblankCallback, this);
  if (ret != TDM_ERROR_NONE) {
    FT_LOGE("ERROR, ret = %d", ret);
    return;
  }
  tdm_client_handle_events(client_);
}
