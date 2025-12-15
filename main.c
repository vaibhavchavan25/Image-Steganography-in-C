/*
Name: Nakul Anil Vadar
Project Name: Steganography (Image-based Data Hiding)
Batch ID: 25021_325
Submission Date: 03/11/2025

Description:
This project performs image-based steganography, allowing users to hide (encode) and extract (decode) secret data inside a BMP image file.
It supports both encoding and decoding modes using command-line options.

Features:

* Encode a secret text file into a BMP image.
* Decode hidden data from an encoded BMP image.
* Validate file formats and check image capacity.
* Maintain original image quality after encoding.
* Display detailed process logs during execution.

Input:

* Command line arguments:

  1. `-e` for encoding or `-d` for decoding
  2. Source BMP file
  3. Secret or output text file

Example Commands:
$ ./a.out -e beautiful.bmp secret.txt
$ ./a.out -d destination.bmp decode.txt

Output:

* Informational messages about each step (file validation, encoding, decoding).
* Creates an encoded image file (destination.bmp).
* Generates decoded secret file (decode.txt).

*/


// ##  Output Analysis

// | Test   | Input Image     | Secret File | Output Image    | Result                                    |
// | ------ | --------------- | ----------- | --------------- | ----------------------------------------- |
// | Test 1 | beautiful.bmp   | secret.txt  | destination.bmp | Successful encoding                       |
// | Test 2 | destination.bmp | —           | decoded.txt     | Decoded data matches original secret file |

// ---

// ## Sample Output Screens

// **Encoding:**

// ```
// INFO: Encoding secret file size...
// INFO: Encoding secret file data...
// INFO: Copying remaining image data...
// INFO: Encoding completed successfully!
// ```

// **Decoding:**

// ```
// Decoded magic string: #***
// INFO: Magic string decoded successfully.
// INFO: Secret data decoded successfully.
// INFO: Decode successfully...
// ```

// ---

// ## Results and Discussion

// * Hidden data successfully embedded into the image.
// * Extracted data identical to original secret file.
// * Visual quality of image unchanged.
// * Demonstrates efficient bitwise manipulation and file handling in C.

// ---

// ## Limitations

// * Works only with **24-bit BMP** (uncompressed) images.
// * Limited by image capacity.
// * Does not use encryption (only conceals data).

// ---

// ## Future Enhancements

// * Add **encryption layer** for secure embedding.
// * Extend to **JPEG/PNG** image formats.
// * Build **GUI interface** (C++/Python).
// * Add **audio/video steganography** module.

// ---

// ## Conclusion

// This project successfully implements image-based steganography using C programming.
// It effectively demonstrates how secret data can be hidden and retrieved without visible distortion, showcasing practical use of file handling, bitwise operations, and modular programming principles.
// ---

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
OperationType check_operation_type(char *);

int main(int argc, char *argv[])
{
    if(argc >= 4){
        if(check_operation_type(argv[1]) == e_encode ){
            EncodeInfo encInfo;
            if(read_and_validate_encode_args(argv,&encInfo)== e_success){
                do_encoding (&encInfo);
            }
            else{
                printf("terminate the program");
                return 0;
            }
        }
        else if(check_operation_type(argv[1]) == e_decode){
            DecodeInfo decInfo;
            if(read_and_validate_decode_args(argv,&decInfo)== e_success){
                do_decoding (&decInfo);
            }
            else{
                printf("terminate the program");
                return 0;
            }
        }
        else{
            return 0;
        }
    }
    else{
        printf("Error. terminate the program");
        return 0;
    }
    // Step 1 : Check the argc >= 4 true - > step 2
    // Step 2 : Call the check_operation_type(argv[1]) == e_encode )) true - > step 3
    // Step 3 : Declare structure variable EncodeInfo enc_info
    // Step 4 : Call the read_and_validate_encode_args(argv,&enc_info)== e_success)
    //  true -> Step 5 , false - > terminate the program
    // Step 5 : Call the do_encoding (&encInfo);
}

OperationType check_operation_type(char *symbol)
{
    // Step 1 : Check whether the symbol is -e or not true - > return e_encode false -> Step 2
    // Step 2 : Check whether the symbol is -d or not true - > return e_decode false -> return e_unsupported
    if(strcmp(symbol,"-e")==0){
        return e_encode;
    }
    else if(strcmp(symbol,"-d")==0){
        return e_decode;
    }
    else{
        return e_unsupported;
    }
}
