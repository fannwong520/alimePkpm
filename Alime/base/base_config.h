#pragma once
#ifndef BASE_BASE_CONFIG_H_
#define BASE_BASE_CONFIG_H_

#include "port.h"

/*
这个世界有两种系统,windows和非windows
所以。。。
*/
#if defined(OS_WIN)
#include "build/windows_config.h"
#include <windows.h>
#endif

#endif // BASE_BASE_CONFIG_H_
