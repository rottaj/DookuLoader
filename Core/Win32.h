//
// Created by j4ck on 10/13/23.
//
#include <windows.h>
#ifndef WIN32_H
#define WIN32_H

HMODULE MagicGetModuleHandle(IN LPCWSTR moduleName); // Include Hashing

FARPROC MagicGetProcAddress(HMODULE hModule, LPCSTR lpProcName);


#endif //WIN32_H
