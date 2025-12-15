#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>

#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname; // To store the src image name
    FILE *fptr_src_image;  // To store the address of the src image

    /* Secret File Info */
    char *secret_fname;       // To store the secret file name
    FILE *fptr_secret;        // To store the secret file address
    char extn_secret_file[5]; // To store the Secret file extension
    char secret_data[1];    // To store the secret data
    long size_secret_file;    // To store the size of the secret data
    int extn_size;
} DecodeInfo;

/* Encoding function prototype */

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_decode(DecodeInfo *decInfo);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

//Status magic_string_size(int size, DecodeInfo *decInfo);
/*decode extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* decode a byte into LSB of image data array */
char decode_byte_to_lsb(char *imageBuffer);
 
// decode a size to lsb
int decode_size_to_lsb(char *imageBuffer);


#endif
