#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/hdreg.h>      //for struct hd_driveid
#include <stdlib.h>
#include <math.h>

#define MAX_STRING_LENGTH 41
#define BIT_ARRAY_LENGTH 16

char* getBitArray();

int main() {
  struct hd_driveid hdd;
  int hddFile;

  if ((hddFile = open("/dev/sda", O_RDONLY | O_NONBLOCK)) < 0) {  //open HDD information file
      puts("ERROR: Cannot open device file /dev/sda");
      return -1;
  }
  if (ioctl(hddFile, HDIO_GET_IDENTITY, &hdd)) {                  //execute HDIO_GET_IDENTITY command
    puts("ERROR: Cannot execute HDIO_GET_IDENTITY command");
    return -1;
  }

  

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
