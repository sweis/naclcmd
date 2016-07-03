#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sodium.h>

int main(void) {
  if (sodium_init() == -1) {
    printf("FAIL\n");
    return 1;
  }
  printf("SUCCESS\n");
  return 0;
}
