#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
//#include "common.h"

/* ============================================================
 * Function : read_and_validate_decode_args
 * Purpose  : To validate command-line arguments for decoding
 * ============================================================ */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (argv[2] == NULL)
    {
        printf("Error: Missing arguments\n");
        return e_failure;
    }
    if ((argv[2][0] != '.'))
    {
        if ((strstr(argv[2], ".bmp") != NULL))
        {
            decInfo->src_image_fname = argv[2];
            if (argv[3][0] != '.')
            {
                if ((strstr(argv[3], ".txt") != NULL) || (strstr(argv[3], ".c") != NULL) ||
                    (strstr(argv[3], ".sh") != NULL) || (strstr(argv[3], ".h") != NULL))
                {
                    decInfo->secret_fname = argv[3];
                    strncpy(decInfo->extn_secret_file, strstr(argv[3], "."), sizeof(decInfo->extn_secret_file) - 1);
                    decInfo->extn_secret_file[sizeof(decInfo->extn_secret_file) - 1] = '\0'; 
                    return e_success;
                }
                else
                {
                    argv[3] = "Decode_secret.txt"; // default file
                    decInfo->secret_fname = argv[3];
                    return e_success;
                }
            }
        }
    }
    printf("Error: Invalid arguments\n");
    return e_failure;
}

/* ============================================================
 * Function : open_files_decode
 * Purpose  : To open source BMP file and output secret file
 * ============================================================ */
Status open_files_decode(DecodeInfo *decInfo)
{
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "rb");
    if (decInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);
        return e_failure;
    }

    fseek(decInfo->fptr_src_image, 54, SEEK_SET); // skip BMP header

    decInfo->fptr_secret = fopen(decInfo->secret_fname, "wb");
    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
        return e_failure;
    }

    return e_success;
}

/* ============================================================
 * Function : decode_magic_string
 * Purpose  : To decode and verify the embedded magic string
 * ============================================================ */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    char buffer[8];
    char magic_buffer[strlen(magic_string) + 1];

    printf("INFO: Decoding magic string...\n");

    for (int i = 0; magic_string[i] != '\0'; i++)
    {
        fread(buffer, sizeof(char), 8, decInfo->fptr_src_image);
        char secret_data = decode_byte_to_lsb(buffer);
        magic_buffer[i] = secret_data;
    }
    
    magic_buffer[strlen(magic_string)] = '\0';
    for(int i=0;magic_buffer[i] != '\0';i++){
        printf("%c\n",magic_buffer[i]);
    }
    if (strcmp(magic_string, magic_buffer) == 0)
    {
        printf("INFO: Magic string verified successfully.\n");
        return e_success;
    }
    printf("ERROR: Magic string mismatch.\n");
    return e_failure;
}

/* ============================================================
 * Function : decode_secret_file_extn_size
 * Purpose  : To decode the size of the secret file extension
 * ============================================================ */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char imageBuffer[32];
    fread(imageBuffer, 32, 1, decInfo->fptr_src_image);
    decInfo->extn_size = decode_size_to_lsb(imageBuffer);
    return e_success;
}

/* ============================================================
 * Function : decode_secret_file_extn
 * Purpose  : To decode and write the secret file extension
 * ============================================================ */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char imageBuffer[8];
    char decoded_extn[10]; // Store decoded extension temporarily

    for (int i = 0; i < decInfo->extn_size; i++)
    {
        fread(imageBuffer, 8, 1, decInfo->fptr_src_image);
        char data = decode_byte_to_lsb(imageBuffer);
        decoded_extn[i] = data;
    }
    decoded_extn[decInfo->extn_size] = '\0';

    printf("INFO: Decoded extension = '%s'\n", decoded_extn);

    // Compare decoded extension with user's given extension
    if (strcmp(decoded_extn, decInfo->extn_secret_file) != 0)
    {
        fprintf(stderr, "ERROR: Extension mismatch! Encoded: '%s', Expected: '%s'\n",
                decoded_extn, decInfo->extn_secret_file);

        // Close the wrong file pointer (user-given file)
        fclose(decInfo->fptr_secret);

        // ✅ Create a new output file with the decoded extension
        char new_filename[50];
        strcpy(new_filename, "decoded_output");
        strcat(new_filename, decoded_extn);

        decInfo->fptr_secret = fopen(new_filename, "w");
        if (decInfo->fptr_secret == NULL)
        {
            fprintf(stderr, "ERROR: Could not create new file '%s'\n", new_filename);
            return e_failure;
        }

        printf("INFO: Created new file '%s' with decoded extension.\n", new_filename);
        // Continue decoding using this new file
    }
    else
    {
        printf("INFO: File extension verified successfully.\n");
    }

    // Write the decoded extension (optional for record)
    fwrite(decoded_extn, sizeof(char), decInfo->extn_size, decInfo->fptr_secret);

    return e_success;
}

/* ============================================================
 * Function : decode_secret_file_size
 * Purpose  : To decode the size of the secret data
 * ============================================================ */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char imageBuffer[32];
    fread(imageBuffer, 32, 1, decInfo->fptr_src_image);
    decInfo->size_secret_file = decode_size_to_lsb(imageBuffer);
    printf("%ld\n",decInfo->size_secret_file);
    return e_success;
}

/* ============================================================
 * Function : decode_secret_file_data
 * Purpose  : To decode the actual secret file data
 * ============================================================ */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char imageBuffer[8];
    printf("%ld\n",decInfo->size_secret_file);
    for (int i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(imageBuffer, 8, 1, decInfo->fptr_src_image);
        char data = decode_byte_to_lsb(imageBuffer);
        fwrite(&data, sizeof(char), 1, decInfo->fptr_secret);
    }
    return e_success;
}

/* ============================================================
 * Function : decode_byte_to_lsb
 * Purpose  : To extract 1 byte of data from 8 LSBs
 * ============================================================ */
char decode_byte_to_lsb(char *imageBuffer)
{
    char data = 0;
    for (int i = 0; i < 8; i++)
        data |= ((imageBuffer[i] & 1) << i);
    return data;
}
/* ============================================================
 * Function : decode_size_to_lsb
 * Purpose  : To extract a 32-bit integer value from LSBs
 * ============================================================ */
int decode_size_to_lsb(char *imageBuffer)
{
    int data = 0;
    for (int i = 0; i < 32; i++)
        data |= ((imageBuffer[i] & 1) << i);
    return data;
}


/* ============================================================
 * Function : do_decoding
 * Purpose  : Master controller for decoding process
 * ============================================================ */
Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO: Starting decoding process...\n");

    /* Step 1: Open required files */
    if (open_files_decode(decInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to open required files for decoding.\n");
        return e_failure;
    }
    printf("INFO: Files opened successfully.\n");

    /* Step 2: Decode the magic string */
    char MAGIC_STRING[20];
    printf("Enter the Magic Sting Password :");
    scanf("%s",MAGIC_STRING);
    if (decode_magic_string(MAGIC_STRING, decInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Magic string mismatch. File may not contain encoded data.\n");
        fclose(decInfo->fptr_src_image);
        return e_failure;
    }
    printf("INFO: Magic string verified successfully.\n");

    /* Step 3: Decode the secret file extension size */
    if (decode_secret_file_extn_size(decInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to decode secret file extension size.\n");
        fclose(decInfo->fptr_src_image);
        return e_failure;
    }

    /* Step 4: Decode the secret file extension */
    if (decode_secret_file_extn(decInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to decode secret file extension.\n");
        fclose(decInfo->fptr_src_image);
        return e_failure;
    }

    /* Step 5: Decode the secret file size */
    if (decode_secret_file_size(decInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to decode secret file size.\n");
        fclose(decInfo->fptr_src_image);
        return e_failure;
    }

    /* Step 6: Decode the secret file data */
    if (decode_secret_file_data(decInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to decode secret file data.\n");
        fclose(decInfo->fptr_src_image);
        fclose(decInfo->fptr_secret);
        return e_failure;
    }

    /* Step 7: Close all files */
    fclose(decInfo->fptr_src_image);
    fclose(decInfo->fptr_secret);

    printf("INFO: Decoding completed successfully.\n");
    return e_success;
}
