#include "../include/ft_ssl.h"

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

#define R1(a,b,c,d,k,s,i) a = b + ROTATE_LEFT((a + F(b,c,d) + X[k] + sins[i]), s)
#define R2(a,b,c,d,k,s,i) a = b + ROTATE_LEFT((a + G(b,c,d) + X[k] + sins[i]), s)
#define R3(a,b,c,d,k,s,i) a = b + ROTATE_LEFT((a + H(b,c,d) + X[k] + sins[i]), s)
#define R4(a,b,c,d,k,s,i) a = b + ROTATE_LEFT((a + I(b,c,d) + X[k] + sins[i]), s)

int sins[64] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

size_t md5_get_padding_len(void *mess);
void *md5_padding(void *mess, size_t final_len);
void *md5_process(unsigned char *mess, size_t N);
void md5_round_one();
void md5_round_two();
void md5_round_three();
void md5_round_four();
void md5_encode(unsigned char *output, unsigned long int *input, unsigned int len);


int ft_md5(char *input_path){
	char *input = 0x0;
	ssize_t len = 0;
	
	if (input_path){
		if ((input = read_file(input_path)) == 0x0)
			return (1);
//		printf("MD5(%s)= ", input_path);
	}
	else{
		if ((input = read_stdin()) == 0x0)
			return (1);
//		printf("MD5(stdin)= ");
	}
	len = md5_get_padding_len(input);
	unsigned char *padded;
	if ((padded = md5_padding(input, len)) == 0x0){
		free(input);
		fprintf(stderr, "Error while padding message input file : %s\n", strerror(errno));
		return (1);
	}
	md5_process(padded, len);
	printf("%zu ", A);
	printf("%zu ", B);
	printf("%zu ", C);
	printf("%zu\n", D);
	unsigned char digest[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	md5_encode(digest, buffers, 16);
	for (int i = 0; i < 16; i++)
 	printf ("%02x", digest[i]);
 free(padded);
	return (0);
}

size_t md5_get_padding_len(void *mess){
	size_t len = strlen(mess);
	
	unsigned int tmp = ceil((float)len / 512);
	if (len > ((512 * tmp) - 64))
		tmp++;
	return ((tmp * 512) + 1);
}

void *md5_padding(void *mess, size_t final_len){
	void *new = malloc(sizeof(*mess) * (final_len + 1));
	if (!new)
		return (0x0);
	memset(new, 0, (final_len) + 1);
	strcpy(new, mess);
	free(mess);
	return (new);
}

void *md5_process(unsigned char *mess, size_t N){
 unsigned long int save_buf[4] = {0,0,0,0};
 
 #define AA save_buf[0]
 #define BB save_buf[1]
 #define CC save_buf[2]
 #define DD save_buf[3]
	for (size_t i = 0; i < (N / 16); i++){
		for (size_t j = 0; j < 16; j++){
			//printf("%zu\n", (i * 16) + j);
			X[j] = mess[(i * 16) + j];
		}
		AA = A;
		BB = B;
		CC = C;
		DD = D;
		md5_round_one();
		md5_round_two();
		md5_round_three();
		md5_round_four();
		A += AA;
		B += BB;
		C += CC;
		D += DD;
	}
	return (mess);
}

void md5_round_one(){
	R1(A, B, C, D, 0,  7, 1);
	R1(D, A, B, C, 1, 12, 2);
	R1(C, D, A, B, 2, 17, 3);
	R1(B, C, D, A, 3, 22, 4);
	
	R1(A, B, C, D, 4,  7, 5);
	R1(D, A, B, C, 5, 12, 6);
	R1(C, D, A, B, 6, 17, 7);
	R1(B, C, D, A, 7, 22, 8);
	
	R1(A, B, C, D, 8,  7, 9);
	R1(D, A, B, C, 9, 12, 0);
	R1(C, D, A, B, 0, 17, 1);
	R1(B, C, D, A, 1, 22, 2);
	
	R1(A, B, C, D, 2,  7, 3);
	R1(D, A, B, C, 3, 12, 4);
	R1(C, D, A, B, 4, 17, 5);
	R1(B, C, D, A, 5, 22, 6);
}

void md5_round_two(){ 
 R2(A, B, C, D,  1,  5, 17);
 R2(D, A, B, C,  6,  9, 18);
 R2(C, D, A, B, 11, 14, 19);
 R2(B, C, D, A,  0, 20, 20);
	
 R2(A, B, C, D,  5,  5, 21);
 R2(D, A, B, C, 10,  9, 22);
 R2(C, D, A, B, 15, 14, 23);
 R2(B, C, D, A,  4, 20, 24);
	
 R2(A, B, C, D,  9,  5, 25);
 R2(D, A, B, C, 14,  9, 26);
 R2(C, D, A, B,  3, 14, 27);
 R2(B, C, D, A,  8, 20, 28);
	
 R2(A, B, C, D, 13,  5, 29);
 R2(D, A, B, C,  2,  9, 30);
 R2(C, D, A, B,  7, 14, 31);
 R2(B, C, D, A, 12, 20, 32);
}

void md5_round_three(){
 
	R3(A, B, C, D,  5,  4, 33);
	R3(D, A, B, C,  8, 11, 34);
	R3(C, D, A, B, 11, 16, 35);
	R3(B, C, D, A, 14, 23, 36);

	R3(A, B, C, D,  1,  4, 37);
	R3(D, A, B, C,  4, 11, 38);
	R3(C, D, A, B,  7, 16, 39);
	R3(B, C, D, A, 10, 23, 40);

	R3(A, B, C, D, 13,  4, 41);
	R3(D, A, B, C,  0, 11, 42);
	R3(C, D, A, B,  3, 16, 43);
	R3(B, C, D, A,  6, 23, 44);

	R3(A, B, C, D,  9,  4, 45);
	R3(D, A, B, C, 12, 11, 46);
	R3(C, D, A, B, 15, 16, 47);
	R3(B, C, D, A,  2, 23, 48);
}

void md5_round_four(){
	R4(A, B, C, D,  0,  6, 49);
	R4(D, A, B, C,  7, 10, 50);
	R4(C, D, A, B, 14, 15, 51);
	R4(B, C, D, A,  5, 21, 52);
	
	R4(A, B, C, D, 12,  6, 53);
	R4(D, A, B, C,  3, 10, 54);
	R4(C, D, A, B, 10, 15, 55);
	R4(B, C, D, A,  1, 21, 56);
	
	R4(A, B, C, D,  8,  6, 57);
	R4(D, A, B, C, 15, 10, 58);
	R4(C, D, A, B,  6, 15, 59);
	R4(B, C, D, A, 13, 21, 60);
	
	R4(A, B, C, D,  4,  6, 61);
	R4(D, A, B, C, 11, 10, 62);
	R4(C, D, A, B,  2, 15, 63);
	R4(B, C, D, A,  9, 21, 64);
}

void md5_encode(unsigned char *output, unsigned long int *input, unsigned int len){
	unsigned int i, j;
	
 for (i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
 }
}

// /* Store state in digest */
// Encode (digest, context->state, 16);

// static void Encode (output, input, len)
// unsigned char *output;
// UINT4 *input;
// unsigned int len;
// {
//   unsigned int i, j;

//   for (i = 0, j = 0; j < len; i++, j += 4) {
//  output[j] = (unsigned char)(input[i] & 0xff);
//  output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
//  output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
//  output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
//   }
// }