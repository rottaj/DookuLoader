//
// Created by j4ck on 10/13/23.
//

#ifndef NTAPI_H
#define NTAPI_H
#include <windows.h>
#include <winternl.h>

/* // Probably better implement custom CLIENT_ID & OBJECT_ATTRIBUTES
typedef struct {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID;
*/

typedef struct _INITIAL_TEB
{
    PVOID PreviousStackBase;
    PVOID PreviousStackLimit;
    PVOID StackBase;
    PVOID StackLimit;
    PVOID AllocatedStackBase;
} INITIAL_TEB, *PINITIAL_TEB;

typedef struct _PS_ATTRIBUTE_LIST
{

} PS_ATTRIBUTE_LIST, *PPS_ATTRIBUTE_LIST;

typedef NTSTATUS (NTAPI* NtOpenProcess)(
        OUT          PHANDLE              ProcessHandle,
        IN           DWORD                DesiredAccess,
        IN           POBJECT_ATTRIBUTES   ObjectAttributes,
        IN OPTIONAL  PCLIENT_ID           ClientId
);

typedef NTSTATUS (NTAPI* NtAllocateVirtualMemory) (
        IN          HANDLE       ProcessHandle,
        IN OUT      PVOID         BaseAddress,
        IN          ULONG_PTR     ZeroBits,
        IN OUT      PSIZE_T        pRegionSize,
        IN          ULONG         AllocationType,
        IN          ULONG         Protect
);

typedef NTSTATUS (NTAPI* NtProtectVirtualMemory)(
        IN          HANDLE ProcessHandle,
        IN OUT      PVOID *BaseAddress,
        IN OUT      PULONG NumberOfBytesToProtect,
        IN          ULONG NewAccessProtection,
        OUT         PULONG OldAccessProtection
);

typedef NTSTATUS (NTAPI* NtWriteVirtualMemory)(
        IN           HANDLE ProcessHandle,
        IN           PVOID BaseAddress,
        IN           PVOID Buffer,
        IN           ULONG NumberOfBytesToWrite,
        OUT OPTIONAL PULONG NumberOfBytesWritten
);


typedef NTSTATUS (NTAPI* NtCreateThread)	(
        OUT PHANDLE 	ThreadHandle,
        IN ACCESS_MASK 	DesiredAccess,
        IN OPTIONAL POBJECT_ATTRIBUTES ObjectAttributes 	,
        IN HANDLE 	ProcessHandle,
        OUT PCLIENT_ID 	ClientId,
        IN PCONTEXT 	ThreadContext,
        IN PINITIAL_TEB 	InitialTeb,
        IN BOOLEAN 	CreateSuspended
);

typedef NTSTATUS (NTAPI* NtCreateThreadEx)(
        OUT PHANDLE                 ThreadHandle,
        IN 	ACCESS_MASK             DesiredAccess,
        IN 	POBJECT_ATTRIBUTES      ObjectAttributes,
        IN 	HANDLE                  ProcessHandle,
        IN 	PVOID                   StartRoutine,
        IN 	PVOID                   Argument,             // set to NULL
        IN 	ULONG                   CreateFlags,          // set to NULL
        IN 	SIZE_T                  ZeroBits,             // Set to NULL
        IN 	SIZE_T                  StackSize,            // Set to NULL
        IN 	SIZE_T                  MaximumStackSize,     // Set to NULL
        IN 	PVOID                   AttributeList         // set to NULL
);


typedef NTSTATUS (NTAPI* NtQueueApcThread)(
        IN HANDLE                       ThreadHandle,
        IN PIO_APC_ROUTINE              ApcRoutine,
        IN OPTIONAL PVOID               ApcRoutineContex,
        IN OPTIONAL PIO_STATUS_BLOCK    ApcStatusBlock,
        IN OPTIONAL ULONG               ApcReserve
);


#endif //NTAPI_H
