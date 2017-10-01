#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/hdreg.h>
#include <stdlib.h>
#include <math.h>

#define MAX_STRING_LENGTH 41
#define BIT_ARRAY_LENGTH 16

char* getBitArray();

int main() {
  return 0;
}


/**
* This function represents an input number in binary format
* @return a string with a binary respesentation
*/
char* getBitArray(unsigned short number) {
  char* bitArray = (char*) malloc(BIT_ARRAY_LENGTH);
  int i;
  for (i = 0; i < BIT_ARRAY_LENGTH; i++) {
    int denominator = pow(2, BIT_ARRAY_LENGTH - 1 - i);
    bitArray[i] = number / denominator;
    number %= denominator;
  }
  return bitArray;
}
