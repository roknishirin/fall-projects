# Assignment 5: Public Key Cryptography

This assignment creates three programs called keygen, encrypt and decrypt. Keygen is a program that is in charge of key generation, producing RSA public and private key pairs. Security requires keeping the private key private, and the public key can be distributed widely. Any person can encrypt a message using the intended receiver’s public key, but that encrypted message can only be decrypted with receivers private key. The encrypt program will encrypt files using a public key. The decrypt program will decrypt the encrypted files using the corresponding private key which consists of the public modulus and public exponent. 

## File Descriptions

decrypt.c : This contains the implementation and main() function for the decrypt program.

encrypt.c : This contains the implementation and main() function for the encrypt program.

keygen.c : This contains the implementation and main() function for the keygen program.

numtheory.c : This contains the implementation of the number theory functions.

numtheory.h : This specifies the interface for the number theory functions.

randstate.c : This contains the implementation of the random state interface for the RSA library and number theory functions.

randstate.h : This specifies the interface for initializing and clearing the random state.

rsa.c : This contains the implementation of the RSA library.

rsa.h : This specifies the interface for the RSA library.

README.md : This file contains instructions on how to build and run decrypt, encrypt and keygen.

DESIGN.pdf : This file describes the purpose of the program and communicates the overall design of the program.

WRITEUP.pdf : This file contains a discussion of how the various programs and files can be tested.

Makefile: This file is responsible for doing the compilation process for all of the programs (keygen, encrypt, and decrypt).

## Building

Run ```make keygen``` to build the keygen.c program.  
Run ```make encrypt``` to build the encrypt.c program.  
Run ```make decrypt``` to build the decrypt.c program.  
Run ```make``` to build the keygen.c, encrypt.c, and decrypt.c programs.  
Run ```make clean``` to remove any compiler-generated files.  

## Running Keygen

```./keygen [-s seed] [-b bits] [-i iters] [-n pbfile] [-d pvfile] [-vh]```  

flag options:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-s <seed>```   : Use <seed> as the random number seed. Default: time()  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-b <bits>```   : Public modulus n must have at least <bits> bits. Default: 1024  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-i <iters>```  : Run <iters> Miller-Rabin iterations for primality testing. Default: 50  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-n <pbfile>``` : Public key file is <pbfile>. Default: rsa.pub  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-d <pvfile>``` : Private key file is <pvfile>. Default: rsa.priv  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-v```          : Enable verbose output.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-h```          : Display program synopsis and usage.  



## Running Encrypt 

```./encrypt [-i infile] [-o outfile] [-n keyfile] [-vh]```  

flag options:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-i <infile>``` : Read input from <infile>. Default: standard input.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-o <outfile>```: Write output to <outfile>. Default: standard output.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-n <keyfile>```: Public key is in <keyfile>. Default: rsa.pub.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-v```          : Enable verbose output.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-h```          : Display program synopsis and usage.  



## Running Decrypt 

```./decrypt [-i infile] [-o outfile] [-n keyfile] [-vh]```  

flag options:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-i <infile>``` : Read input from <infile>. Default: standard input.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-o <outfile>```: Write output to <outfile>. Default: standard output.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-n <keyfile>```: Private key is in <keyfile>. Default: rsa.priv.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-v```          : Enable verbose output.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-h```          : Display program synopsis and usage.  



