#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/hdreg.h>      //for struct hd_driveid
#include <stdlib.h>
#include <math.h>

#define MAX_STRING_LENGTH 41
#define BIT_ARRAY_LENGTH 16

char* getBitArray(unsigned short number);
char* getATA(char* ataBits);
char* getInfoFromBitArray(char* bitArray);

int main() {
  struct hd_driveid hdd;
  int hddFile, i;
  char* tempString;

  if ((hddFile = open("/dev/sda", O_RDONLY | O_NONBLOCK)) < 0) {  //open HDD information file
      puts("ERROR: Cannot open device file /dev/sda");
      return -1;
  }
  if (ioctl(hddFile, HDIO_GET_IDENTITY, &hdd)) {                  //execute HDIO_GET_IDENTITY command
    puts("ERROR: Cannot execute HDIO_GET_IDENTITY command");
    return -1;
  }

  printf("Model:             %22.10s\n\n", hdd.model);
  printf("Firmware Revision: %32.20s\n\n", hdd.fw_rev);
  printf("Serial Numer:      %.20s\n\n", hdd.serial_no);
  tempString = getATA(getBitArray(hdd.major_rev_num));
  printf("Supported ATA standards: %16.20s\n\n", tempString);
  free(tempString);
  tempString = getInfoFromBitArray(getBitArray(hdd.dma_mword));
  printf("DMA:\n  mdma: %.20s\n\n", tempString);
  free(tempString);
  tempString = getInfoFromBitArray(getBitArray(hdd.dma_ultra));
  printf("  udma: %.20s\n\n", tempString);
  free(tempString);
  tempString = getInfoFromBitArray(getBitArray(hdd.eide_pio_modes));
  printf("PIO: %.20s\n\n", tempString);
  free(tempString);
  printf("Memory (GB): \n\ntotal:\n");
  system("df | grep '^/dev/[hs]d' | awk '{s+=$2} END {print s/1048576}'");
  printf("\nfree:\n");
  system("df | grep '^/dev/[hs]d' | awk '{s+=$3} END {print s/1048576}'");
  printf("\navailable:\n");
  system("df | grep '^/dev/[hs]d' | awk '{s+=$4} END {print s/1048576}'");

  return 0;
}


/**
* This function conver a bit sequence to a string with supported ATA standarts
* @return supported ATA standards
*/
char* getATA(char* ataBits) {
  int i, j = 0;
  char* ataStandards = (char*) malloc(MAX_STRING_LENGTH);
  for (i = 4; i < BIT_ARRAY_LENGTH; i++) {
    if (ataBits[i] == 1) {
      ataStandards[j++] = i + '0';
      ataStandards[j++] = ' ';
    }
  }
  ataStandards[j] = '\0';
  free(ataBits);
  return (char*) ataStandards;
}


/**
* This function convert a bit sequence to a string. Used for DMA and PIO
* @return a string with supported DMA or PIO
*/
char* getInfoFromBitArray(char* bitArray) {
  char* info = (char*) malloc(MAX_STRING_LENGTH);
  int i, j = 0;
  for (i = 0; i < 8; i++) {
    if (bitArray[i]) {
      info[j++] = i + '0';
      info[j++] = ' ';
    }
  }
  free(bitArray);
  info[j] = '\0';
  return (char*) info;
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
    bitArray[BIT_ARRAY_LENGTH - 1 - i] = number / denominator;
    number %= denominator;
  }
  return bitArray;
}
