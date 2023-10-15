//
// Created by j4ck on 10/13/23.
//

#include <wchar.h>
#include "Core/Dooku.h"
#include "Cmd/ArgHandler.h"
#include <stdio.h>

SITH_INSTANCE myInstance = { 0 };
PSITH_INSTANCE Instance = &myInstance;


int wmain(int argc, wchar_t *argv[]) {
    if (argc < 3) {
        // Print Command Line Options
        wprintf(L"[!] Insufficient Arguments Passed");
        return -1;
    }
    if (!HandleCommandLineArguments(argc, argv)) {
        wprintf(L"[!] Please Enter Valid Commands");
        return -1;
    }

    getchar();
}

