#include "../include/ft_sha256.h"

extern int OPTIONS;

void printBinary(uint32_t num) {
  for (int i = sizeof(uint32_t) * 8 - 1; i >= 0; i--) {
      printf("%d", (num >> i) & 1);
  }
  printf("\n");
}

int ft_sha256(char *arg){
  FT_FILE *f = malloc(sizeof(*f));
  if (!f)
    return 0;
	
	if (arg){
		if (OPTIONS & OPT_STRING){
			f->content = arg;
		}
		else if ((read_file(arg, f)) == 0x0){
      free(f);
			return (1);
		}
		printf("SHA256(%s)= ", arg);
	}
	else{
		if ((read_stdin(f)) == 0x0){
      free(f);
			return (1);
		}
		printf("SHA256(stdin)= ");
	}	
	
	SHA256_CONTEXT ctx;
	sha256_init(&ctx);
	sha256_process(&ctx, (uint8_t *)f->content, f->size);
	sha256_finalize(&ctx);
	for (uint32_t i = 0; i < 8; i++){
    printf("%08x", ctx.buffer[i]);
	}
	printf("\n");
  if ((arg && !(OPTIONS & OPT_STRING)) || !arg)
    free(f->content);
  free(f);
	return (0);
}


void sha256_init(SHA256_CONTEXT *ctx){
	ctx->size = 0;
	ctx->buffer[0] = A;
	ctx->buffer[1] = B;
	ctx->buffer[2] = C;
	ctx->buffer[3] = D;
	ctx->buffer[4] = E;
	ctx->buffer[5] = F;
	ctx->buffer[6] = G;
	ctx->buffer[7] = H;
}

void sha256_process(SHA256_CONTEXT *ctx, uint8_t *input, size_t N){
	uint32_t offset = ctx->size % 64;
	uint32_t chunk[64];
	ctx->size += N;
	
	for (uint32_t i = 0; i < N; i++){
		ctx->input[offset++] = *(input + i);
		if (offset % 64 == 0){
      for (uint32_t j = 0; j < 16; j++){
				chunk[j] = (uint32_t)(ctx->input[(j * 4)]) << 24 |
				(uint32_t)(ctx->input[(j * 4) + 1]) << 16 |
				(uint32_t)(ctx->input[(j * 4) + 2]) <<  8 |
				(uint32_t)(ctx->input[(j * 4) + 3]);
        // printf("w%2u : ", i);
        // printBinary(input[j]);
        // printf("\n");
      }
			sha256_step(ctx->buffer, chunk);
			offset = 0;
		}
	}
}

void sha256_step(uint32_t *buffer, uint32_t *input){
  for (uint32_t i = 16; i < 64; i++){
    input[i] = input[i-16] + SIGMA(input[i-15], 7, 18, 3) + input[i-7] + SIGMA(input[i-2], 17, 19, 10);
    // printf("w%2u \n", i);
    // printf("SIGMA 1 : \n");
    // printBinary(input[i-15]);
    // printBinary(ROTATE_RIGHT(input[i-15], 7));
    // printBinary(ROTATE_RIGHT(input[i-15], 18));
    // printBinary(input[i-15] >> 3);
    // printBinary(SIGMA(input[i-15], 7, 18, 3));
    // printf("SIGMA 2 : \n");
    // printBinary(input[i-2]);
    // printBinary(ROTATE_RIGHT(input[i-2], 17));
    // printBinary(ROTATE_RIGHT(input[i-2], 19));
    // printBinary(input[i-2] >> 10);
    // printBinary(SIGMA(input[i-2], 17, 19, 10));
  }
  uint32_t tmp1, tmp2, a,b,c,d,e,f,g,h;
  
  a = buffer[0];
  b = buffer[1];
  c = buffer[2];
  d = buffer[3];
  e = buffer[4];
  f = buffer[5];
  g = buffer[6];
  h = buffer[7];
  // printf("\n");
  for (uint32_t i = 0; i < 64; i++){
    
    
    tmp1 = h + S(e, 6, 11, 25) + CHOICE(e, f, g) + K[i] + input[i];
    tmp2 = S(a, 2, 13, 22) + MAJORITY(a, b, c);
    // printf("w%2u ", i);
    // printBinary(input[i]);
  
    // printf("w%u\n", i);
    // printf("a(%8x) : ", a);
    // printBinary(a);
    // // printf("b(%x) : ", b);
    // // printBinary(b);
    // // printf("c(%x) : ", c);
    // // printBinary(c);
    // // printf("d(%x) : ", d);
    // // printBinary(d);
    // printf("e(%8x) : ", e);
    // printBinary(e);
    // // printf("f(%x) : ", f);
    // // printBinary(f);
    // // printf("g(%x) : ", g);
    // // printBinary(g);
    // // printf("h(%x) : ", h);
    // // printBinary(h);
    // // printf("tmp1 : ");
    // // printBinary(tmp1);
    // // printf("tmp2 : ");
    // // printBinary(tmp2);
    h = g;
    g = f;
    f = e;
    e = d + tmp1;
    d = c;
    c = b;
    b = a;
    a = tmp1 + tmp2;
    
    
    
    // printf("\n");
  }
  buffer[0] += a;
  buffer[1] += b;
  buffer[2] += c;
  buffer[3] += d;
  buffer[4] += e;
  buffer[5] += f;
  buffer[6] += g;
  buffer[7] += h;
}


void sha256_finalize(SHA256_CONTEXT *ctx){
	uint32_t offset = ctx->size % 64;
	uint32_t input[64];
  uint32_t padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;
	sha256_process(ctx, PADDING, padding_length);
	ctx->size -= (uint64_t)padding_length;
	for(uint32_t j = 0; j < 14; ++j){
    input[j] = (uint32_t)(ctx->input[(j * 4)]) << 24 |
      (uint32_t)(ctx->input[(j * 4) + 1]) << 16 |
      (uint32_t)(ctx->input[(j * 4) + 2]) <<  8 |
      (uint32_t)(ctx->input[(j * 4) + 3]);
    
    // printf("w%2u : ", j);
    // printBinary(input[j]);
	}
	input[14] = (uint32_t)((ctx->size * 8) >> 32);
	input[15] = (uint32_t)(ctx->size * 8);
  // printf("w%2u : ", 14);
  // printBinary(input[14]);
  // printf("w%2u : ", 15);
  // printBinary(input[15]);
	
	sha256_step(ctx->buffer, input);
}