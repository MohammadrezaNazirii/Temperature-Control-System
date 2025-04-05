#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "aes.h"

uint8_t keyy[17] = "thisisakey123456"; // 16-byte AES key
uint8_t encrypted[16];

void encrypt_password(uint16_t password, uint8_t *key, uint8_t *encrypted)
{
    uint8_t input[16] = {0}; // AES block size is 16 bytes
    memcpy(input, &password, sizeof(password));

    struct AES_ctx ctx;
    AES_init_ctx(&ctx, key);
    AES_ECB_encrypt(&ctx, input);

    // We will send the entire encrypted data (16 bytes)
    memcpy(encrypted, input, 16); // Copy the full block
}