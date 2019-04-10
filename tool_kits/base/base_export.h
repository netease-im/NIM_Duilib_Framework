// Copyright (c) 2013, NetEase Inc. All rights reserved.
//
// Wang Rongtao <rtwang@corp.netease.com>
// 2013/8/30
//
// This file defines BASE_EXPORT macro

#ifndef BASE_BASE_EXPORT_H_
#define BASE_BASE_EXPORT_H_

#if defined(COMPONENT_BUILD)
#if defined(WIN32)
#if defined(BASE_IMPLEMENTATION)
#define BASE_EXPORT __declspec(dllexport)
#else
#define BASE_EXPORT __declspec(dllimport)
#endif  // defined(BASE_IMPLEMENTATION)
#else
#define BASE_EXPORT __attribute__((visibility("default")))
#endif  // defined(WIN32)
#else
#define BASE_EXPORT
#endif  // defined(COMPONENT_BUILD)

#endif  // BASE_BASE_EXPORT_H_