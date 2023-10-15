//
// Created by j4ck on 10/13/23.

// Using VX-API https://github.com/vxunderground/VX-API/blob/main/VX-API/HashStringMurmur.cpp
//
#include <windows.h>

#ifndef HASH_H
#define HASH_H


SIZE_T StringLengthA(_In_ LPCSTR String);
SIZE_T StringLengthW(_In_ LPCWSTR String);
INT32 HashStringMurmurW(_In_ LPCWSTR String);
INT32 HashStringMurmurA(_In_ LPCSTR String);

#endif //HASH_H
