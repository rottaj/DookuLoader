//
// Created by j4ck on 10/13/23.
//
#include <wchar.h>
#include <stdio.h>
#include <winternl.h>
#include "Win32.h"


FARPROC MagicGetProcAddress(HMODULE hModule, LPCSTR lpProcName) {

    /*
    HMODULE hModule = NULL;
    hModule = LoadLibraryW(TARGET_LIBRARY);
    if (hModule == NULL) {
        wprintf(L"LoadLibrary Failed with Error Code: %d\n", GetLastError());
        return NULL;
    }
    */

    // IMPORTANT - Must cast handle address to PBYTE or header parsing will fail
    PBYTE pBase = (PBYTE) hModule;

    PIMAGE_DOS_HEADER pImgDosHdr = (PIMAGE_DOS_HEADER) pBase;
    if (pImgDosHdr->e_magic != IMAGE_DOS_SIGNATURE) {
        wprintf(L"Failed to Get DOS Header");
        return NULL;
    }
    PIMAGE_NT_HEADERS pImgNtHdrs = (PIMAGE_NT_HEADERS) (pBase + pImgDosHdr->e_lfanew);
    if (pImgNtHdrs->Signature != IMAGE_NT_SIGNATURE) {
        wprintf(L"Failed to Get NT Header");
        return NULL;
    }

    IMAGE_OPTIONAL_HEADER ImgOptHdr = pImgNtHdrs->OptionalHeader;
    if (ImgOptHdr.Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC) {
        wprintf(L"Failed to Get Optional Header");
        return NULL;
    }

    PIMAGE_EXPORT_DIRECTORY pImgExportDir = (PIMAGE_EXPORT_DIRECTORY) (pBase +
                                                                       ImgOptHdr.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    // Getting the function's names array pointer
    PDWORD FunctionNameArray = (PDWORD) (pBase + pImgExportDir->AddressOfNames);

    // Getting the function's addresses array pointer
    PDWORD FunctionAddressArray = (PDWORD) (pBase + pImgExportDir->AddressOfFunctions);

    // Getting the function's ordinal array pointer
    PWORD FunctionOrdinalArray = (PWORD) (pBase + pImgExportDir->AddressOfNameOrdinals);

    for (DWORD i = 0; i < pImgExportDir->NumberOfFunctions; i++) {
        // Getting the name of the function
        CHAR *pFunctionName = (CHAR *) (pBase + FunctionNameArray[i]);
        // Getting the address of the function
        PVOID pFunctionAddress = (PVOID) (pBase + FunctionAddressArray[FunctionOrdinalArray[i]]);
        // Getting the ordinal of the function
        WORD wFunctionOrdinal = FunctionOrdinalArray[i];

        if (strcmp((LPCSTR) lpProcName, pFunctionName) == 0) {
            // Return function address
            return pFunctionAddress;
        }
    }

    return NULL;
}




HMODULE MagicGetModuleHandle(IN LPCWSTR moduleName) {

    // 64 bit
    PPEB pPeb = (PPEB)(__readgsqword(0x60));

    // Getting Ldr
    PPEB_LDR_DATA		    pLdr	= (PPEB_LDR_DATA)(pPeb->Ldr);

    // Getting the first element in the linked list which contains information about the first module
    PLDR_DATA_TABLE_ENTRY	pDte	= (PLDR_DATA_TABLE_ENTRY)(pLdr->InMemoryOrderModuleList.Flink);

    while (pDte) {

        // If not null
        if (pDte->FullDllName.Length != NULL) {
            // Print the DLL name
            if (IsStringEqual(pDte->FullDllName.Buffer, moduleName)) {
                return (HMODULE)pDte->Reserved2[0]; // Return DLL once found
            }

        }
        else {
            break;
        }

        // Next element in the linked list
        pDte = *(PLDR_DATA_TABLE_ENTRY*)(pDte);

    }
    // Return NULL if not found
    return NULL;
}

// Used for checking case-sensitive library names
BOOL IsStringEqual (IN LPCWSTR Str1, IN LPCWSTR Str2) {

    WCHAR lStr1[MAX_PATH],
            lStr2[MAX_PATH];

    int len1 = lstrlenW(Str1),
            len2 = lstrlenW(Str2);

    int i = 0,
            j = 0;

    // Checking length. We dont want to overflow the buffers
    if (len1 >= MAX_PATH || len2 >= MAX_PATH)
        return FALSE;

    // Converting Str1 to lower case string (lStr1)
    for (i = 0; i < len1; i++) {
        lStr1[i] = (WCHAR) tolower(Str1[i]);
    }
    lStr1[i++] = L'\0'; // null terminating

    // Converting Str2 to lower case string (lStr2)
    for (j = 0; j < len2; j++) {
        lStr2[j] = (WCHAR) tolower(Str2[j]);
    }
    lStr2[j++] = L'\0'; // null terminating

    // Comparing the lower-case strings
    if (lstrcmpiW(lStr1, lStr2) == 0)
        return TRUE;

    return FALSE;
}