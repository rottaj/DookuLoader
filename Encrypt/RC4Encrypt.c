//
// Created by j4ck on 10/15/23.
//
#include <stdio.h>
#include <stdlib.h>
#include "../Core/Encrypt.h"
#include <windows.h>

// Initialize variables in .data
const unsigned char key[11] = {"testing123"};


void Encrypt(unsigned char* source, unsigned char* dest, size_t payloadSize) {
    Rc4Context ctx = { 0 };

    rc4Init(&ctx, key, sizeof(key));

    rc4Cipher(&ctx, source, dest, payloadSize);
}


void WriteEncryptedPayloadToFile(unsigned char* source, size_t payloadSize) {
    FILE *fp;
    fp = fopen("output.bin", "wb");
    size_t bytesWritten = fwrite(source, payloadSize, 1, fp);
    fclose(fp);
}


BOOL RC4Encrypt(wchar_t *filePath) {
    size_t payloadSize = 0;
    unsigned char *source = NULL;
    FILE *fp = fopen(filePath, "rb");
    if (fp != NULL) {
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) {
            /* Get the size of the file. */
            long bufsize = ftell(fp);
            if (bufsize == -1) { /* Error */ }
            printf("File Size: %d\n", bufsize);
            /* Allocate our buffer to that size. */
            source = malloc(sizeof(unsigned char) * (bufsize + 1));

            printf("Allocated Buffer at address: %p Size %d\n", source, sizeof(unsigned char) * (bufsize + 1));
            payloadSize = sizeof(unsigned char) * (bufsize + 1);
            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

            /* Read the entire file into memory. */
            size_t newLen = fread(source, sizeof(unsigned char), bufsize, fp);
            if (ferror(fp) != 0) {
                fputs("Error reading file", stderr);
            } else {
                source[newLen++] = '\0'; /* Just to be safe. */
            }
        }
        fclose(fp);
    }

    unsigned char *dest = malloc(payloadSize);

    Encrypt(source, dest, payloadSize);

    printf("[+] Encrypted Payload.");
    WriteEncryptedPayloadToFile(dest, payloadSize);

    free(source); /* Don't forget to call free() later! */
    return TRUE;
}