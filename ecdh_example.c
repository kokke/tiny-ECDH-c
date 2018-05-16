/*
  Diffie-Hellman key exchange (without HMAC) aka ECDH_anon in RFC4492


  1. Alice picks a (secret) random natural number 'a', calculates P = a * G and sends P to Bob.
     'a' is Alice's private key. 
     'P' is Alice's public key.

  2. Bob picks a (secret) random natural number 'b', calculates Q = b * G and sends Q to Alice.
     'b' is Bob's private key.
     'Q' is Bob's public key.

  3. Alice calculates S = a * Q = a * (b * G).

  4. Bob calculates T = b * P = b * (a * G).

  .. which are the same two values since multiplication in the field is commutative and associative.

  T = S = the new shared secret.


  Pseudo-random number generator inspired / stolen from: http://burtleburtle.net/bob/rand/smallprng.html

*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ecdh.h"



/* pseudo random number generator with 128 bit internal state... probably not suited for cryptographical usage */
typedef struct
{
  uint32_t a;
  uint32_t b;
  uint32_t c;
  uint32_t d;
} prng_t;

static prng_t prng_ctx;

static uint32_t prng_rotate(uint32_t x, uint32_t k)
{
  return (x << k) | (x >> (32 - k)); 
}

static uint32_t prng_next(void)
{
  uint32_t e = prng_ctx.a - prng_rotate(prng_ctx.b, 27); 
  prng_ctx.a = prng_ctx.b ^ prng_rotate(prng_ctx.c, 17); 
  prng_ctx.b = prng_ctx.c + prng_ctx.d;
  prng_ctx.c = prng_ctx.d + e; 
  prng_ctx.d = e + prng_ctx.a;
  return prng_ctx.d;
}

static void prng_init(uint32_t seed)
{
  uint32_t i;
  prng_ctx.a = 0xf1ea5eed;
  prng_ctx.b = prng_ctx.c = prng_ctx.d = seed;

  for (i = 0; i < 31; ++i) 
  {
    (void) prng_next();
  }
}





static void ecdh_demo(void)
{
  static uint8_t puba[ECC_PUB_KEY_SIZE];
  static uint8_t prva[ECC_PRV_KEY_SIZE];
  static uint8_t seca[ECC_PUB_KEY_SIZE];
  static uint8_t pubb[ECC_PUB_KEY_SIZE];
  static uint8_t prvb[ECC_PRV_KEY_SIZE];
  static uint8_t secb[ECC_PUB_KEY_SIZE];
  uint32_t i;

  /* 0. Initialize and seed random number generator */
  static int initialized = 0;
  if (!initialized)
  {
    prng_init((0xbad ^ 0xc0ffee ^ 42) | 0xcafebabe | 666);
    initialized = 1;
  }

  /* 1. Alice picks a (secret) random natural number 'a', calculates P = a * g and sends P to Bob. */
  for (i = 0; i < ECC_PRV_KEY_SIZE; ++i)
  {
    prva[i] = prng_next();
  }
  assert(ecdh_generate_keys(puba, prva));

  /* 2. Bob picks a (secret) random natural number 'b', calculates Q = b * g and sends Q to Alice. */
  for (i = 0; i < ECC_PRV_KEY_SIZE; ++i)
  {
    prvb[i] = prng_next();
  }
  assert(ecdh_generate_keys(pubb, prvb));

  /* 3. Alice calculates S = a * Q = a * (b * g). */
  assert(ecdh_shared_secret(prva, pubb, seca));

  /* 4. Bob calculates T = b * P = b * (a * g). */
  assert(ecdh_shared_secret(prvb, puba, secb));

  /* 5. Assert equality, i.e. check that both parties calculated the same value. */
  for (i = 0; i < ECC_PUB_KEY_SIZE; ++i)
  {
    assert(seca[i] == secb[i]);
  }
}


/* WARNING: This is not working correctly. ECDSA is not working... */
void ecdsa_broken()
{
  static uint8_t  prv[ECC_PRV_KEY_SIZE];
  static uint8_t  pub[ECC_PUB_KEY_SIZE];
  static uint8_t  msg[ECC_PRV_KEY_SIZE];
  static uint8_t  signature[ECC_PUB_KEY_SIZE];
  static uint8_t  k[ECC_PRV_KEY_SIZE];
  uint32_t i;

  srand(time(0));
  srand(42);

  for (i = 0; i < ECC_PRV_KEY_SIZE; ++i)
  {
    prv[i] = rand();
    msg[i] = prv[i] ^ rand();
    k[i] = rand();
  }

/* int ecdsa_sign(const uint8_t* private, const uint8_t* hash, uint8_t* random_k, uint8_t* signature);
   int ecdsa_verify(const uint8_t* public, const uint8_t* hash, uint8_t* signature);                          */

  ecdh_generate_keys(pub, prv);
  /* No asserts - ECDSA functionality is broken... */
  ecdsa_sign((const uint8_t*)prv, msg, k, signature);
  ecdsa_verify((const uint8_t*)pub, msg, (const uint8_t*)signature); /* fails... */
}



int main(int argc, char* argv[])
{
  int i;
  int ncycles = 1;

  if (argc > 1)
  {
    ncycles = atoi(argv[1]);
  }

  for (i = 0; i < ncycles; ++i)
  {
    ecdh_demo();
    ecdsa_broken();
  }

  return 0;
}


