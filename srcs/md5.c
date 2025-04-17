#include "../include/ft_ssl.h"
#include "../include/ft_md5.h"

extern int OPTIONS;

int ft_md5(char *arg){
  FT_FILE *f = get_input(arg, "MD5");
	if (!f)
    return 1;
	
	MD5_CONTEXT ctx;
	md5_init(&ctx);
	md5_process(&ctx, (uint8_t *)f->content, f->size);
	md5_finalize(&ctx);
	
	for (int i = 0; i < 16; i++)
		printf ("%02x", ctx.digest[i]);
	if (OPTIONS & OPT_REVERSE && !(OPTIONS & OPT_QUIET)){
    if ((OPTIONS & OPT_STRING))
      printf(" \"%s\"", arg);
    else
      printf(" %s", arg);
	}
	printf("\n");

  if ((arg && !(OPTIONS & OPT_STRING)) || !arg)
    free(f->content);
  free(f);
	return (0);
}

void md5_init(MD5_CONTEXT *ctx){
	ctx->size = 0;
	ctx->buffer[0] = A;
	ctx->buffer[1] = B;
	ctx->buffer[2] = C;
	ctx->buffer[3] = D;
}

void *md5_process(MD5_CONTEXT *ctx, uint8_t *mess, size_t N){
	uint32_t offset = ctx->size % 64;
	uint32_t input[16];
	ctx->size += N;

	
	for (uint32_t i = 0; i < N; i++){
		ctx->input[offset++] = *(mess + i);
		if (offset % 64 == 0){
			for(uint32_t j = 0; j < 16; ++j){
				input[j] = (uint32_t)(ctx->input[(j * 4) + 3]) << 24 | (uint32_t)(ctx->input[(j * 4) + 2]) << 16 | (uint32_t)(ctx->input[(j * 4) + 1]) <<  8 | (uint32_t)(ctx->input[(j * 4)]);
			}
			md5_step(ctx->buffer, input);
			offset = 0;
		}
	}
	return (mess);
}

void md5_step(uint32_t *buffer, uint32_t *input){
	uint32_t AA = buffer[0];
	uint32_t BB = buffer[1];
	uint32_t CC = buffer[2];
	uint32_t DD = buffer[3];
	uint32_t E, j;
	
	for(uint32_t i = 0; i < 64; ++i){
		switch(i / 16){
			case 0:
				E = F(BB, CC, DD);
				j = i;
				break;
			case 1:
				E = G(BB, CC, DD);
				j = ((i * 5) + 1) % 16;
				break;
			case 2:
				E = H(BB, CC, DD);
				j = ((i * 3) + 5) % 16;
				break;
			default:
				E = I(BB, CC, DD);
				j = (i * 7) % 16;
			break;
		}

		uint32_t temp = DD;
		DD = CC;
		CC = BB;
		BB = BB + ROTATE_LEFT(AA + E + K[i] + input[j], S[i]);
		AA = temp;
		
	}
	
	buffer[0] += AA;
	buffer[1] += BB;
	buffer[2] += CC;
	buffer[3] += DD;
}

void md5_finalize(MD5_CONTEXT *ctx){
	uint32_t offset = ctx->size % 64;
	uint32_t input[16];
 uint32_t padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;
	md5_process(ctx, PADDING, padding_length);
	ctx->size -= (uint64_t)padding_length;
	for(uint32_t j = 0; j < 14; ++j){
		input[j] = (uint32_t)(ctx->input[(j * 4) + 3]) << 24 |
									(uint32_t)(ctx->input[(j * 4) + 2]) << 16 |
									(uint32_t)(ctx->input[(j * 4) + 1]) <<  8 |
									(uint32_t)(ctx->input[(j * 4)]);
	}
	input[14] = (uint32_t)ctx->size * 8;
	input[15] = (uint32_t)((ctx->size * 8) >> 32);
	
	md5_step(ctx->buffer, input);
	md5_buffer_to_digest(ctx->buffer, ctx->digest);
}

void md5_buffer_to_digest(uint32_t *buff, uint8_t *digest){
	for(uint32_t i = 0; i < 4; ++i){
		digest[(i * 4) + 0] = (uint8_t)((buff[i] & 0x000000FF));
		digest[(i * 4) + 1] = (uint8_t)((buff[i] & 0x0000FF00) >>  8);
		digest[(i * 4) + 2] = (uint8_t)((buff[i] & 0x00FF0000) >> 16);
		digest[(i * 4) + 3] = (uint8_t)((buff[i] & 0xFF000000) >> 24);
	}
}