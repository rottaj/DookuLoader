//
// Created by j4ck on 10/14/23.
//

#ifndef WHOMTHESHELLTOLLS_ENCRYPT_H
#define WHOMTHESHELLTOLLS_ENCRYPT_H

#include <windows.h>
#include <winternl.h>

typedef struct
{
    unsigned int i;
    unsigned int j;
    unsigned char s[256];

} Rc4Context;

typedef NTSTATUS(NTAPI* fnSystemFunction033)(
        struct USTRING* Data,
        struct USTRING* Key
);


void rc4Init(Rc4Context* context, const unsigned char* key, size_t length);
void rc4Cipher(Rc4Context* context, const unsigned char* input, unsigned char* output, size_t length);

#endif //WHOMTHESHELLTOLLS_ENCRYPT_H

