#ifndef FT_MD5_H
# define FT_MD5_H
 #ifndef size_t
		typedef unsigned long size_t;
 #endif
	unsigned long int buffers[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325467};
	unsigned char X[16];

	#define A buffers[0]
	#define B buffers[1]
	#define C buffers[2]
	#define D buffers[3]
	#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
	#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
	#define H(x, y, z) ((x) ^ (y) ^ (z))
	#define I(x, y, z) ((y) ^ ((x) | (~z)))
	#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

	#define R1(a,b,c,d,k,s,i) a = b + ROTATE_LEFT((a + F(b,c,d) + X[k] + i), s)
	#define R2(a,b,c,d,k,s,i) a = b + ROTATE_LEFT((a + G(b,c,d) + X[k] + i), s)
	#define R3(a,b,c,d,k,s,i) a = b + ROTATE_LEFT((a + H(b,c,d) + X[k] + i), s)
	#define R4(a,b,c,d,k,s,i) a = b + ROTATE_LEFT((a + I(b,c,d) + X[k] + i), s)

	size_t md5_get_padding_len(void *mess);
	void *md5_padding(void *mess, size_t final_len);
	void *md5_process(unsigned char *mess, size_t N);
	void md5_round_one();
	void md5_round_two();
	void md5_round_three();
	void md5_round_four();
	void md5_encode(unsigned char *output, unsigned long int *input, unsigned int len);
#endif
