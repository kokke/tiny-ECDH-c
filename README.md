### Tiny ECDH in C

This is a small and portable implementation of the [Elliptic-Curve Diffie-Hellman key agreement algorithm](https://en.wikipedia.org/wiki/Elliptic-curve_Diffie%E2%80%93Hellman) written in C.
It is epspecially usable for generating ephemeral session key and encrypting communication symmetrically after agreement.

You can choose between 10 standard NIST curves of different sizes. 5 "pseudo-random" curves and 5 Koblitz curves providing from ~80 to ~256 bits symmetrically equivalent security.  See [ecdh.h](https://github.com/kokke/tiny-ECDH-c/blob/master/ecdh.h) for clarification.

The API is very simple and looks like this (I am using C99 `<stdint.h>`-style annotated types):

```C
/* NOTE: assumes private is filled with random data before calling */
int ecdh_generate_keys(uint8_t* public, uint8_t* private);

int ecdh_shared_secret(const uint8_t* private, const uint8_t* other_pub, uint8_t* output);
```

You need to provide random data for the private key yourself, for the key generation process.
See [ecdh_example.c](https://github.com/kokke/tiny-ECDH-c/blob/master/ecdh_example.c) for an example of how to use the library.


There is no built-in error checking or protection from out-of-bounds memory access errors as a result of malicious input. The two functions ecdh_shared_secret() and ecdh_generate_keys() expect inputs of a certain length. See [ecdh.h](https://github.com/kokke/tiny-ECDH-c/blob/master/ecdh.h) for clarification.

The module allocates no RAM internally and 2.1K ROM when compiled for ARM (1.4K for Thumb but YMMV).
A keypair requires between 63 and 216 bytes of RAM repending on which curve you choose (how big the underlying base field is).

It is one of the smallest implementation in C I've seen yet, but do contact me if you know of something smaller (or have improvements to the code here). 

I am using the Free Software Foundation, ARM GCC compiler:

    $ arm-none-eabi-gcc --version
    arm-none-eabi-gcc (4.8.4-1+11-1) 4.8.4 20141219 (release)
    Copyright (C) 2013 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


All material in this repository is in the public domain.
