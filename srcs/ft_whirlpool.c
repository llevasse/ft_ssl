#include "../include/ft_whirlpool.h"

extern int OPTIONS;

// #define TRACE_INTERMEDIATE_VALUES

int ft_whirlpool(char *arg){
	FT_FILE *f = get_input(arg, "WHIRLPOOL");
	if (!f)
		return 1;
    
	WHIRLPOOL_CONTEXT ctx;
	whirlpool_init(&ctx);
	
	uint64_t value = f->size * 8;
	uint32_t c = 0;
  for (int i = 31; i >= 0 && (c != 0 || value != 0); i--) {
      c += ctx.bit_length[i] + ((uint32_t)value & 0xff);
      ctx.bit_length[i] = (uint8_t)c;
      c >>= 8;
      value >>= 8;
  }
	
	whirlpool_process(&ctx, (uint8_t *)f->content, f->size);
	whirlpool_finalize(&ctx);

  print_digest(arg, ctx.buffer, UINT64, 8, 16);

	if ((arg && !(OPTIONS & OPT_STRING)) || !arg)
	  free(f->content);
	free(f);
	return (0);
}


void whirlpool_init(WHIRLPOOL_CONTEXT *ctx){
  for (int i = 0; i < 8; i++)
    ctx->buffer[i] = 0;
  for (int i = 0; i < 64; i++)
    ctx->input[i] = 0;
    
  for (int i = 0; i < 32; i++)
    ctx->bit_length[i] = 0;
  ctx->size = 0;
}

void whirlpool_process(WHIRLPOOL_CONTEXT *ctx, uint8_t *input, size_t N){
  uint64_t block[8];
  uint32_t offset = ctx->size % 64;
  ctx->size += N;
  
  for (size_t i = 0; i < N; i++){
    ctx->input[offset++] = *(input + i);
    if (offset % 64 == 0){
      for (int j = 0; j < 8; j++){
				block[j] = (uint64_t)(ctx->input[(j * 8)]) << 56 |
				(uint64_t)(ctx->input[(j * 8) + 1]) << 48 |
				(uint64_t)(ctx->input[(j * 8) + 2]) << 40 |
				(uint64_t)(ctx->input[(j * 8) + 3]) << 32 |
				(uint64_t)(ctx->input[(j * 8) + 4]) << 24 |
				(uint64_t)(ctx->input[(j * 8) + 5]) << 16 |
				(uint64_t)(ctx->input[(j * 8) + 6]) <<  8 |
				(uint64_t)(ctx->input[(j * 8) + 7]);
      }
      
      #ifdef TRACE_INTERMEDIATE_VALUES
        printf("The 8x8 matrix Z' derived from the data-string is as follows.\n");
        for (int i = 0 ; i < 8; i++){
          printf("    %02X %02X %02X %02X %02X %02X %02X %02X\n",
              ctx->input[(i * 8) + 0], ctx->input[(i * 8) + 1], ctx->input[(i * 8) + 2], ctx->input[(i * 8) + 3],
              ctx->input[(i * 8) + 4], ctx->input[(i * 8) + 5], ctx->input[(i * 8) + 6], ctx->input[(i * 8) + 7]);
            }
          printf("\n");
      #endif /* ?TRACE_INTERMEDIATE_VALUES */
      
      whirlpool_step(ctx->buffer, block);
      offset = 0;
      
    }
  }
}

uint8_t ORDER[8][8] = {
  {0, 7, 6, 5, 4, 3, 2, 1},
  {1, 0, 7, 6, 5, 4, 3, 2},
  {2, 1, 0, 7, 6, 5, 4, 3},
  {3, 2, 1, 0, 7, 6, 5, 4},
  {4, 3, 2, 1, 0, 7, 6, 5},
  {5, 4, 3, 2, 1, 0, 7, 6},
  {6, 5, 4, 3, 2, 1, 0, 7},
  {7, 6, 5, 4, 3, 2, 1, 0},
};

void whirlpool_step(uint64_t *buffer, uint64_t *input){
  uint64_t K[8], state[8], L[8];
  K[0] = buffer[0];
  K[1] = buffer[1];
  K[2] = buffer[2];
  K[3] = buffer[3];
  K[4] = buffer[4];
  K[5] = buffer[5];
  K[6] = buffer[6];
  K[7] = buffer[7];
  
  state[0] = input[0] ^ K[0];
  state[1] = input[1] ^ K[1];
  state[2] = input[2] ^ K[2];
  state[3] = input[3] ^ K[3];
  state[4] = input[4] ^ K[4];
  state[5] = input[5] ^ K[5];
  state[6] = input[6] ^ K[6];
  state[7] = input[7] ^ K[7];
  
  #ifdef TRACE_INTERMEDIATE_VALUES
    printf("The K_0 matrix (from the initialization value IV) and X'' matrix are as follows.\n");
    for (int i = 0; i < 8; i++) {
        printf(
            "    %02X %02X %02X %02X %02X %02X %02X %02X        %02X %02X %02X %02X %02X %02X %02X %02X\n",
            (uint8_t)(K[i] >> 56),
            (uint8_t)(K[i] >> 48),
            (uint8_t)(K[i] >> 40),
            (uint8_t)(K[i] >> 32),
            (uint8_t)(K[i] >> 24),
            (uint8_t)(K[i] >> 16),
            (uint8_t)(K[i] >>  8),
            (uint8_t)(K[i]      ),

            (uint8_t)(state[i] >> 56),
            (uint8_t)(state[i] >> 48),
            (uint8_t)(state[i] >> 40),
            (uint8_t)(state[i] >> 32),
            (uint8_t)(state[i] >> 24),
            (uint8_t)(state[i] >> 16),
            (uint8_t)(state[i] >>  8),
            (uint8_t)(state[i]      ));
    }
    printf("\n");
    printf("The following are (hexadecimal representations of) the successive values of the variables K_i for i = 1 to 10 and W'.\n");
    printf("\n");
  #endif /* ?TRACE_INTERMEDIATE_VALUES */
  
  for (int i = 1; i <= NB_ROUND; i++){
    L[0] = 
    C0[(int)(K[ORDER[0][0]] >> 56)] ^
    C1[(int)(K[ORDER[0][1]] >> 48) & 0xff] ^
    C2[(int)(K[ORDER[0][2]] >> 40) & 0xff] ^
    C3[(int)(K[ORDER[0][3]] >> 32) & 0xff] ^
    C4[(int)(K[ORDER[0][4]] >> 24) & 0xff] ^
    C5[(int)(K[ORDER[0][5]] >> 16) & 0xff] ^
    C6[(int)(K[ORDER[0][6]] >>  8) & 0xff] ^
    C7[(int)(K[ORDER[0][7]]      ) & 0xff] ^
    rc[i];
    
    for (uint8_t j = 1; j < 8; j++){
      L[j] = C0[(int)(K[ORDER[j][0]] >> 56)] ^
      C1[(int)(K[ORDER[j][1]] >> 48) & 0xff] ^
      C2[(int)(K[ORDER[j][2]] >> 40) & 0xff] ^
      C3[(int)(K[ORDER[j][3]] >> 32) & 0xff] ^
      C4[(int)(K[ORDER[j][4]] >> 24) & 0xff] ^
      C5[(int)(K[ORDER[j][5]] >> 16) & 0xff] ^
      C6[(int)(K[ORDER[j][6]] >>  8) & 0xff] ^
      C7[(int)(K[ORDER[j][7]]      ) & 0xff];
    }
    
    for (uint8_t j = 0; j < 8; j++){
      K[j] = L[j];
    }
    
    for (uint8_t j = 0; j < 8; j++){
      L[j] = C0[(int)(state[ORDER[j][0]] >> 56)] ^
      C1[(int)(state[ORDER[j][1]] >> 48) & 0xff] ^
      C2[(int)(state[ORDER[j][2]] >> 40) & 0xff] ^
      C3[(int)(state[ORDER[j][3]] >> 32) & 0xff] ^
      C4[(int)(state[ORDER[j][4]] >> 24) & 0xff] ^
      C5[(int)(state[ORDER[j][5]] >> 16) & 0xff] ^
      C6[(int)(state[ORDER[j][6]] >>  8) & 0xff] ^
      C7[(int)(state[ORDER[j][7]]      ) & 0xff] ^
      K[j];
    }
    
    for (uint8_t j = 0; j < 8; j++){
      state[j] = L[j];
    }
    #ifdef TRACE_INTERMEDIATE_VALUES
        printf("i = %d:\n", i);
        for (int j = 0; j < 8; j++) {
            printf(
                "    %02X %02X %02X %02X %02X %02X %02X %02X        %02X %02X %02X %02X %02X %02X %02X %02X\n",
                (uint8_t)(K[j] >> 56),
                (uint8_t)(K[j] >> 48),
                (uint8_t)(K[j] >> 40),
                (uint8_t)(K[j] >> 32),
                (uint8_t)(K[j] >> 24),
                (uint8_t)(K[j] >> 16),
                (uint8_t)(K[j] >>  8),
                (uint8_t)(K[j]      ),
    
                (uint8_t)(state[j] >> 56),
                (uint8_t)(state[j] >> 48),
                (uint8_t)(state[j] >> 40),
                (uint8_t)(state[j] >> 32),
                (uint8_t)(state[j] >> 24),
                (uint8_t)(state[j] >> 16),
                (uint8_t)(state[j] >>  8),
                (uint8_t)(state[j]      ));
        }
        printf("\n");
    #endif /* ?TRACE_INTERMEDIATE_VALUES */
  }
  for (uint8_t i = 0; i < 8; i++){
    buffer[i] ^= state[i] ^ input[i];
  }
  #ifdef TRACE_INTERMEDIATE_VALUES
      //printf("Intermediate hash value (after Miyaguchi-Preneel):\n");
      printf("The value of Y' output from the round-function is as follows.\n");
      for (int i = 0; i < 8; i++) {
          printf("    %02X %02X %02X %02X %02X %02X %02X %02X\n",
              (uint8_t)(buffer[i] >> 56),
              (uint8_t)(buffer[i] >> 48),
              (uint8_t)(buffer[i] >> 40),
              (uint8_t)(buffer[i] >> 32),
              (uint8_t)(buffer[i] >> 24),
              (uint8_t)(buffer[i] >> 16),
              (uint8_t)(buffer[i] >>  8),
              (uint8_t)(buffer[i]      ));
      }
      printf("\n");
  #endif /* ?TRACE_INTERMEDIATE_VALUES */
}

void whirlpool_finalize(WHIRLPOOL_CONTEXT *ctx){
  uint64_t offset = ctx->size % 64;
	uint64_t input[8];
  #ifdef TRACE_INTERMEDIATE_VALUES
  // uint64_t padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;
    uint64_t padding_length = 64 - offset;
    printf("\nEnter finalize stage\n");
    printf("offset : %lu | padding length : %lu | ctx size %lu\n", offset, padding_length, ctx->size);
    
    printf("bit len : ");
    for (int i = 0; i < 32; i++){
      printf("%02x", ctx->bit_length[i]);
    }
    printf("\n");
  #endif
	
	for (size_t i = 0; offset < 64; i++){
    ctx->input[offset++] = *(PADDING + i);
  }
	
	
	memcpy(&ctx->input[32], ctx->bit_length, 32);
  // ctx->input[63] = (uint64_t)(ctx->size * 8);
  
  for (int j = 0; j < 8; j++){
    input[j] = (uint64_t)(ctx->input[(j * 8)]) << 56 |
    (uint64_t)(ctx->input[(j * 8) + 1]) << 48 |
    (uint64_t)(ctx->input[(j * 8) + 2]) << 40 |
    (uint64_t)(ctx->input[(j * 8) + 3]) << 32 |
    (uint64_t)(ctx->input[(j * 8) + 4]) << 24 |
    (uint64_t)(ctx->input[(j * 8) + 5]) << 16 |
    (uint64_t)(ctx->input[(j * 8) + 6]) <<  8 |
    (uint64_t)(ctx->input[(j * 8) + 7]);
  }
  
	
	
  #ifdef TRACE_INTERMEDIATE_VALUES
    printf("The 8x8 matrix Z' derived from the data-string is as follows.\n");
    for (int i = 0 ; i < 8; i++){
      // printf("%016X\n", ctx->input[i]);
      printf("    %02X %02X %02X %02X %02X %02X %02X %02X\n",
          ctx->input[(i * 8) + 0], ctx->input[(i * 8) + 1], ctx->input[(i * 8) + 2], ctx->input[(i * 8) + 3],
          ctx->input[(i * 8) + 4], ctx->input[(i * 8) + 5], ctx->input[(i * 8) + 6], ctx->input[(i * 8) + 7]);
    }
    printf("\n");
  #endif /* ?TRACE_INTERMEDIATE_VALUES */
	
	whirlpool_step(ctx->buffer, input);
	// whirlpool_step(ctx->buffer, input);
}
