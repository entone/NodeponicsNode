#ifndef AES_h
#define AES_h

#include "application.h"

void aes_128_encrypt(char *value, unsigned char key[16], unsigned char output[256]);

#endif
