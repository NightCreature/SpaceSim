#pragma once

#if defined(__GNUC__)
#define IS_GCC
#elif defined(__clang__)
#define IS_CLANG
#elif defined (_MSC_VER)
#define IS_MSVC
#endif

#if defined(IS_GCC) || defined(IS_CLANG)
#define FUNCTION_NAME __PRETTY_FUNCTION__
#elif defined(IS_MSVC)
#define FUNCTION_NAME __FUNCSIG__
#else
#define FUNCTION_NAME
#endif