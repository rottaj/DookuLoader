//
// Created by j4ck on 10/15/23.
//

#include "ArgHandler.h"
#include <stdio.h>
#include <wchar.h>
#include <windows.h>
#include "../Core/Dooku.h"

BOOL HandleCommandLineArguments(int argc, wchar_t *argv[]) {
    if (wcscmp(argv[1], L"-url") == 0) {
        // Execute main function pass url(argv[2])
        if (!PleasureOfYouToJoinUs(argv[2])) {
            wprintf(L"[!] Failed To Join Us! Url!! \n");
            return -1;
        }
        if(!WelcomeToTheDarkSide()) {
            wprintf(L"[!] BLOP!\n");
            return -1;
        }
    }
    else if (wcscmp(argv[1], L"-encrypt") == 0){
        // Execute encryption function pass filepath(argv[2])
    }
    else {
        return FALSE;
    }
}
