#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sodium.h>

#define ERROR 1

size_t hexLen(size_t bytes_len) {
  return bytes_len * 2 + 1;
}

int verify_cert(unsigned char* cert) {
  return crypto_sign_verify_detached(
        cert,
        cert + crypto_sign_bytes(),
        crypto_sign_publickeybytes(),
        cert + crypto_sign_bytes()
      );
}

int genKeypair() {
  size_t pk_len = crypto_sign_publickeybytes();
  size_t sk_len = crypto_sign_secretkeybytes();
  unsigned char pk[pk_len];
  unsigned char sk[sk_len];
  unsigned long long cert_maxlen =
    crypto_sign_bytes() + crypto_sign_publickeybytes();
  unsigned char cert[cert_maxlen];
  size_t hex_len = hexLen(cert_maxlen);
  char hex[hex_len];

  if (crypto_sign_keypair(pk, sk) != 0) {
    printf("Failed to generate keypair\n");
    return ERROR;
  }
  unsigned long long cert_len;
  if (crypto_sign(cert, &cert_len, pk, pk_len, sk) != 0) {
      printf("Failed to sign public key\n");
      return ERROR;
  }
  if (cert_len != cert_maxlen) {
    return ERROR;
  }
  if (sodium_bin2hex(hex, hexLen(cert_len),
        cert, cert_len) == NULL) {
    printf("Failed to convert public key to hex string\n");
    return ERROR;
  }
  if (verify_cert(cert) == 0) {
    printf("%s\n", hex);
    return 0;
  } else {
    return ERROR;
  }
}

int main(int argc, char* argv[]) {
  if (sodium_init() == -1) {
    printf("Failed to initialize\n");
    return ERROR;
  }
  genKeypair();
}
