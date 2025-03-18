/*
 * Project 1 of COMP389: Cryptography
 * Function: hexdump, enc-base64 and dec-base64 handling
 * File Name: fns.c
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdint.h> 
 #include <io.h>
 #include <stdbool.h>
 #include <readline/readline.h>
 #include <readline/history.h>
 


 
 #include "fns.h"
 
 #define MAX_BUFF_SIZE 1024 * 500 // kilobyte * n
 
 typedef size_t usize;
 typedef uint8_t u8;
 typedef uint32_t u32;

 //base64 char array: BASE64[int] = base64 char representation
 char *BASE64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

 //transforms 8 bit base64 values back into the 6 original bits they came from
 
 int UNBASE64[] = {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0-11
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 12-23
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 24-35
  -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, // 36-47
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -2, // 48-59
  -1,  0, -1, -1, -1,  0,  1,  2,  3,  4,  5,  6, // 60-71
  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, // 72-83
  19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 84-95
  -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, // 96-107
  37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, // 108-119
  49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 120-131
};


/*
 * read from the specified file or stdin and do hexdump
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/
int hexdump(void *buffer, usize size){
  u8 *data = buffer;
  u32 i, j;
  
  for(i = 0; i < size; i++){
    //print address for new line
    if(i % 16 == 0){
      printf("%08x ", i);
    }

    //extra space every 8 lines
    if(i % 8 == 0){
     printf(" ");
    }

    //print individual bytes
    u8 byte = data[i];
    printf("%02x ", byte);

    //end of row
    if(((i%16)==15) || (i==size-1)){
      //pad extra -- as needed
      for(j = 0; j < 15-(i%16); j++){
        if((j+i)%8==0){
          printf(" ");
        }
        printf("-- ");
      }

      //print ascii if possible or "." if not
      printf(" |");
      for(j = (i-(i%16)); j <= i; j++){
        u8 ascii_byte = data[j];
        printf("%c", (ascii_byte > 31 && ascii_byte < 127) ? ascii_byte : '.');
      }
      printf("|\n");
    }
  }
  return 1;
}

/*
 * read from the specified file or stdin and do enc-base64
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/
int enc_base64(const char *input, const unsigned long input_length){
  int input_index = 0;
  int count = 0;

  //TODO add counter, newline every 64 characters printed
  while(input_index < input_length){
    //process 3 bytes per cycle
    char currSlice;

    // First Slice
    // input & 11111100, then shift 2 to the right
    // gives 00XXXXXX Xs are the original leftmost bits
    currSlice = BASE64[(input[input_index] & 0xFC) >> 2];
    printf("%c", currSlice);
    count++;
    if(count % 64 == 0){
      printf("\n");
    }

    //handle padding case here
    if(input_index+1 == input_length){
      //take care of remaining bits, add padding
      currSlice = BASE64[(input[input_index] & 0x03) << 4];
      printf("%c==", currSlice);
      break;
    }

    // Second Slice
    // take only the last two bits of the previous slice shifted to the left
    // concat with first 4 from next slice shifted to right 
    currSlice = BASE64[((input[input_index] & 0x3)<<4) | ((input[input_index+1] & 0xF0)>>4)];
    printf("%c", currSlice);
    count++;
    if(count % 64 == 0){
      printf("\n");
    }
    
    if(input_index+1 == input_length){
      //take care of remaining bits, add padding
      currSlice = BASE64[(input[input_index] & 0x0F) << 2];
      printf("%c=", currSlice);
      break;
    }

    //Third Slice
    currSlice = BASE64[((input[input_index + 1] & 0x0F)<<2) | ((input[input_index+2] & 0xC0)>>6)];
    printf("%c", currSlice);
    count++;
    if(count % 64 == 0){
      printf("\n");
    }

    //Fourth Slice
    currSlice = BASE64[(input[input_index + 2] & 0x3F)];
    printf("%c", currSlice);

    input_index+=3;

    count++;
    if(count % 64 == 0){
      printf("\n");
    }

  }
  return 1;
}



int dec_base64(const char *input, unsigned int input_length){
  int input_index = 0;
  int spaceIndex = 64;

  //while bytes left in file, read 16 to buffer (malloc), 
  /*
  if(input_length % 4 != 0){
    fprintf(stderr, "[Error]: Invalid base 64 input, length (%u) not even multiple of 4\n", input_length);
    return 1;
  }
  */


  while(input_index < input_length){
    //collapse each 4 6-bit chunks back into 6 bytes

    //check for newline every 64 characters


    char first, second, third, fourth, currVal;

    //check if the next 4 bytes are properly encoded in base64
    //FIXME also getting errors here, no idea why
    /*
    for(int i = 0; i< 4; i++){
      int value = input[input_index+i];

      if((value < 0) || (value > 131) || (UNBASE64[value] == -1)){
        fprintf(stderr, "[ERROR]: Invalid base64 character, cannot decode: %c\n", value);
        return 0;
      }
    }
      */
      
    if(input_index==spaceIndex){
        input_index++;
        spaceIndex+=65;
    }
    
    first = UNBASE64[(int)input[input_index]];
    second = UNBASE64[(int)input[input_index+1]];
    third = UNBASE64[(int)input[input_index+2]];
    fourth = UNBASE64[(int)input[input_index+3]];

    //first byte
    currVal = (first << 2) | ((second & 0x30) >> 4); 
    printf("%c", currVal);


    //second byte
    if(input[input_index + 2]!= '='){
      currVal = ((second & 0xF) << 4) | ((third & 0x3C) >> 2); 
      printf("%c", currVal);

    }
    //third byte
    if(input[input_index + 3]!= '='){
      currVal = ((third & 0x03) << 6) | fourth; 
      printf("%c", currVal);

    }

    input_index+=4;



  }

  return 1; // FIXME
}
