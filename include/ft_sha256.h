#ifndef FT_SHA256_H
# define FT_SHA256_H
	#include "ft_ssl.h"
 
	typedef	__uint32_t uint32_t;
	typedef	__uint64_t uint64_t;
	typedef	unsigned char uint8_t;
	
 typedef struct sha256_context {
		uint32_t buffer[8];
		uint8_t input[64];
		uint8_t digest[32];
		uint64_t size;
 } SHA256_CONTEXT;

	#define A 0x6a09e667
	#define B 0xbb67ae85
	#define C 0x3c6ef372
	#define D 0xa54ff53a
	#define E 0x510e527f
	#define F 0x9b05688c
	#define G 0x1f83d9ab
	#define H 0x5be0cd19
	
	#define ROTATE_RIGHT(x, n) (((x) >> (n)) | ((x) << (32-(n))))
	
	
	#define SIGMA(n, x, y, z) (ROTATE_RIGHT(n, x) ^ ROTATE_RIGHT(n, y) ^ (n >> z))
	#define S(n, x, y, z) (ROTATE_RIGHT(n, x) ^ ROTATE_RIGHT(n, y) ^ ROTATE_RIGHT(n, z))
	#define CHOICE(e,f,g) (((e) & (f)) ^ ((~e) & (g)))
	#define MAJORITY(a,b,c) (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))
	
	
	uint32_t K[] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
	
	
  static uint8_t PADDING[] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	void sha256_init(SHA256_CONTEXT *ctx);
  void sha256_process(SHA256_CONTEXT *ctx, uint8_t *input, size_t N);
  void sha256_step(uint32_t *buffer, uint32_t *input);
  void sha256_finalize(SHA256_CONTEXT *ctx);
  
	
#endif
