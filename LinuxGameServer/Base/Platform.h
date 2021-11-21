#pragma once

#if __cplusplus >= 201103L || __GNUC__ >= 4 || defined(__clang__)
#ifndef LOCAL_STATIC_INIT_IS_THREADSAFE
#define LOCAL_STATIC_INIT_IS_THREADSAFE 1
#endif
#endif

#define JOIN(X, Y) DO_JOIN(X, Y)
#define DO_JOIN(X, Y) DO_JOIN2(X, Y)
#define DO_JOIN2(X, Y) X##Y

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif // __GNUC__
