/*
 * Project 1 of COMP389: Cryptography
 * Function: Header File of fns.c
 * File Name: fns.h
 */

#ifndef FNS_H
#define FNS_H

typedef size_t usize;
typedef uint8_t u8;
typedef uint32_t u32;
/*
 * read from the specified file or stdin and do hexdump
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/
int hexdump(void *buffer, usize size);

/*
 * read from the specified file or stdin and do enc-base64
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/
int enc_base64(const char *input, const unsigned long input_length);

/*
 * read from the specified file or stdin and do dec-base64
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/
int dec_base64(const char *input,  unsigned int input_length);


#endif
