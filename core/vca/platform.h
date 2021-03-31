#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define VCA_PLATFORM_WINDOWS
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
#define VCA_PLATFORM_IOS
#define VCA_PLATFORM_IOS_SIMULATOR
#elif TARGET_OS_IPHONE
#define VCA_PLATFORM_IOS
#elif TARGET_OS_MAC
#define VCA_PLATFORM_MAC
#else
#error "Unknown Apple platform"
#endif
#elif __ANDROID__
#define VCA_PLATFORM_ANDROID
#elif __linux__
#define VCA_PLATFORM_LINUX
#else
#error "Unsupported platform"
#endif

#ifndef VCA_PLATFORM_WINDOWS
#define VCA_PLATFORM_UNIX
#endif
