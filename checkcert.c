#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sodium.h>

typedef enum _return_codes {
  VALID_CERT = 0,
  LIBSODIUM_ERROR = 1,
  NO_FILE_NAME = 2,
  ERROR_OPENING_FILE = 3,
  INVALID_FILE_SIZE = 4,
  HEX_DECODING_ERROR = 5,
  INVALID_CERT = 6,
} ReturnCode;

<<<<<<< HEAD
=======
//#define ERROR 16
//#define ERROR 32

>>>>>>> 3cd70d533a25029742f1ef2664f82242c406f99d
/**
 * Format is:
 * [ Signature(Public Key) | Public Key ]
 */
int verify_cert(unsigned char* cert) {
  return crypto_sign_verify_detached(
        cert,
        cert + crypto_sign_bytes(),
        crypto_sign_publickeybytes(),
        cert + crypto_sign_bytes()
      );
}

int main(int argc, char* argv[]) {
  if (sodium_init() == -1) {
    return LIBSODIUM_ERROR;
  }
  unsigned long long cert_maxlen =
    crypto_sign_bytes() + crypto_sign_publickeybytes();
  size_t hex_len = cert_maxlen * 2 + 1;
  char hex[hex_len];

  size_t hex_chars_read = 0;
  if (argc == 1) {
    // If no arguments are provided, read from stdin
    hex_chars_read = fread(hex, sizeof(char), hex_len, stdin);
  } else if (argc == 2) {
    // If one argument is provided, try to open it as a file.
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
      int errnum = errno;
      fprintf(stderr, "Error opening file %s: %s\n",
        argv[1], strerror(errnum));
      return ERROR_OPENING_FILE;
    }
    hex_chars_read = fread(hex, sizeof(char), hex_len, input_file);
    fclose(input_file);
  } else {
    printf("Usage: %s [filename]\n", argv[0]);
    printf("If no filename provided, defaults to stdin\n");
    return NO_FILE_NAME;
  }

  if (hex_chars_read != hex_len) {
    fprintf(stderr, "Invalid cert size: %zu bytes read, %zu expected\n",
      hex_chars_read, hex_len);
    return INVALID_FILE_SIZE;
  }

  // Decode hex
  unsigned char cert[cert_maxlen];
  const char *hex_end;
  size_t bin_len;
  if (sodium_hex2bin(cert, cert_maxlen, hex, hex_len,
        NULL, &bin_len, &hex_end) != 0) {
    return HEX_DECODING_ERROR;
  }

  if (verify_cert(cert) == 0) {
    printf("VERIFIED\n");
    return VALID_CERT;
  } else {
    printf("UNVERIFIED\n");
    return INVALID_CERT;
  }
}
