# 🧠 LSB Steganography C Implementation

## 📘 Overview
This project implements *LSB (Least Significant Bit) Image Steganography* using the *C programming language*.  
It allows you to *hide secret data (like text files)* within *24-bit BMP images* without visibly altering the image.

The project demonstrates practical applications of *file handling, **bitwise operations, and **data security concepts*, showcasing clean modular C design.

---

## ✨ Features
- 🔐 Encode (hide) any text file inside a 24-bit BMP image.  
- 🔓 Decode (extract) hidden data back from a stego image.  
- 🧩 Supports 24-bit uncompressed BMP format.  
- ⚙️ Validates file formats and ensures image capacity.  
- 🧠 Maintains original image quality post-encoding.  
- 🪶 Provides detailed logs during encoding and decoding processes.  

---

## ⚙️ How to Compile and Run

### 1️⃣ Clone the Repository
bash
git clone https://github.com/yourusername/LSB-Steganography-C-Implementation.git


### 2️⃣ Navigate to the Project Directory
bash
cd LSB-Steganography-C-Implementation


### 3️⃣ Build the Project
bash
make


### 4️⃣ Encode Mode (Hide Secret Data)
bash
./a.out -e <source_image.bmp> <secret.txt> <stego_image.bmp>

*Example:*
bash
./a.out -e sample/input.bmp sample/secret.txt sample/stego.bmp


### 5️⃣ Decode Mode (Extract Hidden Data)
bash
./a.out -d <stego_image.bmp> <output.txt>

*Example:*
bash
./a.out -d sample/stego.bmp sample/output.txt


---

## 📁 Project Structure

LSB-Steganography-C-Implementation/
│
├── main.c
├── encode.c
├── decode.c
├── types.h
├── encode.h
├── decode.h
├── Makefile
├── README.md
└── sample/
    ├── input.bmp
    ├── secret.txt
    └── stego.bmp


---

## 🧮 Concept Behind LSB Steganography
*LSB (Least Significant Bit)* steganography hides secret information in the *least significant bits of image pixels*.  
Because these bits contribute the least to visual appearance, the change remains invisible to the human eye.


Original Pixel:  10010110
Modified Pixel:  10010111  ← (1 secret bit embedded)


This project demonstrates *bitwise data embedding and extraction*, ensuring accurate recovery of hidden files.

---

## 🔮 Future Enhancements
- Add *encryption layer* before embedding secret data.  
- Extend support to *PNG/JPEG* image formats.  
- Create a *GUI interface* using C++ or Python.  
- Add *audio/video steganography* support.

---

## 👨‍💻 Author
*# 🧠 LSB Steganography C Implementation

## 📘 Overview
This project implements *LSB (Least Significant Bit) Image Steganography* using the *C programming language*.  
It allows you to *hide secret data (like text files)* within *24-bit BMP images* without visibly altering the image.

The project demonstrates practical applications of *file handling, **bitwise operations, and **data security concepts*, showcasing clean modular C design.

---

## ✨ Features
- 🔐 Encode (hide) any text file inside a 24-bit BMP image.  
- 🔓 Decode (extract) hidden data back from a stego image.  
- 🧩 Supports 24-bit uncompressed BMP format.  
- ⚙️ Validates file formats and ensures image capacity.  
- 🧠 Maintains original image quality post-encoding.  
- 🪶 Provides detailed logs during encoding and decoding processes.  

---

## ⚙️ How to Compile and Run

### 1️⃣ Clone the Repository
bash
git clone https://github.com/yourusername/LSB-Steganography-C-Implementation.git


### 2️⃣ Navigate to the Project Directory
bash
cd LSB-Steganography-C-Implementation


### 3️⃣ Build the Project
bash
make


### 4️⃣ Encode Mode (Hide Secret Data)
bash
./a.out -e <source_image.bmp> <secret.txt> <stego_image.bmp>

*Example:*
bash
./a.out -e sample/input.bmp sample/secret.txt sample/stego.bmp


### 5️⃣ Decode Mode (Extract Hidden Data)
bash
./a.out -d <stego_image.bmp> <output.txt>

*Example:*
bash
./a.out -d sample/stego.bmp sample/output.txt


---

## 📁 Project Structure

LSB-Steganography-C-Implementation/
│
├── main.c
├── encode.c
├── decode.c
├── types.h
├── encode.h
├── decode.h
├── Makefile
├── README.md
└── sample/
    ├── input.bmp
    ├── secret.txt
    └── stego.bmp


---

## 🧮 Concept Behind LSB Steganography
*LSB (Least Significant Bit)* steganography hides secret information in the *least significant bits of image pixels*.  
Because these bits contribute the least to visual appearance, the change remains invisible to the human eye.


Original Pixel:  10010110
Modified Pixel:  10010111  ← (1 secret bit embedded)


This project demonstrates *bitwise data embedding and extraction*, ensuring accurate recovery of hidden files.

---

## 🔮 Future Enhancements
- Add *encryption layer* before embedding secret data.  
- Extend support to *PNG/JPEG* image formats.  
- Create a *GUI interface* using C++ or Python.  
- Add *audio/video steganography* support.

---

## 👨‍💻 Author
*Vaibhav Chavan*  
Electronics and Telecommunication Engineer   
---

### ⭐ If you find this project useful, consider giving it a star on GitHub!*  
Electronics and Telecommunication Engineer  
📫 [https://www.linkedin.com/in/nakul-vadar-368780243?utm_source=share&utm_campaign=share_via&utm_content=profile&utm_medium=android_app]

---

### ⭐ If you find this project useful, consider giving it a star on GitHub!
