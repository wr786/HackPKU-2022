#pragma once
#include <cstdio>

using namespace std;

#define DEBUGF(fmt,...) \
    printf("[debug][%s:%d] ",__FILE__,__LINE__); \
    printf(fmt,##__VA_ARGS__); \
    printf("\n")

#define WARNF(fmt,...) \
    printf("[warn][%s:%d] ",__FILE__,__LINE__); \
    printf(fmt,##__VA_ARGS__); \
    printf("\n")

#define ERRORF(fmt,...) \
    printf("[error][%s:%d] ",__FILE__,__LINE__); \
    printf(fmt,##__VA_ARGS__); \
    printf("\n")

