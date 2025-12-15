Image Steganography using LSB Technique (C)
📌 Project Overview

This project implements Image Steganography using the Least Significant Bit (LSB) technique in C programming.
It enables secure hiding and extraction of secret files inside a 24-bit BMP image without causing perceptible visual distortion.

The project demonstrates low-level data manipulation, bitwise operations, and binary file handling, which are core skills required for embedded systems and systems programming roles.

🎯 Objectives

Hide secret data securely inside an image

Retrieve hidden data reliably using decoding logic

Validate data integrity using a magic string

Practice real-world C programming concepts

🧠 Technical Concepts Used

Bitwise operations (&, |, <<, >>)

Binary file handling (rb, wb)

Structures and modular programming

Command-line argument parsing

Data hiding using LSB manipulation

Memory-efficient encoding/decoding logic

⚙️ Features

Encode secret files into BMP images

Decode and extract hidden files

Magic string verification to ensure valid stego image

Automatic handling of:

Secret file extension

Secret file size

Secret file data

Supports secret file formats:

.txt, .c, .h, .sh

Robust error handling and validation

🗂️ Project Structure
Steganography-LSB/
├── encode.c        // Encoding logic
├── decode.c        // Decoding logic
├── main.c          // Program entry point
├── encode.h
├── decode.h
├── common.h        // Magic string definition
├── types.h         // Custom data types & enums
├── beautiful.bmp  // Input image
├── secret.txt     // Sample secret file
├── README.md
└── .gitignore

🔐 Magic String Concept

A predefined magic string is embedded during encoding to verify whether an image contains hidden data.

#define MAGIC_STRING "#***"


During decoding, the program first extracts and validates this string to prevent incorrect decoding.

🔄 Encoding Workflow

Validate command-line arguments

Open source image, secret file, and destination image

Check image capacity against secret data size

Copy BMP header (54 bytes)

Encode magic string

Encode secret file extension size

Encode secret file extension

Encode secret file size

Encode secret file data

Copy remaining image data unchanged

🔓 Decoding Workflow

Validate command-line arguments

Open stego image and output file

Skip BMP header

Decode and verify magic string

Decode secret file extension size

Decode secret file extension

Decode secret file size

Decode secret file data

Write extracted data to output file

🧪 How to Compile
gcc *.c -o stego

▶️ How to Run
Encoding
./stego -e beautiful.bmp secret.txt destination.bmp

Decoding
./stego -d destination.bmp output.txt

🧾 Sample Output
INFO: Starting encoding process...
INFO: Image has enough capacity for encoding.
INFO: Encoding completed successfully!

🚀 Applications

Secure data transmission

Digital watermarking

Cyber-security fundamentals

Steganography research & learning

Embedded systems skill demonstration

⚠️ Limitations

Supports only BMP images

LSB provides data hiding, not encryption

Susceptible to image processing attacks

🔮 Future Enhancements

AES encryption before embedding

Support for PNG and JPEG images

Password-protected decoding

GUI-based interface

CRC or hash-based integrity check

👨‍💻 Author

vaibhav chavan
Electronics & Telecommunication Engineer.
Embedded system engineer.
Skills: C Programming | Embedded Systems | Data Structures | Bitwise Logic
