#include "application.h"
#include "libraries/tropicssl/aes.h"
#include "libraries/tropicssl/rsa.h"
#include "AES.h"
// PKCS #7 padding
// Do this before encrypting to get the message
// up to a multiple of 16 bytes.
size_t pad(unsigned char *buf, size_t messageLength) {
    size_t paddedLength = (messageLength & ~15) + 16;
    char pad = paddedLength - messageLength;
    memset(buf + messageLength, pad, pad);
    return paddedLength;
}

void sixteenRandomBytes(unsigned char buf[16]) {
    for (int i = 0; i < 16; i++) {
        buf[i] = rand() & 0xff;
    }
}

void slice(unsigned char dest[256], unsigned char original[256], int start, int end){
    int counter = 0;
    for(int i=start; i<end; i++){
        dest[counter] = original[i];
        counter++;
    }
}

void append(unsigned char dest[256], unsigned char original[256], int d_len, int o_len){
    int counter = d_len;
    for(int i=0; i<o_len; i++){
        dest[counter] = original[i];
        counter++;
    }
}

void print_char(unsigned char msg[256], size_t len){
    for(int i=0; i<len; i++){
        Serial.write(msg[i]);
    }
    Serial.println("");
}

void print_char(char msg[256], size_t len){
    for(int i=0; i<len; i++){
        Serial.write(msg[i]);
    }
    Serial.println("");
}

void aes_128_encrypt(char *value, unsigned char key[16], unsigned char output[256]){
    unsigned char buf[240];
    unsigned char IV[16];
    //generate random IV
    sixteenRandomBytes(IV);
    //copy original IV into final output
    memcpy(output, IV, 16);

    char original[240];
    String s = String(value);
    int length = s.length()+1;
    Serial.print("Data Length: ");
    Serial.println(length);
    s.toCharArray(original, length);
    memcpy(buf, original, length);
    size_t paddedLength = pad(buf, length);
    Serial.print("Padded Length: ");
    Serial.println(paddedLength);
    aes_context aes;
    aes_setkey_enc(&aes, key, 128);
    aes_crypt_cbc(&aes, AES_ENCRYPT, paddedLength, IV, buf, buf);
    append(output, buf, 16, paddedLength);
}
