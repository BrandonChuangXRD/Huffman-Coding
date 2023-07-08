# Huffman Encoding
A huffman encoder/decoder in C I made for a class at UC Santa Cruz.

## Installation
A makefile is included: run ```make``` with the included flags for the desired outcome:

| Flag | Description |
| ---- | ----------- |
| (None) | Makes all files and binaries |
| all | Makes all files and binaries |
| encode | makes all files relating to encoding |
| decode | makes all files relating to decoding |
| clean | removes all .o files and executable binaries |
| format | formats all .c and .h files using clang-format |

**FUNCTIONALITY**

Encoder encodes text with huffman coding

Decoder decodes text compresse with huffman encoding

**HOW TO MAKE**

run "make" inside the folder to make both executables, and then do ./encoder or ./decoder

## Usage
run ```./encoder``` or ```./decoder``` with the followin flags (both binaries have the same flags):

| Flag | Description |
| ---- | ----------- |
| -i | Specifies the input file (required)|
| -o | Specifies the output file (required)|
| -v | lets you print out the statistics during runtime |

## Known Errors
This huffman encoder actually increases the size of the file, since it uses a stack instead of a priority queue. I may fix this in the future.

## Credits
Many of the pictures in the design document are provided by one of the TA's named Ben Grant.

I will also include this copyright notice, since it was required on my other assignments:
```
BSD 2-Clause License
Copyright (c) 2021, Darrell Long
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```