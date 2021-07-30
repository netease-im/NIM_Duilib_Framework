/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrBackendSemaphore_DEFINED
#define GrBackendSemaphore_DEFINED

#include "include/gpu/GrTypes.h"

#include "include/gpu/gl/GrGLTypes.h"
#include "include/gpu/mtl/GrMtlTypes.h"
#include "include/gpu/vk/GrVkTypes.h"
#ifdef SK_DIRECT3D
#include "include/gpu/d3d/GrD3DTypesMinimal.h"
#endif

/**
 * Wrapper class for passing into and receiving data from Ganesh about a backend semaphore object.
 */
class GrBackendSemaphore {
public:
    // For convenience we just set the backend here to OpenGL. The GrBackendSemaphore cannot be used
    // until either initGL or initVulkan are called which will set the appropriate GrBackend.
    GrBackendSemaphore()
            : fBackend(GrBackendApi::kOpenGL), fGLSync(nullptr), fIsInitialized(false) {}

#ifdef SK_DIRECT3D
    // We only need to specify these if Direct3D is enabled, because it requires special copy
    // characteristics.
    ~GrBackendSemaphore();
    GrBackendSemaphore(const GrBackendSemaphore&);
    GrBackendSemaphore& operator=(const GrBackendSemaphore&);
#endif

    void initGL(GrGLsync sync) {
        fBackend = GrBackendApi::kOpenGL;
        fGLSync = sync;
        fIsInitialized = true;
    }

    void initVulkan(VkSemaphore semaphore) {
        fBackend = GrBackendApi::kVulkan;
        fVkSemaphore = semaphore;
#ifdef SK_VULKAN
        fIsInitialized = true;
#else
        fIsInitialized = false;
#endif
    }

    // It is the creator's responsibility to ref the MTLEvent passed in here, via __bridge_retained.
    // The other end will wrap this BackendSemaphore and take the ref, via __bridge_transfer.
    void initMetal(GrMTLHandle event, uint64_t value) {
        fBackend = GrBackendApi::kMetal;
        fMtlEvent = event;
        fMtlValue = value;
#ifdef SK_METAL
        fIsInitialized = true;
#else
        fIsInitialized = false;
#endif
    }

#ifdef SK_DIRECT3D
    void initDirect3D(const GrD3DFenceInfo& info) {
        fBackend = GrBackendApi::kDirect3D;
        this->assignD3DFenceInfo(info);
        fIsInitialized = true;
    }
#endif

    bool isInitialized() const { return fIsInitialized; }

    GrGLsync glSync() const {
        if (!fIsInitialized || GrBackendApi::kOpenGL != fBackend) {
            return nullptr;
        }
        return fGLSync;
    }

    VkSemaphore vkSemaphore() const {
        if (!fIsInitialized || GrBackendApi::kVulkan != fBackend) {
            return VK_NULL_HANDLE;
        }
        return fVkSemaphore;
    }

    GrMTLHandle mtlSemaphore() const {
        if (!fIsInitialized || GrBackendApi::kMetal != fBackend) {
            return nullptr;
        }
        return fMtlEvent;
    }

    uint64_t mtlValue() const {
        if (!fIsInitialized || GrBackendApi::kMetal != fBackend) {
            return 0;
        }
        return fMtlValue;
    }

#ifdef SK_DIRECT3D
    bool getD3DFenceInfo(GrD3DFenceInfo* outInfo) const;
#endif

private:
#ifdef SK_DIRECT3D
    void assignD3DFenceInfo(const GrD3DFenceInfo& info);
#endif

    GrBackendApi fBackend;
    union {
        GrGLsync    fGLSync;
        VkSemaphore fVkSemaphore;
        GrMTLHandle fMtlEvent;    // Expected to be an id<MTLEvent>
#ifdef SK_DIRECT3D
        GrD3DFenceInfo* fD3DFenceInfo;
#endif
    };
    uint64_t fMtlValue;
    bool fIsInitialized;
};

#endif
