#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
//#include "common.h"

/* ============================================================
 * Function: get_image_size_for_bmp
 * Purpose : To calculate total image capacity in bytes
 * Input   : FILE pointer to BMP image
 * Output  : Returns total image size (width * height * 3)
 * ============================================================ */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Move file pointer to BMP width position (offset 18)
    fseek(fptr_image, 18, SEEK_SET);

    // Read width (4 bytes)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read height (4 bytes)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Calculate total pixel bytes (3 bytes per pixel for 24-bit)
    return width * height * 3;
}

/* ============================================================
 * Function: get_file_size
 * Purpose : To get the total size of secret file
 * ============================================================ */
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr); // Get file size in bytes
    fseek(fptr, 0, SEEK_SET);
    return size;
}

/* ============================================================
 * Function: read_and_validate_encode_args
 * Purpose : Validate command line arguments and assign filenames
 * ============================================================ */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Check for missing arguments
    if (argv[2] == NULL || argv[3] == NULL)
    {
        printf("Error: Missing arguments\n");
        return e_failure;
    }

    // Validate source BMP file
    if (argv[2][0] != '.')
    {
        if (strstr(argv[2], ".bmp") != NULL)
        {
            encInfo->src_image_fname = argv[2];

            // Validate secret file
            if (argv[3][0] != '.')
            {
                if((strstr(argv[3], ".txt") != NULL) || (strstr(argv[3], ".c") != NULL) ||
                    (strstr(argv[3], ".sh") != NULL) || (strstr(argv[3], ".h") != NULL))
                {
                    encInfo->secret_fname = argv[3];
                    strncpy(encInfo->extn_secret_file, strstr(argv[3], "."), sizeof(encInfo->extn_secret_file) - 1);
                    encInfo->extn_secret_file[sizeof(encInfo->extn_secret_file) - 1] = '\0'; 
                    // Default output file if not given
                    if (argv[4] == NULL)
                    {
                        argv[4] = "destination.bmp";
                        encInfo->stego_image_fname = argv[4];
                        return e_success;
                    }
                    else
                    {
                        // Validate destination BMP file
                        if (argv[2][0] != '.')
                        {
                            if (strstr(argv[4], ".bmp") != NULL)
                            {
                                encInfo->stego_image_fname = argv[4];
                                return e_success;
                            }
                        }
                    }
                }
            }
        }
    }
    printf("Error: Invalid arguments\n");
    return e_failure;
}

/* ============================================================
 * Function: open_files
 * Purpose : Open source, secret, and destination files
 * ============================================================ */
Status open_files(EncodeInfo *encInfo)
{
    // Open source BMP
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
        return e_failure;
    }

    // Open secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
        return e_failure;
    }

    // Open output stego image
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
        return e_failure;
    }

    return e_success;
}

/* ============================================================
 * Function: check_capacity
 * Purpose : Check if image has enough capacity for encoding
 * ============================================================ */
Status check_capacity(EncodeInfo *encInfo)
{
    int total_bytes = 0;

    // Get total image size
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    printf("Image capacity = %u bytes\n", encInfo->image_capacity);

    // Get secret file size
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    printf("Secret file size = %ld bytes\n", encInfo->size_secret_file);

    // Calculate required bytes for encoding
    total_bytes = 54 + (strlen(encInfo->magic_string_data) * 8) + 32 +(strlen(encInfo->extn_secret_file) * 8) +32 + (encInfo->size_secret_file * 8);

    // Compare required vs available
    if (encInfo->image_capacity < total_bytes)
    {
        fprintf(stderr, "ERROR: Image capacity is not sufficient to hold the secret file.\n");
        fprintf(stderr, "Required capacity: %d bytes, Available capacity: %u bytes\n", total_bytes, encInfo->image_capacity);
        return e_failure;
    }

    printf("INFO: Image has enough capacity for encoding.\n");
    return e_success;
}

/* ============================================================
 * Function: copy_bmp_header
 * Purpose : Copy 54-byte BMP header from source to output
 * ============================================================ */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char header[54];
    rewind(fptr_src_image);

    // Read BMP header (54 bytes)
    size_t bytes_read = fread(header, sizeof(char), 54, fptr_src_image);
    if (bytes_read != 54)
    {
        fprintf(stderr, "ERROR: Unable to read BMP header from source image.\n");
        return e_failure;
    }

    // Write BMP header to destination
    size_t bytes_written = fwrite(header, sizeof(char), 54, fptr_dest_image);
    if (bytes_written != 54)
    {
        fprintf(stderr, "ERROR: Unable to write BMP header to stego image.\n");
        return e_failure;
    }

    // Verify pointer positions match
    if (ftell(fptr_src_image) == ftell(fptr_dest_image))
    {
        return e_success;
    }
    return e_failure;
}

/* ============================================================
 * Function: encode_magic_string
 * Purpose : Embed a predefined identifier to verify decoding
 * ============================================================ */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char buffer[8];
    size_t bytes_read, bytes_written;

    printf("INFO: Encoding magic string...\n");

    for (int i = 0; magic_string[i] != '\0'; i++)
    {
        // Read 8 bytes from source image
        bytes_read = fread(buffer, sizeof(char), 8, encInfo->fptr_src_image);
        if (bytes_read != 8)
        {
            fprintf(stderr, "ERROR: Failed to read 8 bytes from source image.\n");
            return e_failure;
        }

        // Modify LSBs with character bits
        encode_byte_to_lsb(magic_string[i], buffer);

        // Write modified bytes to stego image
        bytes_written = fwrite(buffer, sizeof(char), 8, encInfo->fptr_stego_image);
        if (bytes_written != 8)
        {
            fprintf(stderr, "ERROR: Failed to write 8 bytes to stego image.\n");
            return e_failure;
        }
    }
    if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image))
    {
        return e_success;
    }
    return e_failure;
    return e_success;
}

/* ============================================================
 * Function: encode_secret_file_extn_size
 * Purpose : Store the length of file extension
 * ============================================================ */
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char img_buffer[32];
    fread(img_buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(size, img_buffer);
    fwrite(img_buffer, 32, 1, encInfo->fptr_stego_image);
    if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image))
        return e_success;
    return e_failure;
}

/* ============================================================
 * Function: encode_secret_file_extn
 * Purpose : Embed the secret file extension
 * ============================================================ */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char Ex_buffer[8];
    
    for (int i = 0; file_extn[i] != '\0'; i++)
    {
        fread(Ex_buffer, sizeof(char), 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(file_extn[i], Ex_buffer);
        fwrite(Ex_buffer, sizeof(char), 8, encInfo->fptr_stego_image);
    }
    if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image))
        return e_success;
    return e_failure;
}
/* ============================================================
 * Function: encode_secret_file_size
 * Purpose : Store size of secret file in image
 * ============================================================ */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char img_buffer[32];
    fread(img_buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(file_size, img_buffer);
    fwrite(img_buffer, 32, 1, encInfo->fptr_stego_image);
    if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image))
        return e_success;
    return e_failure;
}

/* ============================================================
 * Function: encode_secret_file_data
 * Purpose : Embed actual data of secret file
 * ============================================================ */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[8];
    while (fread(encInfo->secret_data, sizeof(char), 1, encInfo->fptr_secret))
    {
        fread(buffer, sizeof(char), 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(encInfo->secret_data[0], buffer);
        fwrite(buffer, sizeof(char), 8, encInfo->fptr_stego_image);
    }
    return e_success;
}

/* ============================================================
 * Function: copy_remaining_img_data
 * Purpose : Copy remaining bytes of image unchanged
 * ============================================================ */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, sizeof(char), sizeof(buffer), fptr_src)) > 0)
        fwrite(buffer, sizeof(char), bytes, fptr_dest);
    return e_success;
}

/* ============================================================
 * Function: encode_byte_to_lsb
 * Purpose : Replace 8 LSBs of 8 image bytes with 8 bits of data
 * ============================================================ */
Status encode_byte_to_lsb(char data, char *imageBuffer)
{
    for (int i = 0; i < 8; i++)
        imageBuffer[i] = (imageBuffer[i] & (~1)) | ((data >> i) & 1);
    return e_success;
}

/* ============================================================
 * Function: encode_size_to_lsb
 * Purpose : Store 32-bit integer into 32 image bytes
 * ============================================================ */
Status encode_size_to_lsb(int size, char *imageBuffer)
{
    for (int i = 0; i < 32; i++)
        imageBuffer[i] = (imageBuffer[i] & (~1)) | ((size >> i) & 1);
    return e_success;
}

/* ============================================================
 * Function: do_encoding
 * Purpose : Main encoding workflow controller
 * ============================================================ */
Status do_encoding(EncodeInfo *encInfo)
{
    printf("INFO: Starting encoding process...\n");
    char temp_magic[20];
    printf("Enter the Magic String Password: ");
    scanf("%s", temp_magic);
    strcpy(encInfo->magic_string_data, temp_magic);

    // Step 1: Open all necessary files
    if (open_files(encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Opening files failed.\n");
        return e_failure;
    }
    printf("INFO: Files opened successfully.\n");

    // Step 2: Validate image capacity
    if (check_capacity(encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Image does not have enough capacity.\n");
        return e_failure;
    }

    // Step 3: Copy BMP header
    printf("INFO: Copying BMP header...\n");
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to copy BMP header.\n");
        return e_failure;
    }

    // Step 4: Encode magic string
    printf("INFO: Encoding magic string...\n");
    if (encode_magic_string(encInfo->magic_string_data, encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to encode magic string.\n");
        return e_failure;
    }

    // Step 5: Encode file extension size
    printf("INFO: Encoding secret file extension size...\n");
    int extn_size = strlen(encInfo->extn_secret_file);
    printf("DEBUG: extn_secret_file = '%s'\n", encInfo->extn_secret_file);
    if (encode_secret_file_extn_size(extn_size, encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to encode secret file extension size.\n");
        return e_failure;
    }
    // Step 6: Encode file extension
    printf("INFO: Encoding secret file extension...\n");
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to encode secret file extension.\n");
        return e_failure;
    }
    

    // Step 7: Encode file size
    printf("INFO: Encoding secret file size...\n");
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to encode secret file size.\n");
        return e_failure;
    }

    // Step 8: Encode file data
    printf("INFO: Encoding secret file data...\n");
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to encode secret file data.\n");
        return e_failure;
    }

    // Step 9: Copy remaining data
    printf("INFO: Copying remaining image data...\n");
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to copy remaining image data.\n");
        return e_failure;
    }

    printf("INFO: Encoding completed successfully!\n");

    // Close all open files
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);

    return e_success;
}
