//
// Created by j4ck on 10/13/23.
//

#include <wchar.h>
#include "Core/Dooku.h"


SITH_INSTANCE myInstance = { 0 };
PSITH_INSTANCE Instance = &myInstance;


int wmain() {
    if (!PleasureOfYouToJoinUs()) {
        wprintf(L"Anakin in turn disarmed him, severing his hands and holding his sabers at the count's neck. To Dooku's shock, his secret master then told Anakin to kill him.\n");
        return -1;
    }
    // Maybe do some random loading idk.
    if(!WelcomeToTheDarkSide()) {
        wprintf(L"BLOP!\n");
        return -1;
    }
}

