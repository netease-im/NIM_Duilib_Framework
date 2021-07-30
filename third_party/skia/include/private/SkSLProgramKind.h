/*
 * Copyright 2021 Google LLC.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkSLProgramKind_DEFINED
#define SkSLProgramKind_DEFINED

#include <cinttypes>

namespace SkSL {

/**
 * SkSL supports several different program kinds.
 */
enum class ProgramKind : int8_t {
    kFragment,
    kVertex,
    kGeometry,
    kFragmentProcessor,
    kRuntimeColorFilter,  // Runtime effect only suitable as SkColorFilter
    kRuntimeShader,       //   "       "     "      "     "  SkShader
    kRuntimeBlender,      //   "       "     "      "     "  SkBlender
    kGeneric,
};

} // namespace SkSL

#endif
