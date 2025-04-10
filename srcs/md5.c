#include "../include/ft_ssl.h"
#include "../include/ft_md5.h"

int ft_md5(char *input_path){
	char *input = 0x0;
	size_t len = 0;
	
	if (input_path){
		if ((input = read_file(input_path)) == 0x0)
			return (1);
		printf("MD5(%s)= ", input_path);
	}
	else{
		if ((input = read_stdin()) == 0x0)
			return (1);
		printf("MD5(stdin)= ");
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
		//printf("%zu\n", i);
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
	R1(A, B, C, D, 0,  7, 0xd76aa478);
	R1(D, A, B, C, 1, 12, 0xe8c7b756);
	R1(C, D, A, B, 2, 17, 0x242070db);
	R1(B, C, D, A, 3, 22, 0xc1bdceee);
	
	R1(A, B, C, D, 4,  7, 0xf57c0faf);
	R1(D, A, B, C, 5, 12, 0x4787c62a);
	R1(C, D, A, B, 6, 17, 0xa8304613);
	R1(B, C, D, A, 7, 22, 0xfd469501);
	
	R1(A, B, C, D, 8,  7, 0x698098d8);
	R1(D, A, B, C, 9, 12, 0x8b44f7af);
	R1(C, D, A, B, 0, 17, 0xffff5bb1);
	R1(B, C, D, A, 1, 22, 0x895cd7be);
	
	R1(A, B, C, D, 2,  7, 0x6b901122);
	R1(D, A, B, C, 3, 12, 0xfd987193);
	R1(C, D, A, B, 4, 17, 0xa679438e);
	R1(B, C, D, A, 5, 22, 0x49b40821);
}

void md5_round_two(){ 
 R2(A, B, C, D,  1,  5, 0xf61e2562);
 R2(D, A, B, C,  6,  9, 0xc040b340);
 R2(C, D, A, B, 11, 14, 0x265e5a51);
 R2(B, C, D, A,  0, 20, 0xe9b6c7aa);
	
 R2(A, B, C, D,  5,  5, 0xd62f105d);
 R2(D, A, B, C, 10,  9, 0x2441453);
 R2(C, D, A, B, 15, 14, 0xd8a1e681);
 R2(B, C, D, A,  4, 20, 0xe7d3fbc8);
	
 R2(A, B, C, D,  9,  5, 0x21e1cde6);
 R2(D, A, B, C, 14,  9, 0xc33707d6);
 R2(C, D, A, B,  3, 14, 0xf4d50d87);
 R2(B, C, D, A,  8, 20, 0x455a14ed);
	
 R2(A, B, C, D, 13,  5, 0xa9e3e905);
 R2(D, A, B, C,  2,  9, 0xfcefa3f8);
 R2(C, D, A, B,  7, 14, 0x676f02d9);
 R2(B, C, D, A, 12, 20, 0x8d2a4c8a);
}

void md5_round_three(){
 
	R3(A, B, C, D,  5,  4, 0xfffa3942);
	R3(D, A, B, C,  8, 11, 0x8771f681);
	R3(C, D, A, B, 11, 16, 0x6d9d6122);
	R3(B, C, D, A, 14, 23, 0xfde5380c);

	R3(A, B, C, D,  1,  4, 0xa4beea44);
	R3(D, A, B, C,  4, 11, 0x4bdecfa9);
	R3(C, D, A, B,  7, 16, 0xf6bb4b60);
	R3(B, C, D, A, 10, 23, 0xbebfbc70);

	R3(A, B, C, D, 13,  4, 0x289b7ec6);
	R3(D, A, B, C,  0, 11, 0xeaa127fa);
	R3(C, D, A, B,  3, 16, 0xd4ef3085);
	R3(B, C, D, A,  6, 23, 0x4881d05);

	R3(A, B, C, D,  9,  4, 0xd9d4d039);
	R3(D, A, B, C, 12, 11, 0xe6db99e5);
	R3(C, D, A, B, 15, 16, 0x1fa27cf8);
	R3(B, C, D, A,  2, 23, 0xc4ac5665);
}

void md5_round_four(){
	R4(A, B, C, D,  0,  6, 0xf4292244);
	R4(D, A, B, C,  7, 10, 0x432aff97);
	R4(C, D, A, B, 14, 15, 0xab9423a7);
	R4(B, C, D, A,  5, 21, 0xfc93a039);
	
	R4(A, B, C, D, 12,  6, 0x655b59c3);
	R4(D, A, B, C,  3, 10, 0x8f0ccc92);
	R4(C, D, A, B, 10, 15, 0xffeff47d);
	R4(B, C, D, A,  1, 21, 0x85845dd1);
	
	R4(A, B, C, D,  8,  6, 0x6fa87e4f);
	R4(D, A, B, C, 15, 10, 0xfe2ce6e0);
	R4(C, D, A, B,  6, 15, 0xa3014314);
	R4(B, C, D, A, 13, 21, 0x4e0811a1);
	
	R4(A, B, C, D,  4,  6, 0xf7537e82);
	R4(D, A, B, C, 11, 10, 0xbd3af235);
	R4(C, D, A, B,  2, 15, 0x2ad7d2bb);
	R4(B, C, D, A,  9, 21, 0xeb86d391);
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