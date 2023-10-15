//
// Created by j4ck on 10/13/23.
//

#ifndef DOOKU_H
#define DOOKU_H

#include <windows.h>
#include <winternl.h>
#include "NTAPI.h"

typedef struct {
    // HTTP Variables ( Not the best way of doing this but whatever.. Will change in future. )
    WCHAR                       url;
    // Start Of SithAPI
    NtOpenProcess               SithOpenProcess;
    NtAllocateVirtualMemory     SithVirtualAlloc;
    NtProtectVirtualMemory      SithVirtualProtect;
    NtWriteVirtualMemory        SithWriteVirtualMemory;
    NtCreateThread              SithCreateThread;
    NtCreateThreadEx            SithCreateThreadEx;
    NtQueueApcThread            SithQueueApcThread;

} SITH_INSTANCE, *PSITH_INSTANCE;


BOOL PleasureOfYouToJoinUs(); // Init Function

BOOL WelcomeToTheDarkSide();

extern PSITH_INSTANCE Instance; // Create shared global variable.

#endif //DOOKU_H
