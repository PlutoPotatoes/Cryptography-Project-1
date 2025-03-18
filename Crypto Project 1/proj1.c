/*
 * Project 1 of COMP389: Cryptography
 * Function: commandline parsing and file reading
 * File Name: proj1.c
 * Authors: Ryan Morrell & Danny Kim
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> 
#include <io.h>

#include "fns.h"

#define MAX_BUFF_SIZE 1024 * 500 // kilobyte * n

typedef size_t usize;
typedef uint8_t u8;
typedef uint32_t u32;


usize file_open(char *filepath, char *buffer, usize size){
  //make sure file exists
  if(access(filepath, F_OK) != 0){
    fprintf(stderr, "[Error] File '%s' does not exist\n", filepath);
    exit(1);
  }
  //make sure we can open the file
  FILE *f = fopen(filepath, "r");
  if(f == NULL){
    fprintf(stderr, "[Error] Could not open file '%s'\n", filepath);
    exit(1);
  }
  //get file size

  usize file_size;
  fseek(f, 0L, SEEK_END);
  file_size = ftell(f);
  fseek(f, 0L, SEEK_SET);

  if(file_size <= 0){
    fprintf(stderr, "[Error] '%s' has '%zu' bytes\n", filepath, file_size);
    exit(1);
  }else if(file_size > MAX_BUFF_SIZE){
    fprintf(stderr, "[Error] File size (%zu) is too large, max file size is %u\n", file_size, MAX_BUFF_SIZE);
    exit(1);
  }
  
  fread(buffer, sizeof(char), file_size, f);
  fclose(f);


  return file_size;
}

int getChars(char *filepath){

  FILE *f = fopen(filepath, "r");
  int count = 0;
  while(getc(f) != EOF){
    count++;
  }
  fclose(f);
  return count;
}


int main(int argc, char *argv[]){

  /*
    argv[0] = .exe file called
    argv[1] = encoding type as string
    argv[2] = file to be encoded
  */
  if(argc != 3){
    fprintf(stderr, "Usage %s <action> <filename>\n", argv[0]);
    exit(1);
  }

  char *filepath = argv[2];
  char buffer[MAX_BUFF_SIZE] = {0};
  usize file_size;


  //open the file and get its size

  file_size = file_open(filepath, buffer, MAX_BUFF_SIZE);


  //Do Yo Schtuff
  if(strcmp(argv[1], "hexdump")==0)
  {
    hexdump(buffer, file_size);
  }else if (strcmp(argv[1], "enc-base64")==0)  
  {
    enc_base64(buffer, file_size);
  }else if (strcmp(argv[1], "dec-base64")==0)
  {
    dec_base64(buffer, file_size);
  }else{
    fprintf(stderr, "[Error]: Invalid command type '%s'", argv[1]);
    exit(2);
  }

}


