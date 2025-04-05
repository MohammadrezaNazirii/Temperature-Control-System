#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "aes.h"

uint8_t key[17] = "thisisakey123456"; // 16-byte AES key
uint8_t encrypted[16];                // Set this to the actual encrypted block from transmitter
uint16_t password;

void decrypt_password(uint8_t *encrypted, uint8_t *key, uint16_t *password)
{
    uint8_t input[16] = {0};      // AES block size is 16 bytes
    memcpy(input, encrypted, 16); // Copy the full block

    struct AES_ctx ctx;
    AES_init_ctx(&ctx, key);
    AES_ECB_decrypt(&ctx, input);

    // Since the original password was 2 bytes, we read those bytes
    memcpy(password, input, sizeof(uint16_t));
}