#pragma once
#ifndef BASE_BASE_CONFIG_H_
#define BASE_BASE_CONFIG_H_

#include "port.h"

/*
�������������ϵͳ,windows�ͷ�windows
���ԡ�����
*/
#if defined(OS_WIN)
#include "build/windows_config.h"
#include <windows.h>
#endif

#endif // BASE_BASE_CONFIG_H_
