### Tiny ECDH / ECC in C

This is a small and portable implementation of the [Elliptic-Curve Diffie-Hellman key agreement algorithm](https://en.wikipedia.org/wiki/Elliptic-curve_Diffie%E2%80%93Hellman) written in C.

Description from Wikipedia:

> Elliptic-curve Diffie–Hellman (ECDH) is an anonymous key agreement protocol that allows two parties, each having an elliptic-curve public–private key pair, to establish a shared secret over an insecure channel. This shared secret may be directly used as a key, or to derive another key. The key, or the derived key, can then be used to encrypt subsequent communications using a symmetric-key cipher. It is a variant of the Diffie–Hellman protocol using elliptic-curve cryptography.
`

You can choose between 10 standard NIST curves of different sizes. 5 pseudo-random curves and 5 Koblitz curves providing from 80 to 256 bits symmetrically equivalent security.  See [ecdh.h](https://github.com/kokke/tiny-ECDH-c/blob/master/ecdh.h) for clarification.

You can define the macro `ECDH_COFACTOR_VARIANT` in [ecdh.c](https://github.com/kokke/tiny-ECDH-c/blob/master/ecdh.c) to enable the [co-factor variant of ECDH](https://crypto.stackexchange.com/questions/18222/difference-between-ecdh-with-cofactor-key-and-ecdh-without-cofactor-key) for safe non-ephemeral use.


The API is very simple and looks like this (I am using C99 `<stdint.h>`-style annotated types):

```C
/* NOTE: assumes private is filled with random data before calling */
int ecdh_generate_keys(uint8_t* public, uint8_t* private);

/* input: own private key + other party's public key, output: shared secret */
int ecdh_shared_secret(const uint8_t* private, const uint8_t* others_pub, uint8_t* output);
```

You need to provide random data for the private key yourself, for the key generation process.
See [ecdh_example.c](https://github.com/kokke/tiny-ECDH-c/blob/master/ecdh_example.c) for an example of how to use the library.

There is no built-in protection from out-of-bounds memory access errors as a result of malicious input. The two functions ecdh_shared_secret() and ecdh_generate_keys() expect inputs of a certain length. See [ecdh.h](https://github.com/kokke/tiny-ECDH-c/blob/master/ecdh.h) for clarification.


The module allocates no RAM internally and takes up 2.1K ROM when compiled for ARM (1.4K for Thumb but YMMV).
A keypair requires between 72 and 216 bytes of RAM depending on which curve you choose (i.e. how big the underlying base field is).


It is the smallest implementation of ECC in C I've seen yet, but do contact me if you know of something smaller (or have improvements to the code here). 


```C
$ arm-none-eabi-gcc -Os -c ecdh.c -mthumb
$ size ecdh.o
   text    data     bss     dec     hex filename
   1413       0       0    1413     585 ecdh.o
```

I am using the Free Software Foundation, ARM GCC compiler:

    $ arm-none-eabi-gcc --version
    arm-none-eabi-gcc (4.8.4-1+11-1) 4.8.4 20141219 (release)
    Copyright (C) 2013 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


All material in this repository is in the public domain.


### TODO:

- Implement ECDSA signing and verifification.
