#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sodium.h>

#define ERROR 1

size_t hexLen(size_t bytes_len) {
  return bytes_len * 2 + 1;
}

int verify_self_signed(unsigned char* self_signed_pk) {
  return crypto_sign_verify_detached(
        self_signed_pk,
        self_signed_pk + crypto_sign_bytes(),
        crypto_sign_publickeybytes(),
        self_signed_pk + crypto_sign_bytes()
      );
}

int main(void) {
  if (sodium_init() == -1) {
    printf("Failed to initialize\n");
    return ERROR;
  }
  size_t pk_len = crypto_sign_publickeybytes();
  size_t sk_len = crypto_sign_secretkeybytes();
  unsigned char pk[pk_len];
  unsigned char sk[sk_len];

  if (crypto_sign_keypair(pk, sk) != 0) {
    printf("Failed to generate keypair\n");
    return ERROR;
  }
  char pk_hex[hexLen(pk_len)];
  char sk_hex[hexLen(sk_len)];
  if (sodium_bin2hex(pk_hex, hexLen(pk_len), pk, pk_len) == NULL) {
    printf("Failed to convert public key to hex string\n");
  }
  if (sodium_bin2hex(sk_hex, hexLen(sk_len), sk, sk_len) == NULL) {
    printf("Failed to convert secret key to hex string\n");
  }
  unsigned char self_signed_pk[crypto_sign_bytes() + pk_len];
  unsigned long long self_signed_pk_len;
  if (crypto_sign(self_signed_pk, &self_signed_pk_len, pk, pk_len, sk) != 0) {
      printf("Failed to sign public key\n");
  }
  char self_signed_pk_hex[hexLen(self_signed_pk_len)];
  if (sodium_bin2hex(self_signed_pk_hex, hexLen(self_signed_pk_len),
        self_signed_pk + crypto_sign_bytes(), self_signed_pk_len) == NULL) {
    printf("Failed to convert public key to hex string\n");
  }
  printf("Self-signed Public Key: %s\n", self_signed_pk_hex);

  if (verify_self_signed(self_signed_pk) == 0) {
    printf("Self-signed: VERIFIED\n");
  } else {
    printf("Self-signed: UNVERIFIED\n");
  }
  return 0;
}
